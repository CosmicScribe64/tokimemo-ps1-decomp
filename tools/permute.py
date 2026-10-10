#!/usr/bin/env python3
"""Set up and run decomp-permuter for one function with the project's exact compile pipeline (T-1330).

Usage (in Docker, repo root as cwd):
    python3 tools/permute.py func_80044700                  # set up build/permute/<func>, run 60 s
    python3 tools/permute.py all func_80044700 --time 300 -j 4    # the same, `all` is optional
    python3 tools/permute.py setup func_80044700 [--src FILE] [--out DIR] [--unit UNIT]
    python3 tools/permute.py run build/permute/func_80044700 --time 300

The function is found by its C definition, not by a mention (tools/funcloc.py, T-5030). All overlays
load at 0x80132000, so a name such as func_8013xxxx exists in several of them: pass `--unit` (`main`,
an overlay name, or a C/asm path) or a `UNIT:` prefix on the name; an unscoped name that several
files define is refused with the list of candidates. The function's .s file comes from the same
unit.

setup builds the directory decomp-permuter wants:
- base.c: the C source file with `INCLUDE_ASM` lines removed, preprocessed (`gcc -E -P -D__sgi
  -DNON_MATCHING`, so a NON_MATCHING variant is the one permuted), stripped to the one function
  by the permuter's own strip_other_fns. `--src` names a scratch file instead of src/.
- target.o: the original bytes, i.e. the function's splat .s (asm/{nonmatchings,matchings}/...)
  assembled with the build's assembler flags and zero-padded to 16 bytes like every compiled object.
- compile.sh: `tools/cc.py <in> <out> ido 5.3`, the same IDO 5.3 + -Wo,-nokpicopt + frame pass +
  asm-processor + padding that ninja uses, so a score of 0 means the build would match.
- settings.toml: func_name and compiler_type = "ido".
run starts permuter.py, stops it (SIGINT) after --time seconds or at score 0, and lists the best
outputs. Outputs are candidates, not results: read the diff, keep only plain C a programmer would
write, rebuild with ninja and check with funcdiff.py. If it needs a dummy variable, forced cast or
similar to hit the bytes it is a fakematch and must carry a FAKE comment (CODING_STANDARDS section 7).
The permuter cannot fix compiler gaps (register promotion of globals, T-0018; ugen temporary order).
"""
import argparse
import glob
import os
import re
import signal
import subprocess
import sys
import tempfile
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import cc  # noqa: E402  (AS flags and pad_text: one definition of the pipeline)
import funcloc  # noqa: E402
import srcscan  # noqa: E402

PERMUTER = os.environ.get("PERMUTER_DIR", "/opt/decomp-permuter")
OUT_ROOT = "build/permute"
INCLUDE_ASM_LINE = re.compile(r"^[ \t]*INCLUDE_ASM\([^)]*\);?[ \t]*\n?", re.M)
CPP = ["gcc", "-E", "-P", "-Iinclude", "-D__sgi", "-DNON_MATCHING", "-D__attribute__(x)="]
SCORE_RE = re.compile(r"output-(\d+)-(\d+)$")


def def_regex(func):
    """Matches the opening of a C definition of `func` (not a call, not INCLUDE_ASM): the ANSI form
    `T f(params) {` and the K&R form `T f(a, b)` + parameter declarations + `{` (T-5030)."""
    return re.compile(r"^[A-Za-z_][^;{}()\n]*[ \t*]%s\([^;{}]*\)\s*(?:[^;{}()]+;\s*)*\{" % re.escape(func), re.M)


def find_source(root, func, unit=None):
    """The src/main or src/ovl file that holds a C definition of `func` in `unit`, or None.
    Raises funcloc.AmbiguousError when several units define it and `unit` does not pick one."""
    try:
        return str(funcloc.locate(func, unit, root, defined_only=True).src)
    except funcloc.AmbiguousError:
        raise
    except funcloc.LocateError:
        return None


