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
     the one before it and gets no line. Nor does an aligned label that only a data word names
     (a pointer into the middle of an array) and no code does: it is reported as interior.

Afterwards replace a line by the C definition of that variable (same place, same size, with
its initialiser): `s32 D_8015EE30[4] = { 0, 0, 0, 0 };`. The bytes then come from the C
compiler, and IDO shares one `lui $at` between the stores to that variable, as the original
does (wiki/decompile-workflow.md, "Data defined in the C file"). Refuses an object without a
data range, a range that does not start on a symbol, or a range the yaml already has as an island.

An object that already has an island elsewhere gets this range as a further island (T-25005): a
dict line with its own `dir` and `linker_section` (data_pieces.secondary()), pieces in the same
directory as the first island's. tools/data_split.py then cuts the compiled `.data` into the
ranges; the C file must emit each range's data back to back (wiki/build-system.md).
"""
import argparse
import os
import re
import sys

import yaml

import data_pieces as dp
import object_boundaries as ob
import split_objects as so
import srcscan

BLOCK_HEAD = ("/* .data of this object (T-9010, tools/data_island.py): one line per variable in\n"
              " * address order; replace a line by the variable's C definition. */\n")


class IslandError(Exception):
    pass


def c_path(unit, addr, root="."):
    """The C file of the object at addr: src/<yaml name>.c (readable once renamed, T-27002)."""
    name = island_name(unit, addr, root)
    return "src/%s.c" % name if unit == "main" else "src/ovl/%s.c" % name


def island_name(unit, addr, root="."):
    """The `c` subsegment name of the object at addr (srcscan.object_name: the yaml's name, else the
    address name)."""
    if not os.path.exists(os.path.join(root, so.yaml_path(unit))):
        return "main/%08X" % addr if unit == "main" else "%s/%08X" % (unit, addr)
    return srcscan.object_name(unit, addr, root)


def piece_dir(unit, addr, root="."):
    name = island_name(unit, addr, root)
    if unit == "main":
        return "asm/data/%s.data" % name
    return "asm/ovl/%s/data/%s.data" % (unit, name)


def split_starts(items, start, code_refs):
    """All piece starts plus advisory pointer-only labels (not ownership evidence).
    Every aligned label starts a piece even if only a data pointer names it.
    When defining an aggregate, replace all includes covered by it together."""
    starts = dp.piece_starts([(a, n) for a, n, _w in items])
    interior, owner = [], None
    for a, n in starts:
        if owner is not None and a != start and n not in code_refs:
            interior.append((a, n, owner))
        else:
            owner = n
    return starts, interior


def insert_block(text, lines, interior=()):
    """C text with `lines` after the last top-of-file #include/#define line. `interior`
    ([(addr, name, owner)]) are advisory aggregate candidates; they still get an include."""
    rows = text.splitlines(True)
    last = -1
    for i, r in enumerate(rows):
        if r.startswith(("#include", "#define")):
            last = i
        elif r.startswith(("INCLUDE_ASM", "INCLUDE_RODATA")) or re.match(r"^\w[^;]*\)\s*\{", r):
            break
    notes = ["/* pointer-only label %s may be inside %s; replace its include too. */\n" % (n, o)
             for _a, n, o in interior]
    block = [BLOCK_HEAD] + [l + "\n" for l in lines] + notes   # no blank line: as split_objects.py renders it
    return "".join(rows[:last + 1] + block + rows[last + 1:])


def island_line(off, name, vram=None):
    """The yaml line of an island; with vram, a further island of the object (T-25005)."""
    if vram is None:
        return "      - [0x%X, .data, %s]\n" % (off, name)
    sdir, section = dp.secondary(vram)
    return ("      - { start: 0x%X, type: .data, name: %s, dir: %s, linker_section: %s }\n"
            % (off, name, sdir, section))


def delta_of(unit, text):
    """vram minus file offset of a unit's code segment."""
    if unit == "main":
        return so.EXE_TEXT_OFF
    return int(re.search(r"^\s*vram:\s*(0x[0-9A-Fa-f]+)", text, re.M).group(1), 16)


def split_yaml(text, unit, start, end, name, further=False):
    """Yaml text with [start, end) cut out of its `data` subsegment as the island `name`
    (further: the object already has an island, so this one gets its own section)."""
    delta = delta_of(unit, text)
    lines = text.splitlines(True)
    parsed = [so.parse_sub_line(l) for l in lines]
    subs = [i for i, q in enumerate(parsed) if q]
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
        new.append(island_line(start - delta, name, start if further else None))
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
    cpath = c_path(unit, addr, root)
    ctext = so.read_text(os.path.join(root, cpath))
    pdir = piece_dir(unit, addr, root)
    ypath = so.yaml_path(unit)
    ytext = so.read_text(os.path.join(root, ypath))
    own = [i for i in dp.islands(yaml.safe_load(ytext)) if i.name == island_name(unit, addr, root)]
    if any(i.start <= r.start - delta_of(unit, ytext) < i.end for i in own) or (
            not own and '"%s"' % pdir in ctext):
        raise IslandError("%s already has the data island" % cpath)
    funcs, _items = ob.load(units[unit], root)
    code_refs = set().union(*(f.refs for f in funcs)) if funcs else set()
    starts, interior = split_starts(items, r.start, code_refs)
    lines = ['INCLUDE_RODATA("%s", %s);' % (pdir, n) for _a, n in starts]
    return [(cpath, insert_block(ctext, lines, interior)),
            (ypath, split_yaml(ytext, unit, r.start, r.end, island_name(unit, addr, root), bool(own)))], \
        r, len(lines), interior


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("objects", nargs="+")
    ap.add_argument("--root", default=".")
    ap.add_argument("--dry-run", action="store_true")
    a = ap.parse_args(argv)
    try:
        for arg in a.objects:
            writes, r, n, interior = plan(a.root, arg)
            print("%s: .data %08X-%08X (%s/%s), %d INCLUDE_RODATA lines%s"
                  % (arg, r.start, r.end, r.start_ev, r.end_ev, n, " (dry run)" if a.dry_run else ""))
            for _a, name, owner in interior:
                print("  %s: included; may be inside %s (only a data pointer names it);"
                      " replace all covered includes when defining an aggregate" % (name, owner))
            if not a.dry_run:
                for p, t in writes:
                    with open(os.path.join(a.root, p), "w") as f:
                        f.write(t)
    except (IslandError, so.SplitError) as e:
        sys.exit("data_island.py: %s" % e)
    return 0


if __name__ == "__main__":
    sys.exit(main())
