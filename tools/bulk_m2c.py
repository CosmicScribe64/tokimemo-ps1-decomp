#!/usr/bin/env python3
"""Unattended m2c + auto-solve pass over the small unmatched functions (T-11190).

Usage (in Docker, repo root as cwd; --out is a scratch directory outside the checkout):
    TOKIMEMO_SCRATCH=/path/to/scratch tools/docker.sh python3 tools/bulk_m2c.py --out /scratch/bulk_m2c \\
        [--max-size 600] [--unit NAME] [--exclude TT] [--limit N] [--jobs N] [--keep-score 30] [--time 60]
    python3 tools/bulk_m2c.py --out DIR --summary      # summary of the state file only, no work

Targets: every `INCLUDE_ASM` function under --max-size bytes (splat `nonmatching` header of its .s)
that is not already a guarded draft, smallest first. For each one:

1. tools/m2c.py's run_m2c gives the draft with the usual context (headers, rodata, callee argument
   counts, post-increment order).
2. Mechanical cleanup only: m2c's unknown type `?` becomes `s32` (declarations, casts, parameters),
   `//` comments become `/* */`, and a draft with an `M2C_*` placeholder is refused. Declarations
   m2c printed for symbols the headers lack are kept for the symbols the body uses (`static` and
   initialisers dropped, functions as K&R `T f();`). A second variant inlines `temp_*` locals that
   are assigned once from a side-effect-free expression and read once on the next line; both are
   scored and the better one is kept.
3. The draft goes into the C file as `#ifdef NON_MATCHING <draft> #else INCLUDE_ASM #endif`, the
   extra declarations in a marked block above it, and only that function is compiled (autotweak's
   BuildScorer: permute.py setup, tools/cc.py). On a compile error the cheap fixes are tried:
   declare an undefined symbol (m2c's declaration, else `extern s32 X;`), drop a declaration the
   headers already have with another type. Otherwise the reason is recorded.
4. Score 0 is verified like a permuter output (ninja object, funcdiff --resolve, unit sha1). A score
   under --keep-score goes to autotweak's hill climb for --time seconds. A verified match replaces
   the guard with plain C; its extra declarations move to include/main_api.h (main-exe addresses)
   or include/ovl/<UNIT>.h (overlay addresses), then check_headers.py and the unit target must pass,
   else everything is reverted. Every other draft is reverted to INCLUDE_ASM and saved as
   <out>/drafts/<UNIT>/<func>.c with its score.

The state file <out>/state.jsonl makes runs resumable: a target already in it is skipped. Jobs
work on disjoint C files; ninja, header writes and the final checks hold one lock (<out>/build.lock),
so one build runs at a time. A draft left in src/ by a killed run is restored from <out>/inflight/
at the next start. Output: one line per target, then MATCH count/bytes, failures by reason, and
<out>/near.txt (near misses by score).
"""
import argparse
import contextlib
import fcntl
import io
import json
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import autotweak  # noqa: E402

TICKET = "T-11190"
DECL_START = "/* bulk_m2c declarations (%s) */" % TICKET
DECL_END = "/* end bulk_m2c declarations */"
NOTE = "/* NON_MATCHING: %s bulk_m2c draft */" % TICKET
MAIN_RANGE = (0x80041000, 0x8012B537)
OVERLAY_BASE = 0x80132000
INCLUDE_ASM_TMPL = r'^[ \t]*INCLUDE_ASM\(\s*"[^"]*"\s*,\s*%s\s*\);[^\n]*\n?'
ERROR_RE = re.compile(r"(?i)\berror\b[^\n]*")


# ---------------------------------------------------------------------------------------------
# Cleanup of the m2c output
# ---------------------------------------------------------------------------------------------

