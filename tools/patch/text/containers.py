"""Container models for every file that holds a translated record (T-39503).

``build_containers(disc_root, units, supplemental)`` returns one ``Container`` per file:
an ``image.Image`` (slots, pointers, structure) plus the role of every record that
points into the file. Roles:

* ``owner``: the record writes its slot.
* ``alias``: a second record over the same bytes (a typed table entry and its raw
  extent, a C literal and its compiled extent, a skip-N stage view, a suffix that
  code reaches at +N). It names its ``owner``; its translation is not written.
* ``derived``: a runtime-only view (``conditional_runtime_variant``): the game
  changes the owner's bytes at run time; T-39502 must keep that patch valid.
* ``reference``: not game text to write (``O.BIN`` is never loaded, the NAME_KAN
  record describes a kanji lookup table).

Talk-table files (SERIFU, BUNKA, MASTER and the supplemental SHIAI/TAIIKU/TARA_OBJ
scripts) get relocatable slots and their pointer words. Every other container
(overlays, the main executable, JOHO.NBN, SPEAKTST) keeps each string in place,
because code or a fixed stride reaches it; growth there is a build-level change
(the decomp relinks it) and is reported, not written.
"""
from __future__ import annotations

import bisect
import json
import re
import struct
from collections import defaultdict
from dataclasses import dataclass, field
from pathlib import Path

from tools.assets.script_profiles import PROFILES
from tools.assets.text import parse_script, tokenize_text
from tools.assets.text_handoff import template

from .image import Image, Pointer, Slot, align_up
from .segments import Control, source_segments

EXE_PATH = "SLPM_86.053"
EXE_BASE = 0x80041000 - 0x800
OVERLAY_BASE = 0x80132000
# Load bases of the supplemental talk files: every string offset of each file is
# reached by a word equal to base + offset (checked when the container is built).
SUPPLEMENTAL_BASES = {
    "CDROM/SHIAI/SIAI_TX.BIN": 0x801E8000, "CDROM/TAIIKU/F_DANCE.BIN": 0x801E8000,
    "CDROM/TAIIKU/KISA23.BIN": 0x801E8000, "CDROM/TAIIKU/KIYOTAI.BIN": 0x801E8000,
    "CDROM/TAIIKU/TAI_TX.BIN": 0x801F0000, "CDROM/TARA_OBJ/KARAKASI.BIN": 0x801E8000,
}
REFERENCE_ONLY = {
    "CDROM/EXEDIR/O.BIN": "unused_file_copy_of_overlay_strings",
    "CDROM/FUKUTEST/NAME_KAN.NBN": "description_of_name_entry_kanji_table",
}
OWNER_PRIORITY = {"source_literal": 0, "dictionary_slot": 1, "script_record": 1, "source_extent": 2,
                  "speaktst_layout_text": 2, "supplemental": 2, "stage_text_record": 3,
                  "initial_runtime_buffer": 3}


class ContainerError(ValueError):
    """A record cannot be placed in its container."""


@dataclass
class Record:
    """One translation record as the writer sees it."""

    id: str
    kind: str
    path: str | None
    offset: int | None
    payload: bytes
    segments: list
    role: str = "owner"
    owner: str | None = None
    note: str = ""
    unit: dict | None = None


@dataclass
class Container:
    path: str
    kind: str
    image: Image | None
    records: dict[str, Record] = field(default_factory=dict)
    notes: list[str] = field(default_factory=list)

    def owners(self) -> list[Record]:
        return [r for r in self.records.values() if r.role == "owner" and r.id in self.image.slots]


def base_of(path: str) -> int:
    if path == EXE_PATH:
        return EXE_BASE
    if path.startswith("CDROM/EXEDIR/"):
        return 0x80134000 if path.endswith("GYOZI.EXN") else 0x800F6000 if path.endswith("EVENT.EXN") \
            else OVERLAY_BASE
    if path in SUPPLEMENTAL_BASES:
        return SUPPLEMENTAL_BASES[path]
    return 0


