#!/usr/bin/env python3
"""Sweep every return-type and callee view of a draft on the full translation unit (T-20810).

Matching often turns on how the file types things: the function itself (a `void` definition, or an `s32` one
without a `return` value, implicit int, so `$v0` is live at the exit) and each callee (`void f();`, K&R
`s32 f();`, `s32 f(s32 a0);`, kept through `*_OVERRIDE_` guards, CODING_STANDARDS.md 8a). 19 of the 102 wave 3
blocker notes were that one lever, and every use was found by hand. This tool tries the whole lattice in
memory, ranks it by funcdiff rows, and prints the recipe when a variant reaches 0 rows. It is step 1 for a
function that is close but not matching ("Do first: run viewsweep" in wiki/register-cheatsheet.md).

    python3 tools/viewsweep.py [UNIT:]func ... | --guarded [--unit U] | --targets FILE | --drafts DIR ...
                               | --body BODY.c --scope UNIT:func
                               [-j 2] [--budget 150] [--json OUT] [--src FILE] [--resume]

Targets
  - `[UNIT:]func`: a function with a C body in its file, guarded (`#ifdef NON_MATCHING`) or plain.
  - `--guarded`: every guarded draft of the tree (`--unit U` narrows it to one unit).
  - `--targets FILE`: lines `UNIT:func [BODY.c]` (blank lines and `#` comments are skipped).
  - `--body BODY.c --scope UNIT:func`, `--drafts DIR`: an archived draft body (`DIR/<UNIT>/<func>/body.c`, the
    layout of drafts/T-NNNN/) for a function that is still `INCLUDE_ASM`. The body is put into the file's text
    in a scratch copy as a `NON_MATCHING` draft; archived functions that are already C or guarded are skipped.
  - `--src FILE`: a scratch copy of the C file to sweep instead of the tree's (one target only).

The unit is preprocessed once per compile with funcdiff_verify.preprocess_draft (`NON_MATCHING` defined when
the function is guarded or has a `--body`), the variant is applied to the preprocessed text in memory
(protoverride.apply with allow_defined), and the result is compiled like the build (tools/cc.py, trailing pad,
object layout) and scored as order_search.make_text_scorer does: the number of funcdiff rows of the function,
relocations resolved. Nothing under src/ or include/ is touched.

The lattice of one target (a variant is a set of edits on different functions):
  own     the target's definition: as is, `s32 f()` (K&R, no return), `s32 f(void)`; a definition with
          parameters keeps them and only changes its return type to `s32`.
  callee  every function that the body calls (the `jal` targets of the original asm, when there is one): as is,
          `s32 c();`, `s32 c(<its declared parameters>);` and, when the body ignores the result (or the callee is
          void already), `void c();`. A callee that the file defines gets its definition's return type
          rewritten too (its parameters stay).
Search order, inside --budget compiles (the baseline counts): (1) the baseline, (2) every single edit, likely
ones first (the asm says which: a copy, zero or constant left in `$v0` at an exit points to `s32` for the
function; a call whose result the original reads points to a non-void callee), (3) the best own edit x every
callee edit, (4) all pairs among the five best single improvers, (5) the best variant extended by the other
improvers one at a time. It stops at the first variant with 0 rows.

A compile that fails (a concurrent build.ninja rewrite, a compiler crash, an edit the file cannot take) is
retried once, serially, after the parallel batch; if it fails again the variant is reported UNTESTED with the
first line of the error. It is never counted as "no gain".

Result per target: a status line and, for rows 0, the literal-byte proof (funcdiff_verify.exact_match on the
variant object; if the other guarded drafts of the file move the function, again on a unit where only this
function is C) and the recipe: the definition change, the header prototype to change, the
`*_OVERRIDE_<symbol>` lines (protoverride.commit_hint), and every caller (grep over src/ and include/) whose
unit must stay sha1-OK after `ninja`. The table at the end is ranked by best rows. `--json OUT` writes one
`UNIT_func.json` per target into the directory OUT (or all targets into OUT when it ends in `.json`);
`--resume` skips targets whose file is there.

Limits: a body is read as written (calls through a function pointer variable are not callees); only return
types and parameter lists change, never an argument or a statement; a view that matches here is a result of
this compile until the definition, header and guards are in the tree and `ninja` agrees (wiki/decompile-workflow.md,
"viewsweep"). Tests: tools/test_viewsweep.py (synthetic units and a fake compiler, no game data).
"""
import argparse
import collections
import hashlib
import itertools
import json
import os
import re
import shutil
import sys
import threading
import time
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import protoverride  # noqa: E402

TICKET = "T-20810"
BIG = 10 ** 6
DEFAULT_BUDGET = 150
TOP_IMPROVERS = 5
UNTESTED_SHOWN = 6
WORK_ROOT = Path("build/viewsweep")

# Not calls: statement keywords that are followed by a parenthesis.
NOT_CALLS = frozenset("if while for switch return sizeof else do case default goto break continue".split())
CALL_RE = re.compile(r"\b([A-Za-z_]\w*)\s*\(")


