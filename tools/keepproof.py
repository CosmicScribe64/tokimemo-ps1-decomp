#!/usr/bin/env python3
"""Byte-match necessity proofs for the `keep` lines of config/migrate_globals.txt (T-20820).

Background (wiki/game-state.md, wiki/keep-lines.md). The aggregate `GameState D_800E6280` absorbs the
splat globals inside it, and a `keep` line gives one source file a separate symbol back. T-7010 accepts
such a line only when the original object has an as1 "hoisted pair" on that symbol. The counter-tail
family (TACO `func_8014616C`, `func_80146754`, `func_801484E4`, `func_80149538`, ...) has no hoisted
pair: the original reads the frame counter 0x800E7384 once for a compare chain and a second time,
as another uopt variable, for the final `+= 1`; plain C reproduces that only with a second symbol. This
tool turns "the separate symbol is needed" into a check anyone can repeat:

  keep <src file> <symbol> proof=<func>:<rows_without>:<asm_sha1_8> <reason>

  necessity    the unit compiled with the symbol folded into the aggregate
               (migrate_globals.rewrite_text, the symbol becomes `D_800E6280.unk_1104.w`) differs from
               the original in <rows_without> > 0 instruction rows of <func> (funcdiff, relocations
               resolved);
  sufficiency  the source as committed (<func> plain C, the symbol separate) links to the original's
               bytes: funcdiff_verify.exact_match, the literal-byte proof of funcdiff MATCH;
  identity     <asm_sha1_8> is migrate_globals.asm_sha1 of the function's original asm, so the line
               goes stale when the function or the original changes.

`migrate_globals.py --check` verifies the cheap part (plain C, sha1); `--verify-proofs` calls
verify_config() here and recompiles.

Usage (Docker, repo root, after configure.py and a build):
  python3 tools/keepproof.py check UNIT:func --symbol D_800E7384 [--src FILE | --from-draft] [--declare s32]
                                             [--write-scratch FILE] [--reason TEXT] [--no-build]
      prove the symbol necessary for func and print the ready-made keep line. --src names a scratch copy
      of the unit's C file (the committed file is the default); the function must be plain C there.
      --declare TYPE adds `extern TYPE SYMBOL;` after the includes when no header declares the symbol
      (the declaration the package adds to include/main_api.h together with the keep line).
      --from-draft proves the committed guarded draft written as plain C with the separate symbol in the
      statement of its last use of the field (--write-scratch FILE keeps that text for the package).
      Exit 0 when the proof holds, 1 otherwise.
  python3 tools/keepproof.py census [--json OUT] [--top N]
      the counter-tail shape in the original asm of every function (see SHAPE below), and for the
      matched ones whether their C needed a keep, a volatile access or an absolute address.
  python3 tools/keepproof.py rows [--only NAME ...] [--max-fields N]
      the control functions and the candidates as unmatched drafts: rows with each aggregate field
      read as a separate symbol and without.

SHAPE (census). In one function's asm, for an address X inside an aggregate reached by `lui; l?
%lo(X)`: a first load L1 of X; later a load L2 of X (no store to X in between: the value is unchanged)
whose value is changed by arithmetic and stored back to X (a read-modify-write on the reload); and the
value of L1 is read between L1 and L2 (the chain of compares). A hit is `held` when a register that
holds L1's value is still read after a call (a callee-saved copy or a spill), the literal func_8014616C
form. The shape is a static approximation: a plain global read early and incremented late after calls has
it too, so the census counts how many matched functions have it without needing a trick.
"""
import argparse
import json
import os
import re
import sys
import tempfile
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import migrate_globals as mg  # noqa: E402

DEFAULT_REASON = "second uopt variable for the late read (byte-match necessity proof, T-20820)"
CONFIG = mg.CONFIG


class FoldError(Exception):
    pass


# ------------------------------------------------------------------------------------- source rewrite

def folded_names(text, aggs, symbol):
    """The other aggregate fields that `text` names (they stay as written when `symbol` is folded)."""
    return {tok for _s, _e, tok in mg.code_tokens(text) if tok != symbol and mg.agg_for(aggs, tok)}


def fold_symbol(text, aggs, views, symbol):
    """`text` with every use of `symbol` written as its field of the aggregate and its own extern
    deleted (migrate_globals.rewrite_text with an empty keep set for that symbol; other absorbed names
    of the file stay, so only this one symbol changes). Raises FoldError when a use cannot be mapped."""
    if not mg.SYM_RE.match(symbol) or mg.agg_for(aggs, symbol) is None:
        raise FoldError("%s is not a field of an aggregate of %s" % (symbol, CONFIG))
    if symbol not in {tok for _s, _e, tok in mg.code_tokens(text)}:
        raise FoldError("%s is not used in the source" % symbol)
    new, problems = mg.rewrite_text(text, aggs, folded_names(text, aggs, symbol), views,
                                    lambda s: views.view(text, s))
    if problems:
        raise FoldError("; ".join("line %d: %s: %s" % (ln, s, m) for ln, s, m in problems))
    return mg.drop_declarations(new, [symbol])


