#!/usr/bin/env python3
"""Measure three code habits of the ASPSX-assembled SDK asm of the main exe (T-11890).

Usage (inside Docker, after configure.py has split the main exe):
    python3 tools/aspsx_survey.py [asm/lib*.s ...]

Reads splat disassembly (default: every asm/lib*.s, the PsyQ library code of SLPM_86.053) and
prints counts, no game bytes:

1. div: every `div`/`divu $zero,rs,rt` followed by ASPSX's check sequence (`bnez rt; nop;
   break 7`, and for div also `addiu $at,$zero,-1; bne rt,$at; lui $at,0x8000; bne rs,$at; nop;
   break 6`). Each site is keyed by the move register rd (first mflo/mfhi after the div)
   against rs/rt, by where the move sits (before the checks or after them) and by whether
   other instructions sit inside the expansion (ASPSX scheduling). A late move followed by an
   instruction that reads rd is counted with and without a nop in between.
2. mflo/mfhi windows: for every mflo/mfhi, the instructions up to the next label, branch or
   mult/div-class instruction, and whether the nops before that boundary fill exactly the two
   slots the hazard rule asks for (a branch whose delay slot is not a nop is taken as one from a
   gcc `.set noreorder` block; a branch with a nop delay slot is one ASPSX filled itself).
3. symbol + register loads/stores: `lui R,%hi(sym); addu R,R,X; op D,%lo(sym)(R)`, keyed by
   load/store and by R ($at or the destination D), with D == X separated.

Results and their reading: wiki/gcc-objects.md, "ASPSX habits in the SDK asm (T-11890)"; the real ASPSX
shows none of the three on gcc's macro output (same page, "The real ASPSX on synthetic input").
"""
import collections
import glob
import re
import sys

INS = re.compile(r"^\s*/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/\s+(\S+)\s*(.*)$")
LAB = re.compile(r"^\s*(\.L[0-9A-F]+|glabel \w+)")
MULDIV = {"mult", "multu", "div", "divu", "mthi", "mtlo"}
BRANCH = {"b", "j", "jr", "jal", "jalr", "beq", "bne", "beqz", "bnez", "bgez", "bgtz",
          "blez", "bltz", "bgezal", "bltzal"}
LOADS = {"lb", "lbu", "lh", "lhu", "lw", "lwl", "lwr"}
STORES = {"sb", "sh", "sw", "swl", "swr"}


def load(path):
    """Items of one file: ("i", addr, op, operands without spaces) or ("l", name)."""
    items = []
    with open(path) as f:
        for line in f:
            m = INS.match(line)
            if m:
                items.append(("i", m.group(1), m.group(2), m.group(3).replace(" ", "")))
            elif LAB.match(line):
                items.append(("l", LAB.match(line).group(1)))
    return items


def reads(item, reg):
    """True if the instruction reads `reg` (enough for the ALU, load, store and branch forms)."""
    op, args = item[2], item[3].split(",")
    if op in STORES or op in BRANCH or op in MULDIV:
        return any(reg in a for a in args)
    return any(reg in a for a in args[1:])


def check_kind(item, rs, rt, signed):
    op, a = item[2], item[3]
    if op == "bnez" and a.startswith(rt + ","):
        return "Z"
    if op == "break" and a in ("7", "6"):
        return "B" + a
    if signed and op == "addiu" and a == "$at,$zero,-0x1":
        return "A"
    if signed and op == "bne" and a.startswith(rt + ",$at"):
        return "N1"
    if signed and op == "lui" and a.startswith("$at,(0x80000000"):
        return "U"
    if signed and op == "bne" and a.startswith(rs + ",$at"):
        return "N2"
    if op == "nop":
        return "n"
    if op in ("mflo", "mfhi"):
        return "M"
    return "x"