class Edit(collections.namedtuple("Edit", "symbol decl kind")):
    """One in-memory prototype change: `decl` (`s32 f();`) for `symbol`; kind is `own` (the target's own
    definition) or `callee`."""
    __slots__ = ()


Outcome = collections.namedtuple("Outcome", "rows error detail", defaults=("",))   # rows None = UNTESTED (error says why)
Hints = collections.namedtuple("Hints", "own reads")             # own: bool; reads: {callee: bool}


# ---------------------------------------------------------------------------------------------
# reading the preprocessed unit

def mask(text):
    """`text` with the inside of string and character literals blanked (same length)."""
    out, i = [], 0
    while i < len(text):
        if text[i] in "\"'":
            j = min(protoverride._skip_literal(text, i), len(text))
            seg = text[i:j]
            out.append(seg[:1] + "".join(c if c == "\n" else " " for c in seg[1:-1]) + seg[1:][-1:])
            i = j
        else:
            out.append(text[i])
            i += 1
    return "".join(out)


def norm_ret(ret):
    """A return type spelled the way views are compared: `int` and a missing type are `s32`."""
    text = " ".join(re.sub(r"\b(?:extern|static)\b", " ", ret or "").split())
    return "s32" if text in ("", "int") else text


def _spelled(params):
    return "(%s)" % " ".join((params or "").split())


def match_paren(text, open_at):
    """Index of the `)` that closes the `(` at open_at, or -1."""
    depth = 0
    for k in range(open_at, len(text)):
        if text[k] == "(":
            depth += 1
        elif text[k] == ")":
            depth -= 1
            if depth == 0:
                return k
    return -1


def body_calls(body, own_name=None):
    """Names called in a function definition's text, in source order, each once. Names that the body declares
    as variables or parameters (function pointers) and the function's own name are left out."""
    masked = mask(body)
    names = []
    for m in CALL_RE.finditer(masked):
        name = m.group(1)
        if name in NOT_CALLS or name == own_name or name in names:
            continue
        if re.search(r"\b[A-Za-z_]\w*[\s*]+%s\s*[;,=\[)]" % re.escape(name), masked):
            continue                    # `s32 name;`, `u8 *name,`, `(s32 name)`: a variable
        names.append(name)
    return names


def _statement_call(masked, start, end):
    """True when the call at masked[start:end] (the name through its closing `)`) is a statement of its own, so
    its result is not used."""
    after = masked[end + 1:].lstrip()
    if not after.startswith(";"):
        return False
    before = masked[:start].rstrip()
    if not before:
        return True
    if before[-1] in ";{}:":
        return True
    if re.search(r"\b(?:else|do)$", before):
        return True
    if before[-1] == ")":
        depth, k = 0, len(before) - 1
        while k >= 0:
            if before[k] == ")":
                depth += 1
            elif before[k] == "(":
                depth -= 1
                if depth == 0:
                    break
            k -= 1
        if k < 0:
            return False
        inner = before[k + 1:-1].strip()
        word = re.search(r"([A-Za-z_]\w*)\s*$", before[:k])
        return inner == "void" or (word is not None and word.group(1) in ("if", "while", "for", "switch"))
    return False


def ignored_results(body, callees):
    """The callees whose every call in `body` is a statement of its own (the result is ignored)."""
    masked = mask(body)
    ignored = set()
    for name in callees:
        uses = [m for m in re.finditer(r"\b%s\s*\(" % re.escape(name), masked)]
        if uses and all(_statement_call(masked, m.start(), match_paren(masked, m.end() - 1)) for m in uses):
            ignored.add(name)
    return ignored


def has_value_return(body):
    return re.search(r"\breturn\s+[^;\s]", mask(body)) is not None


# ---------------------------------------------------------------------------------------------
# the lattice

def own_edits(pre, name):
    """The edits of the target's own type: [Edit]. Empty when the unit does not define `name`. The definition's
    own return type and parameter list are the reference. A header that disagrees with the definition (a redeclaration
    error before any edit) is first answered by an edit that restates the definition, then come the `s32` views."""
    defs = protoverride.definitions(pre, name)
    if not defs:
        return []
    d = defs[0]
    cur_ret = norm_ret(d.ret)
    no_params = protoverride._empty(d.params) and not d.rest.strip()
    parens = "()" if d.rest.strip() else _spelled(d.params)
    out = []
    declared = protoverride.current_view(pre, name)[0]
    if declared is not None and norm_ret(declared) != cur_ret:
        out.append(Edit(name, "%s %s%s;" % (cur_ret, name, parens), "own"))
    if no_params:
        out += [Edit(name, "s32 %s%s;" % (name, spelling), "own") for spelling in ("()", "(void)")
                if (cur_ret, spelling) != ("s32", parens)]
    elif cur_ret != "s32":
        out.append(Edit(name, "s32 %s%s;" % (name, parens), "own"))
    return out


