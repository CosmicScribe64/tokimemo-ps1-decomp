#!/usr/bin/env python3
"""Explain why a C draft gets other registers than the original (T-10400).

Usage (in Docker, repo root as cwd, TOKIMEMO_SCRATCH set; wiki/decompile-workflow.md):
    python3 tools/regdiff.py [UNIT:]func_80114B0C [--unit UNIT] [--src draft.c] [--proto DECL ...] [-v] [--uopt PATH]

The draft is the function's C definition in its source file (a `#ifdef NON_MATCHING` draft counts,
`--src` names a scratch copy of the file). The complete translation unit is retained, including
file-scope data, helper functions and active asm includes, and compiled with the build's exact
pipeline (tools/cc.py: IDO 5.3, K&R, cvt pass, frame
pass, asm-processor), but with the instrumented uopt of T-9800 (wiki/uopt-register-allocation.md),
which prints every live range of the register allocator: its priority, its uses and stores per
basic block, the cost of its cheapest colour and the colour it got. The draft is then compared with
the original (funcdiff's resolved words, register fields masked for the alignment):

  - same instructions with other registers: the first divergence is mapped to the live range that
    holds the draft's register (candidates are ranked by the def/use counts of the register web in
    the draft's code), the range that holds the register the original used, their priorities, and a
    hint from the documented allocation rules;
  - commutative instructions whose source registers are exchanged are reported as operand order, not
    as an allocation difference; constants that sit in other registers are listed by value;
  - other instructions: the first structural difference is printed and the register analysis runs on
    the aligned parts only. Fix the structure first (funcdiff.py shows the full diff).

--proto 'void func_80012345(s32 a0);' (repeatable) replaces the declaration of a function the file calls, for
this compile only (tools/protoverride.py, T-16530): test void against implicit int (`s32 f();`) against typed
arguments without editing a header or a src/ file. The report names each replaced declaration and, on a MATCH,
the `*_OVERRIDE_<symbol>` guard that keeps the view in the tree.

The report ends with the time spent. Exit status 0 for an identical function, 1 when something
differs, 2 for an error (no traced uopt, the draft does not compile, function not found).

The instrumented uopt is not in the repository (no compiler binaries, no reconstruction source).
Build it as wiki/uopt-register-allocation.md describes and pass --uopt PATH, or set REGDIFF_UOPT;
the default is /scratch/r9-uopt/variants/trace, i.e. $TOKIMEMO_SCRATCH/r9-uopt/variants/trace
(tools/docker.sh mounts TOKIMEMO_SCRATCH at /scratch); a missing binary is reported with the paths tried
(find_uopt). Run time: about 1.5 to 2 s for the traced compile of a
function plus 1 to 2 s of start-up and original-side lookup (cached); more on a loaded machine.

The names of variables come from the ucode (stack offset or symbol, line numbers) and the
declarations in the draft; a range with no variable (a hoisted constant, an address) is described by
the instruction that defines it. Treat the mapping as strong evidence, not proof: it is printed with
its confidence.
"""
import argparse
import difflib
import os
import re
import shutil
import subprocess
import sys
import time
from collections import defaultdict

# tools/queue.py is a project CLI, not the standard library queue module (permute imports queue).
_tools_path = sys.path.pop(0) if sys.path and os.path.abspath(sys.path[0]) == os.path.dirname(os.path.abspath(__file__)) else None
import queue  # noqa: F401,E402  (stdlib)
if _tools_path:
    sys.path.insert(0, _tools_path)

import protoverride  # noqa: E402

DEFAULT_UOPT = "/scratch/r9-uopt/variants/trace"
SCRATCH = "build/scratch/regdiff"
UOPT_RECIPE = "wiki/uopt-register-allocation.md"
UOPT_REL = "r9-uopt/variants/trace"

REG_NAMES = ("zero at v0 v1 a0 a1 a2 a3 t0 t1 t2 t3 t4 t5 t6 t7 "
             "s0 s1 s2 s3 s4 s5 s6 s7 t8 t9 k0 k1 gp sp fp ra").split()
REG_NUM = {n: i for i, n in enumerate(REG_NAMES)}

# uopt colour numbers (wiki/uopt-register-allocation.md) to hardware register numbers.
COLOUR_REG = {1: 2, 2: 3, 3: 4, 4: 5, 5: 6, 6: 7, 7: 8, 8: 9, 9: 10, 10: 11, 11: 12, 12: 13,
              13: 31, 14: 16, 15: 17, 16: 18, 17: 19, 18: 20, 19: 21, 20: 22, 21: 23, 22: 30, 23: 31}
REG_COLOUR = {}
for _c, _r in sorted(COLOUR_REG.items(), reverse=True):
    REG_COLOUR[_r] = _c
UGEN_SCRATCH = {1, 14, 15, 24, 25}      # at, t6, t7, t8, t9: ugen/assembler temporaries, not uopt colours


def find_uopt(explicit=None, env=None, exists=os.path.exists, isdir=os.path.isdir, access=os.access):
    """(path, None) for the traced uopt to use, or (None, message) that says where it looked and how
    to provide it (T-16530). Order: --uopt, $REGDIFF_UOPT (a path that is given is never replaced by
    another: a typo must not pick up a different binary), then the default /scratch/r9-uopt/variants/trace
    and, if the container has TOKIMEMO_SCRATCH set, $TOKIMEMO_SCRATCH/r9-uopt/variants/trace."""
    env = os.environ if env is None else env
    if explicit:
        tried = [(explicit, "--uopt")]
    elif env.get("REGDIFF_UOPT"):
        tried = [(env["REGDIFF_UOPT"], "$REGDIFF_UOPT")]
    else:
        tried = [(DEFAULT_UOPT, "default; tools/docker.sh mounts TOKIMEMO_SCRATCH at /scratch")]
        if env.get("TOKIMEMO_SCRATCH"):
            tried.append((os.path.join(env["TOKIMEMO_SCRATCH"], UOPT_REL), "$TOKIMEMO_SCRATCH"))
    unusable = []
    for path, how in tried:
        if exists(path):
            if access(path, os.X_OK):
                return path, None
            unusable.append("%s (%s) exists but is not executable" % (path, how))
    lines = ["regdiff: the instrumented (traced) uopt was not found."]
    lines += ["  looked at: %s  [%s]" % (p, how) for p, how in tried]
    lines += ["  " + u for u in unusable]
    if not explicit and not env.get("REGDIFF_UOPT") and not isdir("/scratch"):
        lines.append("  /scratch is not mounted: start the tool as `TOKIMEMO_SCRATCH=<dir> tools/docker.sh python3 "
                     "tools/regdiff.py ...`, <dir> being the folder that holds %s." % UOPT_REL)
    lines.append("  provide it: build the traced uopt once (tools/ido-ecvt.patch + tools/uopt_patch.py trace; recipe in "
                 "%s) into $TOKIMEMO_SCRATCH/%s, or pass --uopt PATH / set REGDIFF_UOPT=PATH (a path as seen "
                 "inside the container)." % (UOPT_RECIPE, UOPT_REL))
    return None, "\n".join(lines)


def rname(r):
    return "$" + REG_NAMES[r]


def cname(c):
    r = COLOUR_REG.get(c)
    return "%d (%s)" % (c, "$" + REG_NAMES[r]) if r is not None else str(c)


# ---------------------------------------------------------------------------------------------
# MIPS words: register fields, defs and uses

RS, RT, RD = 21, 16, 11


