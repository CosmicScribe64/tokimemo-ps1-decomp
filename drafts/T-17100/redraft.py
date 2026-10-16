#!/usr/bin/env python3
"""T-17100: redraft every unmatched RPG_BAT function with the recovered types and score it like T-14500.

Adapted from drafts/T-16560/redraft.py (runner, rewrites, draft_fixes step) and
drafts/T-16550/redraft.py (asm fold, display-record view, stack-table copies). Run inside the
project Docker image from the repo root:
    python3 drafts/T-17100/redraft.py [--jobs 2] [--timeout 150] [--only func_X ...]

Two drafts per function, each through the census path of tools/exhaustive_drafts.py (m2c with the
default context, which now holds the T-17100 types; the bulk_m2c cleanup; full-translation-unit
compile with the cheap declaration fixes; order_search's resolved diff as the score):
  plain  the m2c draft as the census made it, only the headers differ;
  typed  m2c on folded asm, then the type-driven rewrites below.
The better of the two then goes through tools/draft_fixes.py (as `draft_fixes.py rescore` does).
The best body, its full unit, its declarations and a result.json with every score and the census
score land in drafts/T-17100/RPG_BAT/<func>/.

Type-driven rewrites of the typed variant (each is a candidate; the compiler decides):
  fold            (asm given to m2c) a splat label inside GameState D_800E6280, inside the
                  D_8011ECD0 display records, or on a named field of a typed RPG_BAT aggregate
                  becomes `base + offset`; m2c sees D_8011ECD0 as DisplayActor44[] and the draft
                  writes ((DisplayActor44 *)D_8011ECD0).
  stack-copy      the lw/sw run that copies a constant table (RpgDirTable, RpgStrTbl4, ...) to the
                  stack: one struct assignment, the local typed as the table.
  actor-offset    `*(T *)((unsigned char *)SYM + (N))` on a typed RPG_BAT global: the field.
  pointer-step    `p += 0x44` on a local `DisplayActor44 *p` (or `s32 *p; p += 8`): element steps.
  field-symbols   splat names inside a declared aggregate (RPG_BAT structs, GameState fields): the
                  field access, as tools/migrate_globals.py writes it.
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
import m2c  # noqa: E402

OUT = Path("drafts/T-17100")
UNIT = "RPG_BAT"
VARIANTS = ("plain", "typed", "census")
HEADER = "include/ovl/RPG_BAT.h"
DISPLAY = (0x8011ECD0, 0x44 * 160)
CENSUS = Path("drafts/T-14500/best-quality-catalog.jsonl")
T16510 = Path("drafts/T-16510/results.jsonl")
BIG = 10 ** 6


def digest(text):
    return hashlib.sha256(text.encode()).hexdigest()


# ------------------------------------------------------------------------------------------ types

def header_types():
    """name -> mg.Type for the typedefs of game.h and the RPG_BAT header."""
    text = ""
    for h in ("include/game.h", HEADER):
        text += mg.read(h) + "\n"
    text = re.sub(r"\bstruct\s+(\w+)\s*\*", r"\1 *", mg.strip_comments_keep_offsets(text))
    return {k: v for k, v in mg.parse_types(text).items() if not isinstance(v, mg.LayoutError)}


TYPES = header_types()
DECL_RE = re.compile(r"^\s*extern\s+(\w+)\s*(\*?)\s*(D_[0-9A-F]{8})\s*(\[(\w*)\])?\s*;", re.M)


def typed_globals():
    """SYM -> (struct Type, 'ptr' | 'array' | 'obj') for the RPG_BAT header's struct globals."""
    out = {}
    for m in DECL_RE.finditer(mg.read(HEADER)):
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
    """m2c prints pointer steps in bytes (`DisplayActor44 *p; p += 0x44`, `s32 *q; q += 8`); C counts elements."""
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


class TypedAggregate(mg.Aggregate):
    """An aggregate built from a declaration instead of config/migrate_globals.txt."""

    def __init__(self, base, typ, header):
        self.base, self.tname, self.header = base, typ.spell(), header
        self.addr = int(base[2:], 16)
        self.types, self.type = TYPES, typ
        self.end = self.addr + typ.size


def aggregates():
    aggs, _keep = mg.load_config(".")
    for m in DECL_RE.finditer(mg.read(HEADER)):
        tname, star, sym, arr, n = m.group(1), m.group(2), m.group(3), m.group(4), m.group(5)
        t = TYPES.get(tname)
        if t is None or star or t.kind != "struct":
            continue
        if arr is not None:
            if not n:
                continue
            count = int(n, 0)
            t = mg.Type("array", size=t.size * count, align=t.align, elem=t, count=count)
        aggs.append(TypedAggregate(sym, t, HEADER))
    return aggs