def fix_comments(text):
    """`// x` comments outside strings and block comments become `/* x */`."""
    out, i, n = [], 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
        elif c in "\"'":
            j = i + 1
            while j < n and text[j] not in (c, "\n"):
                j += 2 if text[j] == "\\" else 1
            j = min(j + 1, n)
        elif text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
            out.append("/* %s */" % text[i + 2:j].strip().replace("*/", "* /"))
            i = j
            continue
        else:
            j = i + 1
        out.append(text[i:j])
        i = j
    return "".join(out)


def fix_unknown_types(text):
    """m2c's `?` (unknown type) as s32: at the start of a declaration, in a cast, in a parameter."""
    text = re.sub(r"^([ \t]*(?:extern[ \t]+|static[ \t]+)?)\?(?=[ \t*])", r"\1s32", text, flags=re.M)
    text = re.sub(r"\(\?([ \t]*\**)\)", r"(s32\1)", text)
    text = re.sub(r"([(,][ \t]*)\?(?=[ \t]*[*\w),])", r"\1s32", text)
    return text


def decl_name(line):
    m = re.search(r"(\w+)\s*(?:\(|\[|;)", line)
    return m.group(1) if m else None


def clean_decls(decls):
    """{name: declaration line} from m2c's declarations of symbols missing from the headers."""
    out, depth = {}, 0
    for raw in decls.splitlines():
        line = re.sub(r"/\*.*?\*/", "", raw).strip()
        opened = line.count("{") - line.count("}")
        if depth or opened:
            depth += opened
            continue                       # struct definitions and initialiser blocks
        if not line.endswith(";") or "=" in line or line.startswith("#"):
            continue
        line = fix_unknown_types(re.sub(r"^static\s+", "", line))
        name = decl_name(line)
        if not name:
            continue
        if "(" in line:
            line = line[:line.index("(")] + "();"
        elif not line.startswith("extern "):
            line = "extern " + line
        out[name] = line
    return out


SIDE_EFFECT_RE = re.compile(r"\+\+|--|(?<![=!<>])=(?!=)|\b(?!sizeof\b)[A-Za-z_]\w*\s*\(")
SIMPLE_RE = re.compile(r"^[\w.]+(?:->\w+)*$")


def inline_temps(body):
    """`temp_x = E; <next line using temp_x once>` -> the next line with (E), when temp_x has one
    declaration, one assignment and one use and E has no call, assignment or ++/--."""
    lines = body.split("\n")
    changed = True
    while changed:
        changed = False
        masked = autotweak.mask("\n".join(lines)).split("\n")
        for i, line in enumerate(masked):
            m = re.match(r"^\s*(temp_\w+)\s*=\s*(.+);\s*$", line)
            if not m:
                continue
            name, expr = m.group(1), m.group(2).strip()
            if SIDE_EFFECT_RE.search(expr) or re.search(r"\b%s\b" % name, expr):
                continue
            uses = [k for k, l in enumerate(masked) if re.search(r"\b%s\b" % name, l)]
            decl = [k for k in uses if re.match(r"^\s*[\w ]+[\s*]%s\s*;\s*$" % name, masked[k])]
            nxt = next((k for k in range(i + 1, len(lines)) if masked[k].strip()), None)
            if len(decl) != 1 or sorted(uses) != sorted(decl + [i, nxt] if nxt is not None else []):
                continue
            if len(re.findall(r"\b%s\b" % name, masked[nxt])) != 1:
                continue
            use = re.search(r"\b%s\b" % name, masked[nxt])
            before = masked[nxt][:use.start()].rstrip()
            after = masked[nxt][use.end():].lstrip()
            whole = (before.endswith(("(", ",")) or (before.endswith("=") and before[-2:-1] not in "=!<>")) \
                and after[:1] in (")", ",", ";")
            repl = expr if whole or SIMPLE_RE.match(expr) else "(%s)" % expr
            lines[nxt] = lines[nxt][:use.start()] + repl + lines[nxt][use.end():]
            for k in sorted([decl[0], i], reverse=True):
                del lines[k]
            k = decl[0]
            if 0 < k < len(lines) and not lines[k].strip() and lines[k - 1].rstrip().endswith("{"):
                del lines[k]                # the blank line after the last declaration
            changed = True
            break
    return "\n".join(lines)