class Decoded:
    """One instruction word: its register operands (field shift, role, register), the registers it
    defines and uses, and its control-flow kind."""

    def __init__(self, word, text=""):
        self.word = word
        self.text = text
        self.fields = []            # (shift, "def"|"use", reg)
        self.kind = "plain"         # plain | branch (conditional) | jump | call | ret | jr
        self.target = None          # branch displacement in instructions (from the delay slot)
        self._decode()

    def _decode(self):
        w = self.word
        op = w >> 26
        rs, rt, rd = (w >> RS) & 31, (w >> RT) & 31, (w >> RD) & 31
        f = self.fields
        if op == 0:
            fn = w & 63
            if fn in (0, 2, 3):
                f += [(RD, "def", rd), (RT, "use", rt)]
            elif fn in (4, 6, 7):
                f += [(RD, "def", rd), (RT, "use", rt), (RS, "use", rs)]
            elif fn == 8:
                f += [(RS, "use", rs)]
                self.kind = "ret" if rs == 31 else "jr"
            elif fn == 9:
                f += [(RD, "def", rd), (RS, "use", rs)]
                self.kind = "call"
            elif fn in (16, 18):
                f += [(RD, "def", rd)]
            elif fn in (17, 19):
                f += [(RS, "use", rs)]
            elif fn in (24, 25, 26, 27):
                f += [(RS, "use", rs), (RT, "use", rt)]
            elif fn >= 32:
                f += [(RD, "def", rd), (RS, "use", rs), (RT, "use", rt)]
        elif op == 1:
            f += [(RS, "use", rs)]
            self.kind = "call" if rt in (16, 17) else "branch"
            self.target = _sext16(w)
        elif op == 2:
            self.kind = "jump"
        elif op == 3:
            self.kind = "call"
        elif op in (4, 5):
            f += [(RS, "use", rs), (RT, "use", rt)]
            self.kind = "branch"
            self.target = _sext16(w)
        elif op in (6, 7):
            f += [(RS, "use", rs)]
            self.kind = "branch"
            self.target = _sext16(w)
        elif 8 <= op <= 14:
            f += [(RT, "def", rt), (RS, "use", rs)]
        elif op == 15:
            f += [(RT, "def", rt)]
        elif op in (0x10, 0x12):
            if rs in (0, 2):
                f += [(RT, "def", rt)]
            elif rs in (4, 6):
                f += [(RT, "use", rt)]
        elif op in (0x20, 0x21, 0x23, 0x24, 0x25):
            f += [(RT, "def", rt), (RS, "use", rs)]
        elif op in (0x22, 0x26):
            f += [(RT, "def", rt), (RS, "use", rs), (RT, "use", rt)]
        elif op in (0x28, 0x29, 0x2A, 0x2B, 0x2E):
            f += [(RT, "use", rt), (RS, "use", rs)]
        elif op in (0x32, 0x3A):
            f += [(RS, "use", rs)]
        self.defs = [r for _s, role, r in f if role == "def" and r]
        self.uses = [r for _s, role, r in f if role == "use" and r]

    @property
    def shape(self):
        """The word with every register field cleared: what the alignment compares."""
        w = self.word
        for shift, _role, _r in self.fields:
            w &= ~(31 << shift)
        return w

    def reg_at(self, shift):
        return (self.word >> shift) & 31


def _sext16(w):
    w &= 0xFFFF
    return w - 0x10000 if w & 0x8000 else w


def key_word(key):
    """The 32-bit word at the start of a funcdiff key (`%08x` plus unresolved relocations); objdump's
    `...` for a run of zero words counts as a nop."""
    try:
        return int(key[:8], 16)
    except ValueError:
        return 0


IMM_OPS = set(range(8, 16)) | {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x28, 0x29, 0x2A, 0x2B, 0x2E, 0x32, 0x3A}


def key_shape(key):
    """Alignment key: the word with the register fields and the immediate of an I-type
    operation cleared (a constant or address differs by value, not by structure), plus funcdiff's
    relocation text for everything else."""
    d = Decoded(key_word(key))
    if d.word >> 26 in IMM_OPS:
        return (d.shape & ~0xFFFF, "")
    return (d.shape, key[8:])


def key_imm(key):
    """The immediate of an I-type word with funcdiff's unresolved relocation text, else None."""
    w = key_word(key)
    return (w & 0xFFFF, key[8:]) if w >> 26 in IMM_OPS else None


# ---------------------------------------------------------------------------------------------
# alignment of the draft with the original

class RegDiff:
    """One aligned instruction pair that differs in registers or in a constant."""

    def __init__(self, draft_index, target_index, ops, imm=None, swap=False):
        self.draft_index = draft_index
        self.target_index = target_index
        self.ops = ops              # [(shift, role, draft reg, target reg)]
        self.imm = imm              # (draft immediate, target immediate) when they differ
        self.swap = swap            # the same commutative operation with its source registers exchanged


def align(draft_keys, target_keys):
    """(pairs, structural). `pairs`: RegDiff records of instructions with the same operation that
    differ in registers and/or in a constant; a constant that moved to another instruction (two address
    constants swapped between registers) is paired by value, so the registers are compared for the
    same value. `structural`: (op, draft range, target range) of the remaining differences."""
    d = [key_shape(k) for k in draft_keys]
    t = [key_shape(k) for k in target_keys]
    sm = difflib.SequenceMatcher(None, d, t, autojunk=False)
    pairs, structural, moved = [], [], []
    for op, i1, i2, j1, j2 in sm.get_opcodes():
        if op == "equal":
            for k in range(i2 - i1):
                i, j = i1 + k, j1 + k
                a, b = Decoded(key_word(draft_keys[i])), Decoded(key_word(target_keys[j]))
                ops = [(s, role, a.reg_at(s), b.reg_at(s)) for s, role, _r in a.fields
                       if a.reg_at(s) != b.reg_at(s)]
                ia, ib = key_imm(draft_keys[i]), key_imm(target_keys[j])
                if ia != ib:
                    moved.append((i, j, ia, ib))
                elif ops:
                    pairs.append(RegDiff(i, j, ops, swap=is_swap(a, b)))
        else:
            structural.append((op, (i1, i2), (j1, j2)))
    # constants that changed place: pair each draft constant with the original instruction that has it
    by_imm = defaultdict(list)
    for i, j, ia, ib in moved:
        by_imm[(d[i], ib)].append(j)
    left = []
    for i, j, ia, ib in moved:
        cands = by_imm.get((d[i], ia), [])
        if cands:
            j2 = cands.pop(0)
            a, b = Decoded(key_word(draft_keys[i])), Decoded(key_word(target_keys[j2]))
            ops = [(s, role, a.reg_at(s), b.reg_at(s)) for s, role, _r in a.fields if a.reg_at(s) != b.reg_at(s)]
            if ops:
                pairs.append(RegDiff(i, j2, ops, swap=is_swap(a, b)))
        else:
            a, b = Decoded(key_word(draft_keys[i])), Decoded(key_word(target_keys[j]))
            left.append(RegDiff(i, j, [(s, role, a.reg_at(s), b.reg_at(s)) for s, role, _r in a.fields
                                       if a.reg_at(s) != b.reg_at(s)], imm=(ia, ib)))
    pairs.sort(key=lambda p: p.draft_index)
    return pairs, structural, left


COMMUTATIVE_R = {0x18, 0x19, 0x20, 0x21, 0x24, 0x25, 0x26, 0x27}


def is_swap(a, b):
    """True when two words are the same commutative operation with the two source registers exchanged."""
    if a.shape != b.shape:
        return False
    op = a.word >> 26
    if not (op in (4, 5) or (op == 0 and a.word & 63 in COMMUTATIVE_R)):
        return False
    ra, rb = (a.word >> RS) & 31, (a.word >> RT) & 31
    return (ra != rb and (ra, rb) == ((b.word >> RT) & 31, (b.word >> RS) & 31)
            and (a.word >> RD) & 31 == (b.word >> RD) & 31)


def register_map(pairs):
    """{(draft reg, target reg): count} over every differing operand."""
    out = defaultdict(int)
    for p in pairs:
        if p.swap:
            continue
        for _s, _role, a, b in p.ops:
            out[(a, b)] += 1
    return dict(out)


def first_divergence(pairs):
    """(RegDiff, operand) of the first difference, a defined register before a used one."""
    pairs = [p for p in pairs if not p.swap]
    if not pairs:
        return None
    p = pairs[0]
    for op in p.ops:
        if op[1] == "def":
            return p, op
    return p, p.ops[0]


