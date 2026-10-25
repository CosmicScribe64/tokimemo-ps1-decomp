"""Opt-in proof of native overlay background callback steps (T-28002/T-28004).

Only a strict unconditional dispatcher and exact helper protocol qualify.
Native tables are bounded by an original overlay inventory/hash/config binding.
The result composes legacy contexts; candidates and rejected tables never add
geometry. Run the CLI in Docker; raw input is read-only, reports cache-only.
"""
import argparse
from copy import deepcopy
import hashlib
import json
from pathlib import Path
import re
import struct
import sys

if __package__ in (None, ''):
    sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from assets.input import open_input
from assets.manifest import write_manifest
from assets.media_background_context import _code_only, _functions, build_background_contexts

_WORD = r'[A-Za-z_]\w*'
_LITERAL = r'(?:0[xX][0-9a-fA-F]+|0|[1-9][0-9]*)'
_TYPE = re.compile(r'typedef\s+struct(?:\s+' + _WORD + r')?\s*\{\s*void\s*\(\*(' + _WORD + r')\[(\d+)\]\)\(\)\s*;\s*}\s*(' + _WORD + r')\s*;')
_SYMBOL = re.compile(r'^\s*(' + _WORD + r')\s*=\s*(0x[0-9a-fA-F]+)\s*;', re.M)
_EXPECTED = {
    'mode_step_next': '''s32 *p;s32 z;game_state.mode_step+=1;game_state.mode_step_timer.w=0;
        z=0;p=&z;game_state.substep=*p;''',
    'dec_bg_cd_read': '''if(arg1==1){if(arg0==bg_lba_cache[2]){return 0;}
        if(arg0==bg_lba_cache[3]){return 1;}cd_read_sectors_nowait(0xA,0x80162000,arg0);
        *(s32*)((u8*)&bg_lba_cache[3]+-(D_800B5939*4))=arg0;return -1;}
        if(arg0==bg_lba_cache[0]){return 0;}if(arg0==bg_lba_cache[1]){return 1;}
        cd_read_sectors_nowait(0xA,0x80162000,arg0);
        *(s32*)((u8*)&bg_lba_cache[1]+-(dec_bg_show[0]*4))=arg0;return -1;''',
    'bg_read_sub2': '''s32 t; cd_read_checked=0; t=dec_bg_cd_read(arg0,1);
        if(t==D_800B5939){mode_step_next();mode_step_next();mode_step_next();}
        else if(t==1-D_800B5939){mode_step_next();mode_step_next();}''',
    'dec_init': '''if((((u32*)bg_still_bitstream)[0]&0xFFFF0000)!=0x38000000||(((u32*)bg_still_bitstream)[1]&0xFF010000)!=0x10000){
        bg_lba_cache[2]=0;bg_lba_cache[3]=0;mode_step_next();game_state.mode_step-=3;return 0;}
        dec_bg_decode_to_vram(1);mode_step_next();''',
    'change_dec_bg': 'dec_bg_show_switch(1);mode_step_next();',
    'scene_wait_cd_read': '''s32 pad;u8 sp2B;u8 sp2A;u8 sp29;
        if(cd_read_sync_status()&1){mode_step_next();return;}
        if((u32)game_state.mode_step_timer.w++>=0x401){cd_command_reset();sp29=D_800B5939;
        sp2A=*(u8*)&dec_bg_last_screen_mode;sp2B=bg_brightness;dec_bg_reset();
        *(u8*)&dec_bg_last_screen_mode=sp2A;bg_brightness=sp2B;D_800B5939=sp29;game_state.mode_step-=1;}''',
}
_SIGNATURES = {'mode_step_next': 's32 mode_step_next(void)',
               'dec_bg_cd_read': 's32 dec_bg_cd_read(s32 arg0,s32 arg1)',
               'bg_read_sub2': 'void bg_read_sub2(s32 arg0)', 'dec_init': 's32 dec_init(void)',
               'change_dec_bg': 'void change_dec_bg(void)', 'scene_wait_cd_read': 'void scene_wait_cd_read(void)'}


class StepProofError(ValueError):
    """A required original-source/table binding is not proved."""


def _compact(text):
    return re.sub(r'\s+', '', text)


def _binding_macros(text, names):
    return set(re.findall(r'^\s*#\s*define\s+(' + _WORD + r')\b', _code_only(text), re.M)) & set(names)


