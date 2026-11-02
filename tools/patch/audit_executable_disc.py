"""Read back English executable text and derived table pointers (T-39510).

Run from the repository root in Docker. Link metadata must match the frozen
release before translating original addresses. Mapped slots, derived views and
source-only literal payloads are checked against actual disc bytes. Unresolved
compiled extents or unclassified referenced text make the CLI exit 2. Invalid physical records or
pointers fail immediately. Reports contain metadata only, not extracted game text.
"""
import argparse
import bisect
import hashlib
import json
import struct
from pathlib import Path

from tools import elf32, english_link
from tools.patch.art.verify import read_back
from tools.patch.audit_generated_sources import audit_sources
from tools.patch.build_english import literal_payloads, unloaded_sources
from tools.patch.executable_calls import local_regions, named_calls
from tools.patch.text import c_literals
from tools.patch.text.containers import EXE_PATH, build_containers, load_units
from tools.patch.text.layout import english_payloads
from tools.patch.text.translations import load_jsonl, load_targets
from tools.patch.text.wrap import FontMetrics
from tools.patch.text.retarget import References


def read_address(data, base, address, size):
    """Reject negative slicing, truncation and unmapped addresses explicitly."""
    if address is None or size < 0 or address < base or address + size > base + len(data):
        raise ValueError('Executable read outside file')
    return data[address - base:address - base + size]


def matching_payload(data, base, address, payload):
    if address is None or address < base or address + len(payload) > base + len(data):
        return False
    return read_address(data, base, address, len(payload)) == payload


def snapshot_translator(unit, build, link):
    """Use the original link path: copied scripts still name live objects."""
    stem = 'SLPM_86.053' if unit == 'main' else 'ovl/' + unit
    for suffix in ('.elf', '.map', '.ld'):
        relative = stem + suffix
        if (build / relative).read_bytes() != (link / relative).read_bytes():
            raise ValueError('Live link differs from frozen release: ' + relative)
    # redirected_objects matches literal link-directory prefixes, so an absolute
    # path or renamed snapshot path silently loses the redirect information.
    if link.is_absolute():
        raise ValueError('Link directory must use its repository-relative build path')
    objects = {obj for obj, section in english_link.read_map(str(link / (stem + '.map')))}
    for obj in sorted(objects):
        path = Path(obj)
        if path.is_relative_to(link):
            frozen = build / path.relative_to(link)
            if not frozen.is_file() or frozen.read_bytes() != path.read_bytes():
                raise ValueError('Live object differs from frozen release: ' + obj)
    return english_link.translator(unit, str(link))


def pair_target(data, base, highs, low):
    """Decode actual MIPS instructions, including signed carry and ORI lows."""
    if not highs or low % 4 or any(site % 4 for site in highs):
        raise ValueError('Missing or unaligned MIPS address pair')
    low_word = struct.unpack('<I', read_address(data, base, low, 4))[0]
    opcode = low_word >> 26
    if opcode not in (8, 9, 13) and not 32 <= opcode <= 46:
        raise ValueError('Unsupported low instruction in MIPS address pair')
    low_value = low_word & 0xFFFF
    if opcode != 13 and low_value & 0x8000:
        low_value -= 0x10000
    targets = set()
    for site in highs:
        word = struct.unpack('<I', read_address(data, base, site, 4))[0]
        if word >> 26 != 15:
            raise ValueError(f'High instruction at {site:#x} is not LUI ({word:#x})')
        targets.add((((word & 0xFFFF) << 16) + low_value) & 0xFFFFFFFF)
    if len(targets) != 1:
        raise ValueError('Shared MIPS highs disagree')
    return targets.pop()


class FunctionOwners:
    """Limit nearest-function lookup to the containing input text section."""
    def __init__(self, references):
        self.functions = sorted((value[0], name) for name, value in references.syms.items()
                                if value[1] and value[2])
        self.sections = [(start, start + size) for (_, section), (start, size)
                         in references.placed.items() if section == '.text' and size]

    def at(self, address):
        index = bisect.bisect_right(self.functions, (address, chr(0x10FFFF))) - 1
        if index < 0:
            return None
        start, name = self.functions[index]
        return name if any(begin <= start <= address < end for begin, end in self.sections) else None