def callee_edits(pre, name, ignored=False):
    """The edits of one callee's view: [Edit] in the order `s32 c();`, `s32 c(<same args>);`, `void c();`
    (the last only when the result is ignored or the callee is void already). A view equal to the current one is
    left out. A callee that the unit defines gets its definition rewritten too (protoverride.apply)."""
    ret, params, defined = protoverride.current_view(pre, name)
    declared = ret is not None
    cur = (norm_ret(ret), _spelled(params) if declared and params is not None else "()")
    defs = protoverride.definitions(pre, name) if defined else []
    out = []

    def add(r, parens):
        if (r, parens) != cur:
            out.append(Edit(name, "%s %s%s;" % (r, name, parens), "callee"))

    add("s32", "()")
    if declared and params is not None and not protoverride._empty(params):
        add("s32", _spelled(params))
    keeps_value = bool(defs) and has_value_return(pre[defs[0].brace:defs[0].end])
    if (ignored or cur[0] == "void") and not keeps_value:
        add("void", "()")
    return out


def build_plan(pre, name, callees, ignored):
    """(own edits, {callee: edits}) for the target `name`, callees in source order."""
    own = own_edits(pre, name)
    per = collections.OrderedDict()
    for c in callees:
        edits = callee_edits(pre, c, c in ignored)
        if edits:
            per[c] = edits
    return own, per


def asm_hints(asm_text):
    """(Hints, [callees in call order]) read from the original function's asm (draft_fixes' detectors)."""
    import draft_fixes
    asm = draft_fixes.parse_asm(asm_text)
    own = any(kind in ("copy", "zero", "const") for _i, kind, _slot in draft_fixes.exit_v0_defs(asm))
    reads, order = {}, []
    for callee, r in draft_fixes.call_sites(asm):
        reads[callee] = reads.get(callee, False) or r
        if callee not in order:
            order.append(callee)
    return Hints(own, reads), order


def edit_rank(edit, hints):
    """0: the asm supports the edit, 1: no evidence either way, 2: the asm contradicts it."""
    if hints is None:
        return 1
    if edit.kind == "own":
        return 0 if hints.own else 1
    ret, _split = protoverride.split_declaration(edit.decl)
    read = hints.reads.get(edit.symbol)
    if read is None:
        return 1
    if ret == "void":
        return 2 if read else 1
    return 0 if read else 1


def order_singles(own, per, hints):
    """Every single edit, the likelier ones first (stable inside a rank)."""
    singles = list(own) + [e for edits in per.values() for e in edits]
    return [e for _r, _i, e in sorted(((edit_rank(e, hints), i, e) for i, e in enumerate(singles)),
                                      key=lambda t: (t[0], t[1]))]


def apply_edits(pre, edits):
    """The preprocessed unit with each edit applied (protoverride.apply with allow_defined)."""
    for e in edits:
        pre, _notes = protoverride.apply(pre, [protoverride.Proto(e.symbol, e.decl)], allow_defined=True)
    return pre


def error_summary(err, limit=160):
    """One line for a failed compile: the compiler's own `Error:` line when there is one (without the scratch
    path), else the last line of the message."""
    lines = [l.strip() for l in str(err).strip().splitlines() if l.strip()] or [type(err).__name__]
    line = next((l for l in lines if re.search(r"\bError\b|\berror:", l)), lines[-1])
    return re.sub(r"build/viewsweep/\S*?(?:\.sjis)?\.c", "unit", line)[:limit]


def variant_label(edits):
    return "as is" if not edits else " ".join(e.decl for e in edits)


def variant_key(edits):
    return tuple(sorted(edits, key=lambda e: e.symbol))


# ---------------------------------------------------------------------------------------------
# the search

