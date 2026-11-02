"""English write-through and size report (T-39503).

``english_payloads`` encodes every owner record's translation with a pluggable encoder;
``size_report`` writes each container with those payloads and reports original and
English sizes, sectors, moved strings, in-place overflows, the CD reads that load
the file (sector count and RAM address from the C source) and, for overlays and the
main executable, the growth of their text and the free space they have.
"""
from __future__ import annotations

import re
from collections import Counter, defaultdict
from pathlib import Path

from .containers import EXE_PATH, Container
from .image import SECTOR
from .segments import Control, encode
from .translations import target_of

READ_CALL = re.compile(r"cd_read_sectors(?:_nowait)?\(\s*(0x[0-9A-Fa-f]+|[0-9]+)U?\s*,\s*"
                       r"(0x[0-9A-Fa-f]+|[0-9]+)U?\s*,\s*(0x[0-9A-Fa-f]+|[0-9]+)U?\s*\)")
OVERLAY_SLOT = 0x30000
MAIN_BSS_END = 0x8012B538
OVERLAY_LOAD = 0x80132000


def align4(value: int) -> int:
    return value + (-value) % 4


def english_payloads(containers: dict[str, Container], targets: dict[str, dict], encoder) -> tuple[dict, dict]:
    """Per path {record id: payload}, and per-record problems (ids by problem)."""
    payloads: dict[str, dict[str, bytes]] = defaultdict(dict)
    problems: dict[str, list[str]] = defaultdict(list)
    for path, container in containers.items():
        for record in container.records.values():
            row = targets.get(record.id)
            if row is None:
                problems["no_translation_row"].append(record.id)
                continue
            if record.role not in ("owner", "source_only"):
                continue
            segments, problem = target_of(row, record.segments)
            if problem:
                problems[problem].append(record.id)
            if segments is None:
                continue
            if any(isinstance(s, Control) and s.kind == "opaque" for s in record.segments):
                problems["opaque_source_bytes_kept"].append(record.id)
                continue
            payload = encode(segments, encoder)
            if any(byte >= 0x80 for byte in payload):
                problems["english_payload_keeps_non_ascii_bytes"].append(record.id)
            payloads[path][record.id] = payload
    return payloads, problems


def scan_read_sites(source_root: str | Path) -> list[dict]:
    """Every ``cd_read_sectors`` call in src/ with literal (sectors, address, LBA)."""
    sites = []
    for path in sorted(Path(source_root).rglob("*.c")):
        text = path.read_text(encoding="utf-8")
        for match in READ_CALL.finditer(text):
            sectors, address, lba = (int(group, 0) for group in match.groups())
            line = text.count("\n", 0, match.start()) + 1
            sites.append({"source": f"{path.as_posix()}:{line}", "sectors": sectors,
                          "address": address, "lba": lba})
    return sites


def _overlay_free_tail(data: bytes) -> int:
    end = len(data) - 4
    tail = end
    while tail > 0 and not data[tail - 1]:
        tail -= 1
    return end - align4(tail)


def _max_label_address(asm_root: Path, low: int, high: int, skip: str | None = None) -> int | None:
    best = None
    label = re.compile(r"\bD_(80[0-9A-Fa-f]{6})\b")
    if not asm_root.is_dir():
        return None
    for path in asm_root.rglob("*.s"):
        if skip and skip in path.parts:
            continue
        for match in label.finditer(path.read_text(encoding="utf-8", errors="replace")):
            value = int(match.group(1), 16)
            if low <= value < high and (best is None or value > best):
                best = value
    return best


def size_report(containers: dict[str, Container], payloads: dict, *, lba_map: dict | None = None,
                read_sites: list[dict] | None = None, repository: str | Path | None = None) -> dict:
    """Write every container with ``payloads`` and summarise sizes and limits."""
    by_lba = defaultdict(list)
    for site in read_sites or []:
        by_lba[site["lba"]].append(site)
    max_read = Counter()
    for site in read_sites or []:
        max_read[site["address"]] = max(max_read[site["address"]], site["sectors"])
    rows = []
    pointer_tables = Counter()
    for path, container in sorted(containers.items()):
        image = container.image
        if image is None:
            continue
        result = image.write(payloads.get(path, {}))
        sectors = (len(image.data) + SECTOR - 1) // SECTOR
        new_sectors = (len(result.data) + SECTOR - 1) // SECTOR
        row = {"path": path, "kind": container.kind, "original_bytes": len(image.data),
               "english_bytes": len(result.data), "original_sectors": sectors,
               "english_sectors": new_sectors, "slots": len(image.slots),
               "translated": len(payloads.get(path, {})),
               "moved": sum(p.moved for p in result.placements.values()),
               "pointers_rewritten": sum(1 for p in image.pointers if result.placements[p.slot].moved),
               "in_place_overflows": [o for o in result.overflows if o["reason"] == "in_place_capacity"
                                      and container.records[o["id"]].kind != "source_literal"],
               "literal_extents_relinked": sum(1 for o in result.overflows if o["reason"] == "in_place_capacity"
                                               and container.records[o["id"]].kind == "source_literal"),
               "free_bytes_left": result.free_bytes_left}
        if row["pointers_rewritten"]:
            pointer_tables[path] = row["pointers_rewritten"]
        in_place = [s for s in image.slots.values() if s.mode == "in_place"
                    and container.records[s.id].kind != "source_literal"]
        if in_place:
            new = payloads.get(path, {})
            row["in_place_text_bytes"] = sum(align4(s.size) for s in in_place)
            row["english_text_bytes"] = sum(align4(len(new.get(s.id, s.payload)) + len(s.terminator))
                                            for s in in_place)
            row["overflow_bytes"] = sum(o["needed"] - o["capacity"] for o in row["in_place_overflows"])
        entry = (lba_map or {}).get(path)
        if entry:
            row["lba"] = entry["lba"]
            sites = by_lba.get(entry["lba"], [])
            row["read_sites"] = [{k: (hex(v) if k == "address" else v) for k, v in s.items()} for s in sites]
            for site in sites:
                if new_sectors > site["sectors"]:
                    row.setdefault("read_sites_needing_more_sectors", []).append(site["source"])
                if new_sectors > max_read[site["address"]]:
                    row.setdefault("beyond_largest_original_read_into_buffer", []).append(
                        {"address": hex(site["address"]), "largest_original_sectors": max_read[site["address"]]})
        if container.kind == "overlay":
            row["overlay_zero_tail_bytes"] = _overlay_free_tail(image.data)
            if repository is not None:
                name = Path(path).stem
                top = _max_label_address(Path(repository) / "asm" / "ovl" / name, image.base,
                                         image.base + OVERLAY_SLOT)
                last_data = image.base + len(image.data) - 4 - row["overlay_zero_tail_bytes"]
                row["highest_symbol_in_slot"] = hex(top) if top else None
                row["bss_in_zero_tail"] = bool(top and top >= last_data)
        row["in_place_overflow_count"] = len(row.pop("in_place_overflows"))
        rows.append(row)
    summary = summarize(rows, containers, payloads, repository)
    summary["pointer_rewrites_by_file"] = dict(pointer_tables)
    return {"containers": rows, "summary": summary}


