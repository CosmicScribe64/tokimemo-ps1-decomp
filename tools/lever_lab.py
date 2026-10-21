#!/usr/bin/env python3
"""One-command measurement for compiler-level lever experiments (T-25013); run inside Docker.

A lever (a change to tools/cvt_pass.py, tools/ugen_pass.py, tools/cc.py or an env knob they read)
is only adoptable if it changes no matched function and unblocks something. This tool answers both
questions with one compact report instead of many manual steps:

    tools/docker.sh python3 tools/lever_lab.py run base               # baseline, once per tree state
    tools/docker.sh python3 tools/lever_lab.py run myrule --env CVT_MYRULE=1   # variant, vs base
    tools/docker.sh python3 tools/lever_lab.py show myrule             # reprint the saved report

`run NAME` (1) touches tools/cc.py (build.ninja does not list the pass files as dependencies),
rebuilds every object with `ninja -k 0` under the given environment, (2) snapshots the objects to
build/lever_lab/NAME/, (3) compares them with the base snapshot function by function (bytes and
relocations, trailing pad nops ignored): any changed MATCHED function disqualifies the rule,
(4) re-scores every guarded (`#ifdef NON_MATCHING`) function with funcdiff --resolve --version c
and lists which improved, worsened or reached 0 rows against the base scores. A guarded function at
0 rows is a candidate to promote (still needs the 27/27 gate). The report is also written to
build/lever_lab/NAME/report.txt.
"""
import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
LAB = ROOT / "build" / "lever_lab"
GUARD = re.compile(r"#ifdef NON_MATCHING.*?#else\s*INCLUDE_ASM\(\"([^\"]+)\",\s*(\w+)\)", re.S)


def unit_of(path):
    """Unit name for a src file: src/main/x.c -> main, src/ovl/UNIT/x.c -> UNIT."""
    parts = Path(path).relative_to(ROOT / "src").parts
    return "main" if parts[0] == "main" else parts[1]


def guarded_functions(root=ROOT):
    """(unit, function) for every function kept under `#ifdef NON_MATCHING` with INCLUDE_ASM in #else."""
    found = []
    for f in sorted((root / "src").rglob("*.c")):
        text = f.read_text(errors="ignore")
        for m in GUARD.finditer(text):
            found.append((unit_of(f), m.group(2)))
    return sorted(set(found))


def object_functions(path):
    """{function: [normalised disassembly lines]} of one object (addresses dropped)."""
    out = subprocess.run(["mips-linux-gnu-objdump", "-dr", "-z", str(path)], capture_output=True,
                         text=True).stdout
    res, cur, body = {}, None, []
    for line in out.splitlines():
        m = re.match(r"^[0-9a-f]+ <([^>]+)>:$", line)
        if m:
            if cur:
                res[cur] = body
            cur, body = m.group(1), []
            continue
        if cur and line.strip():
            line = re.sub(r"^\s*[0-9a-f]+:\s*", "", line)
            line = re.sub(r"\b[0-9a-f]+ <", "<", line)
            if "jal" in line and "<" in line and "+0x" not in line.split("<", 1)[1]:
                line = re.sub(r"^[0-9a-f]{8}", "jal-bytes", line)
            body.append(line)
    if cur:
        res[cur] = body
    return res


def trim(body):
    while body and body[-1].startswith("00000000"):
        body = body[:-1]
    return body


def changed_functions(base_dir, new_dir):
    """Functions whose code differs between two object snapshots; returns (changed, total)."""
    changed, total = [], 0
    for obj in sorted(Path(base_dir).rglob("*.o")):
        rel = obj.relative_to(base_dir)
        new = Path(new_dir) / rel
        fb = object_functions(obj)
        fn = object_functions(new) if new.exists() else {}
        for name, body in fb.items():
            total += 1
            if trim(fn.get(name) or []) != trim(body):
                changed.append("%s %s" % (rel, name))
    return changed, total


def snapshot(dest):
    shutil.rmtree(dest, ignore_errors=True)
    for top in ("src", "ovl"):
        for obj in (ROOT / "build" / top).rglob("*.o"):
            target = dest / obj.relative_to(ROOT / "build")
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(obj, target)


def count_rows(text):
    """0 for a MATCH line, the number of resolved diff rows for a DIFF, -1 if funcdiff gave neither
    (a tool error, for example parallel runs colliding); -1 results are retried serially."""
    if re.search(r": MATCH", text):
        return 0
    n = sum(1 for line in text.splitlines() if re.match(r"^  [-+]", line))
    return n if n or "DIFF" in text else -1


