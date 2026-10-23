#!/usr/bin/env python3
"""Put a variant of func_80137784 into src/ovl/TAIIKU/80133C80.c (experiment helper, T-25502).

usage: swap.py VARIANT.c [--proto 'DECL']
Replaces the text between the `#ifdef NON_MATCHING` that guards func_80137784 and its
`#else` / INCLUDE_ASM line with VARIANT.c. --proto replaces the header declaration
`s32 func_80137784(s32 arg0);` in include/ovl/TAIIKU.h (for K&R definitions).
"""
import sys, re, pathlib
root = pathlib.Path(__file__).resolve().parents[5]
src = root / "src/ovl/TAIIKU/80133C80.c"
hdr = root / "include/ovl/TAIIKU.h"
var = pathlib.Path(sys.argv[1]).read_text()
lines = src.read_text().split("\n")
end = next(i for i, l in enumerate(lines) if 'INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80133C80", func_80137784)' in l)
assert lines[end - 1] == "#else", lines[end - 1]
start = max(i for i in range(end) if lines[i] == "#ifdef NON_MATCHING")
new = lines[: start + 1] + var.rstrip("\n").split("\n") + lines[end - 1 :]
src.write_text("\n".join(new))
h = hdr.read_text()
m = re.search(r"^s32 func_80137784\(.*\);$", h, re.M)
proto = sys.argv[sys.argv.index("--proto") + 1] if "--proto" in sys.argv else "s32 func_80137784(s32 arg0);"
hdr.write_text(h[: m.start()] + proto + h[m.end():])
