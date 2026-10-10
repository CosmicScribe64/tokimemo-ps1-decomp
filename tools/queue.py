#!/usr/bin/env python3
"""Ranked work list of the remaining INCLUDE_ASM functions, with a blocker detector (T-1320, T-3340).

Covers the main exe (src/main/<addr>.c) and the 26 overlays (src/ovl/<NAME>.c). For every function
still INCLUDE_ASM it reads the generated splat asm (asm/nonmatchings/...) and reports size, leaf or
not, number of calls, and flags:

  L  has a loop (a backward branch)
  J  has a jump table (jtbl_*)
  S  references a string literal in .rodata
     J and S block only when the function's C file has no rodata island: then its rodata is an
     asm blob the C object cannot provide. Per-object C files (tools/split_objects.py, T-0500)
     give every object with rodata an island, so J and S are workable there (decompile-workflow).
  P  one trailing nop after the last jr (end address 12 mod 16): asm-processor needs 2 (matching-notes)
  R  T-0018 register promotion of an UNSIGNED narrow global (lbu/lhu): loaded in two or more basic
     blocks outside loops into the same register (reloaded after calls or branches). Cause (T-1321):
     cfe widens every unsigned 8/16-bit load with a CVT that uopt treats as another expression, so the
     global is not promoted; tools/cvt_pass.py models it but is not in the build. Signed and word
     globals (lb/lh/lw) are not flagged: IDO 5.3 already keeps them in a register (matching-notes).
  V  T-0018 register choice, same family: the first load of an unsigned narrow global goes to $v1
     while $v0 is dead and the value is read twice or more (old value of `D++` and a compare), outside
     a switch/compare chain.
  T  not a blocker, a hint: the V shape (global loaded into $v1, $v0 dead, read twice) on a signed or
     word global (lw/lh/lb). T-1321: a `lw` global switched or compared in $v1 matches when declared
     u32 (u16 for lh): try the unsigned type before anything else.
  U  blocked, cause unknown: a switch or compare chain at the start of the function on an unsigned
     narrow global. The original keeps the selector in $v1 (U1) or in $v0 (U0). The C of both is the
     same (`switch (D)` on an extern u8, T-1321): the property that decides is the variable's, not
     something the C encodes, so the queue does not guess a cause or a fix and marks both
     blocked-unknown (they stay out of --next and --plan). What the asm adds, measured on this tree
     (`--calibrate`): no matched function has a U1 selector, 48 matched functions have a U0 selector, so
     U1 needs cvt_pass.py (not in the build) and U0 usually matches with the natural C. One natural
     attempt on a U0 function is cheap (`--include-unknown u0`); record a failure in
     wiki/data/t0018-cases.md. `--include-unknown` (all) lets U1 through too.

R, V and U are heuristics calibrated against wiki/data/t0018-cases.md and the matched functions
(`--calibrate`, results in wiki/matching-notes.md; `--calibrate --legacy` scores the T-1320 rule,
which flagged every global load). J, S and P are exact.

Note: the file name shadows the standard library `queue` when tools/ is first on sys.path (running
scripts from tools/); no tool here imports the standard one. Other tools load this file by path
(tools/list_leaves.py, tools/test_queue.py) under the name `work_queue`.

Usage (inside Docker, from the repo root after `ninja` has generated asm/):
  python3 tools/queue.py                        full table, workable functions first
  python3 tools/queue.py --files 80041000,TEL   only these files (main address stems / overlay names)
  python3 tools/queue.py --next 20 --files TEL  the 20 best workable functions, for agents
  python3 tools/queue.py --by bytes --next 20   rank by expected bytes gained per effort (T-3340)
  python3 tools/queue.py --by bytes --plan 60 --agents 4
                                                split the next 60 best functions into 4 work lists, no C file
                                                (`--exclusive unit`: no overlay) shared between lists
  python3 tools/queue.py --blocked              only functions with a blocker flag (R V U J S P)
  python3 tools/queue.py --summary              counts per file and per flag
  python3 tools/queue.py --calibrate            detector precision/recall
  python3 tools/queue.py --no-cache ...         ignore build/queue-cache.json (see below)
Ranking: unblocked first, then leaf before non-leaf, then size ascending, then file and name.

Run time and the cache (T-5030): reading and analysing the 8000 asm files, then fingerprinting them
for the duplicate groups (--by bytes, --plan), took 10 to 25 s on an idle machine and minutes when
several agents were building (every file is read through the Docker bind mount); a `--by bytes` run
over a whole file list was cut off by the Docker daemon after more than five minutes ("error waiting
for container: unexpected EOF"). Nothing else was wrong: output is a few KB and the peak memory about
80 MB. Now the loaded functions and the groups are cached in build/queue-cache.json, keyed on the
size and mtime of every asm, source and config file and of queue.py, dupes.py and neardupes.py, so a
repeat run takes about a second and any edit or regenerated asm invalidates it (--no-cache ignores it).
Without --by bytes / --plan, --files also limits what is read to those files. Progress lines go to
stderr, so a long first run is not silent.

Byte-weighted ranking (--by bytes): score = expected bytes / effort.
  expected bytes = size * P(match) * (1 + credit), credit = 0.9 per byte-identical twin and 0.6 per near
                   duplicate (same shape, other constants) among the k - 1 other unmatched members
  P(match)       = 0.85 - 0.04 * min(calls, 6) - 0.05 if loop; times 0.8 (J), 0.85 (S) with a rodata island;
                   0.25 (R, V), 0.7 (U0), 0.1 (U1), 0 (P, or J/S without an island)
  effort         = 12 + (size / 4) ** 1.25 (+ 10% per extra group member: the copy is cheap, not free)
A function in a dupes.py / neardupes.py group that already has a matched member is not work: matching it
is `dupes.py --apply` / `neardupes.py --apply --check` (score 0, dup column `apply`, listed after the work).
In a group without a matched member only the cheapest member is listed (`=rep` marks the others): matching
it unlocks the rest. Groups come from dupes.py / neardupes.py in-process (a few seconds), from a cached
file (`--groups FILE`, written by `--save-groups FILE`), or, as before, from `--dupes FILE` name lists.
"""
import argparse
import hashlib
import json
import os
import re
import sys
from pathlib import Path
from typing import NamedTuple

