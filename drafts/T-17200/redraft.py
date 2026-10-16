#!/usr/bin/env python3
"""T-17200: redraft every unmatched DATE function with the recovered types and score it like T-14500.

Adapted from drafts/T-16560/redraft.py (plain/typed runs, tools/draft_fixes.py, finish) and
drafts/T-16550/redraft.py (fold, stack-table copies). Run inside the project Docker image from the
repo root:
    python3 drafts/T-17200/redraft.py [--jobs 2] [--timeout 150] [--only func_X ...] [--finish-only]
    python3 drafts/T-17200/redraft.py --rewrite BODY.c      # print the typed rewrites of one body

Two drafts per function, each through the census path of tools/exhaustive_drafts.py (m2c with the
default context, which now holds the T-17200 types; the bulk_m2c cleanup; full-translation-unit
compile with the cheap declaration fixes; order_search's resolved diff as the score):
  plain  the m2c draft as the census made it, only the headers differ;
  typed  m2c on folded asm plus the rewrites below.
The better of the two then goes through tools/draft_fixes.py (as T-16560). The best body, its full
unit and a result.json with every score and the census score land in drafts/T-17200/DATE/<func>/.

Typed rewrites (each is a candidate; the compiler decides):
  fold          (T-16550) a splat label inside a typed DATE aggregate, GameState D_800E6280 or the
                D_8011ECD0 display records becomes `base + offset` in the asm handed to m2c, so m2c
                prints fields (`D_800E6280.unk_110A`, not `D_800E738A`); m2c sees D_8011ECD0 as
                DisplayActor44[].
  stack-table   m2c's word-by-word or M2C_MEMCPY copy of a header table to a stack local is one
                struct assignment (`spXX = D_YYY;`, spXX of the table's type); the element reads
                m2c prints relative to `sp` become `spXX.f[i]` / `spXX.v[i]`.
  fptab         (T-3330 idiom) a step-table local gets `s32 idx;` declared before it and the call
                through it reads `idx = E; spXX.f[idx](...)`.
  record-temp   a local set to `&D_801217D0[i]` (Rec24) or a D_8011ECD0 record (DisplayActor44)
                takes that pointer type; its byte offsets become `p[k].field`.
  actor-offset, pointer-step, field-symbols  (T-16560) on the DATE header's typed globals.
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
import m2c  # noqa: E402
import migrate_globals as mg  # noqa: E402

OUT = Path("drafts/T-17200")
UNIT = "DATE"
HEADER = "include/ovl/DATE.h"
CENSUS = Path("drafts/T-14500/best-quality-catalog.jsonl")
T16510 = Path("drafts/T-16510/results.jsonl")
BIG = 10 ** 6
DISPLAY = (0x8011ECD0, 0x44 * 160)


def digest(text):
    return hashlib.sha256(text.encode()).hexdigest()


# ------------------------------------------------------------------------------------------ types

def typedef_block(text, name):
    m = re.search(r"typedef struct(?: \w+)? \{.*?\} %s;" % name, text, re.S)
    return m.group(0) if m else ""


def header_types():
    """name -> mg.Type for the DATE header's typedefs plus Rec24 and DisplayActor44."""
    text = mg.read(HEADER)
    game = mg.read("include/game.h") + "\n" + mg.read("include/main_api.h")
    text += "\n" + typedef_block(game, "Rec24") + "\n" + typedef_block(game, "DisplayActor44") + "\n"
    text = re.sub(r"\bstruct\s+(\w+)\s*\*", r"\1 *", mg.strip_comments_keep_offsets(text))
    # the layout parser does not read function-pointer arrays: model them as pointer arrays
    text = re.sub(r"void \(\*(\w+)\[(\d+)\]\)\(\);", r"void *\1[\2];", text)
    return {k: v for k, v in mg.parse_types(text).items() if not isinstance(v, mg.LayoutError)}