def verify_hooks(data, base, symbols, hooks):
    """Require the actual disc's entry jump and delay slot, not just config."""
    result = {}
    for original, replacement in sorted(hooks.items()):
        if original not in symbols or replacement not in symbols:
            raise ValueError('Missing hook symbol: ' + original)
        source, target = symbols[original][0], symbols[replacement][0]
        jump, delay = struct.unpack('<II', read_address(data, base, source, 8))
        actual = ((source + 4) & 0xF0000000) | ((jump & 0x03FFFFFF) << 2)
        if jump >> 26 != 2 or actual != target or delay != 0:
            raise ValueError('Disc entry hook differs: ' + original)
        result[original] = {'replacement': replacement, 'site': source, 'target': target}
    return result


def payload_references(data, base, references, payloads, accepts):
    """Require every specified English payload in the selected compiled region."""
    found = {label: [] for label in payloads}
    for pairs in references.pairs.values():
        for highs, low in pairs:
            if not accepts(low):
                continue
            target = pair_target(data, base, highs, low)
            for label, payload in payloads.items():
                if matching_payload(data, base, target, payload + b'\0'):
                    found[label].append({'site': low, 'target': target})
    missing = [label for label, hits in found.items() if not hits]
    if missing:
        raise ValueError('Missing replacement payload references: ' + ', '.join(missing))
    return found


def replacement_helpers(files, bases, link, references):
    """Reviewed replacements of the six obsolete raw extents (T-39506)."""
    main = references[EXE_PATH]
    data, base = files[EXE_PATH], bases[EXE_PATH]
    obj = str(link / 'src/main/hud_screens.o')
    start, size = main.placed[(obj, '.text')]
    end = main.syms['hud_day_bgm_load'][0]
    # IDO omits the static helper's name. It is the prefix of this object,
    # before its first exported function; calls use section-symbol relocations.
    if not start < end <= start + size:
        raise ValueError('Invalid save-helper prefix bounds')
    owners = FunctionOwners(main)
    elf = elf32.Elf(obj)
    section = next(section for section in elf.sections if section.name == '.text')
    calls = []
    for offset, kind, symbol in elf.relocations(section):
        if kind != elf32.R_MIPS_26 or symbol.type != 3:
            continue
        if elf.sections[symbol.shndx].name != '.text':
            continue
        original = struct.unpack_from('<I', elf.contents(section), offset)[0]
        if original & 0x03FFFFFF:
            continue
        site = start + offset
        word = struct.unpack('<I', read_address(data, base, site, 4))[0]
        target = ((site + 4) & 0xF0000000) | ((word & 0x03FFFFFF) << 2)
        if word >> 26 != 3 or target != start or owners.at(site) != 'hud_save_slot_info_show':
            raise ValueError('Save helper call is not bound to expected caller/target')
        calls.append(site)
    if len(calls) != 2:
        raise ValueError('Expected both save/album calls to name helper')
    labels = payload_references(data, base, main, {'name': b'Name: ', 'nickname': b'Nickname: '},
                                lambda site: start <= site < end)
    name_path = 'CDROM/EXEDIR/NAME_ENT.EXN'
    name = references[name_path]
    owners = FunctionOwners(name)
    prompts = payload_references(
        files[name_path], bases[name_path], name,
        {'checking': b'Checking the Memory Card.', 'wait': b'Please wait.'},
        lambda site: owners.at(site) == 'file_select_memory_card_check_begin')
    save = {'status': 'replacement_helper_bound', 'helper_start': start, 'helper_end': end,
            'calls': calls, 'payload_references': labels}
    memory = {'status': 'replacement_helper_bound',
              'function': 'file_select_memory_card_check_begin', 'payload_references': prompts}
    return {
        'SLPM_86.053:000707c8:739d00f04fcf': save,
        'SLPM_86.053:000707d0:8d933ffd7b5c': save,
        'SLPM_86.053:00070824:739d00f04fcf': save,
        'SLPM_86.053:0007082c:8d933ffd7b5c': save,
        name_path + ':000197b4:5689431a4a51': memory,
        name_path + ':000197c8:14eb1689205d': memory,
    }