import srcscan

BLOCKERS = "JSPRVU"
BRANCHES = {"b", "beq", "bne", "beqz", "bnez", "bgez", "bgtz", "blez", "bltz", "bgezal", "bltzal",
            "beql", "bnel", "beqzl", "bnezl", "bgezl", "bgtzl", "blezl", "bltzl"}
CALLS = {"jal", "jalr"}
JUMPS = {"j", "jr"}
INSN_RE = re.compile(r'^\s*/\*\s*\w+\s+([0-9A-Fa-f]{8})\s+[0-9A-Fa-f]{8}\s*\*/\s+(\w+)\s*(.*?)\s*$')
LABEL_RE = re.compile(r'^\s*\.(L[0-9A-Fa-f]{8}):')
LOAD_RE = re.compile(r'^\$(\w+),\s*%lo\((\w+)(\+0x[0-9A-Fa-f]+)?\)\(\$(\w+)\)$')
ARGREGS = {"a0", "a1", "a2", "a3"}
STORES = {"sb", "sh", "sw", "swl", "swr"}
LOADS = {"lb", "lbu", "lh", "lhu", "lw"}
UNSIGNED = {"lbu", "lhu"}                       # the loads cfe widens with a CVT (T-1321)
COMPARES = BRANCHES | {"slt", "sltu", "slti", "sltiu"}
SELECTOR_WINDOW = 12                            # a switch or chain starts within the first instructions
HI_RE = re.compile(r'%hi\((\w+)')


class Insn(NamedTuple):
    addr: int
    op: str
    args: str


class Facts(NamedTuple):
    size: int
    calls: int
    loop: bool
    jtbl: bool
    strings: bool
    pad: bool
    reload: bool     # flag R
    dispatch: bool   # flag V
    selector: str = ""   # flag U: register of the switch / compare-chain selector ("v0", "v1", ...)
    hint: bool = False   # flag T: V shape on a signed or word global: try declaring it unsigned


def parse_asm(text):
    """(instructions, label address set) of one splat .s function."""
    insns, labels = [], set()
    for line in text.splitlines():
        m = INSN_RE.match(line)
        if m:
            insns.append(Insn(int(m.group(1), 16), m.group(2), m.group(3)))
            continue
        m = LABEL_RE.match(line)
        if m:
            labels.add(int(m.group(1)[1:], 16))
    return insns, labels


def branch_target(insn):
    m = re.search(r'\.L([0-9A-Fa-f]{8})\s*$', insn.args)
    return int(m.group(1), 16) if m else None


def regs_of(args):
    return re.findall(r'\$(\w+)', args)


def v0_dead_after(insns, k):
    """True if the next access of $v0 after insns[k] is a write (or there is none)."""
    for i in insns[k + 1:]:
        regs = regs_of(i.args)
        if i.op in CALLS:
            return True   # the callee writes $v0
        if "v0" not in regs:
            continue
        if i.op in STORES or i.op in BRANCHES | JUMPS:
            return False
        return regs[0] == "v0" and "v0" not in regs[1:]
    return True


def reads_before_redef(insns, k, reg):
    """Number of instructions after insns[k] that read `reg`, until it is overwritten."""
    n = 0
    for i in insns[k + 1:]:
        regs = regs_of(i.args)
        if i.op in STORES or i.op in BRANCHES | JUMPS:
            n += reg in regs
            continue
        n += reg in regs[1:]
        if regs[:1] == [reg]:
            break
    return n


def compare_reads(insns, k, reg, sym):
    """Number of compare-class reads (branch, slt*) of `reg` after insns[k] in the rest of the
    function, until another value is put into the register. A compare chain is laid out so that the
    first case falls through into its calls, so calls do not end the count; loading the same global
    again (`lui`/`lbu` of `sym` into `reg`) does not either."""
    n = 0
    for i in insns[k + 1:]:
        regs = regs_of(i.args)
        if i.op in COMPARES:
            n += reg in (regs if i.op in BRANCHES else regs[1:])
        elif i.op not in STORES and i.op not in JUMPS and regs[:1] == [reg] and sym.split("+")[0] not in i.args:
            break
    return n


