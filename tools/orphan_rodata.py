#!/usr/bin/env python3
"""Assign orphan rodata to C objects using reference order (T-10200).

Usage: tools/docker.sh python3 tools/orphan_rodata.py UNIT [--apply]
Run after ninja. The default previews; --apply updates config/objects and uses
split_objects to regenerate C ownership and YAML. Rebuild and check the unit SHA1.

Split text at a function boundary between the last old-chunk user and first
new-chunk user. Keep each original chunk's tail padding. Choose the latest such
boundary: it is a layout hypothesis, not a recovered historical source boundary.
Every direct user must be inside its proposed object. Unsupported/shared chunks
remain orphaned with an explanation. No instructions or generated asm are edited.
The existing island pipeline emits C switches and literals, asm fallbacks and
unreferenced INCLUDE_RODATA pieces; the full checksum verifies padding and bytes.
"""
import argparse
from pathlib import Path
import sys

import object_boundaries as ob
import split_objects as split


def claim(objects, orphans, funcs, items):
    """Return (objects, remaining orphans, decisions), without changing inputs."""
    objects = list(objects)
    remaining, decisions = [], []
    users = [{f.addr for f in funcs if it.name in f.refs} for it in items]

    def indices(lo, hi):
        return [k for k, it in enumerate(items) if lo <= it.addr < hi]

    def owners(ks):
        return {addr for k in ks for addr in users[k]}

    def check(obj, lo, hi):
        return ob.island_check(items, indices(lo, hi), users,
                               [f for f in funcs if obj.text_start <= f.addr < obj.text_end])

    for start, end, reason in sorted(orphans):
        ks = indices(start, end)
        refs = owners(ks)
        candidates = [o for o in objects if refs and
                      all(o.text_start <= a < o.text_end for a in refs)]
        why = "no single text object owns all references"
        if start % 16 or end % 16:
            candidates = []
            why = "rodata range is not 16-aligned"
        if len(candidates) == 1:
            old = candidates[0]
            pos = objects.index(old)
            if old.ro_start is None:
                why = check(old, start, end)
                if why == "yes":
                    objects[pos] = old._replace(ro_start=start, ro_end=end,
                                               island="yes", ro_ev="orphan-owner")
                    decisions.append("%08X-%08X: assign to %08X" % (start, end, old.text_start))
                    continue
            elif old.island == "yes" and old.ro_end == start:
                previous = owners(indices(old.ro_start, old.ro_end))
                cuts = [f.addr for f in funcs if previous and
                        max(previous) < f.addr <= min(refs) and f.addr % 4 == 0 and
                        old.text_start < f.addr < old.text_end]
                if cuts:
                    cut = max(cuts)
                    left = old._replace(text_end=cut)
                    right = ob.Obj(cut, old.text_end, start, end, "yes",
                                   "orphan-cut", "orphan-owner")
                    checks = [check(left, left.ro_start, left.ro_end), check(right, start, end)]
                    if checks == ["yes", "yes"]:
                        objects[pos:pos + 1] = [left, right]
                        decisions.append("%08X-%08X: split at %08X" % (start, end, cut))
                        continue
                    why = ", ".join(checks)
                else:
                    why = "no function boundary separates users"
            else:
                why = "existing island is disabled or not adjacent"
        remaining.append((start, end, reason))
        decisions.append("%08X-%08X: keep orphan: %s" % (start, end, why))
    return objects, remaining, decisions


def config_text(text, objects, orphans):
    """Replace object/orphan rows, preserving other metadata, comments and data ranges."""
    rows = []
    for o in objects:
        rows.append("object %08X %08X %s %s %s %s %s\n" % (
            o.text_start, o.text_end,
            "-" if o.ro_start is None else "%08X" % o.ro_start,
            "-" if o.ro_end is None else "%08X" % o.ro_end,
            o.island, o.text_ev, o.ro_ev))
    rows += ["orphan %08X %08X %s\n" % x for x in orphans]
    out, inserted = [], False
    for line in text.splitlines(True):
        if line.split()[:1] in (["object"], ["orphan"]):
            if not inserted:
                out.extend(rows)
                inserted = True
        else:
            out.append(line)
    if not inserted:
        raise split.SplitError("objects config has no object rows")
    return "".join(out)


def plan(root, unit):
    """Plan config/source/YAML writes without modifying the input tree."""
    units = {u.name: u for u in ob.units(root)}
    if unit not in units:
        raise split.SplitError("unknown unit %s" % unit)
    path = Path(ob.objects_path(unit, root))
    original = path.read_text()
    objects, ro_end, orphans = ob.read_objects(path)
    funcs, items = ob.load(units[unit], root)
    if not funcs or not items:
        raise split.SplitError("missing disassembly; run ninja first")
    updated, remaining, decisions = claim(objects, orphans, funcs, items)
    for decision in decisions:
        print(decision)
    if updated == objects:
        return [], []
    text = config_text(original, updated, remaining)
    changed = [o for o in objects if o not in updated]
    cfiles = split.srcscan.unit_c_files(unit, root)
    selected = sorted({max((c for c in cfiles if c.start <= o.text_start),
                           key=lambda c: c.start).name for o in changed})
    writes, protos = split.plan(root, selected, object_maps={unit: (updated, ro_end, remaining)})
    return [(str(path.relative_to(root)), text)] + writes, protos


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("unit")
    parser.add_argument("--apply", action="store_true")
    parser.add_argument("--root", default=".")
    args = parser.parse_args(argv)
    try:
        writes, protos = plan(args.root, args.unit)
        for path, _ in writes:
            print("write " + path)
        if args.apply:
            split.apply(args.root, writes, protos)
        print("%d planned writes%s" % (len(writes), "; applied, rebuild and verify SHA1" if args.apply else ""))
    except (OSError, ValueError, split.SplitError) as exc:
        print("orphan_rodata.py: %s" % exc, file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
