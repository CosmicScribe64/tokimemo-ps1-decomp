#!/usr/bin/env python3
"""Set up and run decomp-permuter for one function with the project's exact compile pipeline (T-1330).

Usage (in Docker, repo root as cwd):
    python3 tools/permute.py func_80044700                  # set up build/permute/<func>, run 60 s
    python3 tools/permute.py all func_80044700 --time 300 -j 4    # the same, `all` is optional
    python3 tools/permute.py setup func_80044700 [--src FILE] [--out DIR] [--unit UNIT]
    python3 tools/permute.py run build/permute/func_80044700 --time 300
Options (--unit, --src, --out, --time, -j, --no-verify, --allow-decl-edits) are accepted before or
after the subcommand and the function name: `permute.py --unit TT func_8013C764 --time 120` works.

The function is found by its C definition, not by a mention (tools/funcloc.py, T-5030). All overlays
load at 0x80132000, so a name such as func_8013xxxx exists in several of them: pass `--unit` (`main`,
an overlay name, or a C/asm path) or a `UNIT:` prefix on the name; an unscoped name that several
files define is refused with the list of candidates. The function's .s file comes from the same
unit.

setup builds the directory decomp-permuter wants:
- base.c: the C source file with `INCLUDE_ASM` lines removed, preprocessed (`gcc -E -P -D__sgi
  -DNON_MATCHING`, so a NON_MATCHING variant is the one permuted), stripped to the one function
  by a K&R-aware body stripper. `--src` names a scratch file instead of src/.
- target.o: the original bytes, i.e. the function's splat .s (asm/{nonmatchings,matchings}/...)
  assembled with the build's assembler flags and zero-padded to 16 bytes like every compiled object.
- compile.sh: `tools/cc.py <in> <out> ido 5.3`, the same IDO 5.3 + -Wo,-nokpicopt + frame pass +
  asm-processor + padding that ninja uses, so a score of 0 means the build would match.
- settings.toml: func_name and compiler_type = "ido".
The worker adapts upstream type lookup for K&R parameter declarations, default-int
parameters and implicit C89 callees; compiler input stays unchanged.
setup is bounded by --time (including Python scanning and build helpers); a setup
timeout reports the phase and exits 124. all deducts setup time from the search budget.
Seed instruction counts may differ by --instruction-tolerance (default 4); a differing
mnemonic inventory warns, and a count outside the tolerance refuses the search.
run starts permuter.py in its own process group and enforces --time as a wall-clock limit for the
whole group (SIGINT at the limit, SIGKILL ten seconds later; the limit also covers the Loading
phase, T-7030). It prints a progress line every 10 s (elapsed time, best score so far) and lists the
best outputs at the end.

Outputs are candidates, not results (T-7030): the permuter compiles one stripped, preprocessed
function, ninja compiles the whole file with its headers. Every output of score 0 is therefore put
into the real C file (a copy of the function body; the file is restored afterwards), built with
ninja, compared with `funcdiff.py --resolve`, and the unit's sha1 target is built; only a
candidate that passes all of it is called a match. The permuter can also edit what stands outside
the function (types of externs, return and parameter types of called functions); those passes
have weight 0 in settings.toml (`--allow-decl-edits` restores them), and a candidate whose
declarations differ from base.c is flagged. Read the diff, keep only plain C a programmer would
write. If it needs a dummy variable, forced cast or similar to hit the bytes it is a fakematch and
must carry a FAKE comment (CODING_STANDARDS section 7). The permuter cannot fix compiler gaps
(register promotion of globals, T-0018; ugen temporary order).
"""
import os
import sys
# tools/queue.py would shadow the stdlib `queue` the permuter imports (`from queue import
# Empty`): drop the script directory from the front of sys.path and load the stdlib module
# first; tools/ is appended again below.
if sys.path and os.path.abspath(sys.path[0]) == os.path.dirname(os.path.abspath(__file__)):
    sys.path.pop(0)
import queue  # noqa: F401,E402  (stdlib)
import argparse
from collections import Counter
from contextlib import contextmanager
import glob
import json
import os
import re
import runpy
import signal
import subprocess
import sys
import tempfile
import time