def analyze(text, string_syms=frozenset(), legacy=False):
    """Facts about one function's asm text. string_syms: names of .rodata string symbols.
    legacy: the T-1320 rule for R and V (every global load counts, signed or not, no U)."""
    insns, labels = parse_asm(text)
    calls = sum(1 for i in insns if i.op in CALLS)
    loops = []   # (first addr, last addr) of every backward branch
    for i in insns:
        t = branch_target(i)
        if i.op in BRANCHES and t is not None and t <= i.addr:
            loops.append((t, i.addr))
    jtbl = any("%hi(jtbl_" in i.args or "%lo(jtbl_" in i.args for i in insns)
    strings = any(m in string_syms for i in insns for m in HI_RE.findall(i.args))
    pad = len(insns) >= 4 and insns[-1].op == "nop" and insns[-3].op == "jr" and insns[-3].args == "$ra"
    # basic blocks: a label starts one; so does the instruction after a branch/jump/call delay slot
    block, after_delay, pending = 0, False, False
    luireg = {}
    loads = []   # (block, dest reg, symbol, in loop, instruction index)
    for k, i in enumerate(insns):
        if i.addr in labels or after_delay:
            block += 1
            after_delay = False
        if pending:
            after_delay, pending = True, False
        if i.op in BRANCHES | CALLS | JUMPS:
            pending = True
        if i.op in LOADS:
            m = LOAD_RE.match(i.args)
            if m and luireg.get(m.group(4)) == m.group(2):
                inloop = any(lo <= i.addr <= hi for lo, hi in loops)
                loads.append((block, m.group(1), m.group(2) + (m.group(3) or ""), inloop, k))
        if i.op == "lui":
            h = HI_RE.search(i.args)
            luireg[regs_of(i.args)[0]] = h.group(1) if h else None
        elif i.op not in STORES and i.op not in BRANCHES | JUMPS and regs_of(i.args)[:1]:
            luireg.pop(regs_of(i.args)[0], None)
    # T-1321: the gap is the CVT on unsigned narrow loads; signed and word globals are promoted by IDO
    cand = loads if legacy else [l for l in loads if insns[l[4]].op in UNSIGNED]
    # U: the function starts with a switch or compare chain on an unsigned narrow global
    selector = ""
    if not legacy and cand:
        b, reg, key, _loop, k = min(cand, key=lambda l: l[4])
        reads = compare_reads(insns, k, reg, key)
        if k < SELECTOR_WINDOW and reg not in ARGREGS and (reads >= 2 or (jtbl and reads >= 1)):
            selector = reg
    # R: one global, 2+ blocks outside loops, one destination register
    by = {}
    for b, reg, key, inloop, _ in cand:
        if not inloop and reg not in ARGREGS:
            by.setdefault((key, reg), set()).add(b)
    reload = any(len(bs) >= 2 for bs in by.values()) and not selector
    # V: a global load goes to $v1 although $v0 is dead there (IDO takes $v0 first) and the value is
    # used at least twice (old value of D++ and a compare). With an unsigned narrow load it is the
    # CVT gap (V); with lw/lh/lb it is a type hint (T): the original compares the word unsigned
    # (`lw` global switched in $v1 matches declared u32, matching-notes T-1321)
    dispatch = hint = False
    for _, reg, _, _, k in loads:
        if reg == "v1" and v0_dead_after(insns, k) and reads_before_redef(insns, k, "v1") >= 2:
            if legacy or insns[k].op in UNSIGNED:
                dispatch = True
            else:
                hint = True
    if selector:
        dispatch = hint = False
    return Facts(len(insns) * 4, calls, bool(loops), jtbl, strings, pad, reload, dispatch, selector, hint and not legacy)


def string_symbols(root):
    """Names of the .rodata symbols that hold a string (dlabel followed by .asciz)."""
    syms = set()
    files = list(Path(root, "asm").glob("data/**/*rodata*.s")) + list(Path(root, "asm", "ovl").glob("*/data/**/*rodata*.s"))
    for f in files:
        name = None
        for line in f.read_text(errors="replace").splitlines():
            if line.startswith("dlabel "):
                name = line.split()[1]
            elif ".asciz" in line or ".ascii" in line:
                if name:
                    syms.add(name)
    return syms


class Func(NamedTuple):
    file: str
    name: str
    path: str
    facts: Facts
    island: bool = False   # the C file has a rodata island: J and S do not block
    unit: str = ""         # "main" or the overlay name ("" in synthetic data: derived from `file`)

    @property
    def leaf(self):
        return self.facts.calls == 0

    @property
    def unit_name(self):
        return self.unit or (self.file.split("/")[0] if not re.match(r"^[0-9A-Fa-f]{8}$", self.file) else "main")

    @property
    def flags(self):
        f = self.facts
        sel = "U" + {"v0": "0", "v1": "1"}.get(f.selector, "x") if f.selector else ""
        return "".join(c for c, on in (("L", f.loop), ("J", f.jtbl), ("S", f.strings), ("P", f.pad),
                                       ("R", f.reload), ("V", f.dispatch), ("T", f.hint)) if on) + sel

    @property
    def blocked(self):
        stop = BLOCKERS.replace("J", "").replace("S", "") if self.island else BLOCKERS
        return any(c in stop for c in self.flags)

    @property
    def u0(self):
        """Blocked-unknown with the selector in $v0: 48 matched functions, 1 recorded failure."""
        return self.facts.selector == "v0"

    @property
    def unknown(self):
        """Blocked only by U (switch selector, cause unknown): `--include-unknown` lets it through."""
        stop = BLOCKERS.replace("J", "").replace("S", "") if self.island else BLOCKERS
        return self.blocked and not any(c in stop.replace("U", "") for c in self.flags)