class Sweep:
    """Scores variants with `score(edits) -> rows` (raises when the compile fails), at most `jobs` at once, and
    runs the search phases of the module docstring. A failure is retried once serially after its batch (the
    retry counts as a compile but is never refused for the budget); a second failure leaves the variant UNTESTED."""

    def __init__(self, score, own, per, hints=None, budget=DEFAULT_BUDGET, jobs=2, log=print):
        self.score, self.own, self.per = score, own, per
        self.budget, self.jobs, self.log = budget, max(1, jobs), log
        self.singles = order_singles(own, per, hints)
        self.outcomes = {}                 # variant key -> Outcome
        self.compiles = 0                  # compile calls, retries included
        self.stop = threading.Event()      # a variant reached 0 rows
        self.cut = False                   # the budget ended a batch early

    # -- evaluation

    def left(self):
        return self.budget - self.compiles

    def _try(self, key):
        try:
            return Outcome(self.score(key), None)
        except protoverride.ProtoError as err:     # the edit does not apply: deterministic, no retry
            return Outcome(None, "ProtoError: " + (str(err).splitlines() or [""])[0][:160], str(err))
        except Exception as err:                   # noqa: BLE001 - any compile failure is retried once
            return Outcome(None, "%s: %s" % (type(err).__name__, error_summary(err)), str(err)[-3000:])

    def _parallel(self, keys):
        """{key: Outcome} for the keys that were started (all of them unless a variant reached 0 rows)."""
        pending, done, lock = list(reversed(keys)), {}, threading.Lock()

        def worker():
            while True:
                with lock:
                    if not pending or self.stop.is_set():
                        return
                    key = pending.pop()
                out = self._try(key)
                with lock:
                    done[key] = out
                if out.rows == 0:
                    self.stop.set()

        # Plain threads: concurrent.futures imports `queue`, which tools/queue.py shadows on this path.
        threads = [threading.Thread(target=worker) for _ in range(min(self.jobs, len(keys)))]
        for t in threads:
            t.start()
        for t in threads:
            t.join()
        return done

    def evaluate(self, variants, phase=""):
        """Score the variants that are new, within the budget; returns the keys that were scored."""
        keys = []
        for v in variants:
            key = variant_key(v)
            if key not in self.outcomes and key not in keys:
                keys.append(key)
        if len(keys) > max(self.left(), 0):
            self.cut = True
            keys = keys[:max(self.left(), 0)]
        if not keys or self.stop.is_set():
            return []
        done = self._parallel(keys)
        keys = [k for k in keys if k in done]
        self.compiles += len(keys)
        for key in keys:
            out = done[key]
            if out.rows is None and not out.error.startswith("ProtoError"):
                self.compiles += 1
                self.log("viewsweep: retrying serially: %s (%s)" % (variant_label(key), out.error))
                out = self._try(key)
            self.outcomes[key] = out
        best = self.best()
        self.log("viewsweep: %s: %d compiled, best %s" % (phase or "batch", self.compiles, best[0] if best else "-"))
        return keys

    def tested(self, key):
        out = self.outcomes.get(variant_key(key))
        return out.rows if out is not None else None

    def best(self):
        """(rows, key) of the best scored variant (fewest edits, then text, on a tie), or None."""
        scored = [(o.rows, len(k), variant_label(k), k) for k, o in self.outcomes.items() if o.rows is not None]
        if not scored:
            return None
        rows, _n, _label, key = min(scored)
        return rows, key

    # -- the phases

    def symbols(self):
        return {e.symbol for e in self.own} | set(self.per)

    def run(self):
        self.evaluate([()], "baseline")
        base = self.tested(())
        if base == 0:
            return self.result("matches-as-is")
        detail = self.outcomes[()].detail
        if base is None and re.search(r"\bError:", detail) and \
                not any(re.search(r"\b%s\b" % re.escape(sym), detail) for sym in self.symbols()):
            self.log("viewsweep: the unedited unit fails with a compiler error that no view change touches: %s"
                     % self.outcomes[()].error)
            return self.result("failed")
        if base is None:            # the unit does not compile as it is: a type that the edits restate may be why
            self.evaluate([(e,) for e in self.own], "own edits first")
        self.evaluate([(e,) for e in self.singles], "singles")
        for phase in (self.cross_with_best_own, self.pairs, self.extend_best):
            if self.stop.is_set():
                break
            phase()
        return self.result()

    def improvers(self):
        """Single edits that beat the baseline (any scored edit when the baseline does not compile), best first."""
        base = self.tested(())
        base = BIG if base is None else base
        scored = [(self.tested((e,)), i, e) for i, e in enumerate(self.singles)]
        return [e for _r, _i, e in sorted(t for t in scored if t[0] is not None and t[0] < base)]

    def cross_with_best_own(self):
        own = [e for e in self.improvers() if e.kind == "own"]
        if own:
            self.evaluate([(own[0], e) for e in self.singles if e.kind == "callee"], "best own x callees")

    def pairs(self):
        top = self.improvers()[:TOP_IMPROVERS]
        self.evaluate([(a, b) for a, b in itertools.combinations(top, 2) if a.symbol != b.symbol], "pairs")

    def extend_best(self):
        """Add the other improvers to the best variant one at a time while that keeps improving."""
        improvers = self.improvers()[:TOP_IMPROVERS]
        while not self.stop.is_set() and self.left() > 0:
            best = self.best()
            if best is None:
                return
            rows, key = best
            more = [key + (e,) for e in improvers if e.symbol not in {x.symbol for x in key}]
            if not self.evaluate(more, "extend best"):
                return
            now = self.best()
            if now is None or now[0] >= rows:
                return

    # -- the result

    def best_edits(self):
        best = self.best()
        return list(best[1]) if best else []

    def result(self, status=None):
        best, base = self.best(), self.tested(())
        if status is None:
            status = ("failed" if best is None else "match" if best[0] == 0 else
                      "improved" if base is None or best[0] < base else "no-gain")
        baseline = self.outcomes.get(())
        return {
            "status": status,
            "baseline": base,
            "baseline_error": baseline.error if baseline is not None and base is None else None,
            "best_rows": best[0] if best else None,
            "best": [e.decl for e in best[1]] if best else [],
            "compiled": self.compiles,
            "budget": self.budget,
            "budget_cut": self.cut,
            "untested": sorted((variant_label(k), o.error) for k, o in self.outcomes.items() if o.rows is None),
            "variants": [{"edits": [e.decl for e in k], "rows": o.rows, "error": o.error}
                         for k, o in sorted(self.outcomes.items(),
                                            key=lambda t: (BIG if t[1].rows is None else t[1].rows, len(t[0]),
                                                           variant_label(t[0])))],
        }


