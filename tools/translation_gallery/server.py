#!/usr/bin/env python3
"""Local-only, read-only art review with separately scoped durable style decisions."""
from __future__ import annotations
import argparse
import csv
import hashlib
import hmac
import io
import json
import os
from pathlib import Path
import re
import secrets
import stat
import struct
import threading
import time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import parse_qs, urlsplit

SHA = re.compile(r"[0-9a-f]{64}\Z")
CATALOG = '.asset-cache/media/nbn-prologue-published-v4-verified/visual-catalog.json'
CATALOG_SHA = 'd155a2958e7e8694a9b5444412ef71843a0229b15503e713e911a9c1b5f8b4d6'
INDEX = '.asset-cache/translation-images/batch-manifests.jsonl'
MAX_IMAGE = 16 * 1024 * 1024

class Invalid(ValueError):
    pass

def digest(data):
    return hashlib.sha256(data).hexdigest()

def encoded(value):
    return json.dumps(value, ensure_ascii=False, sort_keys=True, separators=(',', ':')).encode()

def parts(relative):
    if not isinstance(relative, str) or not relative or len(relative) > 2048 or '\\' in relative or any(ord(c) < 32 for c in relative):
        raise Invalid('invalid relative path')
    value = relative.split('/')
    if any(x in ('', '.', '..') for x in value):
        raise Invalid('path must stay inside its declared root')
    return value

class Root:
    """Open each component relative to retained directory FDs, without symlinks."""
    def __init__(self, path):
        path = Path(path)
        if not path.is_absolute() or '..' in path.parts:
            raise Invalid('root must be an absolute path')
        for p in reversed([path, *path.parents]):
            if p.is_symlink():
                raise Invalid('symlink root is unsupported')
        self.path = path
        self.fd = os.open(path, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW)
    def close(self):
        os.close(self.fd)
    def open(self, relative, flags=os.O_RDONLY, mode=0o600):
        names = parts(relative)
        directory = os.dup(self.fd)
        try:
            for name in names[:-1]:
                nxt = os.open(name, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW, dir_fd=directory)
                os.close(directory)
                directory = nxt
            fd = os.open(names[-1], flags | os.O_NOFOLLOW | os.O_NONBLOCK, mode, dir_fd=directory)
            if not stat.S_ISREG(os.fstat(fd).st_mode):
                os.close(fd)
                raise Invalid('regular files only')
            if flags & (os.O_WRONLY | os.O_RDWR) and os.fstat(fd).st_nlink != 1:
                os.close(fd)
                raise Invalid('hard-linked writable files are unsupported')
            return fd
        finally:
            os.close(directory)
    def read(self, relative, limit=8 * 1024 * 1024):
        fd = self.open(relative)
        with os.fdopen(fd, 'rb') as stream:
            if os.fstat(stream.fileno()).st_size > limit:
                raise Invalid('file exceeds bounded read size')
            data = stream.read(limit + 1)
            if len(data) > limit:
                raise Invalid('file grew beyond bounded read size')
            return data

def object_json(data):
    def unique(items):
        result = {}
        for key, value in items:
            if key in result:
                raise Invalid('duplicate JSON key')
            result[key] = value
        return result
    try:
        return json.loads(data, object_pairs_hook=unique, parse_constant=lambda _: (_ for _ in ()).throw(Invalid('nonfinite JSON')))
    except (ValueError, UnicodeError) as exc:
        raise Invalid('invalid JSON') from exc

def integer(value, lo=0, hi=8192):
    if isinstance(value, bool) or not isinstance(value, (int, float)) or int(value) != value or not lo <= value <= hi:
        raise Invalid('bounded integer required')
    return int(value)

def image_descriptor(path, sha, dimensions):
    if not parts(path)[0] == '.asset-cache' or not isinstance(sha, str) or not SHA.fullmatch(sha):
        raise Invalid('images must be pinned files below .asset-cache')
    if not isinstance(dimensions, list) or len(dimensions) != 2:
        raise Invalid('two image dimensions required')
    return {'path': path, 'sha256': sha, 'dimensions': [integer(x, 1) for x in dimensions]}

def read_image(root, descriptor):
    data = root.read(descriptor['path'], MAX_IMAGE)
    if digest(data) != descriptor['sha256']:
        raise Invalid('image hash changed; publish a new manifest')
    if data[:8] != b'\x89PNG\r\n\x1a\n' or len(data) < 33 or data[12:16] != b'IHDR' or list(struct.unpack('>II', data[16:24])) != descriptor['dimensions']:
        raise Invalid('PNG dimensions or header differ')
    return data

