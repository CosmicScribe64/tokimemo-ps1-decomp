"""Export original UI profiles through a source-bound, cache-confined command.

T-28002/T-28004. Run in Docker. An exact whole-input hash permits reuse of a
baseline inventory; every decoded file is still re-read and hash-checked.
Unselected baseline exports and unresolved source records remain in the census.
Cached reuse covers the supplied baseline's records; it cannot rediscover records
that were omitted from a filtered baseline.
"""
import argparse
from copy import deepcopy
import importlib
import json
from pathlib import Path
import re
import sys
import tempfile

if __package__ in (None, ''):
    sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from assets.input import open_input
from assets.manifest import SCHEMA, asset_id, digest, output_root, summarize, write_bytes, write_manifest
from assets.media_coverage import _compact_location

# module, probe/export suffix, exact file size, original LBA, paired API mode.
PROFILES = {
    'CDROM/FUKUTEST/TITLE.NBN': ('image_packs_title', 'title_ui', 0x6B000, 0xA0CC, 'optional'),
    'CDROM/FUKUTEST/FILESEL.NBN': ('image_packs_filesel', 'filesel_ui', 0x44800, 0x9CB5, 'optional'),
    'CDROM/FUKUTEST/OPTION.NBN': ('image_packs_option', 'option_ui', 0x1E800, 0x9EA4, 'none'),
    'CDROM/FUKUTEST/EXAMINE.NBN': ('image_packs_exam', 'exam_ui', 0x6800, 0x9CA8, 'required'),
    'CDROM/NAME_ENT/NAME_ENT.NBN': ('image_packs_name', 'name_ui', 0x1B000, 0xA9E2, 'optional'),
    'CDROM/TAIIKU/KASETU.BIN': ('image_packs_sports', 'sports_ui', 0xF800, 0xB79F, 'source_path'),
    'CDROM/TAIIKU/SPSETU.BIN': ('image_packs_sports', 'sports_ui', 0x13000, 0xB969, 'source_path'),
    'CDROM/TAIIKU/TASETU.BIN': ('image_packs_sports', 'sports_ui', 0x13800, 0xBAEE, 'source_path'),
    'CDROM/TAIIKU/NINSETU.BIN': ('image_packs_sports', 'sports_ui', 0x27800, 0xB861, 'source_path'),
    'CDROM/BATTLE/VS_HABU.BIN': ('image_packs_battle', 'battle_ui', 0x40000, 0x3A56, 'source_path'),
    'CDROM/BATTLE/VS_SIKA.BIN': ('image_packs_battle', 'battle_ui', 0x38000, 0x3D15, 'source_path'),
    'CDROM/BATTLE/VS_KUMA.BIN': ('image_packs_battle', 'battle_ui', 0x38000, 0x3BA9, 'source_path'),
    'CDROM/BATTLE/VS_UFO.BIN': ('image_packs_battle', 'battle_ui', 0x4C800, 0x3DF3, 'source_path'),
    'CDROM/BATTLE/VS_ROBO.BIN': ('image_packs_battle', 'battle_ui', 0x4A800, 0x3C5E, 'source_path'),
}
_SHA256 = re.compile(r'[0-9a-f]{64}')


def _baseline(manifest):
    if manifest is None:
        return {}
    if not isinstance(manifest, dict) or manifest.get('schema') != SCHEMA or not isinstance(manifest.get('assets'), list):
        raise ValueError('baseline must be a media.v1 manifest with asset records')
    if (('input' in manifest and not isinstance(manifest['input'], dict))
            or not isinstance(manifest.get('limitations', []), list)
            or any(not isinstance(item, str) for item in manifest.get('limitations', []))):
        raise ValueError('baseline input and limitations metadata are malformed')
    records = {}
    for record in manifest['assets']:
        if (not isinstance(record, dict) or not isinstance(record.get('source'), dict)
                or not isinstance(record['source'].get('path'), str)
                or not record['source']['path'] or not isinstance(record.get('id'), str)
                or re.fullmatch(r'asset-[0-9a-f]{64}', record['id']) is None):
            raise ValueError('baseline asset requires source provenance and safe asset id')
        path = record['source']['path']
        sha = record['source'].get('sha256')
        if sha is not None and (not isinstance(sha, str) or not _SHA256.fullmatch(sha)
                                or record['id'] != asset_id(path, sha)):
            raise ValueError('baseline asset id or source hash differs from its path/content identity')
        if path in records:
            raise ValueError('duplicate baseline source path')
        records[path] = record
    return records