def units(root):
    """[(file label, C source path, asm dir of the matched functions, has island, unit)] for every C
    file of the main exe and the overlays (srcscan.c_files: the `c` subsegments of the splat configs)."""
    out = []
    for cf in srcscan.c_files(root):
        out.append((cf.label, cf.src, cf.matchings, cf.island, cf.unit))
    return out


def note(msg):
    """Progress line on stderr (a long first run is not silent)."""
    print("queue.py: " + msg, file=sys.stderr, flush=True)


def file_wanted(label, files):
    """True when the file label matches one of the --files prefixes (all when `files` is empty)."""
    if not files:
        return True
    low = label.lower()
    return any(low == w.lower() or low.startswith(w.lower()) for w in files)


def load(root=".", string_syms=None, legacy=False, files=None):
    """(remaining Funcs, matched Funcs) for the whole project, or only for the C files whose label
    matches `files` (prefixes, as in select())."""
    if string_syms is None:
        string_syms = string_symbols(root)
    remaining, matched = [], []
    for label, c, mdir, island, unit in units(root):
        if not file_wanted(label, files):
            continue
        for e in srcscan.include_asm_entries(c):
            if Path(e.folder).name == "pad":
                continue
            p = Path(root) / e.folder / (e.name + ".s")
            if p.exists():
                remaining.append(Func(label, e.name, str(p), analyze(p.read_text(errors="replace"), string_syms, legacy),
                                      island, unit))
        if mdir.is_dir():
            defined = srcscan.defined_functions(c)
            for p in sorted(mdir.glob("*.s")):
                if p.stem in defined:
                    matched.append(Func(label, p.stem, str(p), analyze(p.read_text(errors="replace"), string_syms, legacy),
                                        island, unit))
    return remaining, matched


CACHE = Path("build") / "queue-cache.json"
CACHE_VERSION = 1
TOOL_FILES = ("queue.py", "dupes.py", "neardupes.py")


def tree_signature(root):
    """Hash of (path, size, mtime) of everything the loaded data depends on: the splat asm, the C
    sources, the configs and the code of the tools that compute it. Stats only; no file is read."""
    h = hashlib.sha1(("v%d" % CACHE_VERSION).encode())
    here = Path(__file__).resolve().parent
    for name in TOOL_FILES:
        try:
            st = (here / name).stat()
            h.update(("%s %d %d\n" % (name, st.st_size, st.st_mtime_ns)).encode())
        except OSError:
            h.update(name.encode())
    for base, exts in (("asm", (".s",)), ("src", (".c",)), ("config", (".yaml", ".txt"))):
        for dirpath, dirs, fnames in os.walk(Path(root) / base):
            dirs.sort()
            for f in sorted(fnames):
                if f.endswith(exts):
                    st = os.stat(os.path.join(dirpath, f))
                    h.update(("%s/%s %d %d\n" % (dirpath, f, st.st_size, st.st_mtime_ns)).encode())
    return h.hexdigest()


def func_to_json(f):
    return [f.file, f.name, f.path, list(f.facts), f.island, f.unit]


def func_from_json(row):
    file, name, path, facts, island, unit = row
    return Func(file, name, path, Facts(*facts), island, unit)


class Cache:
    """build/queue-cache.json: JSON payloads under a key, valid for one tree signature. Disabled
    (every get misses, put does nothing) with use_cache=False or without a build/ directory."""

    def __init__(self, root=".", use_cache=True):
        self.path = Path(root) / CACHE
        self.enabled = bool(use_cache) and self.path.parent.is_dir()
        self.sig = tree_signature(root) if self.enabled else None
        self.data = {}
        if self.enabled:
            try:
                data = json.loads(self.path.read_text())
                if isinstance(data, dict) and data.get("sig") == self.sig:
                    self.data = data
            except (OSError, ValueError):
                pass

    def get(self, key):
        return self.data.get(key)

    def put(self, key, value):
        if not self.enabled:
            return
        self.data[key] = value
        self.data["sig"] = self.sig
        try:
            tmp = self.path.with_suffix(".tmp%d" % os.getpid())
            tmp.write_text(json.dumps(self.data, separators=(",", ":")))
            os.replace(tmp, self.path)
        except OSError:
            pass


def load_cached(root=".", legacy=False, cache=None, files=None):
    """load() through the cache. A request limited to `files` is served from a full cache but does
    not write one."""
    cache = cache or Cache(root, False)
    key = "legacy" if legacy else "rules"
    hit = cache.get(key)
    if hit:
        rem = [func_from_json(r) for r in hit["remaining"]]
        mat = [func_from_json(r) for r in hit["matched"]]
        if files:
            rem = [f for f in rem if file_wanted(f.file, files)]
            mat = [f for f in mat if file_wanted(f.file, files)]
        return rem, mat
    note("reading the asm of the project%s" % (" (cached afterwards)" if cache.enabled and not files else ""))
    rem, mat = load(root, legacy=legacy, files=files)
    if not files:
        cache.put(key, {"remaining": [func_to_json(f) for f in rem], "matched": [func_to_json(f) for f in mat]})
    return rem, mat