def compact_catalog(root, catalog=CATALOG, expected=CATALOG_SHA):
    raw = root.read(catalog, 160 * 1024 * 1024)
    if not SHA.fullmatch(expected) or digest(raw) != expected:
        raise Invalid('catalog differs from explicit reviewed pin')
    source = object_json(raw)
    if source.get('schema') != 'tokimemo.visual-text.v1' or not isinstance(source.get('regions'), list):
        raise Invalid('unsupported visual catalog')
    records = []
    seen = set()
    for row in source['regions']:
        rid = row['region_id']
        if not isinstance(rid, str) or rid in seen:
            raise Invalid('duplicate or invalid region ID')
        seen.add(rid)
        region = row['region']
        image = image_descriptor(row['png']['path'], row['png']['sha256'], [region['width'], region['height']])
        records.append({'id': rid, 'kind': 'region', 'source_container': row['source']['path'],
            'frame': row.get('frame'), 'palette': row.get('palette_index'),
            'variant': str(row.get('original_metadata', {}).get('variant', 'unknown')),
            'context': row.get('screen_context', {}).get('contexts', []), 'classification': row.get('state', 'unknown'),
            'original': image, 'dimensions': image['dimensions']})
    unknown = []
    for row in source.get('unrepresented_sources', []):
        identity = row['parent_asset_id']
        unknown.append({'id': 'unknown:' + identity, 'kind': 'unknown', 'source_container': row['source']['path'],
            'frame': None, 'palette': None, 'variant': 'unknown', 'context': row.get('source_profile', {}).get('screen_context', ['unknown']),
            'classification': 'unrepresented_source', 'probable_image': bool(row.get('probable_image_suffix')), 'original': None,
            'reason': row.get('reason', 'source coverage unknown'), 'dimensions': None})
    return {'schema': 'tokimemo.translation-review-inventory.v1', 'source_catalog': catalog,
        'source_catalog_sha256': expected, 'counts': source['counts'], 'coverage': {k: source[k] for k in
        ['corpus_complete', 'graphic_source_coverage_complete', 'japanese_text_detection_complete']},
        'records': records + unknown}

class Decisions:
    """Append-only JSONL via a held directory FD. HTTP never supplies a filename."""
    def __init__(self, directory):
        self.root = Root(directory)
        self.lock = threading.Lock()
        fd = self.root.open('style-decisions.jsonl', os.O_CREAT | os.O_APPEND | os.O_WRONLY)
        os.close(fd)
    def history(self):
        raw = self.root.read('style-decisions.jsonl', 64 * 1024 * 1024)
        result = []
        for line in raw.splitlines():
            row = object_json(line)
            if not isinstance(row, dict) or row.get('category') != 'style':
                raise Invalid('durable review history malformed; restore it before writing')
            result.append(row)
        return result
    def append(self, row):
        with self.lock:
            self.history()  # Never append behind corrupt/truncated history.
            data = encoded(row) + b'\n'
            if len(data) > 16000:
                raise Invalid('decision record too large')
            fd = self.root.open('style-decisions.jsonl', os.O_APPEND | os.O_WRONLY)
            try:
                if os.fstat(fd).st_size + len(data) > 64 * 1024 * 1024:
                    raise Invalid('decision log full')
                written = os.write(fd, data)
                if written != len(data):
                    raise Invalid('short durable write')
                os.fsync(fd)
            finally:
                os.close(fd)