def cleanup(decls, body):
    """(body, {name: decl}, problem): the mechanical cleanup of the module docstring."""
    bad = re.search(r"\bM2C_\w+", body)
    if bad:
        return body, {}, "m2c-error: %s" % bad.group(0)
    body = fix_unknown_types(fix_comments(body)).strip("\n") + "\n"
    names = set(re.findall(r"[A-Za-z_]\w*", autotweak.mask(body)))
    return body, {n: d for n, d in clean_decls(decls).items() if n in names}, None


# ---------------------------------------------------------------------------------------------
# Draft placement in the C file
# ---------------------------------------------------------------------------------------------

def guarded_funcs(text):
    return {g.func for g in autotweak.guards(text)}


def include_asm_funcs(text):
    """Functions of plain INCLUDE_ASM lines (not the #else branch of a guarded draft)."""
    guarded = guarded_funcs(text)
    return [f for f in re.findall(r'^[ \t]*INCLUDE_ASM\(\s*"[^"]*"\s*,\s*(\w+)\s*\)', text, re.M)
            if f not in guarded]


def insert_draft(text, func, body, decl_lines):
    """`text` with the INCLUDE_ASM line of `func` wrapped in a NON_MATCHING guard holding `body`,
    and `decl_lines` in a marked block above it. None when there is no plain INCLUDE_ASM line."""
    if func in guarded_funcs(text):
        return None
    m = re.search(INCLUDE_ASM_TMPL % re.escape(func), text, re.M)
    if not m:
        return None
    inc = m.group(0).rstrip("\n") + "\n"
    block = ""
    if decl_lines:
        block = DECL_START + "\n" + "".join(d + "\n" for d in decl_lines) + DECL_END + "\n"
    new = (block + "#ifdef NON_MATCHING\n" + NOTE + "\n" + body.strip("\n") + "\n#else\n" + inc + "#endif\n")
    return text[:m.start()] + new + text[m.end():]


def drop_decl_block(text):
    return re.sub(r"^%s\n.*?^%s\n" % (re.escape(DECL_START), re.escape(DECL_END)), "", text, flags=re.M | re.S)


def header_home(name, unit):
    """include/ path where a declaration of `name` belongs, or None (no address in the name)."""
    m = re.search(r"_([0-9A-Fa-f]{8})$", name)
    if not m:
        return None
    addr = int(m.group(1), 16)
    if MAIN_RANGE[0] <= addr <= MAIN_RANGE[1]:
        return "include/main_api.h"
    if addr >= OVERLAY_BASE and unit != "main":
        return "include/ovl/%s.h" % unit
    return None


def insert_in_header(text, lines):
    """`text` with `lines` before the #endif that closes its include guard (else at the end)."""
    add = "/* %s: declarations added by tools/bulk_m2c.py. */\n" % TICKET + "".join(l + "\n" for l in lines)
    first = re.search(r"^[ \t]*#[ \t]*ifndef\b[^\n]*\n", text, re.M)
    if first:
        depth = 1
        for d in re.finditer(r"^[ \t]*#[ \t]*(if|endif)\w*[^\n]*\n?", text[first.end():], re.M):
            depth += 1 if d.group(1) == "if" else -1
            if depth == 0:
                at = first.end() + d.start()
                return text[:at] + add + "\n" + text[at:]
    return text.rstrip("\n") + "\n\n" + add


def write_atomic(path, text):
    tmp = "%s.bulk_m2c.%d.tmp" % (path, os.getpid())
    with open(tmp, "w") as fh:
        fh.write(text)
    os.replace(tmp, path)


def read(path):
    with open(path) as fh:
        return fh.read()