def declare_symbol(text, views, symbol, ctype):
    """`text`, plus `extern ctype symbol;` after the last #include when ctype is given and no
    declaration of the symbol is visible (own, included header, main_api.h)."""
    if not ctype or views.view(text, symbol):
        return text
    incl = list(re.finditer(r"^[ \t]*#[ \t]*include[^\n]*\n", text, re.M))
    at = incl[-1].end() if incl else 0
    return "%sextern %s %s;\n%s" % (text[:at], ctype, symbol, text[at:])


def unguard(text, func):
    """`text` with the NON_MATCHING arm of the guarded draft of `func` as plain C (the INCLUDE_ASM arm
    dropped), or None when `func` has no such guard."""
    m = re.search(r'#ifdef NON_MATCHING\n(.*?)#else\nINCLUDE_ASM\("[^"]*",\s*%s\);\n#endif\n' % re.escape(func),
                  text, re.S)
    return text[:m.start()] + m.group(1) + text[m.end():] if m else None


def from_draft(text, aggs, func, symbol):
    """(plain text, type): the committed guarded draft of `func` written as plain C whose last statement
    on the aggregate field of `symbol` uses the separate symbol (`D_800E7384 += 1;`; a volatile cast or an
    absolute address of the draft's tail goes). A starting point for the package that commits the line;
    FAKE comments of the draft stay for the author to rewrite."""
    plain = unguard(text, func)
    if plain is None:
        raise FoldError("%s has no NON_MATCHING guard with an INCLUDE_ASM arm in the source" % func)
    span = function_span(plain, func)
    uses = field_uses(plain[span[0]:span[1]], aggs[0])
    key = next((k for k in uses if k[0] == symbol), None)
    if key is None:
        raise FoldError("%s does not use the field at %s" % (func, symbol))
    body = separate_variants(plain[span[0]:span[1]], symbol, uses[key])["tail"]
    return plain[:span[0]] + body + plain[span[1]:], key[1]


# ------------------------------------------------------------------------------------- toolchain

class Toolchain:
    """The compile and score steps (Docker): the pipeline of funcdiff for a text variant of a unit.
    `diff` holds the funcdiff rows (- original, + built) of the last rows() call."""

    diff = ()

    def locate(self, func, scope):
        import funcdiff
        return funcdiff.locate(func, scope)

    def asm_path(self, c, func):
        import trailing_pad
        return trailing_pad.find_asm(c.src, func)

    def compile(self, c, text, work, non_matching=False):
        """Object of the whole unit of `c` with `text` as its source: gcc -E, tools/cc.py IDO 5.3,
        trailing pad, object layout (order_search.compile_variant, the text form of
        funcdiff_verify.compile_unit)."""
        import order_search
        return order_search.compile_variant(c, text, Path(work), non_matching)

    def rows(self, c, obj, func):
        """Number of differing instruction rows of `func` in `obj` against the original, relocations
        resolved (0 = identical words); raises ValueError when the function is missing."""
        import funcdiff
        expected, problem = funcdiff.expected_object(c)
        if problem:
            raise ValueError(problem)
        want = funcdiff.resolved_functions(expected, [func], c)[func]
        got = funcdiff.resolved_functions(obj, [func])[func]
        if want is None or got is None:
            raise ValueError("%s is missing from the %s object" % (func, "original" if want is None else "built"))
        self.diff = funcdiff.diff_lines(want, got)
        return diff_rows(funcdiff.keys(want), funcdiff.keys(got), self.diff)

    def exact(self, c, obj, func, work, build=True):
        """(equal, explanation): the literal-byte proof of funcdiff, the object linked in the unit."""
        import funcdiff_verify
        return funcdiff_verify.exact_match(obj, c, func, Path(work), build, True)


def diff_rows(want, got, lines):
    """Differing instruction rows: 0 for equal rows, else the larger of the removed and added lines of
    funcdiff.diff_lines (equal length: one per differing position)."""
    if want == got:
        return 0
    removed = sum(1 for x in lines if x.startswith("-"))
    added = sum(1 for x in lines if x.startswith("+"))
    return max(removed, added, 1)


# ------------------------------------------------------------------------------------- the proof

class Result:
    def __init__(self, func, symbol, src_rel):
        self.func, self.symbol, self.src_rel = func, symbol, src_rel
        self.plain = None
        self.sufficient = None
        self.why = ""
        self.rows_with = None
        self.rows_without = None
        self.sha = None
        self.diff_with = []
        self.problems = []

    @property
    def necessary(self):
        return self.rows_without is not None and self.rows_without > 0

    @property
    def ok(self):
        return bool(self.plain and self.sufficient and self.necessary and not self.problems)

    def proof(self):
        return mg.Proof(self.func, self.rows_without, self.sha) if self.ok else None

    def keep_line(self, reason=DEFAULT_REASON):
        return "keep %s %s %s %s" % (self.src_rel, self.symbol, self.proof(), reason) if self.ok else None

    def report(self):
        yes = {True: "yes", False: "NO", None: "not run"}
        out = ["%s: %s in %s" % (self.func, self.symbol, self.src_rel)]
        out.append("  plain C:      %s" % yes[self.plain])
        if self.sufficient is not None:
            out.append("  sufficiency:  %s (symbol separate: %s; rows %s)" % (
                yes[self.sufficient], self.why, self.rows_with if self.rows_with is not None else "?"))
        if self.rows_without is not None:
            out.append("  necessity:    %s (symbol folded into the aggregate: %d differing rows)" % (
                yes[self.necessary], self.rows_without))
        if self.sha:
            out.append("  asm sha1:     %s" % self.sha)
        if self.sufficient is False and self.diff_with:
            out.append("  rows with the separate symbol (- original, + built):")
            out += ["    " + x for x in self.diff_with[:12]]
        for p in self.problems:
            out.append("  problem:      %s" % p)
        out.append("  verdict:      %s" % ("PROVEN" if self.ok else "NOT PROVEN"))
        return "\n".join(out)


