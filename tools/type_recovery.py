#!/usr/bin/env python3
"""Recover arrays and structs from the access patterns of the original asm (T-5000).

splat names every address the original code touches (`D_800B0A6E`, `D_800B0A72`, ...), so one
struct or array of the original source shows up as many separate globals. This tool reads the
generated asm of every function (matched and unmatched: `disassemble_all` keeps both, main exe and
the 26 overlays), follows the registers of each function in one linear pass, and collects five
kinds of evidence that several addresses belong to one object:

  idx     an indexed access: `lui at,%hi(D); addu at,at,t6; lw t7,%lo(D)(at)` where t6 is an index
          scaled by N (sll, multu by a constant, shift-and-add): an array with stride N. Symbols
          read with the same stride less than N bytes apart are fields of one element.
  ptr     an address in a register (`addiu a0,%lo(D)`) and accesses at immediate offsets from it
          (`lh t0,0x4(a0)`): the field at D+4 belongs to D. A pointer stepped by a constant
          (`addiu a0,a0,0x44`) is an array walk with that stride; a compare of the walking pointer
          with another address (`bne a0,a1` with a1 = `%lo(E)`) gives the end of the array.
  order   store to one global, then (same basic block, no call, nothing in between that could stop
          it) a load of a neighbouring global. The original compiler hoists loads above stores to
          other symbols (most functions show it), but keeps the order when both accesses go through
          one symbol, because it cannot tell that they do not alias. A load left after the store
          therefore suggests one object. This is the "load hoisted above an earlier store" family
          of wiki/matching-notes.md.
  at      stores to two addresses through one `lui $at`: the original's as1 shares `$at` only
          between accesses through one symbol (T-5020, measured on the whole tree), so this is
          strong evidence; IDO 5.3 re-emits `lui $at` even with one symbol.
  cooc    two neighbouring globals (at most 0x10 apart, nothing in between) that every function
          touching one also touches. Weak; reported only for globals no other evidence covers.

Indexed arrays come first (one proposal per stride and region, grown over the later elements
that a direct access of the same width hits at an indexed field); pointer walks with an end
next; pointer fields, ordered pairs and shared `$at` join the rest with union-find (per address
space: the main exe, or one overlay for its own data). Each cluster becomes a proposal: base
symbol, size, stride (arrays), field offsets with access width and signedness (lb/lh signed, lbu/lhu unsigned),
the splat symbols it absorbs, the evidence counts, and a confidence:

  high    an indexed array seen in two functions (or confirmed by a later element), a walk with
          its end seen twice, a shared `$at`, or pointer/ordered evidence from two functions;
  medium  one function's worth of that evidence, or a walked block of halfwords or words that
          holds globals of other widths (a cleared region, not one element type);
  low     co-access only.
A symbol can sit in more than one proposal (an outer array and a record view); the main-exe bss
game state gives many overlapping ones (wiki/data-types.md).

The proposals are hypotheses for a person writing C: a high proposal says "declare this as one
object and reach the fields through it", which keeps IDO from hoisting loads over stores the same
way the original compiler did not (wiki/data-types.md). The tool never edits sources.

Usage (inside Docker, after configure.py so asm/ exists):
  python3 tools/type_recovery.py                   ranked proposals (high and medium)
  python3 tools/type_recovery.py --all             include low proposals
  python3 tools/type_recovery.py --sym D_800B0A72  the proposal that holds this symbol (any unit)
  python3 tools/type_recovery.py --unit DATE       proposals of one address space (main or overlay)
  python3 tools/type_recovery.py --json OUT.json   all proposals as JSON
  python3 tools/type_recovery.py --fake-sites      check every "first symbol" index trick in src/
"""
import argparse
import glob
import json
import os
import re
import sys
from collections import defaultdict

INSN_RE = re.compile(r'^\s*/\*\s*[0-9A-F]+\s+([0-9A-F]+)\s+[0-9A-F]+\s*\*/\s+(\S+)\s*(.*?)\s*$')
LABEL_RE = re.compile(r'^\s*(?:jlabel\s+)?\.L\w+:?\s*$|^\s*jlabel\s')
DLABEL_RE = re.compile(r'^\s*(?:dlabel|glabel)\s+(\w+)')
DATA_ADDR_RE = re.compile(r'^\s*/\*\s*(?:[0-9A-F]+\s+)?([0-9A-F]{8})(?:\s+[0-9A-F]+)?\s*\*/')
SPACE_RE = re.compile(r'\.space\s+(0x[0-9A-Fa-f]+|\d+)')
HI_RE = re.compile(r'^%hi\((\w+)(?:\s*\+\s*(0x[0-9A-Fa-f]+|\d+))?\)$')
LO_MEM_RE = re.compile(r'^%lo\((\w+)(?:\s*\+\s*(0x[0-9A-Fa-f]+|\d+))?\)\((\$\w+)\)$')
LO_RE = re.compile(r'^%lo\((\w+)(?:\s*\+\s*(0x[0-9A-Fa-f]+|\d+))?\)$')
IMM_MEM_RE = re.compile(r'^(-?0x[0-9A-Fa-f]+|-?\d+)?\((\$\w+)\)$')
LUI_CONST_RE = re.compile(r'^\((0x[0-9A-Fa-f]+) >> 16\)$')
ORI_CONST_RE = re.compile(r'^\((0x[0-9A-Fa-f]+) & 0xFFFF\)$')
ASM_PATH_RE = re.compile(r'^asm/(?:ovl/(\w+)/)?(?:non)?matchings/')