sys.path.append(os.path.dirname(os.path.abspath(__file__)))  # after stdlib: tools/queue.py must not shadow queue
import cc  # noqa: E402  (AS flags and pad_text: one definition of the pipeline)
import funcloc  # noqa: E402
import srcscan  # noqa: E402
import funcdiff  # noqa: E402

PERMUTER = os.environ.get("PERMUTER_DIR", "/opt/decomp-permuter")
OUT_ROOT = "build/permute"
INCLUDE_ASM_LINE = re.compile(r"^[ \t]*INCLUDE_ASM\([^)]*\);?[ \t]*\n?", re.M)
CPP = ["gcc", "-E", "-P", "-Iinclude", "-D__sgi", "-DNON_MATCHING", "-D__attribute__(x)="]
SCORE_RE = re.compile(r"output-(\d+)-(\d+)$")


def def_regex(func):
    """Matches the opening of a C definition of `func` (not a call, not INCLUDE_ASM): the ANSI form
    `T f(params) {` and the K&R form `T f(a, b)` + parameter declarations + `{` (T-5030)."""
    # A K&R declaration starts with a type/name, not whitespace. Keeping those
    # sets disjoint avoids exponential backtracking over header declarations.
    return re.compile(r"^[A-Za-z_][^;{}()\n]*[ \t*]%s\([^;{}]*\)\s*(?:[A-Za-z_][^;{}\n]*;\s*)*\{" % re.escape(func), re.M)


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


# Passes that edit declarations outside the function body: the types of extern variables and the
# return/parameter types of the function and its callees. Their results do not carry over to the
# real file (T-7030), so they are off unless --allow-decl-edits.
DECL_PASSES = ("perm_randomize_external_type", "perm_randomize_function_type")


def settings_toml(func, allow_decl_edits=False):
    text = 'func_name = "%s"\ncompiler_type = "ido"\n' % func
    if not allow_decl_edits:
        text += "\n[weight_overrides]\n" + "".join("%s = 0\n" % p for p in DECL_PASSES)
    return text


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


SH_TIMEOUT = 300   # seconds for any single helper command (preprocessor, assembler)


def sh(cmd, **kw):
    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True,
                            start_new_session=True, **kw)
    try:
        stdout, stderr = proc.communicate(timeout=SH_TIMEOUT)
    except BaseException as exc:
        # gcc may have a cc1 child; kill the helper's whole session on the setup
        # alarm as well as its own timeout, before returning control to main.
        kill_group(proc, signal.SIGKILL)
        proc.communicate()
        if isinstance(exc, subprocess.TimeoutExpired):
            sys.exit("permute.py: %s did not finish in %d s" % (" ".join(cmd[:2]), SH_TIMEOUT))
        raise
    if proc.returncode != 0:
        sys.exit("permute.py: %s failed:\n%s" % (" ".join(cmd[:2]), stderr or stdout))
    return stdout


def install_permuter_compat():
    """Fill upstream's missing K&R/implicit-call types without rewriting compiler input.

    K&R param_decls live outside the function body, which upstream's type visitor
    skips. Undeclared C89 callees have type int (). Install before importing the
    randomizer so its imported build_typemap reference sees this adapter.
    """
    sys.path.insert(0, PERMUTER)
    from src import ast_types
    from perm_pycparser import c_ast as ca
    original = ast_types.build_typemap

    def build_typemap(ast, target_fn):
        tm = original(ast, target_fn)
        for decl in target_fn.param_decls or []:
            tm.var_types[decl.name] = ast_types.get_decl_type(decl)
            tm.local_vars.add(decl.name)
        args = target_fn.decl.type.args
        for param in args.params if args else []:
            if isinstance(param, ca.ID) and param.name not in tm.local_vars:
                tm.var_types[param.name] = ast_types.basic_type("int")
                tm.local_vars.add(param.name)

        class Calls(ca.NodeVisitor):
            def visit_FuncCall(self, node):
                if isinstance(node.name, ca.ID) and node.name.name not in tm.var_types:
                    tm.var_types[node.name.name] = ca.FuncDecl(
                        args=None, type=ast_types.basic_type("int"))
                self.generic_visit(node)

        Calls().visit(target_fn.body)
        return tm

    ast_types.build_typemap = build_typemap


