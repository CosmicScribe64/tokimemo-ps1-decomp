"""Give over-capacity in-place strings new room by moving them and re-pointing their references (T-39506).

An in-place record (``tools/patch/text/inplace.py``) whose English text is longer than its slot cannot be
written there. When the code reaches the slot only through relocations (a ``%hi``/``%lo`` pair in code,
or a pointer word in a table), the string can live anywhere: this module writes it into a free area of
the same binary and rewrites every reference to point there. The original bytes stay where they were.

References come from the English link itself: for every input section in the unit's map
(``build/en/<unit>.map``), the relocations of its object give the site and the target address in the
English layout (a section symbol through the map, any other symbol through the English ELF). A slot
can be moved only when

* at least one reference reaches its first byte, and none reaches a byte inside it (an interior pointer
  would still read the old bytes);
* every ``%lo`` that shares a ``%hi`` with a reference also targets the slot, unless the new address
  keeps the same ``%hi``.

A slot reached by computed addresses (a fixed stride from a table base) has no reference of its own and
stays reported as ``needs_relink``.

Free areas: an overlay's zero tail (after its last nonzero byte and its last symbol, plus 16 bytes, up to
the checksum trailer; wiki/ram-map.md says it is free while the overlay runs); the main executable's
``english_string_pool`` (src/english/script.c), which is part of the loaded file.
"""
from __future__ import annotations

import struct
from collections import defaultdict

from tools import elf32
from tools.english_link import SEGMENT_SYMBOL, read_map, symbols

STT_SECTION = 3
SHF_ALLOC = 2
POOL_SYMBOL = "english_string_pool"
POOL_BYTES_SYMBOL = "english_string_pool_bytes"  # an s32 holding the pool's size (IDO's st_size is not it)
TAIL_MARGIN = 16


class References:
    """Relocation sites of one linked unit, by target address."""

    def __init__(self, map_path: str, elf_path: str):
        self.placed = read_map(map_path)
        self.syms = symbols(elf_path)
        self.words = defaultdict(list)    # target -> [site address]
        self.pairs = defaultdict(list)    # target -> [(hi sites, lo site)]
        self.hi_targets = defaultdict(set)  # hi site -> targets of every lo that uses it
        objects = defaultdict(list)
        for obj, sec in self.placed:
            objects[obj].append(sec)
        for obj in sorted(objects):
            self._scan(obj)

    def _symbol_address(self, obj, elf, sym):
        if sym.type == STT_SECTION:
            placed = self.placed.get((obj, elf.sections[sym.shndx].name))
            return None if placed is None else placed[0]
        if sym.shndx == elf32.SHN_ABS:
            return sym.value
        if sym.name in self.syms and (sym.bind == 1 or sym.shndx == elf32.SHN_UNDEF):
            return self.syms[sym.name][0]
        if sym.shndx in (elf32.SHN_UNDEF, elf32.SHN_COMMON):
            return None
        placed = self.placed.get((obj, elf.sections[sym.shndx].name))
        return None if placed is None else placed[0] + sym.value

    def _scan(self, obj):
        elf = elf32.Elf(obj)
        for sec in elf.sections:
            if sec.type == elf32.SHT_REL or not sec.flags & SHF_ALLOC:
                continue
            placed = self.placed.get((obj, sec.name))
            if placed is None:
                continue
            base = placed[0]
            data = elf.contents(sec)
            pending = defaultdict(list)  # symbol -> HI16 offsets not yet paired
            used = {}                    # symbol -> HI16 offsets of the last pair
            for offset, kind, sym in sorted(elf.relocations(sec), key=lambda r: r[0]):
                if not data:
                    break
                key = (sym.name, sym.shndx, sym.value)
                if kind == elf32.R_MIPS_32:
                    target = self._symbol_address(obj, elf, sym)
                    if target is not None:
                        addend = struct.unpack_from("<I", data, offset)[0]
                        self.words[(target + addend) & 0xFFFFFFFF].append(base + offset)
                elif kind == elf32.R_MIPS_HI16:
                    pending[key].append(offset)
                elif kind == elf32.R_MIPS_LO16:
                    his = pending.pop(key, None) or used.get(key)
                    if not his:
                        continue
                    used[key] = his
                    target = self._symbol_address(obj, elf, sym)
                    if target is None:
                        continue
                    hi = struct.unpack_from("<H", data, his[0])[0]
                    lo = struct.unpack_from("<h", data, offset)[0]
                    address = (target + (hi << 16) + lo) & 0xFFFFFFFF
                    hi_sites = tuple(base + h for h in his)
                    self.pairs[address].append((hi_sites, base + offset))
                    for h in hi_sites:
                        self.hi_targets[h].add(address)

    def inside(self, start: int, end: int) -> bool:
        """True when a reference reaches a byte in (start, end)."""
        return any(start < t < end for t in self.words) or any(start < t < end for t in self.pairs)