def survey_div(items, stats, examples, path):
    for k, x in enumerate(items):
        if not (x[0] == "i" and x[2] in ("div", "divu") and x[3].startswith("$zero,")):
            continue
        rs, rt = x[3].split(",")[1:3]
        signed = x[2] == "div"
        last = "B6" if signed else "B7"
        sig, move, move_at = [], None, None
        for j in range(k + 1, min(k + 40, len(items))):
            y = items[j]
            if y[0] == "l":
                continue
            if y[2] in MULDIV:
                break
            kind = check_kind(y, rs, rt, signed)
            sig.append(kind)
            if kind == "M" and move is None:
                move, move_at = y, j
            if last in sig and move is not None:
                break
        if "B7" not in sig:
            stats["div: not expanded (no break 7)"] += 1
            continue
        if move is None:
            stats["div: no move"] += 1
            continue
        rd = move[3]
        rel = "rd=rs" if rd == rs else "rd=rt" if rd == rt else "rd distinct"
        pos = "early" if sig.index("M") < sig.index("Z") else "late"
        end = sig.index(last) if pos == "early" else sig.index("M")
        sched = "scheduled" if "x" in sig[:end] else "clean"
        key = "div: %s %s %s, move %s, %s" % (x[2], move[2], rel, pos, sched)
        stats[key] += 1
        examples[key].append("%s:%s" % (path, x[1]))
        if pos == "late":
            nxt = [y for y in items[move_at + 1:move_at + 3] if y[0] == "i"]
            if nxt and nxt[0][2] == "nop" and len(nxt) > 1 and reads(nxt[1], rd):
                stats["div: late move, nop, reader of rd"] += 1
            elif nxt and nxt[0][2] != "nop" and reads(nxt[0], rd):
                stats["div: late move, reader of rd directly after"] += 1


def survey_mflo(items, stats, examples, path):
    for k, x in enumerate(items):
        if not (x[0] == "i" and x[2] in ("mflo", "mfhi")):
            continue
        real = pads = 0
        bound = "none"
        j = k + 1
        while j < len(items):
            y = items[j]
            if y[0] == "l":
                bound = "label"
                break
            if y[2] in MULDIV:
                bound = "mult/div"
                break
            if y[2] in BRANCH:
                ds = items[j + 1] if j + 1 < len(items) else None
                filled = ds is not None and ds[0] == "i" and ds[2] != "nop"
                bound = "branch, filled delay slot" if filled else "branch, nop delay slot"
                break
            if real + pads == 2:
                break
            if y[2] == "nop":
                pads += 1
            else:
                real += 1
            j += 1
        if bound in ("label", "mult/div", "branch, filled delay slot"):
            want = max(0, 2 - real)
        else:
            want = 0
        verdict = "as predicted" if pads == want else "NOT as predicted"
        key = "mflo: before %s, %d instr + %d nop: %s" % (bound, real, pads, verdict)
        stats[key] += 1
        examples[key].append("%s:%s" % (path, x[1]))


def survey_hilo(items, stats, examples, path):
    for k, x in enumerate(items):
        if not (x[0] == "i" and x[2] == "lui" and "%hi(" in x[3]):
            continue
        r, hi = x[3].split(",", 1)
        sym = hi[4:-1]
        add = mem = None
        for j in range(k + 1, min(k + 5, len(items))):
            y = items[j]
            if y[0] == "l":
                break
            a = y[3].split(",")
            if add is None and y[2] == "addu" and a[0] == r and a[1] == r:
                add = a[2]
                continue
            if y[2] in LOADS | STORES and "%%lo(%s)(%s)" % (sym, r) in y[3]:
                mem = y
                break
        if mem is None or add is None:
            continue
        d = mem[3].split(",")[0]
        kind = "load" if mem[2] in LOADS else "store"
        via = "$at" if r == "$at" else "dest" if r == d else "other"
        extra = ", dest == index" if kind == "load" and d == add else ""
        key = "sym(reg): %s %s%s, through %s" % (kind, mem[2] if mem[2] in ("lwl", "lwr") else "",
                                                 extra, via)
        stats[key] += 1
        examples[key].append("%s:%s" % (path, x[1]))


def main(argv):
    paths = argv[1:] or sorted(glob.glob("asm/lib*.s"))
    if not paths:
        sys.exit("aspsx_survey: no asm/lib*.s (run configure.py first)")
    stats, examples = collections.Counter(), collections.defaultdict(list)
    for path in paths:
        items = load(path)
        name = path.rsplit("/", 1)[-1]
        survey_div(items, stats, examples, name)
        survey_mflo(items, stats, examples, name)
        survey_hilo(items, stats, examples, name)
    for key in sorted(stats):
        ex = examples.get(key, [])
        print("%5d  %s%s" % (stats[key], key, ("  e.g. " + " ".join(ex[:3])) if ex else ""))


if __name__ == "__main__":
    main(sys.argv)