# ---------------------------------------------------------------------------------------------
# register webs of the draft's code (a web is one allocator live range or one ugen temporary)

CALL_CLOBBER = [r for r in range(1, 26) if r not in (16, 17, 18, 19, 20, 21, 22, 23)]
ARG_REGS = (4, 5, 6, 7)
ENTRY_DEFS = (4, 5, 6, 7) + tuple(range(16, 24)) + (29, 30, 31)


class Web:
    const = None                # value of a web built from constants only

    def __init__(self, reg):
        self.reg = reg
        self.defs = []              # instruction indices of real definitions
        self.uses = []              # instruction indices of real uses
        self.entry = False          # a parameter or callee-saved value live on entry
        self.spill = 0              # stack stores/loads of the register (save across a call)
        self.calls = []             # calls (and "ret") that take the value in this register as argument/result

    @property
    def first(self):
        pts = self.defs + self.uses
        return min(pts) if pts else 1 << 30

    @property
    def last(self):
        pts = self.defs + self.uses
        return max(pts) if pts else -1


def successors(insns):
    """Instruction-level successors; a branch's delay slot is its successor and carries the edge."""
    n = len(insns)
    leaders = set()
    for i, d in enumerate(insns):
        if d.kind == "branch" and d.target is not None:
            leaders.add(i + 1 + d.target)
    succ = [[i + 1] if i + 1 < n else [] for i in range(n)]
    for i in range(1, n):
        b = insns[i - 1]
        if b.kind == "branch":
            succ[i] = [x for x in {i + 1, i + b.target} if 0 <= x < n] if b.target is not None else [i + 1]
        elif b.kind == "ret":
            succ[i] = []
        elif b.kind in ("jr", "jump"):
            succ[i] = sorted(leaders)       # jump table or absolute jump: any label (conservative)
    return succ


def build_webs(insns):
    """[Web] of the draft: reaching definitions per register, definitions joined by a common use.
    A call clobbers the caller-saved registers after its delay slot; a stack store and reload of the
    same slot and register belong to one web (the allocator's save across a call)."""
    n = len(insns)
    succ = successors(insns)
    pred = [[] for _ in range(n)]
    for i, ss in enumerate(succ):
        for s in ss:
            pred[s].append(i)
    webs = []
    for r in range(1, 32):
        if r in (28, 29):
            continue
        bit_of, kinds = [], []
        real_bit, call_bit = {}, {}
        for i, d in enumerate(insns):
            if r in d.defs:
                real_bit[i] = len(bit_of)
                bit_of.append(i)
                kinds.append("real")
            if d.kind == "call" and r in CALL_CLOBBER and i + 1 < n:
                call_bit[i + 1] = len(bit_of)       # takes effect at the delay slot's end
                bit_of.append(i + 1)
                kinds.append("call")
        entry = None
        if r in ENTRY_DEFS:
            entry = len(bit_of)
            bit_of.append(-1)
            kinds.append("entry")
        if not real_bit and not any(r in d.uses for d in insns):
            continue

        def transfer(i, inn):
            if i in call_bit:
                return 1 << call_bit[i]
            if i in real_bit:
                return 1 << real_bit[i]
            return inn

        def after_slot(i, inn):         # the state a call or return sees: the slot's own def, no clobber
            return (1 << real_bit[i]) if i in real_bit else inn

        inn = [0] * n
        out = [0] * n
        work = list(range(n))
        while work:
            i = work.pop()
            cur = (1 << entry) if (i == 0 and entry is not None) else 0
            for p in pred[i]:
                cur |= out[p]
            new_out = transfer(i, cur)
            inn[i] = cur
            if new_out != out[i]:
                out[i] = new_out
                work.extend(succ[i])
        parent = list(range(len(bit_of)))

        def find(x):
            while parent[x] != x:
                parent[x] = parent[parent[x]]
                x = parent[x]
            return x

        def join(mask):
            bits = [b for b in range(len(bit_of)) if mask >> b & 1]
            for b in bits[1:]:
                parent[find(b)] = find(bits[0])
            return bits[0] if bits else None

        use_at = {}                 # instruction -> a bit of the web it reads (real uses)
        inplace = set()             # instructions that update their own register (not counted as uses)
        pseudo = []                 # (bit, call instruction or -1 for a return): arguments and results
        for i, d in enumerate(insns):
            if r in d.uses:
                b = join(inn[i])
                if b is not None:
                    use_at[i] = b
                    if i in real_bit and not 0x20 <= (d.word >> 26) <= 0x26:
                        # `addiu t1,t1,lo` after `lui t1,hi`: one value built in place, one range
                        parent[find(real_bit[i])] = find(b)
                        inplace.add(i)
            if d.kind == "call" and r in ARG_REGS and i + 1 < n:
                b = join(after_slot(i + 1, inn[i + 1]))
                if b is not None:
                    pseudo.append((b, i))
            if d.kind == "ret" and r in (2, 3) and i + 1 < n:
                b = join(after_slot(i + 1, inn[i + 1]))
                if b is not None:
                    pseudo.append((b, -1))
        # a save across a call: sw r,off(sp) ... lw r,off(sp)
        stores, loads = {}, {}
        for i, d in enumerate(insns):
            op = d.word >> 26
            if (d.word >> RS) & 31 == 29 and (d.word >> RT) & 31 == r:
                off = _sext16(d.word)
                if op == 0x2B and i in use_at:
                    stores.setdefault(off, []).append(i)
                elif op == 0x23 and i in real_bit:
                    loads.setdefault(off, []).append(i)
        spill = set()
        for off in set(stores) & set(loads):
            for i in stores[off]:
                for j in loads[off]:
                    parent[find(use_at[i])] = find(real_bit[j])
                    spill.update((i, j))
        groups = {}

        def web(b):
            root = find(b)
            if root not in groups:
                groups[root] = Web(r)
            return groups[root]

        for b, i in enumerate(bit_of):
            if kinds[b] == "real" and i not in spill:
                web(b).defs.append(i)
            elif kinds[b] == "real":
                web(b).spill += 1
            elif kinds[b] == "entry":
                web(b).entry = True
        for b, i in pseudo:
            web(b).calls.append(i)
        for i, b in use_at.items():
            if i in spill:
                web(b).spill += 1
            elif i not in inplace:
                web(b).uses.append(i)
        for g in groups.values():
            g.defs.sort()
            g.uses.sort()
            if g.defs or g.uses:
                g.const = web_constant(g, insns)
                webs.append(g)
    return webs


def web_at(webs, index, reg, role):
    """The Web of `reg` that is defined (role def) or used (role use) at instruction `index`."""
    best = None
    for w in webs:
        if w.reg != reg:
            continue
        if (index in w.defs) if role == "def" else (index in w.uses):
            return w
        if best is None and role == "use" and w.first <= index <= w.last:
            best = w
    return best


# ---------------------------------------------------------------------------------------------
# the allocator trace

class Range:
    def __init__(self, bit, lr):
        self.bit = bit
        self.lr = lr
        self.blocks = 0             # blocks reached (references plus traversed)
        self.priority = None
        self.state = None
        self.units = []             # dicts: bb, freq, loads, stores, load, save, dead
        self.cost = None            # cost of the cheapest colour
        self.cost_reg = None        # that colour
        self.order = None           # position in the allocation sequence
        self.colour = None          # final colour, None when not coloured
        self.mode = None            # constrained | unconstrained
        self.reason = None          # "-ve save" and similar
        self.split_from = None

    @property
    def loads(self):
        return sum(u["loads"] for u in self.units)

    @property
    def stores(self):
        return sum(u["stores"] for u in self.units)

    @property
    def span(self):
        bbs = [u["bb"] for u in self.units]
        return (min(bbs), max(bbs)) if bbs else None

    @property
    def denominator(self):
        n = self.blocks
        return n if n <= 2 else (n - 2) // 4 + 2

    @property
    def reg(self):
        return COLOUR_REG.get(self.colour)

    def label(self):
        return "bit %d" % self.bit if self.lr == self.bit else "bit %d/lr %d" % (self.bit, self.lr)


