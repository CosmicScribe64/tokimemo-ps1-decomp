#!/usr/bin/env python3
"""CD read sites, file placement checks and the English CD remap table (T-39507).

The English disc moves every text file that grew by a sector or more to the end of the data
track (tools/patch/disc.py). The game names files by literal sector numbers: 920
``cd_read_sectors`` calls in src/, LBA tables in main and overlay data (the BUNKA tour tables
``tour_invite_script_sectors`` / ``tour_invite_script_lba`` among them), and the end-word check
of ``cd_read_sectors`` needs the read to end on the file's trailer. The English build therefore
redirects both read entry points (src/english/cd.c, config/english_hooks.txt) through a table of
moved files, generated here from the layout the disc build produced:

  original LBA, new LBA, original sector count, new sector count

A read that starts inside a moved file's original sectors goes to the new place; a read of at
least the whole original file grows by the sectors the file gained.

Before a file is moved, ``plan_file`` checks what the code is known to do with it:

* every literal read site and table entry that reaches the file (``scan_read_sites``,
  ``tour_table_sites``): a read that starts inside the file but not at its start, or that also
  covers another file, cannot be remapped exactly and is reported;
* the load buffer: the new size must not exceed the largest read of the original into the same
  address, or the limit in ``BUFFER_LIMITS`` (the DATE buffer at 0x80198000 holds 24 sectors);
  otherwise the original file stays and the RAM map work (T-39505) has to make room;
* a literal ``cd_file_checksum_ok(blocks, address)`` with the file's original sector count
  would check the wrong end and is reported.

CLI (in Docker): ``python3 tools/patch/cd_layout.py remap-asm LAYOUT.json OUT.s`` writes the
island table; a missing LAYOUT.json gives an empty table, so ``ninja english`` works before the
first disc build.
"""
from __future__ import annotations

import json
import re
import struct
import sys
from dataclasses import dataclass, field
from pathlib import Path

SECTOR = 2048
_ARG = r"(?:\(\s*[A-Za-z_][A-Za-z0-9_ ]*\**\s*\)\s*)?(0x[0-9A-Fa-f]+|[0-9]+)[uU]?"
READ_CALL = re.compile(r"\bcd_read_sectors(_nowait)?\(\s*" + _ARG + r"\s*,\s*" + _ARG + r"\s*,\s*" + _ARG + r"\s*\)")
CHECKSUM_CALL = re.compile(r"\bcd_file_checksum_ok\(\s*" + _ARG + r"\s*,\s*" + _ARG + r"\s*\)")

# Load buffers whose size the code proves, beyond "the largest original read into the address".
BUFFER_LIMITS = {
    0x80198000: (24, "DATE talk buffer: the largest original read is 24 sectors and the RAM above it "
                     "is not mapped yet (T-39505)"),
}
# Read tables in overlay data: (overlay, sectors table, lba table, entries, buffer address, source).
READ_TABLES = (
    ("BUNKAKEN", "tour_invite_script_sectors", "tour_invite_script_lba", 12, 0x801E8000,
     "src/ovl/culture_festival_tour/invitation.c, companion_menu.c"),
)
OVERLAY_BASE = 0x80132000
MAX_SECTORS = 0xFF  # the count is a u8 argument