LOADS = {"lb": (1, "s"), "lbu": (1, "u"), "lh": (2, "s"), "lhu": (2, "u"), "lw": (4, ""),
         "lwl": (4, ""), "lwr": (4, ""), "lwc2": (4, "")}
STORES = {"sb": 1, "sh": 2, "sw": 4, "swl": 4, "swr": 4, "swc2": 4}
NO_DEST = {"mult", "multu", "div", "divu", "mthi", "mtlo", "break", "syscall", "nop", "jr", "j",
           "b", "mtc0", "mtc2", "ctc2", "cop2"}
BRANCHES = {"b", "j", "jr", "beq", "bne", "beqz", "bnez", "bgez", "bgtz", "blez", "bltz",
            "bgezal", "bltzal", "beql", "bnel", "jal", "jalr"}
CALLER_SAVED = ["$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3",
                "$t4", "$t5", "$t6", "$t7", "$t8", "$t9", "$ra"]

MAIN_LO, MAIN_HI = 0x80041000, 0x8012B538   # main exe text..bss (CODING_STANDARDS 8a)
ORDER_MAX_DIST = 0x100   # an ordered pair farther apart is not used as an edge
COOC_MAX_GAP = 0x10
ELEMENT_GAP = 4          # stop extending an array after this many elements with no direct access
MAX_ELEMENTS = 256
RUN_GAP = 16             # indexed runs of one stride at most this many elements apart: one array
MAX_FIELD_OFFSET = 0x400  # a pointer offset beyond this is not taken as a field of the base
MAX_END_SPAN = 0x4000    # a loop end farther than this from the walked base is ignored
WEIGHTS = {"at": 3.0, "field": 1.5, "idx": 3.0, "ptr": 3.0, "walk": 3.0, "end": 3.0, "order": 2.0, "cooc": 0.5}


def to_int(s):
    return int(s, 0) if s else 0


def split_args(s):
    out, depth, cur = [], 0, ""
    for ch in s:
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += ch
    if cur.strip():
        out.append(cur.strip())
    return out


class Symbols:
    """Name -> (space, address) for one build: data labels of the main exe and of each overlay."""

    def __init__(self):
        self.defs = defaultdict(dict)     # space -> name -> addr
        self.spans = defaultdict(dict)    # space -> addr -> bytes up to the next label
        self.names = defaultdict(dict)    # space -> addr -> name

    def add_label(self, space, name, addr, span=None):
        self.defs[space][name] = addr
        self.names[space].setdefault(addr, name)
        if span is not None:
            self.spans[space][addr] = span

    def resolve(self, unit, name):
        """(space, addr) of a symbol referenced from `unit` ('main' or an overlay name)."""
        if unit != "main" and name in self.defs[unit]:
            return unit, self.defs[unit][name]
        if name in self.defs["main"]:
            return "main", self.defs["main"][name]
        m = re.match(r'^D_([0-9A-F]{8})$', name)
        if m:
            addr = int(m.group(1), 16)
            # a main-exe address that no main function names (only overlays use it)
            return ("main" if unit == "main" or MAIN_LO <= addr < MAIN_HI else "ext"), addr
        return None

    def name_at(self, space, addr):
        n = self.names[space].get(addr)
        return n if n else "D_%08X" % addr


def load_labels(paths_by_space):
    """Read dlabel/glabel names and their addresses from data asm files."""
    syms = Symbols()
    for space, paths in paths_by_space.items():
        for path in paths:
            with open(path, errors="replace") as f:
                lines = f.read().splitlines()
            labels = []
            for i, line in enumerate(lines):
                m = DLABEL_RE.match(line)
                if not m:
                    continue
                for nxt in lines[i + 1:i + 4]:
                    a = DATA_ADDR_RE.match(nxt)
                    if a:
                        sp = SPACE_RE.search(nxt)
                        labels.append((int(a.group(1), 16), m.group(1), to_int(sp.group(1)) if sp else None))
                        break
            labels.sort()
            for k, (addr, name, sp) in enumerate(labels):
                span = sp
                if span is None and k + 1 < len(labels):
                    span = labels[k + 1][0] - addr
                syms.add_label(space, name, addr, span)
    return syms


class Access:
    __slots__ = ("space", "addr", "width", "sign", "store", "kind", "stride", "func", "pos")

    def __init__(self, space, addr, width, sign, store, kind, stride, func, pos):
        self.space, self.addr, self.width, self.sign, self.store = space, addr, width, sign, store
        self.kind, self.stride, self.func, self.pos = kind, stride, func, pos


class FuncResult:
    def __init__(self, name):
        self.name = name
        self.accesses = []
        self.orders = []    # (space, store_addr, load_addr)
        self.ptrs = []      # (space, base, field)
        self.walks = []     # (space, base, stride)
        self.ends = []      # (space, base, end_addr)
        self.shared_at = []  # (space, addr_a, addr_b): stores through one `lui $at`


