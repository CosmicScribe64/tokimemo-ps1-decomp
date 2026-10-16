#!/usr/bin/env python3
"""T-16560: redraft every unmatched TACO function with the recovered types and score it like T-14500.

Run inside the project Docker image from the repo root:
    python3 drafts/T-16560/redraft.py [--jobs 2] [--timeout 120] [--only func_X ...]

Two drafts per function, each through the census path of tools/exhaustive_drafts.py (m2c with the
default context, which now holds the T-16560 types; the bulk_m2c cleanup; full-translation-unit
compile with the cheap declaration fixes; order_search's resolved diff as the score):
  plain  the m2c draft as the census made it, only the headers differ;
  typed  the same draft after the type-driven rewrites below.
The better of the two then goes through tools/draft_fixes.py (all rewrites, each alone, greedy
build-up, as `draft_fixes.py rescore` does). The best body, its full unit, its declarations and a
result.json with every score and the census score land in drafts/T-16560/TACO/<func>/.

Type-driven rewrites (each is a candidate; the compiler decides):
  actor-offset    `*(T *)((unsigned char *)SYM + (N))` and `SYM + N` on a typed global table or
                  pointer (TcActor *D_8015EDB4, Tc14 tables, TcObj50/Tc10/TcPos arrays): written as
                  the element and field, `D_8015EDB4[8].unk2`; m2c prints byte offsets past the
                  first element.
  pointer-step    `p += 0x14` on a local `Tc14 *p`: m2c prints the byte step, C needs the element step.
  matrix-copy     eight word copies from D_80129C40 or D_80125C60 into a stack local: one MATRIX copy.
  field-symbols   splat names inside a declared aggregate (D_8015F5F2 in TacoCam D_8015F5F0,
                  D_8012749C in D_80127480[0], GameState fields): the field access, as
                  tools/migrate_globals.py writes it.
Nothing under src/ or include/ is edited.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import signal
import subprocess
import sys
import time

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools"))
import bulk_m2c as bulk  # noqa: E402
import draft_fixes  # noqa: E402
import exhaustive_drafts as ex  # noqa: E402
import migrate_globals as mg  # noqa: E402

OUT = Path("drafts/T-16560")
UNIT = "TACO"
CENSUS = Path("drafts/T-14500/best-quality-catalog.jsonl")
T16510 = Path("drafts/T-16510/results.jsonl")
BIG = 10 ** 6


def digest(text):
    return hashlib.sha256(text.encode()).hexdigest()


# ------------------------------------------------------------------------------------------ types

def header_types():
    """name -> mg.Type for the typedefs of libgte.h, psx_types.h and the TACO header."""
    text = ""
    for h in ("include/libgte.h", "include/ovl/TACO.h"):
        text += mg.read(h) + "\n"
    text = re.sub(r"\bstruct\s+(\w+)\s*\*", r"\1 *", mg.strip_comments_keep_offsets(text))
    return {k: v for k, v in mg.parse_types(text).items() if not isinstance(v, mg.LayoutError)}


TYPES = header_types()
DECL_RE = re.compile(r"^\s*extern\s+(\w+)\s*(\*?)\s*(D_[0-9A-F]{8})\s*(\[(\w*)\])?\s*;", re.M)


def typed_globals():
    """SYM -> (struct Type, 'ptr' | 'array' | 'obj') for the TACO header's struct globals."""
    out = {}
    for m in DECL_RE.finditer(mg.read("include/ovl/TACO.h")):
        tname, star, sym, arr = m.group(1), m.group(2), m.group(3), m.group(4)
        t = TYPES.get(tname)
        if t is None or t.kind != "struct":
            continue
        out[sym] = (t, "ptr" if star else "array" if arr else "obj")
    return out


GLOBALS = typed_globals()


