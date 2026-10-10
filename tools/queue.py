#!/usr/bin/env python3
"""Ranked work list of the remaining INCLUDE_ASM functions, with a blocker detector (T-1320).

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
  R  T-0018 register promotion: one global scalar is loaded in two or more basic blocks outside
     loops into the same register (reloaded after calls or branches). Original uopt promotes the
     global; IDO 5.3 does that only inside loops, so the function does not match.
  V  T-0018 register choice: a global is loaded into $v1 while $v0 is dead and the value is read
     twice or more (compare chain / switch, old value of `D++`). The original promotes it to $v1,
     IDO takes $v0 first.

R and V are heuristics calibrated against wiki/data/t0018-cases.md and the matched functions
(`--calibrate`, results in wiki/matching-notes.md). J, S and P are exact.

Note: the file name shadows the standard library `queue` when tools/ is first on sys.path (running
scripts from tools/); no tool here imports the standard one. Other tools load this file by path
(tools/list_leaves.py, tools/test_queue.py) under the name `work_queue`.

Usage (inside Docker, from the repo root after `ninja` has generated asm/):
  python3 tools/queue.py                        full table, workable functions first
  python3 tools/queue.py --files 80041000,TEL   only these files (main address stems / overlay names)
  python3 tools/queue.py --next 20 --files TEL  the 20 best workable functions, for agents
  python3 tools/queue.py --blocked              only functions with a blocker flag (R V J S P)
  python3 tools/queue.py --summary              counts per file and per flag
  python3 tools/queue.py --calibrate            detector precision/recall
Ranking: unblocked first, then leaf before non-leaf, then size ascending, then file and name.
Duplicates: if a dupes list exists (--dupes FILE, default build/dupes.txt or wiki/data/dupes.txt;
lines with two or more function names are groups) the later members of a group are marked `=first`.
"""
import argparse
import re
import sys
from pathlib import Path
from typing import NamedTuple

import srcscan

BLOCKERS = "JSPRV"
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


def analyze(text, string_syms=frozenset()):
    """Facts about one function's asm text. string_syms: names of .rodata string symbols."""
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
    # R: one global, 2+ blocks outside loops, one destination register
    by = {}
    for b, reg, key, inloop, _ in loads:
        if not inloop and reg not in ARGREGS:
            by.setdefault((key, reg), set()).add(b)
    reload = any(len(bs) >= 2 for bs in by.values())
    # V: the first global load of the function goes to $v1 although $v0 is dead there (IDO takes
    # $v0 first) and the value is used at least twice (compare chain, old value of D++)
    dispatch = False
    for _, reg, _, _, k in loads:
        if reg == "v1" and v0_dead_after(insns, k) and reads_before_redef(insns, k, "v1") >= 2:
            dispatch = True
            break
    return Facts(len(insns) * 4, calls, bool(loops), jtbl, strings, pad, reload, dispatch)


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

    @property
    def leaf(self):
        return self.facts.calls == 0

    @property
    def flags(self):
        f = self.facts
        return "".join(c for c, on in (("L", f.loop), ("J", f.jtbl), ("S", f.strings), ("P", f.pad),
                                       ("R", f.reload), ("V", f.dispatch)) if on)

    @property
    def blocked(self):
        stop = BLOCKERS.replace("J", "").replace("S", "") if self.island else BLOCKERS
        return any(c in stop for c in self.flags)


def units(root):
    """[(file label, C source path, asm dir of the matched functions, has island)] for every C file
    of the main exe and the overlays (srcscan.c_files: the `c` subsegments of the splat configs)."""
    out = []
    for cf in srcscan.c_files(root):
        out.append((cf.label, cf.src, cf.matchings, cf.island))
    return out


def load(root=".", string_syms=None):
    """(remaining Funcs, matched Funcs) for the whole project."""
    if string_syms is None:
        string_syms = string_symbols(root)
    remaining, matched = [], []
    for label, c, mdir, island in units(root):
        for e in srcscan.include_asm_entries(c):
            if Path(e.folder).name == "pad":
                continue
            p = Path(root) / e.folder / (e.name + ".s")
            if p.exists():
                remaining.append(Func(label, e.name, str(p), analyze(p.read_text(errors="replace"), string_syms),
                                      island))
        if mdir.is_dir():
            defined = srcscan.defined_functions(c)
            for p in sorted(mdir.glob("*.s")):
                if p.stem in defined:
                    matched.append(Func(label, p.stem, str(p), analyze(p.read_text(errors="replace"), string_syms),
                                        island))
    return remaining, matched


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


