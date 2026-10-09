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


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--root", default=".")
    root = Path(ap.parse_args().root)

    # (file, name) -> size for every function split by splat; asm/matchings holds decompiled
    # functions (splat disassemble_all), asm/nonmatchings the rest
    sizes = {}
    for s in [f for d in ("nonmatchings", "matchings") for f in (root / "asm" / d).rglob("*.s")]:
        head = srcscan.nonmatching_size(s)
        if not head or head[0] != s.stem:
            print("error: no `nonmatching %s, size` header in %s" % (s.stem, s), file=sys.stderr)
            return 1
        sizes[(s.parent.name, head[0])] = head[1]
    if not sizes:
        print("error: no asm/nonmatchings found (run ninja first)", file=sys.stderr)
        return 1

    remaining = set()
    defined = set()
    for c in srcscan.source_files(root):
        remaining.update((Path(e.folder).name, e.name) for e in srcscan.include_asm_entries(c))
        defined |= srcscan.defined_functions(c)
    missing = [k for k in remaining if k not in sizes]
    if missing:
        print("error: no size for %s" % ", ".join(n for _, n in missing[:5]), file=sys.stderr)
        return 1
    lost = [k for k in sizes if k not in remaining and k[1] not in defined]
    if lost:
        print("error: neither INCLUDE_ASM nor C body for %s" % ", ".join(n for _, n in lost[:5]), file=sys.stderr)
        return 1

    files = {}
    for (file, name), size in sizes.items():
        files[file] = files.get(file, Totals(0, 0, 0, 0)).add(size, (file, name) not in remaining)
    print("%-12s %12s %18s" % ("file", "functions", "bytes"))
    total = Totals(0, 0, 0, 0)
    for file, t in sorted(files.items()):
        print("%-12s %5d/%-5d %7d/%-7d %5.1f%%" % (file, t.done_functions, t.functions, t.done_bytes, t.nbytes,
                                                 100.0 * t.done_bytes / t.nbytes))
        total = Totals(*(a + b for a, b in zip(total, t)))
    print("%-12s %5d/%-5d %7d/%-7d %5.1f%%" % ("total", total.done_functions, total.functions, total.done_bytes,
                                              total.nbytes, 100.0 * total.done_bytes / total.nbytes))
    return 0


if __name__ == "__main__":
    sys.exit(main())