class Gallery:
    def __init__(self, root, inventory, index, decisions):
        self.root, self.index, self.decisions = root, index, decisions
        self.inventory = object_json(root.read(inventory, 16 * 1024 * 1024))
        if self.inventory.get('schema') != 'tokimemo.translation-review-inventory.v1':
            raise Invalid('unsupported compact inventory')
        self.media = {}
        self.lock = threading.RLock()
    def drafts(self):
        raw = self.root.read(self.index, 1024 * 1024)
        batch_index = {}
        for line in raw.splitlines():
            row = object_json(line)
            if not isinstance(row, dict) or not isinstance(row.get('batch_id'), str) or not isinstance(row.get('sha256'), str) or not SHA.fullmatch(row['sha256']):
                raise Invalid('invalid batch index record')
            previous = batch_index.get(row['batch_id'])
            if previous and row.get('supersedes_sha256') != previous['sha256']:
                raise Invalid('duplicate batch requires explicit supersedes_sha256')
            batch_index[row['batch_id']] = row
        if len(batch_index) > 1000:
            raise Invalid('batch index too large')
        all_rows = {}
        for batch, entry in batch_index.items():
            rel = '.asset-cache/translation-images/' + entry['manifest']
            parts(rel)
            data = self.root.read(rel)
            if digest(data) != entry['sha256']:
                raise Invalid('batch manifest hash differs: ' + batch)
            manifest = object_json(data)
            if not isinstance(manifest.get('assets'), list):
                raise Invalid('batch assets must be an array')
            parent = str(Path(rel).parent)
            for asset in manifest['assets']:
                ident = asset.get('id')
                if not isinstance(ident, str) or not re.fullmatch(r'[A-Za-z0-9_.:-]{1,160}', ident):
                    raise Invalid('invalid translation asset ID')
                if 'draft:' + ident in all_rows:
                    raise Invalid('duplicate active asset ID; revise within its original batch')
                original = image_descriptor(parent + '/' + asset['source'], asset['source_sha256'], asset['dimensions'])
                output = image_descriptor(parent + '/' + asset['output'], asset['output_sha256'], asset['dimensions'])
                rid = 'draft:' + ident
                revision = digest(encoded([rid, original['sha256'], output['sha256']]))
                all_rows[rid] = {'id': rid, 'label': ident, 'kind': 'draft', 'batch': batch,
                    'manifest_sha256': entry['sha256'],
                    'source_container': asset.get('source_container', 'unmapped original container'),
                    'frame': asset.get('frame'), 'palette': asset.get('palette_id', 'unknown'), 'variant': asset.get('variant', 'unknown'),
                    'context': asset.get('context', ['unmapped']), 'classification': 'translated_draft',
                    'original': original, 'output': output, 'dimensions': original['dimensions'],
                    'revision': revision, 'catalog_region_ids': asset.get('catalog_region_ids', []),
                    'wording': asset.get('wording', asset.get('approved_wording', [])),
                    'technical': 'separate review; not inferred from style', 'runtime': 'not accepted by this gallery'}
        return list(all_rows.values())
    def snapshot(self):
        with self.lock:
            drafts = self.drafts()
            regions = {r['id']: r for r in self.inventory['records'] if r['kind'] == 'region'}
            mapped = set()
            histories = self.decisions.history()
            latest = {row['asset_id']: row for row in histories}
            for row in drafts:
                good_links = []
                for rid in row['catalog_region_ids']:
                    original = regions.get(rid)
                    if original and original['original']['sha256'] == row['original']['sha256'] and original['frame'] == row['frame'] and original['source_container'] == row['source_container']:
                        good_links.append(rid)
                row['mapped_regions'] = good_links
                mapped.update(good_links)
                row['mapping'] = 'verified original hash/frame/container' if good_links else 'unmapped'
                errors = []
                for role in ['original', 'output']:
                    try:
                        read_image(self.root, row[role])
                    except (Invalid, OSError) as exc:
                        errors.append(str(exc))
                row['image_error'] = '; '.join(errors)
                prior = latest.get(row['id'])
                row['style'] = prior['decision'] if prior and prior['revision'] == row['revision'] and not errors else ('stale' if prior else 'unreviewed')
                row['notes'] = prior.get('notes', '') if prior else ''
                row['status'] = row['style']
                row['reviewable'] = not errors
            pending = []
            for original in self.inventory['records']:
                if original['id'] in mapped:
                    continue
                row = dict(original)
                row.update({'label': row['source_container'], 'batch': '', 'reviewable': False, 'style': 'not translated', 'notes': ''})
                row['status'] = 'unknown' if row['kind'] == 'unknown' else ('untranslated' if row['classification'] == 'confirmed_japanese_text' else ('needs_triage' if row['classification'] == 'suspected' else 'no_translation_assigned'))
                pending.append(row)
            rows = drafts + pending
            media = {}
            for row in rows:
                for role in ['original', 'output']:
                    descriptor = row.get(role)
                    if descriptor:
                        token = digest(encoded([descriptor['path'], descriptor['sha256']]))
                        media[token] = descriptor
                        row[role + '_url'] = '/media/' + token
            self.media = media
            counts = self.inventory['counts']
            summary = {'drafts': len(drafts), 'style_approved': sum(x['style'] == 'approve' for x in drafts),
                'changes_requested': sum(x['style'] == 'request_changes' for x in drafts), 'stale': sum(x['style'] == 'stale' for x in drafts),
                'decoded_regions': counts['regions'], 'known_japanese': counts['reviews']['confirmed_japanese_text'],
                'mapped_japanese': sum(regions[x]['classification'] == 'confirmed_japanese_text' for x in mapped),
                'suspected': counts['reviews']['suspected'], 'unrepresented_sources': counts['unrepresented_sources'],
                'probable_images_unrepresented': counts['probable_image_sources_unrepresented'],
                'unresolved_graphics_sources': counts['unreviewed_unresolved_graphics_sources']}
            batches = [{'id': batch, 'drafts': sum(x['batch'] == batch for x in drafts), 'style_approved': sum(x['batch'] == batch and x['style'] == 'approve' for x in drafts)} for batch in sorted({x['batch'] for x in drafts})]
            return {'items': rows, 'summary': summary, 'batches': batches, 'coverage': self.inventory['coverage'],
                'source_catalog_sha256': self.inventory['source_catalog_sha256'], 'review_category': 'style'}
    def decide(self, body, actor='local-reviewer'):
        if not isinstance(body, dict) or set(body) != {'asset_id', 'revision', 'decision', 'notes'}:
            raise Invalid('exact decision fields required')
        if body['decision'] not in ['approve', 'request_changes', 'defer'] or not isinstance(body['notes'], str) or len(body['notes']) > 4000 or '\x00' in body['notes']:
            raise Invalid('invalid style decision or notes')
        with self.lock:
            row = next((r for r in self.snapshot()['items'] if r['id'] == body['asset_id']), None)
            if not row or not row['reviewable'] or row.get('revision') != body['revision']:
                raise Invalid('asset changed or has no validated original/translation pair; reload')
            event = {**body, 'category': 'style', 'source_sha256': row['original']['sha256'], 'output_sha256': row['output']['sha256'],
                'recorded_utc': time.strftime('%Y-%m-%dT%H:%M:%SZ', time.gmtime()), 'actor': actor, 'event_id': secrets.token_hex(16)}
            self.decisions.append(event)
            return event