def _read_checked(source, entry):
    sha = entry.get('sha256')
    if not isinstance(sha, str) or not _SHA256.fullmatch(sha):
        raise ValueError('readable source has no valid SHA-256: ' + entry['path'])
    data = source.read(entry)
    if digest(data) != sha:
        raise ValueError('source changed after inventory: ' + entry['path'])
    return data


def export_ui_source(source, destination, baseline=None, *, include_sd=False):
    """Publish established original UI profiles, preserving the complete input census."""
    old = _baseline(baseline)
    destination = output_root(destination)
    original = Path(source.path).resolve()
    if original == destination or original in destination.parents or destination in original.parents:
        raise ValueError('input and output paths must not overlap')
    description = source.describe()
    input_hash = description.get('source_sha256')
    cached = bool(old and isinstance(input_hash, str) and _SHA256.fullmatch(input_hash)
                  and (baseline.get('input') or {}).get('source_sha256') == input_hash)
    if cached and (baseline.get('input') or {}).get('format') == description.get('format'):
        description = dict(baseline['input'], **description)
    entries = ([deepcopy(record['source']) for record in old.values()] if cached else source.entries())
    indexed = {}
    for entry in entries:
        if (not isinstance(entry, dict) or not isinstance(entry.get('path'), str)
                or not entry['path'] or entry['path'] in indexed
                or not isinstance(entry.get('size'), int) or isinstance(entry['size'], bool)
                or entry['size'] < 0 or not isinstance(entry.get('kind'), str)
                or ('location' in entry and not isinstance(entry['location'], dict))):
            raise ValueError('input entries require unique paths, nonnegative sizes and kind')
        indexed[entry['path']] = entry
    for path, entry in indexed.items():
        previous = old.get(path)
        if previous and (entry.get('sha256') != previous['source'].get('sha256')
                         or entry['size'] != previous['source'].get('size')):
            raise ValueError('baseline source differs from current inventory: ' + path)
    paired_entry = indexed.get('SLPM_86.053')
    paired_data = None
    profiles = dict(PROFILES)
    sd_status = 'not requested'
    if include_sd:
        if (paired_entry is None or paired_entry['kind'] != 'file'
                or not 0 < paired_entry['size'] <= 4 * 1024 * 1024):
            sd_status = 'paired executable required to resolve original SD loader tables'
        else:
            paired_data = _read_checked(source, paired_entry)
            from assets.image_packs_sd import sd_profile_rows
            rows = sd_profile_rows(paired_data)
            for path, entry in indexed.items():
                matches = [row for row in rows if row['source_lba'] == (entry.get('location') or {}).get('lba')]
                layouts = {(row['tpage_table_offset'], row['frame_table_offset'], row['sequence_table_offset'],
                            row['read_sector_count']) for row in matches}
                if matches and len(layouts) == 1:
                    profiles[path] = ('image_packs_sd', 'sd_ui', matches[0]['read_sector_count'] * 2048,
                                      matches[0]['source_lba'], 'source_entry')
            sd_status = 'bound by original executable LBA/layout tables'
    destination.mkdir(parents=True, exist_ok=True)
    records = []
    for path, entry in sorted(indexed.items()):
        previous = old.get(path)
        record = deepcopy(previous) if previous else {
            'id': asset_id(path, entry.get('sha256') or input_hash or ''),
            'status': 'unsupported', 'format': 'unknown',
            'reason': entry.get('unsupported', 'not decoded by the selected UI profiles')}
        record['source'] = deepcopy(entry)
        if 'location' in record['source']:
            record['source']['location'] = _compact_location(record['source']['location'])
        records.append(record)
        if path not in profiles:
            continue
        module_name, suffix, expected_size, expected_lba, pair_mode = profiles[path]
        for key in ('export', 'outputs', 'original', 'paired_original', 'paired_source', 'inspection'):
            record.pop(key, None)
        record.update(status='unsupported', format='tokimemo-' + ('sd-animation' if suffix == 'sd_ui' else suffix.replace('_', '-')))
        if entry['kind'] != 'file':
            record['reason'] = entry.get('unsupported', 'original UI source entry is not a readable file')
            continue
        if entry['size'] != expected_size:
            record['reason'] = 'source size differs from the original UI profile'
            continue
        lba = (entry.get('location') or {}).get('lba')
        if (lba is not None or pair_mode in {'source_path', 'source_entry'}) and lba != expected_lba:
            record['reason'] = 'source LBA differs from the original UI profile'
            continue
        usable_pair = (pair_mode in {'optional', 'required', 'source_entry'} and paired_entry is not None
                       and paired_entry['kind'] == 'file' and 0 < paired_entry['size'] <= 4 * 1024 * 1024)
        if pair_mode in {'required', 'source_entry'} and not usable_pair:
            record['reason'] = 'paired original executable required for source CLUTs'
            continue
        if usable_pair and paired_data is None:
            paired_data = _read_checked(source, paired_entry)
        exe = paired_data if usable_pair else None
        data = _read_checked(source, entry)
        module = importlib.import_module('assets.' + module_name)
        probe = getattr(module, 'probe_' + suffix)
        exporter = getattr(module, 'export_' + suffix)
        if pair_mode == 'none':
            inspection = probe(data)
        elif pair_mode == 'source_path':
            inspection = probe(data, source_path=path)
        elif pair_mode == 'source_entry':
            inspection = probe(data, exe, source_entry=entry)
        else:
            inspection = probe(data, exe)
        record['inspection'] = inspection
        record['format'] = inspection.get('format', record['format'])
        if inspection.get('status') != 'supported':
            record.update(status=inspection.get('status', 'invalid'),
                          reason=inspection.get('reason', 'source profile validation failed'))
            continue
        target = destination / 'assets' / record['id']
        with tempfile.TemporaryDirectory(prefix='.ui-', dir=destination) as temporary:
            stage = Path(temporary)
            if pair_mode == 'none':
                exported = exporter(data, stage, record['id'])
            elif pair_mode == 'source_path':
                exported = exporter(data, exe, stage, record['id'], source_path=path)
            elif pair_mode == 'source_entry':
                exported = exporter(data, exe, stage, record['id'], source_entry=entry)
            else:
                exported = exporter(data, exe, stage, record['id'])
            outputs = []
            for output in sorted(stage.iterdir()):
                if output.is_symlink() or not output.is_file():
                    raise ValueError('UI reader produced unexpected output entry')
                outputs.append(write_bytes(target / output.name, output.read_bytes()))
        record.update(status=exported.get('status', 'exported'), export=exported, outputs=outputs,
                      original=write_bytes(target / 'original.bin', data))
        if usable_pair:
            record.update(paired_source=deepcopy(paired_entry),
                          paired_original=write_bytes(target / 'paired-executable.bin', paired_data))
        record.pop('reason', None)
        write_manifest(target / 'metadata.json', record)
    result = {'schema': SCHEMA, 'input': description, 'assets': records,
              'coverage': summarize(records),
              'options': {'reader': 'original-ui-profiles', 'baseline_exports_retained': baseline is not None,
                          'inventory_reused_by_whole_input_hash': cached,
                          'inventory_scope': 'supplied baseline source records' if cached else 'fresh original-input reader inventory',
                          'sd_profile_status': sd_status, 'profiles': sorted(profiles)},
              'limitations': sorted(set((baseline or {}).get('limitations', []) + [
                  'static previews do not reproduce runtime scene composition or blending',
                  'cached baseline inventory reuse does not rediscover omitted source records',
                  'unmapped source extents and unselected formats remain unresolved'])),
              'original_reconstruction_proven': False}
    if baseline and 'context_provenance' in baseline:
        result['context_provenance'] = deepcopy(baseline['context_provenance'])
    write_manifest(destination / 'manifest.json', result)
    return result


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('input')
    parser.add_argument('--baseline-media')
    parser.add_argument('--include-sd', action='store_true', help='resolve additional SD sources from paired executable loader tables')
    parser.add_argument('--output', required=True)
    args = parser.parse_args(argv)
    try:
        baseline = json.loads(Path(args.baseline_media).read_text()) if args.baseline_media else None
        with open_input(args.input) as source:
            report = export_ui_source(source, args.output, baseline, include_sd=args.include_sd)
        print(json.dumps(report['coverage'], sort_keys=True))
        return 1 if report['coverage']['status'].get('invalid') else 0
    except (OSError, ValueError, EOFError) as error:
        print('media_ui_export.py: %s' % error, file=sys.stderr)
        return 1


if __name__ == '__main__':
    raise SystemExit(main())
