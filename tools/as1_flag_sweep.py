#!/usr/bin/env python3
"""Compile one C file under a list of compiler flag variants and count the functions that match (T-24800).

Tests whether a per-file option (as1 scheduling switches, driver options that reach ugen/as1) explains the
order-only near misses (wiki/as1-scheduling.md). The file's full translation unit is preprocessed once with
NON_MATCHING defined (funcdiff_verify.preprocess_draft), so the guarded drafts are compiled as C, and then
compiled like the build (tools/cc.py, trailing pad, object trim) once per variant. Every function of the
object is compared with the original-side object (funcdiff.expected_object, the all-INCLUDE_ASM build of the
original disassembly) by funcdiff's resolved rows (relocations applied, as `funcdiff.py --resolve`).

A variant is a whitespace-separated list of tokens:
  -Olimit 3000      a driver option, appended to tools/cc.py IDO_CFLAGS (`-Wb,...`, `-Wo,...` included)
  cc-:-O2           remove that driver option from IDO_CFLAGS
  as1:-noxbb        an option appended to as1's own command line (through the frame_pass as1 shim)
  as1-:-O2          remove that option from as1's command line
The empty variant is the baseline (the build's flags, with the object's own config/toolchains.txt options). A variant `ok` only if every function that matches
under the baseline still matches; `gain` lists guarded functions that match under it but not the baseline.

Usage (in Docker, from the repository root):
  python3 tools/as1_flag_sweep.py FILE.c [-V VARIANT ...] [--variants LIST] [--standard] [-j 4]
                                  [--json OUT] [--show-args]
  --standard runs the built-in variant list; --variants reads one variant per line (`#` comments);
  --show-args prints the as1 command line the baseline compile uses. Scratch files go to build/scratch/as1flags/.
"""
import argparse
import json
import os
import shlex
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = os.path.dirname(os.path.abspath(__file__))
# tools/queue.py shadows the standard queue module that concurrent.futures needs
sys.path = [p for p in sys.path if os.path.abspath(p or ".") != HERE]
import concurrent.futures.thread  # noqa: E402
sys.path.insert(0, HERE)

SCRATCH = Path("build/scratch/as1flags")
SELF = os.path.abspath(__file__)

AS1_LEVEL = (["as1:-O%d" % n for n in (0, 1, 3)] + ["as1:-g%d" % n for n in (0, 1, 2, 3)] +
             ["as1:-p0", "as1:-p1", "as1:-noxbb", "as1:-aggr_xbb", "as1:-nopeep", "as1:-noswpipe",
              "as1:-nosymregs", "as1:-nobopt", "as1:-noglobal", "as1:-no_const_opts", "as1:-no_lui_opts",
              "as1:-no_div_rem_opts", "as1:-no_at_compression", "as1:-no_branch_target", "as1:-nopool",
              "as1:-newhilo", "as1:-fpstall_nop", "as1:-r3000", "as1:-r4000", "as1:-pic0", "as1:-pic1",
              "as1:-trapuv", "as1:-Olimit as1:50", "as1:-Olimit as1:200", "as1:-Olimit as1:500", "as1:-Olimit as1:1000"])
DRIVER = ["cc-:-O2 -O1", "cc-:-O2 -O3", "-g3", "-g", "-g2", "-Olimit 100", "-Olimit 3000",
          "-Wo,-loopunroll,0", "-Wb,-noshortcut", "-Wb,-noxbb", "-Wb,-nopeep", "-Wb,-noswpipe", "-Wb,-nosymregs",
          "-Wb,-nobopt", "-Wb,-noglobal", "-Wb,-aggr_xbb", "-mips2", "-Wo,-noloopopt", "-p"]
COMBOS = ["as1:-noxbb as1:-nopeep", "as1:-noxbb as1:-nobopt", "as1:-nopeep as1:-nobopt",
          "as1:-noxbb as1:-nopeep as1:-nobopt", "as1:-noglobal as1:-noxbb", "as1:-O1 as1:-noxbb",
          "-g3 cc-:-O2 -O2", "as1:-no_lui_opts as1:-noxbb", "as1:-nosymregs as1:-noxbb"]
STANDARD = AS1_LEVEL + DRIVER + COMBOS


def parse_variant(text):
    """(driver adds, driver removes, as1 adds, as1 removes) of a variant string."""
    add, rm, a_add, a_rm = [], [], [], []
    for tok in shlex.split(text):
        if tok.startswith("as1:"):
            a_add.append(tok[4:])
        elif tok.startswith("as1-:"):
            a_rm.append(tok[5:])
        elif tok.startswith("cc-:"):
            rm.append(tok[4:])
        else:
            add.append(tok)
    return add, rm, a_add, a_rm


def as1_shim(argv):
    """as1 stand-in: edit as1's arguments from the environment, then run the frame pass and the real as1
    (frame_pass.run_as1, with `-Olimit N` also known as an option with a value)."""
    import frame_pass
    real, args = argv[0], list(argv[1:])
    spec = json.loads(os.environ.get("AS1_SWEEP", "{}"))
    args = spec.get("add", []) + [a for a in args if a not in spec.get("remove", [])]
    log = os.environ.get("AS1_SWEEP_LOG")
    if log:
        with open(log, "a") as f:
            f.write(" ".join([real] + args) + "\n")
    i, inp = 0, None
    while i < len(args):
        if args[i] in ("-G", "-o", "-t", "-temp", "-Olimit"):
            i += 2
        else:
            if not args[i].startswith("-"):
                inp = i
            i += 1
    with open(args[inp], "rb") as f:
        data = frame_pass.transform(f.read())
    fd, tmp = tempfile.mkstemp(prefix="framepass", suffix=".G")
    with os.fdopen(fd, "wb") as f:
        f.write(data)
    args[inp] = tmp
    try:
        return os.spawnv(os.P_WAIT, real, [real] + args)
    finally:
        os.unlink(tmp)


