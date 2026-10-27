"""T-29029: admission from a code-reviewed, read-only coordinator registry.

The registry is the trust anchor, not a producer's labels or pass assertions.
It ships alongside this module; incoming files/CLI options cannot replace it.
Changing tool code or this registry is an operator action requiring review.
This does not authenticate an operator who can alter the trusted code bundle.
"""
from __future__ import annotations

import hashlib
import json
import os
import re
import stat
from pathlib import Path, PurePosixPath

import observer_evidence

REGISTRY = Path(__file__).with_name('reviewed_routes.json')
MAX_METADATA = 32 * 1024 * 1024
HASH = re.compile(r'^[a-f0-9]{64}$')
VIEWPORT = 'app.get_viewport().get_texture().get_image()'
INPUT_API = 'Input.parse_input_event + Input.flush_buffered_events'


def require(ok: bool, message: str) -> None:
    if not ok:
        raise ValueError('capture admission: ' + message)


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def relative(value: object) -> str:
    require(isinstance(value, str) and bool(value) and '\\' not in value, 'invalid evidence path')
    path = PurePosixPath(value)
    require(not path.is_absolute() and '..' not in path.parts and value == path.as_posix(), 'unsafe evidence path')
    return value


def checked_bytes(path: Path, limit: int) -> bytes:
    require(path.is_absolute() and '..' not in path.parts, 'absolute physical path required')
    require(not any(part.is_symlink() for part in [path, *path.parents]), 'symlink evidence is forbidden')
    before = path.lstat()
    require(stat.S_ISREG(before.st_mode) and 0 < before.st_size <= limit, 'evidence size/type differs')
    fd = os.open(path, os.O_RDONLY | getattr(os, 'O_NOFOLLOW', 0))
    signature = lambda info: (info.st_dev, info.st_ino, info.st_size, info.st_mtime_ns, info.st_ctime_ns)
    with os.fdopen(fd, 'rb') as source:
        require(signature(os.fstat(source.fileno())) == signature(before), 'evidence changed before read')
        data = source.read(limit + 1)
        require(signature(os.fstat(source.fileno())) == signature(before), 'evidence changed during read')
    require(len(data) == before.st_size and signature(path.lstat()) == signature(before), 'evidence changed after read')
    return data


def object_json(data: bytes) -> dict:
    def pairs(items):
        result = {}
        for key, value in items:
            require(key not in result, 'duplicate JSON key')
            result[key] = value
        return result
    def invalid_constant(_value):
        raise ValueError('capture admission: non-finite JSON value')
    result = json.loads(data, object_pairs_hook=pairs, parse_constant=invalid_constant)
    require(isinstance(result, dict), 'JSON evidence must be an object')
    return result


def registry_revision() -> str:
    """Wake previously-refused stable files when the operator updates trust."""
    return digest(checked_bytes(REGISTRY, 256 * 1024))