SIGNATURE_PREFIX = re.compile(r"^[A-Za-z_][^;{}()\n]*[ \t*](\w+)\(", re.M)


def signature_names(text):
    """Cheap signature prefixes, including multiline ANSI/K&R definitions."""
    # Collect cheap signature prefixes first. Searching for a whole definition in
    # one regex backtracks exponentially across long prototype lists (T-9810).
    # function_range checks each name separately, including multiline ANSI/K&R.
    return set(SIGNATURE_PREFIX.findall(text))


def strip_functions(pre, func):
    """Strip other ANSI and K&R bodies, preserving the target text/line layout."""
    edits = []
    seen = set()
    # T-12200: inspect each signature at its known offset instead of searching
    # the entire translation unit once for every prototype in the shared headers.
    for prefix in SIGNATURE_PREFIX.finditer(pre):
        name = prefix.group(1)
        if name == func or name.startswith("PERM") or name in seen:
            continue
        opening = def_regex(name).match(pre, prefix.start())
        if opening is None:
            continue
        seen.add(name)
        a, b = opening.start(), match_brace(pre, opening.end() - 1)
        if b < 0:
            continue
        sig = pre[a:opening.end() - 1].rstrip()
        if re.search(r"\)\s*[^{}]+;\s*$", sig):
            sig = sig[:sig.index("(")] + "()"
        edits.append((a, b, sig + ";"))
    for a, b, replacement in sorted(edits, reverse=True):
        pre = pre[:a] + replacement + pre[b:]
    return pre


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
    if not def_regex(func).search(pre):
        sys.exit("permute.py: no C definition of %s in %s" % (func, src))
    return strip_functions(pre, func)


def target_source(asm_text):
    """The function's .s with the assembler prelude the compiled C objects use (asm-processor's)."""
    return '.include "asmproc_prelude.inc"\n' + asm_text


# The prelude's jlabel is global (asm-processor needs that), so the assembler keeps an R_MIPS_PC16
# relocation on every branch to a switch-case label and the permuter's objdump parser asserts
# (T-11210). target.o is only scored, so those labels are local here and the branches resolve.
LOCAL_JLABEL = (".purgem jlabel\n.macro jlabel label, visibility=local\n    .\\visibility \\label\n"
                "    \\label:\n.endm\n")


def build_target(root, asm_path, out_obj):
    """Assemble the original function bytes like the build does and pad .text to 16."""
    with open(asm_path) as f:
        text = target_source(f.read()).replace("\n", "\n" + LOCAL_JLABEL, 1)
    with tempfile.NamedTemporaryFile("w", suffix=".s", dir=os.path.dirname(out_obj) or ".",
                                     delete=False) as t:
        t.write(text)
        tmp = t.name
    try:
        sh(cc.AS + ["-o", out_obj, tmp], cwd=root)
    finally:
        os.unlink(tmp)
    cc.pad_text(out_obj)


def scratch_free(src, root):
    """True when `src` is a file of the project's src/ tree (a --src scratch file cannot be verified)."""
    rel = os.path.relpath(os.path.abspath(src), os.path.abspath(root)).replace(os.sep, "/")
    return rel.startswith("src/")