class ProcTrace:
    def __init__(self, name):
        self.name = name
        self.pool = ""
        self.ranges = {}            # lr -> Range (the first one of each lr is the initial range)
        self.sequence = []          # lr in allocation order
        self.table = {}             # bit -> dict(kind, op, l, r, ops, id)
        self.rows = {}              # expression id -> row
        self.max_table_bit = -1


R9_RE = re.compile(r"^R9(\w+) (.*)$")


def kv(text):
    return dict(p.split("=", 1) for p in text.split() if "=" in p)


def parse_stderr(text):
    """{procedure: ProcTrace} from the R9 lines of the instrumented uopt (priority, units, costs)."""
    procs, cur, last = {}, None, None
    for line in text.splitlines():
        m = R9_RE.match(line)
        if not m:
            continue
        tag, rest = m.groups()
        if tag == "PROC":
            cur = procs.setdefault(rest.strip(), ProcTrace(rest.strip()))
            last = None
        elif cur is None:
            continue
        elif tag == "POOL":
            cur.pool = rest.strip()
        elif tag == "SAVE":
            v = kv(rest)
            lr = int(v["lr"])
            last = Range(int(v["bit"]), lr)
            last.blocks = int(v["blocks"])
            last.priority = float(v["priority"])
            last.state = int(v["state"])
            # a later SAVE of the same live range is a recomputation after a split: keep both,
            # the new one under a fresh key
            key = lr if lr not in cur.ranges else ("again", lr, len(cur.ranges))
            cur.ranges[key] = last
        elif tag == "UNIT" and last is not None:
            v = kv(rest)
            last.units.append({k: int(float(x)) for k, x in v.items()})
        elif tag == "COST":
            v = kv(rest)
            lr = int(v["lr"])
            r = cur.ranges.get(lr)
            if r is None:
                continue
            r.cost = float(v["cost"])
            r.cost_reg = int(v["reg"])
            if r.order is None:
                r.order = len(cur.sequence)
            cur.sequence.append(lr)
    return procs


PHASE_RE = re.compile(r"^ \* \*\s+[\d. ]+ SECONDS IN (.+?) OF (\S+)\s*$", re.I)
ASSIGN_RE = re.compile(r"^\s*(\d+):\s+(\d+) assigned \((\w+)\)\s+(\d+)\s*$")
UNCOLOR_RE = re.compile(r"^\s*(\d+):\s+(\d+) not colored \((.*)\)\s*$")
SPLIT_RE = re.compile(r"^live range\s+(\d+):\s+(\d+) split out\s+(\d+)\s*$")
ROW_RE = re.compile(r"^\{\s*(\d+)\|(\d+)\}\s+(\d+) is(var|op)\s+(.*?)\s*$")


def parse_listing(text, procs):
    """Add the expression table and the final colours of uopt's native listing (-l) to `procs`."""
    chunk = []
    for line in text.splitlines():
        m = PHASE_RE.match(line)
        if not m:
            chunk.append(line)
            continue
        phase, name = m.groups()
        proc = procs.get(name)
        if proc is not None:
            if phase.startswith("CODE MOTION"):
                _read_table(proc, chunk)
            elif phase.startswith("global coloring"):
                _read_colours(proc, chunk)
        chunk = []
    return procs


def _read_table(proc, lines):
    for line in lines:
        m = ROW_RE.match(line)
        if not m:
            continue
        rid, ver, bit, kind, rest = m.groups()
        row = {"id": int(rid), "ver": int(ver), "bit": int(bit), "kind": kind}
        if kind == "var":
            parts = rest.replace("vreg", "").split()
            row.update(cls=parts[0], l=int(parts[1]), r=int(parts[2]))
        else:
            op = re.match(r"(\w+)((?:\{[^}]*\}\s*)*)", rest)
            row["op"] = op.group(1)
            row["ops"] = [(int(a), int(b)) for a, b in re.findall(r"\{\s*(\d+)\|(\d+)\}", op.group(2))]
        proc.table[row["bit"]] = row
        proc.rows[row["id"]] = row
        proc.max_table_bit = max(proc.max_table_bit, row["bit"])


def _read_colours(proc, lines):
    by_lr = {}
    for key, r in proc.ranges.items():
        by_lr.setdefault(r.lr, []).append(r)
    for line in lines:
        m = ASSIGN_RE.match(line)
        if m:
            bit, lr, mode, colour = int(m[1]), int(m[2]), m[3], int(m[4])
            for r in reversed(by_lr.get(lr, [])):
                if r.bit == bit:
                    r.colour, r.mode = colour, mode
                    break
            continue
        m = UNCOLOR_RE.match(line)
        if m:
            bit, lr = int(m[1]), int(m[2])
            for r in reversed(by_lr.get(lr, [])):
                if r.bit == bit and r.colour is None:
                    r.reason = m[3]
                    break
            continue
        m = SPLIT_RE.match(line)
        if m:
            bit, old, new = int(m[1]), int(m[2]), int(m[3])
            for r in by_lr.get(new, []):
                r.split_from = old


def final_ranges(proc):
    """The ranges that took part in the final colouring: the last record of each live range."""
    last = {}
    for key, r in proc.ranges.items():
        last[r.lr] = r
    return list(last.values())


# ---------------------------------------------------------------------------------------------
# names and expressions from the ucode and the source

KEYWORDS = set("""auto break case char const continue default do double else enum extern float for goto if int
long register return short signed sizeof static struct switch typedef union unsigned void volatile while
s8 u8 s16 u16 s32 u32 s64 u64 f32 f64""".split())
IDENT = re.compile(r"[A-Za-z_]\w*")
MEMCLASS = {1: "M", 2: "P", 3: "E", 4: "S"}


def ucode_refs(data, parse, ops):
    """{(class, l, r): sorted source lines} for the variables the ucode reads or stores."""
    refs = defaultdict(set)
    line = 0
    for ins in parse(data):
        opc = ops[ins.opc]
        if opc == "loc":
            line = ins.words[1]
        elif opc in ("lod", "str", "lda", "rlod", "rstr", "rlda") and len(ins.words) >= 4:
            cls = MEMCLASS.get(ins.mtype)
            if cls:
                refs[(cls, ins.words[1], ins.words[3] - (1 << 32) if ins.words[3] >= 1 << 31 else ins.words[3])].add(line)
    return {k: sorted(v) for k, v in refs.items()}


def declared_locals(text):
    """Names declared in the function head: parameters and locals, in order, from the first `{`."""
    brace = text.find("{")
    head = text[:brace]
    m = re.search(r"\(([^()]*)\)", head)
    params = []
    if m:
        for p in m.group(1).split(","):
            ids = IDENT.findall(p)
            ids = [i for i in ids if i not in KEYWORDS]
            if ids:
                params.append(ids[-1])
    tail = head[m.end():] if m else ""
    params2 = []
    for decl in re.findall(r"[^;]+;", tail):
        ids = [i for i in IDENT.findall(decl) if i not in KEYWORDS]
        params2 += ids[-1:] if ids else []
    if params2:
        params = [p for p in params if p in params2] or params
    body = text[brace + 1:]
    locals_ = []
    for line in body.split("\n"):
        mm = re.match(r"^[ \t]*(?:register\s+)?(?:(?:unsigned|signed|const|volatile|struct|union)\s+)*"
                      r"[A-Za-z_]\w*[ \t*]+([A-Za-z_]\w*(?:\[[^\]]*\])?(?:\s*,\s*\**\s*[A-Za-z_]\w*(?:\[[^\]]*\])?)*)\s*;\s*(?:/\*.*\*/)?\s*$",
                      line)
        if not mm:
            if line.strip() and not line.strip().startswith(("/*", "//", "#")):
                break
            continue
        locals_ += [i for i in IDENT.findall(mm.group(1)) if i not in KEYWORDS]
    return params, locals_