def compile_variant(unit_c, obj, variant):
    """Compile the preprocessed unit with tools/cc.py's pipeline under `variant` (run in a child process)."""
    import cc
    add, rm, a_add, a_rm = parse_variant(variant)
    cc.IDO_CFLAGS = [f for f in cc.IDO_CFLAGS if f not in rm] + add
    cc.SHIMS = dict(cc.SHIMS, as1=SELF)
    os.environ["AS1_SWEEP"] = json.dumps({"add": a_add, "remove": a_rm})
    cc.main(["cc.py", unit_c, obj] + json.loads(os.environ.get("AS1_SWEEP_TOOLCHAIN", '["ido", "5.3"]')))


def build(c, pre, variant, job, log=None):
    """Object path of the unit compiled under `variant`, or (None, error text)."""
    import cc
    import object_layout
    import trailing_pad
    job.mkdir(parents=True, exist_ok=True)
    unit_c, obj = job / "unit.c", job / "unit.o"
    unit_c.write_text(pre)
    env = dict(os.environ, AS1_SWEEP_TOOLCHAIN=json.dumps(cc.toolchain_of(c.name)))
    if log:
        env["AS1_SWEEP_LOG"] = str(log)
    proc = subprocess.run(["python3", SELF, "--compile", variant, str(unit_c), str(obj)],
                          stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, env=env)
    if proc.returncode != 0 or not obj.exists():
        return None, proc.stdout.strip().splitlines()[-3:]
    includes = {name for _, name in cc.INCLUDE_ASM_RE.findall(pre)}
    trailing_pad.pad_functions(str(obj), str(c.src), includes)
    obj.write_bytes(object_layout.trim(obj.read_bytes(), object_layout.source_extent(c.src)))
    return str(obj), None


def score(obj, names, want):
    import funcdiff
    got = funcdiff.resolved_functions(obj, names)
    return {n for n in names if got[n] is not None and want[n] is not None
            and funcdiff.keys(got[n]) == funcdiff.keys(want[n])}


def main(argv):
    if argv[:1] == ["--as1"]:
        return as1_shim(argv[1:])
    if argv[:1] == ["--compile"]:
        return compile_variant(argv[2], argv[3], argv[1])
    import funcdiff
    import funcdiff_verify
    import srcscan
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("src")
    ap.add_argument("-V", "--variant", action="append", default=[])
    ap.add_argument("--variants")
    ap.add_argument("--standard", action="store_true")
    ap.add_argument("-j", type=int, default=4)
    ap.add_argument("--json")
    ap.add_argument("--show-args", action="store_true")
    args = ap.parse_args(argv)
    variants = list(args.variant)
    if args.standard:
        variants += STANDARD
    if args.variants:
        variants += [l.strip() for l in open(args.variants) if l.strip() and not l.startswith("#")]
    src = os.path.normpath(args.src)
    c = next((f for f in srcscan.c_files() if os.path.normpath(str(f.src)) == src), None)
    if c is None:
        sys.exit("as1_flag_sweep.py: %s is not a C file of the build" % src)
    exp, problem = funcdiff.expected_object(c)
    if problem:
        sys.exit("as1_flag_sweep.py: " + problem)
    names = sorted(funcdiff.split_functions(funcdiff.disassemble(exp)))
    want = funcdiff.resolved_functions(exp, names, c)
    guarded = sorted(n for n in srcscan.defined_functions(Path(src)) if funcdiff_verify.guarded(src, n))
    pre = funcdiff_verify.preprocess_draft(src, True)
    SCRATCH.mkdir(parents=True, exist_ok=True)
    root = Path(tempfile.mkdtemp(dir=SCRATCH, prefix=Path(src).stem + "-"))
    log = root / "as1-args.txt" if args.show_args else None
    base, err = build(c, pre, "", root / "base", log)
    if base is None:
        sys.exit("as1_flag_sweep.py: baseline compile failed: %s" % err)
    if log:
        print("as1 command line (baseline):", log.read_text().strip())
    ok0 = score(base, names, want)
    print("baseline differs: %s" % ", ".join(sorted(set(names) - ok0)))
    print("%s: %d functions, %d match under the baseline (NON_MATCHING), guarded: %s"
          % (src, len(names), len(ok0), ", ".join("%s%s" % (g, "*" if g in ok0 else "") for g in guarded) or "-"))
    rows = []

    def run(i, v):
        obj, e = build(c, pre, v, root / ("v%03d" % i))
        if obj is None:
            return v, None, e
        return v, score(obj, names, want), None

    with concurrent.futures.ThreadPoolExecutor(max(1, args.j)) as ex:
        for v, ok, e in ex.map(lambda p: run(*p), enumerate(variants)):
            if ok is None:
                print("%-40s compile failed: %s" % (v, " | ".join(e or [])))
                rows.append({"variant": v, "error": e})
                continue
            lost, gain = sorted(ok0 - ok), sorted(n for n in ok - ok0)
            print("%-40s match %3d  lost %3d  gain %s%s" % (v, len(ok), len(lost), ",".join(gain) or "-",
                  "  OK" if not lost and gain else ""))
            rows.append({"variant": v, "match": len(ok), "lost": lost, "gain": gain})
    if args.json:
        with open(args.json, "w") as f:
            json.dump({"src": src, "functions": len(names), "baseline": len(ok0), "guarded": guarded,
                       "baseline_guarded_match": sorted(set(guarded) & ok0), "rows": rows}, f, indent=1)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
