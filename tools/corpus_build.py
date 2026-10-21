"""Build the matched-function regression corpus (T-25015 tool 1).

Extracts real-C functions (no INCLUDE_ASM) from project source files,
builds one packet TU per function (the file's own header block plus
that function), compiles each with the unmodified pipeline in a tree
checkout, and stores the normalized instruction rows (shape_score) as
the baseline. The corpus is the local uniformity gate: a pipeline
variant that changes any corpus function's rows is not uniform.

Usage:
  corpus_build.py --tree DIR --out corpus.json [--per-file N] FILE...
Corpus data is a build artifact; keep it out of the repo.
"""
import argparse
import json
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from shape_score import norm_rows  # noqa: E402

FUNC_START = re.compile(r"^[A-Za-z_][\w\s\*]*?\b(func_[0-9A-Fa-f]+)\s*\(")


def split_functions(text):
    """Yield (name, header, body) for each top-level function."""
    lines = text.split("\n")
    starts = []
    for i, ln in enumerate(lines):
        m = FUNC_START.match(ln)
        if m and not ln.rstrip().endswith(";"):
            starts.append((i, m.group(1)))
    header = "\n".join(lines[:starts[0][0]]) if starts else ""
    out = []
    for k, (i, name) in enumerate(starts):
        end = starts[k + 1][0] if k + 1 < len(starts) else len(lines)
        # trim trailing blank/comment-only lines belonging to the next fn
        body_lines = lines[i:end]
        while body_lines and body_lines[-1].strip() == "":
            body_lines.pop()
        body = "\n".join(body_lines)
        if "INCLUDE_ASM" in body or "INCLUDE_RODATA" in body:
            continue
        out.append((name, header, body))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--tree", required=True)
    ap.add_argument("--out", required=True)
    ap.add_argument("--per-file", type=int, default=8)
    ap.add_argument("files", nargs="+")
    args = ap.parse_args()

    corpus = {}
    skipped = []
    for path in args.files:
        text = open(path).read()
        got = 0
        for name, header, body in split_functions(text):
            if got >= args.per_file:
                break
            tu_rel = "build/corpus-tu.c"
            with open(os.path.join(args.tree, tu_rel), "w") as f:
                f.write(header + "\n" + body + "\n")
            r = subprocess.run([sys.executable, "tools/cc.py", tu_rel,
                                "build/corpus-cand.o", "ido", "5.3"],
                               cwd=args.tree, capture_output=True, text=True)
            if r.returncode != 0:
                skipped.append(name)
                continue
            try:
                rows = norm_rows(os.path.join(args.tree, "build/corpus-cand.o"), name)
            except Exception:
                skipped.append(name)
                continue
            corpus[name] = {"file": os.path.basename(path), "rows": rows}
            got += 1
    with open(args.out, "w") as f:
        json.dump(corpus, f, indent=1)
    print("corpus: %d functions, %d skipped (compile/extract)" % (len(corpus), len(skipped)))


if __name__ == "__main__":
    main()