def groups_cached(root, cache):
    """compute_groups() through the cache."""
    hit = cache.get("groups")
    if hit is not None:
        return [[tuple(m) for m in g] for g in hit]
    note("fingerprinting the functions for duplicate groups")
    groups = compute_groups(root)
    cache.put("groups", [[list(m) for m in g] for g in groups])
    return groups


def read_cases(path):
    """[(file, function, category, symptom)] from the rows of the T-0018 cases table."""
    rows = []
    p = Path(path)
    if not p.exists():
        return rows
    for line in p.read_text().splitlines():
        cells = [c.strip().strip("`") for c in line.strip().strip("|").split("|")]
        if line.startswith("|") and len(cells) >= 4 and re.match(r'^(func_[0-9A-Fa-f]{8}|\w+)$', cells[1]) \
                and cells[0].lower() != "file" and not set(cells[0]) <= set("-: "):
            rows.append((cells[0], cells[1], cells[2], cells[3]))
    return rows


def read_dupes(path):
    """{function: first member of its group} for the groups after the first member; {} if no file."""
    p = Path(path)
    if not p.exists():
        return {}
    first = {}
    for line in p.read_text(errors="replace").splitlines():
        names = re.findall(r'\b(?:func_[0-9A-Fa-f]{8}|[A-Za-z_]\w*)\b', line)
        names = [n for n in names if n.startswith("func_")] or []
        for n in names[1:]:
            first.setdefault(n, names[0])
    return first


def rank_key(f):
    return (f.blocked, not f.leaf, f.facts.size, f.file, f.name)


def select(funcs, files):
    if not files:
        return funcs
    want = [x.lower() for x in files]
    return [f for f in funcs if any(f.file.lower() == w or f.file.lower().startswith(w) for w in want)]


# ---------------------------------------------------------------------------------------------
# byte-weighted ranking (T-3340)

GAIN = dict(base=0.85, call=0.04, call_cap=6, loop=0.05, jtbl=0.8, strings=0.85, promo=0.25, u0=0.7, u1=0.1,
            effort0=12.0, exponent=1.25, group=0.9, near=0.6, copy=0.1)


def p_match(f):
    """Chance that a try on this function ends in a match (an estimate, not a measurement: see the
    constants in GAIN). 0 for functions that cannot be built."""
    fa = f.facts
    if fa.pad:
        return 0.0
    p = GAIN["base"] - GAIN["call"] * min(fa.calls, GAIN["call_cap"]) - (GAIN["loop"] if fa.loop else 0.0)
    if fa.jtbl:
        p *= GAIN["jtbl"] if f.island else 0.0
    if fa.strings:
        p *= GAIN["strings"] if f.island else 0.0
    if fa.reload or fa.dispatch:
        p *= GAIN["promo"]
    if fa.selector:
        p *= GAIN["u0"] if fa.selector == "v0" else GAIN["u1"]
    return max(p, 0.0)


def effort(f, k=1):
    """Work units for one function; k = unmatched members of its duplicate group (copies are cheap)."""
    e = GAIN["effort0"] + (f.facts.size / 4.0) ** GAIN["exponent"]
    return e * (1 + GAIN["copy"] * (k - 1))


def expected_bytes(f, credit=0.0):
    """Bytes expected from working on f: its size times P(match), times the group it unlocks."""
    return f.facts.size * p_match(f) * (1 + credit)


class Group(NamedTuple):
    key: int
    unmatched: int      # members still in assembly
    matched: str        # name of a matched member ("" when none): dupes.py can copy it
    rep: bool           # this function is the cheapest member to match by hand
    credit: float = 0.0   # extra bytes (in units of this function) the rep unlocks


class Scored(NamedTuple):
    func: Func
    score: float        # expected bytes per effort unit; 0 for functions that are not work
    ev: float           # expected bytes
    eff: float
    group: object = None   # Group or None

    @property
    def dup(self):
        g = self.group
        if g is None:
            return ""
        if g.matched:
            return "apply:" + g.matched
        return ("rep+%d" % (g.unmatched - 1)) if g.rep and g.unmatched > 1 else ("" if g.rep else "=rep")


def unit_of(f):
    return f.unit_name


def compute_groups(root):
    """Near-duplicate groups (tools/dupes.py exact + tools/neardupes.py shapes) as lists of
    (unit, name, matched, bytes, exact fingerprint). Runs the two tools' fingerprinting in-process."""
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    import dupes
    groups = {}
    for fn in dupes.load_funcs(str(root), near=True):
        if not fn.bad:
            groups.setdefault(fn.key, []).append((fn.unit, fn.name, bool(fn.matched), fn.n * 4, fn.exact))
    return [g for g in groups.values() if len(g) > 1]


def save_groups(groups, path):
    import json
    Path(path).write_text(json.dumps({"groups": groups}, separators=(",", ":")))


def load_groups_file(path):
    import json
    return [[tuple(m) for m in g] for g in json.loads(Path(path).read_text())["groups"]]


