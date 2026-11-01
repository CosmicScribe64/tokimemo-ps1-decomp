"""Coverage map: every English record id to one written location, and the gaps (T-39503).

``coverage_map`` lists each record with its container, offset, role and owner.
``check_ids`` compares that map with the two JSONL files. ``text_gaps`` scans the
data parts of every container (and every other CDROM file) for NUL-terminated
Shift-JIS runs that no record covers. ``magazine_tables`` checks the four tables of
``hud_magazine_page_build`` (src/main/hud_screens.c) slot by slot.
"""
from __future__ import annotations

import bisect
import struct
from collections import Counter, defaultdict
from pathlib import Path

from .containers import EXE_BASE, EXE_PATH, Container

# First rodata byte of the main executable (wiki/executable.md) and the code size
# of each overlay (config/overlays.txt): text before these offsets is not scanned.
EXE_DATA_START = 0x6EB40
MIN_RUN_CHARS = 2


def coverage_map(containers: dict[str, Container]) -> dict[str, dict]:
    out = {}
    for path, container in containers.items():
        for record in container.records.values():
            if record.id in out:
                raise ValueError(f"record {record.id} appears in two containers")
            entry = {"container": path, "container_kind": container.kind, "kind": record.kind,
                     "offset": record.offset, "role": record.role}
            if record.owner:
                entry["owner"] = record.owner
            if record.note:
                entry["note"] = record.note
            context = (record.unit or {}).get("context", {})
            if record.kind == "source_literal":
                entry["source"] = {"path": context.get("source_locator_id", "").split(":")[1]
                                   if context.get("source_locator_id") else None,
                                   "line": context.get("source_line"), "column": context.get("source_column"),
                                   "link_unit": context.get("link_unit")}
            out[record.id] = entry
    return out


def check_ids(mapping: dict[str, dict], translation_ids: list[str]) -> dict:
    ids = Counter(translation_ids)
    duplicates = sorted(i for i, n in ids.items() if n > 1)
    unmapped = sorted(i for i in ids if i not in mapping)
    unused = sorted(i for i in mapping if i not in ids)
    roles = Counter(mapping[i]["role"] for i in ids if i in mapping)
    return {"translation_ids": len(ids), "mapped": len(ids) - len(unmapped), "unmapped": unmapped,
            "duplicate_ids": duplicates, "map_ids_without_translation": unused, "roles": dict(roles)}


def overlay_code_sizes(repository: str | Path) -> dict[str, int]:
    sizes = {}
    path = Path(repository) / "config" / "overlays.txt"
    for line in path.read_text(encoding="utf-8").splitlines():
        if line.startswith("#") or not line.strip():
            continue
        name, _, size = line.split()
        sizes[f"CDROM/EXEDIR/{name}.EXN"] = int(size, 16)
    return sizes


def _is_glyph(high: int, low: int) -> bool:
    code = high << 8 | low
    return (0x8140 <= code <= 0x84BE or 0x889F <= code <= 0x9872) and low >= 0x40 and low != 0x7F


def _reads_as_text(raw: bytes) -> bool:
    """Kana-rich and not one repeated glyph: graphics and code rarely look like this."""
    pairs = [raw[i] << 8 | raw[i + 1] for i in range(len(raw) - 1) if raw[i] >= 0x81]
    pairs = [p for p in pairs if 0x8140 <= p <= 0x9872]
    if len(set(pairs)) < 2:
        return False
    kana = [p for p in pairs if 0x829F <= p <= 0x82F1 or 0x8340 <= p <= 0x8396]
    latin = sum(0x824F <= p <= 0x829A for p in pairs)
    return len(set(kana)) >= 3 and len(kana) * 10 >= len(pairs) * 3 and latin * 10 < len(pairs) * 3


def sjis_runs(data: bytes, start: int = 0, end: int | None = None) -> list[tuple[int, int]]:
    """(offset, length) of NUL-terminated runs of at least two glyph pairs.

    A run may contain the talk-script single-byte controls (0x20 and the name letters)
    between pairs. Detection is a heuristic for finding gaps, not a parser.
    """
    end = len(data) if end is None else end
    runs, pos = [], start
    while pos < end - 1:
        if not _is_glyph(data[pos], data[pos + 1]):
            pos += 1
            continue
        run, glyphs = pos, 0
        while pos < end - 1:
            if _is_glyph(data[pos], data[pos + 1]):
                glyphs += 1
                pos += 2
            elif data[pos] in b" qnPpmFGYSOWAM":
                pos += 1
            else:
                break
        if glyphs >= MIN_RUN_CHARS and pos < end and data[pos] == 0 and _reads_as_text(data[run:pos]):
            runs.append((run, pos - run))
        pos = max(pos, run + 1)
    return runs