def unit_payload(unit: dict) -> bytes:
    raw = bytes.fromhex(unit.get("raw_hex") or "")
    if unit["kind"] == "source_extent" and raw.endswith(b"\0"):
        return raw[:-1]
    return raw


def record_from_unit(unit: dict) -> Record:
    return Record(unit["id"], unit["kind"], unit.get("source_path"), unit.get("source_offset"),
                  unit_payload(unit), source_segments(unit), unit=unit)


# ---------------------------------------------------------------- talk scripts

def _script_container(path: str, data: bytes, profile: dict, units: list[dict]) -> Container:
    parsed = parse_script(data, path, profile)
    base = profile["load_address"]
    by_id = {u["id"]: u for u in units}
    container = Container(path, "talk_script", None)
    slots, pointers = [], []
    structure = bytearray(b"\1" * len(data))
    for rng in parsed["unclaimed_ranges"]:
        structure[rng["offset"]:rng["offset"] + rng["length"]] = bytes(rng["length"])
    for item in parsed["records"]:
        text = item.get("text")
        if text is None:
            continue
        unit = by_id.get(item["id"])
        if unit is None:
            raise ContainerError(f"{item['id']}: talk record has no unit")
        record = record_from_unit(unit)
        if record.offset != text["offset"] or record.payload.hex() != text["raw_hex"]:
            raise ContainerError(f"{item['id']}: unit bytes differ from the parsed table")
        container.records[record.id] = record
        slots.append(Slot(record.id, text["offset"], record.payload, "relocatable"))
        structure[text["offset"]:text["offset"] + len(record.payload) + 1] = bytes(len(record.payload) + 1)
        pointers.append(Pointer(text["pointer_offset"], record.id))
    container.image = Image(path, data, base, slots, pointers, structure=structure)
    for unit in units:
        if unit["id"] not in container.records:
            record = record_from_unit(unit)
            if unit["kind"] != "conditional_runtime_variant":
                raise ContainerError(f"{unit['id']}: unexpected unit in a talk script")
            record.role, record.owner = "derived", unit["parent_id"]
            record.note = "runtime patch of the owner (" + str(unit["context"].get("rule_id")) + ")"
            container.records[record.id] = record
    return container


# ---------------------------------------------------------------- supplemental scripts

_OFFSET = re.compile(r"(?:^|; )(?:str_)?offset=0x([0-9a-fA-F]+)")
_PTR = re.compile(r"ptr_offset=0x([0-9a-fA-F]+)")


def supplemental_segments(raw: bytes) -> list:
    """Source segments of a supplemental talk string.

    The command byte is not known, so the generic rules of command 9 apply (row and
    choice separators, name codes); a leading ``> - + <`` is read as an inline
    speaker label (command 7), which is how these files mark one.
    """
    view = tokenize_text(raw, 7 if raw[:1] in (b">", b"-", b"+", b"<") else 9)
    unit = {"source_text": view["source_decoded_text"], "raw_hex": raw.hex(), "tokens": view["tokens"],
            "kind": "supplemental"}
    copy_template, _, blocker = template(unit)
    if blocker is not None:
        return [Control("opaque", "source_bytes", raw)] if raw else []
    return source_segments(unit)


def _supplemental_container(path: str, data: bytes, rows: list[dict]) -> Container:
    base = SUPPLEMENTAL_BASES[path]
    container = Container(path, "talk_script_supplemental", None)
    words = defaultdict(list)
    for pos in range(0, len(data) - 3, 4):
        words[struct.unpack_from("<I", data, pos)[0]].append(pos)
    slots, pointers = [], []
    for row in rows:
        match = _OFFSET.search(row["context"])
        offset = int(match.group(1), 16)
        end = data.index(b"\0", offset)
        raw = data[offset:end]
        record = Record(row["id"], "supplemental", path, offset, raw, supplemental_segments(raw))
        sites = words.get(base + offset, [])
        explicit = _PTR.search(row["context"])
        if explicit and int(explicit.group(1), 16) not in sites:
            raise ContainerError(f"{row['id']}: stated pointer does not reach the string")
        if not sites:
            raise ContainerError(f"{row['id']}: no pointer word reaches the string")
        container.records[record.id] = record
        slots.append(Slot(record.id, offset, raw, "relocatable"))
        pointers.extend(Pointer(site, record.id) for site in sites)
    structure = bytearray(b"\1" * len(data))
    for slot in slots:
        structure[slot.offset:slot.offset + slot.size] = bytes(slot.size)
    # Without a parsed table layout only the strings and the final zero run are
    # free: alignment bytes after a string may belong to a following byte table.
    tail = len(data) - 4 if data[-2:] == b"\x12\x10" else len(data)
    while tail > 0 and not data[tail - 1]:
        tail -= 1
        structure[tail] = 0
    for pointer in pointers:
        structure[pointer.offset:pointer.offset + 4] = b"\1\1\1\1"
    container.image = Image(path, data, base, slots, pointers, structure=structure)
    return container