# ---------------------------------------------------------------------------------------------
# targets

Target = collections.namedtuple("Target", "scope name unit c source text non_matching body_path note")


def insert_body(text, name, body, decls=()):
    """`text` with the plain `INCLUDE_ASM(..., name);` line replaced by an `#ifdef NON_MATCHING` block holding
    `body`, the lines `decls` (the draft's own declarations) above it; None when there is no such line (the
    function is C already)."""
    m = re.search(r'^[ \t]*INCLUDE_ASM\(\s*"[^"]*"\s*,\s*%s\s*\);[^\n]*\n?' % re.escape(name), text, re.M)
    if m is None:
        return None
    line = m.group(0).rstrip("\n") + "\n"
    head = "".join(d.rstrip("\n") + "\n" for d in decls if d.strip())
    block = "%s#ifdef NON_MATCHING\n%s\n#else\n%s#endif\n" % (head, body.strip("\n"), line)
    return text[:m.start()] + block + text[m.end():]


def parse_targets(text):
    """[(spec, body path or None)] from a targets file: lines `UNIT:func [BODY.c]`."""
    out = []
    for raw in text.splitlines():
        parts = raw.split("#", 1)[0].split()
        if not parts:
            continue
        if len(parts) > 2:
            raise ValueError("targets file: expected `UNIT:func [BODY.c]`, got %r" % raw.strip())
        out.append((parts[0], parts[1] if len(parts) == 2 else None))
    return out


def draft_bodies(folders):
    """[(`UNIT:func`, body path)] for DIR/<UNIT>/<func>/body.c under each folder."""
    out = []
    for folder in folders:
        for body in sorted(Path(folder).glob("*/*/body.c")):
            out.append(("%s:%s" % (body.parent.parent.name, body.parent.name), str(body)))
    return out


def guarded_scopes(unit=None):
    """Sorted `UNIT:func` of every guarded draft of the tree (or of one unit)."""
    import autotweak
    import srcscan
    found = set()
    for c in srcscan.c_files(units=[unit] if unit else None):
        if not Path(c.src).exists():
            continue
        text = Path(c.src).read_text(errors="replace")
        if "NON_MATCHING" in text:
            found.update("%s:%s" % (c.unit, g.func) for g in autotweak.guards(text))
    return sorted(found)


def make_target(spec, unit=None, body=None, src=None, work=WORK_ROOT):
    """The Target for `[UNIT:]func`. With `body` (a path to an archived draft body) the function must still be a
    plain INCLUDE_ASM and the body goes into a scratch copy of the file as a NON_MATCHING draft. Raises ValueError
    with the reason when the function cannot be swept."""
    import autotweak
    import funcdiff_verify as proof
    import funcloc
    import srcscan
    name, scope = funcloc.split_scope(spec, unit)
    try:
        c = funcloc.locate(name, scope)
    except funcloc.LocateError as err:
        raise ValueError(str(err))
    path = Path(src) if src else Path(c.src)
    text = path.read_text()
    scope_name = "%s:%s" % (c.unit, name)
    defined = name in srcscan.defined_functions(path)
    if body:
        if defined or any(g.func == name for g in autotweak.guards(text)):
            raise ValueError("%s is C or guarded in %s already; sweep it without a body" % (scope_name, path))
        extra = Path(body).with_name("declarations.h")          # the draft's own extern lines, when it has them
        placed = insert_body(text, name, Path(body).read_text(),
                             extra.read_text().splitlines() if extra.exists() else ())
        if placed is None:
            raise ValueError("%s has no plain INCLUDE_ASM line in %s" % (scope_name, path))
        folder = Path(work) / ("%s_%s" % (c.unit, name))
        folder.mkdir(parents=True, exist_ok=True)
        scratch = folder / "source.c"
        scratch.write_text(placed)
        return Target(scope_name, name, c.unit, c, scratch, placed, True, str(body), "archived draft " + str(body))
    if not defined:
        raise ValueError("%s has no C body (INCLUDE_ASM only); give --body or --drafts" % scope_name)
    guarded = proof.guarded(path, name)
    return Target(scope_name, name, c.unit, c, path, text, guarded, None, "guarded draft" if guarded else "C")


# ---------------------------------------------------------------------------------------------
# compiling and scoring one target (needs the Docker toolchain)