class Admission:
    """Verify exact reviewed bytes and actual-input/capture cross-bindings."""
    def __init__(self, evidence_root: Path | None = None):
        self.root = evidence_root or Path(os.environ.get('PORT_PROGRESS_EVIDENCE_ROOT', '/evidence'))
        raw = checked_bytes(REGISTRY, 256 * 1024)
        self.registry_sha = digest(raw)
        policy = object_json(raw)
        require(policy.get('schema') == 'tokimemo.progress-admission.v1' and
                policy.get('purpose') == 'actual_gameplay_progress', 'unsupported trust registry')
        require(isinstance(policy.get('routes'), list) and isinstance(policy.get('denied'), list), 'malformed trust registry')
        self.denied = set()
        for item in policy['denied']:
            require(isinstance(item, dict) and isinstance(item.get('sha256'), str) and HASH.fullmatch(item['sha256']), 'invalid denied hash')
            require(item['sha256'] not in self.denied, 'duplicate denied hash')
            self.denied.add(item['sha256'])
        self.captures = {}
        route_ids = set()
        for route in policy['routes']:
            require(isinstance(route, dict) and isinstance(route.get('id'), str) and route['id'] not in route_ids, 'duplicate/invalid reviewed route')
            route_ids.add(route['id'])
            relative(route.get('directory'))
            for key in ['producer_sha256', 'route_sha256', 'artifacts_sha256']:
                require(isinstance(route.get(key), str) and HASH.fullmatch(route[key]), 'invalid reviewed evidence hash')
            require(isinstance(route.get('reviews'), list) and bool(route['reviews']), 'coordinator review evidence missing')
            require(isinstance(route.get('captures'), list) and bool(route['captures']), 'reviewed captures missing')
            for capture in route['captures']:
                require(isinstance(capture, dict) and isinstance(capture.get('sha256'), str) and HASH.fullmatch(capture['sha256']), 'invalid capture hash')
                ident = capture['sha256']
                require(ident not in self.captures and ident not in self.denied, 'duplicate or excluded reviewed capture')
                relative(capture.get('image'))
                require(isinstance(capture.get('receipt_sha256'), str) and HASH.fullmatch(capture['receipt_sha256']), 'invalid capture receipt hash')
                require(type(capture.get('bytes')) is int and capture['bytes'] > 0 and isinstance(capture.get('capture_id'), str), 'invalid capture identity')
                self.captures[ident] = (route, capture)
        # Observer captures are a separate trusted type. They never enter the
        # gameplay route table and cannot satisfy a route's progress checks.
        self.observer_captures = {}
        self.observers = {}
        observer_records = policy.get('observer_captures', [])
        require(isinstance(observer_records, list), 'observer capture registry is malformed')
        for observer in observer_records:
            require(isinstance(observer, dict) and observer.get('type') == 'first_arrival_observer_v1'
                    and isinstance(observer.get('id'), str) and observer['id'] not in self.observers,
                    'invalid observer registry record')
            relative(observer.get('directory'))
            expected = {
                'application_sha256': observer_evidence.APPLICATION_SHA256,
                'producer_sha256': observer_evidence.FIRST_SCENE_TEST_SHA256,
                'captured_production_tree_sha256': observer_evidence.PRODUCTION_TREE_SHA256,
                'packet_manifest_sha256': observer_evidence.BGM_PACKET_MANIFEST_SHA256,
                'status_sha256': observer_evidence.STATUS_SHA256,
                'screenshot_sha256': observer_evidence.SCREEN_SHA256,
                'audio_sha256': observer_evidence.WAV_SHA256,
            }
            for key, pinned in expected.items():
                require(observer.get(key) == pinned, 'observer registry pin differs: ' + key)
            capture = observer.get('screenshot')
            require(isinstance(capture, dict) and capture.get('path') == 'gui-v1/first-scene.png'
                    and capture.get('sha256') == observer_evidence.SCREEN_SHA256
                    and type(capture.get('bytes')) is int and capture['bytes'] == 99899
                    and capture.get('dimensions') == [1024, 768],
                    'observer screenshot capture record differs')
            audio = observer.get('audio')
            require(isinstance(audio, dict) and audio.get('path') == 'gui-v1/first-scene-mix.wav'
                    and audio.get('sha256') == observer_evidence.WAV_SHA256
                    and audio.get('bytes') == 917548 and audio.get('pcm_bytes') == 917504
                    and audio.get('sample_rate') == 44100 and audio.get('channels') == 2
                    and audio.get('format') == 'pcm_s16le' and audio.get('gallery_media') is False
                    and audio.get('pcm_sha256') == 'd9ff0be15d43b7ebac382c7a056088c97a0652f49829754c3890c0da7c8a38cd',
                    'observer audio provenance differs')
            ident = capture['sha256']
            require(ident not in self.observer_captures and ident not in self.captures
                    and ident not in self.denied, 'duplicate or denied observer screenshot')
            self.observers[observer['id']] = observer
            self.observer_captures[ident] = (observer, capture)
        self.loaded = {}
        configured_source_root = os.environ.get('TOKIMEMO_SOURCE_ROOT')
        self.source_root = Path(configured_source_root) if configured_source_root else self.root

    def read(self, path: str, expected: str, limit: int = MAX_METADATA) -> bytes:
        require(isinstance(expected, str) and HASH.fullmatch(expected), 'invalid expected evidence hash')
        data = checked_bytes(self.root / relative(path), limit)
        require(digest(data) == expected, 'reviewed file hash differs: ' + path)
        return data

    def reviewed_route(self, entry: dict) -> tuple[dict, dict, dict]:
        if entry['id'] in self.loaded:
            return self.loaded[entry['id']]
        base = entry['directory']
        producer = object_json(self.read(base + '/producer.json', entry['producer_sha256']))
        route = object_json(self.read(base + '/proof/route.json', entry['route_sha256']))
        artifacts = object_json(self.read(base + '/proof/artifacts.json', entry['artifacts_sha256']))
        for review in entry['reviews']:
            require(isinstance(review, dict), 'invalid review anchor')
            self.read(relative(review.get('path')), review.get('sha256'))
        require(producer.get('schema') == 'tokimemo.actual-app-producer.v1' and
                route.get('schema') == 'tokimemo.actual-app-route.v1' and
                artifacts.get('schema') == 'tokimemo.actual-app-artifacts.v1', 'unsupported actual-input receipt schema')
        require(isinstance(producer.get('runtime_tree_sha256'), str) and HASH.fullmatch(producer['runtime_tree_sha256']) and
                isinstance(producer.get('revision'), str) and bool(producer['revision']), 'runtime identity missing')
        require(isinstance(route.get('captures'), list) and bool(route['captures']), 'capture list missing or malformed')
        require(route.get('passed') is True and route.get('failures') == [] and route.get('unattempted') == [], 'route did not complete successfully')
        require(type(route.get('completed_steps')) is int and route['completed_steps'] > 0 and
                type(route.get('total_steps')) is int and route['completed_steps'] == route['total_steps'], 'incomplete reviewed route')
        checks = route.get('checks')
        require(isinstance(checks, list) and bool(checks) and all(isinstance(c, dict) and c.get('ok') is True and c.get('outcome') == 'pass' for c in checks), 'route has failed or malformed checks')
        require(artifacts.get('outcome') == 'pass' and type(artifacts.get('runtime_exit_code')) is int and artifacts['runtime_exit_code'] == 0 and
                type(artifacts.get('import_exit_code')) is int and artifacts['import_exit_code'] == 0, 'actual runtime/finalizer failed')
        checks = artifacts.get('checks')
        require(isinstance(checks, list) and bool(checks) and all(isinstance(c, dict) and c.get('outcome') == 'pass' for c in checks), 'finalizer checks failed')
        require(route.get('test_kind') == artifacts.get('test_kind') == 'production_input' and
                route.get('host_input_api') == INPUT_API and isinstance(route.get('inputs'), list) and bool(route['inputs']), 'fixture/reference/translation is not actual input')
        require(producer.get('harness_gameplay_writes') == route.get('harness_gameplay_writes') == [] and
                producer.get('harness_service_completion_injection') is False and producer.get('production_script_changes') == [], 'producer changed gameplay or services')
        require(isinstance(producer.get('environment'), dict) and producer['environment'].get('TOKIMEMO_PLAYABLE_SMOKE') == '0', 'synthetic smoke is not actual input')
        require(route.get('producer_sha256') == entry['producer_sha256'] and
                artifacts.get('producer') == {'path': 'producer.json', 'sha256': entry['producer_sha256']} and
                artifacts.get('route') == {'path': 'proof/route.json', 'sha256': entry['route_sha256']}, 'route/producer/artifact links differ')
        require(producer.get('scenario') == route.get('scenario') and
                producer.get('start_kind') == route.get('start_kind') == artifacts.get('start_kind') and
                producer.get('revision') == route.get('revision') == artifacts.get('revision') and
                producer.get('runtime_tree_sha256') == artifacts.get('runtime_tree_sha256'), 'reviewed scenario/runtime identity differs')
        self.loaded[entry['id']] = (producer, route, artifacts)
        return self.loaded[entry['id']]

    def authorize(self, image_sha: str, suffix: str, size: int) -> dict:
        require(image_sha not in self.denied, 'known fixture/unresolved capture is excluded')
        if image_sha in self.observer_captures:
            observer, capture = self.observer_captures[image_sha]
            require(suffix == '.png' and Path(capture['path']).suffix == suffix
                    and size == capture['bytes'], 'observer gallery admission is screenshot-only')
            evidence_dir = self.root / relative(observer['directory'])
            receipt = observer_evidence.verify_observation(self.source_root, evidence_dir)
            image = self.read(observer['directory'] + '/' + capture['path'], image_sha, 32 * 1024 * 1024)
            require(len(image) == size and image[:8] == b'\x89PNG\r\n\x1a\n' and image[12:16] == b'IHDR'
                    and int.from_bytes(image[16:20], 'big') == 1024
                    and int.from_bytes(image[20:24], 'big') == 768,
                    'observer screenshot dimensions or bytes differ')
            return {
                'schema': 'tokimemo.progress-observer-admission.v1',
                'registry_sha256': self.registry_sha,
                'observer_id': observer['id'],
                'scope': 'first_arrival_line0_observer_only',
                'gameplay_route_acceptance': False,
                'new_game_completion': False,
                'native_pixel_parity': False,
                'native_pcm_parity': False,
                'video_capture': False,
                'producer_sha256': observer['producer_sha256'],
                'captured_production_tree_sha256': observer['captured_production_tree_sha256'],
                'application_sha256': observer['application_sha256'],
                'status_sha256': observer['status_sha256'],
                'capture': {'path': capture['path'], 'sha256': image_sha, 'bytes': size,
                            'dimensions': [1024, 768]},
                'audio_provenance_only': {**receipt['captures']['audio'], 'gallery_media': False},
            }
        require(image_sha in self.captures, 'no coordinator-reviewed capture for image hash')
        entry, capture = self.captures[image_sha]
        require(suffix == '.png' and Path(capture['image']).suffix == suffix and size == capture['bytes'], 'reviewed media type/size differs; no approved video receipt')
        _producer, route, artifacts = self.reviewed_route(entry)
        matches = [c for c in route.get('captures', []) if isinstance(c, dict) and c.get('id') == capture['capture_id']]
        require(len(matches) == 1, 'reviewed route capture missing or ambiguous')
        receipt_path = 'proof/' + capture['image'] + '.json'
        receipt_bytes = self.read(entry['directory'] + '/' + receipt_path, capture['receipt_sha256'])
        receipt = object_json(receipt_bytes)
        require(receipt == matches[0], 'capture sidecar differs from reviewed route record')
        require(receipt.get('image') == capture['image'] and receipt.get('png_sha256') == image_sha and
                receipt.get('producer_sha256') == entry['producer_sha256'], 'capture is not bound to source image/producer')
        require(receipt.get('resized') is False and receipt.get('saved') is True and
                receipt.get('image_producer') == VIEWPORT and isinstance(receipt.get('display'), str) and
                receipt['display'].lower() not in ['', 'headless'], 'not an actual unscaled viewport capture')
        image_path = 'proof/' + capture['image']
        image = self.read(entry['directory'] + '/' + image_path, image_sha, 32 * 1024 * 1024)
        require(len(image) == size and image[:8] == b'\x89PNG\r\n\x1a\n' and image[12:16] == b'IHDR' and len(image) >= 24, 'source PNG identity differs')
        dimensions = [int.from_bytes(image[16:20], 'big'), int.from_bytes(image[20:24], 'big')]
        require(isinstance(receipt.get('dimensions'), list) and len(receipt['dimensions']) == 2 and
                all(type(value) is int and value > 0 for value in receipt['dimensions']) and
                receipt['dimensions'] == dimensions, 'capture dimensions differ from exact source image')
        files = artifacts.get('artifacts')
        require(isinstance(files, dict) and files.get(image_path) == {'sha256': image_sha, 'size': size} and
                files.get(receipt_path) == {'sha256': capture['receipt_sha256'], 'size': len(receipt_bytes)}, 'finalizer does not bind source image and receipt')
        return {'schema': 'tokimemo.progress-admission.v1', 'registry_sha256': self.registry_sha,
                'route_sha256': entry['route_sha256'], 'producer_sha256': entry['producer_sha256'],
                'artifacts_sha256': entry['artifacts_sha256'], 'capture_receipt_sha256': capture['receipt_sha256']}