def _protocol(sources, legacy):
    evidence = []
    if any(_binding_macros(text, set(_EXPECTED) | {'game_state', 'mode_step'}) for text in sources.values()):
        raise StepProofError('preprocessor macro may shadow a protocol binding')
    for name, expected in _EXPECTED.items():
        definitions = [(path, code, fn) for path, text in sources.items()
                       for code in [_code_only(text)] for fn in _functions(code) if fn['name'] == name]
        if (len(definitions) != 1 or _compact(definitions[0][2]['body']) != _compact(expected)
                or _compact(definitions[0][1][definitions[0][2]['start']:definitions[0][2]['opening']])
                != _compact(_SIGNATURES[name])):
            raise StepProofError('helper body differs from strict protocol: ' + name)
        path, code, fn = definitions[0]
        evidence.append({'file': path, 'function': name, 'line': code.count('\n', 0, fn['start']) + 1,
                         'source_sha256': hashlib.sha256(sources[path].encode()).hexdigest()})
    geometry = legacy.get('layer_geometry', {}).get('1')
    reads = legacy.get('read_protocol_evidence', [])
    counts = {row['sectors'] for row in reads}
    if not geometry or len(counts) != 1 or not legacy.get('geometry_body_proved'):
        raise StepProofError('legacy decoder/read protocol does not prove unique layer1 geometry and sector count')
    return geometry, next(iter(counts)), evidence


def _config(text, overlay):
    code = re.sub(r'#[^\n]*', '', text)
    targets = re.findall(r'^\s*target_path:\s*(\S+)\s*$', code, re.M)
    bases = re.findall(r'^\s*(?:-\s*)?vram:\s*(0x[0-9a-fA-F]+)\s*$', code, re.M)
    hashes = re.findall(r'^sha1:\s*([0-9a-fA-F]{40})\s*$', code, re.M)
    paths = re.search(r'^\s*symbol_addrs_path:\s*\n((?:\s*-\s*\S+\s*\n)+)', code, re.M)
    target = 'disc/files/CDROM/EXEDIR/' + overlay + '.EXN'
    if targets != [target] or len(bases) != 1 or len(hashes) != 1 or paths is None:
        raise StepProofError('overlay config path/load base/hash/symbol list is not unique')
    files = re.findall(r'-\s*(\S+)', paths[1])
    if len(files) != len(set(files)):
        raise StepProofError('overlay config repeats a symbol file')
    return int(bases[0], 16), hashes[0].lower(), files


def _symbols(paths, symbol_files):
    names, addresses = {}, {}
    for path in paths:
        if path not in symbol_files:
            raise StepProofError('configured symbol file unavailable: ' + path)
        for name, value in _SYMBOL.findall(_code_only(symbol_files[path])):
            address = int(value, 16)
            names.setdefault(name, set()).add(address)
            addresses.setdefault(address, set()).add(name)
    return names, addresses


def _address(name, names, addresses):
    values = names.get(name, set())
    if len(values) != 1:
        raise StepProofError('symbol missing or conflicting: ' + name)
    address = next(iter(values))
    if addresses[address] != {name}:
        raise StepProofError('pointer aliases are ambiguous: ' + name)
    return address


def _definitions(header):
    code = _code_only(header)
    types = {}
    for field, count, name in _TYPE.findall(code):
        value = (field, int(count))
        if name in types:
            raise StepProofError('ambiguous callback type: ' + name)
        if field in {'f', 'callbacks'} and 4 <= value[1] <= 1024:
            types[name] = value
    definitions = {}
    for kind, table in re.findall(r'extern\s+(' + _WORD + r')\s+(' + _WORD + r')\s*;', code):
        if kind in types:
            if table in definitions:
                raise StepProofError('duplicate callback table declaration: ' + table)
            definitions[table] = (kind, *types[kind])
    return definitions