class Scorer:
    """score(edits) -> rows of the target in the full unit compiled with the edits, against the original. The
    object of a variant with 0 rows is kept in self.objects for the proof; the others are deleted."""

    def __init__(self, target, work_root=WORK_ROOT):
        import funcdiff
        import funcdiff_verify as proof
        self.target, self.funcdiff, self.proof = target, funcdiff, proof
        self.root = Path(work_root) / ("%s_%s" % (target.unit, target.name))
        expected, problem = funcdiff.expected_object(target.c)
        if problem:
            raise ValueError(problem)
        self.want = funcdiff.resolved_functions(expected, [target.name], target.c)[target.name]
        if self.want is None:
            raise ValueError("%s is missing from the original object" % target.name)
        self.objects = {}
        self.lock = threading.Lock()

    def directory(self, edits):
        d = self.root / hashlib.sha1(variant_label(edits).encode()).hexdigest()[:12]
        d.mkdir(parents=True, exist_ok=True)
        return d

    def compile(self, edits, non_matching=None, source=None):
        """(object, directory) of the unit with the edits applied."""
        t = self.target
        directory = self.directory(edits)
        obj, _notes = self.proof.compile_unit(
            t.c, directory, non_matching=t.non_matching if non_matching is None else non_matching,
            edit=(lambda pre: apply_edits(pre, edits)) if edits else None, source=source or t.source)
        return obj, directory

    def __call__(self, edits):
        obj, directory = self.compile(edits)
        got = self.funcdiff.resolved_functions(obj, [self.target.name])[self.target.name]
        if got is None:
            raise ValueError("%s is missing from the compiled object" % self.target.name)
        rows = 0 if self.funcdiff.keys(got) == self.funcdiff.keys(self.want) \
            else len(self.funcdiff.diff_lines(self.want, got))
        if rows:
            shutil.rmtree(directory, ignore_errors=True)
        else:
            with self.lock:
                self.objects[variant_key(edits)] = (obj, directory)
        return rows


def isolated_source(target):
    """Text of the target's file where only this function's draft is C (the other guards stay asm), or None when
    the file has no guard of it."""
    import autotweak
    for g in autotweak.guards(target.text):
        if g.func == target.name:
            return autotweak.rewrite_guard(target.text, target.name, target.text[g.body:g.else_start])
    return None


def last_line(err):
    return error_summary(err)


def prove(target, scorer, edits):
    """(equal, explanation) of the literal-byte proof of a 0-row variant. First on the sweep's own object; when
    that fails and the file has a guard of the function, again on a unit where only this function is C (the other
    guarded drafts of the file would move it)."""
    key = variant_key(edits)
    obj, directory = scorer.objects[key]
    try:
        equal, reason = scorer.proof.exact_match(obj, target.c, target.name, directory, True, True)
    except (OSError, ValueError, SystemExit) as err:
        equal, reason = False, "literal-byte proof unavailable: " + last_line(err)
    if equal:
        return True, reason
    only = isolated_source(target)
    if only is None:
        return False, reason
    scratch = Path(directory) / "isolated.c"
    scratch.write_text(only)
    try:
        iso, iso_dir = scorer.compile(key, non_matching=False, source=scratch)
        equal, reason2 = scorer.proof.exact_match(iso, target.c, target.name, iso_dir, True, True)
    except (OSError, ValueError, SystemExit) as err:
        return False, "%s; only this function as C: %s" % (reason, last_line(err))
    return equal, "only this function as C: " + reason2 if equal else "%s; only this function as C: %s" % (reason, reason2)


# ---------------------------------------------------------------------------------------------
# the recipe

def header_declarations(name, root="."):
    """[(path, line number, text)] of the declarations of `name` in the headers under include/."""
    pat = re.compile(r"^[ \t]*(?:extern[ \t]+)?[A-Za-z_][\w \t\*]*?\b%s[ \t]*\([^;{]*\)[ \t]*;" % re.escape(name))
    found = []
    for path in sorted(Path(root, "include").rglob("*.h")):
        try:
            lines = path.read_text(errors="replace").splitlines()
        except OSError:
            continue
        found += [(str(path.relative_to(root)), n, line.strip()) for n, line in enumerate(lines, 1) if pat.match(line)]
    return found


def uses(name, root="."):
    """[(path, line number)] of the lines of src/ that mention `name` (calls, definitions and data; the
    INCLUDE_ASM lines are not uses)."""
    pat = re.compile(r"\b%s\b" % re.escape(name))
    found = []
    for path in sorted(Path(root, "src").rglob("*.c")):
        try:
            lines = path.read_text(errors="replace").splitlines()
        except OSError:
            continue
        found += [(str(path.relative_to(root)), n) for n, line in enumerate(lines, 1)
                  if pat.search(line) and not line.lstrip().startswith("INCLUDE_ASM(")]
    return found