def _covered_spans(container: Container) -> list[tuple[int, int]]:
    spans = []
    for record in container.records.values():
        if record.offset is not None:
            spans.append((record.offset, record.offset + max(1, len(record.payload))))
    if container.image is not None:
        for slot in container.image.slots.values():
            spans.append((slot.offset, slot.offset + max(1, slot.size)))
    return sorted(spans)


def _uncovered(runs, spans):
    starts = [s for s, _ in spans]
    out = []
    for offset, length in runs:
        index = bisect.bisect_right(starts, offset + length) - 1
        hit = False
        while index >= 0 and spans[index][0] <= offset + length:
            if spans[index][1] > offset:
                hit = True
                break
            if offset - spans[index][0] > 4096:
                break
            index -= 1
        if not hit:
            out.append((offset, length))
    return out


def text_gaps(containers: dict[str, Container], disc_root: str | Path, repository: str | Path,
              *, scan_other_files: bool = True) -> dict:
    """Japanese runs without a record, per container and in other CDROM files."""
    root = Path(disc_root)
    code_sizes = overlay_code_sizes(repository)
    per_container = {}
    for path, container in sorted(containers.items()):
        if container.image is None or container.kind == "speaktst":
            continue
        data = container.image.data
        start = EXE_DATA_START if path == EXE_PATH else code_sizes.get(path, 0)
        runs = sjis_runs(data, start)
        if container.kind == "talk_script":
            structure = container.image.structure
            runs = [(o, n) for o, n in runs if not any(structure[o:o + n])]
        gaps = _uncovered(runs, _covered_spans(container))
        if gaps:
            per_container[path] = {"count": len(gaps), "bytes": sum(n for _, n in gaps),
                                   "offsets": [hex(o) for o, _ in gaps[:40]]}
    others = {}
    if scan_other_files:
        for file in sorted(root.rglob("*")):
            rel = file.relative_to(root).as_posix()
            if not file.is_file() or rel in containers or rel.endswith((".STR", ".XA")):
                continue
            data = file.read_bytes()
            runs = [r for r in sjis_runs(data) if r[1] >= 6]
            if len(runs) >= 3:
                others[rel] = {"count": len(runs), "bytes": sum(n for _, n in runs),
                               "offsets": [hex(o) for o, _ in runs[:10]]}
    return {"containers": per_container, "other_files": others,
            "rule": f"NUL-terminated runs of >= {MIN_RUN_CHARS} font glyph pairs in data parts; "
                    "other files: >= 3 runs of >= 3 glyphs"}


def _exe_offset(address: int) -> int:
    return address - EXE_BASE


def _string_at(data: bytes, offset: int, limit: int) -> tuple[int, int]:
    end = data.find(b"\0", offset, offset + limit)
    return offset, (end if end >= 0 else offset + limit) - offset


MAGAZINE_TABLES = (
    # label, address, shape, evidence (src/main/hud_screens.c, include/main/hud.h, asm/data)
    ("D_800B32CC", 0x800B32CC, "pointer_to_pointer_table_16"),
    ("D_800B5C2C", 0x800B5C2C, (13, 17)),
    ("D_800B5D1C", 0x800B5D1C, (13, 17)),
    ("D_800B5EA4", 0x800B5EA4, (13, 3, 36)),
)


def magazine_tables(containers: dict[str, Container]) -> list[dict]:
    """Every slot of the magazine-page tables with the record that covers it."""
    container = containers[EXE_PATH]
    data = container.image.data
    spans = defaultdict(list)
    for record in container.records.values():
        if record.offset is not None:
            spans[record.offset].append(record)
    rows = []
    for label, address, shape in MAGAZINE_TABLES:
        slots = []
        if shape == "pointer_to_pointer_table_16":
            table = struct.unpack_from("<I", data, _exe_offset(address))[0]
            for index in range(16):
                target = struct.unpack_from("<I", data, _exe_offset(table) + 4 * index)[0]
                slots.append(((index,), _exe_offset(target), 8))
        else:
            stride = shape[-1]
            count = 1
            for size in shape[:-1]:
                count *= size
            for flat in range(count):
                index, rest = [], flat
                for size in reversed(shape[:-1]):
                    index.insert(0, rest % size)
                    rest //= size
                slots.append((tuple(index), _exe_offset(address) + flat * stride, stride))
        for index, offset, limit in slots:
            start, length = _string_at(data, offset, limit)
            ids = [r.id for r in spans.get(start, [])]
            rows.append({"table": label, "index": list(index), "file_offset": hex(start), "length": length,
                         "records": [{"id": i, "role": container.records[i].role} for i in ids],
                         "blank": data[start:start + length].replace(b"\x81\x40", b"") == b""})
    return rows
