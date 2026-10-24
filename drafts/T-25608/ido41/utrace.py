#!/usr/bin/env python3
"""Print the block-copy state ops of ugen input ucode and each rule's states (T-25608).

Usage (Docker, cwd /work): python3 drafts/T-25608/ido41/utrace.py U.O [NAME=ugen_pass.py ...]
One line per procedure that has a Umov (its index, as in ug41_shim.py UG41_FORCE): the ops the state rules read (I istr/istv, S str,
C cup/icuf, . lab, u ujp, M mov, f/t fjp/tjp followed by [cmp const] when a compare against a
constant feeds it, `<` marked when the build's detector counts it as a signed-immediate `<`),
then the B/F sequence that each named ugen_pass.py variant's rewrite_ucode gives its Umovs.
"""
import importlib.util
import os
import sys

sys.path.insert(0, "/work/tools")
import ugen_pass as base  # noqa: E402

OP = base.OP
NAMES = {v: k for k, v in OP.items()}


def load(path, name):
    spec = importlib.util.spec_from_file_location(name, path)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def jump_note(insns, start, k):
    idx = k - 1
    while idx > start and insns[idx].opc in base.SKIP_OPS:
        idx -= 1
    neg = ""
    while idx > start and insns[idx].opc == OP["lnot"]:
        neg += "!"
        idx -= 1
    cmp_ins = insns[idx]
    rhs = insns[idx - 1]
    if NAMES.get(cmp_ins.opc) in ("les", "leq", "geq", "grt", "equ", "neq") and rhs.opc == OP["ldc"]:
        return "[%s%s %d]" % (neg, NAMES[cmp_ins.opc], base.s32(rhs.words[4]))
    return ""


def trace(insns, start, end):
    out = []
    for k in range(start + 1, end):
        ins = insns[k]
        o = ins.opc
        if o in (OP["istr"], OP["istv"]):
            out.append("I")
        elif o == OP["str"] and ins.mtype != base.MT_R:
            out.append("S")
        elif o in (OP["cup"], OP["icuf"]):
            out.append("C")
        elif o == OP["lab"]:
            out.append(".")
        elif o == OP["ujp"]:
            out.append("u")
        elif o == OP["mov"]:
            out.append("M")
        elif o in (OP["fjp"], OP["tjp"]):
            s = "f" if o == OP["fjp"] else "t"
            s += jump_note(insns, start, k)
            if base._is_signed_blt_pos_imm_jump(insns, start, k):
                s += "<"
            out.append(s)
    return "".join(out)


def rule_states(mod, insns, start, end):
    """B/F of each Umov in [start, end) under `mod`'s rewrite_ucode (read from its gadgets)."""
    new, _ = mod.rewrite_ucode(insns[start:end + 1])
    # one gadget per Umov, inserted before its operands: str (B) or istr (F) at SENTINEL_OFF
    seq = []
    for ins in new:
        if ins.opc == OP["str"] and ins.words[3] == base.SENTINEL_OFF:
            seq.append("B")
        elif ins.opc == OP["istr"] and ins.words[1] == base.SENTINEL_OFF:
            seq.append("F")
    return "".join(seq)


def main(argv):
    if len(argv) < 2:
        sys.exit(__doc__)
    with open(argv[1], "rb") as f:
        insns = base.parse_ucode(f.read())
    rules = []
    for a in argv[2:]:
        name, path = a.split("=", 1)
        rules.append((name, load(path, "ugp_" + name)))
    n = 0
    for start, end in base._procedures(insns):
        if not any(insns[k].opc == OP["mov"] for k in range(start, end)):
            continue
        print("%d ops:" % n, trace(insns, start, end))
        n += 1
        print("    " + "  ".join("%s=%s" % (name, rule_states(mod, insns, start, end)) for name, mod in rules))


if __name__ == "__main__":
    main(sys.argv)