def recipe(target, edits, pre, root=".", unit_of=None):
    """Lines that say how to keep the winning view in the tree (CODING_STANDARDS.md 8a): the definition change for
    a function the file defines, the header prototype that follows it, the `*_OVERRIDE_<symbol>` guard for a
    callee, and the units whose sha1 must stay OK."""
    import funcloc
    unit_of = unit_of or funcloc.unit_of_path
    src = os.path.relpath(str(target.c.src), root) if os.path.isabs(str(target.c.src)) else str(target.c.src)
    lines, undefined, units = [], [], set()
    for e in edits:
        proto = protoverride.Proto(e.symbol, e.decl)
        where = uses(e.symbol, root)
        owners = sorted({unit_of(path) or "?" for path, _n in where})
        units.update(owners)
        defs = protoverride.definitions(pre, e.symbol)
        found = header_declarations(e.symbol, root)
        if defs:
            d = defs[0]
            restore = (" (after putting the draft body %s in place of the INCLUDE_ASM)" % target.body_path
                       if target.body_path and e.kind == "own" else "")
            lines.append("definition in %s%s: change `%s` to `%s`%s" % (
                src, restore, " ".join(pre[d.start:d.brace].split()), " ".join(protoverride._redefine(d, proto).split()),
                "; the body has no `return <value>`, so `$v0` stays live at the exit (implicit int)"
                if e.kind == "own" and not has_value_return(pre[d.brace:d.end]) else ""))
            for path, n, text in found:
                if " ".join(text.split()) == " ".join(e.decl.split()):
                    lines.append("header prototype %s:%d already says `%s`" % (path, n, e.decl.rstrip(";")))
                else:
                    lines.append("header prototype %s:%d: `%s` becomes `%s`, the type of the definition" % (
                        path, n, text.rstrip(";"), e.decl.rstrip(";")))
            if not found:
                lines.append("header prototype: %s is not declared in include/" % e.symbol)
        else:
            undefined.append(proto)
            lines += ["alternative to the guard: change %s:%d `%s` to `%s` for every user" % (
                path, n, text.rstrip(";"), e.decl.rstrip(";")) for path, n, text in found]
        lines.append("uses of %s (%d, units %s): %s%s" % (
            e.symbol, len(where), ", ".join(owners) or "none", ", ".join("%s:%d" % w for w in where[:6]) or "none",
            " ..." if len(where) > 6 else ""))
    lines += protoverride.commit_hint(undefined, os.path.join(root, "include"))
    lines.append("then run `ninja`: every unit above (%s) must stay sha1 OK; a unit that breaks needs the "
                 "`*_OVERRIDE_<symbol>` guard in its own file instead of the shared type" % (", ".join(sorted(units)) or "none"))
    return lines


# ---------------------------------------------------------------------------------------------
# the run

def sweep_target(target, jobs, budget, log=print):
    """Sweep one target; returns the per-target result dict."""
    import funcdiff_verify as proof
    started = time.time()
    scorer = Scorer(target)
    pre = proof.preprocess_draft(target.source, target.non_matching)
    defs = protoverride.definitions(pre, target.name)
    result = dict(scope=target.scope, note=target.note, body=target.body_path)
    if not defs:
        result.update(status="no-definition", baseline=None, best_rows=None, best=[], compiled=0, untested=[],
                      variants=[], seconds=0)
        return result
    hints, asm_order = None, []
    import trailing_pad
    path = trailing_pad.find_asm(target.c.src, target.name)
    if path is not None:
        hints, asm_order = asm_hints(Path(path).read_text(errors="replace"))
    body = pre[defs[0].start:defs[0].end]
    callees = body_calls(body, target.name)
    if asm_order:
        callees = [c for c in callees if c in asm_order]
    own, per = build_plan(pre, target.name, callees, ignored_results(body, callees))
    sweep = Sweep(scorer, own, per, hints, budget, jobs, log)
    result.update(sweep.run())
    result["seconds"] = round(time.time() - started, 1)
    result["lattice"] = {"own": [e.decl for e in own], "callees": {c: [e.decl for e in es] for c, es in per.items()}}
    if result["status"] == "match":
        edits = sweep.best_edits()
        equal, reason = prove(target, scorer, edits)
        result["exact"] = {"equal": equal, "reason": reason}
        result["recipe"] = recipe(target, edits, pre) if equal else []
        if not equal:
            result["status"] = "match-unproven"
    if result["status"] not in ("match", "match-unproven"):
        shutil.rmtree(scorer.root, ignore_errors=True)
    return result


def format_table(results):
    """The ranked table: best rows first, then the largest gain."""
    def order(r):
        best, base = r.get("best_rows"), r.get("baseline")
        gain = base - best if best is not None and base is not None else 0
        return (BIG if best is None else best, -gain, r["scope"])
    head = "%-28s %-14s %6s %6s %5s %4s  %s" % ("target", "status", "base", "best", "tries", "unt", "edits")
    rows = [head, "-" * len(head)]
    for r in sorted(results, key=order):
        rows.append("%-28s %-14s %6s %6s %5s %4s  %s" % (
            r["scope"], r["status"], "err" if r.get("baseline") is None else r["baseline"],
            "-" if r.get("best_rows") is None else r["best_rows"], r.get("compiled", 0),
            len(r.get("untested") or []), " ".join(r.get("best") or []) or "-"))
    return "\n".join(rows)


def output_path(out, scope):
    """The JSON file of one target for --json OUT (a directory), or None when OUT is a single .json file."""
    return None if out.endswith(".json") else Path(out) / (scope.replace(":", "_") + ".json")


