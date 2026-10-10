#!/usr/bin/env python3
"""Entry rule of the unsigned-load conversion pass, seen from the original asm (T-5010).

tools/cvt_pass.py (in the build since T-5010) reproduces the original's register promotion of an
unsigned byte or halfword global only when the procedure touches the global before its first call,
branch or label. The original does the same: a switch or compare chain on such a global keeps the
selector in $v1 when it is loaded at the entry and in $v0 when a call or branch comes first
(wiki/matching-notes.md, "Selector register rule (T-5010)"). tools/queue.py uses these helpers to
drop the R, V and U1 flags that the pass now covers and to keep the ones it cannot reproduce:

  * U1 (selector in $v1): blocked only when the selector is loaded after a call or branch;
  * U0 (selector in $v0): blocked-unknown only when the selector is loaded at the entry (the pass
    gives $v1 there; the original's $v0 is unit-private data or an unexplained exception);
  * R, V: only for a global whose first load comes after a call or branch.

The helpers work on tools/queue.py's parsed instructions (addr, op, args) and label addresses.
Tests: tools/test_entry_rule.py.
"""

CALLS = {"jal", "jalr"}
FLOW = {"b", "j", "jr", "beq", "bne", "beqz", "bnez", "bgez", "bgtz", "blez", "bltz", "bgezal", "bltzal",
        "beql", "bnel", "beqzl", "bnezl", "bgezl", "bgtzl", "blezl", "bltzl"}


def at_entry(insns, labels, k):
    """True if instruction k comes before the function's first call, branch, jump and label."""
    for i in insns[:k]:
        if i.op in CALLS or i.op in FLOW:
            return False
    return not any(i.addr in labels for i in insns[1:k + 1])


def late_globals(insns, labels, loads):
    """Keys of the globals whose first load (loads: tuples ending with the instruction index, with the
    key at position 2, as tools/queue.py's analyze builds them) is not at the entry."""
    first = {}
    for load in loads:
        key, k = load[2], load[-1]
        first[key] = min(k, first.get(key, k))
    return {key for key, k in first.items() if not at_entry(insns, labels, k)}


def selector_flag(reg, entry):
    """The U flag register to keep for a switch selector in `reg`, or "" when the build reproduces it:
    $v1 at the entry (cvt_pass) and $v0 after a call or branch (stock IDO)."""
    if (reg == "v1" and entry) or (reg == "v0" and not entry):
        return ""
    return reg