def analyze_function(name, lines, unit, syms):
    """One linear pass over a function's asm lines; returns a FuncResult."""
    res = FuncResult(name)
    regs = {}
    counter = [0]
    hilo = [None]
    last_store = [None]          # (space, addr) of the last direct store in this block
    pending_call = [None]        # 'call' or 'branch' after the delay slot

    def fresh(reg):
        counter[0] += 1
        return ("lin", (reg, counter[0]), 1)

    def get(reg):
        if reg == "$zero":
            return ("const", 0)
        v = regs.get(reg)
        if v is None:
            v = regs[reg] = fresh(reg)
        return v

    def setr(reg, val):
        if reg != "$zero":
            regs[reg] = val

    def ref(symname, add):
        r = syms.resolve(unit, symname)
        if r is None:
            return None
        return r[0], r[1] + add

    def record(space, addr, op, store, kind, stride, pos):
        if store:
            width, sign = STORES[op], ""
        else:
            width, sign = LOADS[op]
        res.accesses.append(Access(space, addr, width, sign, store, kind, stride, name, pos))

    pos = 0
    for line in lines:
        if LABEL_RE.match(line):
            last_store[0] = None
            continue
        m = INSN_RE.match(line)
        if not m:
            continue
        pos += 1
        op, args = m.group(2), split_args(m.group(3))
        delay_end = pending_call[0]
        pending_call[0] = None

        if op in LOADS or op in STORES:
            store = op in STORES
            rt, mem = args[0], args[1] if len(args) > 1 else ""
            lo = LO_MEM_RE.match(mem)
            target = None
            if lo:
                r = ref(lo.group(1), to_int(lo.group(2)))
                if r:
                    base = get(lo.group(3))
                    if base[0] == "hiidx" and base[1] == r[0]:
                        target = (r[0], r[1], "idx", base[3])
                    else:
                        target = (r[0], r[1], "direct", None)
                        # one `lui $at` serving stores to two addresses: one symbol (T-5020)
                        if store and lo.group(3) == "$at" and base[0] == "hi" and base[1] == r[0] \
                                and base[2] != r[1]:
                            res.shared_at.append((r[0], base[2], r[1]))
            else:
                im = IMM_MEM_RE.match(mem)
                if im:
                    off = to_int(im.group(1))
                    base = get(im.group(2))
                    if base[0] == "addr":
                        target = (base[1], base[2] + off, "ptr", None)
                        if off:
                            res.ptrs.append((base[1], base[2], base[2] + off))
                    elif base[0] == "idx":
                        target = (base[1], base[2] + off, "idx", base[3])
            if target:
                space, addr, kind, stride = target
                record(space, addr, op, store, kind, stride, pos)
                if store and kind in ("direct", "ptr"):
                    last_store[0] = (space, addr)
                elif not store:
                    if kind in ("direct", "ptr") and last_store[0] and last_store[0][0] == space \
                            and last_store[0][1] != addr:
                        res.orders.append((space, last_store[0][1], addr))
                    last_store[0] = None
            elif not store:
                last_store[0] = None
            if not store and op != "lwc2":
                setr(rt, fresh(rt))
        elif op == "lui":
            hi = HI_RE.match(args[1])
            c = LUI_CONST_RE.match(args[1])
            if hi:
                r = ref(hi.group(1), to_int(hi.group(2)))
                setr(args[0], ("hi", r[0], r[1]) if r else fresh(args[0]))
            elif c:
                setr(args[0], ("const", to_int(c.group(1)) & 0xFFFF0000))
            else:
                try:
                    setr(args[0], ("const", (to_int(args[1]) << 16) & 0xFFFFFFFF))
                except ValueError:
                    setr(args[0], fresh(args[0]))
        elif op == "addiu" and len(args) == 3:
            rd, rs, imm = args
            src = get(rs)
            lo = LO_RE.match(imm)
            if lo:
                r = ref(lo.group(1), to_int(lo.group(2)))
                if r and src[0] == "hi" and src[1] == r[0]:
                    setr(rd, ("addr", r[0], r[1]))
                elif r and src[0] == "hiidx" and src[1] == r[0]:
                    setr(rd, ("idx", r[0], r[1], src[3]))
                else:
                    setr(rd, fresh(rd))
            else:
                try:
                    v = to_int(imm)
                except ValueError:
                    v = None
                if v is None:
                    setr(rd, fresh(rd))
                elif src[0] == "const":
                    setr(rd, ("const", (src[1] + v) & 0xFFFFFFFF))
                elif src[0] in ("addr", "idx") and rd == rs and v:
                    res.walks.append((src[1], src[2], abs(v)))
                    setr(rd, ("idx", src[1], src[2], abs(v)))
                elif src[0] == "addr":
                    setr(rd, ("addr", src[1], src[2] + v))
                elif src[0] == "idx":
                    setr(rd, ("idx", src[1], src[2] + v, src[3]))
                else:
                    setr(rd, fresh(rd))
        elif op == "ori" and len(args) == 3:
            src = get(args[1])
            c = ORI_CONST_RE.match(args[2])
            v = to_int(c.group(1)) & 0xFFFF if c else None
            if v is None:
                try:
                    v = to_int(args[2])
                except ValueError:
                    v = None
            setr(args[0], ("const", src[1] | v) if src[0] == "const" and v is not None else fresh(args[0]))
        elif op in ("addu", "subu", "or", "move") and len(args) >= 2:
            rd = args[0]
            a = get(args[1])
            b = get(args[2]) if len(args) > 2 else ("const", 0)
            setr(rd, combine(op, a, b) or fresh(rd))
        elif op == "sll" and len(args) == 3:
            src = get(args[1])
            k = to_int(args[2])
            if src[0] == "lin":
                setr(args[0], ("lin", src[1], src[2] << k))
            elif src[0] == "const":
                setr(args[0], ("const", (src[1] << k) & 0xFFFFFFFF))
            else:
                setr(args[0], fresh(args[0]))
        elif op in ("mult", "multu") and len(args) == 2:
            a, b = get(args[0]), get(args[1])
            if a[0] == "const" and b[0] == "lin":
                a, b = b, a
            hilo[0] = ("lin", a[1], a[2] * b[1]) if a[0] == "lin" and b[0] == "const" else None
        elif op == "mflo":
            setr(args[0], hilo[0] or fresh(args[0]))
        elif op in ("bne", "beq", "sltu", "slt") and len(args) >= 2:
            ra, rb = (args[0], args[1]) if op in ("bne", "beq") else (args[1], args[2])
            a, b = get(ra), get(rb)
            for p, e in ((a, b), (b, a)):
                if p[0] in ("idx", "addr") and e[0] == "addr" and e[1] == p[1] and e[2] > p[2]:
                    res.ends.append((p[1], p[2], e[2]))
            if op in ("sltu", "slt"):
                setr(args[0], fresh(args[0]))
        elif op in ("jal", "jalr"):
            pass
        elif op not in NO_DEST and op not in BRANCHES and args and args[0].startswith("$"):
            setr(args[0], fresh(args[0]))

        if op in ("jal", "jalr"):
            pending_call[0] = "call"
        elif op in BRANCHES:
            pending_call[0] = "branch"
        if delay_end:
            last_store[0] = None
            if delay_end == "call":
                for r in CALLER_SAVED:
                    regs.pop(r, None)
    return res


