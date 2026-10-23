"""T-25604 scratch helper: the rev1 source delta of NAME_ENT func_80143580 (cross-version check).

The rev1 copy (versions/asm/rev1/NAME_ENT/func_80143580.s, gitignored, 1111 instructions) differs
from the Best target in three places only; everything else, case 9's store order included, is the
same instruction sequence with other registers:
  - case 0 has no `D_8014D170 = 0;`
  - case 3 runs the body of func_80143498 (without its condition and without `D_8014D170 = 1`)
    inline, inside the `if`, after the selector advance; k_disp_start(3)
  - case 4 has no `func_80143498();` call at its head
apply() makes those edits on the text of a whole 80142A80.c variant. The three strings are taken
from func_80143498 in the same text, so this file holds no game text.
"""
import re

def apply(t, parts="034"):
    """parts: which edits to make (\"0\" case 0, \"3\" case 3 inline, \"4\" case 4 call)."""
    g = "D_800E6280."
    c0 = f"            {g}unk_110D++;\n            D_8014D170 = 0;\n"
    assert t.count(c0) == 1, "case 0 shape"
    if "0" in parts:
        t = t.replace(c0, f"            {g}unk_110D++;\n")
    c4 = "    case 4:\n        func_80143498();\n"
    assert t.count(c4) == 1, "case 4 shape"
    strings = re.findall(r'func_8004E788\(-0x90, 0x(14|24|34), 0, ("[^"]*"), 0\);', t)
    assert len(strings) == 3, strings
    inline = ("                func_80046290(0x06000CBB, 0x12000E0C, 0xF);\n"
              "                func_80044750(0x300);\n"
              "                func_8004E58C();\n" +
              "".join(f"                set_kanji_string(-0x90, 0x{y}, 0, {s}, 0);\n" for y, s in strings) +
              "                k_disp_start(3);\n"
              "                k_disp_switch(0, 0);\n"
              "                k_disp_switch(1, 0);\n"
              "                k_disp_switch(2, 0);\n")
    # the last statement of case 3's if-body is the advance right before case 4
    tail = f"                {g}unk_110D++;\n            }}\n        }}\n        break;\n" + c4
    assert t.count(tail) == 1, "case 3 tail shape"
    new = (f"                {g}unk_110D++;\n" + (inline if "3" in parts else "") +
           "            }\n        }\n        break;\n" + ("    case 4:\n" if "4" in parts else c4))
    return t.replace(tail, new)