def score_all(guards, env, jobs):
    """{unit:func: rows} of every guarded function (NON_MATCHING build), `jobs` funcdiff runs at a time.

    Plain subprocess polling instead of concurrent.futures: tools/ holds a module that shadows the
    stdlib `queue`, which concurrent.futures imports."""
    pending, running, scores = list(guards), [], {}
    while pending or running:
        while pending and len(running) < jobs:
            unit, func = pending.pop(0)
            p = subprocess.Popen([sys.executable, "tools/funcdiff.py", "--resolve", "--version", "c",
                                  "--unit", unit, func], cwd=ROOT, env=env, stdout=subprocess.PIPE,
                                 stderr=subprocess.STDOUT, text=True)
            running.append(("%s:%s" % (unit, func), p))
        still = []
        for key, p in running:
            if p.poll() is None:
                still.append((key, p))
            else:
                scores[key] = count_rows(p.stdout.read())
        running = still
        if running:
            time.sleep(0.2)
    for key in [k for k, v in scores.items() if v < 0]:
        unit, func = key.split(":", 1)
        p = subprocess.run([sys.executable, "tools/funcdiff.py", "--resolve", "--version", "c", "--unit",
                            unit, func], cwd=ROOT, env=env, capture_output=True, text=True)
        scores[key] = count_rows(p.stdout + p.stderr)
    return scores


def compare_scores(base, new):
    """Lines describing guarded functions that improved, worsened, or reached 0 rows."""
    lines = []
    for key in sorted(new):
        if key not in base:
            continue
        b, n = base[key], new[key]
        if n == b:
            continue
        tag = "ZERO " if n == 0 else ("better" if n < b else "WORSE ")
        lines.append("  %s %s: %d -> %d" % (tag, key, b, n))
    return lines


def run(name, envs, jobs):
    env = dict(os.environ)
    for kv in envs:
        k, _, v = kv.partition("=")
        env[k] = v
    (ROOT / "tools" / "cc.py").touch()
    build = subprocess.run(["ninja", "-j", str(jobs), "-k", "0"], cwd=ROOT, env=env, capture_output=True,
                           text=True)
    failed = [l for l in (build.stdout + build.stderr).splitlines() if l.startswith("FAILED")]
    oks = len(list((ROOT / "build").glob("SLPM_*.ok"))) + len(list((ROOT / "build" / "ovl").glob("*.ok")))
    dest = LAB / name
    snapshot(dest / "obj")
    out = ["lever_lab %s env=%s" % (name, " ".join(envs) or "-"),
           "build: %d FAILED, %d .ok files (27 expected)" % (len(failed), oks)]
    out += ["  " + l for l in failed[:10]]
    base_obj = LAB / "base" / "obj"
    if name != "base" and base_obj.exists():
        changed, total = changed_functions(base_obj, dest / "obj")
        out.append("matched functions changed vs base: %d of %d%s" % (
            len(changed), total, "  <- RULE IS NOT UNIFORM, reject" if changed else ""))
        out += ["  " + c for c in changed[:40]]
    guards = guarded_functions()
    scores = score_all(guards, env, jobs)
    (dest).mkdir(parents=True, exist_ok=True)
    (dest / "scores.json").write_text(json.dumps(scores, indent=1, sort_keys=True))
    out.append("guarded functions scored: %d, at 0 rows: %s" % (
        len(scores), ", ".join(k for k, v in scores.items() if v == 0) or "none"))
    base_scores = LAB / "base" / "scores.json"
    if name != "base" and base_scores.exists():
        diff = compare_scores(json.loads(base_scores.read_text()), scores)
        out.append("guarded changes vs base: %d" % len(diff))
        out += diff
    report = "\n".join(out)
    (dest / "report.txt").write_text(report + "\n")
    print(report)
    return 1 if failed else 0


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    r = sub.add_parser("run")
    r.add_argument("name")
    r.add_argument("--env", action="append", default=[], help="K=V passed to the build (repeatable)")
    r.add_argument("-j", "--jobs", type=int, default=4)
    s = sub.add_parser("show")
    s.add_argument("name")
    a = ap.parse_args()
    if a.cmd == "show":
        print((LAB / a.name / "report.txt").read_text(), end="")
        return 0
    return run(a.name, a.env, a.jobs)


if __name__ == "__main__":
    sys.exit(main())