def combine(op, a, b):
    if op in ("or", "move"):
        if b == ("const", 0):
            return a
        if a == ("const", 0):
            return b
        return None
    if op == "addu":
        for x, y in ((a, b), (b, a)):
            if x[0] == "hi" and y[0] in ("lin",):
                return ("hiidx", x[1], x[2], y[2])
            if x[0] == "hi" and y[0] == "const":
                return ("hi", x[1], x[2])
            if x[0] == "addr" and y[0] == "lin":
                return ("idx", x[1], x[2], y[2])
            if x[0] == "addr" and y[0] == "const":
                return ("addr", x[1], x[2] + y[1])
            if x[0] in ("idx", "hiidx") and y[0] in ("lin", "const"):
                return x
        if a[0] == "lin" and b[0] == "lin" and a[1] == b[1]:
            return ("lin", a[1], a[2] + b[2])
        if a[0] == "const" and b[0] == "const":
            return ("const", (a[1] + b[1]) & 0xFFFFFFFF)
        return None
    if op == "subu":
        if a[0] == "lin" and b[0] == "lin" and a[1] == b[1]:
            return ("lin", a[1], a[2] - b[2])
        if a[0] == "lin" and b == ("const", 0):
            return a
        if a == ("const", 0) and b[0] == "lin":
            return ("lin", b[1], -b[2])
        return None
    return None


def parse_function_file(path):
    with open(path, errors="replace") as f:
        lines = f.read().splitlines()
    name = None
    for line in lines:
        m = re.match(r'^\s*glabel\s+(\w+)', line)
        if m:
            name = m.group(1)
            break
    return name or os.path.splitext(os.path.basename(path))[0], lines


# ---------------------------------------------------------------- clustering


class UnionFind:
    def __init__(self):
        self.parent = {}

    def find(self, x):
        self.parent.setdefault(x, x)
        while self.parent[x] != x:
            self.parent[x] = self.parent[self.parent[x]]
            x = self.parent[x]
        return x

    def union(self, a, b):
        ra, rb = self.find(a), self.find(b)
        if ra != rb:
            self.parent[max(ra, rb)] = min(ra, rb)


