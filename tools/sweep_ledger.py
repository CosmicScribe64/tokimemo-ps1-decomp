"""Sweep ledger + batch runner (T-25015 tool 6).

A machine-readable record of what has been tried on each unmatched
function, so brute-force search is cumulative instead of repeated:
every sweep (method, budget, best score) is appended per function,
and the batch runner skips functions already exhausted at the
requested method and budget.

Usage:
  sweep_ledger.py record --ledger L.json --fn NAME --method M
                         --budget N --best SCORE [--note TEXT]
  sweep_ledger.py show   --ledger L.json [--fn NAME]
  sweep_ledger.py batch  --ledger L.json --manifest M.json --tree DIR
                         [--budget N] [--method shape_search]

Batch manifest entries: {"fn", "header", "body", "ref", "out"}; each
runs tools/shape_search.py against the packet bed and records the
final score. Ledger and manifests are work artifacts; keep them out
of the repo.
"""
import argparse
import json
import os
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))


def load(path):
    if os.path.exists(path):
        return json.load(open(path))
    return {}


def save(path, data):
    json.dump(data, open(path, "w"), indent=1)


def record(data, fn, method, budget, best, note=""):
    entry = {"method": method, "budget": budget, "best": best, "note": note}
    data.setdefault(fn, []).append(entry)
    return entry


def exhausted(data, fn, method, budget):
    for e in data.get(fn, []):
        if e["method"] == method and e["budget"] >= budget:
            return True
    return False


def cmd_record(args):
    data = load(args.ledger)
    record(data, args.fn, args.method, args.budget, args.best, args.note)
    save(args.ledger, data)
    print("recorded %s: %s budget=%d best=%s" % (args.fn, args.method, args.budget, args.best))
    return 0


def cmd_show(args):
    data = load(args.ledger)
    names = [args.fn] if args.fn else sorted(data)
    for fn in names:
        for e in data.get(fn, []):
            print("%-16s %-14s budget=%-5d best=%s %s" % (fn, e["method"], e["budget"], e["best"], e["note"]))
    return 0


def cmd_batch(args):
    data = load(args.ledger)
    entries = json.load(open(args.manifest))
    engine = os.path.join(os.path.dirname(os.path.abspath(__file__)), "shape_search.py")
    for e in entries:
        fn = e["fn"]
        if exhausted(data, fn, args.method, args.budget):
            print("skip %s (exhausted at budget %d)" % (fn, args.budget))
            continue
        r = subprocess.run([sys.executable, engine, "--tree", args.tree,
                            "--header", e["header"], "--body", e["body"],
                            "--ref", e["ref"], "--fn", fn,
                            "--out", e["out"], "--budget", str(args.budget)],
                           capture_output=True, text=True)
        best = None
        for line in r.stdout.split("\n"):
            if line.startswith("final:"):
                best = int(line.split()[1])
        record(data, fn, args.method, args.budget, best, "batch")
        save(args.ledger, data)
        print("%s: best=%s" % (fn, best))
    return 0


def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd", required=True)
    r = sub.add_parser("record")
    r.add_argument("--ledger", required=True)
    r.add_argument("--fn", required=True)
    r.add_argument("--method", required=True)
    r.add_argument("--budget", type=int, required=True)
    r.add_argument("--best", type=int, required=True)
    r.add_argument("--note", default="")
    r.set_defaults(func=cmd_record)
    s = sub.add_parser("show")
    s.add_argument("--ledger", required=True)
    s.add_argument("--fn")
    s.set_defaults(func=cmd_show)
    b = sub.add_parser("batch")
    b.add_argument("--ledger", required=True)
    b.add_argument("--manifest", required=True)
    b.add_argument("--tree", required=True)
    b.add_argument("--budget", type=int, default=300)
    b.add_argument("--method", default="shape_search")
    b.set_defaults(func=cmd_batch)
    args = ap.parse_args()
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