def sectors_of(size: int) -> int:
    return max(1, (size + SECTOR - 1) // SECTOR)


def scan_read_sites(source_root: str | Path) -> list[dict]:
    """Every ``cd_read_sectors``/``_nowait`` call in the C sources with literal arguments."""
    sites = []
    root = Path(source_root)
    for path in sorted(root.rglob("*.c")):
        text = path.read_text(encoding="utf-8")
        for match in READ_CALL.finditer(text):
            sectors, address, lba = (int(group, 0) for group in match.groups()[1:])
            sites.append({"source": "%s:%d" % (path.relative_to(root.parent).as_posix(),
                                               text.count("\n", 0, match.start()) + 1),
                          "sectors": sectors, "address": address, "lba": lba,
                          "checked": match.group(1) is None})
    return sites


def scan_checksum_sites(source_root: str | Path) -> list[dict]:
    """Every ``cd_file_checksum_ok(blocks, address)`` with literal arguments."""
    sites = []
    root = Path(source_root)
    for path in sorted(root.rglob("*.c")):
        text = path.read_text(encoding="utf-8")
        for match in CHECKSUM_CALL.finditer(text):
            blocks, address = (int(group, 0) for group in match.groups())
            sites.append({"source": "%s:%d" % (path.relative_to(root.parent).as_posix(),
                                               text.count("\n", 0, match.start()) + 1),
                          "blocks": blocks, "address": address})
    return sites


def _symbol(path: Path, name: str) -> int:
    for line in path.read_text(encoding="utf-8").splitlines():
        words = line.split("//")[0].replace(";", " ").split()
        if len(words) >= 3 and words[0] == name and words[1] == "=":
            return int(words[2], 16)
    raise KeyError("%s: %s not found" % (path, name))


def tour_table_sites(disc_root: str | Path, config_root: str | Path) -> list[dict]:
    """Reads made through the overlay read tables, from the original overlay bytes."""
    sites = []
    for overlay, sectors_name, lba_name, entries, address, source in READ_TABLES:
        data = (Path(disc_root) / "CDROM" / "EXEDIR" / (overlay + ".EXN")).read_bytes()
        symbols = Path(config_root) / ("symbols_%s.txt" % overlay)
        sectors_at = _symbol(symbols, sectors_name) - OVERLAY_BASE
        lba_at = _symbol(symbols, lba_name) - OVERLAY_BASE
        for i in range(entries):
            sites.append({"source": "%s %s[%d] (%s)" % (overlay, lba_name, i, source),
                          "sectors": data[sectors_at + i], "address": address,
                          "lba": struct.unpack_from("<i", data, lba_at + 4 * i)[0], "checked": True})
    return sites


@dataclass
class FilePlan:
    path: str
    lba: int
    sectors: int
    new_sectors: int
    accept: bool = True
    reason: str | None = None
    sites: list = field(default_factory=list)
    problems: list = field(default_factory=list)
    warnings: list = field(default_factory=list)


def buffer_limits(sites: list[dict]) -> dict[int, tuple[int, str]]:
    """Largest original read per load address, overridden by ``BUFFER_LIMITS``."""
    limits: dict[int, tuple[int, str]] = {}
    for site in sites:
        best = limits.get(site["address"], (0, ""))[0]
        if site["sectors"] > best:
            limits[site["address"]] = (site["sectors"], "largest original read into this address")
    limits.update(BUFFER_LIMITS)
    return limits


def plan_file(path: str, lba: int, size: int, new_size: int, sites: list[dict], limits: dict,
              checksum_sites: list[dict], files_by_lba: dict[int, tuple[str, int]]) -> FilePlan:
    """Decide whether a file may take its English size; see the module docstring."""
    sectors, new_sectors = sectors_of(size), sectors_of(new_size)
    plan = FilePlan(path, lba, sectors, new_sectors)
    if new_sectors <= sectors:
        return plan
    end = lba + sectors
    for site in sites:
        site_end = site["lba"] + site["sectors"]
        if site_end <= lba or site["lba"] >= end:
            continue
        plan.sites.append(site["source"])
        if site["lba"] != lba:
            plan.problems.append("%s reads from inside the file (sector %d of %d)"
                                 % (site["source"], site["lba"] - lba, sectors))
            continue
        if site["sectors"] > sectors:
            others = sorted(name for start, (name, _) in files_by_lba.items() if end <= start < site_end)
            if others:
                plan.problems.append("%s also reads %s" % (site["source"], ", ".join(others[:3])))
        if site["sectors"] < sectors:
            plan.warnings.append("%s reads %d of the %d sectors" % (site["source"], site["sectors"], sectors))
        limit, basis = limits.get(site["address"], (None, None))
        if limit is not None and new_sectors > limit:
            plan.accept = False
            plan.reason = ("buffer_limit: needs %d sectors at 0x%08X, limit %d (%s)"
                           % (new_sectors, site["address"], limit, basis))
    for site in checksum_sites:
        if site["blocks"] == sectors and any(s["address"] == site["address"] for s in sites
                                              if s["source"] in plan.sites):
            plan.problems.append("%s checks %d blocks at 0x%08X" % (site["source"], site["blocks"], site["address"]))
    if not plan.sites:
        plan.warnings.append("no literal read site or table entry; read through data the scan does not know")
    if new_sectors > MAX_SECTORS:
        plan.accept, plan.reason = False, "needs %d sectors, a read holds at most %d" % (new_sectors, MAX_SECTORS)
    return plan


def remap_entries(placed: list[dict]) -> list[dict]:
    """Table rows for every file whose LBA or sector count changed, sorted by original LBA."""
    rows = [p for p in placed if (p["lba"], p["sectors"]) != (p["new_lba"], p["new_sectors"])]
    return sorted(rows, key=lambda p: p["lba"])


def remap_asm(entries: list[dict]) -> str:
    """Assembly for ``english_cd_remap`` / ``english_cd_remap_count`` (include/english/cd.h)."""
    lines = ["# Generated by tools/patch/cd_layout.py (T-39507). Do not edit.",
             ".section .data", ".balign 4",
             ".globl english_cd_remap_count", ".type english_cd_remap_count, @object",
             "english_cd_remap_count:", "    .word %d" % len(entries),
             ".size english_cd_remap_count, 4",
             ".globl english_cd_remap", ".type english_cd_remap, @object", "english_cd_remap:"]
    for entry in entries:
        if not 0 < entry["new_sectors"] <= MAX_SECTORS or not 0 < entry["sectors"] <= MAX_SECTORS:
            raise ValueError("%s: sector counts outside 1-%d" % (entry.get("path"), MAX_SECTORS))
        lines.append("    .word %d, %d  # %s" % (entry["lba"], entry["new_lba"], entry.get("path", "")))
        lines.append("    .byte %d, %d, 0, 0" % (entry["sectors"], entry["new_sectors"]))
    lines.append(".size english_cd_remap, %d" % (12 * len(entries)))
    return "\n".join(lines) + "\n"


def main(argv: list[str]) -> int:
    if len(argv) != 4 or argv[1] != "remap-asm":
        sys.exit(__doc__)
    layout_path, out = Path(argv[2]), Path(argv[3])
    entries = json.loads(layout_path.read_text())["remap"] if layout_path.is_file() else []
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(remap_asm(entries))
    print("english cd remap: %d files -> %s" % (len(entries), out))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