def group_index(groups, funcs):
    """{(unit, name): Group} for the remaining functions in `funcs` that sit in a duplicate group."""
    byname = {(unit_of(f), f.name): f for f in funcs}
    out = {}
    for n, members in enumerate(groups):
        todo = [(byname[(m[0], m[1])], m[4] if len(m) > 4 else None) for m in members
                if not m[2] and (m[0], m[1]) in byname]
        matched = [m[1] for m in members if m[2]]
        if not todo:
            continue
        rep, rep_exact = min(todo, key=lambda t: (t[0].blocked, p_match(t[0]) == 0, t[0].facts.size, t[0].file, t[0].name))
        credit = sum(GAIN["group"] if ex is not None and ex == rep_exact else GAIN["near"]
                     for f, ex in todo if f is not rep)
        for f, _ex in todo:
            out[(unit_of(f), f.name)] = Group(n, len(todo), matched[0] if matched else "", f is rep, credit)
    return out


def score_funcs(funcs, groups=None):
    """Scored list of `funcs` (not sorted). groups: output of group_index."""
    out = []
    for f in funcs:
        g = (groups or {}).get((unit_of(f), f.name))
        live = g and not g.matched and g.rep
        k = g.unmatched if live else 1
        ev, eff = expected_bytes(f, g.credit if live else 0.0), effort(f, k)
        work = not (g and (g.matched or not g.rep))
        out.append(Scored(f, ev / eff if work else 0.0, ev if work else 0.0, eff, g))
    return out


def plan_lists(items, agents, load, exclusive=lambda x: x.func.file):
    """Split `items` into `agents` lists with no `exclusive` key (the C file) in two lists, balancing
    the summed `load`. Greedy longest-processing-time first: biggest file to the lightest list.
    Returns a list of lists of items, in the input order inside each list."""
    by = {}
    for it in items:
        by.setdefault(exclusive(it), []).append(it)
    bins = [[0.0, n, []] for n in range(agents)]
    for key, its in sorted(by.items(), key=lambda kv: (-sum(load(i) for i in kv[1]), kv[0])):
        b = min(bins, key=lambda b: (b[0], b[1]))
        b[0] += sum(load(i) for i in its)
        b[2] += its
    order = {id(it): n for n, it in enumerate(items)}
    return [sorted(b[2], key=lambda it: order[id(it)]) for b in bins]


def print_table(funcs, dupes, out=None):
    out = out or sys.stdout
    out.write("%-4s %-15s %-26s %5s %-4s %5s %-6s %s\n" % ("rank", "file", "function", "size", "leaf", "calls", "flags", "dup"))
    for n, f in enumerate(funcs, 1):
        out.write("%-4d %-15s %-26s %5d %-4s %5d %-6s %s\n" % (
            n, f.file, f.name, f.facts.size, "leaf" if f.leaf else "call", f.facts.calls,
            f.flags or "-", "=" + dupes[f.name] if f.name in dupes else ""))


def print_scored(items, out=None):
    out = out or sys.stdout
    out.write("%-4s %-15s %-26s %5s %-4s %5s %-6s %6s %7s %s\n" % (
        "rank", "file", "function", "size", "leaf", "calls", "flags", "score", "exp.B", "dup"))
    for n, sc in enumerate(items, 1):
        f = sc.func
        out.write("%-4d %-15s %-26s %5d %-4s %5d %-6s %6.2f %7.1f %s\n" % (
            n, f.file, f.name, f.facts.size, "leaf" if f.leaf else "call", f.facts.calls,
            f.flags or "-", sc.score, sc.ev, sc.dup))


def print_plan(lists, out=None, by="bytes"):
    out = out or sys.stdout
    total = [sum(sc.func.facts.size for sc in l) for l in lists]
    effs = [sum(sc.eff for sc in l) for l in lists]
    n = sum(len(l) for l in lists)
    out.write("plan: %d functions, %d bytes, %d lists, effort per list %s (max/min %.2f)\n" % (
        n, sum(total), len(lists), "/".join("%.0f" % e for e in effs),
        (max(effs) / min(e for e in effs if e) if any(effs) and all(effs) else 0.0)))
    for a, l in enumerate(lists, 1):
        files = sorted({sc.func.file for sc in l})
        out.write("\nagent %d: %d functions, %d bytes (expected %.0f), %d file(s)\n" % (
            a, len(l), total[a - 1], sum(sc.ev for sc in l), len(files)))
        if not l:
            out.write("  (no work left: fewer files than lists)\n")
            continue
        out.write("  --files %s\n" % ",".join(files))
        for sc in l:
            f = sc.func
            out.write("  %-15s %-26s %5d %-6s %6.2f %s\n" % (f.file, f.name, f.facts.size, f.flags or "-", sc.score, sc.dup))


def plan_json(lists):
    import json
    return json.dumps([{"agent": a, "files": sorted({sc.func.file for sc in l}),
                        "bytes": sum(sc.func.facts.size for sc in l),
                        "functions": [{"file": sc.func.file, "name": sc.func.name, "size": sc.func.facts.size,
                                       "flags": sc.func.flags, "score": round(sc.score, 3)} for sc in l]}
                       for a, l in enumerate(lists, 1)], indent=1)


