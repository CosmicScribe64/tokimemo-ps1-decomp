#!/usr/bin/env python3
"""T-39004: authoritative, machine-checked English translation inventory.

Extends tools/translation_assets.py (historical artwork audit, catalog groups)
and the published candidate ledgers; it is not a parallel system. It expands the
153 grouped catalog rows to frame-level items, binds every item to a source
identity and to the English counterpart, and gives every item exactly one status:

  english-reviewed           exact human style approval of a mapped historical asset
  candidate-only             English draft exists; no human approval (agent review is separate metadata)
  missing                    required and not yet produced
  unmapped                   asset or text exists but its exact source cannot be bound yet
  deliberately-untranslated  no English needed, with a stated reason

A candidate that a newer ledger names in its `supersedes` list leaves the
evidence and is listed under `superseded_records` instead.

Agent acceptance is never recorded as human approval. Runtime verification is a
separate field. Operations (all Docker, Python 3 with Pillow not required):

  sources   capture original container/frame hashes from local caches and disc
  generate  write translations/en/graphics/translation_inventory.json
  check     verify the committed inventory against its committed inputs

Only hashes, ids and reasons are committed; no original game pixels or text.
"""
import argparse
import json
from pathlib import Path
import re

from translation_assets import ROOT, audit, digest, encoded

GRAPHICS = ROOT / 'translations/en/graphics'
SNAPSHOT = GRAPHICS / 'artwork'
CATALOG = GRAPHICS / 'image_text_catalog.jsonl'
DECISIONS = GRAPHICS / 'inventory_decisions.json'
SOURCES = GRAPHICS / 'source_identities.json'
INVENTORY = GRAPHICS / 'translation_inventory.json'
LEDGERS = [SNAPSHOT / 'checkpoints/T39004-2026-10-29/ledger.json',
           SNAPSHOT / 'checkpoints/T39004-pass2/ledger.json',
           SNAPSHOT / 'checkpoints/T39004-pass3/ledger.json',
           SNAPSHOT / 'checkpoints/T39004-battle-miss-v1/ledger.json',
           SNAPSHOT / 'checkpoints/T39004-filesel-v3/ledger.json',
           SNAPSHOT / 'checkpoints/T39004-bs-v1/ledger.json',
           SNAPSHOT / 'checkpoints/T39004-date-panel-v1/ledger.json']
TEXT_MAIN = ROOT / 'translations/en/returned-v3.jsonl'
TEXT_SUPP = ROOT / 'translations/en/supplemental.jsonl'
TEXT_MANIFEST = ROOT / 'translations/en/manifest.json'

STATUSES = ('english-reviewed', 'candidate-only', 'missing', 'unmapped', 'deliberately-untranslated')
RANK = {'missing': 0, 'unmapped': 1, 'candidate-only': 2, 'english-reviewed': 3}
CONTAINER_KEYS = {
    'CDROM/FUKUTEST/MAINROM.NBN': 'MAINROM', 'CDROM/FUKUTEST/OPTION.NBN': 'OPTION',
    'CDROM/FUKUTEST/FILESEL.NBN': 'FILESEL', 'CDROM/NAME_ENT/NAME_ENT.NBN': 'NAME_ENT',
    'CDROM/FUKUTEST/TITLE.NBN': 'TITLE', 'CDROM/FUKUTEST/EXAMINE.NBN': 'EXAMINE'}
KEY_PATHS = {v: k for k, v in CONTAINER_KEYS.items()}
DEFAULT_REASON = {'english-reviewed': 'exact human style approval of every known mapped variant',
                  'candidate-only': 'English draft exists; no human approval, not counted as finished',
                  'missing': 'a known source variant has no English image'}
TAIIKU_LINES = {'kasetu': 123, 'ninsetu': 124, 'spsetu': 125, 'tasetu': 126}
# Groups whose original files cannot be hashed locally (stated in inventory_decisions.json).
NO_CONTAINER_HASH = ('catalog-152', 'catalog-153')
LEDGER_STATUSES = ('draft', 'agent-reviewed-in-game')
# Weekday drafts name their day; frame 0x52 + weekday (Monday first) is the HUD weekday frame.
WEEKDAYS = ('mon', 'tue', 'wed', 'thu', 'fri', 'sat', 'sun')
WEEKDAY_FRAME0 = 82
NO_CHANGE = ('no change required', 'no japanese text present')


def load_json(path):
    return json.loads(path.read_text())


def selector(row):
    return next((row[k] for k in ('frame_id', 'frame_ids_normal___highlight', 'frame___region_id',
                                  'region___cell', 'asset_path') if k in row), '')


def frames_of(sel):
    text = re.sub(r'\([^)]*\)', '', sel)
    numbers = [int(n) for n in re.findall(r'\d{4}', text)]
    if '..' in text and len(numbers) == 2:
        return list(range(numbers[0], numbers[1] + 1))
    return numbers