def prove(tc, root, aggs, c, func, symbol, text, declare=None, work=None, build=True):
    """Necessity and sufficiency of the separate `symbol` for the plain C function `func` in the
    source `text` of the unit file `c`. Never raises for a tool failure; the Result lists problems."""
    src_rel = os.path.normpath(str(c.src)).replace(os.sep, "/")
    res = Result(func, symbol, src_rel)
    if not mg.plain_c(text, func):
        res.plain = False
        res.problems.append("%s is not plain C in the source (undefined, INCLUDE_ASM or a NON_MATCHING guard); "
                            "the committed build would use the asm and match itself" % func)
        return res
    res.plain = True
    views = mg.Views(root)
    with_text = declare_symbol(text, views, symbol, declare)
    try:
        without_text = fold_symbol(with_text, aggs, views, symbol)
    except FoldError as e:
        res.problems.append("cannot fold %s: %s" % (symbol, e))
        return res
    asm = tc.asm_path(c, func)
    if asm is None:
        res.problems.append("no original asm of %s (run configure.py)" % func)
        return res
    res.sha = mg.asm_sha1(mg.read(asm))
    own = tempfile.TemporaryDirectory(prefix="keepproof-") if work is None else None
    base = Path(work) if work else Path(own.name)
    try:
        # necessity first: a folded source that already matches ends the check without the second compile
        for name, body in (("without", without_text), ("with", with_text)):
            d = base / name
            d.mkdir(parents=True, exist_ok=True)
            try:
                obj = tc.compile(c, body, d)
                rows = tc.rows(c, obj, func)
                if name == "without":
                    res.rows_without = rows
                    if rows == 0:
                        break
                else:
                    res.rows_with = rows
                    res.diff_with = list(getattr(tc, "diff", ()))
                    res.sufficient, res.why = tc.exact(c, obj, func, d, build)
            except (ValueError, OSError, SystemExit) as e:
                msg = str(e).strip()
                res.problems.append("%s compile or score failed: %s" % (name, msg.splitlines()[-1] if msg else e))
                return res
    finally:
        if own:
            own.cleanup()
    return res


def verify_config(root, aggs, keep, tc=None):
    """Recompile every proof= keep line (migrate_globals.py --verify-proofs): the lines of the failures."""
    tc = tc or Toolchain()
    lines = []
    for rel, syms in sorted(keep.items()):
        for sym, proof in [(s, pr) for s, prs in sorted(getattr(syms, "proofs", {}).items()) for pr in prs]:
            try:
                c = tc.locate(proof.func, rel)
            except Exception as e:                  # LocateError, AmbiguousError
                lines.append("%s: %s: keep %s: %s" % (CONFIG, rel, sym, e))
                continue
            res = prove(tc, root, aggs, c, proof.func, sym, mg.read(os.path.join(root, rel)))
            print(res.report())
            if not res.ok:
                lines.append("%s: %s: keep %s: proof %s not reproduced: %s" % (
                    CONFIG, rel, sym, proof, "; ".join(res.problems) or
                    ("not necessary" if res.sufficient else "the bytes do not match: " + res.why)))
            elif res.sha != proof.sha:
                lines.append("%s: %s: keep %s: proof %s is stale: the asm sha1 is %s" % (
                    CONFIG, rel, sym, proof, res.sha))
            elif res.rows_without != proof.rows:
                print("note: %s rows_without is %d now (the line says %d); the proof still holds"
                      % (proof.func, res.rows_without, proof.rows))
    return lines


# ------------------------------------------------------------------------------------- census: shape

class Hit:
    __slots__ = ("addr", "load", "reload", "store", "uses", "held")

    def __init__(self, addr, load, reload, store, uses, held):
        self.addr, self.load, self.reload, self.store, self.uses, self.held = addr, load, reload, store, uses, held

    def as_dict(self):
        return {"addr": "D_%08X" % self.addr, "load": self.load, "reload": self.reload,
                "store": self.store, "uses": self.uses, "held": self.held}


COPIES = {"or", "move", "addu"}
CALLER_SAVED = set(["$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3",
                    "$t4", "$t5", "$t6", "$t7", "$t8", "$t9"])
REG_RE = re.compile(r"\$\w+")
EMPTY = (frozenset(), False)


