"""Diff census + delay-slot oracle (T-25015 tool 3).

Classifies the diff between a reference object and a compiled
candidate for each packet in a manifest, bucketing functions by the
shape of their remaining gap:

  delay-slot    a nop in one stream sits where the other stream has a
                real instruction next to a branch (an as1 fill choice)
  register-only the differing rows agree once register names are
                canonicalized (an allocator choice, no code change)
  frame         the stack frame size or sp offsets differ
  addressing    rows agree once symbol names are canonicalized
  instruction   anything else (instruction selection or count)

For delay-slot functions the oracle section names each stall/fill
pair: the branch, the instruction the reference filled its slot with,
and where that instruction sits in the candidate stream.

Usage:
  census.py --manifest packets.json [--oracle]
Manifest entries: {"fn": NAME, "ref": OBJ, "cand": OBJ}
"""
import argparse
import difflib
import json
import re
import sys
import os

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from shape_score import norm_rows  # noqa: E402

REG = re.compile(r"\b(zero|at|v[01]|a[0-3]|t[0-9]|s[0-7]|k[01]|gp|sp|fp|ra)\b")
SYM = re.compile(r"\b[DL]_[0-9A-Fa-f]{6,}\b|\bfunc_[0-9A-Fa-f]+\b")
FRAME = re.compile(r"addiu sp,sp,-?\d+")
BRANCH = re.compile(r"^(beq|bne|beqz|bnez|bgtz|blez|bltz|bgez|j|jal|jr|jalr)\b")


def canon(row, regs=True, syms=False):
    if regs:
        row = REG.sub("R", row)
    if syms:
        row = SYM.sub("SYM", row)
    return row


def classify_hunk(ref_rows, cand_rows):
    """Bucket one diff hunk (lists of rows from each stream)."""
    if any(r == "nop" for r in cand_rows) and any(r != "nop" for r in ref_rows):
        return "delay-slot"
    if any(r == "nop" for r in ref_rows) and any(r != "nop" for r in cand_rows):
        return "delay-slot"
    if [canon(r) for r in ref_rows] == [canon(r) for r in cand_rows]:
        return "register-only"
    if any(FRAME.search(r) for r in ref_rows + cand_rows):
        return "frame"
    if [canon(r, syms=True) for r in ref_rows] == [canon(r, syms=True) for r in cand_rows]:
        return "addressing"
    return "instruction"


def census_function(ref_obj, cand_obj, fn):
    a = norm_rows(ref_obj, fn)
    b = norm_rows(cand_obj, fn)
    canon_sm = difflib.SequenceMatcher(None, [canon(r) for r in a],
                                       [canon(r) for r in b], autojunk=False)
    canon_rows = 0
    for tag, i1, i2, j1, j2 in canon_sm.get_opcodes():
        if tag != "equal":
            canon_rows += max(i2 - i1, j2 - j1)
    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
    buckets = {}
    total = 0
    fills = []
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == "equal":
            continue
        total += max(i2 - i1, j2 - j1)
        bucket = classify_hunk(a[i1:i2], b[j1:j2])
        buckets[bucket] = buckets.get(bucket, 0) + max(i2 - i1, j2 - j1)
        if bucket == "delay-slot":
            for k in range(j1, min(j2, len(b))):
                if b[k] == "nop":
                    fills.append({"at_row": k,
                                  "ref_fill": a[i1:i2],
                                  "cand_context": b[max(0, k - 1):k + 2]})
    primary = max(buckets, key=buckets.get) if buckets else "match"
    return {"fn": fn, "rows": total, "rows_canon": canon_rows,
            "buckets": buckets, "primary": primary, "fills": fills}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--manifest", required=True)
    ap.add_argument("--oracle", action="store_true")
    args = ap.parse_args()
    entries = json.load(open(args.manifest))
    results = []
    for e in entries:
        try:
            results.append(census_function(e["ref"], e["cand"], e["fn"]))
        except Exception as ex:
            results.append({"fn": e["fn"], "rows": -1, "buckets": {},
                            "primary": "error: %s" % ex, "fills": []})
    results.sort(key=lambda r: r["rows"])
    counts = {}
    for r in results:
        counts[r["primary"]] = counts.get(r["primary"], 0) + 1
        print("%-18s rows=%-4d primary=%s %s" % (
            r["fn"], r["rows"], r["primary"],
            " ".join("%s=%d" % kv for kv in sorted(r["buckets"].items()))))
        if args.oracle:
            for fill in r["fills"]:
                print("    stall at cand row %d: ref fills with %s (cand context: %s)"
                      % (fill["at_row"], fill["ref_fill"], fill["cand_context"]))
    print("buckets:", counts)
    return 0


if __name__ == "__main__":
    sys.exit(main())