AGGS = aggregates()


def field_symbols(body, known):
    """(body, known) with splat names inside an aggregate written as fields; their declarations dropped."""
    views = {}
    for d in known.values():
        views.update(mg.declarations(d.rstrip() + "\n"))
    hdr = mg.declarations(mg.read(HEADER)) | mg.declarations(mg.read("include/main_api.h"))
    new, _problems = mg.rewrite_text(body, AGGS, set(), None, lambda s: views.get(s) or hdr.get(s))
    used = set(re.findall(r"\bD_[0-9A-F]{8}\b", new))
    known = {k: v for k, v in known.items() if not (k.startswith("D_") and k not in used)}
    return new, known


# ------------------------------------------------------------------------------------------ fold (T-16550)

def named_offsets(t):
    """Offsets inside `t` where a named (non-pad) scalar, pointer or array element starts."""
    out = set()
    for off, size, path, lt in mg.walk_layout(t):
        if "pad" in path:
            continue
        if lt.kind == "array":
            elem = lt.elem
            while elem.kind == "array":
                elem = elem.elem
            out.update(range(off, off + size, elem.size))
        else:
            out.add(off)
    return out


def fold_ranges():
    """(start, end, base, offsets or None) for GameState, the display records and each typed RPG_BAT aggregate;
    offsets None: fold every label in the range."""
    out = [(0x800E6280, 0x800E6280 + 0x1A90, "D_800E6280", None),
           (DISPLAY[0], DISPLAY[0] + DISPLAY[1], "D_8011ECD0", None)]
    for agg in AGGS:
        if agg.header != HEADER:
            continue
        out.append((agg.addr, agg.end, agg.base, named_offsets(agg.type)))
    return sorted(out)


SYM_RE = re.compile(r"%(hi|lo)\(\s*(D_([0-9A-F]{8}))\s*(?:([+-])\s*(0x[0-9A-Fa-f]+|\d+)\s*)?\)")


def fold(text):
    ranges = fold_ranges()

    def rep(m):
        addr = int(m.group(3), 16)
        if m.group(4):
            addr += int(m.group(5), 0) * (-1 if m.group(4) == "-" else 1)
        for start, end, base, offs in ranges:
            if start <= addr < end and m.group(2) != base:
                off = addr - start
                if offs is not None and off not in offs:
                    return m.group(0)
                # m2c reads a lui %hi(sym+k) as the whole address sym+k for every %lo paired with it;
                # give the hi the bare base when its high half is the same, so each %lo keeps its own k
                if m.group(1) == "hi" and (start + 0x8000) >> 16 == (addr + 0x8000) >> 16:
                    off = 0
                return "%%%s(%s+0x%X)" % (m.group(1), base, off) if off else "%%%s(%s)" % (m.group(1), base)
        return m.group(0)
    return SYM_RE.sub(rep, text)