def _dispatcher(body, definitions):
    """Accept only declarations/copy/index/call, with no hidden control flow."""
    statements = [statement.strip() for statement in body.split(';') if statement.strip()]
    declarations, remaining = {}, []
    for statement in statements:
        match = re.fullmatch(r'(' + _WORD + r')\s+(' + _WORD + r')', statement)
        if match and match[1] not in {'return', 'goto'}:
            if match[2] in declarations:
                return None
            declarations[match[2]] = match[1]
        else:
            remaining.append(statement)
    if len(remaining) not in {2, 3} or 'game_state' in declarations:
        return None
    copy = re.fullmatch(r'(' + _WORD + r')\s*=\s*(' + _WORD + r')', remaining[0])
    if not copy or copy[2] not in definitions:
        return None
    local, table = copy[1], copy[2]
    kind, field, count = definitions[table]
    if table in declarations or declarations.get(local) != kind:
        return None
    index = r'game_state\s*\.\s*mode_step'
    if len(remaining) == 3:
        assignment = re.fullmatch(r'(' + _WORD + r')\s*=\s*game_state\s*\.\s*mode_step', remaining[1])
        if not assignment or declarations.get(assignment[1]) not in {'s32', 'u32', 'int'}:
            return None
        index = re.escape(assignment[1])
        if set(declarations) != {local, assignment[1]}:
            return None
    elif set(declarations) != {local}:
        return None
    call = re.fullmatch(re.escape(local) + r'\s*\.\s*' + re.escape(field) + r'\s*\[\s*' + index
                        + r'\s*\]\s*\(\s*(?:' + _LITERAL + r')?\s*\)', remaining[-1])
    return {'table': table, 'local': local, 'field': field, 'count': count, 'type': kind,
            'index_expression': 'game_state.mode_step'} if call else None


def _void_signature(code, function):
    return re.fullmatch(r'void\s+' + re.escape(function['name']) + r'\s*\(\s*(?:void)?\s*\)\s*',
                        code[function['start']:function['opening']]) is not None


