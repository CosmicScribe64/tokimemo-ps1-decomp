#!/usr/bin/env python3
"""Report decompilation progress for the main executable.

Counts functions still included via INCLUDE_ASM in src/main/*.c against functions
written in C, using sizes from the splat `nonmatching <name>, <size>` headers
in asm/nonmatchings/<seg>/<name>.s. A function counts as decompiled only if a C definition
exists in src/main/*.c (the ninja build sha1 check guarantees it matches).

Usage: python3 tools/progress.py [--root DIR]
Run inside Docker: tools/docker.sh python3 tools/progress.py
Exit code is non-zero if a function's size cannot be determined.
"""
import argparse
import re
import sys
from pathlib import Path

INC = re.compile(r'^\s*INCLUDE_ASM\(\s*"([^"]+)"\s*,\s*(\w+)\s*\)', re.M)
DEF = re.compile(r'^[A-Za-z_][\w \t*]*[ \t*](\w+)\(.*\)\s*\{', re.M)
NONMATCH = re.compile(r'^nonmatching\s+(\w+),\s*(0x[0-9A-Fa-f]+|\d+)', re.M)


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--root", default=".")
    root = Path(ap.parse_args().root)

    # name -> size for every function split by splat
    sizes = {}
    # asm/matchings holds decompiled functions (splat disassemble_all), asm/nonmatchings the rest
    for s in [f for d in ("nonmatchings", "matchings") for f in (root / "asm" / d).rglob("*.s")]:
        m = NONMATCH.search(s.read_text(errors="replace")[:400])
        if not m or m.group(1) != s.stem:
            print("error: no `nonmatching %s, size` header in %s" % (s.stem, s), file=sys.stderr)
            return 1
        sizes[(s.parent.name, m.group(1))] = int(m.group(2), 0)
    if not sizes:
        print("error: no asm/nonmatchings found (run ninja first)", file=sys.stderr)
        return 1

    remaining = {}
    defined = set()
    # one row per C file (src/main/<addr>.c, T-0012); overlays under src/ovl are not counted
    for c in sorted((root / "src" / "main").glob("*.c")):
        text = c.read_text()
        for folder, name in INC.findall(text):
            remaining[(Path(folder).name, name)] = c.name
        defined.update(DEF.findall(text))
    missing = [k for k in remaining if k not in sizes]
    if missing:
        print("error: no size for %s" % ", ".join(n for _, n in missing[:5]), file=sys.stderr)
        return 1

    lost = [k for k in sizes if k not in remaining and k[1] not in defined]
    if lost:
        print("error: neither INCLUDE_ASM nor C body for %s" % ", ".join(n for _, n in lost[:5]), file=sys.stderr)
        return 1

    segs = {}
    for (seg, name), size in sizes.items():
        t = segs.setdefault(seg, [0, 0, 0, 0])
        t[0] += 1
        t[1] += size
        if (seg, name) not in remaining:
            t[2] += 1
            t[3] += size
    print("%-12s %12s %18s" % ("file", "functions", "bytes"))
    tot = [0, 0, 0, 0]
    for seg, t in sorted(segs.items()):
        print("%-12s %5d/%-5d %7d/%-7d %5.1f%%" % (seg, t[2], t[0], t[3], t[1], 100.0 * t[3] / t[1]))
        tot = [a + b for a, b in zip(tot, t)]
    print("%-12s %5d/%-5d %7d/%-7d %5.1f%%" % ("total", tot[2], tot[0], tot[3], tot[1], 100.0 * tot[3] / tot[1]))
    return 0


if __name__ == "__main__":
    sys.exit(main())