def leaf_at(t, rel, want_size):
    """C path (".a.b[2]") of the scalar of `want_size` bytes that starts at `rel` inside `t`."""
    for off, size, path, lt in mg.walk_layout(t):
        if lt.kind == "array":
            elem = lt.elem
            while elem.kind == "array":
                elem = elem.elem
            if off <= rel < off + size and (rel - off) % elem.size == 0 and elem.size == want_size:
                idx, rest = rel - off, []
                dims, e = [], lt
                while e.kind == "array":
                    dims.append(e.count)
                    e = e.elem
                stride = [elem.size] * len(dims)
                for i in range(len(dims) - 2, -1, -1):
                    stride[i] = stride[i + 1] * dims[i + 1]
                for s in stride:
                    rest.append(idx // s)
                    idx %= s
                return path + "".join("[%d]" % i for i in rest), elem
        elif off == rel and size == want_size and lt.kind in ("scalar", "ptr"):
            return path, lt
    return None, None


SIZES = {"u8": 1, "s8": 1, "u16": 2, "s16": 2, "u32": 4, "s32": 4}
OFF_RE = re.compile(r"\*\((u8|s8|u16|s16|u32|s32) \*\)\(\(unsigned char \*\)\(?(&?)(D_[0-9A-F]{8})(?:\[([^\[\]]+)\])?\)? \+ \((\d+)\)\)")


def actor_offsets(body):
    def element(sym, idx, k):
        if idx is None:
            return "%s[%d]" % (sym, k)
        return "%s[%s + %d]" % (sym, idx, k) if k else "%s[%s]" % (sym, idx)

    def repl(m):
        ctype, amp, sym, idx, n = m.group(1), m.group(2), m.group(3), m.group(4), int(m.group(5))
        if sym not in GLOBALS:
            return m.group(0)
        t, kind = GLOBALS[sym]
        if idx is not None and not amp:
            return m.group(0)
        k, r = divmod(n, t.size)
        path, lt = leaf_at(t, r, SIZES[ctype])
        if path is None:
            return m.group(0)
        if idx is not None:                       # &SYM[i] + n
            expr = element(sym, idx, k) + path
        elif amp and kind == "obj":               # &SYM + n
            if k:
                return m.group(0)
            expr = sym + path
        elif amp and kind == "ptr":               # address of the pointer itself
            return m.group(0)
        else:                                     # SYM + n (pointer value or array)
            expr = element(sym, None, k) + path
        if kind == "ptr" and k == 0 and idx is None:
            expr = sym + "->" + path[1:]
        store = re.match(r"\s*=(?!=)", m.string[m.end():]) is not None
        if lt.name == ctype or (store and lt.kind == "scalar" and lt.size == SIZES[ctype]):
            return expr                           # a store does not depend on the signedness
        return "*(%s *)&%s" % (ctype, expr)
    body = OFF_RE.sub(repl, body)

    def plus(m):
        sym, n = m.group(1), int(m.group(2), 0)
        if sym not in GLOBALS or GLOBALS[sym][1] == "obj":
            return m.group(0)
        t = GLOBALS[sym][0]
        if n % t.size:
            return m.group(0)
        return "&%s[%d]" % (sym, n // t.size)
    return re.sub(r"(?<![\w&])(D_[0-9A-F]{8}) \+ (0x[0-9A-Fa-f]+|\d+)\b(?!\s*[\[(])", plus, body)


LOCAL_PTR_RE = re.compile(r"^\s*(\w+) (\*+)(\w+);\s*$", re.M)
SCALAR_SIZES = {"s16": 2, "u16": 2, "s32": 4, "u32": 4}


def pointer_steps(body):
    """m2c prints pointer steps in bytes (`Tc14 *p; p += 0x14`, `s16 **q; q += 4`); C counts elements."""
    for tname, stars, name in LOCAL_PTR_RE.findall(body):
        if len(stars) > 1:
            size = 4
        elif tname in SCALAR_SIZES:
            size = SCALAR_SIZES[tname]
        else:
            t = TYPES.get(tname)
            size = t.size if t is not None and t.kind == "struct" else 0
        if size <= 1:
            continue

        def step(m, size=size):
            n = int(m.group(3), 0)
            if n % size:
                return m.group(0)
            return "%s %s %d" % (m.group(1), m.group(2), n // size)
        body = re.sub(r"\b(%s) ([-+]=) (0x[0-9A-Fa-f]+|\d+)\b" % re.escape(name), step, body)
    return body


MATRIX_GLOBALS = {"D_80129C40": "%s", "D_80125C60": "*(MATRIX *)%s"}
OBJ50_FIELDS = {"unk0": 0, "unk18": 0x18, "unk1C": 0x1C, "unk20": 0x20}
ID = r"[A-Za-z_]\w*"


def word_ref(expr):
    """(base, offset) of a word access as m2c and the bulk cleanup print it, or None.
    base is a variable name, or '*' + pointer name for an access through a pointer."""
    e = expr.strip()
    while e.startswith("(s32) "):
        e = e[6:].strip()
    pats = (
        (r"^\(\*\(s32 \*\)&(%s)\)$" % ID, lambda m: (m.group(1), 0)),
        (r"^\*\(s32 \*\)\(\(unsigned char \*\)\(&(%s)\) \+ \((\d+)\)\)$" % ID, lambda m: (m.group(1), int(m.group(2)))),
        (r"^\*\(s32 \*\)\(\(unsigned char \*\)(%s) \+ \((\d+)\)\)$" % ID,
         lambda m: (m.group(1) if m.group(1) in MATRIX_GLOBALS else "*" + m.group(1), int(m.group(2)))),
        (r"^\(\*\(s32 \*\)(%s)\)$" % ID,
         lambda m: (m.group(1) if m.group(1) in MATRIX_GLOBALS else "*" + m.group(1), 0)),
        (r"^(%s)\.t\[(\d)\]$" % ID, lambda m: (m.group(1), 0x14 + 4 * int(m.group(2)))),
        (r"^(%s)\.unk0$" % ID, lambda m: (m.group(1), 0)),
        (r"^(%s)->(unk0|unk18|unk1C|unk20)$" % ID, lambda m: ("*" + m.group(1), OBJ50_FIELDS[m.group(2)])),
        (r"^(D_80129C40)$", lambda m: (m.group(1), 0)),
    )
    for pat, fn in pats:
        m = re.match(pat, e)
        if m:
            return fn(m)
    return None


def matrix_lvalue(base, off, body):
    if not base.startswith("*"):
        if base in MATRIX_GLOBALS and off == 0:
            return MATRIX_GLOBALS[base] % base
        return base if off == 0 else None
    ptr = base[1:]
    if off == 4 and re.search(r"^\s*TcObj50 \*%s;" % re.escape(ptr), body, re.M):
        return "*(MATRIX *)%s->unk4" % ptr
    return "*(MATRIX *)%s" % ptr if off == 0 else "*(MATRIX *)((u8 *)%s + %d)" % (ptr, off)


def matrix_copies(body):
    """Eight word copies that move a whole MATRIX (from D_80129C40, D_80125C60, a coordinate object
    or a stack matrix), in any order and interleaved with other statements: one struct copy."""
    lines = body.split("\n")
    groups = {}
    for i, l in enumerate(lines):
        m = re.match(r"^(\s*)(.+?) = (.+);\s*$", l)
        if not m:
            continue
        d, s_ = word_ref(m.group(2)), word_ref(m.group(3))
        if d is None or s_ is None or d[0] == s_[0]:
            continue
        key = (d[0], s_[0], d[1] - s_[1])
        groups.setdefault(key, []).append((i, s_[1]))
    retype = set()
    for (dbase, sbase, delta), items in groups.items():
        offs = sorted(o for _, o in items)
        if len(items) != 8 or len(set(offs)) != 8:
            continue
        start = min(offs)
        if offs != list(range(start, start + 32, 4)):
            continue
        dst = matrix_lvalue(dbase, start + delta, body)
        src = matrix_lvalue(sbase, start, body)
        if dst is None or src is None:
            continue
        first = min(i for i, _ in items)
        indent = re.match(r"^(\s*)", lines[first]).group(1)
        lines[first] = "%s%s = %s;" % (indent, dst, src)
        for i, _ in items:
            if i != first:
                lines[i] = None
        for b in (dbase, sbase):
            if not b.startswith("*") and b not in MATRIX_GLOBALS:
                retype.add(b)
    out = [l for l in lines if l is not None]
    text = "\n".join(out)
    for name in retype:
        text = re.sub(r"^(\s*)(?:s32|\?) %s(?:\[\d*\])?;" % re.escape(name), r"\1MATRIX %s;" % name, text,
                      flags=re.M)
    for g in MATRIX_GLOBALS:
        text = re.sub(r"\b%s\.unk0\b" % g, "(*(s32 *)&%s)" % g, text)
    return text


class TypedAggregate(mg.Aggregate):
    """An aggregate built from a declaration instead of config/migrate_globals.txt."""

    def __init__(self, base, typ, header):
        self.base, self.tname, self.header = base, typ.spell(), header
        self.addr = int(base[2:], 16)
        self.types, self.type = TYPES, typ
        self.end = self.addr + typ.size


def aggregates():
    aggs, _keep = mg.load_config(".")
    counts = {"D_80127480": 64, "D_80127080": 64, "D_80128880": 64}
    for m in DECL_RE.finditer(mg.read("include/ovl/TACO.h")):
        tname, star, sym, arr, n = m.group(1), m.group(2), m.group(3), m.group(4), m.group(5)
        t = TYPES.get(tname) or (mg.base_type(tname, TYPES) if tname in mg.BASE_SIZES else None)
        if t is None or star:
            continue
        if arr is not None:
            count = int(n, 0) if n else counts.get(sym)
            if count is None and t.kind == "struct" and tname in ("Tc14", "TacoParticle14"):
                count = 16
            if not count:
                continue
            t = mg.Type("array", size=t.size * count, align=t.align, elem=t, count=count)
        elif t.kind != "struct":
            continue
        aggs.append(TypedAggregate(sym, t, "include/ovl/TACO.h"))
    return aggs


AGGS = aggregates()


def field_symbols(body, known):
    """(body, known) with splat names inside an aggregate written as fields; their declarations dropped."""
    views = {}
    for d in known.values():
        views.update(mg.declarations(d.rstrip() + "\n"))
    hdr = mg.declarations(mg.read("include/ovl/TACO.h")) | mg.declarations(mg.read("include/main_api.h"))
    new, _problems = mg.rewrite_text(body, AGGS, set(), None, lambda s: views.get(s) or hdr.get(s))
    used = set(re.findall(r"\bD_[0-9A-F]{8}\b", new))
    known = {k: v for k, v in known.items() if not (k.startswith("D_") and k not in used)}
    return new, known


TCPOS_CALLS = {"func_8015ACCC": (2, 4, 6),       # arg-word index of each TcPos parameter
               "func_8013D1E0": (0, 2)}          # no prototype: m2c prints subroutine_arg0..3
ASM_INS = re.compile(r"^\s*/\*[^*]*\*/\s+(\w+)\s+(.*?)\s*$")


def asm_lines(asm_text):
    out = []
    for line in asm_text.splitlines():
        m = ASM_INS.match(line)
        if m:
            out.append((m.group(1), [a.strip() for a in m.group(2).split(",")]))
        elif re.match(r"^\s*\.L\w+:", line):
            out.append(("label", []))
    return out


def tcpos_source(ins, j, home):
    """C expression for the TcPos copied to sp+home before the call at ins[j], or None."""
    reg = off = base = None
    for k in range(min(j + 1, len(ins) - 1), max(j - 80, 0), -1):     # the delay slot first
        op, a = ins[k]
        if k < j and (op in ("jal", "jalr") or (op == "label" and base is None)):
            return None
        if reg is None:
            if op == "swr" and len(a) == 2 and a[1] == "0x%X($sp)" % home:
                reg = a[0]
        elif base is None:
            if op == "lwr" and a[0] == reg:
                m = re.match(r"(-?0x[0-9A-Fa-f]+|\d+)?\((\$\w+)\)$", a[1])
                if not m:
                    return None
                off, base = int(m.group(1) or "0", 0), m.group(2)
        elif a and a[0] == base and op not in ("sw", "sh", "sb", "swl", "swr"):
            if op == "addiu" and len(a) == 3 and a[1] == "$sp":
                return "sp%X" % (int(a[2], 0) + off)
            m = re.match(r"%lo\((D_[0-9A-F]{8})\)", a[2]) if op == "addiu" and len(a) == 3 else None
            if m and off == 0:
                return m.group(1)
            if op == "lw" and a[1].startswith("%lo(D_8015EDB4)") and off in (0x74, 0x7C):
                return "*(TcPos *)&D_8015EDB4->unk%X" % off
            return ("reg", base[1:], off)
    return None


def reg_variable(body, reg):
    """The draft's variable for register `reg` when m2c named exactly one (temp_s0 or var_s0)."""
    names = [n for n in ("temp_" + reg, "var_" + reg) if re.search(r"\b%s\b" % n, body)]
    return names[0] if len(names) == 1 else None


def split_args(text, start):
    """(args, end) of the call whose '(' is at text[start]."""
    depth, args, cur, i = 0, [], start + 1, start
    while i < len(text):
        c = text[i]
        if c in "([":
            depth += 1
        elif c in ")]":
            depth -= 1
            if depth == 0:
                args.append(text[cur:i])
                return args, i
        elif c == "," and depth == 1:
            args.append(text[cur:i])
            cur = i + 1
        i += 1
    return None, None


def tcpos_args(body, asm_text):
    """Calls passing TcPos by value: m2c splits each into two words (`/* TcPos+0x0 */ X, /* TcPos+0x4 */ Y`)
    and loses the source of the register words. The asm names each copy's source; stack locals at
    spN..spN+6 become one `TcPos spN`."""
    if not asm_text:
        return body
    ins = asm_lines(asm_text)
    locals_ = set()
    for func, words in TCPOS_CALLS.items():
        sites = [j for j, (op, a) in enumerate(ins) if op == "jal" and a and a[0] == func]
        calls = [m.start() for m in re.finditer(r"\b%s\(" % func, body)]
        if not sites or len(sites) != len(calls):
            continue
        new, last = [], 0
        for site, pos in zip(sites, calls):
            open_ = pos + len(func)
            args, end = split_args(body, open_)
            if args is None:
                return body
            srcs = [tcpos_source(ins, site, 4 * w) for w in words]
            for k, src in enumerate(srcs):
                if isinstance(src, tuple):
                    var = reg_variable(body, src[1])
                    srcs[k] = None if var is None else "*(TcPos *)((u8 *)%s + %d)" % (var, src[2])
            marks = [i for i, x in enumerate(args) if "TcPos+0x0" in x]
            if not marks and len(args) >= 2 * len(words) and all(
                    re.sub(r"/\*.*?\*/", "", args[k]).strip() == "subroutine_arg%d" % k for k in range(2 * len(words))):
                marks = [2 * k for k in range(len(words))]
            if None in srcs or len(marks) != len(words):
                new.append(body[last:end + 1])
                last = end + 1
                continue
            plain = [re.sub(r"/\*.*?\*/", "", x).strip() for x in args]
            out, i, n = [], 0, 0
            while i < len(plain):
                if i in marks:
                    s = srcs[n]
                    n += 1
                    if re.match(r"^sp[0-9A-F]+$", s):
                        locals_.add(int(s[2:], 16))
                        s = "@TCPOS%s@" % s
                    out.append(s)
                    i += 2
                else:
                    out.append(plain[i])
                    i += 1
            new.append(body[last:open_] + "(" + ", ".join(out) + ")")
            last = end + 1
        new.append(body[last:])
        body = "".join(new)
    decl = re.compile(r"^[ \t]*(s16|u16|s32|u32|\?) (sp[0-9A-F]+)(\[\d*\])?;[ \t]*\n", re.M)
    for n in sorted(locals_):
        names = {"sp%X" % (n + d): f for d, f in ((0, "x"), (2, "y"), (4, "z"), (6, "unk6"))}
        found = [m for m in decl.finditer(body) if m.group(2) in names]
        kinds = {m.group(2): m.group(1) for m in found}
        for m in reversed(found):
            body = body[:m.start()] + body[m.end():]
        body = re.sub(r"\{\n", "{\n    TcPos @TCPOSsp%X@;\n" % n, body, count=1)
        for name, field in names.items():
            if name not in kinds or kinds[name] not in ("s16", "u16"):
                continue
            body = re.sub(r"(?<!@TCPOS)\b%s\b(?!@)" % name, "sp%X.%s" % (n, field), body)
    return re.sub(r"@TCPOS(sp[0-9A-F]+)@", r"\1", body)


def typed_rewrites(body, known, log=None, asm_text=None):
    steps = (("actor-offset", actor_offsets), ("pointer-step", pointer_steps), ("matrix-copy", matrix_copies),
             ("tcpos-args", lambda b: tcpos_args(b, asm_text)))
    for name, fn in steps:
        new = fn(body)
        if new != body and log is not None:
            log.append(name)
        body = new
    new, known = field_symbols(body, known)
    if new != body and log is not None:
        log.append("field-symbols")
    return new, known


# ------------------------------------------------------------------------------------------ workers

def inventory():
    rows = [r for r in ex.census() if r["unit"] == UNIT]
    return rows


def worker(index, variant):
    rows = json.loads((OUT / "inventory.json").read_text())
    row = rows[index]
    ex.BASE = OUT / "_runs" / variant
    os.environ["TOKIMEMO_REGENERATE_GUARDS"] = "1"
    applied = []
    if variant == "typed":
        plain_cleanup = bulk.cleanup

        def cleanup(decls, body, glob=None):
            body, known, problem = plain_cleanup(decls, body, glob)
            if problem:
                return body, known, problem
            asm = draft_fixes.asm_for(row["scope"], draft_fixes.asm_files("."))
            body, known = typed_rewrites(body, known, applied, asm)
            return body, known, None
        bulk.cleanup = cleanup
    got = ex.process(row)
    if variant == "typed":
        got["rewrites"] = applied
        ex.save(ex.BASE / row["unit"] / row["func"] / "result.json", json.dumps(got, sort_keys=True) + "\n")


def run_all(rows, variants, jobs, timeout, only):
    todo = [(i, v) for v in variants for i, r in enumerate(rows) if not only or r["func"] in only]
    todo = [(i, v) for i, v in todo if not (OUT / "_runs" / v / UNIT / rows[i]["func"] / "result.json").exists()]
    running = []
    while todo or running:
        while todo and len(running) < jobs:
            i, v = todo.pop(0)
            folder = OUT / "_runs" / v / UNIT / rows[i]["func"]
            folder.mkdir(parents=True, exist_ok=True)
            log = open(folder / "worker.log", "w")
            p = subprocess.Popen([sys.executable, __file__, "--worker", str(i), "--variant", v], stdout=log,
                                 stderr=subprocess.STDOUT, start_new_session=True)
            running.append((p, i, v, time.monotonic(), log))
        time.sleep(0.2)
        for item in list(running):
            p, i, v, t0, log = item
            if p.poll() is None and time.monotonic() - t0 < timeout:
                continue
            if p.poll() is None:
                os.killpg(p.pid, signal.SIGKILL)
                p.wait()
            log.close()
            running.remove(item)
            res = OUT / "_runs" / v / UNIT / rows[i]["func"] / "result.json"
            if not res.exists():
                ex.save(res, json.dumps(dict(rows[i], status="timeout" if p.returncode < 0 else "worker-error",
                                             score=None, variant=v)) + "\n")
            got = json.loads(res.read_text())
            print(v, rows[i]["func"], got.get("status"), got.get("score"), flush=True)


# ------------------------------------------------------------------------------------------ final

def census_scores():
    out = {}
    for line in CENSUS.read_text().splitlines():
        r = json.loads(line)
        if r["scope"].startswith(UNIT + ":"):
            out[r["scope"].split(":")[1]] = dict(score=r["score"], path=r["selected_path"])
    if T16510.exists():
        for line in T16510.read_text().splitlines():
            r = json.loads(line)
            if r.get("unit") == UNIT and r.get("score") is not None and r["status"] in ("improved", "zero"):
                if r["score"] < (out[r["func"]]["score"] or BIG):
                    out[r["func"]] = dict(score=r["score"], path="drafts/T-16510/%s/%s" % (UNIT, r["func"]))
    return out


def finish(rows, only):
    files = draft_fixes.asm_files(".")
    census = census_scores()
    summary = []
    for row in rows:
        func = row["func"]
        if only and func not in only:
            continue
        runs = {}
        for v in ("plain", "typed"):
            folder = OUT / "_runs" / v / UNIT / func
            res = folder / "result.json"
            if res.exists():
                runs[v] = (json.loads(res.read_text()), folder)
        scored = {v: r for v, r in runs.items() if r[0].get("score") is not None}
        dest = OUT / UNIT / func
        dest.mkdir(parents=True, exist_ok=True)
        record = dict(scope=row["scope"], func=func, src=row["src"], size=row["size"], unit=UNIT,
                      guarded=row["guarded"], census_score=census[func]["score"], census_path=census[func]["path"],
                      plain_score=runs.get("plain", ({},))[0].get("score"),
                      typed_score=runs.get("typed", ({},))[0].get("score"),
                      rewrites=runs.get("typed", ({},))[0].get("rewrites", []),
                      tool="T-16560 drafts/T-16560/redraft.py", harness_hash=digest(Path(__file__).read_text()),
                      created=time.strftime("%Y-%m-%d", time.gmtime()))
        if not scored:
            record.update(status="failed", score=None,
                          reason=" | ".join(r[0].get("reason", "") for r in runs.values())[:300])
            src = runs.get("typed") or runs.get("plain")
            if src and (src[1] / "body.c").exists():
                shutil.copy(src[1] / "body.c", dest / "body.c")
        else:
            v = min(scored, key=lambda k: (scored[k][0]["score"], k != "typed"))
            got, folder = scored[v]
            body = (folder / "body.c").read_text()
            if (folder / "context.c").exists():
                context = (folder / "context.c").read_text()
            else:                                  # pruned run folder: rebuild the unit from src
                context = draft_fixes.fresh_context(dict(row), body, folder)
            record.update(variant=v, score=got["score"])
            if got["score"] > 0:
                scorer = draft_fixes.make_scorer(dict(row))
                base, score, chosen, new_body, new_context, tried = draft_fixes.best_rewrite(
                    row, body, draft_fixes.asm_for(row["scope"], files), context, scorer, OUT / "_work" / func)
                record.update(draft_fixes_baseline=base, draft_fixes=["%s#%d" % c for c in chosen],
                              draft_fixes_tried=tried)
                if score < got["score"]:
                    body, context = new_body, new_context
                    record["score"] = score
            for name in ("declarations.h",):
                if (folder / name).exists():
                    shutil.copy(folder / name, dest / name)
            (dest / "body.c").write_text(body)
            (dest / "context.c").write_text(context)
            record["status"] = "candidate-zero" if record["score"] == 0 else "scored"
            record["draft_hash"], record["context_hash"] = digest(body), digest(context)
        c = record["census_score"]
        s = record["score"]
        record["vs_census"] = None if s is None or c is None else ("better" if s < c else "same" if s == c else "worse")
        (dest / "result.json").write_text(json.dumps(record, sort_keys=True) + "\n")
        summary.append(record)
    merged = {}
    if (OUT / "catalog.jsonl").exists():
        for line in (OUT / "catalog.jsonl").read_text().splitlines():
            r = json.loads(line)
            merged[r["func"]] = r
    for r in summary:
        merged[r["func"]] = r
    with open(OUT / "catalog.jsonl", "w") as fh:
        for row in rows:
            if row["func"] in merged:
                fh.write(json.dumps(merged[row["func"]], sort_keys=True) + "\n")
    shutil.rmtree(OUT / "_work", ignore_errors=True)
    return summary


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--worker", type=int)
    p.add_argument("--variant", choices=("plain", "typed"))
    p.add_argument("--jobs", type=int, default=2)
    p.add_argument("--timeout", type=int, default=120)
    p.add_argument("--only", nargs="*")
    p.add_argument("--finish-only", action="store_true")
    p.add_argument("--rewrite", help="print the typed rewrite of one body file and exit")
    p.add_argument("--scope", help="UNIT:func for --rewrite (asm-based rewrites)")
    a = p.parse_args()
    if a.rewrite:
        log = []
        asm = draft_fixes.asm_for(a.scope, draft_fixes.asm_files(".")) if a.scope else None
        body, _ = typed_rewrites(Path(a.rewrite).read_text(), {}, log, asm)
        sys.stdout.write(body)
        print(log, file=sys.stderr)
        return
    if a.worker is not None:
        worker(a.worker, a.variant)
        return
    OUT.mkdir(parents=True, exist_ok=True)
    if not (OUT / "inventory.json").exists():
        ex.save(OUT / "inventory.json", json.dumps(inventory(), indent=1) + "\n")
    if not (OUT / "environment.json").exists():
        env = dict(base=subprocess.run(["git", "rev-parse", "HEAD"], capture_output=True, text=True).stdout.strip(),
                   files={h: digest(mg.read(h)) for h in ("include/ovl/TACO.h", "include/main_api.h",
                                                          "include/game.h", "include/libgte.h")})
        ex.save(OUT / "environment.json", json.dumps(env, indent=1, sort_keys=True) + "\n")
    rows = json.loads((OUT / "inventory.json").read_text())
    for v in ("plain", "typed"):
        (OUT / "_runs" / v).mkdir(parents=True, exist_ok=True)
        shutil.copy(OUT / "environment.json", OUT / "_runs" / v / "environment.json")
    if not a.finish_only:
        run_all(rows, ("plain", "typed"), a.jobs, a.timeout, a.only)
    summary = finish(rows, a.only)
    print("DONE", len(summary), sum(r["status"] == "candidate-zero" for r in summary), "zero", flush=True)


if __name__ == "__main__":
    main()