def catalog_paths(sel, container=''):
    """All container paths a catalog row names; bare sibling names inherit the previous directory."""
    paths, directory = [], None
    for token in re.findall(r'`([^`]+)`', sel):
        if re.fullmatch(r'CDROM/[A-Z0-9_]+/[A-Z0-9_]+\.[A-Z]{2,3}', token):
            paths.append(token)
            directory = token.rsplit('/', 1)[0]
        elif directory and re.fullmatch(r'[A-Z0-9_]+\.[A-Z]{2,3}', token):
            paths.append(directory + '/' + token)
    if not paths:
        paths = re.findall(r'CDROM/[A-Z0-9_]+/[A-Z0-9_]+\.[A-Z]{2,3}', container)
    return paths


def catalog_atoms(rows):
    """Expand catalog rows to (id, line, container_key, frame, selector_text)."""
    atoms = []
    for index, row in enumerate(rows, 1):
        section, container = row['section'], row['container']
        sel = selector(row)
        key = None
        if section.startswith('1.'):
            key = 'MAINROM'
        elif section.startswith('2.'):
            key = 'OPTION'
        elif container in CONTAINER_KEYS:
            key = CONTAINER_KEYS[container]
        if key and 'sheet-' not in sel:
            for frame in frames_of(sel):
                atoms.append({'id': '%s-%04d' % (key.lower(), frame), 'line': index, 'container_key': key,
                              'frame': frame, 'scope': 'frame'})
        elif section.startswith('1.'):
            atoms.append({'id': 'mainrom-sheet-02000', 'line': index, 'container_key': 'MAINROM',
                          'frame': None, 'scope': 'sheet'})
        elif section.startswith('7.'):
            continue  # expanded from candidate records below
        elif section.startswith('8.'):
            continue
        else:
            atoms.append({'id': 'catalog-%03d' % index, 'line': index, 'container_key': None,
                          'frame': None, 'scope': 'group'})
    return atoms


def candidate_key(entry):
    """Map a ledger id to (atom_id, container_key, frame, variant)."""
    cid = entry['id']
    m = re.fullmatch(r'option-label-(\d{4})', cid)
    if m:
        return 'option-%04d' % int(m[1]), 'OPTION', int(m[1]), None
    m = re.fullmatch(r'name-(?:label|panel)-(\d{4})', cid)
    if m:
        return 'name_ent-%04d' % int(m[1]), 'NAME_ENT', int(m[1]), None
    m = re.fullmatch(r'title-(\d{4})', cid)
    if m:
        return 'title-%04d' % int(m[1]), 'TITLE', int(m[1]), None
    m = re.fullmatch(r'exam-(\d{4})', cid)
    if m:
        return 'examine-%04d' % int(m[1]), 'EXAMINE', int(m[1]), None
    m = re.fullmatch(r'filesel-frame-(\d+)\.(base|selected)', cid)
    if m:
        return 'filesel-%04d' % int(m[1]), 'FILESEL', int(m[1]), m[2]
    m = re.fullmatch(r'filesel-page-information-(\d)', cid)
    if m:
        return cid, 'FILESEL', None, None
    m = re.fullmatch(r'mainrom-draft-(\d{4})\.([a-z]+(?:-\d+)?)', cid)
    if m:
        if entry.get('frame') != int(m[1]) or entry.get('palette_id') != m[2]:
            raise ValueError('candidate id disagrees with its frame or palette: ' + cid)
        return 'mainrom-%04d' % int(m[1]), 'MAINROM', int(m[1]), m[2]
    # Alternates (a `-partNN` suffix is one edited part of a multi-part frame) and weekday drafts.
    m = re.fullmatch(r'mainrom-(?:alt-(\d{4})-[a-z]+(?:-part\d{2})?|weekday-[a-z]+(?:-[a-z]+)*-(%s))\.([a-z]+(?:-\d+)?)' % '|'.join(WEEKDAYS), cid)
    if m:
        frame = int(m[1]) if m[1] else WEEKDAY_FRAME0 + WEEKDAYS.index(m[2])
        if entry.get('frame') != frame or entry.get('palette_id') != m[3]:
            raise ValueError('candidate id disagrees with its frame or palette: ' + cid)
        return 'mainrom-%04d' % frame, 'MAINROM', frame, m[3]
    m = re.fullmatch(r'vs_(habu|kuma|robo|sika|ufo)-miss-(\d)', cid)
    if m:
        return 'battle-%s-miss-%s' % (m[1], m[2]), 'BATTLE', None, None
    if re.fullmatch(r'catalog-\d{3}', entry.get('catalog_item', '')):
        return entry['catalog_item'], None, None, None
    m = re.fullmatch(r'(kasetu|ninsetu|spsetu|tasetu)-(\d)', cid)
    if m:
        return 'sports-%s-%s' % (m[1], m[2]), 'TAIIKU', None, None
    m = re.fullmatch(r'vs_(habu|kuma|robo|sika|ufo)-(\d)', cid)
    if m:
        return 'battle-%s-command-%s' % (m[1], m[2]), 'BATTLE', None, None
    if cid.startswith('legacy-rebind-'):
        return 'mainrom-%04d' % entry['frame'], 'MAINROM', entry['frame'], entry.get('palette_id')
    raise ValueError('unrecognised candidate id: ' + cid)