def name_variables(proc, refs, src_lines, base_line0):
    """{(class, l, r): (name, lines)} guessed from the lines that mention each variable."""
    func_text = "\n".join(src_lines[base_line0 - 1:])
    params, locals_ = declared_locals(func_text)
    names = {}
    skip = set(KEYWORDS)
    for key, lines in refs.items():
        cls = key[0]
        toks = []
        for ln in lines:
            if 0 < ln <= len(src_lines):
                toks.append(set(IDENT.findall(src_lines[ln - 1])) - skip)
        if not toks:
            continue
        if cls == "P":
            idx = key[2] // 4
            name = params[idx] if 0 <= idx < len(params) else "arg%d" % idx
            names[key] = (name, lines)
            continue
        pool = set.intersection(*toks) if toks else set()
        if cls == "M":
            cand = sorted(pool & set(locals_))
            if not cand:
                counts = defaultdict(int)
                for t in toks:
                    for i in t & set(locals_):
                        counts[i] += 1
                cand = sorted(counts, key=lambda i: (-counts[i], locals_.index(i)))[:1]
        else:
            cand = sorted(pool - set(locals_) - set(params))
            if not cand:
                counts = defaultdict(int)
                for t in toks:
                    for i in t - set(locals_) - set(params):
                        counts[i] += 1
                cand = sorted(counts, key=lambda i: (-counts[i], i))[:2]
        if cand:
            names[key] = ("/".join(cand[:3]), lines)
    return names


def render(proc, names, ident, depth=0):
    """Readable text of expression `ident` of the table (variables by their guessed names)."""
    row = proc.rows.get(ident)
    if row is None:
        return "#%d" % ident
    if row["kind"] == "var":
        key = (row["cls"], row["l"], row["r"])
        if key in names:
            return names[key][0]
        return "%s%d%+d" % (row["cls"], row["l"], row["r"]) if row["cls"] != "R" else "$ret"
    if depth >= 3:
        return "(...)"
    op = row["op"][1:] if row["op"].startswith("u") else row["op"]
    return "%s(%s)" % (op, ", ".join(render(proc, names, i, depth + 1) for i, _v in row["ops"]))


# ---------------------------------------------------------------------------------------------
# linking the draft's registers to ranges, and the hints

def candidate_ranges(ranges, reg):
    return [r for r in ranges if r.colour is not None and r.reg == reg]


def score_web(r, web):
    """Smaller is better: distance between the range's references and the web's real ones. The
    allocator's loads are uses and its stores are definitions (a built-in-place constant has one
    definition less than it has instructions)."""
    defs = 0 if web.const is not None else abs(len(web.defs) - r.stores)   # a constant is not a store
    return defs + abs(len(web.uses) - r.loads)


def assign_ranges(ranges, webs):
    """{id(Web): (Range, exact)}: each coloured range to at most one web of its register, best
    def/use match first, program order as the tie-break (ranges sharing a register are disjoint)."""
    out = {}
    for reg in {w.reg for w in webs}:
        ws = sorted((w for w in webs if w.reg == reg), key=lambda w: w.first)
        rs = sorted(candidate_ranges(ranges, reg), key=lambda r: (r.span[0] if r.span else 0, r.bit))
        edges = []
        for wi, w in enumerate(ws):
            for ri, r in enumerate(rs):
                edges.append((score_web(r, w), abs(wi * len(rs) - ri * len(ws)), wi, ri))
        edges.sort()
        taken_w, taken_r = set(), set()
        for score, _d, wi, ri in edges:
            if wi in taken_w or ri in taken_r:
                continue
            taken_w.add(wi)
            taken_r.add(ri)
            out[id(ws[wi])] = (rs[ri], score == 0)
    return out


def overlap(a, b):
    sa, sb = a.span, b.span
    if sa is None or sb is None:
        return True
    return sa[0] <= sb[1] and sb[0] <= sa[1]


def web_constant(web, insns):
    """Value of a web built from constants (`lui`+`addiu`/`ori`, `li`), else None."""
    val = 0
    seen = False
    for i in web.defs:
        w = insns[i].word
        op = w >> 26
        if op == 15:
            val = (w & 0xFFFF) << 16
            seen = True
        elif op in (9, 13) and (w >> RS) & 31 in (0, (w >> RT) & 31):
            low = _sext16(w) if op == 9 else (w & 0xFFFF)
            val = (val + low) & 0xFFFFFFFF if (w >> RS) & 31 else low & 0xFFFFFFFF
            seen = True
        else:
            return None
    return val if seen else None


def web_text(web, insns):
    """What a web holds, from its defining instruction."""
    const = web_constant(web, insns)
    if const is not None:
        return "constant 0x%X (built by `%s`)" % (const, insns[web.defs[0]].text.split("; ", 1)[-1])
    if web.defs:
        return "value computed by `%s`" % insns[web.defs[0]].text.split("; ", 1)[-1]
    return "value live on entry"


def describe_range(r, proc, names, web=None, insns=None):
    """(what, lines): a short description of what a live range holds, from the expression table."""
    row = proc.table.get(r.bit)
    if row is None or (row["kind"] == "op" and web is not None and web_constant(web, insns) is not None):
        if web is not None and insns is not None:
            return "new temporary: " + web_text(web, insns), []
        return ("new temporary (bit %d is past the expression table: hoisted constant, address "
                "or strength-reduced value)" % r.bit), []
    if row["kind"] == "var":
        key = (row["cls"], row["l"], row["r"])
        kind = {"M": "local", "P": "parameter", "S": "global", "R": "return value"}.get(row["cls"], row["cls"])
        name = names.get(key, ("?", []))
        return "%s %s" % (kind, name[0] if name[0] != "?" else "%s%d%+d" % key), name[1]
    return "expression %s" % render(proc, names, row["id"]), []


def occupants(webs, reg, lo, hi, skip=None):
    """Webs of `reg` alive somewhere inside the instruction interval (lo, hi): the values that sit in a
    register while another value is held elsewhere."""
    return [w for w in webs if w.reg == reg and w is not skip and w.first < hi and w.last > lo]


def scratch_sequence(insns, limit=12):
    """Registers of ugen's temporaries (t6..t9) in the order the code defines them."""
    seq = []
    for d in insns:
        for r in d.defs:
            if r in (14, 15, 24, 25):
                seq.append(REG_NAMES[r])
    return seq[:limit]