# ---------------------------------------------------------------- in-place binaries

def _owner_key(record: Record):
    return (record.offset, -len(record.payload), OWNER_PRIORITY.get(record.kind, 9), record.id)


def _assign_roles(records: list[Record]) -> None:
    """Pick one owner per byte span; overlapping records become aliases of it."""
    placed = sorted((r for r in records if r.offset is not None and r.role == "owner"), key=_owner_key)
    spans: list[Record] = []
    for record in placed:
        end = record.offset + len(record.payload) + 1
        host = None
        for other in reversed(spans):
            other_end = other.offset + len(other.payload) + 1
            if other.offset <= record.offset < other_end or (
                    record.offset <= other.offset < end):
                host = other
                break
            if other_end <= record.offset and other.offset < record.offset - 4096:
                break
        if host is None:
            spans.append(record)
            continue
        better = (OWNER_PRIORITY.get(record.kind, 9) < OWNER_PRIORITY.get(host.kind, 9)
                  and record.offset == host.offset and len(record.payload) == len(host.payload))
        if better:
            host.role, host.owner = "alias", record.id
            spans[spans.index(host)] = record
        else:
            record.role, record.owner = "alias", host.id


def _in_place_capacity(data: bytes, record: Record, starts: list[int], base: int,
                       fixed: dict[str, tuple[int, int]]) -> tuple[int, bool]:
    if record.id in fixed:
        slot_offset, stride = fixed[record.id]
        capacity = slot_offset + stride - record.offset
        tail = data[record.offset + len(record.payload) + 1:slot_offset + stride]
        return capacity, any(tail)
    end = record.offset + len(record.payload) + 1
    stop = align_up(end, 4, base)
    index = bisect.bisect_right(starts, record.offset)
    if index < len(starts):
        stop = min(stop, starts[index])
    while end < stop and not data[end]:
        end += 1
    return end - record.offset, False


