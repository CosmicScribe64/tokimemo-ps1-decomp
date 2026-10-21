"""Rule fuzzing lab (T-25015 tool 4).

Systematically enumerates candidate pipeline knobs and validates each
in two stages, replacing hand-run one-off sweeps:

  singles   every OPTN option (0..530; values 0, plus 1 and 2 for
            400..464) scored against the target beds only. Knobs that
            change no target are inert and need no gate.
  gate      the active knobs from singles are run against the
            regression corpus (corpus_build.py): a knob is a uniform
            rule candidate only if it changes zero corpus functions.
  pairs     pairwise combinations of the active knobs, scored on the
            targets; improving pairs go through the gate the same way.

Usage:
  rule_fuzz.py singles --tree DIR --bed H,B,REF,FN [--bed ...] --out R.json
  rule_fuzz.py gate    --tree DIR --corpus C.json --src-root DIR --in R.json
  rule_fuzz.py pairs   --tree DIR --bed ... --in R.json --out R2.json
"""
import argparse
import itertools
import json
import os
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from corpus_build import split_functions  # noqa: E402
from shape_score import diff_rows, norm_rows  # noqa: E402


def compile_bed(tree, bed, env):
    header, body = bed["header"], bed["body"]
    tu_rel = "build/fuzz-tu.c"
    with open(os.path.join(tree, tu_rel), "w") as f:
        f.write(open(header).read())
        f.write(open(body).read())
    r = subprocess.run([sys.executable, "tools/cc.py", tu_rel,
                        "build/fuzz-cand.o", "ido", "5.3"],
                       cwd=tree, env=env, capture_output=True, text=True)
    if r.returncode != 0:
        return None
    return diff_rows(os.path.join(tree, bed["ref"]),
                     os.path.join(tree, "build/fuzz-cand.o"), bed["fn"])


def corpus_delta(tree, corpus_path, src_root, env):
    corpus = json.load(open(corpus_path))
    by_file = {}
    for name, entry in corpus.items():
        by_file.setdefault(entry["file"], {})[name] = entry
    changed = []
    for fname, fns in by_file.items():
        matches = subprocess.run(["find", os.path.join(src_root, "src"),
                                  "-name", fname], capture_output=True, text=True).stdout.split()
        text = open(matches[0]).read()
        bodies = {n: (h, b) for n, h, b in split_functions(text)}
        for name, entry in fns.items():
            header, body = bodies[name]
            tu_rel = "build/fuzz-corpus.c"
            with open(os.path.join(tree, tu_rel), "w") as f:
                f.write(header + "\n" + body + "\n")
            r = subprocess.run([sys.executable, "tools/cc.py", tu_rel,
                                "build/fuzz-corpus.o", "ido", "5.3"],
                               cwd=tree, env=env, capture_output=True, text=True)
            if r.returncode != 0:
                return None
            rows = norm_rows(os.path.join(tree, "build/fuzz-corpus.o"), name)
            if rows != entry["rows"]:
                changed.append(name)
    return changed


def knob_env(base_env, pairs):
    env = dict(base_env)
    parts = []
    for n, v in pairs:
        parts.append("%d:%d" % (n, v))
    env["TOKIMEMO_OPTN"] = ",".join(parts) if parts else ""
    return env


def parse_beds(specs):
    beds = []
    for s in specs:
        header, body, ref, fn = s.split(",")
        beds.append({"header": header, "body": body, "ref": ref, "fn": fn})
    return beds


def singles(args):
    beds = parse_beds(args.bed)
    base_env = dict(os.environ)
    base = [compile_bed(args.tree, b, knob_env(base_env, [])) for b in beds]
    print("baselines:", base)
    active = []
    for n in range(0, 531):
        for v in ((0,) if n < 400 or n > 464 else (0, 1, 2)):
            env = knob_env(base_env, [(n, v)])
            scores = [compile_bed(args.tree, b, env) for b in beds]
            if scores != base:
                active.append({"knob": [n, v], "scores": scores})
                print("active: OPTN %d:%d -> %s (base %s)" % (n, v, scores, base))
    json.dump({"baselines": base, "active": active}, open(args.out, "w"), indent=1)
    print("singles done: %d active knobs" % len(active))
    return 0


def gate(args):
    data = json.load(open(args.inp))
    base_env = dict(os.environ)
    for entry in data["active"]:
        n, v = entry["knob"]
        changed = corpus_delta(args.tree, args.corpus, args.src_root,
                                knob_env(base_env, [(n, v)]))
        entry["corpus_changed"] = changed
        print("OPTN %d:%d corpus_changed=%s" % (n, v, len(changed) if changed is not None else "ERR"))
    json.dump(data, open(args.inp, "w"), indent=1)
    return 0


def pairs(args):
    data = json.load(open(args.inp))
    beds = parse_beds(args.bed)
    base_env = dict(os.environ)
    base = data["baselines"]
    knobs = [tuple(e["knob"]) for e in data["active"]]
    results = []
    for a, b in itertools.combinations(knobs, 2):
        env = knob_env(base_env, [a, b])
        scores = [compile_bed(args.tree, bd, env) for bd in beds]
        if scores != base:
            results.append({"pair": [a, b], "scores": scores})
            print("pair %s + %s -> %s (base %s)" % (a, b, scores, base))
    data["pairs"] = results
    json.dump(data, open(args.out, "w"), indent=1)
    print("pairs done: %d interacting pairs" % len(results))
    return 0


def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd", required=True)
    for name in ("singles", "pairs"):
        p = sub.add_parser(name)
        p.add_argument("--tree", required=True)
        p.add_argument("--bed", action="append", required=True)
        if name == "singles":
            p.add_argument("--out", required=True)
            p.set_defaults(func=singles)
        else:
            p.add_argument("--in", required=True, dest="inp")
            p.add_argument("--out", required=True)
            p.set_defaults(func=pairs)
    g = sub.add_parser("gate")
    g.add_argument("--tree", required=True)
    g.add_argument("--corpus", required=True)
    g.add_argument("--src-root", required=True)
    g.add_argument("--in", required=True, dest="inp")
    g.set_defaults(func=gate)
    args = ap.parse_args()
    if "TOKIMEMO_OPTN" in os.environ:
        del os.environ["TOKIMEMO_OPTN"]
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