def report(result, out=print):
    out("%s: %s, baseline %s, best %s, %s compiles, %s UNTESTED, %ss" % (
        result["scope"], result["status"], result.get("baseline"), result.get("best_rows"), result.get("compiled"),
        len(result.get("untested") or []), result.get("seconds")))
    for key in ("error", "baseline_error"):
        if result.get(key):
            out("    %s: %s" % (key.replace("_", " "), result[key]))
    if result.get("best") and result.get("best_rows") != result.get("baseline"):
        out("    best: " + " ".join(result["best"]))
    for label, error in (result.get("untested") or [])[:UNTESTED_SHOWN]:
        out("    UNTESTED %s: %s" % (label, error))
    if len(result.get("untested") or []) > UNTESTED_SHOWN:
        out("    ... %d more UNTESTED (all in the JSON)" % (len(result["untested"]) - UNTESTED_SHOWN))
    if result.get("exact"):
        out("    literal-byte proof: %s (%s)" % ("MATCH" if result["exact"]["equal"] else "DIFF", result["exact"]["reason"]))
    for line in result.get("recipe") or []:
        out("    " + line)


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("funcs", nargs="*", metavar="[UNIT:]func")
    ap.add_argument("--guarded", action="store_true", help="every guarded draft of the tree")
    ap.add_argument("--unit", help="with --guarded: only this unit; with a bare function name: the unit that holds it")
    ap.add_argument("--targets", metavar="FILE", help="lines `UNIT:func [BODY.c]`")
    ap.add_argument("--drafts", action="append", default=[], metavar="DIR",
                    help="archived drafts DIR/<UNIT>/<func>/body.c (repeatable; only functions that are INCLUDE_ASM)")
    ap.add_argument("--body", metavar="BODY.c", help="a draft body for the function of --scope")
    ap.add_argument("--scope", metavar="UNIT:func")
    ap.add_argument("--src", metavar="FILE", help="a scratch copy of the C file to sweep (one target)")
    ap.add_argument("-j", type=int, default=2, help="parallel compiles (default 2: the machine is shared)")
    ap.add_argument("--budget", type=int, default=DEFAULT_BUDGET, help="compiles per target (default %d)" % DEFAULT_BUDGET)
    ap.add_argument("--json", metavar="OUT", help="a directory (one UNIT_func.json per target) or a .json file")
    ap.add_argument("--resume", action="store_true", help="skip targets whose JSON file already exists")
    args = ap.parse_args(argv)
    if not os.path.isdir("config"):
        sys.exit("viewsweep.py: run it from the repository root")
    if bool(args.body) != bool(args.scope):
        sys.exit("viewsweep.py: --body and --scope UNIT:func go together")
    work = [(f, None) for f in args.funcs]
    try:
        if args.guarded:
            work += [(s, None) for s in guarded_scopes(args.unit)]
        if args.targets:
            work += parse_targets(Path(args.targets).read_text())
        work += draft_bodies(args.drafts)
    except (OSError, ValueError) as err:
        sys.exit("viewsweep.py: %s" % err)
    if args.body:
        work.append((args.scope, args.body))
    if not work:
        sys.exit("viewsweep.py: no target (give functions, --guarded, --targets, --drafts or --body)")
    if args.src and len(work) != 1:
        sys.exit("viewsweep.py: --src names one file; give one target")
    results = []
    for n, (spec, body) in enumerate(work, 1):
        mark = "[%d/%d] " % (n, len(work))
        path = output_path(args.json, spec) if args.json else None
        if args.resume and path is not None and path.exists():
            results.append(json.loads(path.read_text()))
            print(mark + "%s: resumed from %s" % (spec, path), flush=True)
            continue
        try:
            target = make_target(spec, None if args.guarded else args.unit, body, args.src)
            result = sweep_target(target, args.j, args.budget, log=lambda m: print("    " + m, flush=True))
        except (ValueError, OSError, SystemExit) as err:
            reason = last_line(err) if isinstance(err, SystemExit) else (str(err).splitlines() or ["?"])[0]
            if body is not None and ("is C or guarded" in reason or "no plain INCLUDE_ASM" in reason):
                print(mark + "%s: skipped (%s)" % (spec, reason), flush=True)
                continue
            result = dict(scope=spec, status="error", error=reason, baseline=None, best_rows=None, best=[],
                          compiled=0, untested=[], variants=[], seconds=0)
        results.append(result)
        report(result, lambda line: print(mark + line if not line.startswith("    ") else line, flush=True))
        if path is not None:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(json.dumps(result, indent=1, sort_keys=True) + "\n")
    if args.json and args.json.endswith(".json"):
        Path(args.json).parent.mkdir(parents=True, exist_ok=True)
        Path(args.json).write_text(json.dumps(results, indent=1, sort_keys=True) + "\n")
    print("\n" + format_table(results))
    counts = collections.Counter(r["status"] for r in results)
    print("\nviewsweep: %s" % (", ".join("%d %s" % (v, k) for k, v in sorted(counts.items())) or "nothing swept"))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
