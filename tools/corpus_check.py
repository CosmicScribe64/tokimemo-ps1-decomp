"""Check a pipeline variant against the regression corpus (T-25015).

Recompiles every corpus packet TU with the given environment knobs
set, and compares the normalized rows to the baseline in corpus.json.
A variant is uniform only if zero corpus functions change.

Usage:
  corpus_check.py --tree DIR --corpus corpus.json [--set K=V ...]
Exit code is 0 when uniform, 1 when any corpus function changed,
2 on tool failure. Corpus TUs are rebuilt from src/ headers, so the
corpus JSON must come from corpus_build.py on the same checkout.
"""
import argparse
import json
import os
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from corpus_build import split_functions  # noqa: E402
from shape_score import norm_rows  # noqa: E402


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--tree", required=True)
    ap.add_argument("--corpus", required=True)
    ap.add_argument("--src-root", required=True,
                    help="checkout root holding the corpus source files")
    ap.add_argument("--set", action="append", default=[], dest="knobs")
    args = ap.parse_args()

    corpus = json.load(open(args.corpus))
    by_file = {}
    for name, entry in corpus.items():
        by_file.setdefault(entry["file"], {})[name] = entry

    env = dict(os.environ)
    for kv in args.knobs:
        k, v = kv.split("=", 1)
        env[k] = v

    changed = []
    checked = 0
    for fname, fns in by_file.items():
        # find the source file under src/
        matches = subprocess.run(["find", os.path.join(args.src_root, "src"),
                                  "-name", fname], capture_output=True, text=True).stdout.split()
        if not matches:
            print("missing source file:", fname)
            return 2
        text = open(matches[0]).read()
        bodies = {n: (h, b) for n, h, b in split_functions(text)}
        for name, entry in fns.items():
            if name not in bodies:
                print("missing function:", name)
                return 2
            header, body = bodies[name]
            tu_rel = "build/corpus-tu.c"
            with open(os.path.join(args.tree, tu_rel), "w") as f:
                f.write(header + "\n" + body + "\n")
            r = subprocess.run([sys.executable, "tools/cc.py", tu_rel,
                                "build/corpus-cand.o", "ido", "5.3"],
                               cwd=args.tree, env=env, capture_output=True, text=True)
            if r.returncode != 0:
                print("compile failed:", name)
                return 2
            rows = norm_rows(os.path.join(args.tree, "build/corpus-cand.o"), name)
            checked += 1
            if rows != entry["rows"]:
                changed.append(name)
    print("checked %d corpus functions; %d changed" % (checked, len(changed)))
    for name in changed:
        print("CHANGED:", name)
    return 1 if changed else 0


if __name__ == "__main__":
    sys.exit(main())