TYPES = header_types()
DECL_RE = re.compile(r"^\s*extern\s+(\w+)\s*(\*?)\s*(D_[0-9A-F]{8})\s*(\[(\w*)\])?\s*;", re.M)
TABLE_RE = re.compile(r"typedef\s+struct\s+\w*\s*\{\s*(?:/\*[^*]*\*/\s*)?(\w+|void \(\*)\s*(\w+)((?:\[\w+\])+)\)?(?:\(\))?;\s*\}\s*(\w+)\s*;")


def typed_globals():
    """SYM -> (struct Type, 'ptr' | 'array' | 'obj') for the DATE header's struct globals."""
    out = {}
    for m in DECL_RE.finditer(mg.read(HEADER)):
        tname, star, sym, arr = m.group(1), m.group(2), m.group(3), m.group(4)
        t = TYPES.get(tname)
        if t is None or t.kind != "struct":
            continue
        out[sym] = (t, "ptr" if star else "array" if arr else "obj")
    return out


GLOBALS = typed_globals()
ESIZE = {"u8": 1, "s8": 1, "u16": 2, "s16": 2, "u32": 4, "s32": 4, "void (*": 4}


def table_types():
    """sym -> (type name, field, dims, element size) for globals whose type is a one-array struct."""
    text = mg.read(HEADER)
    field = {}
    for m in TABLE_RE.finditer(text):
        dims = [int(d, 0) for d in re.findall(r"\[(\w+)\]", m.group(3))]
        if m.group(1) in ESIZE:
            field[m.group(4)] = (m.group(2), dims, ESIZE[m.group(1)])
    out = {}
    for m in DECL_RE.finditer(text):
        tname, star, sym, arr = m.group(1), m.group(2), m.group(3), m.group(4)
        if tname in field and not star and not arr:
            out[sym] = (tname,) + field[tname]
    return out


TABLES = table_types()


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


# ------------------------------------------------------------------------------------------ fold (T-16550)

def aggregate_ranges():
    """(start, end, base) for each typed DATE aggregate, GameState and the display records."""
    text = mg.read(HEADER)
    sizes = dict((n, int(v, 16)) for n, v in re.findall(r"\}\s*(\w+)\s*;\s*/\*\s*size\s+(0x[0-9A-Fa-f]+)", text))
    out = []
    for m in re.finditer(r"^\s*extern\s+(?:volatile\s+)?(\w+)\s+(D_([0-9A-F]{8}))\s*(?:\[(0x[0-9A-Fa-f]+|\d+)\])?\s*;",
                         text, re.M):
        tname, base, addr, count = m.groups()
        size = sizes.get(tname)
        if size is None:
            if tname in TYPES:
                size = TYPES[tname].size
            else:
                try:
                    size = mg.base_type(tname, TYPES).size
                except (mg.LayoutError, KeyError):
                    continue
        size *= int(count, 0) if count else 1
        if size > 4 or count:
            out.append((int(addr, 16), int(addr, 16) + size, base))
    out.append((0x800E6280, 0x800E6280 + 0x1A90, "D_800E6280"))
    out.append((DISPLAY[0], DISPLAY[0] + DISPLAY[1], "D_8011ECD0"))
    return sorted(out)


RANGES = aggregate_ranges()
SYM_RE = re.compile(r"%(hi|lo)\(\s*(D_([0-9A-F]{8}))\s*(?:([+-])\s*(0x[0-9A-Fa-f]+|\d+)\s*)?\)")


def fold(text):
    def rep(m):
        addr = int(m.group(3), 16)
        if m.group(4):
            addr += int(m.group(5), 0) * (-1 if m.group(4) == "-" else 1)
        for start, end, base in RANGES:
            if start <= addr < end and m.group(2) != base:
                off = addr - start
                # m2c reads a lui %hi(sym+k) as the whole address sym+k for every %lo paired with it
                if m.group(1) == "hi" and (start + 0x8000) >> 16 == (addr + 0x8000) >> 16:
                    off = 0
                return "%%%s(%s+0x%X)" % (m.group(1), base, off) if off else "%%%s(%s)" % (m.group(1), base)
        return m.group(0)
    return SYM_RE.sub(rep, text)