def _attach(legacy, calls, inventory):
    result = deepcopy(legacy)
    streams = {(row['parent_asset_id'], row['source_offset']): row for row in result['streams']}
    extents = [(asset['source']['location'].get('lba'), asset) for asset in inventory['assets']
               if isinstance(asset.get('source', {}).get('location'), dict)
               and isinstance(asset['source'].get('size'), int) and asset['source']['size'] > 0]
    for call in calls:
        matches = [(lba, asset) for lba, asset in extents if isinstance(lba, int)
                   and lba <= call['lba'] < lba + (asset['source']['size'] + 2047) // 2048]
        if len(matches) != 1:
            result['unmapped_calls' if not matches else 'ambiguous_calls'].append(call)
            continue
        start, asset = matches[0]
        offset = (call['lba'] - start) * 2048
        key = (asset['id'], offset)
        stream = streams.setdefault(key, {'parent_asset_id': asset['id'], 'source': deepcopy(asset['source']),
                                         'source_offset': offset, 'lba': call['lba'], 'contexts': []})
        context = dict(call, width=call['shape_candidates'][0]['width'], height=call['shape_candidates'][0]['height'],
                       file_bytes_available=asset['source']['size'] - offset,
                       physical_read_bytes=call['read_sector_count'] * 2048,
                       physical_read_exceeds_logical_file=call['read_sector_count'] * 2048 > asset['source']['size'] - offset)
        stream['contexts'].append(context)
    for stream in streams.values():
        shapes = sorted({(row['width'], row['height']) for row in stream['contexts'] if row.get('width') is not None})
        unknown = any(not row['shape_candidates'] for row in stream['contexts'])
        stream['shape_candidates'] = [{'width': width, 'height': height} for width, height in shapes]
        stream['context_status'] = ('proved_single_shape_with_unresolved_uses' if len(shapes) == 1 and unknown else
                                    'proved_single_shape' if len(shapes) == 1 else
                                    'conflicting_shapes' if len(shapes) > 1 else 'shape_unresolved')
    result['streams'] = sorted(streams.values(), key=lambda row: (row['source']['path'], row['source_offset']))
    represented = {row['parent_asset_id'] for row in result['streams']}
    result['unresolved_bs_sources'] = [row for row in result['unresolved_bs_sources'] if row['parent_asset_id'] not in represented]
    result['counts'].update(streams=len(streams),
        proved_single_shape_streams=sum(row['context_status'] == 'proved_single_shape' for row in streams.values()),
        proved_shape_with_unresolved_uses=sum(row['context_status'] == 'proved_single_shape_with_unresolved_uses' for row in streams.values()),
        unresolved_bs_sources=len(result['unresolved_bs_sources']), unmapped_calls=len(result['unmapped_calls']),
        ambiguous_calls=len(result['ambiguous_calls']))
    return result


def build_background_step_contexts(inventory, sources, headers, configs, symbol_files, native_bytes):
    """Compose existing contexts with source/native-proved four-step sequences.

    Mappings use repository-relative paths; native_bytes keys are original
    inventory paths, not host extraction locations. No inputs are modified.
    """
    legacy = build_background_contexts(inventory, sources)
    calls, rejected, proofs = [], [], []
    try:
        geometry, sectors, protocol = _protocol(sources, legacy)
    except StepProofError as error:
        legacy['native_step_proof'] = {'status': 'unresolved', 'reason': str(error), 'calls': [], 'rejected': []}
        legacy['counts'].update(native_step_calls=0, native_step_rejections=0)
        return legacy
    assets = {asset['source']['path']: asset for asset in inventory['assets']}
    if len(assets) != len(inventory['assets']):
        raise StepProofError('duplicate original inventory paths cannot bind overlays')
    for config_path, config_text in sorted(configs.items()):
        overlay = Path(config_path).stem
        try:
            base, sha1, symbol_paths = _config(config_text, overlay)
            native_path = 'CDROM/EXEDIR/' + overlay + '.EXN'
            asset = assets.get(native_path)
            data = native_bytes.get(native_path)
            if (not asset or not isinstance(data, bytes) or len(data) != asset['source'].get('size')
                    or hashlib.sha256(data).hexdigest() != asset['source'].get('sha256')
                    or hashlib.sha1(data).hexdigest() != sha1):
                raise StepProofError('native overlay bytes do not match original inventory/config hash and size')
            header_path = 'include/ovl/' + overlay + '.h'
            if header_path not in headers:
                raise StepProofError('overlay header unavailable')
            definitions = _definitions(headers[header_path])
            label_path = 'config/labels/' + overlay + '.txt'
            active_symbols = symbol_paths + ([label_path] if label_path in symbol_files and label_path not in symbol_paths else [])
            names, addresses = _symbols(active_symbols, symbol_files)
            helpers = [_address(name, names, addresses) for name in ('scene_wait_cd_read', 'dec_init', 'change_dec_bg')]
            overlay_sources = {path: text for path, text in sources.items() if path.startswith('src/ovl/' + overlay + '/')}
            wrappers = {}
            functions = []
            for path, text in sorted(overlay_sources.items()):
                code = _code_only(text)
                for fn in _functions(code):
                    functions.append((path, code, fn))
                    match = re.fullmatch(r'\s*bg_read_sub2\(\s*(' + _LITERAL + r')\s*\);\s*mode_step_next\(\s*\);\s*', fn['body'])
                    if match and _void_signature(code, fn):
                        address = _address(fn['name'], names, addresses)
                        if address in wrappers or not base <= address < base + len(data):
                            raise StepProofError('wrapper pointer alias or address outside native overlay')
                        wrappers[address] = {'file': path, 'function': fn['name'], 'lba': int(match[1], 0),
                                             'line': code.count('\n', 0, fn['opening']) + 1}
            for path, code, fn in functions:
                if not _void_signature(code, fn):
                    continue
                dispatch = _dispatcher(fn['body'], definitions)
                if not dispatch:
                    if ('game_state.mode_step' in _compact(fn['body'])
                            and any(re.search(r'\b' + re.escape(table) + r'\b', fn['body']) for table in definitions)):
                        rejected.append({'overlay': overlay, 'file': path, 'function': fn['name'],
                                         'reason': 'dispatcher is outside strict unconditional declaration/copy/index/call grammar'})
                    continue
                table = dispatch['table']
                if any(_binding_macros(text, {table, dispatch['type'], dispatch['field'], 'game_state', 'mode_step',
                                              'bg_read_sub2', 'mode_step_next'})
                       for text in list(overlay_sources.values()) + [headers[header_path]]):
                    rejected.append({'overlay': overlay, 'file': path, 'function': fn['name'],
                                     'table': table, 'reason': 'macro may shadow dispatcher/table/helper bindings'})
                    continue
                # A pointer alias or assignment to the original table invalidates
                # a static native-table interpretation, regardless of function.
                if any(re.search(r'&\s*\b' + re.escape(table) + r'\b|\b' + re.escape(table)
                                 + r'\s*(?:\.|\[|=)', _code_only(text)) for text in overlay_sources.values()):
                    rejected.append({'overlay': overlay, 'file': path, 'function': fn['name'],
                                     'table': table, 'reason': 'table may be mutated or aliased'})
                    continue
                table_address = _address(table, names, addresses)
                dispatch_address = _address(fn['name'], names, addresses)
                offset, count = table_address - base, dispatch['count']
                if offset % 4 or not 0 <= offset <= len(data) - count * 4 or not base <= dispatch_address < base + len(data):
                    raise StepProofError('table/dispatcher pointer outside native overlay extent or alignment')
                pointers = struct.unpack_from('<%dI' % count, data, offset)
                for index, pointer in enumerate(pointers):
                    if pointer not in wrappers:
                        continue
                    if index + 4 > count or list(pointers[index + 1:index + 4]) != helpers:
                        rejected.append({'overlay': overlay, 'table': table, 'table_index': index,
                                         'reason': 'wrapper is not followed by exact wait/decode/show pointers'})
                        continue
                    wrapper = wrappers[pointer]
                    proof = dict(dispatch, dispatcher_file=path, dispatcher_function=fn['name'],
                                 dispatcher_line=code.count('\n', 0, fn['start']) + 1,
                                 dispatcher_address=dispatch_address, table_address=table_address,
                                 table_source_offset=offset, table_index=index, native_pointer_sequence=list(pointers[index:index + 4]),
                                 native_path=native_path, native_sha256=hashlib.sha256(data).hexdigest(),
                                 native_sha1=sha1, native_size=len(data), native_load_base=base,
                                 config_path=config_path, config_sha256=hashlib.sha256(config_text.encode()).hexdigest(),
                                 header_path=header_path, header_sha256=hashlib.sha256(headers[header_path].encode()).hexdigest(),
                                 symbol_source_hashes={p: hashlib.sha256(symbol_files[p].encode()).hexdigest() for p in active_symbols})
                    proofs.append(proof)
                    calls.append(dict(wrapper, symbol='bg_read_sub2', layer=1,
                                      read_sector_count=sectors, read_protocol_evidence=legacy['read_protocol_evidence'],
                                      geometry_binding='source_dispatch_and_native_adjacent_callback_sequence',
                                      decoder_evidence=protocol + [row for row in legacy['geometry_evidence'] if row['layer'] == 1],
                                      shape_candidates=[geometry], native_step_evidence=proof))
        except (StepProofError, struct.error) as error:
            rejected.append({'overlay': overlay, 'config_path': config_path, 'reason': str(error)})
    report = _attach(legacy, calls, inventory)
    report['native_step_proof'] = {'status': 'bounded_source_and_native_proof', 'calls': calls,
                                   'proofs': proofs, 'rejected': rejected, 'protocol_evidence': protocol,
                                   'limitations': ['strict unconditional local-copy dispatchers only; other dispatch/control flow unresolved',
                                                  'table count comes from one-array header typedef; runtime index range is not globally proved',
                                                  'helper cached-screen skips and read retries follow the exact source protocol; no runtime execution claimed',
                                                  'native pointer order proves this callback sequence only, not file ownership or hardware pixels']}
    report['counts']['native_step_calls'] = len(calls)
    report['counts']['native_step_rejections'] = len(rejected)
    return report


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--inventory', required=True)
    parser.add_argument('--input', required=True)
    parser.add_argument('--repo-root', default='.')
    parser.add_argument('--output', required=True)
    args = parser.parse_args(argv)
    try:
        root = Path(args.repo_root)
        inventory = json.loads(Path(args.inventory).read_text())
        sources = {p.relative_to(root).as_posix(): p.read_text() for p in sorted((root / 'src').rglob('*.c'))}
        headers = {p.relative_to(root).as_posix(): p.read_text() for p in sorted((root / 'include/ovl').glob('*.h'))}
        configs = {p.relative_to(root).as_posix(): p.read_text() for p in sorted((root / 'config/overlays').glob('*.yaml'))}
        symbols = {p.relative_to(root).as_posix(): p.read_text() for p in sorted((root / 'config').rglob('*.txt'))}
        native = {}
        with open_input(args.input) as source:
            entries = {entry['path']: entry for entry in source.entries()}
            for asset in inventory['assets']:
                path = asset['source']['path']
                if path.startswith('CDROM/EXEDIR/') and path.endswith('.EXN') and path in entries:
                    native[path] = source.read(entries[path])
        report = build_background_step_contexts(inventory, sources, headers, configs, symbols, native)
        write_manifest(args.output, report)
        print(json.dumps(report['counts'], sort_keys=True))
        return 0
    except (OSError, ValueError) as error:
        print('media_background_steps.py: ' + str(error), file=sys.stderr)
        return 1


if __name__ == '__main__':
    raise SystemExit(main())