class Handler(BaseHTTPRequestHandler):
    server_version = 'LocalArtReview/1'
    def log_message(self, *_):
        pass
    def allowed(self, write=False):
        host = self.headers.get('Host', '')
        expected = {'127.0.0.1:' + str(self.server.port), 'localhost:' + str(self.server.port)}
        if host not in expected:
            raise Invalid('localhost Host required')
        origin = self.headers.get('Origin')
        if origin and origin not in {'http://' + x for x in expected}:
            raise Invalid('same-origin requests only')
        if write and (not origin or self.headers.get('Content-Type') != 'application/json' or not hmac.compare_digest(self.headers.get('X-Review-Token', ''), self.server.token)):
            raise Invalid('same-origin JSON and review token required')
    def respond(self, code, body, content_type='application/json; charset=utf-8'):
        if not isinstance(body, bytes):
            body = encoded(body)
        self.send_response(code)
        self.send_header('Content-Type', content_type)
        self.send_header('Content-Length', str(len(body)))
        self.send_header('Cache-Control', 'no-store')
        self.send_header('X-Content-Type-Options', 'nosniff')
        self.send_header('Content-Security-Policy', "default-src 'self'; script-src 'self'; style-src 'self'; img-src 'self'; connect-src 'self'; frame-ancestors 'none'; base-uri 'none'; form-action 'none'")
        self.send_header('Referrer-Policy', 'no-referrer')
        self.end_headers()
        self.wfile.write(body)
    def do_GET(self):
        try:
            self.allowed()
            url = urlsplit(self.path)
            if url.path in ['/', '/app.js', '/style.css']:
                name = {'/': 'index.html', '/app.js': 'app.js', '/style.css': 'style.css'}[url.path]
                content = (self.server.static / name).read_bytes()
                self.respond(200, content, {'index.html': 'text/html; charset=utf-8', 'app.js': 'text/javascript; charset=utf-8', 'style.css': 'text/css; charset=utf-8'}[name])
            elif url.path == '/api/catalog':
                result = self.server.gallery.snapshot()
                q = parse_qs(url.query)
                rows = result.pop('items')
                status = q.get('status', [''])[0]
                if status == 'reviewed':
                    rows = [r for r in rows if r['kind'] == 'draft' and r['style'] in ['approve', 'request_changes', 'defer']]
                elif status == 'unreviewed':
                    rows = [r for r in rows if r['kind'] == 'draft' and r['style'] in ['unreviewed', 'stale']]
                elif status:
                    rows = [r for r in rows if str(r.get('status', '')) == status]
                for field in ['batch', 'classification', 'kind']:
                    if q.get(field, [''])[0]: rows = [r for r in rows if str(r.get(field, '')) == q[field][0]]
                term = q.get('q', [''])[0].casefold()[:200]
                if term: rows = [r for r in rows if term in ' '.join(str(r.get(k, '')) for k in ['id', 'label', 'source_container', 'frame', 'palette', 'variant', 'context', 'wording']).casefold()]
                page = int(q.get('page', ['1'])[0]);page = max(1, min(page, 10000))
                result.update({'items': rows[(page-1)*24:page*24], 'total': len(rows), 'page': page, 'page_size': 24, 'review_token': self.server.token})
                self.respond(200, result)
            elif url.path.startswith('/media/') and re.fullmatch(r'/media/[0-9a-f]{64}', url.path):
                with self.server.gallery.lock:
                    descriptor = self.server.gallery.media.get(url.path[7:])
                    if not descriptor: raise Invalid('unknown image reference; reload catalog')
                    self.respond(200, read_image(self.server.gallery.root, descriptor), 'image/png')
            elif url.path == '/api/export.csv':
                rows = self.server.gallery.snapshot()['items']
                stream = io.StringIO();writer = csv.writer(stream)
                writer.writerow(['asset_id', 'source_container', 'frame', 'palette', 'style', 'notes', 'source_sha256', 'output_sha256', 'runtime'])
                for row in rows:
                    if row['kind'] != 'draft': continue
                    values = [row['id'], row['source_container'], row['frame'], row['palette'], row['style'], row['notes'], row['original']['sha256'], row['output']['sha256'], row['runtime']]
                    writer.writerow(["'" + str(v) if str(v).lstrip().startswith(('=', '+', '-', '@')) else v for v in values])
                self.respond(200, stream.getvalue().encode(), 'text/csv; charset=utf-8')
            else:
                self.respond(404, {'error': 'not found'})
        except (Invalid, OSError, ValueError, KeyError, TypeError) as exc:
            self.respond(409, {'error': str(exc)})
    def do_POST(self):
        try:
            self.allowed(write=True)
            if self.path != '/api/reviews':
                self.respond(404, {'error': 'not found'});return
            if self.headers.get('Transfer-Encoding'):
                raise Invalid('chunked writes unsupported')
            length = int(self.headers.get('Content-Length', '-1'))
            if not 0 <= length <= 16000:
                raise Invalid('bounded JSON body required')
            result = self.server.gallery.decide(object_json(self.rfile.read(length)))
            self.respond(200, result)
        except (Invalid, OSError, ValueError, KeyError, TypeError) as exc:
            self.respond(409, {'error': str(exc)})