def whole_file_records(asset, ledger_path):
    """Expand a whole-file (BS/NBN) ledger asset into one record per committed crop.

    These ledgers carry no per-crop source or output hash; both come from the
    checkpoint manifest, which must match the manifest_sha256 the ledger binds.
    """
    paths = asset.get('paths') or [asset['path']]
    revision = Path(paths[0]).parts[0]
    manifest_path = ledger_path.parent / revision / 'manifest.json'
    if digest(manifest_path.read_bytes()) != asset['manifest_sha256']:
        raise ValueError('ledger %s: manifest hash differs for %s' % (ledger_path.name, asset['id']))
    manifest = load_json(manifest_path)
    found = [r for r in manifest.get('assets', []) + manifest.get('indexed_assets', [])
             if r['catalog_item'] == asset['catalog_item'] and r.get('id', asset['id']) == asset['id']]
    if len(found) != 1 or found[0]['source_container_sha256'] != asset['source_container_sha256'] \
            or found[0].get('human_approved', False) is not False:
        raise ValueError('ledger %s: %s is not bound to one unapproved manifest record' % (ledger_path.name, asset['id']))
    record = found[0]
    if 'crop' in record:
        if [revision + '/' + record['crop']['png']] != paths or record['crop']['sha256'] != asset.get('crop_sha256'):
            raise ValueError('ledger %s: crop of %s differs from its manifest' % (ledger_path.name, asset['id']))
        return [{**asset, 'revision': revision, 'path': paths[0], 'output_sha256': record['crop']['sha256'],
                 'source_png_sha256': record['source_png_sha256']}]
    pages = {'source-page-%d' % r['page']: r for r in record['edit_regions']}
    sources = {p['variant']: p['source_png_sha256'] for p in record['source_pages']}
    if sorted(revision + '/' + r['png'] for r in pages.values()) != sorted(paths) or not set(pages) <= set(sources):
        raise ValueError('ledger %s: crops of %s differ from its manifest' % (ledger_path.name, asset['id']))
    return [{**asset, 'id': '%s.%s' % (asset['id'], page), 'revision': revision, 'path': revision + '/' + r['png'],
             'output_sha256': r['sha256'], 'source_png_sha256': sources[page], 'palette_id': page}
            for page, r in sorted(pages.items())]


def load_ledgers(ledgers=None):
    entries = []
    for path in (LEDGERS if ledgers is None else ledgers):
        if not path.exists():
            continue
        ledger = load_json(path)
        for asset in ledger['assets']:
            if asset.get('human_approved') is not False:
                raise ValueError('ledger %s claims or omits human approval state: %s' % (path.name, asset.get('id')))
            if asset.get('status') not in LEDGER_STATUSES:
                raise ValueError('ledger %s has unknown status %r: %s' % (path.name, asset.get('status'), asset.get('id')))
            records = whole_file_records(asset, path) if 'catalog_item' in asset else [asset]
            for record in records:
                entries.append({**record, 'ledger': str(path.relative_to(ROOT)) if path.resolve().is_relative_to(ROOT) else path.name,
                                'ledger_sha256': digest(path.read_bytes())})
    return entries


def split_superseded(entries, ledgers=None):
    """(active, superseded) ledger entries.

    A ledger's `supersedes` list names older candidates ({ledger, id, revision,
    output_sha256}) that a newer draft replaces; they leave the candidate
    evidence but stay listed. A reference into a loaded ledger must match one
    entry exactly; references into ledgers that are not loaded are ignored.
    """
    replaced = {}
    for path in (LEDGERS if ledgers is None else ledgers):
        if not path.exists():
            continue
        by = str(path.relative_to(ROOT)) if path.resolve().is_relative_to(ROOT) else path.name
        for ref in load_json(path).get('supersedes', []):
            key = (ref['ledger'], ref['id'], ref['revision'])
            if key in replaced:
                raise ValueError('candidate superseded twice: %s' % (key,))
            replaced[key] = {**ref, 'superseded_by': by}
    loaded = {e['ledger'] for e in entries}
    active, superseded, seen = [], [], set()
    for entry in entries:
        ref = replaced.get((entry['ledger'], entry['id'], entry['revision']))
        if ref is None:
            active.append(entry)
            continue
        if ref['output_sha256'] != entry['output_sha256'] or ref['superseded_by'] == entry['ledger']:
            raise ValueError('superseded reference differs from its candidate: ' + entry['id'])
        seen.add((entry['ledger'], entry['id'], entry['revision']))
        superseded.append({**entry, 'superseded_by': ref['superseded_by']})
    missing = sorted(key for key in replaced if key[0] in loaded and key not in seen)
    if missing:
        raise ValueError('superseded reference names no candidate: %s' % (missing[0],))
    return active, superseded


def historical(report):
    """Attach the container key (None for legacy assets without source mapping)."""
    out = []
    for asset in report['assets']:
        key = CONTAINER_KEYS.get(asset['source_container'])
        out.append({**asset, 'container_key': key})
    return out