def instruction_shape(disassembly, func):
    """Mnemonic counts of one function, ignoring alignment nops and instruction order.

    Register allocation, constants, relocations and scheduling are search inputs.
    A different instruction inventory warns; the configurable count tolerance bounds preflight.
    """
    lines = funcdiff.split_functions(disassembly).get(func)
    if lines is None:
        raise RuntimeError("invalid base: object has no function %s" % func)
    mnemonics = []
    for line in lines:
        m = re.match(r"\s*[0-9a-f]+:\s+[0-9a-f]{8}\s+(\w+)", line)
        if m:
            mnemonics.append(m.group(1))
    while mnemonics and mnemonics[-1] == "sll":
        # Only sll zero,zero,0 is padding; retain real shifts.
        last = next(l for l in reversed(lines) if re.match(r"\s*[0-9a-f]+:\s+[0-9a-f]{8}", l))
        if not re.search(r"\bsll\s+zero,zero,0x0\s*$", last):
            break
        mnemonics.pop()
        lines = lines[:lines.index(last)]
    if not mnemonics:
        raise RuntimeError("invalid base: %s has no instructions" % func)
    return Counter(mnemonics)


def validate_base(directory, instruction_tolerance=4, warn=print):
    """Compile the search seed, warn about shape differences and bound the count difference."""
    directory = os.path.abspath(directory)
    try:
        with open(os.path.join(directory, "settings.toml")) as f:
            import toml
            func = toml.loads(f.read())["func_name"]
        with tempfile.TemporaryDirectory(prefix="preflight-", dir=directory) as tmp:
            obj = os.path.join(tmp, "base.o")
            try:
                sh([os.path.join(directory, "compile.sh"), os.path.join(directory, "base.c"), "-o", obj],
                   cwd=directory)
            except SystemExit as exc:
                raise RuntimeError("invalid base: base.c does not build; search not started\n%s" % exc) from exc
            shapes = []
            for path in (obj, os.path.join(directory, "target.o")):
                dump = sh(["mips-linux-gnu-objdump", "-d", "-z", "-M", "no-aliases", path])
                shapes.append(instruction_shape(dump, func))
            if shapes[0] != shapes[1]:
                detail = "instruction shape differs from target (%d vs %d instructions; base-only %s; target-only %s)" % (
                    sum(shapes[0].values()), sum(shapes[1].values()),
                    dict(shapes[0] - shapes[1]), dict(shapes[1] - shapes[0]))
                if instruction_tolerance < 0:
                    raise RuntimeError("instruction tolerance must be nonnegative")
                if abs(sum(shapes[0].values()) - sum(shapes[1].values())) > instruction_tolerance:
                    raise RuntimeError("invalid base: %s; exceeds tolerance %d; search not started" %
                                       (detail, instruction_tolerance))
                warn("warning: %s; searching within instruction tolerance %d" % (detail, instruction_tolerance))
    except (OSError, KeyError, ValueError) as exc:
        raise RuntimeError("invalid base: cannot validate search directory: %s" % exc) from exc


def setup(root, func, src=None, out=None, unit=None, allow_decl_edits=False, instruction_tolerance=4):
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
        f.write(settings_toml(func, allow_decl_edits))
    with open(os.path.join(out, "meta.json"), "w") as f:     # what `run DIR` needs to verify a candidate
        json.dump({"func": func, "unit": unit, "src": os.path.relpath(src, root) if scratch_free(src, root) else None,
                   "scratch": not scratch_free(src, root)}, f)
    validate_base(out, instruction_tolerance)
    return out


def kill_group(proc, sig):
    try:
        os.killpg(proc.pid, sig)
    except (ProcessLookupError, PermissionError):
        pass