def instructions(lines):
    """[(op, args)] of one function's asm lines; the 1-based index is the `pos` of aggregate_audit."""
    import type_recovery as tr
    out = []
    for line in lines:
        if tr.LABEL_RE.match(line):
            continue
        m = tr.INSN_RE.match(line)
        if m:
            out.append((m.group(2), tr.split_args(m.group(3))))
    return out


def shape_hits(lines, unit, syms, ranges):
    """The Hits of the counter-tail shape (module docstring) in one function's asm lines.

    A linear pass like tools/aggregate_audit.py: `lui` registers name an address, a load or store whose
    %lo names that address is a direct access. Each register carries the ids of the aggregate loads its
    value comes from and whether arithmetic touched it; a call clears the caller-saved registers
    (callee-saved ones and $sp slots keep theirs), so a read after a call proves the value was held."""
    import type_recovery as tr
    his, vals, stack = {}, {}, {}
    loads, stores = defaultdict(list), defaultdict(list)
    reads, calls = [], []
    nid = 0
    pending = None

    def val(reg):
        return vals.get(reg, EMPTY)

    def mem_of(arg):
        lo = tr.LO_MEM_RE.match(arg)
        if lo:
            t = syms.resolve(unit, lo.group(1))
            return ("sym", (t[0], t[1] + tr.to_int(lo.group(2))) if t else None, lo.group(3))
        im = tr.IMM_MEM_RE.match(arg)
        if im:
            return ("imm", tr.to_int(im.group(1)), im.group(2))
        return None

    for pos, (op, args) in enumerate(instructions(lines), 1):
        delay_end, pending = pending, None
        srcs = [r for a in (args[1:] if op not in tr.STORES and op not in tr.BRANCHES else args)
                for r in REG_RE.findall(a)]
        ids = set()
        for r in srcs:
            ids |= val(r)[0]
        if ids:
            reads.append((pos, frozenset(ids)))
        if op in tr.LOADS or op in tr.STORES:
            store = op in tr.STORES
            rt = args[0]
            mem = mem_of(args[1]) if len(args) > 1 else None
            direct = None
            if mem and mem[0] == "sym" and mem[1] and his.get(mem[2]) == mem[1] and ranges.find(*mem[1]):
                direct = mem[1][1]
            if store:
                if direct is not None:
                    stores[direct].append((pos, val(rt)))
                elif mem and mem[0] == "imm" and mem[2] == "$sp" and op == "sw":
                    stack[mem[1]] = val(rt)
            elif op != "lwc2":
                if direct is not None:
                    nid += 1
                    loads[direct].append((nid, pos, rt))
                    vals[rt] = (frozenset([nid]), False)
                elif mem and mem[0] == "imm" and mem[2] == "$sp" and op == "lw":
                    vals[rt] = stack.get(mem[1], EMPTY)
                else:
                    vals[rt] = (val(mem[2])[0], True) if mem and val(mem[2])[0] else EMPTY
                his.pop(rt, None)
        elif op == "lui":
            h = tr.HI_RE.match(args[1]) if len(args) > 1 else None
            t = syms.resolve(unit, h.group(1)) if h else None
            his[args[0]] = (t[0], t[1] + tr.to_int(h.group(2))) if t else None
            vals[args[0]] = EMPTY
        elif op in ("jal", "jalr"):
            calls.append(pos)
            pending = "call"
        elif op in tr.BRANCHES:
            pass
        elif op not in tr.NO_DEST and args and args[0].startswith("$"):
            d = args[0]
            sv = [val(r) for r in REG_RE.findall(" ".join(args[1:]))]
            ids = frozenset().union(*[v[0] for v in sv]) if sv else frozenset()
            copy = op in COPIES and len(args) == 3 and args[2] == "$zero" or op == "move"
            vals[d] = (ids, any(v[1] for v in sv) or (bool(ids) and not copy))
            his.pop(d, None)
        if delay_end == "call":
            for r in CALLER_SAVED | {"$ra"}:
                vals.pop(r, None)
                his.pop(r, None)
    hits = []
    for addr, ls in loads.items():
        if len(ls) < 2:
            continue
        for spos, (sids, arith) in stores.get(addr, []):
            if not arith:
                continue
            found = None
            for rid, rpos, _r in ls:            # the reload: the value the store is computed from
                if rid not in sids or rpos >= spos:
                    continue
                for fid, fpos, _f in ls:        # the first load, unchanged until the reload
                    if fpos >= rpos or fid in sids or any(fpos < p < rpos for p, _v in stores.get(addr, [])):
                        continue
                    uses = [p for p, i in reads if fid in i and fpos < p < rpos]
                    if uses:
                        held = any(c > fpos and any(c < p < rpos for p in uses) for c in calls)
                        found = Hit(addr, fpos, rpos, spos, len(uses), held)
                        break
                if found:
                    break
            if found:
                hits.append(found)
    return hits


# ------------------------------------------------------------------------------------- census: C side

function_span = mg.function_span


def leading_comment(text, start):
    """The comment lines directly above a definition (up to the previous blank or code line)."""
    lines = text[:start].splitlines()
    out = []
    for line in reversed(lines):
        s = line.strip()
        if not s or not (s.startswith("/*") or s.startswith("*") or s.endswith("*/") or s.startswith("//")):
            break
        out.append(line)
    return "\n".join(reversed(out))