def default_decision(row):
    recommendation = row.get('suggested_graphic___abbreviation_same_size', '').lower()
    if any(phrase in recommendation for phrase in NO_CHANGE) or 'already latin' in recommendation:
        return 'deliberately-untranslated', 'catalog states no change is required (not independently re-verified from pixels)'
    return 'missing', 'catalog requires a replacement; no English artwork exists yet'


def build(catalog=CATALOG, decisions_path=DECISIONS, sources_path=SOURCES, snapshot=SNAPSHOT, ledgers=None,
          text_main=TEXT_MAIN, text_supplemental=TEXT_SUPP):
    decisions = load_json(decisions_path)
    sources = load_json(sources_path)
    rows = [json.loads(l) for l in catalog.read_bytes().splitlines() if l]
    report = audit(snapshot)
    hist = historical(report)
    all_entries = load_ledgers(ledgers)
    ledger, superseded = split_superseded(all_entries, ledgers)
    atoms = {}
    order = []

    def add(atom):
        if atom['id'] in atoms:
            return
        atoms[atom['id']] = {**atom, 'historical': [], 'candidates': [], 'variants': {}}
        order.append(atom['id'])

    for atom in catalog_atoms(rows):
        add(atom)
    # Candidate-derived atoms (battle and sports have no frame selector in the catalog).
    for entry in ledger:
        atom_id, key, frame, variant = candidate_key(entry)
        line = {'BATTLE': 119, 'TAIIKU': 123}.get(key)
        if atom_id not in atoms:
            add({'id': atom_id, 'line': line, 'container_key': key, 'frame': frame, 'scope': 'region' if key == 'BATTLE' else 'card'})
    for enc in ('habu', 'kuma', 'robo', 'sika', 'ufo'):
        for miss in (0, 1):
            add({'id': 'battle-%s-miss-%d' % (enc, miss), 'line': 122, 'container_key': 'BATTLE', 'frame': None, 'scope': 'region'})
    for extra in decisions['extra_atoms']:
        add({'id': extra['id'], 'line': extra.get('line'), 'container_key': extra.get('container_key'),
             'frame': extra.get('frame'), 'scope': extra['scope']})
    # Attach evidence.
    for asset in hist:
        resolution = decisions['legacy_resolution'].get(asset['id'].replace('draft:', '', 1))
        if asset['container_key'] is None and resolution:
            asset['container_key'] = CONTAINER_KEYS[resolution['container']]
        if asset['container_key'] is None or asset['frame'] is None:
            continue
        atom_id = '%s-%04d' % (asset['container_key'].lower(), asset['frame'])
        if atom_id in atoms:
            atoms[atom_id]['historical'].append(asset)
    for entry in ledger:
        atom_id, key, frame, variant = candidate_key(entry)
        atoms[atom_id]['candidates'].append(entry)
    items = []
    for atom_id in order:
        atom = atoms[atom_id]
        override = decisions['atom_overrides'].get(atom_id)
        line = atom['line']
        row = rows[line - 1] if line else None
        mapped = [a for a in atom['historical'] if a['mapping_complete'] and a['style'] == 'approve']
        legacy = [a for a in atom['historical'] if not a['mapping_complete']]
        frame_hashes = sources['frames'].get(atom['container_key'] or '', {}).get(str(atom['frame']), {})
        if atom['scope'] == 'page':
            frame_hashes = sources['frames'].get('FILESEL', {}).get('page:source-' + atom_id[len('filesel-'):], {})
        if not frame_hashes and atom['candidates']:
            frame_hashes = {c['id']: c['source_png_sha256'] for c in atom['candidates']}
        # English evidence is matched to source images by source hash, never by palette name.
        evidence = [(a['id'], a['source_png_sha256'], 'english-reviewed') for a in mapped]
        evidence += [(c['id'], c['source_png_sha256'], 'candidate-only') for c in atom['candidates']]
        groups = {}
        for name, sha in frame_hashes.items():
            groups.setdefault(sha, []).append(name)
        variants = {}
        if evidence:
            for sha, names in groups.items():
                have = [st for _, esha, st in evidence if esha == sha]
                variants['|'.join(sorted(names))] = max(have, key=lambda t: RANK[t]) if have else 'missing'
            for label, esha, _ in evidence:
                if esha not in groups:
                    variants['unbound:' + label] = 'unmapped'
        if variants:
            status = min(variants.values(), key=lambda s: RANK[s])
            reason = None
            lacking = sorted(k for k, v in variants.items() if v == 'missing')
            if lacking:
                reason = 'source variants without English: ' + ', '.join(lacking)
            unbound = sorted(k for k, v in variants.items() if v == 'unmapped')
            if unbound:
                reason = 'English evidence not bound to a known source image: ' + ', '.join(unbound)
        elif override:
            status, reason = override['status'], override['reason']
        elif row is not None:
            status, reason = default_decision(row)
        else:
            raise ValueError('atom without status source: ' + atom_id)
        if status not in STATUSES:
            raise ValueError('bad status ' + status)
        path = KEY_PATHS.get(atom['container_key'])
        parts = atom_id.split('-')
        if atom['container_key'] == 'TAIIKU':
            path = 'CDROM/TAIIKU/%s.BIN' % parts[1].upper()
            line = TAIIKU_LINES[parts[1]]
        elif atom['container_key'] == 'BATTLE':
            path = 'CDROM/BATTLE/VS_%s.BIN' % parts[1].upper()
            line = 122 if parts[2] == 'miss' else 119
        row = rows[line - 1] if line else None
        all_paths = [path] if path else []
        if path is None and row is not None:
            all_paths = catalog_paths(selector(row), row['container']) or [row['container']]
            path = all_paths[0]
        corrected = sources.get('path_corrections', {}).get(path)
        known = sources['containers'].get(path) if path else None
        if known is None and atom['candidates']:
            known = atom['candidates'][0]['source_container_sha256']
        for candidate in atom['candidates']:
            if candidate['source_container_sha256'] != known:
                raise ValueError('candidate %s is bound to a different source container than %s' % (candidate['id'], atom_id))
        item = {
            'id': atom_id, 'scope': atom['scope'], 'catalog_line': line,
            'catalog_lines': [119, 120, 121] if (atom['container_key'] == 'BATTLE' and line == 119) else ([line] if line else []),
            'container': corrected or path,
            'catalog_container_path': path if corrected else None,
            'container_sha256': known, 'frame': atom['frame'],
            'containers': [{'path': sources.get('path_corrections', {}).get(q, q), 'catalog_path': q if q in sources.get('path_corrections', {}) else None,
                            'sha256': sources['containers'].get(q)} for q in all_paths] if atom['scope'] == 'group' else [],
            'source_png_sha256': frame_hashes,
            'status': status,
            'reason': reason or (override or {}).get('reason') or DEFAULT_REASON[status],
            'english': {
                'historical_assets': sorted(a['id'] for a in mapped),
                'legacy_unmapped_assets': sorted(a['id'] for a in legacy),
                'candidates': sorted(({'id': c['id'], 'revision': c['revision'],
                                       'path': c['path'], 'output_sha256': c['output_sha256']}
                                      for c in atom['candidates']), key=lambda c: (c['revision'], c['id']))},
            'variant_status': {('default' if k is None else str(k)): v for k, v in sorted(variants.items(), key=lambda kv: str(kv[0]))},
            'human_review': 'approved-exact' if mapped else None,
            'agent_review': 'bounded-in-game' if any(c['status'] == 'agent-reviewed-in-game' for c in atom['candidates']) else None,
            'runtime': 'none',
            'required_by_source': 'catalog',
            'notes': []}
        if override:
            item['notes'] = override.get('notes', [])
            for field in ('runtime', 'required_by_source', 'blockers'):
                if field in override:
                    item[field] = override[field]
            if override.get('status') and variants and override['status'] != status:
                raise ValueError('override contradicts evidence: ' + atom_id)
        items.append(item)
    for item in items:
        for runtime_id, runtime in decisions['runtime'].items():
            if item['id'] in runtime['atoms']:
                item['runtime'] = runtime['state']
    # Historical and candidate sections.
    hist_items = []
    for asset in hist:
        rebind = decisions['legacy_resolution'].get(asset['id'].replace('draft:', '', 1))
        status = 'english-reviewed' if asset['mapping_complete'] and asset['style'] == 'approve' else 'unmapped'
        hist_items.append({'id': asset['id'], 'batch': asset['batch'], 'output': asset['output'],
                           'output_sha256': asset['output_sha256'], 'dimensions': asset['dimensions'],
                           'source_png_sha256': asset['source_png_sha256'],
                           'source_container': asset['source_container'], 'frame': asset['frame'],
                           'palette': asset['palette'], 'status': status,
                           'human_review': {'decision': asset['style'], 'event_id': asset['exact_review_event']},
                           'resolution': rebind})
    cand_items = []
    for entry in ledger:
        atom_id = candidate_key(entry)[0]
        cand_items.append({'id': entry['id'], 'revision': entry['revision'], 'path': entry['path'],
                           'ledger': entry['ledger'], 'output_sha256': entry['output_sha256'],
                           'source_png_sha256': entry['source_png_sha256'], 'palette': entry.get('palette_id'),
                           'source_container_sha256': entry['source_container_sha256'],
                           'status': 'candidate-only', 'atom': atom_id,
                           'review_level': 'agent-bounded-in-game' if entry['status'] == 'agent-reviewed-in-game' else 'none',
                           'human_approved': entry['human_approved']})
    sup_items = [{'id': entry['id'], 'revision': entry['revision'], 'path': entry['path'], 'ledger': entry['ledger'],
                  'output_sha256': entry['output_sha256'], 'source_png_sha256': entry['source_png_sha256'],
                  'palette': entry.get('palette_id'), 'atom': candidate_key(entry)[0], 'status': 'superseded',
                  'superseded_by': entry['superseded_by'], 'human_approved': entry['human_approved']}
                 for entry in superseded]
    approval = decisions.get('text_approval')
    text = text_groups(text_main, text_supplemental, sources.get('text_blank_checks', {}), approval is not None)
    summary = summarize(items, hist_items, cand_items, text)
    summary['superseded_records'] = len(sup_items)
    return {'schema': 'tokimemo.translation-inventory.v1',
            'inputs': {'catalog_sha256': digest(catalog.read_bytes()),
                       'snapshot_sha256': report['snapshot_sha256'],
                       'decisions_sha256': digest(decisions_path.read_bytes()),
                       'sources_sha256': digest(sources_path.read_bytes()),
                       'ledgers': {e['ledger']: e['ledger_sha256'] for e in all_entries},
                       'text_main_sha256': digest(text_main.read_bytes()),
                       'text_supplemental_sha256': digest(text_supplemental.read_bytes())},
            'statuses': list(STATUSES), 'coverage_complete': False,
            'limitations': decisions['limitations'],
            'summary': summary, 'artwork_items': items, 'historical_assets': hist_items,
            'candidate_records': cand_items, 'superseded_records': sup_items, 'text_groups': text['groups'], 'text_blank_evidence': text['evidence'], 'text_approval': approval,
            'name_header_prompts': [{**q, 'wording_status': 'english-approved' if approval else 'candidate-only'} for q in decisions['name_header_prompts']]}


