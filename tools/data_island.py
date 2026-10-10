#!/usr/bin/env python3
"""Let one C object define its own `.data` (T-9010).

usage: data_island.py [--dry-run] UNIT/ADDR...
       (UNIT = main or an overlay name, ADDR = the object's text start, as in src/<...>/ADDR.c;
        needs a split asm/: run ninja once)

The object's `.data` range comes from its `data` line in config/objects/<UNIT>.txt
(tools/object_boundaries.py --data). The script

  1. turns that range of the splat config into a `.data` island named like the C file
     (`[0x2CD90, .data, RPG_BAT/8014E780]`), cutting the `data` subsegment around it: the
     linker then takes the range from the C object's `.data` (wiki/build-system.md, "Data
     islands");
  2. inserts one `INCLUDE_RODATA("<asm_path>/data/<name>.data", D_X);` line per variable of the
     range into the C file, in address order, after its `#include` lines. tools/data_pieces.py
     (run after every split) writes those pieces. A variable starts on a multiple of 4 (IDO
     aligns every data definition to 4), so a symbol at an unaligned address is a member of
     the one before it and gets no line.

Afterwards replace a line by the C definition of that variable (same place, same size, with
its initialiser): `s32 D_8015EE30[4] = { 0, 0, 0, 0 };`. The bytes then come from the C
compiler, and IDO shares one `lui $at` between the stores to that variable, as the original
does (wiki/decompile-workflow.md, "Data defined in the C file"). Refuses an object without a
data range, a range that does not start on a symbol, or a C file that already has the island.
"""
import argparse
import os
import re
import sys

import object_boundaries as ob
import split_objects as so

BLOCK_HEAD = ("/* .data of this object (T-9010, tools/data_island.py): one line per variable in\n"
              " * address order; replace a line by the variable's C definition. */\n")


class IslandError(Exception):
    pass


def c_path(unit, addr):
    return "src/main/%08X.c" % addr if unit == "main" else "src/ovl/%s/%08X.c" % (unit, addr)


def island_name(unit, addr):
    return "main/%08X" % addr if unit == "main" else "%s/%08X" % (unit, addr)


def piece_dir(unit, addr):
    if unit == "main":
        return "asm/data/main/%08X.data" % addr
    return "asm/ovl/%s/data/%s/%08X.data" % (unit, unit, addr)


def insert_block(text, lines):
    """C text with `lines` after the last top-of-file #include/#define line."""
    rows = text.splitlines(True)
    last = -1
    for i, r in enumerate(rows):
        if r.startswith(("#include", "#define")):
            last = i
        elif r.startswith(("INCLUDE_ASM", "INCLUDE_RODATA")) or re.match(r"^\w[^;]*\)\s*\{", r):
            break
    block = [BLOCK_HEAD] + [l + "\n" for l in lines]   # no blank line: as split_objects.py renders it
    return "".join(rows[:last + 1] + block + rows[last + 1:])


def split_yaml(text, unit, start, end, name):
    """Yaml text with [start, end) cut out of its `data` subsegment as the island `name`."""
    delta = so.EXE_TEXT_OFF if unit == "main" else None
    lines = text.splitlines(True)
    parsed = [so.parse_sub_line(l) for l in lines]
    subs = [i for i, q in enumerate(parsed) if q]
    if delta is None:
        seg = re.search(r"^\s*vram:\s*(0x[0-9A-Fa-f]+)", text, re.M)
        delta = int(seg.group(1), 16)
    for n, i in enumerate(subs):
        off, typ, sname = parsed[i]
        if typ != "data":
            continue
        nxt = subs[n + 1] if n + 1 < len(subs) else None
        hi_off = parsed[nxt][0] if nxt is not None else None
        if hi_off is None:
            m = re.search(r"^  - \[(0x[0-9A-Fa-f]+)\]", text, re.M)
            hi_off = int(m.group(1), 16)
        lo, hi = off + delta, hi_off + delta
        if not lo <= start < end <= hi:
            continue
        new = []
        if start > lo:
            new.append(lines[i])
        new.append("      - [0x%X, .data, %s]\n" % (start - delta, name))
        if end < hi:
            stem = "data" if unit == "main" else "%s_data" % unit
            new.append("      - [0x%X, data, %s_%08X]\n" % (end - delta, stem, end))
        return "".join(lines[:i] + new + lines[i + 1:])
    raise IslandError("%s: no data subsegment holds %08X-%08X" % (unit, start, end))


def plan(root, arg):
    """[(path, new text)] for one UNIT/ADDR."""
    if "/" not in arg:
        raise IslandError("expected UNIT/ADDR, got %s" % arg)
    unit, a = arg.split("/", 1)
    addr = int(a, 16)
    units = {u.name: u for u in ob.units(root)}
    if unit not in units:
        raise IslandError("unknown unit %s" % unit)
    ranges = [r for r in ob.read_data(ob.objects_path(unit, root)) if r.kind == "data" and r.obj == addr]
    if not ranges:
        raise IslandError("%s: object %08X has no data line in %s (object_boundaries.py --data)"
                          % (unit, addr, ob.objects_path(unit, root)))
    r = ranges[0]
    items = ob.load_data_items(units[unit], root, r.start, r.end)
    if not items or items[0][0] != r.start:
        raise IslandError("%s/%08X: no symbol at the data start %08X (run ninja first)" % (unit, addr, r.start))
    cpath = c_path(unit, addr)
    ctext = so.read_text(os.path.join(root, cpath))
    pdir = piece_dir(unit, addr)
    if '"%s"' % pdir in ctext:
        raise IslandError("%s already has the data island" % cpath)
    lines = ['INCLUDE_RODATA("%s", %s);' % (pdir, n) for a_, n, _w in items if a_ % 4 == 0]
    ypath = so.yaml_path(unit)
    ytext = so.read_text(os.path.join(root, ypath))
    return [(cpath, insert_block(ctext, lines)),
            (ypath, split_yaml(ytext, unit, r.start, r.end, island_name(unit, addr)))], r, len(lines)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("objects", nargs="+")
    ap.add_argument("--root", default=".")
    ap.add_argument("--dry-run", action="store_true")
    a = ap.parse_args(argv)
    try:
        for arg in a.objects:
            writes, r, n = plan(a.root, arg)
            print("%s: .data %08X-%08X (%s/%s), %d INCLUDE_RODATA lines%s"
                  % (arg, r.start, r.end, r.start_ev, r.end_ev, n, " (dry run)" if a.dry_run else ""))
            if not a.dry_run:
                for p, t in writes:
                    with open(os.path.join(a.root, p), "w") as f:
                        f.write(t)
    except (IslandError, so.SplitError) as e:
        sys.exit("data_island.py: %s" % e)
    return 0


if __name__ == "__main__":
    sys.exit(main())