def retired_parser_format(candidates, binding, data, base, references, regions):
    """Reviewed ASCII formats replaced by positioning/append, not displayed text.

    The old speaker format and blank glyph were replaced by separate label/mark
    drawing. The old concatenation template became bounded english_append.
    Require the actual hook and replacement call paths before retiring a record.
    Runtime/GPU behavior is covered separately; this is a function-level proof.
    """
    if len(candidates) != 1 or not binding.get('replacement_functions'):
        return None
    payload = next(iter(candidates))
    replacements = binding['replacement_functions']
    if {item['function'] for item in binding.get('original_references', [])} != set(replacements):
        return None
    ranges, edges = regions
    symbols = references.syms
    if payload in (b'%s ', b' ') and replacements == {'talk_speak_part': 'english_talk_speak_part'}:
        caller = symbols['english_talk_speak_part'][0]
        helpers = []
        for target in sorted({edge['target'] for edge in edges if edge['caller'] == caller}):
            calls = named_calls(data, base, target, ranges[target], symbols,
                                ('set_kanji_string', 'english_text_width'))
            if len(calls['set_kanji_string']) == 2 and len(calls['english_text_width']) == 2:
                helpers.append({'start': target, 'calls': calls,
                                'incoming_calls': [edge for edge in edges
                                                   if edge['caller'] == caller and edge['target'] == target]})
        if len(helpers) != 1:
            raise ValueError('Cannot identify compiled separate speaker/mark helper')
        return {'status': 'retired_parser_format', 'replacement_functions': replacements,
                'basis': 'separate speaker/mark drawing replaces format and blank glyph',
                'helper': helpers[0]}
    if payload == b'%s%s' and replacements == {'talk_render': 'english_talk_render'}:
        start = symbols['english_talk_render'][0]
        calls = named_calls(data, base, start, ranges[start], symbols, ('english_append',))
        if not calls['english_append']:
            raise ValueError('Renderer lacks compiled bounded suffix append')
        return {'status': 'retired_parser_format', 'replacement_functions': replacements,
                'basis': 'bounded append replaces concatenation format', 'calls': calls}
    return None