BLANK_STATUS = ('empty_source_slot', 'whitespace_only_source_slot', 'empty_dictionary_slot')


def text_class(record, blank_checks=None, approved=False):
    """Return (class, evidence). A blank target is deliberate only when the source is shown blank:

    an empty/whitespace/dictionary-slot review status, an 'Empty source slot' translator note,
    or original source bytes checked blank (captured by the sources operation). A whitespace
    target over a non-blank or unverified source stays an ordinary candidate.

    Returns (class, evidence, context_status). Produced English (including whitespace targets over a source not shown blank) is 'english-approved' only
    when the explicit user decision is passed in; otherwise it is a candidate. The technical
    context status ('ready' or 'unproven') is independent of wording approval.
    """
    target = record.get('target_text') or record.get('translation') or ''
    status = record['review_status']
    note = record.get('translator_note') or ''
    context = 'ready' if status in ('ready_for_review', 'supplemental_ready') else 'unproven'
    ready = 'candidate-only:' + ('ready' if context == 'ready' else 'blocked')
    if target.strip():
        return ('english-approved' if approved else ready), None, context
    evidence = None
    for key in BLANK_STATUS:
        if key in status:
            evidence = 'review-status:' + key
    if 'Empty source slot' in note:
        evidence = 'translator-note:empty-source-slot'
    check = (blank_checks or {}).get(record.get('id'))
    if evidence is None and check is not None and check['blank']:
        evidence = 'source-bytes-checked-blank'
    if evidence:
        return 'deliberately-untranslated', evidence, None
    if target:  # whitespace target, source not shown blank; approved by the user addendum
        return ('english-approved' if approved else ready), 'whitespace-target-source-not-proven-blank' if check is None else 'whitespace-target-source-not-blank', context
    return 'missing', None, None