def classify_c(text, func, aggs):
    """Tags of the plain C function `func`: which trick the source needed for the aggregate fields.
    keep      a separate field symbol (D_XXXXXXXX inside an aggregate) is used,
    volatile  a line with `volatile` that names an aggregate or an address inside one,
    absolute  a numeric address literal inside an aggregate (`*(s32 *)0x800E7384`),
    fake      a FAKE comment on the function (any reason; informational),
    local     a `GameState *g = ...` pointer local (ordinary C; informational)."""
    span = function_span(text, func)
    if span is None:
        return None
    body = text[span[0]:span[1]]
    tags = set()
    bases = {g.base for g in aggs}
    for _s, _e, tok in mg.code_tokens(body):
        if mg.agg_for(aggs, tok) is not None:
            tags.add("keep")
    lits = [int(x, 16) for x in re.findall(r"\b0x([0-9A-Fa-f]{8})\b", body)]
    if any(g.addr <= v < g.end for g in aggs for v in lits):
        tags.add("absolute")
    for line in body.splitlines():
        if re.search(r"\bvolatile\b", line) and (any(b in line for b in bases) or any(
                g.addr <= int(x, 16) < g.end for g in aggs for x in re.findall(r"\b0x([0-9A-Fa-f]{8})\b", line))):
            tags.add("volatile")
    if "FAKE" in body or "FAKE" in leading_comment(text, span[0]):
        tags.add("fake")
    if any(re.search(r"\b%s\s*\*\s*\w+\s*=" % re.escape(g.tname), body) for g in aggs):
        tags.add("local")          # a pointer local to the aggregate: ordinary C, informational
    return tags


# ------------------------------------------------------------------------------------- census: tree

class FuncInfo:
    def __init__(self, unit, name, matched, rel_src, words):
        self.unit, self.name, self.matched, self.src, self.words = unit, name, matched, rel_src, words
        self.hits = []
        self.tags = None


def tree_functions(root, aggs, ranges):
    """FuncInfo of every original function with its shape hits (aggregate_audit's tree walk)."""
    import aggregate_audit as aa
    import type_recovery as tr
    syms = tr.load_labels(tr.data_files(root))
    owner = {}
    for path in mg.src_files(root):
        rel = os.path.normpath(os.path.relpath(path, root)).replace(os.sep, "/")
        for f in mg.object_asm_files(root, rel):
            owner[os.path.normpath(f)] = rel
    out = []
    for path in tr.function_files(root):
        rel = os.path.relpath(path, root).replace(os.sep, "/")
        m = tr.ASM_PATH_RE.match(rel)
        unit = m.group(1) if m and m.group(1) else "main"
        name, lines = tr.parse_function_file(path)
        f = FuncInfo(unit, name, "/matchings/" in rel, owner.get(os.path.normpath(path)),
                     len(mg.ASM_ROW_RE.findall("\n".join(lines).split("\nendlabel", 1)[0])))
        f.hits = shape_hits(lines, unit, syms, ranges)
        out.append(f)
    if not out:
        raise SystemExit("keepproof: no function asm under %s/asm (run configure.py first)" % root)
    return out


def census(root, aggs):
    """Dict with the census numbers (see main) and the per-function rows."""
    import aggregate_audit as aa
    _a, ranges = aa.load_ranges(root)
    funcs = tree_functions(root, aggs, ranges)
    cache = {}
    for f in funcs:
        if not f.hits or not f.matched or not f.src:
            continue
        if f.src not in cache:
            cache[f.src] = mg.read(os.path.join(root, f.src))
        text = cache[f.src]
        f.tags = classify_c(text, f.name, aggs) if mg.plain_c(text, f.name) else None
    return funcs


def summarize(funcs):
    hit = [f for f in funcs if f.hits]
    matched = [f for f in hit if f.matched]
    unmatched = [f for f in hit if not f.matched]
    tricks = ("keep", "volatile", "absolute")
    classified = [f for f in matched if f.tags is not None]
    plain = [f for f in classified if not f.tags & set(tricks)]
    held = [f for f in matched if any(h.held for h in f.hits)]
    held_classified = [f for f in held if f.tags is not None]
    held_plain = [f for f in held_classified if not f.tags & set(tricks)]
    return {
        "functions": len(funcs),
        "matched_functions": sum(1 for f in funcs if f.matched),
        "hit_functions": len(hit),
        "hit_matched": len(matched),
        "hit_unmatched": len(unmatched),
        "matched_classified": len(classified),
        "matched_plain": len(plain),
        "matched_trick": len(classified) - len(plain),
        "false_positive_rate": (len(plain) / len(classified)) if classified else None,
        "held_matched": len(held_classified),
        "held_matched_plain": len(held_plain),
        "held_false_positive_rate": (len(held_plain) / len(held_classified)) if held_classified else None,
        "held_unmatched": sum(1 for f in unmatched if any(h.held for h in f.hits)),
        "trick_counts": dict(Counter(t for f in classified for t in f.tags if t in tricks)),
        "other_tags": dict(Counter(t for f in classified for t in f.tags if t not in tricks)),
    }