def summary(funcs, out=None):
    out = out or sys.stdout
    per = {}
    for f in funcs:
        d = per.setdefault(f.file, [0, 0, 0])
        d[0] += 1
        d[1] += f.blocked
        d[2] += f.leaf
    out.write("%-16s %6s %8s %6s\n" % ("file", "left", "blocked", "leaf"))
    for k, (a, b, c) in sorted(per.items()):
        out.write("%-16s %6d %8d %6d\n" % (k, a, b, c))
    out.write("%-16s %6d %8d %6d\n" % ("total", len(funcs), sum(f.blocked for f in funcs), sum(f.leaf for f in funcs)))
    out.write("flags: " + " ".join("%s=%d" % (c, sum(c in f.flags for f in funcs)) for c in "LJSPRVTU") + "\n")
    out.write("R, V or U (T-0018 candidates): %d, of which U (blocked-unknown): %d\n" % (
        sum(any(c in f.flags for c in "RVU") for f in funcs), sum("U" in f.flags for f in funcs)))


def fired(f):
    return any(c in f.flags for c in "RVUT")


# promo rows about a global kept in a register across blocks (compare chain, post-increment, D++,
# dispatch); the other promo rows (`D = N; f(N)`, `i * 0x38` constants, hoisted loads) are other gaps
FAMILY_RE = re.compile(r"compare chain|post-inc|D\+\+|old value in \$v1|dispatch on a global|compared in two loops")


def calibration(remaining, matched, cases):
    """Counts for the detector: positives = the `promo` rows of the cases table found in either set
    (a row may have been matched since, with a trick: its asm still has the pattern), negatives =
    matched functions that are in no row. fam = the positives whose symptom is the global-in-register
    family. Returns a dict."""
    index = {}
    for f in remaining + matched:
        index.setdefault((f.file.split("/")[0].lower(), f.name), f)
    rows = {(file.lower(), name) for file, name, _c, _s in cases}
    pos, fam, missing = [], [], []
    for file, name, cat, sym in cases:
        if cat != "promo":
            continue
        f = index.get((file.lower(), name))
        if not f:
            missing.append((file, name))
            continue
        pos.append(f)
        if FAMILY_RE.search(sym):
            fam.append(f)
    neg = [f for f in matched if (f.file.split("/")[0].lower(), f.name) not in rows]
    other = [index[(file.lower(), name)] for file, name, cat, _ in cases if cat != "promo" and (file.lower(), name) in index]
    return dict(pos=pos, fam=fam, missing=missing, neg=neg, other=other)


def calibrate(remaining, matched, cases, out=None, title="detector"):
    out = out or sys.stdout
    c = calibration(remaining, matched, cases)
    pos, fam, neg = c["pos"], c["fam"], c["neg"]
    out.write("[%s] promo rows of the cases table found in asm: %d (%d of the global-in-register family; not found: %d)\n" % (
        title, len(pos), len(fam), len(c["missing"])))
    out.write("negatives: %d matched functions outside the table\n" % len(neg))
    out.write("%-26s %9s %9s %10s %10s\n" % ("flags", "recall", "family", "FP matched", "precision"))
    sets = [("R V U T (any)", lambda f: fired(f)),
            ("R V U (not the T hint)", lambda f: any(x in f.flags for x in "RVU")),
            ("R V U1 (blocked: cvt gap)", lambda f: any(x in f.flags for x in ("R", "V", "U1"))),
            ("R", lambda f: "R" in f.flags), ("V", lambda f: "V" in f.flags), ("T", lambda f: "T" in f.flags),
            ("U (U0 and U1)", lambda f: "U" in f.flags), ("U0 ($v0 selector)", lambda f: "U0" in f.flags),
            ("U1 ($v1 selector)", lambda f: "U1" in f.flags)]
    res = {}
    for name, test in sets:
        tp, tf = sum(1 for f in pos if test(f)), sum(1 for f in fam if test(f))
        fp = sum(1 for f in neg if test(f))
        res[name] = (tp, tf, fp)
        out.write("%-26s %4d/%-4d %4d/%-4d %10d %9.1f%%\n" % (
            name, tp, len(pos), tf, len(fam), fp, 100.0 * tp / (tp + fp) if tp + fp else 0.0))
    out.write("other recorded cases (regorder, reverse; not expected to fire): %d, of which flagged R V U: %d\n" % (
        len(c["other"]), sum(1 for f in c["other"] if any(x in f.flags for x in "RVU"))))
    for x in c["missing"]:
        out.write("  not found: %s\n" % (x,))
    for f in fam:
        if not fired(f):
            out.write("  family row missed: %s %s %s\n" % (f.file, f.name, f.flags or "-"))
    rem_fl = [f for f in remaining if any(x in f.flags for x in "RVU")]
    out.write("remaining INCLUDE_ASM functions flagged R, V or U: %d of %d (U1 %d, U0 %d)\n" % (
        len(rem_fl), len(remaining), sum("U1" in f.flags for f in remaining), sum("U0" in f.flags for f in remaining)))
    return res