def text_groups(text_main=TEXT_MAIN, text_supplemental=TEXT_SUPP, blank_checks=None, approved=False):
    groups, evidence = {}, {}
    for path, source in ((text_main, 'returned-v3'), (text_supplemental, 'supplemental')):
        for line in path.read_bytes().splitlines():
            record = json.loads(line)
            parts = record['id'].split(':')
            if source == 'supplemental':
                family, container = 'supplemental', record['context'].split(';')[0]
            elif parts[0] in ('talk', 'speaktst-layout', 'dictionary', 'overlay-table', 'bunka-sd-view', 'c',
                              'runtime', 'rpg-buffer-initial', 'unresolved-c-locator'):
                family, container = parts[0], parts[1]
            else:
                family, container = 'exe-literal', parts[0]
            key = (source, family, container)
            group = groups.setdefault(key, {'source': source, 'family': family, 'container': container,
                                            'records': 0, 'statuses': {}})
            cls, why, context = text_class(record, blank_checks, approved)
            group['records'] += 1
            group['statuses'][cls] = group['statuses'].get(cls, 0) + 1
            if context:
                counts = group.setdefault('context_status', {})
                counts[context] = counts.get(context, 0) + 1
                if context == 'unproven':
                    reasons = group.setdefault('context_reasons', {})
                    reasons[record['review_status']] = reasons.get(record['review_status'], 0) + 1
            if why:
                evidence[record['id']] = why
    return {'groups': [groups[k] for k in sorted(groups)], 'evidence': dict(sorted(evidence.items()))}


def summarize(items, hist, cands, text):
    def count(rows, field='status'):
        out = {s: 0 for s in STATUSES}
        for row in rows:
            out[row[field]] += 1
        return out
    text_counts, context_counts = {}, {}
    for group in text['groups']:
        for cls, n in group['statuses'].items():
            text_counts[cls] = text_counts.get(cls, 0) + n
        for cls, n in group.get('context_status', {}).items():
            context_counts[cls] = context_counts.get(cls, 0) + n
    return {'artwork_items': {'total': len(items), 'by_status': count(items)},
            'historical_assets': {'total': len(hist), 'by_status': count(hist)},
            'candidate_records': {'total': len(cands),
                                  'distinct_assets': len({c['id'] + '|' + c['source_png_sha256'] for c in cands if c['review_level'] == 'none'}),
                                  'agent_reviewed_revisions': sum(c['review_level'] != 'none' for c in cands),
                                  'human_approved': sum(c['human_approved'] for c in cands)},
            'text_records': {'total': sum(g['records'] for g in text['groups']), 'by_class': text_counts,
                             'context_status': context_counts, 'groups': len(text['groups'])}}