def hi_half(address: int) -> int:
    return ((address + 0x8000) >> 16) & 0xFFFF


class Pool:
    """Free bytes of a built binary (file offsets) and the RAM address of file offset 0."""

    def __init__(self, start: int, end: int, vram_of_offset_0: int):
        self.cursor, self.end, self.vram = (start + 3) & ~3, end, vram_of_offset_0

    def take(self, size: int) -> int | None:
        if self.cursor + size > self.end:
            return None
        at = self.cursor
        self.cursor = (self.cursor + size + 3) & ~3
        return at


def overlay_pool(built: bytes, elf_path: str, vram: int) -> Pool:
    """Zero tail of an English overlay image (see the module docstring)."""
    end = len(built) - 4
    start = end
    while start > 0 and built[start - 1] == 0:
        start -= 1
    for sym in elf32.Elf(elf_path).symbols:
        if sym.name and sym.shndx not in (elf32.SHN_UNDEF, elf32.SHN_ABS) and not SEGMENT_SYMBOL.search(sym.name) \
                and vram <= sym.value < vram + end:
            start = max(start, sym.value - vram)
    return Pool(start + TAIL_MARGIN, end, vram)


def main_pool(built: bytes, elf_path: str, vram_of_offset_0: int) -> Pool | None:
    """english_string_pool of the English executable, sized by english_string_pool_bytes."""
    found = {sym.name: sym.value for sym in elf32.Elf(elf_path).symbols
             if sym.name in (POOL_SYMBOL, POOL_BYTES_SYMBOL)}
    if len(found) != 2:
        return None
    start = found[POOL_SYMBOL] - vram_of_offset_0
    size = struct.unpack_from("<i", built, found[POOL_BYTES_SYMBOL] - vram_of_offset_0)[0]
    return Pool(start, start + size, vram_of_offset_0)


class Retargeter:
    """Moves over-capacity slots of one binary into its pool."""

    def __init__(self, references: References, pool: Pool | None, default_elf: str | None = None):
        self.refs, self.pool = references, pool
        self.moved: dict = {}
        self.refused: dict = {}
        self.default_labels = defaultdict(list)
        if default_elf is not None:
            for sym in elf32.Elf(default_elf).symbols:
                if sym.name and sym.shndx not in (elf32.SHN_UNDEF, elf32.SHN_ABS) \
                        and not SEGMENT_SYMBOL.search(sym.name):
                    self.default_labels[sym.value].append(sym.name)

    def locate_by_label(self, default_address: int) -> int | None:
        """File offset in the English binary of a default address that a label names, when the English
        ELF has the same label (a slot inside an object the English build compiles again)."""
        if self.pool is None:
            return None
        for name in self.default_labels.get(default_address, []):
            if name in self.refs.syms:
                return self.refs.syms[name][0] - self.pool.vram
        return None

    def referenced(self, slot_offset: int) -> bool:
        """True when a relocation reaches the first byte of the slot at this file offset."""
        if self.pool is None:
            return False
        old = self.pool.vram + slot_offset
        return bool(self.refs.words.get(old) or self.refs.pairs.get(old))

    def place(self, out: bytearray, rid: str, slot_offset: int, capacity: int, body: bytes) -> bool:
        """Write body (with its NUL) into the pool and re-point the references of the slot at slot_offset."""
        if self.pool is None:
            self.refused[rid] = "no pool"
            return False
        vram = self.pool.vram
        old = vram + slot_offset
        words = self.refs.words.get(old, [])
        pairs = self.refs.pairs.get(old, [])
        if not words and not pairs:
            self.refused[rid] = "no reference to the slot (fixed stride or computed address)"
            return False
        if self.refs.inside(old, old + capacity):
            self.refused[rid] = "a reference reaches inside the slot"
            return False
        at = self.pool.take(len(body))
        if at is None:
            self.refused[rid] = "pool full"
            return False
        new = vram + at
        for hi_sites, _lo in pairs:
            for h in hi_sites:
                if self.refs.hi_targets[h] != {old} and hi_half(new) != hi_half(old):
                    self.refused[rid] = "a %%hi at 0x%08X is shared with other targets" % h
                    self.pool.cursor = at
                    return False
        out[at:at + len(body)] = body
        for site in words:
            struct.pack_into("<I", out, site - vram, new)
        for hi_sites, lo in pairs:
            for h in hi_sites:
                struct.pack_into("<H", out, h - vram, hi_half(new))
            struct.pack_into("<H", out, lo - vram, new & 0xFFFF)
        self.moved[rid] = {"from": old, "to": new, "words": len(words), "pairs": len(pairs)}
        return True
