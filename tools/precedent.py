"""Construct precedent miner (T-25015 tool 5).

Indexes the project's matched source for the source constructs
behind tricky codegen, so a hard diff can be answered with evidence:
"which already-matched functions use this construct, and where".

Patterns indexed per function (real-C bodies only):
  empty-if        `if (x) {}` priority probes
  volatile-global a global declared volatile in the file header
  mul1            `* 1` operand-order wrappers
  goto            goto statements (CFG shaping)
  switch          switch statements (count)
  byte-view       a u8/s8 pointer cast read of a wider value
  field-copy      a local initialized from a global field read

Usage:
  precedent.py build --src-root DIR --out index.json
  precedent.py query --index index.json PATTERN [PATTERN ...]
"""
import argparse
import json
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from corpus_build import split_functions  # noqa: E402

PATTERNS = {
    "empty-if": re.compile(r"if\s*\([^)]*\)\s*\{\s*\}"),
    "mul1": re.compile(r"\*\s*1(?![\d.])"),
    "goto": re.compile(r"\bgoto\s+\w+\s*;"),
    "switch": re.compile(r"\bswitch\s*\("),
    "byte-view": re.compile(r"\(\s*(u8|s8)\s*\*\s*\)"),
    "field-copy": re.compile(r"\b\w+\s+(\w+)\s*=\s*D_[0-9A-F]{8}\.\w+"),
}


VOL_DECL = re.compile(r"^\s*(?:extern\s+)?volatile\s+[\w\s\*]+?(D_[0-9A-F]{8})\s*(?:\[|;)", re.M)


def volatile_symbols(src_root):
    """D_ symbols with a declaration-shaped volatile line in a header
    (include/**/*.h, or a src file's pre-function header block): such a
    declaration is project-wide, so any function reading one was
    compiled with the volatile form."""
    syms = set()
    base = os.path.join(src_root, "include")
    for root, _, files in os.walk(base):
        for fname in files:
            if fname.endswith(".h"):
                syms.update(VOL_DECL.findall(open(os.path.join(root, fname)).read()))
    base = os.path.join(src_root, "src")
    for root, _, files in os.walk(base):
        for fname in files:
            if fname.endswith(".c"):
                text = open(os.path.join(root, fname)).read()
                fns = split_functions(text)
                header = fns[0][1] if fns else ""
                syms.update(VOL_DECL.findall(header))
    return syms


def scan_function(body, header, vol_syms=()):
    hits = {}
    for name, rx in PATTERNS.items():
        n = len(rx.findall(body))
        if n:
            hits[name] = n
    if VOL_DECL.search(header) or any(s in body for s in vol_syms):
        hits["volatile-global"] = 1
    return hits


def build(args):
    index = {}
    vol_syms = volatile_symbols(args.src_root)
    for root, _, files in os.walk(os.path.join(args.src_root, "src")):
        for fname in files:
            if not fname.endswith(".c"):
                continue
            text = open(os.path.join(root, fname)).read()
            for name, header, body in split_functions(text):
                hits = scan_function(body, header, vol_syms)
                if hits:
                    index[name] = {"file": fname, "hits": hits}
    json.dump(index, open(args.out, "w"), indent=1)
    counts = {}
    for entry in index.values():
        for k in entry["hits"]:
            counts[k] = counts.get(k, 0) + 1
    print("indexed %d functions with constructs:" % len(index), counts)
    return 0


def query(args):
    index = json.load(open(args.index))
    for pat in args.patterns:
        rows = [(fn, e) for fn, e in index.items() if pat in e["hits"]]
        print("%s: %d matched functions" % (pat, len(rows)))
        for fn, e in rows[:args.limit]:
            print("  %-16s %s (x%d)" % (fn, e["file"], e["hits"][pat]))
    return 0


def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd", required=True)
    b = sub.add_parser("build")
    b.add_argument("--src-root", required=True)
    b.add_argument("--out", required=True)
    b.set_defaults(func=build)
    q = sub.add_parser("query")
    q.add_argument("--index", required=True)
    q.add_argument("--limit", type=int, default=10)
    q.add_argument("patterns", nargs="+")
    q.set_defaults(func=query)
    args = ap.parse_args()
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