def capture_sources(disc, cache):
    """Collect original container hashes and per-frame PNG hashes from local caches."""
    rows = [json.loads(l) for l in CATALOG.read_bytes().splitlines() if l]
    paths = set(CONTAINER_KEYS)
    paths.update('CDROM/BATTLE/VS_%s.BIN' % enc for enc in ('HABU', 'KUMA', 'ROBO', 'SIKA', 'UFO'))
    for row in rows:
        paths.update(catalog_paths(selector(row), row['container']))
    containers, corrections, absent = {}, {}, []
    for path in sorted(paths):
        target = disc / path
        if not target.is_file() and '/PROLODIR/' in path and (disc / path.replace('/PROLODIR/', '/BS/')).is_file():
            corrections[path] = path.replace('/PROLODIR/', '/BS/')
            target = disc / corrections[path]
        if target.is_file():
            containers[path] = digest(target.read_bytes())
        else:
            absent.append(str(disc / path))
    frames = {}
    mapping = {'MAINROM': 'original-mainrom-v1', 'OPTION': 'original-option-v1', 'FILESEL': 'original-filesel-v1',
               'NAME_ENT': 'original-name-v1', 'TITLE': 'original-title-v1', 'EXAMINE': 'original-exam-v1'}
    for key, directory in mapping.items():
        base = cache / directory
        manifest = base / 'manifest.json'
        if not manifest.exists():
            absent.append(str(manifest))
            continue
        data = load_json(manifest)
        found = {}
        for image in ([] if key == 'MAINROM' else data.get('images', [])):
            if image.get('frame_index') is None and key == 'FILESEL' and str(image.get('variant', '')).startswith('source-page-'):
                found.setdefault('page:' + image['variant'], {})[image['variant']] = digest((base / image['png']).read_bytes())
                continue
            if image.get('frame_index') is None or 'png' not in image:
                continue
            found.setdefault(str(image['frame_index']), {})[image.get('variant', 'frame')] = digest((base / image['png']).read_bytes())
        if key == 'MAINROM':
            for path in sorted(base.glob('mainrom.frame-*.png')):
                if '.stp.' in path.name:
                    continue
                m = re.fullmatch(r'mainrom\.frame-(\d+)\.(.+)\.png', path.name)
                found.setdefault(str(int(m[1])), {})[m[2]] = digest(path.read_bytes())
        frames[key] = found
    blank_checks = {}
    for path, key in ((TEXT_MAIN, 'target_text'), (TEXT_SUPP, 'translation')):
        for line in path.read_bytes().splitlines():
            record = json.loads(line)
            target = record.get(key) or ''
            parts = record['id'].split(':')
            if not (target and target.isspace()):
                continue
            if parts[0] == 'supplemental':
                container, offset = parts[1], int(parts[2], 16)
            elif parts[0] == 'SLPM_86.053' or parts[0].startswith('CDROM/EXEDIR/'):
                container, offset = parts[0], int(parts[1], 16)
            else:
                continue
            target_file = disc / container
            if not target_file.is_file():
                absent.append(str(target_file))
                continue
            data = target_file.read_bytes()
            end = data.find(b'\0', offset)
            raw = data[offset:end if 0 <= end < offset + 80 else offset + 80]
            try:
                blank = bool(raw) and all(c in ' \u3000' for c in raw.decode('cp932'))
            except UnicodeDecodeError:
                blank = False
            blank_checks[record['id']] = {'blank': blank, 'length': len(raw), 'sha256': digest(raw)}
    if absent:
        raise SystemExit('cannot capture source identities; missing inputs:\n' + '\n'.join(absent))
    return {'schema': 'tokimemo.translation-source-identities.v1', 'containers': containers, 'frames': frames,
            'path_corrections': corrections, 'text_blank_checks': dict(sorted(blank_checks.items())),
            'note': 'Hashes of original files only; captured from the local disc extraction and restored caches.'}


def validate_artwork(item):
    problems = []
    name, status = item['id'], item['status']
    if not item.get('reason'):
        problems.append('artwork item %s has no reason' % name)
    if item.get('container') is None and item['scope'] != 'group':
        problems.append('artwork item %s has no source container' % name)
    if name not in NO_CONTAINER_HASH and not item['container_sha256']:
        problems.append('artwork item %s has no container hash' % name)
    needs_source = item['scope'] in ('frame', 'page') or (item['scope'] in ('card', 'region') and status != 'missing') \
        or status in ('english-reviewed', 'candidate-only')
    if needs_source and not item['source_png_sha256']:
        problems.append('artwork item %s has no source image hash' % name)
    english = item['english']
    if status == 'english-reviewed' and not (english['historical_assets'] and item['human_review'] == 'approved-exact'):
        problems.append('artwork item %s claims review without an exact human approval' % name)
    if status == 'candidate-only' and not english['candidates'] and not english['historical_assets']:
        problems.append('artwork item %s claims candidate without a file' % name)
    if status in ('english-reviewed', 'candidate-only') and any(v in ('missing', 'unmapped') for v in item['variant_status'].values()):
        problems.append('artwork item %s hides an uncovered source variant behind %s' % (name, status))
    return problems


