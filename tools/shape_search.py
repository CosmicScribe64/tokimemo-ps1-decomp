"""Packet variant search engine (T-25014).

Lives in tools/ with shape_transforms.py and shape_score.py; run it
from a tree checkout so tools/cc.py and tools/funcdiff.py resolve.

Hill-climbs source transforms (transforms.py) on one function's draft
body against its packet reference object. Each step generates every
one-change variant of the current best text, compiles each in the
scratch tree, scores it with the packet metric (score.py), and keeps
the best improvement. Every trial is logged to a TSV so runs are
reproducible and auditable.

Usage:
  engine.py --tree DIR --header FILE --body FILE --ref OBJ --fn NAME
            --out DIR [--budget N] [--cc-args ...]
The header file holds the TU's include block; the body file holds the
draft function text. Variants replace the body only.
"""
import argparse
import os
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from shape_score import diff_rows  # noqa: E402
import shape_transforms as transforms  # noqa: E402


def compile_tu(tree, header, body_text, out_obj, extra, tag):
    tu_rel = "build/shape-tu-%s.c" % tag
    with open(os.path.join(tree, tu_rel), "w") as f:
        f.write(header)
        f.write(body_text)
    r = subprocess.run(
        [sys.executable, "tools/cc.py", tu_rel, out_obj, "ido", "5.3"] + extra,
        cwd=tree, capture_output=True, text=True)
    return r.returncode == 0


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--tree", required=True)
    ap.add_argument("--header", required=True)
    ap.add_argument("--body", required=True)
    ap.add_argument("--ref", required=True)
    ap.add_argument("--fn", required=True)
    ap.add_argument("--out", required=True)
    ap.add_argument("--budget", type=int, default=300)
    ap.add_argument("--extra", nargs="*", default=[])
    args = ap.parse_args()

    os.makedirs(args.out, exist_ok=True)
    header = open(args.header).read()
    best_text = open(args.body).read()
    tag = os.path.basename(os.path.normpath(args.out))
    obj = "build/shape-cand-%s.o" % tag

    def score(text):
        if not compile_tu(args.tree, header, text, obj, args.extra, tag):
            return None
        return diff_rows(os.path.join(args.tree, args.ref),
                         os.path.join(args.tree, obj), args.fn)

    log = open(os.path.join(args.out, "trials.tsv"), "w")
    log.write("step\ttransform\tscore\n")
    base = score(best_text)
    print("baseline:", base)
    if base is None:
        print("error: the bed body does not compile; check the bed", file=sys.stderr)
        raise SystemExit(2)
    log.write("0\t<base>\t%s\n" % base)
    best = base
    trials = 1
    step = 0
    sideways = 0
    visited = {hash(best_text)}
    while trials < args.budget and best > 0:
        step += 1
        improved = None
        plateau = None
        for name, vt in transforms.variants(best_text):
            if trials >= args.budget:
                break
            if hash(vt) in visited:
                continue
            trials += 1
            s = score(vt)
            log.write("%d\t%s\t%s\n" % (step, name, s if s is not None else "FAIL"))
            log.flush()
            if s is None:
                continue
            if s < best:
                best = s
                improved = (name, vt)
                print("step %d: %s -> %d" % (step, name, s))
            elif s == best and plateau is None:
                plateau = (name, vt)
        if improved is not None:
            sideways = 0
            best_text = improved[1]
        elif plateau is not None and sideways < 4:
            sideways += 1
            best_text = plateau[1]
            print("step %d: sideways via %s (still %d)" % (step, plateau[0], best))
        else:
            print("no improvement at step %d; stopping at %d" % (step, best))
            break
        visited.add(hash(best_text))
        with open(os.path.join(args.out, "best.c"), "w") as f:
            f.write(best_text)
    print("final:", best, "after", trials, "trials")
    with open(os.path.join(args.out, "best.c"), "w") as f:
        f.write(best_text)


if __name__ == "__main__":
    main()