class Evidence:
    """Everything the functions say about one address space, indexed for the proposal builders."""

    def __init__(self, results):
        self.acc = defaultdict(list)          # (space, addr) -> [Access]
        self.funcs = defaultdict(set)         # (space, addr) -> functions touching it
        self.direct = defaultdict(set)        # space -> addrs with a direct/ptr access
        self.idx = defaultdict(lambda: defaultdict(set))   # (space, stride) -> addr -> funcs
        self.walks = defaultdict(lambda: defaultdict(set))  # (space, base) -> stride -> funcs
        self.ends = defaultdict(lambda: defaultdict(set))   # (space, base) -> end -> funcs
        self.ptrs = defaultdict(lambda: defaultdict(set))   # (space, base) -> field -> funcs
        self.orders = defaultdict(set)        # ((space, a), (space, b)) -> funcs
        self.shared_at = defaultdict(set)     # ((space, a), (space, b)) -> funcs
        for r in results:
            for a in r.accesses:
                if a.space == "ext":
                    continue
                k = (a.space, a.addr)
                self.acc[k].append(a)
                self.funcs[k].add(r.name)
                if a.kind in ("direct", "ptr"):
                    self.direct[a.space].add(a.addr)
                elif a.kind == "idx" and a.stride and a.stride >= max(a.width, 2):
                    self.idx[(a.space, a.stride)][a.addr].add(r.name)
            for space, base, field in r.ptrs:
                if space != "ext" and abs(field - base) <= MAX_FIELD_OFFSET:
                    self.ptrs[(space, base)][field].add(r.name)
            for space, base, stride in r.walks:
                if space != "ext":
                    self.walks[(space, base)][stride].add(r.name)
            for space, base, end in r.ends:
                if space != "ext" and 0 < end - base <= MAX_END_SPAN:
                    self.ends[(space, base)][end].add(r.name)
            for space, x, y in r.shared_at:
                if space != "ext" and x != y:
                    a, b = sorted(((space, x), (space, y)))
                    self.shared_at[(a, b)].add(r.name)
            for space, st, ld in r.orders:
                if space != "ext" and 0 < abs(ld - st) <= ORDER_MAX_DIST:
                    a, b = sorted(((space, st), (space, ld)))
                    self.orders[(a, b)].add(r.name)


def array_proposals(E, syms):
    """One proposal per indexed array: (space, stride) runs of fields, extended over the elements
    that later direct accesses reach at the same field offsets."""
    out = []
    taken = defaultdict(list)   # space -> [(base, end, stride)] arrays found with a smaller stride
    for (space, stride) in sorted(E.idx, key=lambda k: (k[1], k[0])):
        addrs = E.idx[(space, stride)]
        if stride < 2:
            continue
        # an outer stride (0xE0 = 4 * 0x38) over an array already found is its 2-D index
        rest = {a: f for a, f in addrs.items()
                if not any(b <= a < e and stride % s == 0 and s >= 4 and (a - b) % s in fl
                           for b, e, s, fl in taken[space])}
        if not rest:
            continue
        runs = []
        for a in sorted(rest):
            if runs and a - runs[-1][0] < stride:
                runs[-1].append(a)
            else:
                runs.append([a])
        # runs of one stride close together are one array (the run that starts it may begin at
        # any field, so the phase is not checked)
        groups = []
        for run in runs:
            g = groups[-1] if groups else None
            if g and run[0] - g["top"] <= RUN_GAP * stride:
                g["addrs"] |= set(run)
                g["top"] = run[-1]
            else:
                groups.append({"addrs": set(run), "top": run[-1]})
        gi = 0
        while gi < len(groups):
            g = groups[gi]
            base, members, last = grow_array(E, space, stride, g["addrs"])
            # a later group that the grown array reaches is part of it
            while gi + 1 < len(groups) and min(groups[gi + 1]["addrs"]) < base + (last + 1) * stride:
                g["addrs"] |= groups.pop(gi + 1)["addrs"]
                base, members, last = grow_array(E, space, stride, g["addrs"])
            gi += 1
            idx_funcs = set()
            for a in g["addrs"]:
                idx_funcs |= rest.get(a, set())
            ev = {"idx": idx_funcs}
            field_hits = len(members) - len(g["addrs"])
            if field_hits:
                ev["field"] = {"%d" % i for i in range(field_hits)}
            end = None
            for b in sorted(members):
                for e, fs in E.ends.get((space, b), {}).items():
                    ev.setdefault("end", set()).update(fs)
                    end = max(end or 0, e)
            for b in sorted(members):
                for st, fs in E.walks.get((space, b), {}).items():
                    if st % stride == 0:
                        ev.setdefault("walk", set()).update(fs)
            size = (last + 1) * stride
            if end and end - base > size:
                size = end - base
            if size % stride:
                size += stride - size % stride
            members |= {a for a in E.direct[space] if base <= a < base + size}
            taken[space].append((base, base + size, stride, {(a - base) % stride for a in g["addrs"]}))
            conf = "high" if len(idx_funcs) >= 2 or (field_hits and idx_funcs) else "medium"
            # a block walked in halfwords or words that holds globals of other widths is a region
            # (cleared or copied as a whole), not evidence of one element type
            if stride <= 4 and any(x.width != stride for a in members for x in E.acc[(space, a)]):
                conf = "medium"
            out.append(make_proposal(space, sorted(members), base, size, stride, ev, conf, E, syms))
    return out