def explain(insns, pairs, webs, ranges, proc, names, target=None):
    """Lines of text for the first register divergence. target: (insns, webs) of the original."""
    out = []
    first = first_divergence(pairs)
    if first is None:
        return out
    pair, op = first
    shift, role, dreg, treg = op
    d = insns[pair.draft_index]
    assign = assign_ranges(ranges, webs)
    webs_of = {id(r): w for w in webs for (r, _e) in [assign.get(id(w), (None, 0))] if r is not None}
    web = web_at(webs, pair.draft_index, dreg, role)
    got = assign.get(id(web)) if web is not None else None
    out.append("draft puts %s in %s, the original in %s (%s of `%s`)."
               % (web_text(web, insns) if web is not None else "the value", rname(dreg), rname(treg),
                  "result" if role == "def" else "operand", d.text.split("; ", 1)[-1]))
    if dreg not in REG_COLOUR or dreg in UGEN_SCRATCH:
        if target is not None:
            out.append("ugen temporaries (t6..t9) in order of definition: draft %s; original %s"
                       % (" ".join(scratch_sequence(insns)) or "-", " ".join(scratch_sequence(target[0])) or "-"))
        out.append("%s is not a uopt colour: ugen (or the assembler for a macro) picked it for an expression "
                   "temporary, so the allocator trace does not decide it. ugen hands out t6, t7, t8, t9 in turn; "
                   "a swapped pair means two temporaries are created or freed in the other order: look at the "
                   "expression shape and evaluation order (operand order, which sub-expressions are shared, what "
                   "stays in memory), starting at the earliest temporary of the function." % rname(dreg))
        return out
    if got is None:
        cands = candidate_ranges(ranges, dreg)
        if web is not None and cands:
            cands.sort(key=lambda r: (score_web(r, web), r.order if r.order is not None else 1 << 30))
            got = (cands[0], False)
        else:
            out.append("no live range of the trace holds %s: ugen temporary, or a range the trace splits."
                       % rname(dreg))
            return out
    w_range, exact = got
    what, lines = describe_range(w_range, proc, names, web, insns)
    holding = len(candidate_ranges(ranges, dreg))
    conf = ("the only range in %s" % rname(dreg) if holding == 1 else
            "def/use counts agree" if exact else "best guess, %d ranges hold %s" % (holding, rname(dreg)))
    out.append("draft range: %s, %s%s" % (w_range.label(), what,
                                          " (lines %s)" % ",".join(map(str, lines[:6])) if lines else ""))
    out.append("  priority %s = %d use(s) + %d def(s) over %d unit(s), loop weight %s, denominator %d; "
               "%s, allocated %s, cheapest colour cost %s, got colour %s [%s]"
               % (_fmt(w_range.priority), w_range.loads, w_range.stores, len(w_range.units),
                  "/".join(sorted({str(u["freq"]) for u in w_range.units}, key=int)) or "-", w_range.denominator,
                  w_range.mode or "?", _nth(w_range.order), _fmt(w_range.cost), cname(w_range.colour), conf))
    if w_range.split_from is not None:
        parent = next((r for r in proc.ranges.values() if r.lr == w_range.split_from), None)
        if parent is not None:
            out.append("  this is a piece of lr %d, which was split because the whole range (priority %s over %d "
                       "units, cheapest colour cost %s) did not pay for a register across a call or a block"
                       % (parent.lr, _fmt(parent.priority), len(parent.units), _fmt(parent.cost)))
    # the competitor: the range of the mirror difference (the original's register in the draft), else
    # any range holding the original's register next to this one
    z = None
    mirror = [p for p in pairs for (_s, _r, a, b) in p.ops if a == treg and b == dreg and p is not pair]
    for p in mirror:
        for (s2, r2, a2, b2) in p.ops:
            if a2 == treg and b2 == dreg:
                w2 = web_at(webs, p.draft_index, a2, r2)
                g2 = assign.get(id(w2)) if w2 is not None else None
                if g2 and g2[0] is not w_range:
                    z = (g2[0], w2)
                    break
        if z:
            break
    if z is None:
        holders = [r for r in candidate_ranges(ranges, treg) if r is not w_range and overlap(r, w_range)]
        holders.sort(key=lambda r: r.order if r.order is not None else 1 << 30)
        z = (holders[0], None) if holders else None
    out.extend(hint_lines(w_range, dreg, treg, z, ranges, proc, names, insns, bool(mirror), webs_of))
    if target is not None and web is not None:
        tinsns, twebs = target
        tweb = web_at(twebs, pair.target_index, treg, role)
        if tweb is not None and treg in ARG_REGS and any(c >= 0 for c in tweb.calls) and not any(
                c >= 0 for c in web.calls):
            out.append("the original passes this value to the call at its insn %d in %s: uopt precolours a value "
                       "that is an argument to that argument register, which makes it the cheapest colour. The "
                       "draft does not pass the same value straight to a call (a cast, a copy or another "
                       "expression sits in between)." % (min(c for c in tweb.calls if c >= 0), rname(treg)))
    if target is not None and not mirror and web is not None:
        tinsns, twebs = target
        tweb = web_at(twebs, pair.target_index, treg, role)
        if tweb is not None:
            for ow in occupants(twebs, dreg, tweb.first, tweb.last)[:2]:
                out.append("in the original %s is held by %s (insns %d-%d) while this value sits in %s; "
                           "the draft has no such value alive there." % (
                               rname(dreg), web_text(ow, tinsns), ow.first, ow.last, rname(treg)))
        for ow in occupants(webs, treg, web.first, web.last, skip=web)[:2]:
            g = assign.get(id(ow))
            out.append("in the draft %s is held by %s (insns %d-%d)%s while this value sits in %s; the original "
                       "does not have it there." % (
                           rname(treg), web_text(ow, insns), ow.first, ow.last,
                           (", range %s priority %s" % (g[0].label(), _fmt(g[0].priority))) if g else "",
                           rname(dreg)))
    if target is not None:
        out.extend(constant_table(insns, webs, target[0], target[1]))
    return out


def constants_by_value(insns, webs):
    """{value: [register, ...]} of the constants the code builds in registers, in program order."""
    out = {}
    for w in sorted(webs, key=lambda w: w.first):
        if w.defs and (w.uses or w.calls):
            v = web_constant(w, insns)
            if v is not None:
                out.setdefault(v, []).append(w.reg)
    return out


def constant_table(insns, webs, tinsns, twebs, limit=8):
    """Lines listing the constants that sit in other registers in the draft than in the original."""
    mine, theirs = constants_by_value(insns, webs), constants_by_value(tinsns, twebs)
    rows = []
    for v, regs in mine.items():
        want = theirs.get(v)
        if want is None:
            rows.append("0x%X: draft %s, the original does not keep it in a register" % (v, " ".join(map(rname, regs))))
        elif regs != want:
            rows.append("0x%X: draft %s, original %s" % (v, " ".join(map(rname, regs)), " ".join(map(rname, want))))
    for v, regs in theirs.items():
        if v not in mine:
            rows.append("0x%X: the draft does not keep it in a register, original %s" % (v, " ".join(map(rname, regs))))
    if not rows:
        return []
    return ["constants kept in registers that differ (%d): " % len(rows) + "; ".join(rows[:limit])
            + (" ..." if len(rows) > limit else "")]


def hoisted_sequence(ranges, webs_of, insns, limit=10):
    """['bit 32 0x800CA22C -> $t0', ...] for the temporaries past the expression table, in bit order."""
    out = []
    for r in sorted(ranges, key=lambda r: r.bit):
        w = webs_of.get(id(r)) if webs_of else None
        if w is None or r.colour is None:
            continue
        v = web_constant(w, insns)
        if v is not None:
            out.append("%d 0x%X -> %s" % (r.bit, v, rname(r.reg)))
    return out[:limit]


def _fmt(x):
    return ("%.4g" % x) if x is not None else "?"


def _nth(order):
    if order is None:
        return "-"
    return "#%d" % (order + 1)