def find_asm(root, func, source=None, unit=None):
    """Path of the function's splat .s file, or None. It comes from the unit (and the C file) of
    `source`/`unit`: another overlay's .s of the same address is never taken."""
    if source is not None:
        unit = unit or funcloc.unit_of_path(os.path.relpath(source, root) if os.path.isabs(source) else source)
        for c in srcscan.c_files(root):
            if os.path.abspath(c.src) == os.path.abspath(source):
                for d in (c.nonmatchings, c.matchings):
                    if os.path.exists(os.path.join(d, func + ".s")):
                        return os.path.join(d, func + ".s")
    pats = ["asm/nonmatchings/main/*/%s.s", "asm/matchings/main/*/%s.s",
            "asm/ovl/*/nonmatchings/**/%s.s", "asm/ovl/*/matchings/**/%s.s"]
    hits = []
    for pat in pats:
        hits += sorted(glob.glob(os.path.join(root, pat % func), recursive=True))
    if unit:
        hits = [h for h in hits if funcloc.unit_of_path(os.path.relpath(h, root)) == unit]
    units = {funcloc.unit_of_path(os.path.relpath(h, root)) for h in hits}
    if len(units) > 1:
        sys.exit("permute.py: %s has asm files in several units (%s); pass --unit" % (func, ", ".join(sorted(units))))
    return hits[0] if hits else None


def strip_include_asm(text):
    return INCLUDE_ASM_LINE.sub("", text)


def settings_toml(func):
    return 'func_name = "%s"\ncompiler_type = "ido"\n' % func


def compile_script(root):
    """compile.sh: the permuter calls `compile.sh input.c -o output.o` from its own directory."""
    cc_py = os.path.join(os.path.abspath(root), "tools", "cc.py")
    return ('#!/bin/bash\n'
            '# T-1330: same compile as ninja (tools/cc.py: IDO 5.3, frame pass, asm-processor, padding)\n'
            'in="$(realpath "$1")"\n'
            'out="$(realpath -m "$3")"\n'
            'cd "%s" && exec python3 "%s" "$in" "$out" ido 5.3\n'
            % (os.path.abspath(root), cc_py))


def list_outputs(directory):
    """[(score, n, path)] of the permuter's output-<score>-<n> directories, best first."""
    found = []
    for p in glob.glob(os.path.join(directory, "output-*")):
        m = SCORE_RE.search(p)
        if m:
            found.append((int(m.group(1)), int(m.group(2)), p))
    return sorted(found)


def sh(cmd, **kw):
    proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, **kw)
    if proc.returncode != 0:
        sys.exit("permute.py: %s failed:\n%s" % (" ".join(cmd[:2]), proc.stderr or proc.stdout))
    return proc.stdout


def build_base(root, src, func):
    """Preprocessed, single-function C for the permuter."""
    with open(src) as f:
        text = strip_include_asm(f.read())
    with tempfile.NamedTemporaryFile("w", suffix=".c", dir=root, delete=False) as t:
        t.write(text)
        tmp = t.name
    try:
        pre = sh(CPP + [tmp], cwd=root)
    finally:
        os.unlink(tmp)
    sys.path.insert(0, PERMUTER)
    import strip_other_fns  # noqa: E402  (from decomp-permuter)
    if not def_regex(func).search(pre):
        sys.exit("permute.py: no C definition of %s in %s" % (func, src))
    return strip_other_fns.strip_other_fns(pre, func)


def target_source(asm_text):
    """The function's .s with the assembler prelude the compiled C objects use (asm-processor's)."""
    return '.include "asmproc_prelude.inc"\n' + asm_text


def build_target(root, asm_path, out_obj):
    """Assemble the original function bytes like the build does and pad .text to 16."""
    with open(asm_path) as f:
        text = target_source(f.read())
    with tempfile.NamedTemporaryFile("w", suffix=".s", dir=os.path.dirname(out_obj) or ".",
                                     delete=False) as t:
        t.write(text)
        tmp = t.name
    try:
        sh(cc.AS + ["-o", out_obj, tmp], cwd=root)
    finally:
        os.unlink(tmp)
    cc.pad_text(out_obj)


