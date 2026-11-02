"""Rebuild one binary container from its text payloads and its non-text structure (T-39503).

A container is the original file plus a model of where its text lives:

* ``Slot`` with ``mode="relocatable"``: a NUL-terminated string reached only through
  pointer words in the same file (talk tables). The writer may move it.
* ``Slot`` with ``mode="in_place"``: a string reached from code, a fixed-stride table
  or an unknown place. It keeps its offset and must fit ``capacity`` bytes
  (terminator included).
* ``Pointer``: a little-endian 32-bit word holding ``base + offset(slot) + addend``.

Everything else (tables, command bytes, voice pairs, code, unclaimed bytes) is
structure and is copied unchanged. Relocatable strings are laid out again from
scratch: the space they occupied, plus the zero alignment bytes after each one,
becomes free regions that are filled in original order, each string aligned to
``alignment``; strings that do not fit go to the zero tail of the file and then to
appended sectors. With the original payloads this reproduces the original layout,
which is what the identity round-trip proves. The CD trailer (sum16 high, low,
0x12, 0x10) stays at the end of the file; its sum is recomputed when the original
one is valid and kept as it was otherwise.
"""
from __future__ import annotations

import struct
from dataclasses import dataclass, field

SECTOR = 2048
TRAILER_MARKER = b"\x12\x10"


class ImageError(ValueError):
    """The container model contradicts the original bytes."""


@dataclass
class Slot:
    id: str
    offset: int
    payload: bytes
    mode: str = "in_place"
    terminator: bytes = b"\0"
    capacity: int | None = None
    keep_tail: bool = False

    @property
    def size(self) -> int:
        return len(self.payload) + len(self.terminator)


@dataclass
class Pointer:
    offset: int
    slot: str
    addend: int = 0


@dataclass
class Placement:
    id: str
    offset: int
    size: int
    moved: bool


@dataclass
class WriteResult:
    data: bytes
    placements: dict[str, Placement]
    overflows: list[dict] = field(default_factory=list)
    grown_bytes: int = 0
    free_bytes_left: int = 0


def has_cd_trailer(data: bytes) -> bool:
    """True when the last four bytes are a valid sum16 + 0x12 0x10 trailer."""
    if len(data) < 4 or len(data) % SECTOR or data[-2:] != TRAILER_MARKER:
        return False
    return int.from_bytes(data[-4:-2], "big") == sum(data[:-4]) & 0xFFFF


def write_cd_trailer(buf: bytearray) -> None:
    total = sum(buf[:-4]) & 0xFFFF
    buf[-4:] = total.to_bytes(2, "big") + TRAILER_MARKER


def align_up(value: int, alignment: int, base: int) -> int:
    """Smallest offset >= value whose RAM address (base + offset) is a multiple of alignment."""
    return value + (-(base + value)) % alignment