def literal_growth(containers: dict[str, Container], payloads: dict) -> dict:
    """Growth of C string literals by link unit (each literal 4-aligned, NUL included)."""
    out = defaultdict(lambda: {"literals": 0, "original": 0, "english": 0})
    for path, container in containers.items():
        for record in container.records.values():
            if record.kind != "source_literal" or record.role not in ("owner", "source_only"):
                continue
            unit = (record.unit or {}).get("context", {}).get("link_unit") or "unknown"
            new = payloads.get(path, {}).get(record.id, record.payload)
            item = out[unit]
            item["literals"] += 1
            item["original"] += align4(len(record.payload) + 1)
            item["english"] += align4(len(new) + 1)
    for item in out.values():
        item["growth"] = item["english"] - item["original"]
    return dict(sorted(out.items()))


def overlay_fit(rows: list[dict], literals: dict) -> dict:
    """Net text growth of each overlay against the zero tail of its 0x30000-byte slot.

    Growth is the C-literal growth of the overlay's link unit plus the growth of its
    in-place tables and extents (4-aligned). A relinked overlay must fit its slot;
    ``bss_in_zero_tail`` marks overlays whose symbols reach into that tail.
    """
    out = {}
    for row in rows:
        if row["kind"] != "overlay":
            continue
        name = Path(row["path"]).stem
        growth = literals.get(name, {}).get("growth", 0) + row.get("english_text_bytes", 0) \
            - row.get("in_place_text_bytes", 0)
        tail = row.get("overlay_zero_tail_bytes", 0)
        out[name] = {"net_growth": growth, "zero_tail_bytes": tail, "fits": growth <= tail,
                     "bss_in_zero_tail": row.get("bss_in_zero_tail")}
    return out


def summarize(rows: list[dict], containers: dict[str, Container], payloads: dict, repository) -> dict:
    files = [r for r in rows if r["kind"].startswith("talk_script")]
    grown = [r for r in files if r["english_sectors"] > r["original_sectors"]]
    binary = [r for r in rows if not r["kind"].startswith("talk_script") and r["kind"] != "speaktst"]
    speaktst = [r for r in rows if r["kind"] == "speaktst"]
    literals = literal_growth(containers, payloads)
    exe = next((r for r in rows if r["path"] == EXE_PATH), None)
    main_growth = literals.get("main", {}).get("growth", 0)
    if exe:
        main_growth += max(0, exe.get("english_text_bytes", 0) - exe.get("in_place_text_bytes", 0))
    summary = {
        "talk_files": len(files),
        "talk_original_bytes": sum(r["original_bytes"] for r in files),
        "talk_english_bytes": sum(r["english_bytes"] for r in files),
        "talk_files_growing_in_sectors": len(grown),
        "talk_files_needing_read_site_changes": sum(bool(r.get("read_sites_needing_more_sectors")) for r in files),
        "talk_files_beyond_largest_buffer_read": sum(bool(r.get("beyond_largest_original_read_into_buffer"))
                                                     for r in files),
        "in_place_containers": len(binary),
        "in_place_overflow_records": sum(r["in_place_overflow_count"] for r in binary),
        "in_place_overflow_bytes": sum(r.get("overflow_bytes", 0) for r in binary),
        "speaktst_overflow_records_out_of_scope": sum(r["in_place_overflow_count"] for r in speaktst),
        "c_literal_growth_by_unit": literals,
        "c_literal_growth_total": sum(item["growth"] for item in literals.values()),
        "overlay_fit": overlay_fit(rows, literals),
        "main_exe_text_growth_bytes": main_growth,
        "main_exe_headroom_bytes": OVERLAY_LOAD - MAIN_BSS_END,
    }
    if repository is not None:
        top = _max_label_address(Path(repository) / "asm", MAIN_BSS_END, OVERLAY_LOAD, skip="ovl")
        summary["labels_between_main_bss_and_overlay"] = hex(top) if top else None
    return summary