def validate(inventory):
    """Return a list of problems; an empty list means every item is complete."""
    problems = []
    seen = set()
    for section in ('artwork_items', 'historical_assets', 'candidate_records'):
        for item in inventory[section]:
            key = (section, item['id'], item.get('revision'))
            if key in seen:
                problems.append('duplicate item %r' % (key,))
            seen.add(key)
            status = item.get('status')
            if status not in STATUSES:
                problems.append('%s %s lacks a valid status: %r' % (section, item['id'], status))
                continue
            if section == 'artwork_items':
                problems.extend(validate_artwork(item))
            if section == 'candidate_records' and (item.get('human_approved') or status != 'candidate-only'):
                problems.append('candidate %s claims approval' % item['id'])
            if section == 'historical_assets' and status == 'unmapped' and not item.get('resolution'):
                problems.append('unmapped asset %s has no recorded resolution' % item['id'])
    for item in inventory.get('superseded_records', []):
        if item.get('human_approved') or item.get('status') != 'superseded' or not item.get('superseded_by'):
            problems.append('superseded candidate %s lacks its replacement or claims approval' % item['id'])
    hist = {h['id']: h for h in inventory['historical_assets']}
    cand = {(c['id'], c['revision']): c for c in inventory['candidate_records']}
    for item in inventory['artwork_items']:
        hashes = item['source_png_sha256']
        evidence = [(hid, hist[hid]['source_png_sha256'], hist[hid]['palette']) for hid in item['english']['historical_assets']]
        evidence += [(c['id'], cand[(c['id'], c['revision'])]['source_png_sha256'], cand[(c['id'], c['revision'])].get('palette'))
                     for c in item['english']['candidates']]
        for label, sha, palette in evidence:
            if sha not in hashes.values():
                problems.append('artwork item %s: source of %s is not a known source image' % (item['id'], label))
            elif palette in hashes and hashes[palette] != sha:
                problems.append('artwork item %s: %s names palette %s but its source hash differs' % (item['id'], label, palette))
    for group in inventory['text_groups']:
        if sum(group['statuses'].values()) != group['records']:
            problems.append('text group %s/%s has unclassified records' % (group['family'], group['container']))
    approval = inventory.get('text_approval')
    approved_classes = sum(g['statuses'].get('english-approved', 0) for g in inventory['text_groups'])
    if approval is None and approved_classes:
        problems.append('text records are approved without an explicit approval decision')
    if approval is not None:
        for field, value in (('approver', 'user (project owner)'), ('source', 'chat instruction')):
            if approval.get(field) != value:
                problems.append('text approval decision lacks %s %r' % (field, value))
        if not approval.get('date') or not approval.get('scope'):
            problems.append('text approval decision lacks date or scope')
    for group in inventory['text_groups']:
        unproven = group.get('context_status', {}).get('unproven', 0)
        if unproven != sum(group.get('context_reasons', {}).values()):
            problems.append('text group %s/%s lost its unproven-context reasons' % (group['family'], group['container']))
    for prompt in inventory['name_header_prompts']:
        if prompt['status'] not in STATUSES:
            problems.append('prompt %s lacks a valid status' % prompt['key'])
    return problems


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('operation', choices=['sources', 'generate', 'check'])
    parser.add_argument('--disc', type=Path, default=ROOT / 'disc/files')
    parser.add_argument('--cache', type=Path, default=ROOT / '.asset-cache/translation-assets')
    parser.add_argument('--catalog', type=Path, default=CATALOG)
    parser.add_argument('--decisions', type=Path, default=DECISIONS)
    parser.add_argument('--sources', type=Path, default=SOURCES)
    parser.add_argument('--snapshot', type=Path, default=SNAPSHOT)
    parser.add_argument('--inventory', type=Path, default=INVENTORY)
    args = parser.parse_args()
    if args.operation == 'sources':
        args.sources.write_bytes(encoded(capture_sources(args.disc, args.cache)) + b'\n')
        print('wrote', args.sources)
        return
    inventory = build(args.catalog, args.decisions, args.sources, args.snapshot)
    if args.operation == 'generate':
        args.inventory.write_bytes(encoded(inventory) + b'\n')
    else:
        if args.inventory.read_bytes() != encoded(inventory) + b'\n':
            raise SystemExit('committed inventory differs from its inputs; run generate')
        problems = validate(inventory)
        if problems:
            raise SystemExit('\n'.join(problems))
    print(json.dumps(inventory['summary'], sort_keys=True))


if __name__ == '__main__':
    main()