def display_records(body):
    """m2c prints D_8011ECD0 arithmetic in bytes; write records of the DisplayActor44 view."""
    body = re.sub(r"\bD_8011ECD0 \+ \(([^()]+) \* 0x44\)", r"&((DisplayActor44 *)D_8011ECD0)[\1]", body)
    # a byte offset computed into a temporary first (`temp_lo = i * 0x44; p = D_8011ECD0 + temp_lo;`)
    for name in re.findall(r"^\s+(\w+) = [^;]+ \* 0x44;$", body, re.M):
        body = re.sub(r"\bD_8011ECD0 \+ %s\b" % name, "(DisplayActor44 *)((u8 *)@@DISP@@ + %s)" % name, body)

    def disp(m):
        n = int(m.group(1), 0)
        return "@@REC%d@@" % (n // 0x44) if n % 0x44 == 0 else "@@BYTE%d@@" % n
    body = re.sub(r"\bD_8011ECD0 \+ (0x[0-9A-Fa-f]+|\d+)\b", disp, body)
    body = re.sub(r"(?<!\*)\bD_8011ECD0\b(?!\)\[)", "((DisplayActor44 *)D_8011ECD0)", body)
    body = re.sub(r"@@REC(\d+)@@", r"&((DisplayActor44 *)D_8011ECD0)[\1]", body)
    body = body.replace("@@DISP@@", "D_8011ECD0")
    body = re.sub(r"@@BYTE(\d+)@@", lambda m: "(DisplayActor44 *)(D_8011ECD0 + 0x%X)" % int(m.group(1)), body)
    return body


# ------------------------------------------------------------------------------------------ stack tables

CAST = r"\*\((?:[^()]|\([^()]*\))*\)"


def stack_tables(body):
    """Copies of header tables into stack locals become one struct assignment; element reads too."""
    locals_ = {}
    for sym, (tname, fld, dims, esize) in TABLES.items():
        lines = body.split("\n")
        hits = []
        for i, line in enumerate(lines):
            if re.search(r"\b%s\b" % sym, line) and ("=" in line or "M2C_MEMCPY" in line):
                m = re.search(r"\b(sp[0-9A-F]+)\b", line)
                if m:
                    hits.append((i, m.group(1)))
        if not hits:
            continue
        sp = hits[0][1]
        drop = {i for i, s in hits if s == sp}
        indent = re.match(r"\s*", lines[hits[0][0]]).group(0)
        out = []
        for i, line in enumerate(lines):
            if i == hits[0][0]:
                out.append("%s%s = %s;" % (indent, sp, sym))
            elif i not in drop:
                out.append(line)
        body = "\n".join(out)
        body, n = re.subn(r"^(\s+)(?:\?|\w+) \*?%s(?:\[\d+\])*;$" % sp, r"\g<1>%s %s;" % (tname, sp), body,
                          count=1, flags=re.M)
        if not n:  # m2c printed no declaration (the bulk cleanup would synthesize an s32 array)
            body = re.sub(r"^(.*\)\s*\{\n)", r"\g<1>    %s %s;\n" % (tname, sp), body, count=1)
        locals_[sp] = (tname, fld, dims, esize)
    for sp, (tname, fld, dims, esize) in locals_.items():
        base = int(sp[2:], 16)
        rows = [1] * len(dims)
        for i in range(len(dims) - 2, -1, -1):
            rows[i] = rows[i + 1] * dims[i + 1]
        row = rows[0] * esize
        total = row * dims[0]

        def addr_elem(expr, k, off):
            rel = off - base
            if not (0 <= rel < total) or k != row:
                return None
            first, r = divmod(rel, row)
            rest = []
            for s in rows[1:]:
                rest.append(r // (s * esize))
                r %= s * esize
            if r:
                return None
            head = "%s + %d" % (expr, first) if first else expr
            return "%s.%s[%s]%s" % (sp, fld, head, "".join("[%d]" % i for i in rest))

        def read(m):
            got = addr_elem(m.group(1), int(m.group(2), 0), int(m.group(3), 0))
            return got if got else m.group(0)
        body = re.sub(CAST + r"\(\(unsigned char \*\)\(sp \+ \(([^()]+) \* (\d+)\)\) \+ \((\d+)\)\)", read, body)
        if len(dims) == 2 and esize == 1:
            body = re.sub(r"\*\(&%s \+ \(([^()]+) \* (0x[0-9A-Fa-f]+|\d+)\) \+ ([^()]+?)\)" % sp,
                          lambda m: "%s.%s[%s][%s]" % (sp, fld, m.group(1), m.group(3))
                          if int(m.group(2), 0) == dims[1] else m.group(0), body)
        body = re.sub(r"\(&%s(?:\[0\])?\)\[" % sp, "%s.%s[" % (sp, fld), body)
        body = re.sub(r"(?<![.\w&])%s\[" % sp, "%s.%s[" % (sp, fld), body)
        body = re.sub(r"&%s\b(?![.\[])" % sp, "%s.%s" % (sp, fld), body)
    return body, locals_


def fptab(body, locals_):
    """T-3330: one scalar declared above the step table; the call indexes it through `idx`."""
    for sp, (tname, fld, dims, esize) in locals_.items():
        if not tname.startswith("FnTbl") and not tname.startswith("DateDispatch"):
            continue
        call = re.compile(r"^(\s+)\(?%s\.%s\[([^\[\]]+)\]\)?\((.*)\);$" % (sp, fld), re.M)
        if not call.search(body) or re.search(r"\bidx\b", body):
            continue
        body = call.sub(lambda m: "%sidx = %s;\n%s%s.%s[idx](%s);" % (m.group(1), m.group(2), m.group(1), sp, fld,
                                                                    m.group(3)), body)
        body = re.sub(r"^(\s+)%s %s;$" % (tname, sp), r"\1s32 idx; /* declared before the table: T-3330 */\n\1%s %s;"
                      % (tname, sp), body, count=1, flags=re.M)
    return body


# ------------------------------------------------------------------------------------------ record pointers

RECORDS = {"D_801217D0": "Rec24", "D_8011ECD0": "DisplayActor44"}
OFF_PTR_RE = r"\*\((u8|s8|u16|s16|u32|s32) \*\)\(\(unsigned char \*\)%s \+ \((\d+)\)\)"


DISP_OFF_RE = re.compile(r"\*\((u8|s8|u16|s16|u32|s32) \*\)\(\(unsigned char \*\)\(\(DisplayActor44 \*\)D_8011ECD0\) \+ \((\d+)\)\)")


def display_offsets(body):
    """`*(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6535))` is record 96's field."""
    t = TYPES.get("DisplayActor44")

    def repl(m):
        ctype, n = m.group(1), int(m.group(2))
        k, r = divmod(n, t.size)
        path, lt = leaf_at(t, r, SIZES[ctype])
        if path is None:
            return m.group(0)
        expr = "((DisplayActor44 *)D_8011ECD0)[%d]%s" % (k, path)
        store = re.match(r"\s*=(?!=)", m.string[m.end():]) is not None
        if lt.name == ctype or (store and lt.kind == "scalar" and lt.size == SIZES[ctype]):
            return expr
        return "*(%s *)&%s" % (ctype, expr)
    return DISP_OFF_RE.sub(repl, body) if t is not None else body


def record_temps(body):
    body = display_offsets(body)
    for sym, tname in RECORDS.items():
        t = TYPES.get(tname)
        if t is None:
            continue
        # p = D_801217D0 + temp_lo; with temp_lo = i * 0x24
        for var, lo in re.findall(r"^\s+(\w+) = %s \+ (\w+);$" % sym, body, re.M):
            m = re.search(r"^\s+%s = (.+) \* 0x%X;$" % (lo, t.size), body, re.M)
            use = re.search(r"^\s+%s = %s \+ %s;$" % (var, sym, lo), body, re.M)
            # the index must not change between the product and its use (`lo = i * 0x24; i += 1;`)
            if m and use and re.fullmatch(r"\w+", m.group(1)) and re.search(
                    r"\b%s\s*(?:[-+*/|&]?=(?!=)|\+\+|--)" % m.group(1), body[m.end():use.start()]):
                body = re.sub(r"^(\s+)%s = %s \+ %s;$" % (var, sym, lo),
                              r"\1%s = (%s *)((u8 *)%s + %s);" % (var, tname, sym, lo), body, flags=re.M)
                continue
            if m:
                body = re.sub(r"^(\s+)%s = %s \+ %s;$" % (var, sym, lo), r"\1%s = &%s[%s];" % (var, sym, m.group(1)),
                              body, flags=re.M)
        view = "((DisplayActor44 *)D_8011ECD0)" if sym == "D_8011ECD0" else sym
        byte_form = "(%s *)((u8 *)%s + " % (tname, sym)
        ptrs = set(re.findall(r"^\s+(\w+) = &%s\[" % re.escape(view), body, re.M))
        ptrs |= set(re.findall(r"^\s+(\w+) = %s" % re.escape(byte_form), body, re.M))
        for var in ptrs:
            assigns = re.findall(r"^\s+%s = (.+);$" % var, body, re.M)
            if any(not (a.startswith("&" + view + "[") or a.startswith(byte_form)) for a in assigns):
                continue

            def repl(m):
                ctype, n = m.group(1), int(m.group(2))
                k, r = divmod(n, t.size)
                path, lt = leaf_at(t, r, SIZES[ctype])
                store = re.match(r"\s*=(?!=)", m.string[m.end():]) is not None
                if path is None:
                    return "*(%s *)((u8 *)&%s[%d] + %d)" % (ctype, var, k, r) if k else m.group(0)
                expr = "%s[%d]%s" % (var, k, path) if k else "%s->%s" % (var, path[1:])
                if lt.name == ctype or (store and lt.kind == "scalar" and lt.size == SIZES[ctype]):
                    return expr
                return "*(%s *)&%s" % (ctype, expr)
            new = re.sub(OFF_PTR_RE % re.escape(var), repl, body)
            if new != body:
                body = re.sub(r"^(\s+)(?:\?|\w+) \*?%s;$" % var, r"\1%s *%s;" % (tname, var), new, count=1, flags=re.M)
    return body


OFF_RE = re.compile(r"\*\((u8|s8|u16|s16|u32|s32) \*\)\(\(unsigned char \*\)\(?(&?)(D_[0-9A-F]{8})(?:\[([^\[\]]+)\])?\)? \+ \((\d+)\)\)")


def actor_offsets(body):
    def elem(sym, idx, k):
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
        if idx is not None:
            expr = elem(sym, idx, k) + path
        elif amp and kind == "obj":
            if k:
                return m.group(0)
            expr = sym + path
        elif amp and kind == "ptr":
            return m.group(0)
        else:
            expr = elem(sym, None, k) + path
        if kind == "ptr" and k == 0 and idx is None:
            expr = sym + "->" + path[1:]
        store = re.match(r"\s*=(?!=)", m.string[m.end():]) is not None
        if lt.name == ctype or (store and lt.kind == "scalar" and lt.size == SIZES[ctype]):
            return expr
        return "*(%s *)&%s" % (ctype, expr)
    return OFF_RE.sub(repl, body)


LOCAL_PTR_RE = re.compile(r"^\s*(\w+) (\*+)(\w+);\s*$", re.M)
SCALAR_SIZES = {"s16": 2, "u16": 2, "s32": 4, "u32": 4}


def pointer_steps(body):
    """m2c prints pointer steps in bytes (`Rec24 *p; p += 0x24`); C counts elements."""
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
    for sym, (t, kind) in GLOBALS.items():
        if kind == "obj":
            aggs.append(TypedAggregate(sym, t, HEADER))
    return aggs


AGGS = aggregates()


def field_symbols(body, known):
    """(body, known) with splat names inside an aggregate written as fields; their declarations dropped."""
    views = {}
    for d in known.values():
        views.update(mg.declarations(d.rstrip() + "\n"))
    hdr = mg.declarations(mg.read(HEADER)) | mg.declarations(mg.read("include/main_api.h"))
    try:
        new, _problems = mg.rewrite_text(body, AGGS, set(), None, lambda s: views.get(s) or hdr.get(s))
    except Exception:  # a layout the rewriter cannot read: keep the body
        return body, known
    used = set(re.findall(r"\bD_[0-9A-F]{8}\b", new))
    known = {k: v for k, v in known.items() if not (k.startswith("D_") and k not in used)}
    return new, known


def pre_rewrites(body, log=None):
    """Rewrites on m2c's raw function text (before the bulk cleanup)."""
    new = display_records(body)
    if new != body and log is not None:
        log.append("display-records")
    body = new
    new, locals_ = stack_tables(body)
    if new != body and log is not None:
        log.append("stack-table")
    body = new
    new = fptab(body, locals_)
    if new != body and log is not None:
        log.append("fptab")
    return new


def typed_rewrites(body, known, log=None):
    for name, fn in (("record-temp", record_temps), ("actor-offset", actor_offsets), ("pointer-step", pointer_steps)):
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
    return [r for r in ex.census() if r["unit"] == UNIT]


def install_typed(applied):
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
        return decls, pre_rewrites(body, applied)
    m2c.split_draft = split_draft
    plain_cleanup = bulk.cleanup

    def cleanup(decls, body, glob=None):
        body, known, problem = plain_cleanup(decls, body, glob)
        if problem:
            return body, known, problem
        body, known = typed_rewrites(body, known, applied)
        return body, known, None
    bulk.cleanup = cleanup


def worker(index, variant):
    rows = json.loads((OUT / "inventory.json").read_text())
    row = rows[index]
    ex.BASE = OUT / "_runs" / variant
    os.environ["TOKIMEMO_REGENERATE_GUARDS"] = "1"
    applied = []
    if variant == "typed":
        install_typed(applied)
    got = ex.process(row)
    if variant == "typed":
        got["rewrites"] = sorted(set(applied))
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
                if r["score"] < (out.get(r["func"], {}).get("score") or BIG):
                    out[r["func"]] = dict(score=r["score"], path="drafts/T-16510/%s/%s" % (UNIT, r["func"]))
    return out


def finish(rows, only, fixes=True):
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
        c = census.get(func, {})
        record = dict(scope=row["scope"], func=func, src=row["src"], size=row["size"], unit=UNIT,
                      guarded=row["guarded"], census_score=c.get("score"), census_path=c.get("path"),
                      plain_score=runs.get("plain", ({},))[0].get("score"),
                      typed_score=runs.get("typed", ({},))[0].get("score"),
                      rewrites=runs.get("typed", ({},))[0].get("rewrites", []),
                      tool="T-17200 drafts/T-17200/redraft.py", harness_hash=digest(Path(__file__).read_text()),
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
            else:
                context = draft_fixes.fresh_context(dict(row), body, folder)
            record.update(variant=v, score=got["score"])
            if got["score"] > 0 and fixes:
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
        s, cs = record["score"], record["census_score"]
        record["vs_census"] = None if s is None or cs is None else ("better" if s < cs else "same" if s == cs else "worse")
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
    p.add_argument("--timeout", type=int, default=150)
    p.add_argument("--only", nargs="*")
    p.add_argument("--finish-only", action="store_true")
    p.add_argument("--no-fixes", action="store_true", help="finish without the draft_fixes search")
    p.add_argument("--rewrite", help="print the typed rewrites of one body file and exit")
    a = p.parse_args()
    if a.rewrite:
        log = []
        body = pre_rewrites(Path(a.rewrite).read_text(), log)
        body, _ = typed_rewrites(body, {}, log)
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
                   files={h: digest(mg.read(h)) for h in (HEADER, "include/main_api.h", "include/game.h")})
        ex.save(OUT / "environment.json", json.dumps(env, indent=1, sort_keys=True) + "\n")
    rows = json.loads((OUT / "inventory.json").read_text())
    for v in ("plain", "typed"):
        (OUT / "_runs" / v).mkdir(parents=True, exist_ok=True)
        shutil.copy(OUT / "environment.json", OUT / "_runs" / v / "environment.json")
    if not a.finish_only:
        run_all(rows, ("plain", "typed"), a.jobs, a.timeout, a.only)
    summary = finish(rows, a.only, not a.no_fixes)
    print("DONE", len(summary), sum(r["status"] == "candidate-zero" for r in summary), "zero", flush=True)


if __name__ == "__main__":
    main()
