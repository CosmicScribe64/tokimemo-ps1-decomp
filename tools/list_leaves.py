#!/usr/bin/env python3
"""List remaining INCLUDE_ASM leaf functions (no jal/jalr) of the main exe, smallest first.

Thin wrapper over tools/queue.py (T-1320), kept for old command lines; use `queue.py --leaf` for
the ranked list with blocker flags and the overlays.

Usage: python3 tools/list_leaves.py [--file 80079B10] [--skip LO-HI] [--limit N]
Prints "size name file" lines. --file restricts to files whose name contains the text; --skip takes
a hex address range (default 80080000-80086810).
"""
import argparse
import importlib.util
import re
import sys
from pathlib import Path


def _queue():
    spec = importlib.util.spec_from_file_location("work_queue", Path(__file__).with_name("queue.py"))
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--file", default="")
    ap.add_argument("--skip", default="80080000-80086810")
    ap.add_argument("--limit", type=int, default=0)
    a = ap.parse_args()
    lo, hi = (int(x, 16) for x in a.skip.split("-"))
    wq = _queue()
    remaining, _ = wq.load(".")
    rows = []
    for f in remaining:
        if not re.match(r'^[0-9A-Fa-f]{8}$', f.file) or a.file not in f.file or not f.leaf:
            continue
        am = re.match(r'func_([0-9A-Fa-f]{8})$', f.name)
        if am and lo <= int(am.group(1), 16) < hi:
            continue
        rows.append((f.facts.size, f.name, f.file))
    rows.sort()
    for size, name, file in rows[:a.limit or None]:
        print("%d %s %s" % (size, name, file))
    print("total leaves: %d" % len(rows), file=sys.stderr)


if __name__ == "__main__":
    sys.exit(main())