def census_text(funcs, top=40):
    s = summarize(funcs)
    pct = lambda x: "n/a" if x is None else "%.1f%%" % (100 * x)    # noqa: E731
    out = ["census of the counter-tail shape (keepproof.py docstring), %d functions, %d matched"
           % (s["functions"], s["matched_functions"]),
           "  functions with the shape:    %d  (matched %d, unmatched %d)" % (
               s["hit_functions"], s["hit_matched"], s["hit_unmatched"]),
           "  matched, C found and plain:  %d of %d needed no keep, volatile or absolute address: "
           "false-positive rate %s" % (s["matched_plain"], s["matched_classified"], pct(s["false_positive_rate"])),
           "  matched that needed a trick: %d  %s   (other tags among the matched: %s)" % (
               s["matched_trick"], s["trick_counts"] or "", s["other_tags"] or "none"),
           "  held across a call (the func_8014616C form): %d matched (plain %d, false-positive rate %s), "
           "%d unmatched" % (s["held_matched"], s["held_matched_plain"], pct(s["held_false_positive_rate"]),
                             s["held_unmatched"])]
    out.append("")
    out.append("unmatched functions with the shape (candidates; held first, then by size):")
    cand = sorted((f for f in funcs if f.hits and not f.matched),
                  key=lambda f: (not any(h.held for h in f.hits), -f.words, f.unit, f.name))
    for f in cand[:top]:
        h = max(f.hits, key=lambda x: (x.held, x.uses))
        out.append("  %-9s %-18s %4d B  D_%08X  uses %d%s" % (
            f.unit, f.name, 4 * f.words, h.addr, h.uses, "  held" if h.held else ""))
    if len(cand) > top:
        out.append("  ... %d more (--json OUT lists all)" % (len(cand) - top))
    out.append("")
    out.append("matched functions that needed a trick:")
    for f in sorted((f for f in funcs if f.hits and f.matched and f.tags and f.tags & {"keep", "volatile", "absolute"}),
                    key=lambda f: (f.unit, f.name)):
        out.append("  %-9s %-18s %s" % (f.unit, f.name, ",".join(sorted(f.tags))))
    return "\n".join(out)


def census_json(funcs):
    return {"summary": summarize(funcs),
            "functions": [{"unit": f.unit, "name": f.name, "matched": f.matched, "src": f.src, "bytes": 4 * f.words,
                           "tags": sorted(f.tags) if f.tags is not None else None,
                           "hits": [h.as_dict() for h in f.hits]} for f in funcs if f.hits]}


# ------------------------------------------------------------------------------------- rows

# (unit, function, extra declaration lines the draft needs in today's tree)
CONTROLS = [("TACO", "func_8014616C", ()), ("TACO", "func_80146754", ("#define D_8015F830 D_8015F82C[1]",)),
            ("TACO", "func_801484E4", ()), ("TACO", "func_80149538", ())]
CANDIDATES = [("OMIMAI", "func_8013388C", ("void func_801340E4(void);",)), ("main", "func_800590CC", ()),
              ("main", "func_800591D8", ()), ("TACO", "func_8013506C", ()), ("SHOUGATU", "func_801347E0", ()),
              ("main", "csr_load_vram", ())]

PATH_RE = re.compile(r"(?:\.\w+|\[\s*(?:0x[0-9A-Fa-f]+|\d+)\s*\])+")


def path_target(agg, path):
    """(offset, Type) of a constant field path such as `.unk_1BC[3].unk_0C` of the aggregate, or None."""
    t, off = agg.type, 0
    for m in re.finditer(r"\.(\w+)|\[\s*(0x[0-9A-Fa-f]+|\d+)\s*\]", path):
        if m.group(1):
            if t.kind not in ("struct", "union"):
                return None
            hit = next((f for f in t.fields if f[0] == m.group(1)), None)
            if hit is None:
                return None
            off, t = off + hit[1], hit[2]
        else:
            if t.kind != "array" or int(m.group(2), 0) >= t.count:
                return None
            off, t = off + int(m.group(2), 0) * t.elem.size, t.elem
    return off, t


CAST_RE = re.compile(r"\*\s*\(\s*(?:volatile\s+)?(\w+)\s*\*\s*\)\s*&?\s*$")
ABS_RE = re.compile(r"\*\s*\(\s*(?:volatile\s+)?(\w+)\s*\*\s*\)\s*0x([0-9A-Fa-f]{8})\b")