class ReviewServer(ThreadingHTTPServer):
    daemon_threads = True
    def __init__(self, address, gallery, static, public_port):
        self.gallery, self.static, self.port = gallery, Path(static), public_port
        self.token = secrets.token_urlsafe(32)
        super().__init__(address, Handler)
    def get_request(self):
        sock, address = super().get_request()
        sock.settimeout(10)
        return sock, address

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest='command', required=True)
    build = sub.add_parser('build')
    build.add_argument('--repo-root', required=True);build.add_argument('--catalog', default=CATALOG);build.add_argument('--catalog-sha256', default=CATALOG_SHA);build.add_argument('--output', required=True)
    serve = sub.add_parser('serve')
    serve.add_argument('--repo-root', required=True);serve.add_argument('--inventory', required=True);serve.add_argument('--index', default=INDEX);serve.add_argument('--state-root', required=True);serve.add_argument('--bind', default='127.0.0.1');serve.add_argument('--port', type=int, default=8766)
    args = parser.parse_args()
    root = Root(args.repo_root)
    if args.command == 'build':
        data = encoded(compact_catalog(root, args.catalog, args.catalog_sha256))
        out = Path(args.output)
        if not out.is_absolute() or not out.parent.is_dir() or out.is_symlink(): raise Invalid('explicit regular output in existing owned directory required')
        fd = os.open(out, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW, 0o600)
        with os.fdopen(fd, 'wb') as stream:stream.write(data)
        print(json.dumps({'output': str(out), 'sha256': digest(data), 'bytes': len(data)}))
    else:
        if args.bind not in ['127.0.0.1', '0.0.0.0'] or not 1024 <= args.port <= 65535: raise Invalid('unsupported local bind/port')
        decisions = Decisions(args.state_root)
        gallery = Gallery(root, args.inventory, args.index, decisions)
        gallery.snapshot()
        server = ReviewServer((args.bind, args.port), gallery, Path(__file__).parent / 'static', args.port)
        print('Local translation review: http://127.0.0.1:' + str(args.port), flush=True)
        server.serve_forever()

if __name__ == '__main__':
    main()
