"""Read back English executable text and derived table pointers (T-39510).

Run from the repository root in Docker. Link metadata must match the frozen
release before translating original addresses. Exact mapped slots and derived
views are checked against actual disc bytes. Recompiled C extents still needing
function-reference proof remain explicitly pending: the CLI exits 2, never a
false success. Invalid physical records or pointers fail immediately. Reports
contain metadata only, not extracted game text.
"""
import argparse
import bisect
import hashlib
import json
import struct
from pathlib import Path

from tools import english_link
from tools.patch.art.verify import read_back
from tools.patch.audit_generated_sources import audit_sources
from tools.patch.build_english import literal_payloads, unloaded_sources
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


def bind_extent(record, container, candidates, data, base, address, old, new, hooks=None):
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
    if redirected:
        return {'status': 'pending', 'reason': 'original consumer replaced by entry hook',
                'original_references': references,
                'replacement_functions': {name: hooks[name] for name in sorted(redirected)}}
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
    return {'status': 'pending' if missing else 'function_bound',
            'original_references': references, 'english_function_references': matches,
            'unresolved_functions': missing, 'rejected_pairs': stale_pairs}


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
    bindings, pending = {}, []
    for path, rows in slots.items():
        unresolved = {rid: item for rid, item in rows.items() if not item['exact']}
        if not unresolved:
            continue
        unit = 'main' if path == EXE_PATH else Path(path).stem
        stem = 'SLPM_86.053' if unit == 'main' else 'ovl/' + unit
        _, old_map, old_elf = english_link.default_paths(unit)
        old = References(old_map, old_elf)
        new = References(str(link / (stem + '.map')), str(link / (stem + '.elf')))
        hooks = dict(english_link.read_hooks('config/english_hooks.txt', unit))
        for rid, item in unresolved.items():
            record = by_id[rid]
            key = (unit, bytes.fromhex(record.get('raw_hex') or '').rstrip(b'\0'))
            candidates = {payloads[cid] for cid in canonical.get(key, []) if cid in payloads}
            binding = bind_extent(record, containers[path], candidates, files[path], bases[path],
                                  item['address'], old, new, hooks)
            bindings[rid] = binding
            if binding['status'] == 'pending':
                pending.append({'path': path, 'id': rid})
    return {'status': 'incomplete',
            'scope': 'source generation, mapped executable slots and derived consumer pointers',
            'remaining_checks': ['entry-hook replacements', 'replacement helper references',
                                 'referenced-byte language census'],
            'excluded_units': list(english_link.UNLOADED), 'source_generation': source,
            'containers': len(files), 'slots': sum(len(rows) for rows in slots.values()),
            'exact_slots': sum(item['exact'] for rows in slots.values() for item in rows.values()),
            'pending_slots': pending, 'extent_bindings': bindings, 'derived_records': len(derived),
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
          f"pending slots {len(result['pending_slots'])}; audit incomplete")
    raise SystemExit(2)
