"""Argument counts of asm callees and the old-value compare of post-increments, for tools/m2c.py (T-7030).

m2c reads one function; a callee it cannot see has either the signature the context gives it or, when
the context declares it with `()` or not at all, a signature it makes up from the registers that happen
to hold values at the call (a stale `$a1` of an earlier call counts as an argument). This module reads
the callee's own asm and counts the parameters it uses, so the wrapper can hand m2c a real prototype.

  arity(asm_text)            number of parameter words the function reads before it writes them
                             ($a0-$a3 and stack words above the frame), or None when the asm is not
                             a function this scan understands
  post_increment_old(asm)    {symbol} of globals that are loaded, incremented, stored back, and
                             compared through the register that holds the OLD value
  rewrite_post_increment(draft, symbols)   m2c's `D += 1; if (D >= N)` -> `if (D++ >= N)` for those
"""
import re

INSN_RE = re.compile(r"^\s*/\*\s*[0-9A-Fa-f]+\s+[0-9A-Fa-f]{8}\s+[0-9A-Fa-f]{8}\s*\*/\s*(\S+)\s*(.*?)\s*$")
REG_RE = re.compile(r"\$(zero|at|v[01]|a[0-3]|t[0-9]|s[0-8]|k[01]|gp|sp|fp|ra|\d+)\b")
ARG_REGS = ("a0", "a1", "a2", "a3")
CALL_CLOBBER = {"v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "t8", "t9", "at", "ra"}
STORES = {"sb", "sh", "sw", "swl", "swr", "swc1", "swc2"}
LOADS = {"lb", "lbu", "lh", "lhu", "lw", "lwl", "lwr", "lwc1", "lwc2"}
BRANCHES = {"beq", "bne", "beqz", "bnez", "bgez", "bgtz", "blez", "bltz", "bgezal", "bltzal", "bc1t", "bc1f", "b"}
NO_WRITE = {"mult", "multu", "div", "divu", "mthi", "mtlo", "nop", "jr", "j", "break", "syscall"}
LABEL_RE = re.compile(r"^\s*\.L\w+:|^\s*glabel\b|^\s*jlabel\b")


def parse(text):
    """[(mnemonic, [operand strings])] of the instruction lines of a splat function .s."""
    out = []
    for line in text.splitlines():
        m = INSN_RE.match(line)
        if m:
            ops = [o.strip() for o in re.split(r",(?![^()]*\))", m.group(2)) if o.strip()]
            out.append((m.group(1), ops))
    return out


def regs(op):
    return [r for r in REG_RE.findall(op)]


def effects(mn, ops):
    """(read registers, written registers) of one instruction (names without `$`)."""
    if mn in STORES:
        return set(regs(" ".join(ops))), set()
    if mn in LOADS:
        rs = regs(ops[1]) if len(ops) > 1 else []
        return set(rs), set(regs(ops[0]))
    if mn in BRANCHES:
        return set(regs(" ".join(ops))), set()
    if mn == "jal":
        return set(), set(CALL_CLOBBER)
    if mn == "jalr":
        return set(regs(" ".join(ops))), set(CALL_CLOBBER)
    if mn in NO_WRITE:
        return set(regs(" ".join(ops))), set()
    if mn in ("lui", "li", "la") or (mn in ("mfhi", "mflo")):
        return set(), set(regs(ops[0])) if ops else set()
    if mn in ("mfc1", "mfc0", "cfc1"):
        return set(), set(regs(ops[0]))
    if mn in ("mtc1", "ctc1", "mtc0"):
        return set(regs(ops[0])), set()
    if not ops:
        return set(), set()
    return set(regs(" ".join(ops[1:]))), set(regs(ops[0]))      # rd, rs[, rt|imm]


def arity(text):
    """Parameter words the function reads before writing them: $a0-$a3 in order, and stack words above
    its frame (`lw $x, N($sp)` with N beyond frame + 0x10). Linear scan: a register written on one path
    and read on another counts as a parameter only when no earlier instruction wrote it, which is how
    the original compiler's leaf and non-leaf code reads its arguments. None if there is no code."""
    insns = parse(text)
    if not insns:
        return None
    written, used = set(), set()
    frame = 0
    for mn, ops in insns:
        if mn in ("addiu", "addi") and len(ops) == 3 and ops[0] == "$sp" and ops[1] == "$sp":
            try:
                v = int(ops[2], 0)
            except ValueError:
                v = 0
            if v < 0 and not frame:
                frame = -v
        reads, writes = effects(mn, ops)
        for r in reads:
            if r in ARG_REGS and r not in written:
                used.add(r)
        written |= writes
        if mn in LOADS and len(ops) > 1:
            m = re.match(r"^(-?(?:0x)?[0-9A-Fa-f]+)\(\$sp\)$", ops[1])
            if m and frame:
                off = int(m.group(1), 0)
                if off >= frame + 16 and off < frame + 16 + 0x40:
                    used.add("stack%d" % ((off - frame - 16) // 4 + 4))
    n = 0
    for i, r in enumerate(ARG_REGS):
        if r in used:
            n = i + 1
    stack = [int(s[5:]) for s in used if s.startswith("stack")]
    return max([n] + [k + 1 for k in stack])


def post_increment_old(text):
    """Symbols loaded into Rl, incremented into Rt (`addiu Rt, Rl, +-1`), stored back from Rt, with a
    compare or branch in between or after that reads Rl (the old value) rather than Rt."""
    insns = parse(text)
    found = set()
    for i, (mn, ops) in enumerate(insns):
        if mn not in LOADS or len(ops) < 2:
            continue
        m = re.match(r"^%lo\((\w+)\)\((\$\w+)\)$", ops[1])
        if not m:
            continue
        sym, rl = m.group(1), ops[0]
        for j in range(i + 1, min(i + 16, len(insns))):
            mn2, ops2 = insns[j]
            if mn2 in ("addiu", "addi") and len(ops2) == 3 and ops2[1] == rl and ops2[2] in ("1", "-1", "0x1"):
                rt = ops2[0]
                if rt == rl:
                    break
                stored = any(m3 in STORES and len(o3) > 1 and o3[0] == rt and ("%%lo(%s)" % sym) in o3[1]
                             for m3, o3 in insns[j + 1:j + 14])
                compared = any(k != j and mn3 not in LOADS and mn3 not in STORES and rl in o3
                               and mn3 in ("slti", "sltiu", "slt", "sltu", "xori", "xor", "beq", "bne", "beqz", "bnez",
                                           "andi", "sltiu")
                               for k, (mn3, o3) in enumerate(insns[i + 1:j + 14], i + 1))
                if stored and compared:
                    found.add(sym)
                break
            if mn2 in ("jal", "jalr") or mn2 in BRANCHES and j > i + 8:
                break
    return found


def rewrite_post_increment(draft, symbols):
    """m2c prints the store before the compare and then compares the variable (the new value); the asm
    compares the register that held the old value. For each symbol of `symbols` turn
        D += 1;            (or `D -= 1;`)
        if ((u8) D >= N) {
    into `if ((u8) D++ >= N) {`. Returns (text, [symbols rewritten])."""
    done = []
    for sym in sorted(symbols):
        pat = re.compile(r"^([ \t]*)%s[ \t]*([+-])=[ \t]*1;[ \t]*\n([ \t]*(?:if|while)[ \t]*\([^\n]*?)\b%s\b"
                         % (re.escape(sym), re.escape(sym)), re.M)
        new, n = pat.subn(lambda m: m.group(3) + sym + ("++" if m.group(2) == "+" else "--"),
                          draft)
        if n:
            draft = new
            done.append(sym)
    return draft, done
