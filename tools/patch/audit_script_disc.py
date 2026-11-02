"""Audit every active talk-script string read back from a built disc (T-39510).

Follows actual on-disc pointer words, compares their strings with the encoded
translation records, and rejects original-font bytes. Covers talk containers,
including supplemental and fixed-table data and the relocated profile table.
Executable literals and artwork are explicitly excluded and audited separately.
Run in Docker. Reports contain counts/hashes/record IDs, never game text.
"""
import argparse
import hashlib
import json
import struct
import copy
from dataclasses import replace
from pathlib import Path

from tools.patch.art.verify import read_back
from tools.patch.text.containers import build_containers, load_units
from tools.patch.text.layout import english_payloads
from tools.patch.text import profile
from tools.patch.text.translations import load_jsonl, load_targets
from tools.patch.text.wrap import FontMetrics


def audit_image(image, data, payloads):
    checked = set()
    for rid, slot in image.slots.items():
        if slot.mode != 'in_place':
            continue
        expected = payloads.get(rid, slot.payload)
        end = slot.offset + len(expected) + len(slot.terminator)
        if slot.offset < 0 or end > len(data):
            raise ValueError('Fixed string outside file: ' + rid)
        if data[slot.offset:end] != expected + slot.terminator:
            raise ValueError('On-disc string differs from translation: ' + rid)
        if any(0x80 <= byte < 0xA0 or byte >= 0xE0 for byte in expected):
            raise ValueError('Original-font byte in active script: ' + rid)
        checked.add(rid)
    for pointer in image.pointers:
        if pointer.offset < 0 or pointer.offset + 4 > len(data):
            raise ValueError('Pointer outside file: ' + pointer.slot)
        offset = struct.unpack_from('<I', data, pointer.offset)[0] - image.base
        if offset < 0 or offset >= len(data):
            raise ValueError('String pointer outside file: ' + pointer.slot)
        end = data.find(b'\0', offset)
        if end < 0:
            raise ValueError('Unterminated string: ' + pointer.slot)
        actual = data[offset:end]
        expected = payloads.get(pointer.slot, image.slots[pointer.slot].payload)[pointer.addend:]
        if actual != expected:
            raise ValueError('On-disc string differs from translation: ' + pointer.slot)
        if any(0x80 <= byte < 0xA0 or byte >= 0xE0 for byte in actual):
            raise ValueError('Original-font byte in active script: ' + pointer.slot)
        checked.add(pointer.slot)
    missing = set(image.slots) - checked
    if missing:
        raise ValueError('Unverified script slots: ' + ', '.join(sorted(missing)[:5]))
    return {'strings': len(checked), 'pointers': len(image.pointers),
            'sha256': hashlib.sha256(data).hexdigest()}


def audit(build):
    units = load_units('.asset-cache/text/units/units.jsonl')
    supplemental = 'translations/en/supplemental.jsonl'
    containers = build_containers('disc/files', units, load_jsonl(supplemental))
    targets = load_targets('translations/en/returned-v3.jsonl', supplemental)
    text = english_payloads(containers, targets, FontMetrics.load(build / 'font_metrics.json'))
    scripts = {path: c for path, c in containers.items()
               if c.kind in ('talk_script', 'talk_script_supplemental', 'binary_table')}
    data = read_back(build / 'disc/track1.bin', list(scripts))
    results = {path: audit_image(profile_image(c.image) if path == profile.PATH else c.image,
                                data[path], text.payloads.get(path, {}))
               for path, c in sorted(scripts.items())}
    return {'scope': 'active strings in talk, supplemental and fixed-table data containers',
            'excluded': ['executable literals and tables', 'artwork'],
            'containers': len(results), 'strings': sum(r['strings'] for r in results.values()),
            'pointers': sum(r['pointers'] for r in results.values()), 'files': results}


def profile_image(image):
    """Locate JOHO's moved label slots at the addresses used by its English reader."""
    result = copy.copy(image)
    result.slots = dict(image.slots)
    for table, offset, stride, count in (
            ('club_labels', profile.CLUB_OFFSET, profile.CLUB_STRIDE, profile.CLUB_COUNT),
            ('zodiac_labels', profile.ZODIAC_OFFSET, profile.ZODIAC_STRIDE, profile.ZODIAC_COUNT)):
        for index in range(count):
            rid = f'dictionary:{profile.PATH}:{table}:{index:02d}'
            if rid not in result.slots:
                raise ValueError('Profile label missing from audit model: ' + rid)
            result.slots[rid] = replace(result.slots[rid], offset=offset + index * stride)
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.build)
    args.out.write_text(json.dumps(result, indent=2) + '\n')
    print('%d text-data containers, %d strings, %d pointers verified; zero original-font bytes' %
          (result['containers'], result['strings'], result['pointers']))