def run(directory, seconds, jobs, extra=(), progress=print, interval=10, clock=time.time, sleep=time.sleep,
        cmd=None, instruction_tolerance=4):
    """Run permuter.py for at most `seconds` of wall-clock time; returns the list_outputs result.

    The permuter runs in its own process group. At the limit the whole group gets SIGINT, and ten
    seconds later SIGKILL (its worker processes ignore a SIGINT that arrives while they compile; a
    plain proc.kill() left them running and the call never returned, T-7030). Every `interval`
    seconds `progress` gets a line with the elapsed time and the best score so far."""
    if cmd is None:
        preflight_start = time.monotonic()
        with setup_limit(seconds):
            validate_base(directory, instruction_tolerance)
        seconds -= time.monotonic() - preflight_start
        if seconds <= 0:
            raise TimeoutError("time limit reached validating base; search not started")
    cmd = cmd or [sys.executable, os.path.abspath(__file__), "_worker", directory, "-j", str(jobs),
                  "--stop-on-zero", "--best-only", "--quiet", "--stack-diffs"] + list(extra)
    start = clock()
    deadline = start + seconds
    proc = subprocess.Popen(cmd, start_new_session=True)
    progress("permute.py: started %s (limit %d s, %d threads)" % (os.path.basename(directory.rstrip("/")), seconds, jobs))
    last = start
    try:
        while proc.poll() is None and clock() < deadline:
            sleep(0.5)
            if clock() - last >= interval:
                last = clock()
                outs = list_outputs(directory)
                best = ("best score %d (%s)" % (outs[0][0], os.path.basename(outs[0][2]))) if outs \
                    else "no output better than the base yet"
                progress("permute.py: %d/%d s, %s" % (clock() - start, seconds, best))
        if proc.poll() is None:
            progress("permute.py: time limit reached, stopping the permuter")
            kill_group(proc, signal.SIGINT)
            stop = clock() + 10
            while proc.poll() is None and clock() < stop:
                sleep(0.2)
    finally:
        kill_group(proc, signal.SIGKILL)       # workers that outlive the parent
        try:
            proc.wait(timeout=10)
        except subprocess.TimeoutExpired:
            pass
    if proc.returncode not in (0, -signal.SIGINT, -signal.SIGKILL):
        raise RuntimeError("permuter worker failed (exit %s); search did not complete" % proc.returncode)
    return list_outputs(directory)


# ---------------------------------------------------------------------------------------------
# verification of candidates against the real build (T-7030)

TOKEN_RE = re.compile(r"[A-Za-z_]\w*|0[xX][0-9A-Fa-f]+|\d+|\S")


def tokens(text):
    return TOKEN_RE.findall(text)


def match_brace(text, i):
    """Index after the `}` that closes the `{` at text[i]; -1 if unbalanced."""
    depth = 0
    for j in range(i, len(text)):
        if text[j] == "{":
            depth += 1
        elif text[j] == "}":
            depth -= 1
            if depth == 0:
                return j + 1
    return -1


def function_range(text, func):
    """(start, end) of the C definition of `func` in `text` (ANSI or K&R), or None."""
    m = def_regex(func).search(text)
    if not m:
        return None
    end = match_brace(text, m.end() - 1)
    return (m.start(), end) if end > 0 else None


def declarations_outside(text, func):
    """Token list of `text` without the definition of `func`: what the permuter must not edit."""
    r = function_range(text, func)
    return tokens(text[:r[0]] + text[r[1]:]) if r else tokens(text)


def outside_edits(base_text, cand_text, func):
    """[(old, new)] token runs that differ between base.c and a candidate outside the function; the
    permuter's printer and gcc -E spell declarations alike, so any difference is a real edit."""
    import difflib
    a, b = declarations_outside(base_text, func), declarations_outside(cand_text, func)
    out = []
    for op, i1, i2, j1, j2 in difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes():
        if op != "equal":
            out.append((" ".join(a[i1:i2]), " ".join(b[j1:j2])))
    return out


RODATA_RE = re.compile(r'^[ \t]*INCLUDE_RODATA\([^)]*\);[ \t]*\n?', re.M)
GUARD_RE = re.compile(r"^[ \t]*#[ \t]*if(?:def[ \t]+NON_MATCHING|[ \t]+defined\(NON_MATCHING\))[^\n]*\n", re.M)