def setup(root, func, src=None, out=None, unit=None):
    func, unit = funcloc.split_scope(func, unit)
    try:
        src = src or find_source(root, func, unit)
    except funcloc.LocateError as e:
        sys.exit("permute.py: %s" % e)
    if not src:
        sys.exit("permute.py: no C definition of %s under src/%s (use --src FILE)"
                 % (func, " in unit " + unit if unit else ""))
    asm_path = find_asm(root, func, src, unit)
    if not asm_path:
        sys.exit("permute.py: no asm/ file for %s (run ninja once to generate asm/)" % func)
    out = out or os.path.join(root, OUT_ROOT, func)
    os.makedirs(out, exist_ok=True)
    with open(os.path.join(out, "base.c"), "w") as f:
        f.write(build_base(root, src, func))
    build_target(root, asm_path, os.path.join(out, "target.o"))
    script = os.path.join(out, "compile.sh")
    with open(script, "w") as f:
        f.write(compile_script(root))
    os.chmod(script, 0o755)
    with open(os.path.join(out, "settings.toml"), "w") as f:
        f.write(settings_toml(func))
    return out


def run(directory, seconds, jobs, extra=()):
    """Run permuter.py for at most `seconds`; returns the list_outputs result."""
    cmd = [sys.executable, os.path.join(PERMUTER, "permuter.py"), directory, "-j", str(jobs),
           "--stop-on-zero", "--best-only", "--quiet", "--stack-diffs"] + list(extra)
    proc = subprocess.Popen(cmd)
    deadline = time.time() + seconds
    while proc.poll() is None and time.time() < deadline:
        time.sleep(0.5)
    if proc.poll() is None:
        proc.send_signal(signal.SIGINT)
        try:
            proc.wait(timeout=20)
        except subprocess.TimeoutExpired:
            proc.kill()
    return list_outputs(directory)


def parse_args(argv):
    """Parsed command line; a bare function name means `all <func>`, `all <func>` works too."""
    argv = list(argv)
    if argv and argv[0] not in ("setup", "run", "all") and not argv[0].startswith("-"):
        argv = ["all"] + argv
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sub = ap.add_subparsers(dest="cmd", required=True)

    def setup_args(p):
        p.add_argument("func")
        p.add_argument("--src", help="C file to take the function from (default: find it in src/)")
        p.add_argument("--out", help="permuter directory (default build/permute/<func>)")
        p.add_argument("--unit", help="unit (main or overlay name) or C/asm path that holds the function")

    def run_args(p):
        p.add_argument("--time", type=int, default=60, help="seconds to run (default 60)")
        p.add_argument("-j", type=int, default=os.cpu_count() or 1, help="threads")
        p.add_argument("--args", nargs=argparse.REMAINDER, default=[], help="extra permuter.py args")

    setup_args(sub.add_parser("setup"))
    all_p = sub.add_parser("all")
    setup_args(all_p)
    run_args(all_p)
    run_p = sub.add_parser("run")
    run_p.add_argument("directory")
    run_args(run_p)
    return ap.parse_args(argv)


def main(argv):
    args = parse_args(argv)
    if args.cmd == "setup":
        print(setup(".", args.func, args.src, args.out, args.unit))
        return
    directory = args.directory if args.cmd == "run" else setup(".", args.func, args.src, args.out, args.unit)
    outs = run(directory, args.time, args.j, args.args)
    if not outs:
        print("permute.py: no output better than the base in %d s (%s)" % (args.time, directory))
        return
    for score, n, path in outs[:5]:
        print("score %d: %s/source.c" % (score, path))
    print("permute.py: candidates only; keep plain C, mark any trick FAKE, then rebuild and funcdiff.")


if __name__ == "__main__":
    main(sys.argv[1:])