def field_uses(text, agg):
    """{(symbol, type spelling): [(start, end)]} of the uses of scalar fields of `agg` in `text`: the
    constant field paths (`D_800E6280.unk_1104.w`, also inside a `*(volatile s32 *)&...` cast of the
    field's own type) and the absolute reads/writes of a field's address (`*(s32 *)0x800E7384`)."""
    out = defaultdict(list)
    for m in re.finditer(r"\b%s(%s)" % (re.escape(agg.base), PATH_RE.pattern), text):
        target = path_target(agg, m.group(1))
        if not target or target[1].kind != "scalar":
            continue
        start = m.start()
        wrap = CAST_RE.search(text[:start])
        if wrap and wrap.group(1) == target[1].spell() and text[:start].rstrip().endswith("&"):
            start = wrap.start()
        out[("D_%08X" % (agg.addr + target[0]), target[1].spell())].append((start, m.end()))
    for m in ABS_RE.finditer(text):
        addr = int(m.group(2), 16)
        if agg.addr < addr < agg.end:
            scalar = [t for _p, t in mg.subobjects(agg.type, addr - agg.addr)
                      if t.kind == "scalar" and t.spell() == m.group(1)]
            if scalar:
                out[("D_%08X" % addr, m.group(1))].append((m.start(), m.end()))
    for spans in out.values():
        spans.sort()
    return out


def replace_spans(body, items):
    """`body` with the (start, end, text) items replaced; items are sorted and do not overlap."""
    out, last = [], 0
    for s, e, text in sorted(items):
        out.append(body[last:s] + text)
        last = e
    return "".join(out) + body[last:]


def separate_variants(body, sym, spans):
    """{mode: body}: every use of the field written as `sym` (all), or the uses in the statement of the
    last use only (tail: `D += 1;` at the end of a function, FAKE casts and absolute addresses included)."""
    last_s, last_e = spans[-1]
    lo = max(body.rfind(c, 0, last_s) for c in ";{}") + 1
    hi = body.find(";", last_e) + 1 or len(body)
    return {"all": replace_spans(body, [(s, e, sym) for s, e in spans]),
            "tail": replace_spans(body, [(s, e, sym) for s, e in spans if lo <= s and e <= hi])}


def with_lines(text, lines):
    """`text` plus declaration `lines` after its last #include."""
    if not lines:
        return text
    incl = list(re.finditer(r"^[ \t]*#[ \t]*include[^\n]*\n", text, re.M))
    at = incl[-1].end() if incl else 0
    return text[:at] + "".join(x + "\n" for x in lines) + text[at:]


def archived_draft(root, unit, func):
    """Folder of the newest archived draft of `func` in drafts/ (highest ticket number) that has a
    body.c, or None."""
    best = None
    for p in (Path(root) / "drafts").glob("*/%s/%s/body.c" % (unit, func)):
        key = (p.parent.parent.parent.name, str(p))
        if best is None or key > best[0]:
            best = (key, p.parent)
    return best[1] if best else None


def draft_source(root, c, func, extra=()):
    """(text, origin): the guarded NON_MATCHING draft of `func` in its committed file; else the newest
    archived draft (body.c, declarations.h of drafts/*/<unit>/<func>) spliced into the committed file in
    place of the function's INCLUDE_ASM line (bulk_m2c.insert_draft); (None, reason) when there is none.
    `extra` declaration lines (what today's headers need) are added after the includes."""
    text = mg.read(str(c.src))
    import srcscan
    defined = set(srcscan.DEF_RE.findall(text)) | set(srcscan.KR_DEF_RE.findall(text))
    if func in defined and not mg.plain_c(text, func):
        return with_lines(text, extra), "committed guarded draft"
    folder = archived_draft(root, c.unit, func)
    if folder is None:
        return None, "no draft"
    import bulk_m2c
    decls = []
    if (folder / "declarations.h").exists():
        decls = [x for x in (folder / "declarations.h").read_text().splitlines() if x.strip()]
    spliced = bulk_m2c.insert_draft(text, func, (folder / "body.c").read_text(), decls)
    if spliced is None:
        return None, "cannot splice %s (no plain INCLUDE_ASM line)" % folder
    return with_lines(spliced, extra), "archived draft %s" % folder


def compiler_error(e):
    """The line of a toolchain failure that says what is wrong (an `error` line, else the last line)."""
    lines = [x.strip() for x in str(e).strip().splitlines() if x.strip()]
    hit = [x for x in lines if re.search(r"\berror\b|Error:|undefined|not declared", x, re.I)]
    return (hit or lines or [str(e)])[0][:160]


