#!/usr/bin/env python3
"""Create experimental uniform uopt variants from pinned recomp output (T-9800).

Not a production build step. Input must be the unedited IDO 5.3 uopt.c generated
by ido-static-recomp 9c242ad. Output belongs in external scratch space. These
patches alter allocator decisions for every procedure, with no source markers
or function-name exceptions. The reference reconstruction is not included.
See wiki/uopt-register-allocation.md for hypotheses and rejection scores.
"""

import argparse
import hashlib
from pathlib import Path


SOURCE_SHA256 = "b0058f1559441c1a194d649271eb43b8637ec255682cfdd629031340b915b13f"
CANDIDATES = ("trace", "zero_threshold", "free_entry_load", "half_move_cost",
              "late_priority_tie", "late_constant_tie", "reverse_unconstrained", "prefer_v1", "prefer_a2a3")


def replace_exact(text, old, new, count=1):
    if text.count(old) != count:
        raise ValueError("unexpected recomp anchor count for " + repr(old))
    return text.replace(old, new)


def function_edit(text, name, edit):
    start = text.index("static void f_" + name + "(", text.index("static void f_" + name + "(") + 1)
    end = text.index("\n}\n", start) + 3
    return text[:start] + edit(text[start:end]) + text[end:]


def patch(text, candidate):
    if hashlib.sha256(text.encode()).hexdigest() != SOURCE_SHA256:
        raise ValueError("uopt.c does not match the pinned pristine recomp output")
    if candidate == "trace":
        def trace(body):
            return replace_exact(body, "L46f254:\n", """L46f254:
{
    float priority;
    uint32_t bits = MEM_U32(s1 + 48);
    memcpy(&priority, &bits, sizeof(priority));
    fprintf(stderr, "R9SAVE bit=%u lr=%u blocks=%u priority=%.9g state=%u\\n",
            MEM_U16(MEM_U32(s1) + 2), MEM_U32(s1 + 4), MEM_U32(s1 + 28),
            (double)priority, MEM_U8(s1 + 35));
    for (uint32_t unit = MEM_U32(s1 + 8); unit; unit = MEM_U32(unit + 4)) {
        uint32_t node = MEM_U32(unit);
        fprintf(stderr, "R9UNIT bb=%u freq=%u loads=%u stores=%u load=%u save=%u dead=%u\\n",
                MEM_U16(node + 8), MEM_U32(node + 44), MEM_U16(unit + 16),
                MEM_U8(unit + 18), MEM_U8(unit + 21), MEM_U8(unit + 22), MEM_U8(unit + 23));
    }
}
""")
        text = function_edit(text, "compute_save", trace)
        def trace_color(body):
            start = body.index("\n") + 1
            body = body[:start] + """fprintf(stderr, "R9PROC ");
for (unsigned i = 0; i < MEM_U32(0x1001c8d0) && i < 1024; i++)
    fputc(MEM_U8(0x1001c4d0 + i), stderr);
fprintf(stderr, "\\nR9POOL caller=%u..%u saved=%u..%u forbidden=%08x\\n",
        MEM_U32(0x1001e5d0), MEM_U32(0x1001e5e0), MEM_U32(0x1001e5d8),
        MEM_U32(0x1001e5e8), MEM_U32(0x1001e660));
""" + body[start:]
            for label in ("L471b74", "L472628"):
                body = replace_exact(body, label + ":\n", label + ":\n" + """fprintf(stderr, "R9COST bit=%u lr=%u cost=%.9g reg=%u\\n",
        MEM_U16(MEM_U32(s5) + 2), MEM_U32(s5 + 4),
        (double)f20.f[0], MEM_U32(sp + 220));
""")
            return body
        return function_edit(text, "globalcolor", trace_color)
    if candidate == "zero_threshold":
        text = function_edit(text, "compute_save", lambda body: replace_exact(
            body, "cf = f18.f[0] < f8.f[0];", "cf = f18.f[0] <= f8.f[0];"))
        def threshold(body):
            body = replace_exact(body, "cf = f6.f[0] <= f20.f[0];", "cf = f6.f[0] < f20.f[0];")
            return replace_exact(body, "cf = f20.f[0] < f10.f[0];", "cf = f20.f[0] <= f10.f[0];")
        return function_edit(text, "globalcolor", threshold)
    if candidate == "free_entry_load":
        return function_edit(text, "compute_save", lambda body: replace_exact(
            body, "f20.f[0] = f20.f[0] - f4.f[0];",
            "if (MEM_U32(s0) != MEM_U32(0x1001c8f8)) f20.f[0] -= f4.f[0];"))
    if candidate == "half_move_cost":
        # Halve live-unit reload/store penalties; call costs stay unchanged.
        return function_edit(text, "compute_save", lambda body: replace_exact(
            replace_exact(body, "f20.f[0] = f20.f[0] - f4.f[0];", "f20.f[0] -= 0.5f * f4.f[0];"),
            "f20.f[0] = f20.f[0] - f8.f[0];", "f20.f[0] -= 0.5f * f8.f[0];"))
    if candidate == "late_priority_tie":
        return function_edit(text, "globalcolor", lambda body: replace_exact(
            body, "cf = f20.f[0] < f0.f[0];", "cf = f20.f[0] <= f0.f[0];"))
    if candidate == "late_constant_tie":
        # Refine the observed TT gain by expression class, never by game site.
        return function_edit(text, "globalcolor", lambda body: replace_exact(
            body, "cf = f20.f[0] < f0.f[0];", """cf = f20.f[0] < f0.f[0];
if (!cf && f20.f[0] == f0.f[0] && MEM_U32(sp + 268) != 0xffffffff) {
    uint32_t best_lr = MEM_U32(MEM_U32(0x1001cc30) + 8 * MEM_U32(sp + 268) + 4);
    cf = MEM_U8(MEM_U32(s5)) == 2 && MEM_U8(MEM_U32(best_lr)) == 2;
}"""))
    if candidate == "reverse_unconstrained":
        def reverse(body):
            body = replace_exact(body, "MEM_U32(sp + 272) = zero;\ns7 = s7 + 0xfffffffc;",
                                 "MEM_U32(sp + 272) = v1 - 1;\ns7 = s7 + 0xfffffffc;")
            return replace_exact(body, "t7 = t8 + 0x1;\nif (t7 != t2) {",
                                 "t7 = t8 - 1;\nif (t8 != 0) {")
        return function_edit(text, "globalcolor", reverse)
    if candidate in ("prefer_v1", "prefer_a2a3"):
        rank = ("return reg == 2 ? 0 : reg;" if candidate == "prefer_v1" else
                "return reg == 3 || reg == 4 ? reg + 2 : reg == 5 || reg == 6 ? reg - 2 : reg;")
        text = "static unsigned r9_rank(unsigned reg) { " + rank + " }\n" + text
        return function_edit(text, "globalcolor", lambda body: replace_exact(
            body, "cf = f2.f[0] < f20.f[0];",
            "cf = f2.f[0] < f20.f[0] || (f2.f[0] == f20.f[0] && r9_rank(s2) < r9_rank(MEM_U32(sp + 220)));", 4))
    raise ValueError("unknown candidate: " + candidate)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("candidate", choices=CANDIDATES)
    parser.add_argument("source", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    args.output.write_text(patch(args.source.read_text(), args.candidate))


if __name__ == "__main__":
    main()