def replace_function(text, func, body):
    """`text` with the definition of `func` (or its INCLUDE_ASM line) replaced by `body`. A
    `#ifdef NON_MATCHING ... #else INCLUDE_ASM ... #endif` block around it is replaced as a whole,
    because ninja builds without NON_MATCHING. Returns None when the function is not in `text`."""
    body = body.strip("\n") + "\n"
    r = function_range(text, func)
    inc = re.search(r'^[ \t]*INCLUDE_ASM\([^)]*,\s*%s\s*\);[ \t]*\n?' % re.escape(func), text, re.M)
    where = r or (inc.span() if inc else None)
    if where is None:
        return None
    for g in GUARD_RE.finditer(text):
        # the matching #endif of this guard
        depth, pos, end = 1, g.end(), None
        for line in re.finditer(r"^[ \t]*#[ \t]*(if|endif)\w*[^\n]*\n?", text[g.end():], re.M):
            depth += 1 if line.group(1) == "if" else -1
            if depth == 0:
                end = g.end() + line.end()
                break
        if end and g.start() <= where[0] and where[1] <= end:
            # the jump tables and constants INCLUDE_RODATA'd inside the block belong to the object, not to
            # the function body: the real build needs them (T-9030)
            keep = "".join(m.group(0) if m.group(0).endswith("\n") else m.group(0) + "\n"
                           for m in RODATA_RE.finditer(text[g.end():end]))
            return text[:g.start()] + keep + body + text[end:]
    if r and text[r[1]:r[1] + 1] == "\n":
        return text[:r[0]] + body.rstrip("\n") + text[r[1]:]
    return text[:where[0]] + body + text[where[1]:]


def layout_notes(body):
    """Lines of the candidate body that hold several statements. IDO numbers the stores of one source
    line together, so `a = 0; b = 0;` on a line is not the same code as the two statements on lines of
    their own (main `func_8004111C`: the permuter's `do { rect.x = 0; ... } while (0);` line matches, the
    reformatted statements store in the other order). A score-0 candidate has to be copied with its line
    layout, then simplified one change at a time with ninja as the judge."""
    notes = []
    for n, line in enumerate(body.splitlines(), 1):
        stmts = line.count(";") - 2 * len(re.findall(r"\bfor\s*\(", line))
        if stmts > 1:
            notes.append("line %d of the body holds several statements: %s" % (n, line.strip()[:90]))
    return notes


def unit_target(unit):
    return "build/SLPM_86.053.ok" if unit in (None, "main") else "build/ovl/%s.ok" % unit