def hint_lines(w, dreg, treg, z, ranges, proc, names, insns, swapped, webs_of=None):
    """The advice for range `w` (draft register dreg, original register treg); z = (range, web) is the
    range that holds the original's register in the draft."""
    out = []
    dc, tc = REG_COLOUR.get(dreg), REG_COLOUR.get(treg)
    if tc is None or treg in UGEN_SCRATCH:
        out.append("hint: the original's %s is not a uopt colour (ugen temporary): an expression shape or "
                   "order difference, not allocator priority." % rname(treg))
        return out
    if z is not None:
        zr, zweb = z
        zwhat, zlines = describe_range(zr, proc, names, zweb, insns)
        out.append("%s in the draft: %s, %s%s" % (
            "the range that holds %s where the original puts the other one" % rname(treg) if swapped else
            "range holding %s next to it" % rname(treg), zr.label(), zwhat,
            " (lines %s)" % ",".join(map(str, zlines[:6])) if zlines else ""))
        out.append("  priority %s = %d use(s) + %d def(s), %s, allocated %s, cheapest colour cost %s, got colour %s"
                   % (_fmt(zr.priority), zr.loads, zr.stores, zr.mode or "?", _nth(zr.order), _fmt(zr.cost),
                      cname(zr.colour)))
        zp, wp, zo, wo = zr.priority, w.priority, zr.order, w.order
        if zo is not None and wo is not None:
            first = w if wo < zo else zr
            if zp == wp:
                lo, hi = (w, zr) if w.bit < zr.bit else (zr, w)
                hoisted = lo.bit > proc.max_table_bit and hi.bit > proc.max_table_bit
                how = ("bits past the expression table (hoisted constants and addresses) are numbered in the order "
                       "the code first uses them, and the address of the left side of an assignment is used before "
                       "its right side (`a[i] = b[i]` uses a first, `v = b[i]; a[i] = v;` uses b first)"
                       if hoisted else
                       "bit positions follow the order the ucode first mentions the values, i.e. the evaluation "
                       "order of the expressions")
                if swapped:
                    out.append("hint: tie. Equal priority %s; %s is coloured before %s, so it takes the lower colour. "
                               "The original has them the other way round: change the order of first use. %s%s." % (
                                   _fmt(zp), lo.label(), hi.label(), how[0].upper() + how[1:],
                                   "" if hoisted else ", or give the one the original colours first one more use"))
                else:
                    out.append("hint: equal priorities (%s): the lower bit position is coloured first. Make the "
                               "value the original colours first the first one used. %s." % (
                                   _fmt(zp), how[0].upper() + how[1:]))
                if hoisted:
                    seq = hoisted_sequence(ranges, webs_of, insns)
                    if seq:
                        out.append("hoisted temporaries in bit order: " + "; ".join(seq))
            elif first is w:
                out.append("hint: %s (priority %s) is coloured before %s (priority %s) and takes the lower colour. "
                           "The original has the order the other way: raise %s above %s (one more use; a use inside "
                           "a loop weighs 10x), or lower %s below %s (fewer uses, a use moved out of the loop)."
                           % (w.label(), _fmt(wp), zr.label(), _fmt(zp), zr.label(), _fmt(wp), w.label(), _fmt(zp)))
            else:
                out.append("hint: %s (priority %s) outranks %s (priority %s). The original needs %s first: reduce %s's "
                           "uses or loop weight below %s, or raise %s's above %s."
                           % (zr.label(), _fmt(zp), w.label(), _fmt(wp), w.label(), zr.label(), _fmt(wp), w.label(), _fmt(zp)))
            if (w.mode == "unconstrained" or zr.mode == "unconstrained") and zp != wp:
                out.append("note: unconstrained ranges are coloured in bit order, not by priority "
                           "(wiki/uopt-register-allocation.md, range order).")
    else:
        lower = tc < dc
        out.append("no live range holding %s overlaps it in the draft." % rname(treg))
        if lower:
            out.append("hint: colours are tried lowest first, so an interfering range holds the colour(s) below %s "
                       "here that the original does not have: an extra live range alive at the same time in the "
                       "draft (a constant, address or copy). Remove it or share it with another use." % cname(dc))
        else:
            out.append("hint: the draft took the lower colour %s; in the original an interfering range holds it: "
                       "look for a value the original keeps in a register there (a constant or pointer that is "
                       "separate or lives longer)." % cname(dc))
    def isvar(r):
        row = proc.table.get(r.bit)
        return row is not None and row["kind"] == "var"

    unalloc = [r for r in ranges if r.colour is None and r is not w and overlap(r, w)
               and (r.loads + r.stores >= 2 or isvar(r))]
    for r in unalloc[:2]:
        what, _l = describe_range(r, proc, names)
        reloads = sum(1 for u in r.units if u.get("load"))
        if r.priority is not None and r.priority == 0:
            why = ("net zero: %d use(s) and %d def(s) are paid back by %d reload/save charge(s)"
                   % (r.loads, r.stores, reloads + sum(1 for u in r.units if u.get("save"))))
        else:
            why = "priority %s, %s" % (_fmt(r.priority), r.reason or "no profit")
        out.append("not allocated in the draft: %s, %s (%s). If the original keeps it in a register, make it "
                   "profitable: more uses, a store plus uses, no call between them." % (r.label(), what, why))
    return out


# ---------------------------------------------------------------------------------------------
# the draft compile

WRAPPER = '''#!/usr/bin/env python3
"""Saves uopt's input ucode, then runs the instrumented uopt (generated by tools/regdiff.py)."""
import os, shutil, sys
sys.path.insert(0, %(tools)r)
import cvt_pass
argv = sys.argv[1:]
files, i = [], 0
while i < len(argv):
    if argv[i] in cvt_pass.VALUED:
        i += 2
    elif argv[i].startswith("-"):
        i += 1
    else:
        files.append(argv[i])
        i += 1
shutil.copy(files[0], os.environ["REGDIFF_UCODE"])
real = os.environ["REGDIFF_REAL_UOPT"]
os.execv(real, [real] + argv)
'''


class Compiled:
    pass


def compile_draft(root, func, src, uopt, workdir, unit=None, protos=()):
    """Compile the complete draft TU with traced uopt; retain file-scope ownership/aliasing.
    `protos` (protoverride.Proto) replace the declarations of those functions in the unit (T-16530)."""
    from funcdiff_verify import preprocess_draft
    workdir = os.path.abspath(workdir)
    os.makedirs(workdir, exist_ok=True)
    text = preprocess_draft(src)
    notes = []
    if protos:
        text, notes = protoverride.apply(text, protos)
    base = os.path.join(workdir, "draft.c")
    with open(base, "w") as f:
        f.write(text)
    wrapper = os.path.join(workdir, "uopt-wrap.py")
    with open(wrapper, "w") as f:
        f.write(WRAPPER % {"tools": os.path.dirname(os.path.abspath(__file__))})
    os.chmod(wrapper, 0o755)
    obj = os.path.join(workdir, "draft.o")
    ucode = os.path.join(workdir, "draft.B")
    log = os.path.join(workdir, "draft.log")
    for p in (obj, ucode, obj + ".trace"):
        if os.path.exists(p):
            os.unlink(p)
    env = dict(os.environ, REGDIFF_UCODE=ucode, REGDIFF_REAL_UOPT=os.path.abspath(uopt))
    cmd = [sys.executable, os.path.join(os.path.dirname(os.path.abspath(__file__)), "uopt_eval.py"),
           "compile", base, obj, "--uopt", wrapper, "--trace"]
    with open(log, "w") as lf:
        rc = subprocess.run(cmd, stdout=lf, stderr=subprocess.STDOUT, env=env, cwd=root).returncode
    c = Compiled()
    c.obj, c.base, c.base_text, c.log_path = obj, base, text, log
    c.protos, c.proto_notes = list(protos), notes
    c.returncode = rc
    c.log = _read(log, "r")
    c.listing = _read(obj + ".trace", "r")
    c.ucode = _read(ucode, "rb")
    return c


def _read(path, mode):
    """The contents of `path`, or an empty value when it does not exist."""
    if not os.path.exists(path):
        return b"" if "b" in mode else ""
    with open(path, mode, **({} if "b" in mode else {"errors": "replace"})) as f:
        return f.read()


def source_offset(base_text, src_text, func):
    """Add this to a line of draft.c to get the line in the source file: the function starts at
    different lines in the two files (the preprocessor removed lines before it)."""
    import permute
    m1 = permute.def_regex(func).search(base_text)
    m2 = permute.def_regex(func).search(src_text)
    if not m1 or not m2:
        return 0
    return src_text.count("\n", 0, m2.start()) - base_text.count("\n", 0, m1.start())


# ---------------------------------------------------------------------------------------------
# main

def load_ucode_parser():
    import cvt_pass
    return cvt_pass.parse, cvt_pass.OPS


def resolved(obj, name, original_file=None):
    import funcdiff
    got = funcdiff.resolved_functions(obj, [name], original_file)
    return got[name]