def bind_extent(record, container, candidates, data, base, address, old, new, hooks=None,
                helper_regions=None):
    """Require matching references in every original owning function.

    This is function-level evidence combined with source-generation equality,
    not instruction-for-instruction correspondence after recompilation.
    """
    if len(candidates) != 1:
        return {'status': 'pending', 'reason': 'missing or ambiguous canonical C payload'}
    expected = next(iter(candidates)) + b'\0'
    if matching_payload(data, base, address, expected):
        return {'status': 'canonical_exact', 'address': address}
    original = container.image.base + record['source_offset']
    end = original + len(bytes.fromhex(record['raw_hex']))
    old_owners, new_owners = FunctionOwners(old), FunctionOwners(new)
    references = []
    for target, pairs in old.pairs.items():
        if original <= target < end:
            references.extend({'site': low, 'function': old_owners.at(low),
                               'addend': target - original, 'kind': 'hi_lo'}
                              for highs, low in pairs)
    for target, words in old.words.items():
        if original <= target < end:
            references.extend({'site': site, 'function': None,
                               'addend': target - original, 'kind': 'pointer'} for site in words)
    if not references or any(not item['function'] or item['addend'] for item in references):
        return {'status': 'pending', 'reason': 'unresolved original consumer',
                'original_references': references}
    wanted = {item['function'] for item in references}
    redirected = wanted & set(hooks or {})
    replacements = {name: hooks[name] for name in sorted(redirected)}
    if redirected:
        verify_hooks(data, base, new.syms, replacements)
        wanted = {replacements.get(name, name) for name in wanted}
    matches, stale_pairs = [], []
    for pairs in new.pairs.values():
        for highs, low in pairs:
            function = new_owners.at(low)
            if function not in wanted:
                continue
            try:
                target = pair_target(data, base, highs, low)
            except ValueError as error:
                # Post-link hooks can replace old instruction pairs. They are
                # not evidence: retain the rejection and require a valid pair.
                stale_pairs.append({'function': function, 'site': low, 'reason': str(error)})
                continue
            if matching_payload(data, base, target, expected):
                matches.append({'function': function, 'site': low, 'target': target})
    missing = sorted(wanted - {item['function'] for item in matches})
    if missing and helper_regions:
        ranges, edges = helper_regions
        for function in list(missing):
            start = new.syms[function][0]
            calls = [edge for edge in edges if edge['caller'] == start]
            for call in calls:
                begin, end = call['target'], ranges[call['target']]
                for pairs in new.pairs.values():
                    for highs, low in pairs:
                        if not begin <= low < end:
                            continue
                        target = pair_target(data, base, highs, low)
                        if matching_payload(data, base, target, expected):
                            matches.append({'function': function, 'site': low, 'target': target,
                                            'helper_call': call})
            if function in {item['function'] for item in matches}:
                missing.remove(function)
    return {'status': ('pending' if missing else
                       'replacement_function_bound' if redirected else 'function_bound'),
            'original_references': references, 'english_function_references': matches,
            'unresolved_functions': missing, 'rejected_pairs': stale_pairs,
            'replacement_functions': replacements}


def audit_slots(container, data, base, translate, payloads, placements):
    results = {}
    for rid, payload in sorted(payloads.items()):
        record = container.records[rid]
        slot = container.image.slots.get(rid)
        if record.kind == 'source_literal' or slot is None or slot.mode != 'in_place':
            continue
        moved = placements['retargeted'].get(rid)
        address = moved['to'] if moved else translate(container.image.base + slot.offset)
        exact = matching_payload(data, base, address, payload + slot.terminator)
        results[rid] = {'kind': record.kind, 'address': address, 'exact': exact,
                        'retargeted': bool(moved)}
    return results


def audit_derived(record, data, base, translate, payloads, slots):
    """Check both physical storage and every recorded consumer pointer."""
    context = record['context']
    physical = context.get('source_inventory_record_ids', [])
    if not physical:
        raise ValueError('Derived record has no physical source: ' + record['id'])
    for rid in physical:
        if rid not in payloads or rid not in slots or not slots[rid]['exact']:
            raise ValueError('Derived physical source is unverified: ' + rid)
    result = {'id': record['id'], 'kind': record['kind'],
              'physical_ids': physical, 'pointers': []}
    if record['kind'] == 'initial_runtime_buffer':
        return result
    if record['kind'] != 'stage_text_record':
        raise ValueError('Unsupported derived record: ' + record['id'])
    stage = context['stage_context']
    uses = stage.get('uses', [])
    if not uses:
        raise ValueError('Stage record has no consumers: ' + record['id'])
    for use in uses:
        evidence = use.get('pointer_evidence')
        if not evidence or 'address' not in evidence:
            raise ValueError('Stage consumer has no pointer evidence: ' + record['id'])
        site = translate(evidence['address'])
        if site is None or site % 4:
            raise ValueError('Unmapped or unaligned stage pointer: ' + record['id'])
        target = struct.unpack('<I', read_address(data, base, site, 4))[0]
        matched = [rid for rid in physical
                   if matching_payload(data, base, target, payloads[rid] + b'\0')]
        if not matched:
            raise ValueError('Stage pointer does not reach expected payload: ' + record['id'])
        result['pointers'].append({'site': site, 'target': target, 'matched_ids': matched,
                                   'body_skip': stage.get('body_relative_offset', 0)})
    return result


