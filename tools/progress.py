#!/usr/bin/env python3
"""Report decompilation progress: main game, the 26 overlays, and the SDK libs.

Counts functions still included via INCLUDE_ASM in the C sources against functions
written in C, using sizes from the splat `nonmatching <name>, <size>` headers of the asm
files. A function counts as decompiled only if a C definition exists in the source (the
ninja build sha1 check guarantees it matches).
  main game: src/main/*.c against asm/{nonmatchings,matchings}/main/<addr>/*.s, one row per file
  overlays:  src/ovl/<NAME>.c against asm/ovl/<NAME>/{nonmatchings,matchings}/<NAME>/*.s, one row per overlay
  grand total = main game + overlays
  SDK libs:  the asm-only PsyQ library functions (asm/*.s). Reported on a separate line and
             NOT counted in any total; there is no C for them (T-0010).

Usage: python3 tools/progress.py [--root DIR]
Run inside Docker: tools/docker.sh python3 tools/progress.py
Exit code is non-zero if a function's size cannot be determined.
"""
import argparse
import sys
from pathlib import Path
from typing import NamedTuple

import srcscan


class Totals(NamedTuple):
    functions: int
    nbytes: int
    done_functions: int
    done_bytes: int

    def add(self, size, done):
        return Totals(self.functions + 1, self.nbytes + size,
                      self.done_functions + done, self.done_bytes + (size if done else 0))


ZERO = Totals(0, 0, 0, 0)


def read_sizes(files):
    """{(folder, name): size} from the `nonmatching` headers of the given .s files; raises ValueError."""
    sizes = {}
    for s in files:
        head = srcscan.nonmatching_size(s)
        if not head or head[0] != s.stem:
            raise ValueError("no `nonmatching %s, size` header in %s" % (s.stem, s))
        sizes[(s.parent.name, head[0])] = head[1]
    return sizes


def unit_totals(sizes, sources):
    """Per-folder Totals for one unit. sizes: read_sizes() result; sources: C paths of the unit.

    Raises ValueError if a function has no size, or is neither INCLUDE_ASM nor defined in C."""
    remaining, defined = set(), set()
    for c in sources:
        # src/ovl/pad/*.s are hand-written nop padding between objects, not functions
        remaining.update((Path(e.folder).name, e.name) for e in srcscan.include_asm_entries(c)
                         if Path(e.folder).name != "pad")
        defined |= srcscan.defined_functions(c)
    missing = [k for k in remaining if k not in sizes]
    if missing:
        raise ValueError("no size for %s" % ", ".join(n for _, n in missing[:5]))
    lost = [k for k in sizes if k not in remaining and k[1] not in defined]
    if lost:
        raise ValueError("neither INCLUDE_ASM nor C body for %s" % ", ".join(n for _, n in lost[:5]))
    rows = {}
    for (folder, name), size in sizes.items():
        rows[folder] = rows.get(folder, ZERO).add(size, (folder, name) not in remaining)
    return rows


def asm_files(base):
    return [f for d in ("nonmatchings", "matchings") for f in sorted((base / d).rglob("*.s"))]


def sum_totals(rows):
    total = ZERO
    for t in rows:
        total = Totals(*(a + b for a, b in zip(total, t)))
    return total


def fmt_row(label, t):
    return "%-12s %5d/%-5d %7d/%-7d %5.1f%%" % (label, t.done_functions, t.functions, t.done_bytes, t.nbytes,
                                               100.0 * t.done_bytes / t.nbytes if t.nbytes else 0.0)


def sdk_totals(files):
    """(functions, bytes) of the asm-only SDK library files (top-level asm/*.s `nonmatching` headers)."""
    n = b = 0
    for s in files:
        for m in srcscan.NONMATCHING_RE.finditer(s.read_text(errors="replace")):
            n += 1
            b += int(m.group(2), 0)
    return n, b


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--root", default=".")
    root = Path(ap.parse_args().root)

    try:
        main_sizes = read_sizes(asm_files(root / "asm"))
        if not main_sizes:
            raise ValueError("no asm/nonmatchings found (run ninja first)")
        main_rows = unit_totals(main_sizes, srcscan.source_files(root))
        ovl_rows = {}
        for d in sorted((root / "asm" / "ovl").iterdir()):
            sizes = read_sizes(asm_files(d))
            ovl_rows[d.name] = unit_totals(sizes, [root / "src" / "ovl" / (d.name + ".c")]).get(d.name, ZERO)
        sdk = sdk_totals(sorted((root / "asm").glob("*.s")))
    except (ValueError, OSError) as e:
        print("error: %s" % e, file=sys.stderr)
        return 1

    print("%-12s %12s %18s" % ("main file", "functions", "bytes"))
    for file, t in sorted(main_rows.items()):
        print(fmt_row(file, t))
    main_total = sum_totals(main_rows.values())
    print(fmt_row("main game", main_total))
    print()
    print("%-12s %12s %18s" % ("overlay", "functions", "bytes"))
    for name, t in sorted(ovl_rows.items()):
        print(fmt_row(name, t))
    ovl_total = sum_totals(ovl_rows.values())
    print(fmt_row("overlays", ovl_total))
    print()
    print(fmt_row("grand total", sum_totals([main_total, ovl_total])))
    print("%-12s %5d funcs %9d bytes   asm only, not counted above (no C for SDK libs)" % ("SDK libs", sdk[0], sdk[1]))
    return 0


if __name__ == "__main__":
    sys.exit(main())
