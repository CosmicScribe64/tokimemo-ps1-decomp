#!/usr/bin/env python3
"""Find functions that PsyQ gcc (+ maspsx) built, from the splat disassembly (T-9200).

usage: gcc_fingerprint.py [--units UNIT,...] [--all] [--matched] [--root DIR]
       (run after a split: reads asm/**/{nonmatchings,matchings}/<object>/*.s; default: the
       non-matching functions of every overlay and the main exe; --all adds one line per
       object that holds a hit; --matched also scans the matched functions as a control)

The game code is IDO output; the SDK libraries are gcc 2.7.x + ASPSX or hand-written assembly.
Both SDK kinds share instruction habits that IDO does not have. Two are used, each one measured
on every matched (IDO) function of the tree, where it occurs 0 times in 4521:
  move      `addu rd,rs,$zero` (or `addu rd,$zero,rs`) as the register move; IDO writes
            `or rd,rs,$zero`, and `addu` with $zero never comes out of IDO for any C.
  jlocal    `j .Lxxxx`, a plain jump to a label inside the function; IDO uses `b`.
Both occur in the libgte, libgpu, libsnd, libspu, libcd, libetc, libgs, libpress and libapi
asm of the main exe (every library has them; 38 of 61 libcd and 50 of 96 libgpu functions have a
`move`), so they fingerprint the SDK compilers, not a single function. A single `move` in a
large function is not enough (three IDO-shaped functions of 230 to 1100 instructions have one or
two), so a hit needs `jlocal`, or at least one `move` per 32 instructions.

The fingerprint cannot tell gcc C from hand-written libgte assembly (RotMatrixZ is hand-written
and has the same fingerprint); whether a hit is plain C is decided by compiling it (`cc.py ...
gcc 2.7.2-psx 2.79`) and diffing. It also misses gcc code with neither habit (a leaf function
without a move or a local jump); such a function shows as an IDO near-miss only.
"""
import argparse
import re
import sys
from pathlib import Path

INS_RE = re.compile(r'^\s*/\* \S+ \S+ \S+ \*/\s+(\S+)\s*(.*?)\s*$')
GLABEL_RE = re.compile(r'^glabel (\w+)')
# one `move` per 32 instructions
MOVE_DENSITY = 32


def is_move(op, args):
    """`addu rd,rs,$zero` or `addu rd,$zero,rs`: the move that IDO spells `or`."""
    if op != "addu":
        return False
    parts = [a.strip() for a in args.split(",")]
    return len(parts) == 3 and "$zero" in parts[1:]


def is_local_jump(op, args):
    return op == "j" and args.startswith(".L")


def features(lines):
    """(instructions, moves, local jumps) of one function's splat listing."""
    n = moves = jumps = 0
    for line in lines:
        m = INS_RE.match(line)
        if not m:
            continue
        op, args = m.groups()
        n += 1
        moves += is_move(op, args)
        jumps += is_local_jump(op, args)
    return n, moves, jumps


def is_sdk_style(n, moves, jumps):
    """True when the counts fingerprint PsyQ gcc / SDK code (module docstring)."""
    return jumps > 0 or (moves > 0 and n > 0 and moves * MOVE_DENSITY >= n)


def split_functions(text):
    """{name: listing lines} of a splat .s file that may hold one or more `glabel`s."""
    funcs, cur = {}, None
    for line in text.splitlines():
        m = GLABEL_RE.match(line)
        if m and not m.group(1).startswith("jtbl"):
            cur = funcs.setdefault(m.group(1), [])
        elif cur is not None:
            cur.append(line)
    return funcs


def scan(root, units=None, matched=False):
    """[(unit, object, function, n, moves, jumps)] for every function with a fingerprint hit."""
    root = Path(root)
    kinds = ("nonmatchings", "matchings") if matched else ("nonmatchings",)
    out = []
    for kind in kinds:
        for p in sorted(root.glob("asm/**/%s/**/*.s" % kind)):
            rel = p.relative_to(root / "asm").parts
            if rel[0] == "ovl":
                unit, obj = rel[1], rel[-2]
            else:
                unit, obj = "main", rel[-2]
            if units and unit not in units:
                continue
            for name, lines in split_functions(p.read_text(errors="replace")).items():
                n, moves, jumps = features(lines)
                if is_sdk_style(n, moves, jumps):
                    out.append((unit, obj, name, n, moves, jumps, kind))
    return out


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--units", default="", help="comma separated: main, overlay names")
    ap.add_argument("--matched", action="store_true", help="also scan matched functions (control)")
    ap.add_argument("--root", default=".")
    a = ap.parse_args(argv)
    units = {u for u in a.units.split(",") if u}
    hits = scan(a.root, units, a.matched)
    for unit, obj, name, n, moves, jumps, kind in hits:
        print("%-9s %-9s %-14s insns=%-4d move=%d jlocal=%d %s" % (unit, obj, name, n, moves, jumps, kind))
    print("%d function(s) with the PsyQ gcc / SDK fingerprint" % len(hits))
    return 0


if __name__ == "__main__":
    sys.exit(main())