# ---------------------------------------------------------------------------------------------
# Compile errors and their cheap fixes
# ---------------------------------------------------------------------------------------------

def first_error(msg):
    """The first error line of a compiler or tool message, without directory paths."""
    msg = (msg or "").strip()
    m = ERROR_RE.search(msg)
    line = m.group(0) if m else (msg.splitlines()[-1] if msg else "unknown")
    return re.sub(r"\S*/", "", line).strip()[:100]


def cheap_fix(msg, decl_lines, known):
    """New declaration list for a compile error `msg`, or None when no cheap fix applies."""
    new = list(decl_lines)
    for name in re.findall(r"redeclaration of '(\w+)'", msg):
        new = [d for d in new if decl_name(d) != name]
    for name in re.findall(r"'(\w+)' undefined", msg):
        if any(decl_name(d) == name for d in new):
            continue
        new.append(known.get(name) or "extern s32 %s;" % name)
    return new if new != list(decl_lines) else None


# ---------------------------------------------------------------------------------------------
# Backend: the parts that need Docker (m2c, IDO, ninja); mocked in the tests
# ---------------------------------------------------------------------------------------------

class Scorer(autotweak.BuildScorer):
    INSTRUCTION_TOLERANCE = 10 ** 6


class Backend:
    def m2c(self, root, unit, func):
        """(declarations, body) of the m2c draft; raises LookupError/RuntimeError."""
        import m2c
        with contextlib.redirect_stderr(io.StringIO()):
            text = m2c.run_m2c(root, "%s:%s" % (unit, func), log=lambda _m: None)
        return m2c.split_draft(text, func)

    def scorer(self, root, draft):
        """An object with score(text) -> int|None and verify(text) -> bool; raises on compile errors."""
        with contextlib.redirect_stdout(io.StringIO()):
            return Scorer(root, draft)

    def check_headers(self, root):
        p = subprocess.run([sys.executable, "tools/check_headers.py", "include", "src"], cwd=root,
                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        return p.returncode == 0, p.stdout

    def build_unit(self, root, unit):
        import permute
        p = subprocess.run(["ninja", permute.unit_target(unit)], cwd=root,
                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        return p.returncode == 0, p.stdout


@contextlib.contextmanager
def locked(path):
    with open(path, "a") as fh:
        fcntl.flock(fh, fcntl.LOCK_EX)
        try:
            yield
        finally:
            fcntl.flock(fh, fcntl.LOCK_UN)


# ---------------------------------------------------------------------------------------------
# One target
# ---------------------------------------------------------------------------------------------

class Target:
    def __init__(self, unit, src, func, size):
        self.unit, self.src, self.func, self.size = unit, src, func, size

    @property
    def scope(self):
        return "%s:%s" % (self.unit, self.func)

    @property
    def key(self):
        return "%s:%s:%s" % (self.unit, self.src, self.func)

    def as_tuple(self):
        return (self.unit, self.src, self.func, self.size)


class Run:
    """Paths and options shared by the jobs."""

    def __init__(self, root, out, keep_score=30, time_budget=60.0, backend=None, climb=None, emit=print):
        self.root, self.out = root, out
        self.keep_score, self.time_budget = keep_score, time_budget
        self.backend = backend or Backend()
        self.climb = climb or autotweak.climb
        self.emit = emit
        self.lock = os.path.join(out, "build.lock")
        self.state = os.path.join(out, "state.jsonl")
        self.inflight = os.path.join(out, "inflight")

    def record(self, t, status, score=None, reason=None, draft=None):
        row = {"key": t.key, "scope": t.scope, "src": t.src, "size": t.size, "status": status,
               "score": score, "reason": reason, "draft": draft}
        with locked(self.state + ".lock"):
            with open(self.state, "a") as fh:
                fh.write(json.dumps(row) + "\n")
        detail = {"match": "", "near": " %s" % score, "fail": " %s" % reason}[status]
        self.emit("%s %s 0x%X%s" % (status.upper() if status == "match" else status, t.scope, t.size, detail))
        return row

    def save_draft(self, t, text, score, note):
        d = os.path.join(self.out, "drafts", t.unit)
        os.makedirs(d, exist_ok=True)
        path = os.path.join(d, t.func + ".c")
        what = note if score is None else ("score %d %s" % (score, note)).strip()
        with open(path, "w") as fh:
            fh.write("/* %s %s (%s): %s */\n%s\n" % (TICKET, t.scope, t.src, what, text.strip("\n")))
        return path

    def inflight_path(self, t):
        return os.path.join(self.inflight, "%s__%s.json" % (t.unit, t.func))

    # -- the pipeline -----------------------------------------------------------------------

    def process(self, t):
        path = os.path.join(self.root, t.src)
        original = read(path)
        if t.func not in include_asm_funcs(original):
            return self.record(t, "fail", reason="gone: no plain INCLUDE_ASM line")
        try:
            decls, body = self.backend.m2c(self.root, t.unit, t.func)
        except (LookupError, RuntimeError, OSError, SystemExit) as exc:
            return self.record(t, "fail", reason="m2c: %s" % first_error(str(exc)))
        body, known, problem = cleanup(decls, body)
        if problem:
            return self.record(t, "fail", reason=problem, draft=self.save_draft(t, body, None, problem))
        os.makedirs(self.inflight, exist_ok=True)
        with open(self.inflight_path(t), "w") as fh:
            json.dump({"src": t.src, "text": original}, fh)
        matched = False
        try:
            matched, row = self._solve(t, path, original, body, known)
            return row
        except Exception as exc:  # noqa: BLE001  (unattended: record and go on with the next target)
            return self.record(t, "fail", reason="error: %s" % first_error("%s: %s" % (type(exc).__name__, exc)))
        finally:
            if not matched and read(path) != original:
                write_atomic(path, original)
            os.unlink(self.inflight_path(t))

    def _compile(self, t, path, original, body, decl_lines, known):
        """(scorer, decl_lines) once the guarded draft compiles, or (None, reason)."""
        for _attempt in range(4):
            text = insert_draft(original, t.func, body, decl_lines)
            write_atomic(path, text)
            try:
                return self.backend.scorer(self.root, autotweak.Draft(t.src, t.unit, t.func, body)), decl_lines
            except (RuntimeError, SystemExit, OSError, ImportError) as exc:
                msg = str(exc)
                fixed = cheap_fix(msg, decl_lines, known)
                if fixed is None:
                    return None, "compile: %s" % first_error(msg)
                decl_lines = fixed
        return None, "compile: no fix after 4 attempts"

    def _solve(self, t, path, original, body, known):
        scorer, decl_lines = self._compile(t, path, original, body, sorted(known.values()), known)
        if scorer is None:
            return False, self.record(t, "fail", reason=decl_lines, draft=self.save_draft(t, body, None, decl_lines))
        cache = {}

        def score(text):
            if text not in cache:
                cache[text] = scorer.score(text)
            return cache[text]

        def verify(text):
            with locked(self.lock):
                return scorer.verify(text)

        start = [(score(v), v) for v in dict.fromkeys([body, inline_temps(body)])]
        start = sorted((s, v) for s, v in start if s is not None)
        if not start:
            return False, self.record(t, "fail", reason="compile: draft does not score",
                                      draft=self.save_draft(t, body, None, "does not score"))
        best, text = start[0]
        final = None
        if best == 0:
            final = text if verify(text) else None
        elif best < self.keep_score:
            res = self.climb(text, score, verify, budget=self.time_budget)
            if res.matched and res.best == 0:
                final = res.text
            elif res.best is not None and res.best < best:
                best, text = res.best, res.text
        note = "score 0 but verification failed" if best == 0 and final is None else ""
        if final is not None:
            reason = self._finish(t, path, original, final, decl_lines)
            if reason is None:
                return True, self.record(t, "match", score=0)
            return False, self.record(t, "fail", reason=reason, draft=self.save_draft(t, final, 0, reason))
        return False, self.record(t, "near", score=best, reason=note or None,
                                  draft=self.save_draft(t, text, best, note))

    def _finish(self, t, path, original, final, decl_lines):
        """Write the verified match (guard -> plain C, declarations -> headers); None or a reason."""
        with locked(self.lock):
            new = autotweak.rewrite_guard(read(path), t.func, final)
            if new is None:
                return "rewrite: guard block not found"
            write_atomic(path, drop_decl_block(new))
            if not decl_lines:
                return None
            saved, homes = {}, {}
            for d in decl_lines:
                home = header_home(decl_name(d), t.unit)
                if home is None:
                    write_atomic(path, original)
                    return "headers: no header home for %s" % decl_name(d)
                homes.setdefault(home, []).append(d)
            for home, lines in homes.items():
                hp = os.path.join(self.root, home)
                saved[hp] = read(hp)
                write_atomic(hp, insert_in_header(saved[hp], lines))
            ok, msg = self.backend.check_headers(self.root)
            if ok:
                ok, msg = self.backend.build_unit(self.root, t.unit)
            if ok:
                return None
            for hp, text in saved.items():
                write_atomic(hp, text)
            write_atomic(path, original)
            return "headers: %s" % first_error(msg)


# ---------------------------------------------------------------------------------------------
# Targets, state, jobs, summary
# ---------------------------------------------------------------------------------------------

def find_targets(root, max_size, units=None, exclude=()):
    import srcscan
    out = []
    for c in srcscan.c_files(root):
        if c.unit in exclude or (units and c.unit not in units) or not c.src.exists():
            continue
        rel = os.path.relpath(str(c.src), root)
        for func in include_asm_funcs(c.src.read_text(errors="replace")):
            asm = c.nonmatchings / (func + ".s")
            got = srcscan.nonmatching_size(asm) if asm.exists() else None
            if got and got[1] < max_size:
                out.append(Target(c.unit, rel, func, got[1]))
    return select(out, max_size)


def select(targets, max_size, done=(), limit=None):
    """Targets under max_size and not in `done`, smallest first, at most `limit`."""
    out = sorted((t for t in targets if t.size < max_size and t.key not in done),
                 key=lambda t: (t.size, t.unit, t.src, t.func))
    return out[:limit] if limit else out


def load_state(path):
    rows = {}
    if os.path.exists(path):
        with open(path) as fh:
            for line in fh:
                line = line.strip()
                if line:
                    row = json.loads(line)
                    rows[row["key"]] = row
    return rows


def recover(root, out, emit=print):
    """Restore C files that a killed run left with a bulk_m2c draft in them."""
    d = os.path.join(out, "inflight")
    if not os.path.isdir(d):
        return 0
    n = 0
    for name in sorted(os.listdir(d)):
        p = os.path.join(d, name)
        with open(p) as fh:
            info = json.load(fh)
        src = os.path.join(root, info["src"])
        if os.path.exists(src) and (NOTE in read(src) or DECL_START in read(src)):
            write_atomic(src, info["text"])
            emit("restored %s" % info["src"])
            n += 1
        os.unlink(p)
    return n


def group_by_file(targets):
    groups = {}
    for t in targets:
        groups.setdefault(t.src, []).append(t)
    return list(groups.values())


def run_group(root, out, keep_score, time_budget, items):
    """Pool worker: one C file's targets, in order, with the real backend."""
    run = Run(root, out, keep_score, time_budget, emit=lambda s: print(s, flush=True))
    for item in items:
        run.process(Target(*item))
    return len(items)


def summary(out, emit=print):
    rows = load_state(os.path.join(out, "state.jsonl")).values()
    match = [r for r in rows if r["status"] == "match"]
    near = sorted((r for r in rows if r["status"] == "near"), key=lambda r: (r["score"], r["size"], r["scope"]))
    fails = {}
    for r in rows:
        if r["status"] == "fail":
            kind = r["reason"].split(":")[0]
            fails[kind] = fails.get(kind, 0) + 1
    with open(os.path.join(out, "near.txt"), "w") as fh:
        for r in near:
            fh.write("%d\t0x%X\t%s\t%s\t%s\n" % (r["score"], r["size"], r["scope"], r["src"], r["draft"]))
    emit("MATCH %d (0x%X bytes)  near %d  fail %d  (%d processed)" % (
        len(match), sum(r["size"] for r in match), len(near), sum(fails.values()), len(rows)))
    for kind, n in sorted(fails.items(), key=lambda kv: (-kv[1], kv[0])):
        emit("  fail %-12s %d" % (kind, n))
    emit("near misses: %s" % os.path.join(out, "near.txt"))
    return len(match)


def parse_args(argv):
    p = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    p.add_argument("--out", default="/scratch/bulk_m2c", help="state, drafts and near.txt (outside the repo)")
    p.add_argument("--max-size", type=lambda s: int(s, 0), default=600, help="bytes, exclusive (default 600)")
    p.add_argument("--unit", default="", help="comma-separated units to work on (default all)")
    p.add_argument("--exclude", default="TT", help="comma-separated units to skip (default TT)")
    p.add_argument("--limit", type=int, help="at most N new targets")
    p.add_argument("--jobs", type=int, default=1, help="parallel jobs, one C file each (default 1)")
    p.add_argument("--keep-score", type=int, default=30, help="climb drafts scoring below this (default 30)")
    p.add_argument("--time", type=float, default=60, help="autotweak seconds per draft (default 60)")
    p.add_argument("--summary", action="store_true", help="print the summary of the state file and stop")
    p.add_argument("--root", default=".", help=argparse.SUPPRESS)
    return p.parse_args(argv)


def main(argv=None, backend=None, targets=None, climb=None, emit=print):
    args = parse_args(sys.argv[1:] if argv is None else argv)
    root, out = os.path.abspath(args.root), os.path.abspath(args.out)
    if out == root or out.startswith(root + os.sep):
        emit("bulk_m2c.py: --out must be outside the checkout (got %s)" % args.out)
        return 2
    try:
        os.makedirs(out, exist_ok=True)
    except OSError as exc:
        emit("bulk_m2c.py: cannot create --out %s: %s (in Docker set TOKIMEMO_SCRATCH)" % (args.out, exc))
        return 2
    if args.summary:
        summary(out, emit)
        return 0
    recover(root, out, emit)
    done = load_state(os.path.join(out, "state.jsonl"))
    units = {u for u in args.unit.split(",") if u}
    exclude = {u for u in args.exclude.split(",") if u}
    if targets is None:
        targets = find_targets(root, args.max_size, units, exclude)
    else:
        targets = [t for t in targets if t.unit not in exclude and (not units or t.unit in units)]
    todo = select(targets, args.max_size, set(done), args.limit)
    emit("%d targets (%d already processed)" % (len(todo), len([t for t in targets if t.key in done])))
    if args.jobs <= 1 or backend is not None:
        run = Run(root, out, args.keep_score, args.time, backend, climb, emit)
        for t in todo:
            run.process(t)
    else:
        from concurrent.futures import ProcessPoolExecutor
        with ProcessPoolExecutor(max_workers=args.jobs) as pool:
            futures = [pool.submit(run_group, root, out, args.keep_score, args.time,
                                   [t.as_tuple() for t in g]) for g in group_by_file(todo)]
            for f in futures:
                f.result()
    summary(out, emit)
    return 0


if __name__ == "__main__":
    sys.exit(main())