class Image:
    """One container: original bytes, slots, pointers and the structure mask."""

    def __init__(self, path: str, data: bytes, base: int, slots: list[Slot], pointers: list[Pointer],
                 *, alignment: int = 4, max_size: int | None = None, growable: bool = True,
                 structure: bytearray | None = None):
        self.path, self.data, self.base = path, bytes(data), base
        self.slots = {slot.id: slot for slot in slots}
        if len(self.slots) != len(slots):
            raise ImageError(f"{path}: duplicate slot ids")
        self.pointers = pointers
        self.alignment = alignment
        # The marker is structure whenever present; the sum is recomputed only when the
        # original one is valid (one BUNKA file carries a stale sum and keeps it).
        self.trailer = (len(self.data) >= 4 and len(self.data) % SECTOR == 0
                        and self.data[-2:] == TRAILER_MARKER)
        self.trailer_sum_valid = has_cd_trailer(self.data)
        self.max_size = max_size
        self.growable = growable
        self.structure = structure if structure is not None else bytearray(len(data))
        self._check()
        self.regions = self._free_regions()

    def _check(self) -> None:
        data = self.data
        for slot in self.slots.values():
            end = slot.offset + slot.size
            if slot.offset < 0 or end > len(data):
                raise ImageError(f"{self.path}: slot {slot.id} outside the file")
            if data[slot.offset:end] != slot.payload + slot.terminator:
                raise ImageError(f"{self.path}: slot {slot.id} bytes differ from the original")
            if slot.mode == "in_place":
                if slot.capacity is None or slot.capacity < slot.size:
                    raise ImageError(f"{self.path}: slot {slot.id} capacity below its original size")
                tail = data[end:slot.offset + slot.capacity]
                if any(tail) and not slot.keep_tail:
                    raise ImageError(f"{self.path}: slot {slot.id} capacity covers non-zero bytes")
            elif slot.mode != "relocatable":
                raise ImageError(f"{self.path}: unknown slot mode {slot.mode}")
        for pointer in self.pointers:
            slot = self.slots.get(pointer.slot)
            if slot is None:
                raise ImageError(f"{self.path}: pointer at {pointer.offset:#x} names no slot")
            word = struct.unpack_from("<I", data, pointer.offset)[0]
            if word != self.base + slot.offset + pointer.addend:
                raise ImageError(f"{self.path}: pointer at {pointer.offset:#x} does not reach {slot.id}")
            self.structure[pointer.offset:pointer.offset + 4] = b"\1\1\1\1"

    def _payload_end(self) -> int:
        end = len(self.data) - (4 if self.trailer else 0)
        return end

    def _free_regions(self) -> tuple[list[list[int]], int | None]:
        """Closed spans the relocatable strings may use, and the start of the open tail.

        Closed spans are each string's own bytes plus the zero alignment bytes after it.
        The open tail is the zero run before the trailer (or the end of file); it
        continues into appended sectors when the image may grow.
        """
        data, free = self.data, bytearray(len(self.data))
        owned = bytearray(len(data))
        for slot in self.slots.values():
            owned[slot.offset:slot.offset + slot.size] = b"\1" * slot.size
        relocatable = [s for s in self.slots.values() if s.mode == "relocatable"]
        for slot in relocatable:
            end = slot.offset + slot.size
            stop = min(align_up(end, self.alignment, self.base), self._payload_end())
            while end < stop and not data[end] and not owned[end] and not self.structure[end]:
                end += 1
            free[slot.offset:end] = b"\1" * (end - slot.offset)
        regions, pos = [], 0
        while pos < len(free):
            if not free[pos]:
                pos += 1
                continue
            start = pos
            while pos < len(free) and free[pos]:
                pos += 1
            regions.append([start, pos])
        if not relocatable or not self.growable:
            return regions, None
        tail = self._payload_end()
        while tail > 0 and not data[tail - 1] and not owned[tail - 1] and not self.structure[tail - 1]:
            tail -= 1
        tail = align_up(tail, self.alignment, self.base)
        if regions and regions[-1][1] >= tail:
            return regions[:-1], regions[-1][0]
        return regions, tail

    def write(self, payloads: dict[str, bytes] | None = None, *, compact: bool = False) -> WriteResult:
        """Rebuild the file; ``payloads`` maps slot ids to new payloads (others keep theirs)."""
        payloads = payloads or {}
        unknown = set(payloads) - set(self.slots)
        if unknown:
            raise ImageError(f"{self.path}: payloads for unknown slots {sorted(unknown)[:3]}")
        regions, open_start = self.regions
        end_of_payload = self._payload_end()
        buf = bytearray(self.data[:end_of_payload])
        for start, end in regions:
            buf[start:end] = bytes(end - start)
        if open_start is not None:
            buf[open_start:] = bytes(len(buf) - open_start)
        placements: dict[str, Placement] = {}
        overflows = []
        index, cursor, closed_left = 0, None, 0
        relocatable = sorted((s for s in self.slots.values() if s.mode == "relocatable"),
                             key=lambda s: s.offset)
        # T-39506: place long translated strings first, retaining every unused gap.
        # Each slot owns separate bytes: scripts may modify strings at run time.
        available = [list(region) for region in regions]
        tail_cursor = open_start
        if compact:
            relocatable.sort(key=lambda s: (-len(payloads.get(s.id, s.payload) + s.terminator), s.offset))
        for slot in relocatable:
            body = payloads.get(slot.id, slot.payload) + slot.terminator
            pos = None
            if compact:
                fits = []
                for region_index, (start, end) in enumerate(available):
                    candidate = align_up(start, self.alignment, self.base)
                    if candidate + len(body) <= end:
                        fits.append((end - candidate - len(body), region_index, candidate))
                if fits:
                    _, region_index, pos = min(fits)
                    available[region_index][0] = pos + len(body)
                elif tail_cursor is not None:
                    pos = align_up(tail_cursor, self.alignment, self.base)
                    tail_cursor = pos + len(body)
                else:
                    raise ImageError(f"{self.path}: compact layout has no room for {slot.id}")
                if len(buf) < pos + len(body):
                    buf.extend(bytes(pos + len(body) - len(buf)))
                buf[pos:pos + len(body)] = body
                placements[slot.id] = Placement(slot.id, pos, len(body), pos != slot.offset)
                continue
            while index < len(regions):
                start = regions[index][0] if cursor is None else cursor
                candidate = align_up(start, self.alignment, self.base)
                if candidate + len(body) <= regions[index][1]:
                    pos = candidate
                    break
                index += 1
                cursor = None
            if pos is None:
                if open_start is None:
                    overflows.append({"id": slot.id, "needed": len(body), "reason": "no_free_space"})
                    placements[slot.id] = Placement(slot.id, slot.offset, slot.size, False)
                    continue
                pos = align_up(open_start if cursor is None else cursor, self.alignment, self.base)
                if len(buf) < pos + len(body):
                    buf.extend(bytes(pos + len(body) - len(buf)))
            buf[pos:pos + len(body)] = body
            placements[slot.id] = Placement(slot.id, pos, len(body), pos != slot.offset)
            cursor = pos + len(body)
        if index < len(regions):
            closed_left = (regions[index][1] - (cursor or regions[index][0])) + sum(
                end - start for start, end in regions[index + 1:])
        if compact:
            closed_left = sum(end - start for start, end in available)
            cursor = tail_cursor
        for slot in self.slots.values():
            if slot.mode != "in_place":
                continue
            body = payloads.get(slot.id, slot.payload) + slot.terminator
            if len(body) > slot.capacity:
                overflows.append({"id": slot.id, "needed": len(body), "capacity": slot.capacity,
                                  "reason": "in_place_capacity"})
                placements[slot.id] = Placement(slot.id, slot.offset, slot.size, False)
                continue
            if slot.keep_tail:
                buf[slot.offset:slot.offset + len(body)] = body
            else:
                buf[slot.offset:slot.offset + slot.capacity] = body + bytes(slot.capacity - len(body))
            placements[slot.id] = Placement(slot.id, slot.offset, len(body), False)
        for pointer in self.pointers:
            target = placements[pointer.slot].offset
            struct.pack_into("<I", buf, pointer.offset, self.base + target + pointer.addend)
        payload_size = max(len(buf), end_of_payload)
        total = payload_size + (4 if self.trailer else len(self.data) - end_of_payload)
        if total > len(self.data):
            total += (-total) % SECTOR
        else:
            total = len(self.data)
        if self.trailer:
            buf.extend(bytes(total - 4 - len(buf)))
            buf.extend(self.data[-4:])
            if self.trailer_sum_valid:
                write_cd_trailer(buf)
        else:
            buf.extend(self.data[end_of_payload:] if total == len(self.data) else bytes(total - len(buf)))
        if self.max_size is not None and len(buf) > self.max_size:
            overflows.append({"id": None, "needed": len(buf), "capacity": self.max_size,
                              "reason": "load_buffer"})
        open_left = (len(buf) - (4 if self.trailer else 0) - max(cursor or 0, open_start)) \
            if open_start is not None else 0
        return WriteResult(bytes(buf), placements, overflows, len(buf) - len(self.data),
                           closed_left + max(0, open_left))