def verify_candidate(root, meta, cand_dir, runner=subprocess.run):
    """Put the candidate's function body into the real C file, build it with ninja and compare with
    funcdiff --resolve; the file is restored. Returns (ok, [report lines]).

    The permuter judged a stripped, preprocessed copy of the function; this is the check that counts.
    Reasons a score-0 candidate fails here: declarations outside the function that the permuter
    changed (reported first), or a context the stripped copy does not have (the other functions of
    the file, the real headers), in which case the funcdiff lines show where the bytes differ."""
    report = []
    if not meta.get("src"):
        return False, ["not verified: the function was permuted from a scratch file (--src)"]
    src = os.path.join(root, meta["src"])
    func, unit = meta["func"], meta.get("unit")
    with open(os.path.join(cand_dir, "source.c")) as f:
        cand_text = f.read()
    base_path = os.path.join(os.path.dirname(cand_dir.rstrip("/")), "base.c")
    if os.path.exists(base_path):
        with open(base_path) as f:
            edits = outside_edits(f.read(), cand_text, func)
        for old, new in edits[:6]:
            report.append("note: candidate declarations outside the function differ (not applied): '%s' -> '%s'"
                          % (old[:100], new[:100]))
    r = function_range(cand_text, func)
    if not r:
        return False, report + ["candidate has no definition of %s" % func]
    with open(src) as f:
        original = f.read()
    new = replace_function(original, func, cand_text[r[0]:r[1]])
    if new is None:
        return False, report + ["%s is not in %s" % (func, meta["src"])]
    scope = "%s:%s" % (unit, func) if unit else func
    try:
        with open(src, "w") as f:
            f.write(new)
        fd = runner([sys.executable, os.path.join(root, "tools", "funcdiff.py"), "--resolve", scope], cwd=root,
                    stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        fd_ok = fd.returncode == 0 and "MATCH" in fd.stdout
        # a switch with a jump table (T-9030): funcdiff compares the table by name (`jtbl_X` of the original
        # against `.rodata` of the build) and reports a DIFF for code that matches; the unit's sha1, which
        # covers .text and .rodata, is then the judge (it is the stricter test whatever funcdiff says)
        jtbl = not fd_ok and bool(re.search(r"\bjtbl_\w+|\.rodata", fd.stdout)) and "ERROR" not in fd.stdout
        report.append("ninja object + funcdiff --resolve: %s" % ("MATCH" if fd_ok else
                                                                 "DIFF (jump table or rodata relocation: the unit sha1 decides)" if jtbl else "DIFF"))
        if fd_ok:
            report += ["note: " + n + " (IDO orders the stores of one source line together: keep the layout)"
                       for n in layout_notes(cand_text[r[0]:r[1]])]
        if not fd_ok and not jtbl:
            report += ["    " + l for l in fd.stdout.strip().splitlines()[:14]]
            return False, report
        nj = runner(["ninja", unit_target(unit)], cwd=root, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        report.append("ninja %s (unit sha1): %s" % (unit_target(unit), "OK" if nj.returncode == 0 else "FAILED"))
        if nj.returncode != 0:
            report += ["    " + l for l in nj.stdout.strip().splitlines()[-6:]]
            if jtbl:
                report += ["    funcdiff:"] + ["    " + l for l in fd.stdout.strip().splitlines()[:8]]
            return False, report
    finally:
        with open(src, "w") as f:
            f.write(original)
    # Only the function was inserted: real headers and declarations stayed in place.
    # Outside edits may explain a failure, but cannot invalidate a passing unit sha1.
    return True, report


SUBCOMMANDS = ("setup", "run", "all")
VALUE_OPTIONS = ("--unit", "--src", "--out", "--time", "-j", "--args", "--instruction-tolerance")


def with_subcommand(argv):
    """argv with `all` inserted before the first positional when none of the subcommands is given
    (`permute.py func_1`, `permute.py --unit TT func_1 --time 5`)."""
    argv = list(argv)
    i = 0
    while i < len(argv):
        tok = argv[i]
        if tok in VALUE_OPTIONS:
            i += 2
        elif tok.startswith("-"):
            i += 1
        else:
            return argv if tok in SUBCOMMANDS else argv[:i] + ["all"] + argv[i:]
    return argv


def parse_args(argv):
    """Parsed command line. Every option works before or after the subcommand and the function;
    a bare function name means `all <func>`."""
    argv = with_subcommand(argv)
    S = argparse.SUPPRESS

    def options(p, top):
        # the sub-parsers must not overwrite what the top parser read (default SUPPRESS)
        d = (lambda v: v) if top else (lambda v: S)
        p.add_argument("--unit", default=d(None), help="unit (main or overlay name) or C/asm path that holds the function")
        p.add_argument("--src", default=d(None), help="C file to take the function from (default: find it in src/)")
        p.add_argument("--out", default=d(None), help="permuter directory (default build/permute/<func>)")
        p.add_argument("--time", type=int, default=d(60), help="wall-clock limit in seconds (default 60)")
        p.add_argument("--instruction-tolerance", type=int, default=d(4),
                       help="allow this instruction-count difference in the seed (default 4; warn on shape differences)")
        p.add_argument("-j", type=int, default=d(os.cpu_count() or 1), help="threads")
        p.add_argument("--no-verify", action="store_true", default=d(False),
                       help="do not check score-0 candidates against the real build")
        p.add_argument("--allow-decl-edits", action="store_true", default=d(False),
                       help="let the permuter change extern and function types (off by default: those edits "
                            "do not carry over to the real file)")
        if not top:
            p.add_argument("--args", nargs=argparse.REMAINDER, default=[], help="extra permuter.py args")

    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    options(ap, True)
    sub = ap.add_subparsers(dest="cmd", required=True)
    for name in ("setup", "all"):
        p = sub.add_parser(name)
        p.add_argument("func")
        options(p, False)
    run_p = sub.add_parser("run")
    run_p.add_argument("directory")
    options(run_p, False)
    return ap.parse_args(argv)


def report_outputs(outs, directory, root, args, out=print):
    """Print the best outputs; verify the score-0 ones against the build. Returns True when a verified
    match exists."""
    for score, n, path in outs[:5]:
        out("score %d: %s/source.c" % (score, path))
    zeros = [o for o in outs if o[0] == 0]
    if not zeros:
        out("permute.py: no score-0 candidate; candidates only, keep plain C and mark any trick FAKE.")
        return False
    if args.no_verify:
        out("permute.py: score 0 is not a match until ninja and funcdiff agree (verification skipped by --no-verify).")
        return False
    meta_path = os.path.join(directory, "meta.json")
    if not os.path.exists(meta_path):
        out("permute.py: %s has no meta.json (set up by an older version): rerun setup to verify." % directory)
        return False
    with open(meta_path) as f:
        meta = json.load(f)
    good = False
    for score, n, path in zeros[:3]:
        ok, lines = verify_candidate(root, meta, path)
        out("verify %s: %s" % (path, "MATCH" if ok else "NOT A MATCH"))
        for l in lines:
            out("  " + l)
        good = good or ok
    out("permute.py: %s" % ("verified candidate above: put its body into src/, strip permuter noise, rebuild."
                            if good else "score 0 did not survive the real build (see the cause above); not a match."))
    return good


def main(argv):
    args = parse_args(argv)
    if args.instruction_tolerance < 0:
        print("permute.py: --instruction-tolerance must be nonnegative", file=sys.stderr)
        return 2
    if args.time <= 0:
        print("permute.py: --time must be positive", file=sys.stderr)
        return 2
    start = time.monotonic()
    try:
        if args.cmd == "run":
            directory = args.directory
        else:
            # An alarm also bounds Python setup work (e.g. a pathological regex),
            # which subprocess timeouts alone cannot interrupt. Docker is Unix.
            with setup_limit(args.time):
                directory = setup(".", args.func, args.src, args.out, args.unit, args.allow_decl_edits,
                                  args.instruction_tolerance)
    except TimeoutError:
        print("permute.py: time limit reached during setup (%d s); search not started" % args.time,
              file=sys.stderr)
        return 124
    except RuntimeError as exc:
        print("permute.py: %s" % exc, file=sys.stderr)
        return 1
    if args.cmd == "setup":
        print(directory)
        return 0
    remaining = args.time - (time.monotonic() - start)
    if remaining <= 0:
        print("permute.py: time limit exhausted by setup; search not started", file=sys.stderr)
        return 124
    try:
        outs = run(directory, remaining, args.j, args.args, instruction_tolerance=args.instruction_tolerance)
    except (RuntimeError, TimeoutError) as exc:
        print("permute.py: %s" % exc, file=sys.stderr)
        return 124 if isinstance(exc, TimeoutError) else 1
    if not outs:
        print("permute.py: no output better than the base in %d s (%s)" % (args.time, directory))
        return 0
    return 0 if report_outputs(outs, directory, ".", args) or outs[0][0] != 0 else 1


@contextmanager
def setup_limit(seconds):
    """Interrupt all CLI setup work after seconds; restore the caller's alarm."""
    def expired(_signum, _frame):
        raise TimeoutError("permuter setup deadline")

    handler = signal.getsignal(signal.SIGALRM)
    previous = signal.setitimer(signal.ITIMER_REAL, 0)
    start = time.monotonic()
    signal.signal(signal.SIGALRM, expired)
    signal.setitimer(signal.ITIMER_REAL, seconds)
    try:
        yield
    finally:
        signal.setitimer(signal.ITIMER_REAL, 0)
        signal.signal(signal.SIGALRM, handler)
        if previous[0]:
            signal.setitimer(signal.ITIMER_REAL, max(0.000001, previous[0] - (time.monotonic() - start)),
                            previous[1])


if __name__ == "__main__":
    if sys.argv[1:2] == ["_worker"]:
        install_permuter_compat()
        sys.argv = [os.path.join(PERMUTER, "permuter.py")] + sys.argv[2:]
        runpy.run_path(sys.argv[0], run_name="__main__")
    else:
        sys.exit(main(sys.argv[1:]))