def referenced_candidates(data, base, references, hooks):
    """Scan NUL-terminated targets of actual R32 and HI/LO references.

    This detects printable Shift-JIS text containing Japanese punctuation,
    kana or ideographs. It does not discover computed pointers or image text.
    Entry-hook instructions are excluded only after hook verification.
    """
    targets = {}
    excluded = []
    replaced = {site for hook in hooks.values()
                for site in (hook['site'], hook['site'] + 4)}
    for sites in references.words.values():
        for site in sites:
            target = struct.unpack('<I', read_address(data, base, site, 4))[0]
            targets.setdefault(target, set()).add(site)
    for pairs in references.pairs.values():
        for highs, low in pairs:
            if not highs:
                raise ValueError('Missing high instruction in census address pair')
            for high in highs:
                if high in replaced or low in replaced:
                    excluded.append({'high': high, 'low': low, 'reason': 'verified entry hook'})
                    continue
                # A shared low may serve different high halves on separate paths.
                target = pair_target(data, base, [high], low)
                targets.setdefault(target, set()).add(low)
    candidates = []
    for target, sites in sorted(targets.items()):
        offset = target - base
        if not 0 <= offset < len(data):
            continue
        end = data.find(b'\0', offset)
        if end < 0:
            continue
        raw = data[offset:end]
        try:
            decoded = raw.decode('shift_jis')
        except UnicodeDecodeError:
            continue
        if any(ord(char) < 32 and char not in '\n\r\t' for char in decoded):
            continue
        if not any('\u3000' <= char <= '\u30ff' or '\u3400' <= char <= '\u9fff'
                   or '\uff01' <= char <= '\uff60' for char in decoded):
            continue
        candidates.append({'address': target, 'sites': sorted(sites),
                           'bytes_sha256': hashlib.sha256(raw).hexdigest()})
    return {'targets': len(targets), 'excluded_pairs': excluded, 'candidates': candidates}


def audit_language_census(files, bases, translations, units, report, link):
    reviewed = json.loads(Path('translations/en/reviews/executable-binding-2026-11-02.json')
                          .read_text())['candidate_classification']
    numeric = {(row['unit'], row['symbol']): row for row in reviewed
               if row['classification'] == 'numeric_data'}
    code = next(row for row in reviewed if row['classification'] == 'code_pointer')
    by_id = {record['id']: record for record in units}
    keyboard_path = 'CDROM/EXEDIR/NAME_ENT.EXN'
    keyboard = {}
    for row in report['records']['records']['kept_original']:
        if row['reason'] != 'name entry kana page (T-39506)':
            continue
        record = by_id[row['id']]
        if record['source_path'] != keyboard_path:
            raise ValueError('Keyboard exception outside NAME_ENT')
        address = translations[keyboard_path](english_link.OVERLAY_SLOT + record['source_offset'])
        keyboard[address] = record
    results, unexplained = {}, []
    for path, data in files.items():
        unit = 'main' if path == EXE_PATH else Path(path).stem
        stem = 'SLPM_86.053' if unit == 'main' else 'ovl/' + unit
        refs = References(str(link / (stem + '.map')), str(link / (stem + '.elf')))
        hooks = verify_hooks(data, bases[path], refs.syms,
                             dict(english_link.read_hooks('config/english_hooks.txt', unit)))
        scan = referenced_candidates(data, bases[path], refs, hooks)
        owners = FunctionOwners(refs)
        for row in scan['candidates']:
            address = row['address']
            if unit == 'NAME_ENT' and address in keyboard:
                record = keyboard[address]
                expected = bytes.fromhex(record['raw_hex']).rstrip(b'\0')
                if row['bytes_sha256'] == hashlib.sha256(expected).hexdigest():
                    row.update(classification='approved_keyboard', record_id=record['id'])
            for name, value in refs.syms.items():
                reviewed_row = numeric.get((unit, name.removesuffix('.NON_MATCHING')))
                if value[0] == address and reviewed_row and row['bytes_sha256'] == reviewed_row['bytes_sha256']:
                    row.update(classification='numeric_data', symbol=reviewed_row['symbol'])
            if (unit == 'main' and owners.at(address) == 'scene_bg_wipe_in_step'
                    and address == refs.syms['scene_bg_wipe_in_step'][0] + 532
                    and row['bytes_sha256'] == code['bytes_sha256']
                    and all(any(section == '.rodata' and start <= site < start + size
                                for (_, section), (start, size) in refs.placed.items())
                            for site in row['sites'])):
                row['classification'] = 'code_pointer'
            if 'classification' not in row:
                unexplained.append({'unit': unit, **row})
        results[unit] = scan
    return {'scope': 'NUL-terminated Shift-JIS candidates at R32 and HI/LO targets',
            'units': results, 'unexplained': unexplained}