def run_rows(root, aggs, targets, max_fields, tc=None, out=lambda s: print(s, flush=True)):
    """Rows of each target draft as it is (base) and with the aggregate fields it uses written as
    separate symbols: one field at a time, every use (all) or only the statement of the last use
    (tail), and all of the picked fields at once. The base symbol of the aggregate is not a field. Returns dicts."""
    tc = tc or Toolchain()
    agg = aggs[0]
    views = mg.Views(root)
    fmt = "%-9s %-15s %-24s %5s %6s %6s %6s  %s"
    out(fmt % ("unit", "function", "separate symbol", "uses", "base", "all", "tail", "draft"))
    results = []
    for unit, func, extra in targets:
        try:
            c = tc.locate(func, unit)
        except Exception as e:
            out("%-9s %-15s %s" % (unit, func, e))
            continue
        text, origin = draft_source(root, c, func, extra)
        span = function_span(text, func) if text is not None else None
        if span is None:
            out("%-9s %-15s %s" % (unit, func, "no draft to score (%s)" % origin if text is None else
                                   "the draft has no definition (%s)" % origin))
            continue
        body = text[span[0]:span[1]]
        picks = [kv for kv in sorted(field_uses(body, agg).items(), key=lambda kv: -len(kv[1]))
                 if kv[0][0] != agg.base][:max_fields]

        def score(variant, ctypes, name, tmp):
            for sym, ctype in ctypes:
                variant = declare_symbol(variant, views, sym, ctype)
            try:
                return tc.rows(c, tc.compile(c, variant, Path(tmp) / name, True), func)
            except (ValueError, OSError, SystemExit) as e:
                return "err: " + compiler_error(e)

        with tempfile.TemporaryDirectory(prefix="keeprows-") as tmp:
            base = score(text, [], "base", tmp)
            if not isinstance(base, int):
                out(fmt % (unit, func, "", "", base, "", "", origin))
                continue
            if not picks:
                out(fmt % (unit, func, "(no scalar field used)", 0, base, "-", "-", origin))
            for (sym, ctype), spans in picks:
                row = {"unit": unit, "func": func, "symbol": sym, "uses": len(spans), "base": base}
                for mode, nb in separate_variants(body, sym, spans).items():
                    row[mode] = score(text[:span[0]] + nb + text[span[1]:], [(sym, ctype)], sym + mode, tmp)
                results.append(row)
                out(fmt % (unit, func, "%s %s" % (sym, ctype), len(spans), base, row["all"], row["tail"], origin))
            if len(picks) > 1:
                items = [(s, e, sym) for (sym, _t), spans in picks for s, e in spans]
                every = score(text[:span[0]] + replace_spans(body, items) + text[span[1]:], [k for k, _v in picks],
                              "every", tmp)
                results.append({"unit": unit, "func": func, "symbol": "every", "uses": len(items), "base": base,
                                "all": every, "tail": None})
                out(fmt % (unit, func, "all %d fields" % len(picks), len(items), base, every, "-", origin))
    return results


# ------------------------------------------------------------------------------------- main

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--root", default=".")
    sub = ap.add_subparsers(dest="cmd", required=True)
    ck = sub.add_parser("check", help="necessity and sufficiency of one separate symbol")
    ck.add_argument("target", help="UNIT:func (a unit name, or a C file path before the colon)")
    ck.add_argument("--symbol", required=True)
    ck.add_argument("--src", help="scratch copy of the unit's C file (default: the committed file)")
    ck.add_argument("--from-draft", action="store_true",
                    help="prove the committed guarded draft written as plain C with the separate symbol "
                         "in its last statement on the field (implies --declare with the field's type)")
    ck.add_argument("--write-scratch", metavar="FILE", help="write the text that --from-draft proves")
    ck.add_argument("--declare", metavar="TYPE", help="declare the symbol with this type when no header does")
    ck.add_argument("--reason", default=DEFAULT_REASON)
    ck.add_argument("--no-build", action="store_true", help="do not run ninja for the unit's link inputs")
    ce = sub.add_parser("census", help="the counter-tail shape in the original asm")
    ce.add_argument("--json")
    ce.add_argument("--top", type=int, default=40)
    rw = sub.add_parser("rows", help="controls and candidates as drafts: rows with and without a separate symbol")
    rw.add_argument("--only", nargs="*", help="function names to run (default: the six candidates)")
    rw.add_argument("--controls", action="store_true", help="add the four counter-tail functions of TACO")
    rw.add_argument("--max-fields", type=int, default=4)
    a = ap.parse_args(argv)
    if a.root != ".":
        os.chdir(a.root)
    try:
        aggs, keep = mg.load_config(".")
    except mg.LayoutError as e:
        print("keepproof: layout: %s" % e)
        return 1
    if a.cmd == "check":
        unit, _, func = a.target.rpartition(":")
        if not unit or not func:
            ap.error("target must be UNIT:func")
        tc = Toolchain()
        try:
            c = tc.locate(func, unit)
        except Exception as e:
            print("keepproof: %s" % e)
            return 1
        text = mg.read(a.src or str(c.src))
        declare = a.declare
        if a.from_draft:
            try:
                text, ctype = from_draft(text, aggs, func, a.symbol)
            except FoldError as e:
                print("keepproof: %s" % e)
                return 1
            declare = declare or ctype
            if a.write_scratch:
                with open(a.write_scratch, "w") as f:
                    f.write(text)
        res = prove(tc, ".", aggs, c, func, a.symbol, text, declare, build=not a.no_build)
        print(res.report())
        if a.src or a.from_draft:
            print("  note:         %s was proven, not the committed %s; commit it before adding the line"
                  % ("the draft written as plain C" if a.from_draft else "--src " + a.src, res.src_rel))
        if res.ok:
            print("\n" + res.keep_line(a.reason))
        return 0 if res.ok else 1
    if a.cmd == "census":
        funcs = census(".", aggs)
        print(census_text(funcs, a.top))
        if a.json:
            with open(a.json, "w") as f:
                json.dump(census_json(funcs), f, indent=1)
        return 0
    targets = [t for t in (CONTROLS if a.controls or a.only else []) + CANDIDATES if not a.only or t[1] in a.only]
    run_rows(".", aggs, targets, a.max_fields)
    return 0


if __name__ == "__main__":
    sys.exit(main())