def run(args, out=print):
    import funcdiff
    import funcloc
    t0 = time.time()
    root = os.getcwd()
    try:
        name, scope = funcloc.split_scope(args.func, args.unit)
        c = funcloc.locate(name, scope, defined_only=False)
    except funcloc.LocateError as e:
        out("regdiff: %s" % e)
        return 2
    src = args.src or str(c.src)
    try:
        protos = protoverride.parse(getattr(args, "proto", None))
    except protoverride.ProtoError as e:
        out("regdiff: %s" % e)
        return 2
    uopt, problem = find_uopt(args.uopt)
    if problem:
        out(problem)
        return 2
    expected, problem = funcdiff.expected_object(c)
    if problem:
        out("regdiff: %s" % problem)
        return 2
    t1 = time.time()
    try:
        comp = compile_draft(root, name, src, uopt, os.path.join(args.scratch, name), protos=protos)
    except (SystemExit, OSError, ValueError) as e:
        out("regdiff: %s" % e)
        return 2
    t2 = time.time()
    if comp.returncode:
        out("regdiff: the draft does not compile:\n" + "\n".join(comp.log.strip().splitlines()[-12:]))
        return 2
    got = resolved(comp.obj, name)
    want = resolved(expected, name, c)
    if got is None or want is None:
        out("regdiff: %s is missing in the %s object" % (name, "draft" if got is None else "original"))
        return 2
    return report(args, name, c, src, comp, got, want, (t0, t1, t2), out)


def strip_padding(entries):
    """The instructions of a function without its trailing alignment words (nops, `...`): the draft is
    compiled outside the unit, so the original's trailing words are not put back (tools/trailing_pad.py)."""
    entries = list(entries)
    while entries and (entries[-1][0] == "..." or entries[-1][0].startswith("00000000")):
        entries.pop()
    return entries


def text_of(entry):
    return entry[1].split("; ", 1)[-1]


def report(args, name, c, src, comp, got, want, times, out):
    got, want = strip_padding(got), strip_padding(want)
    dkeys, tkeys = [k for k, _d in got], [k for k, _d in want]
    out("regdiff %s [%s]  draft: %s%s" % (name, c.unit, os.path.relpath(src),
                                          "" if args.src else " (the NON_MATCHING variant if the function is guarded)"))
    for note in getattr(comp, "proto_notes", ()):
        out("  " + note)
    out("  %d draft / %d original instructions" % (len(dkeys), len(tkeys)))
    if dkeys == tkeys:
        out("  MATCH: identical instructions, no divergence.")
        if getattr(comp, "protos", None):
            out("  (with the prototype override above; nothing on disk changed)")
            for line in protoverride.commit_hint(comp.protos):
                out("  " + line)
        _timing(out, times)
        return 0
    pairs, structural, left = align(dkeys, tkeys)
    if structural:
        op, (i1, i2), (j1, j2) = structural[0]
        out("  STRUCTURE differs in %d place(s); first at draft insn %d / original insn %d:" % (len(structural), i1, j1))
        for k in range(i1, min(i2, i1 + 3)):
            out("    - draft    %s" % text_of(got[k]))
        for k in range(j1, min(j2, j1 + 3)):
            out("    + original %s" % text_of(want[k]))
        out("  Fix the instruction differences first (tools/funcdiff.py --resolve shows them all); regdiff "
            "explains the registers of the instructions that agree.")
    for p in left[:3]:
        out("  constant differs at draft insn %d: `%s` against `%s`" % (
            p.draft_index, text_of(got[p.draft_index]), text_of(want[p.target_index])))
    swaps = [p for p in pairs if p.swap]
    for p in swaps[:2]:
        out("  operand order: draft `%s` against original `%s` (same values, the commutative operands are "
            "exchanged; this is the C operand order, not the allocation)" % (
                text_of(got[p.draft_index]), text_of(want[p.target_index])))
    if len(swaps) > 2:
        out("  ... and %d more such instructions" % (len(swaps) - 2))
    if first_divergence(pairs) is None:
        if not structural and not left and not swaps:
            out("  no register difference found")
        elif swaps and not structural and not left:
            out("  hint: write the operands of the comparison or sum the other way round (`a == b` as `b == a`); "
                "uopt itself orders `==` and `!=` with a plain variable first, so a value copied into a local first "
                "can change which side it lands on.")
        _timing(out, times)
        return 1
    out("  %d instruction(s) differ in registers%s" % (len(pairs) - len(swaps), " only" if not structural and not left else ""))
    rm = register_map(pairs)
    out("  register map (draft -> original): " + ", ".join(
        "%s->%s x%d" % (rname(a), rname(b), n) for (a, b), n in sorted(rm.items(), key=lambda kv: (-kv[1], kv[0]))[:10]))
    procs = parse_listing(comp.listing, parse_stderr(comp.log))
    proc = procs.get(name)
    if proc is None or not proc.ranges:
        out("  (no allocator trace for %s: is the uopt the instrumented one?)" % name)
        _timing(out, times)
        return 1
    parse, ops = load_ucode_parser()
    try:
        refs = ucode_refs(comp.ucode, parse, ops) if comp.ucode else {}
    except Exception:      # the ucode only gives names
        refs = {}
    import permute
    base_lines = comp.base_text.split("\n")
    m = permute.def_regex(name).search(comp.base_text)
    start = comp.base_text.count("\n", 0, m.start()) + 1 if m else 1
    with open(src) as f:
        shift = source_offset(comp.base_text, f.read(), name)
    names = name_variables(proc, refs, base_lines, start)
    names = {k: (n, [ln + shift for ln in lines]) for k, (n, lines) in names.items()}
    ranges = final_ranges(proc)
    insns = [Decoded(key_word(k), d) for k, d in got]
    webs = build_webs(insns)
    tinsns = [Decoded(key_word(k), d) for k, d in want]
    twebs = build_webs(tinsns)
    p, op = first_divergence(pairs)
    out("")
    out("FIRST DIVERGENCE at draft insn %d" % p.draft_index)
    out("  draft    %s" % text_of(got[p.draft_index]))
    out("  original %s" % text_of(want[p.target_index]))
    for line in explain(insns, pairs, webs, ranges, proc, names, (tinsns, twebs)):
        out("  " + line)
    if args.verbose:
        out("")
        out("all ranges (allocation order), webs matched by register and def/use counts:")
        assign = assign_ranges(ranges, webs)
        by_range = {id(r): w for w in webs for (r, _e) in [assign.get(id(w), (None, 0))] if r is not None}
        for r in sorted(ranges, key=lambda r: (r.order if r.order is not None else 1 << 30, r.bit)):
            w = by_range.get(id(r))
            what, lines = describe_range(r, proc, names, w, insns)
            out("  %-3s %-12s prio %-7s %-13s uses %d defs %d  %s %s  %s%s"
                % (_nth(r.order), r.label(), _fmt(r.priority), r.mode or "-", r.loads, r.stores,
                   "-> " + rname(r.reg) if r.reg else "uncoloured", "" if r.reg else "(%s)" % (r.reason or "?"),
                   what, " lines %s" % lines[:5] if lines else ""))
    _timing(out, times)
    return 1


def _timing(out, times):
    t0, t1, t2 = times
    out("  [lookup %.1fs, traced compile %.1fs, analysis %.1fs]" % (t1 - t0, t2 - t1, time.time() - t2))


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("func", help="function name, optionally UNIT:func")
    ap.add_argument("--unit", help="unit (main or an overlay name) or C path that holds the function")
    ap.add_argument("--src", help="C file with the draft (default: the function's source file)")
    ap.add_argument("--uopt", help="instrumented uopt (default $REGDIFF_UOPT or %s)" % DEFAULT_UOPT)
    protoverride.add_argument(ap)
    ap.add_argument("--scratch", default=SCRATCH, help="work directory (default %(default)s)")
    ap.add_argument("-v", "--verbose", action="store_true", help="also list every live range")
    args = ap.parse_args(argv)
    if shutil.which("mips-linux-gnu-objdump") is None:
        sys.exit("regdiff.py: mips-linux-gnu-objdump not found. The tools only run in Docker: "
                 "tools/docker.sh python3 tools/regdiff.py ...")
    return run(args)


if __name__ == "__main__":
    sys.exit(main())