def print_table(funcs, dupes, out=sys.stdout):
    out.write("%-4s %-15s %-26s %5s %-4s %5s %-6s %s\n" % ("rank", "file", "function", "size", "leaf", "calls", "flags", "dup"))
    for n, f in enumerate(funcs, 1):
        out.write("%-4d %-15s %-26s %5d %-4s %5d %-6s %s\n" % (
            n, f.file, f.name, f.facts.size, "leaf" if f.leaf else "call", f.facts.calls,
            f.flags or "-", "=" + dupes[f.name] if f.name in dupes else ""))


def summary(funcs, out=sys.stdout):
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
    out.write("flags: " + " ".join("%s=%d" % (c, sum(c in f.flags for f in funcs)) for c in "LJSPRV") + "\n")
    out.write("R or V (T-0018 candidates): %d\n" % sum(("R" in f.flags or "V" in f.flags) for f in funcs))


def calibrate(remaining, matched, cases, out=sys.stdout):
    """Precision and recall of R/V against recorded T-0018 cases (promo) and matched functions."""
    index = {}
    for f in remaining:
        index[(f.file.lower(), f.name)] = f
    pos, missing = [], []
    for file, name, cat, _ in cases:
        if cat != "promo":
            continue
        f = index.get((file.lower(), name))
        (pos if f else missing).append(f or (file, name))
    fired = lambda f: "R" in f.flags or "V" in f.flags
    tp = sum(1 for f in pos if fired(f))
    fp = sum(1 for f in matched if fired(f))
    out.write("recorded promo cases found in asm: %d (not found: %d)\n" % (len(pos), len(missing)))
    out.write("recall    %d/%d = %.1f%%\n" % (tp, len(pos), 100.0 * tp / len(pos) if pos else 0))
    out.write("matched functions flagged (false positives): %d of %d\n" % (fp, len(matched)))
    out.write("precision  %d/%d = %.1f%%  (TP over TP + flagged matched functions)\n" % (
        tp, tp + fp, 100.0 * tp / (tp + fp) if tp + fp else 0))
    for flag in "RV":
        t = sum(1 for f in pos if flag in f.flags)
        m = sum(1 for f in matched if flag in f.flags)
        out.write("  flag %s: recorded hit %d/%d, matched flagged %d\n" % (flag, t, len(pos), m))
    other = [(f.file, f.name, c) for (file, name, c, _) in cases if c != "promo"
             for f in [index.get((file.lower(), name))] if f]
    n_other = sum(1 for (file, name, c, _) in cases if c != "promo" and (file.lower(), name) in index)
    out.write("other recorded cases (not expected to fire): %d, of which flagged R/V: %d\n" % (
        n_other, sum(1 for o in other if fired(index[(o[0].lower(), o[1])]))))
    for x in missing:
        out.write("  not found: %s\n" % (x,))
    for f in pos:
        if not fired(f):
            out.write("  missed: %s %s %s\n" % (f.file, f.name, f.flags or "-"))
    for f in matched:
        if fired(f):
            out.write("  false positive: %s %s %s\n" % (f.file, f.name, f.flags))
    remaining_flagged = [f for f in remaining if fired(f)]
    out.write("remaining INCLUDE_ASM functions flagged R or V: %d of %d\n" % (len(remaining_flagged), len(remaining)))


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--root", default=".")
    ap.add_argument("--files", help="comma separated main address stems / overlay names (prefix match)")
    ap.add_argument("--next", type=int, metavar="N", help="print the N best unblocked functions")
    ap.add_argument("--blocked", action="store_true", help="only functions with a blocker flag")
    ap.add_argument("--all", action="store_true", help="with --next, include blocked functions")
    ap.add_argument("--flag", default="", help="only functions that have all of these flag letters")
    ap.add_argument("--max-size", type=int, default=0)
    ap.add_argument("--leaf", action="store_true", help="only leaf functions")
    ap.add_argument("--summary", action="store_true")
    ap.add_argument("--calibrate", action="store_true")
    ap.add_argument("--cases", default="wiki/data/t0018-cases.md")
    ap.add_argument("--dupes", default="")
    a = ap.parse_args(argv)
    root = Path(a.root)
    remaining, matched = load(root)
    if not remaining:
        print("error: no INCLUDE_ASM functions found (run ninja first to generate asm/)", file=sys.stderr)
        return 1
    if a.calibrate:
        calibrate(remaining, matched, read_cases(root / a.cases))
        return 0
    funcs = select(remaining, a.files.split(",") if a.files else [])
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
    if a.next and not a.all and not a.blocked:
        funcs = [f for f in funcs if not f.blocked]
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