def _fixed_slots(units: list[dict]) -> dict[str, tuple[int, int]]:
    """Fixed-stride table entries: record id -> (slot offset, stride)."""
    out = {}
    for unit in units:
        if unit["kind"] != "dictionary_slot":
            continue
        context = unit["context"]
        slot_raw = context.get("slot_raw_hex")
        slot_offset = context.get("slot_offset")
        if slot_offset is None or not slot_raw:
            continue
        if len(slot_raw) // 2 > 4 or slot_offset == unit["source_offset"]:
            out[unit["id"]] = (slot_offset, len(slot_raw) // 2)
    return out


def _binary_container(path: str, data: bytes, units: list[dict], rows: list[dict]) -> Container:
    base = base_of(path)
    kind = ("main_executable" if path == EXE_PATH else "overlay" if path.endswith(".EXN")
            else "speaktst" if "/SPEAKTST/" in path else "binary_table")
    container = Container(path, kind, None)
    records = []
    for unit in units:
        record = record_from_unit(unit)
        if unit["kind"] == "conditional_runtime_variant":
            record.role, record.owner = "derived", unit["parent_id"]
        elif record.offset is None:
            record.role, record.note = "source_only", "C literal without a proven binary extent"
        records.append(record)
    for row in rows:
        match = _OFFSET.search(row["context"])
        offset = int(match.group(1), 16)
        raw = data[offset:data.index(b"\0", offset)]
        records.append(Record(row["id"], "supplemental", path, offset, raw, supplemental_segments(raw)))
    _assign_roles(records)
    fixed = _fixed_slots(units)
    owners = sorted((r for r in records if r.role == "owner"), key=lambda r: r.offset)
    starts = sorted({r.offset for r in owners})
    slots = []
    for record in owners:
        capacity, keep_tail = _in_place_capacity(data, record, starts, base, fixed)
        slots.append(Slot(record.id, record.offset, record.payload, "in_place",
                          capacity=capacity, keep_tail=keep_tail))
    for record in records:
        container.records[record.id] = record
    container.image = Image(path, data, base, slots, [], growable=False)
    return container


# ---------------------------------------------------------------- entry point

def load_units(path: str | Path) -> list[dict]:
    with open(path, encoding="utf-8") as stream:
        return [json.loads(line) for line in stream]


def load_supplemental(path: str | Path) -> list[dict]:
    with open(path, encoding="utf-8") as stream:
        return [json.loads(line) for line in stream]


def build_containers(disc_root: str | Path, units: list[dict], supplemental: list[dict]) -> dict[str, Container]:
    """Model every container that a unit or supplemental record points into."""
    root = Path(disc_root)
    profiles = {profile["path"]: profile for profile in PROFILES}
    by_path: dict[str, list[dict]] = defaultdict(list)
    no_path = []
    for unit in units:
        if unit.get("source_path") and unit.get("source_offset") is not None:
            by_path[unit["source_path"]].append(unit)
        else:
            no_path.append(unit)
    supplemental_by_path: dict[str, list[dict]] = defaultdict(list)
    for row in supplemental:
        supplemental_by_path[row["id"].split(":")[1]].append(row)
    containers = {}
    for path in sorted(set(by_path) | set(supplemental_by_path)):
        if path in REFERENCE_ONLY:
            container = Container(path, "reference_only", None, notes=[REFERENCE_ONLY[path]])
            for row in supplemental_by_path[path]:
                container.records[row["id"]] = Record(row["id"], "supplemental", path, None, b"", [],
                                                      role="reference", note=REFERENCE_ONLY[path])
            if by_path.get(path):
                raise ContainerError(f"{path}: units in a reference-only file")
            containers[path] = container
            continue
        data = (root / path).read_bytes()
        if path in profiles:
            if supplemental_by_path.get(path):
                raise ContainerError(f"{path}: supplemental rows in a profiled talk file")
            containers[path] = _script_container(path, data, profiles[path], by_path[path])
        elif path in SUPPLEMENTAL_BASES:
            if by_path.get(path):
                raise ContainerError(f"{path}: units in a supplemental talk file")
            containers[path] = _supplemental_container(path, data, supplemental_by_path[path])
        else:
            containers[path] = _binary_container(path, data, by_path[path], supplemental_by_path[path])
    if no_path:
        container = Container("(source only)", "c_source", None)
        for unit in no_path:
            record = record_from_unit(unit)
            record.role, record.note = "source_only", "C literal without a proven binary extent"
            container.records[record.id] = record
        containers[container.path] = container
    return containers


def identity_check(container: Container) -> dict:
    """Rebuild the container from its owners' original segments; compare with the file."""
    from .segments import ShiftJisEncoder, encode
    if container.image is None:
        return {"path": container.path, "status": "no_image"}
    encoder = ShiftJisEncoder()
    payloads = {}
    mismatched = []
    for record in container.owners():
        payload = encode(record.segments, encoder)
        if payload != record.payload:
            mismatched.append(record.id)
        payloads[record.id] = payload
    result = container.image.write(payloads)
    same = result.data == container.image.data
    return {"path": container.path, "kind": container.kind, "status": "identical" if same and not mismatched
            else "different", "segment_mismatches": mismatched[:5], "slots": len(container.image.slots),
            "pointers": len(container.image.pointers), "size": len(result.data),
            "moved": sum(p.moved for p in result.placements.values()), "overflows": result.overflows[:3]}