def allow_unknown(f, mode):
    """True when f is blocked only by U and `--include-unknown MODE` lets it through."""
    return bool(mode) and f.unknown and (mode == "all" or f.u0)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--root", default=".")
    ap.add_argument("--files", help="comma separated main address stems / overlay names (prefix match)")
    ap.add_argument("--next", type=int, metavar="N", help="print the N best unblocked functions")
    ap.add_argument("--blocked", action="store_true", help="only functions with a blocker flag")
    ap.add_argument("--all", action="store_true", help="with --next, include blocked functions")
    ap.add_argument("--include-unknown", nargs="?", const="all", choices=("u0", "all"), default="",
                    help="treat U (switch selector on an unsigned global, cause unknown) as workable in "
                         "--next and --plan: u0 = selector in $v0 only, all = U0 and U1")
    ap.add_argument("--flag", default="", help="only functions that have all of these flag letters")
    ap.add_argument("--max-size", type=int, default=0)
    ap.add_argument("--leaf", action="store_true", help="only leaf functions")
    ap.add_argument("--summary", action="store_true")
    ap.add_argument("--calibrate", action="store_true")
    ap.add_argument("--legacy", action="store_true", help="with --calibrate: score the T-1320 rule only")
    ap.add_argument("--cases", default="wiki/data/t0018-cases.md")
    ap.add_argument("--dupes", default="")
    ap.add_argument("--by", choices=("default", "bytes"), default="default",
                    help="bytes: expected bytes gained per effort, duplicate groups first (T-3340)")
    ap.add_argument("--plan", type=int, metavar="N", help="split the next N best functions into --agents work lists")
    ap.add_argument("--agents", type=int, default=4, metavar="K")
    ap.add_argument("--exclusive", choices=("file", "unit"), default="file",
                    help="no C file (or no overlay) shared between lists")
    ap.add_argument("--groups", default="", help="cached duplicate groups (JSON written by --save-groups)")
    ap.add_argument("--save-groups", default="", metavar="FILE")
    ap.add_argument("--no-groups", action="store_true", help="skip the duplicate-group computation")
    ap.add_argument("--with-applicable", action="store_true",
                    help="keep functions whose duplicate group already has a matched member in --next/--plan")
    ap.add_argument("--json", action="store_true", help="with --plan: JSON instead of text")
    ap.add_argument("--no-cache", action="store_true", help="ignore and do not write build/queue-cache.json")
    a = ap.parse_args(argv)
    root = Path(a.root)
    cache = Cache(root, not a.no_cache)
    wanted = a.files.split(",") if a.files else []
    # only --by bytes / --plan need the whole project (duplicate groups, balancing); otherwise a
    # --files list limits what is read
    narrow = wanted if not (a.calibrate or a.plan or a.by == "bytes") else []
    remaining, matched = load_cached(root, legacy=a.calibrate and a.legacy, cache=cache, files=narrow)
    if not remaining and not narrow:
        print("error: no INCLUDE_ASM functions found (run ninja first to generate asm/)", file=sys.stderr)
        return 1
    if a.calibrate:
        cases = read_cases(root / a.cases)
        calibrate(remaining, matched, cases, title="T-1320 rule" if a.legacy else "T-3340 rule")
        if not a.legacy:
            print()
            lr, lm = load_cached(root, legacy=True, cache=cache)
            calibrate(lr, lm, cases, title="T-1320 rule, for comparison")
        return 0
    funcs = select(remaining, wanted)
    if a.summary:
        summary(funcs)
        return 0
    if a.blocked:
        funcs = [f for f in funcs if f.blocked]
    if a.leaf:
        funcs = [f for f in funcs if f.leaf]
    if a.max_size:
        funcs = [f for f in funcs if f.facts.size <= a.max_size]
    funcs = [f for f in funcs if all(c in f.flags for c in a.flag)]
    count = a.plan or a.next
    allow = lambda f: not f.blocked or a.all or allow_unknown(f, a.include_unknown)
    if count and not a.blocked:
        funcs = [f for f in funcs if allow(f)]
    if a.by == "bytes" or a.plan:
        groups = {}
        if not a.no_groups:
            if a.groups and Path(a.groups).exists():
                glist = load_groups_file(a.groups)
            else:
                glist = groups_cached(root, cache)
                if a.save_groups:
                    save_groups(glist, a.save_groups)
            groups = group_index(glist, remaining)
        items = score_funcs(funcs, groups)
        if a.by == "bytes":
            items.sort(key=lambda sc: (sc.func.blocked and not allow_unknown(sc.func, a.include_unknown),
                                       -sc.score, sc.func.file, sc.func.name))
        else:
            items.sort(key=lambda sc: rank_key(sc.func))
        if count and not a.with_applicable:
            items = [sc for sc in items if sc.score > 0 or a.all or a.by != "bytes"]
        if a.plan:
            items = items[:a.plan]
            if a.by != "bytes":
                load_of = lambda sc: sc.func.facts.size
            else:
                load_of = lambda sc: sc.eff
            lists = plan_lists(items, max(1, a.agents), load_of,
                               (lambda sc: sc.func.file) if a.exclusive == "file" else (lambda sc: unit_of(sc.func)))
            print(plan_json(lists) if a.json else "", end="" if not a.json else "\n")
            if not a.json:
                print_plan(lists, by=a.by)
            return 0
        if count:
            items = items[:count]
        print_scored(items)
        return 0
    funcs.sort(key=rank_key)
    if a.next:
        funcs = funcs[:a.next]
    dupes = {}
    for cand in ([a.dupes] if a.dupes else ["build/dupes.txt", "wiki/data/dupes.txt"]):
        if (root / cand).exists():
            dupes = read_dupes(root / cand)
            break
    print_table(funcs, dupes)
    return 0


if __name__ == "__main__":
    sys.exit(main())