def grow_array(E, space, stride, addrs):
    """Base, members and last element index of an indexed array: the indexed addresses, the later
    elements that have a direct access of the same width at an indexed field, and every direct
    access in between."""
    base = min(addrs)
    fields = {(a - base) % stride for a in addrs}
    widths = defaultdict(set)
    for a in addrs:
        widths[(a - base) % stride].update(x.width for x in E.acc[(space, a)] if x.kind == "idx")
    members = set(addrs)
    last = max((a - base) // stride for a in addrs)
    k, misses = last + 1, 0
    while misses < ELEMENT_GAP and k <= MAX_ELEMENTS:
        hit = {base + k * stride + f for f in fields
               if any(x.width in widths[f] for x in E.acc.get((space, base + k * stride + f), ())
                      if x.kind in ("direct", "ptr"))}
        if hit:
            last, misses = k, 0
        else:
            misses += 1
        k += 1
    members |= {a for a in E.direct[space] if base <= a < base + (last + 1) * stride}
    return base, members, last


def walk_proposals(E, syms, covered):
    """Pointer walks with a known end: `for (p = &D; p != &E; p += N)` over [D, E)."""
    out = []
    for (space, base), ends in E.ends.items():
        if (space, base) in covered:
            continue
        end = max(ends)
        stride = pick_stride(list(E.walks.get((space, base), {}).keys()))
        members = sorted({base} | {a for a in E.direct[space] if base <= a < end})
        ev = {"end": set().union(*ends.values())}
        if stride:
            ev["walk"] = set().union(*E.walks[(space, base)].values())
        conf = "high" if stride and len(ev["end"]) >= 2 else "medium"
        size = end - base
        out.append(make_proposal(space, members, base, size, stride, ev, conf, E, syms))
    return out


def struct_proposals(E, syms, covered):
    """Address in a register plus immediate field offsets; and ordered store/load pairs."""
    out = []
    uf = UnionFind()
    kinds = defaultdict(lambda: defaultdict(set))
    for (space, base), fields in E.ptrs.items():
        for f, fs in fields.items():
            uf.union((space, base), (space, f))
            kinds[(space, base)]["ptr"] |= fs
    for (a, b), fs in E.orders.items():
        uf.union(a, b)
        kinds[a]["order"] |= fs
        kinds[b]["order"] |= fs
    for (a, b), fs in E.shared_at.items():
        uf.union(a, b)
        kinds[a]["at"] |= fs
        kinds[b]["at"] |= fs
    comps = defaultdict(list)
    for k in list(uf.parent):
        comps[uf.find(k)].append(k)
    for keys in comps.values():
        if len(keys) < 2 or all(k in covered for k in keys):
            continue
        space = keys[0][0]
        addrs = sorted(k[1] for k in keys)
        ev = defaultdict(set)
        for k in keys:
            for kind, fs in kinds[k].items():
                ev[kind] |= fs
        base = addrs[0]
        top = max(a + max((x.width for x in E.acc.get((space, a), [])), default=1) for a in addrs)
        nfun = len(ev.get("order", ())) + len(ev.get("ptr", ()))
        conf = "high" if nfun >= 2 or ev.get("at") else "medium"
        out.append(make_proposal(space, addrs, base, top - base, None, dict(ev), conf, E, syms))
    return out


def cooc_proposals(E, syms, covered):
    """Neighbours (gap at most COOC_MAX_GAP) touched by exactly the same two or more functions."""
    out = []
    by_space = defaultdict(list)
    for k in E.acc:
        if k not in covered:
            by_space[k[0]].append(k[1])
    for space, addrs in by_space.items():
        addrs.sort()
        run = []
        for a in addrs + [None]:
            if a is not None and run and a - run[-1] <= COOC_MAX_GAP \
                    and len(E.funcs[(space, a)]) >= 2 and E.funcs[(space, a)] == E.funcs[(space, run[-1])]:
                run.append(a)
                continue
            if len(run) >= 2:
                top = run[-1] + max(x.width for x in E.acc[(space, run[-1])])
                out.append(make_proposal(space, run, run[0], top - run[0], None,
                                         {"cooc": set(E.funcs[(space, run[0])])}, "low", E, syms))
            run = [a] if a is not None else []
    return out


def build_proposals(results, syms):
    """All proposals, best first. A symbol can sit in more than one (an array and a struct view)."""
    E = Evidence(results)
    props = array_proposals(E, syms)
    covered = {(p["space"], a) for p in props for a in p["addrs"]}
    walks = walk_proposals(E, syms, covered)
    props += walks
    covered |= {(p["space"], a) for p in walks for a in p["addrs"]}
    props += struct_proposals(E, syms, covered)
    covered |= {(p["space"], a) for p in props for a in p["addrs"]}
    props += cooc_proposals(E, syms, covered)
    # attach ordered pairs to the proposal that holds both ends
    where = defaultdict(list)
    for i, p in enumerate(props):
        for a in p["addrs"]:
            where[(p["space"], a)].append(i)
    for kind, pairs in (("order", E.orders), ("at", E.shared_at)):
        for (a, b), fs in pairs.items():
            for i in set(where.get(a, [])) & set(where.get(b, [])):
                props[i].setdefault("_" + kind, set()).update(fs)
    for p in props:
        for kind in ("order", "at"):
            if "_" + kind in p:
                p["evidence"][kind] = len(p.pop("_" + kind))
        p["score"] = round(sum(WEIGHTS[k] * min(n, 5) for k, n in p["evidence"].items())
                           + 0.1 * len(p["addrs"]), 2)
    props.sort(key=lambda p: (-p["score"], p["space"], p["base_addr"]))
    return props


def pick_stride(seen):
    """The element stride: the smallest stride (4 or more if any) that divides the others seen;
    an outer stride (0x330 = 12 * 0x44) then does not hide the record size."""
    seen = sorted(set(seen))
    if not seen:
        return None
    big = [x for x in seen if x >= 4] or seen
    for cand in big:
        if all(x % cand == 0 for x in big):
            return cand
    return max(big)


def make_proposal(space, addrs, base, size, stride, ev, conf, E, syms):
    fields = {}
    for a in addrs:
        off = a - base
        if stride:
            off %= stride
        for x in E.acc.get((space, a), []):
            f = fields.setdefault(off, {"signs": set(), "n": 0, "widths": set()})
            f["widths"].add(x.width)
            if x.sign:
                f["signs"].add(x.sign)
            f["n"] += 1
    field_list = []
    for off in sorted(fields):
        f = fields[off]
        sign = "signed" if f["signs"] == {"s"} else "unsigned" if f["signs"] == {"u"} else \
            "mixed" if f["signs"] else ""
        field_list.append({"offset": off, "width": max(f["widths"]), "widths": sorted(f["widths"]),
                           "sign": sign, "accesses": f["n"]})
    names = [syms.name_at(space, a) for a in addrs]
    funcs = sorted({f for a in addrs for f in E.funcs.get((space, a), ())})
    return {
        "space": space, "base": syms.name_at(space, base), "base_addr": base, "size": max(size, 1),
        "stride": stride, "count": (size // stride) if stride else None, "fields": field_list,
        "symbols": names, "addrs": list(addrs), "absorbs": [n for n, a in zip(names, addrs) if a != base],
        "evidence": {k: len(v) for k, v in sorted(ev.items()) if v},
        "functions": len(funcs), "sample_functions": funcs[:6],
        "confidence": conf, "score": 0.0,
    }


# ---------------------------------------------------------------- tree loading


def function_files(root):
    pats = ["asm/nonmatchings/**/*.s", "asm/matchings/**/*.s", "asm/ovl/*/nonmatchings/**/*.s",
            "asm/ovl/*/matchings/**/*.s"]
    out = []
    for pat in pats:
        out.extend(glob.glob(os.path.join(root, pat), recursive=True))
    return sorted(out)


def data_files(root):
    spaces = defaultdict(list)
    for p in glob.glob(os.path.join(root, "asm/data/**/*.s"), recursive=True):
        spaces["main"].append(p)
    for p in glob.glob(os.path.join(root, "asm/ovl/*/data/**/*.s"), recursive=True):
        unit = os.path.relpath(p, root).split(os.sep)[2]
        spaces[unit].append(p)
    return spaces


def analyze_tree(root="."):
    syms = load_labels(data_files(root))
    results = []
    for path in function_files(root):
        rel = os.path.relpath(path, root).replace(os.sep, "/")
        m = ASM_PATH_RE.match(rel)
        unit = (m.group(1) if m and m.group(1) else "main")
        name, lines = parse_function_file(path)
        r = analyze_function("%s:%s" % (unit, name) if unit != "main" else name, lines, unit, syms)
        results.append(r)
    if not results:
        raise SystemExit("type_recovery: no function asm under %s/asm (run configure.py first)" % root)
    return build_proposals(results, syms), syms


# ---------------------------------------------------------------- src FAKE sites

SITE_RES = [
    # (&D_X)[n]
    re.compile(r'\(&(D_[0-9A-F]{8})\)\[(-?(?:0x[0-9A-Fa-f]+|\d+))\]'),
    # *(&D_X + n)
    re.compile(r'\*\(&(D_[0-9A-F]{8})\s*\+\s*(-?(?:0x[0-9A-Fa-f]+|\d+))\)'),
    # ((T *)&D_X)[n]
    re.compile(r'\(\((\w+)\s*\*\)\s*&(D_[0-9A-F]{8})\)\[(-?(?:0x[0-9A-Fa-f]+|\d+))\]'),
    # ((u8 *)&D_X + n)  /  (u8 *)&D_X + n
    re.compile(r'\(?\((\w+)\s*\*\)\s*&(D_[0-9A-F]{8})\)?\s*\+\s*(-?(?:0x[0-9A-Fa-f]+|\d+))'),
]
TYPE_SIZES = {"u8": 1, "s8": 1, "u16": 2, "s16": 2, "u32": 4, "s32": 4, "char": 1, "short": 2,
              "int": 4, "void": 1}
DECL_RE = re.compile(r'^\s*extern\s+(?:volatile\s+)?(?:unsigned\s+)?(\w+)\s+\*?\s*(D_[0-9A-F]{8})\s*(\[[^\]]*\])?\s*;')


def declared_sizes(root):
    sizes = {}
    for path in glob.glob(os.path.join(root, "include/**/*.h"), recursive=True) + \
            glob.glob(os.path.join(root, "src/**/*.c"), recursive=True):
        with open(path, errors="replace") as f:
            for line in f:
                m = DECL_RE.match(line)
                if m and m.group(2) not in sizes:
                    t = m.group(1)
                    sizes[m.group(2)] = 4 if "*" in line.split(m.group(2))[0] else TYPE_SIZES.get(t, 4)
    return sizes


def fake_sites(root, proposals):
    """Each `(&D_X)[n]`-style access in src/ near a FAKE comment, with the proposal that covers it."""
    sizes = declared_sizes(root)
    index = {}
    for i, p in enumerate(proposals):
        for addr in p["addrs"]:
            index.setdefault((p["space"], addr), i)
    out = []
    for path in sorted(glob.glob(os.path.join(root, "src/**/*.c"), recursive=True)):
        rel = os.path.relpath(path, root)
        unit = rel.split(os.sep)[2] if rel.startswith("src" + os.sep + "ovl") else "main"
        with open(path, errors="replace") as f:
            lines = f.read().splitlines()
        for ln, line in enumerate(lines):
            near = " ".join(lines[max(0, ln - 6):ln + 2])
            if "FAKE" not in near:
                continue
            for k, rx in enumerate(SITE_RES):
                for m in rx.finditer(line):
                    if k < 2:
                        sym, n, esz = m.group(1), to_int(m.group(2)), sizes.get(m.group(1), 1)
                    else:
                        sym, n, esz = m.group(2), to_int(m.group(3)), TYPE_SIZES.get(m.group(1), 1)
                    base = int(sym[2:], 16)
                    target = base + n * esz
                    space = "main"
                    for sp in (unit, "main"):
                        if (sp, base) in index:
                            space = sp
                            break
                    pi, ti = index.get((space, base)), index.get((space, target))
                    out.append({"file": rel, "line": ln + 1, "base": sym,
                                "target": "D_%08X" % target, "space": space,
                                "covered": pi is not None and pi == ti,
                                "proposal": pi if pi is not None and pi == ti else None})
    return out


# ---------------------------------------------------------------- output


def fmt_proposal(p):
    kind = "array" if p["stride"] else "struct"
    head = "%-6s %-8s %-10s %s size 0x%X" % (p["confidence"], p["space"], p["base"], kind, p["size"])
    if p["stride"]:
        head += " stride 0x%X x%d" % (p["stride"], p["count"] or 0)
    ev = " ".join("%s=%d" % kv for kv in p["evidence"].items())
    fields = " ".join("+%X:%d%s" % (f["offset"], f["width"], {"signed": "s", "unsigned": "u",
                                                               "mixed": "?"}.get(f["sign"], ""))
                      for f in p["fields"][:12])
    if len(p["fields"]) > 12:
        fields += " ..."
    absorbs = ", ".join(p["absorbs"][:8]) + (" ..." if len(p["absorbs"]) > 8 else "")
    return "%s  score %.1f [%s]\n    fields %s\n    absorbs %s\n    seen in %d functions: %s" % (
        head, p["score"], ev, fields, absorbs or "-", p["functions"], ", ".join(p["sample_functions"]))


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--root", default=".")
    ap.add_argument("--all", action="store_true", help="include low-confidence proposals")
    ap.add_argument("--sym", help="show the proposal holding this symbol")
    ap.add_argument("--unit", help="only this address space (main or an overlay name)")
    ap.add_argument("--top", type=int, default=40)
    ap.add_argument("--json", help="write all proposals to this file")
    ap.add_argument("--fake-sites", action="store_true", help="check the FAKE index tricks in src/")
    ap.add_argument("--summary", action="store_true", help="counts only")
    args = ap.parse_args(argv)

    proposals, syms = analyze_tree(args.root)
    if args.json:
        with open(args.json, "w") as f:
            json.dump(proposals, f, indent=1)
    if args.fake_sites:
        sites = fake_sites(args.root, proposals)
        for s in sites:
            p = proposals[s["proposal"]] if s["proposal"] is not None else None
            print("%s:%d %s -> %s: %s" % (s["file"], s["line"], s["base"], s["target"],
                                          "%s %s (%s)" % (p["confidence"], p["base"], p["space"]) if p
                                          else "no proposal"))
        cov = sum(1 for s in sites if s["covered"])
        print("%d sites, %d covered by a proposal" % (len(sites), cov))
        return 0
    if args.sym:
        r = None
        for space in ([args.unit] if args.unit else list(syms.defs)) + ["main"]:
            r = syms.resolve(space, args.sym)
            if r:
                break
        hits = [p for p in proposals if args.sym in p["symbols"] or
                (r and p["space"] == r[0] and p["base_addr"] <= r[1] < p["base_addr"] + max(p["size"], 1))]
        if args.unit:
            hits = [p for p in hits if p["space"] == args.unit]
        for p in hits:
            print(fmt_proposal(p))
        if not hits:
            print("%s: no proposal" % args.sym)
        return 0
    shown = [p for p in proposals if (args.all or p["confidence"] != "low") and
             (not args.unit or p["space"] == args.unit)]
    counts = defaultdict(int)
    for p in proposals:
        counts[p["confidence"]] += 1
    absorbed = sum(len(p["absorbs"]) for p in proposals if p["confidence"] != "low")
    print("%d proposals: %d high, %d medium, %d low; high+medium absorb %d symbols" % (
        len(proposals), counts["high"], counts["medium"], counts["low"], absorbed))
    if args.summary:
        return 0
    for p in shown[:args.top]:
        print(fmt_proposal(p))
    return 0


if __name__ == "__main__":
    sys.exit(main())
