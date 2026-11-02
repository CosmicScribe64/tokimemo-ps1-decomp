"""Resolve IDO's unnamed static helpers through actual MIPS JALs (T-39510).

IDO omits static function symbols. Named function entries and section-relative
JAL targets delimit the callable regions in the English runtime object. Never
use object relocations alone as proof: every reported edge must match the
instruction read from the release disc.
"""
import bisect
import struct

from tools import elf32


def jal_target(word, site):
    if word >> 26 != 3:
        raise ValueError('Expected JAL at compiled helper call')
    return ((site + 4) & 0xF0000000) | ((word & 0x03FFFFFF) << 2)


def named_calls(data, base, start, end, symbols, names):
    """Return actual direct calls in a bounded compiled function region."""
    if start % 4 or end % 4 or not base <= start < end <= base + len(data):
        raise ValueError('Invalid compiled call-scan bounds')
    found = {name: [] for name in names}
    for site in range(start, end, 4):
        word = struct.unpack_from('<I', data, site - base)[0]
        if word >> 26 != 3:
            continue
        target = jal_target(word, site)
        for name in found:
            if symbols[name][0] == target:
                found[name].append(site)
    return found


def local_regions(object_path, placed_start, data, base):
    elf = elf32.Elf(object_path)
    section = elf.section('.text')
    original = elf.contents(section)
    entries = {symbol.value for symbol in elf.symbols
               if symbol.type == 2 and symbol.shndx == section.index}
    edges = []
    for offset, kind, symbol in elf.relocations(section):
        if kind != elf32.R_MIPS_26 or symbol.type != 3 or symbol.shndx != section.index:
            continue
        word = struct.unpack_from('<I', original, offset)[0]
        if word >> 26 != 3:
            continue
        target = (word & 0x03FFFFFF) << 2
        if target >= section.size or target % 4:
            raise ValueError('Static helper target outside input text')
        entries.add(target)
        site = placed_start + offset
        at = site - base
        if at < 0 or at + 4 > len(data):
            raise ValueError('Static helper call outside disc file')
        actual = struct.unpack_from('<I', data, at)[0]
        if jal_target(actual, site) != placed_start + target:
            raise ValueError('Static helper JAL differs from linked target')
        edges.append({'site': site, 'target': placed_start + target})
    starts = sorted(placed_start + offset for offset in entries)
    ends = starts[1:] + [placed_start + section.size]
    ranges = dict(zip(starts, ends))
    for edge in edges:
        index = bisect.bisect_right(starts, edge['site']) - 1
        if index < 0 or edge['site'] >= ranges[starts[index]]:
            raise ValueError('Static helper call has no containing region')
        edge['caller'] = starts[index]
    return ranges, edges