def audit_source_only(records, payloads, files):
    """Check literal presence by link unit; this does not establish call-site binding."""
    checked, excluded = {}, []
    for rid, record in records.items():
        unit = record.unit['context']['link_unit']
        if unit in english_link.UNLOADED:
            excluded.append(rid)
            continue
        path = EXE_PATH if unit == 'main' else 'CDROM/EXEDIR/' + unit + '.EXN'
        if path not in files:
            raise ValueError('Source-only literal has no audited executable: ' + rid)
        if rid not in payloads:
            raise ValueError('Missing source-only target: ' + rid)
        expected = payloads[rid] + b'\0'
        offset = files[path].find(expected)
        if offset < 0:
            raise ValueError('Source-only target absent from its executable: ' + rid)
        checked[rid] = {'path': path, 'offset': offset,
                        'sha256': hashlib.sha256(expected).hexdigest()}
    return {'scope': 'payload presence in the owning executable; not call-site binding',
            'checked': len(checked), 'records': checked, 'excluded_unloaded': excluded}


def audit(build, link):
    units = load_units('.asset-cache/text/units/units.jsonl')
    supplemental = 'translations/en/supplemental.jsonl'
    containers = build_containers('disc/files', units, load_jsonl(supplemental))
    targets = load_targets('translations/en/returned-v3.jsonl', supplemental)
    text = english_payloads(containers, targets, FontMetrics.load(build / 'font_metrics.json'))
    source = audit_sources('.', c_literals.locate_sites('.', units),
                           literal_payloads(containers, text), unloaded_sources(), [link, build])
    paths = sorted(path for path, container in containers.items()
                   if container.kind in ('main_executable', 'overlay')
                   and Path(path).stem not in english_link.UNLOADED)
    if not paths:
        raise ValueError('No executable containers to audit')
    files = read_back(build / 'disc/track1.bin', paths)
    source_only = audit_source_only(containers['(source only)'].records,
                                    text.payloads.get('(source only)', {}), files)
    report = json.loads((build / 'disc/report.json').read_text())
    translations, bases, slots = {}, {}, {}
    for path in paths:
        unit = 'main' if path == EXE_PATH else Path(path).stem
        translations[path] = snapshot_translator(unit, build, link)
        bases[path] = (english_link.EXE_VRAM - english_link.EXE_HEADER
                       if unit == 'main' else english_link.OVERLAY_SLOT)
        slots[path] = audit_slots(containers[path], files[path], bases[path],
                                 translations[path], text.payloads.get(path, {}),
                                 report['binaries']['in_place'][path])
    derived = []
    for record in units:
        if record['kind'] not in ('stage_text_record', 'initial_runtime_buffer'):
            continue
        path = record['source_path']
        if path not in files:
            raise ValueError('Derived source outside audited executable set: ' + record['id'])
        derived.append(audit_derived(record, files[path], bases[path], translations[path],
                                     text.payloads.get(path, {}), slots[path]))
    by_id = {record['id']: record for record in units}
    canonical = {}
    for record in units:
        if record['kind'] == 'source_literal':
            key = (record['context'].get('link_unit'),
                   bytes.fromhex(record.get('raw_hex') or '').rstrip(b'\0'))
            canonical.setdefault(key, []).append(record['id'])
    payloads = {rid: payload for rows in text.payloads.values() for rid, payload in rows.items()}
    bindings, pending, hook_checks, linked_references = {}, [], {}, {}
    for path, rows in slots.items():
        unresolved = {rid: item for rid, item in rows.items() if not item['exact']}
        if not unresolved:
            continue
        unit = 'main' if path == EXE_PATH else Path(path).stem
        stem = 'SLPM_86.053' if unit == 'main' else 'ovl/' + unit
        _, old_map, old_elf = english_link.default_paths(unit)
        old = References(old_map, old_elf)
        new = References(str(link / (stem + '.map')), str(link / (stem + '.elf')))
        linked_references[path] = new
        hooks = dict(english_link.read_hooks('config/english_hooks.txt', unit))
        hook_checks[unit] = verify_hooks(files[path], bases[path], new.syms, hooks)
        helpers = None
        if unit == 'main':
            obj = str(link / 'src/english/script.o')
            helpers = local_regions(obj, new.placed[(obj, '.text')][0], files[path], bases[path])
        for rid, item in unresolved.items():
            record = by_id[rid]
            key = (unit, bytes.fromhex(record.get('raw_hex') or '').rstrip(b'\0'))
            candidates = {payloads[cid] for cid in canonical.get(key, []) if cid in payloads}
            binding = bind_extent(record, containers[path], candidates, files[path], bases[path],
                                  item['address'], old, new, hooks, helpers)
            if binding['status'] == 'pending' and helpers:
                retired = retired_parser_format(candidates, binding, files[path], bases[path], new, helpers)
                if retired is not None:
                    binding = retired
            bindings[rid] = binding
            if binding['status'] == 'pending':
                pending.append({'path': path, 'id': rid})
    helpers = replacement_helpers(files, bases, link, linked_references)
    for rid, evidence in helpers.items():
        if rid not in bindings or bindings[rid]['status'] != 'pending':
            raise ValueError('Replacement recipe does not match a pending extent: ' + rid)
        bindings[rid] = evidence
    pending = [item for item in pending if bindings[item['id']]['status'] == 'pending']
    census = audit_language_census(files, bases, translations, units, report, link)
    remaining = (['compiled extent bindings'] if pending else [])
    if census['unexplained']:
        remaining.append('unclassified referenced bytes')
    return {'status': 'incomplete' if remaining else 'pass',
            'scope': 'source generation, source-only payload presence, mapped executable slots, derived pointers and referenced-byte census',
            'remaining_checks': remaining, 'language_census': census,
            'excluded_units': list(english_link.UNLOADED), 'source_generation': source,
            'source_only_literals': source_only,
            'containers': len(files), 'slots': sum(len(rows) for rows in slots.values()),
            'exact_slots': sum(item['exact'] for rows in slots.values() for item in rows.values()),
            'pending_slots': pending, 'extent_bindings': bindings, 'entry_hooks': hook_checks,
            'derived_records': len(derived),
            'derived_pointers': sum(len(item['pointers']) for item in derived),
            'files': {path: {'sha256': hashlib.sha256(files[path]).hexdigest(), 'slots': slots[path]}
                      for path in paths}, 'derived': derived}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build', type=Path, required=True)
    parser.add_argument('--link', type=Path, default=Path('build/en'))
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.build, args.link)
    args.out.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print(f"Exact slots {result['exact_slots']}/{result['slots']}; "
          f"derived records {result['derived_records']}, pointers {result['derived_pointers']}; "
          f"pending slots {len(result['pending_slots'])}; audit {result['status']}")
    raise SystemExit(0 if result['status'] == 'pass' else 2)