def display_view(body):
    """m2c saw D_8011ECD0 as DisplayActor44[]; the real header declares u8 D_8011ECD0[]."""
    def disp(m):
        n = int(m.group(1), 0)
        return "@@REC%d@@" % (n // 0x44) if n % 0x44 == 0 else "@@BYTE%d@@" % n
    body = re.sub(r"\bD_8011ECD0 \+ (0x[0-9A-Fa-f]+|\d+)\b", disp, body)
    body = re.sub(r"(?<![\w(])D_8011ECD0\b", "((DisplayActor44 *)D_8011ECD0)", body)
    body = re.sub(r"@@REC(\d+)@@", r"&((DisplayActor44 *)D_8011ECD0)[\1]", body)
    return re.sub(r"@@BYTE(\d+)@@", lambda m: "(DisplayActor44 *)(D_8011ECD0 + 0x%X)" % int(m.group(1)), body)


def install_fold():
    orig_hi = m2c.fix_shared_hi
    m2c.fix_shared_hi = lambda t: orig_hi(fold(t))
    orig_ctx = m2c.build_context

    def build_context(root, heads, out_path, *a, **k):
        orig_ctx(root, heads, out_path, *a, **k)
        s = open(out_path).read()
        s = re.sub(r"\bextern\s+u8\s+D_8011ECD0\s*\[\s*\]\s*;", "", s) + "\nextern DisplayActor44 D_8011ECD0[];\n"
        open(out_path, "w").write(s)
    m2c.build_context = build_context
    orig_split = m2c.split_draft

    def split_draft(text, name):
        decls, body = orig_split(text, name)
        return decls, display_view(body)
    m2c.split_draft = split_draft


# ------------------------------------------------------------------------------------------ stack tables

def table_types():
    """sym -> (type, array field) for header globals whose type is a struct of one array (stack tables)."""
    text = mg.strip_comments(mg.read(HEADER))
    field = {}
    for m in re.finditer(r"typedef\s+struct\s+\w*\s*\{\s*\w+\s*\**\s*(\w+)\[[^\]]+\];\s*\}\s*(\w+)\s*;", text):
        field[m.group(2)] = m.group(1)
    out = {}
    for m in DECL_RE.finditer(mg.read(HEADER)):
        if m.group(1) in field and not m.group(2) and m.group(4) is None:
            out[m.group(3)] = (m.group(1), field[m.group(1)])
    return out


TABLES = table_types()
WORD_SRC = r"(?:\(\w+\) )?(?:\*\(\w+ \*\)\(\(unsigned char \*\)\(&%s\) \+ \((\d+)\)\)|%s\.%s\[(\w+)\]|\(\*\(\w+ \*\)&%s\)|%s)"
WORD_DST = r"(?:\*\(\w+ \*\)\(\(unsigned char \*\)\(&%s(?:\[0\])?\) \+ \((\d+)\)\)|\(\*\(\w+ \*\)&%s\)|%s(?:\[(\d+)\])?|\*\(M2cBlock\w+ \*\)\(&%s(?:\[0\])?\))"


def stack_copies(body):
    """m2c prints the copy of a constant table to the stack as one assignment per word (or an M2cBlock
    copy plus a tail word): write it as `spXX = D_YYY;` with spXX of the table's type."""
    for sym, (tname, fld) in TABLES.items():
        if sym not in body:
            continue
        lines = body.split("\n")
        sps = set(re.findall(r"\b(sp[0-9A-F]+)\b[^;\n]*= [^;\n]*\b%s\b" % sym, body))
        for sp in sps:
            src = WORD_SRC % (sym, sym, fld, sym, sym)
            dst = WORD_DST % (sp, sp, sp, sp)
            pat = re.compile(r"^(\s*)%s = (?:\*\(M2cBlock\w+ \*\)\(&%s\)|%s);$" % (dst, sym, src))
            hits = [i for i, l in enumerate(lines) if pat.match(l)]
            if not hits:
                continue
            indent = pat.match(lines[hits[0]]).group(1)
            lines[hits[0]] = "%s%s = %s;" % (indent, sp, sym)
            for i in hits[1:]:
                lines[i] = None
            lines = [l for l in lines if l is not None]
            text = "\n".join(lines)
            text = re.sub(r"^(\s*)[\w ]+?\**\s*%s(?:\[\d*\])?;$" % sp, r"\g<1>%s %s;" % (tname, sp), text,
                          count=1, flags=re.M)
            text = re.sub(r"\(&%s(?:\[0\])?\)\[" % sp, "%s.%s[" % (sp, fld), text)
            text = re.sub(r"\*\(\w+ \*\)\(\(unsigned char \*\)\(?&%s(?:\[0\])?\)? \+ \((\d+)\)\)" % sp,
                          lambda m: "%s.%s[%d]" % (sp, fld, int(m.group(1)) // (2 if tname == "RpgHalfTbl12" else 4)),
                          text)
            text = re.sub(r"\(\*\(\w+ \*\)&%s\)" % sp, "%s.%s[0]" % (sp, fld), text)
            text = re.sub(r"&%s\[0\]" % sp, "%s.%s" % (sp, fld), text)
            text = re.sub(r"&%s\b(?![.\[])" % sp, "%s.%s" % (sp, fld), text)
            text = re.sub(r"(?<![.\w])%s\[" % sp, "%s.%s[" % (sp, fld), text)
            text = re.sub(r"(?<![.\w&])%s\b(?![.\w\[])(?! = %s;)" % (sp, sym), "%s.%s[0]" % (sp, fld), text)
            text = text.replace("%s %s.%s[0];" % (tname, sp, fld), "%s %s;" % (tname, sp))
            esize = 2 if tname == "RpgHalfTbl12" else 4
            home = int(sp[2:], 16)

            def stack_index(m, fld=fld, sp=sp, esize=esize):
                idx = m.group(1)
                k = re.fullmatch(r"\(?(.+?) \* %d\)?" % esize, idx)
                return "%s.%s[%s]" % (sp, fld, k.group(1) if k else "(%s) / %d" % (idx, esize))
            text = re.sub(r"\*\(\w+ \*\)\(\(unsigned char \*\)\(sp \+ \((.+?)\)\) \+ \(%d\)\)" % home,
                          stack_index, text)
            # locals m2c placed inside the table (menu_set(..., &sp3A, &sp40) for sp34): its elements
            tsize = TYPES[tname].size
            for other in set(re.findall(r"\bsp([0-9A-F]+)\b", text)):
                k = int(other, 16) - home
                if 0 < k < tsize and k % esize == 0:
                    o = "sp" + other
                    text = re.sub(r"^\s*[\w ]+?\**\s*%s(?:\[\d*\])?;\n" % o, "", text, flags=re.M)
                    text = re.sub(r"(?<![\w.])%s(?:\[0\])?\b" % o, "%s.%s[%d]" % (sp, fld, k // esize), text)
            body = text
            lines = body.split("\n")
    for blk in set(re.findall(r"\b(M2cBlock\w+);", body)):
        if len(re.findall(r"\b%s\b" % blk, body)) == 1:
            body = re.sub(r"^\s*typedef struct \{[^\n]*\} %s;[^\n]*\n" % blk, "", body, flags=re.M)
    return body


REC_RE = re.compile(r"\*\((u8|s8|u16|s16|u32|s32) \*\)\(\(unsigned char \*\)(\(\(DisplayActor44 \*\)D_8011ECD0\)|\w+) \+ \((\d+)\)\)")


def record_offsets(body):
    """`*(T *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (N))` and the same on a local `S *p` of a
    struct type: the element and field (m2c and the cleanup print byte offsets past the first element)."""
    ptrs = {name: TYPES[t] for t, stars, name in LOCAL_PTR_RE.findall(body)
            if stars == "*" and t in TYPES and TYPES[t].kind == "struct"}

    def repl(m):
        ctype, base, n = m.group(1), m.group(2), int(m.group(3))
        if base.startswith("(("):
            t, arr = TYPES["DisplayActor44"], True
        elif base in ptrs:
            t, arr = ptrs[base], False
        else:
            return m.group(0)
        k, r = divmod(n, t.size)
        path, lt = leaf_at(t, r, SIZES[ctype])
        if path is None or "pad" in path:
            return m.group(0)
        expr = ("%s[%d]" % (base, k) + path) if (arr or k) else base + "->" + path[1:]
        store = re.match(r"\s*=(?!=)", m.string[m.end():]) is not None
        if lt.name == ctype or (store and lt.kind == "scalar" and lt.size == SIZES[ctype]):
            return expr
        return "*(%s *)&%s" % (ctype, expr)
    return REC_RE.sub(repl, body)


def typed_rewrites(body, known, log=None, asm_text=None):
    steps = (("stack-copy", stack_copies), ("actor-offset", actor_offsets), ("record-offset", record_offsets),
             ("pointer-step", pointer_steps))
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
    if variant == "census":
        # the T-14500 census body (or a better T-16510 one) through the same cleanup, typed rewrites and score
        path = Path(census_scores()[row["func"]]["path"])

        declared = set(mg.declarations(mg.read(HEADER)))

        def run_census(root, name, *a, **k):
            decls = (path / "declarations.h").read_text() if (path / "declarations.h").exists() else ""
            # the census declared some table globals as s32; the header now has their real type
            decls = "\n".join(l for l in decls.splitlines()
                              if not (set(re.findall(r"\bD_[0-9A-F]{8}\b", l)) & declared))
            return decls + "\n" + (path / "body.c").read_text()
        m2c.run_m2c = run_census
    else:
        if variant == "typed":
            install_fold()
    if variant in ("typed", "census"):
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
    if variant in ("typed", "census"):
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
        for v in VARIANTS:
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
                      census_typed_score=runs.get("census", ({},))[0].get("score"),
                      rewrites=runs.get("typed", ({},))[0].get("rewrites", []),
                      census_rewrites=runs.get("census", ({},))[0].get("rewrites", []),
                      tool="T-17100 drafts/T-17100/redraft.py", harness_hash=digest(Path(__file__).read_text()),
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
    p.add_argument("--variant", choices=("plain", "typed", "census"))
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
                   files={h: digest(mg.read(h)) for h in (HEADER, "include/main_api.h",
                                                          "include/game.h")})
        ex.save(OUT / "environment.json", json.dumps(env, indent=1, sort_keys=True) + "\n")
    rows = json.loads((OUT / "inventory.json").read_text())
    for v in VARIANTS:
        (OUT / "_runs" / v).mkdir(parents=True, exist_ok=True)
        shutil.copy(OUT / "environment.json", OUT / "_runs" / v / "environment.json")
    if not a.finish_only:
        run_all(rows, VARIANTS, a.jobs, a.timeout, a.only)
    summary = finish(rows, a.only)
    print("DONE", len(summary), sum(r["status"] == "candidate-zero" for r in summary), "zero", flush=True)


if __name__ == "__main__":
    main()
