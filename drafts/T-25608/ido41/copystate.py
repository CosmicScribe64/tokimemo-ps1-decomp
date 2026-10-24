"""Block-copy state of each end-pointer copy loop in an `objdump -dr` listing (T-19001, T-25608).

A struct copy longer than ugen's unrolled limit is a loop `addiu END, SRC, n` ... `bne SRC, END, top`.
END is allocated after SRC from ugen's rotation t6..t9, t0..t5: two steps on (SRC + 2) is state F,
three (SRC + 3) is state B. Each copy is reported as `B(t7>t0)`, `F(t9>t1)` or `?(..)`.
"""
import re

ROT = ["t6", "t7", "t8", "t9", "t0", "t1", "t2", "t3", "t4", "t5"]
FUNC = re.compile(r"^[0-9a-f]+ <([^>]+)>:")
INSN = re.compile(r"^\s*([0-9a-f]+):\s+[0-9a-f]{8}\s+(\S+)\s*(.*)$")


def functions(dis):
    """[(name, [(addr, op, [operands])])] in listing order."""
    out = []
    for line in dis.splitlines():
        m = FUNC.match(line)
        if m:
            out.append((m.group(1), []))
            continue
        m = INSN.match(line)
        if m and out:
            out[-1][1].append((int(m.group(1), 16), m.group(2), [x.strip() for x in m.group(3).split(",")]))
    return out


def state_of(src, end):
    if src in ROT and end in ROT:
        step = (ROT.index(end) - ROT.index(src)) % len(ROT)
        return {2: "F", 3: "B"}.get(step, "?")
    return "?"


def states(dis):
    """[(function, ["B(t7>t0)", ...])] for every function of the listing."""
    res = []
    for name, insns in functions(dis):
        found = []
        for i, (addr, op, ops) in enumerate(insns):
            if op != "bne" or len(ops) != 3:
                continue
            m = re.match(r"([0-9a-f]+)", ops[2])
            if not m or int(m.group(1), 16) >= addr:
                continue
            top = int(m.group(1), 16)
            body = [x for x in insns if top <= x[0] <= addr + 4]
            # a copy loop moves words through $at (lw/lwl/lwr ... at)
            if not any(o in ("lw", "lwl", "lwr") and p and p[0] == "at" for _, o, p in body):
                continue
            a, b = ops[0], ops[1]
            for _, op2, ops2 in reversed(insns[:i]):
                if op2 == "addiu" and len(ops2) == 3 and ops2[0] in (a, b) and ops2[1] in (a, b) \
                        and ops2[0] != ops2[1]:
                    src, end = ops2[1], ops2[0]
                    found.append("%s(%s>%s)" % (state_of(src, end), src, end))
                    break
        res.append((name, found))
    return res
