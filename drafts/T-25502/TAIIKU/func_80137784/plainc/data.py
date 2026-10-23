#!/usr/bin/env python3
"""Experiment helper (T-25502): define the slot objects in C instead of island pieces.

usage: data.py MODE   MODE = static | global | restore
static/global: replace the five INCLUDE_RODATA lines D_8014981C..D_8014985C by C definitions
(with `static` or without) and drop their header externs; restore: git checkout both files' data parts
is done by the caller (git checkout -- include/ovl/TAIIKU.h, then swap.py re-applies the function).
"""
import sys, pathlib, re
root = pathlib.Path(__file__).resolve().parents[5]
src = root / "src/ovl/TAIIKU/80133C80.c"
hdr = root / "include/ovl/TAIIKU.h"
mode = sys.argv[1]
st = "static " if mode == "static" else ""
defs = {
    "D_8014981C": "%ss32 D_8014981C[4] = { 0 };" % st,
    "D_8014982C": "%ss32 D_8014982C[4] = { 0 };" % st,
    "D_8014983C": "%ss32 D_8014983C[4] = { 0 };" % st,
    "D_8014984C": "%sFix32 D_8014984C[4] = { 0 };" % st,
    "D_8014985C": "%sTkS16x4 D_8014985C = { { 0x1400, 0x1400, 0x13F0, 0x0070 } };" % st,
}
only = sys.argv[2].split(",") if len(sys.argv) > 2 else list(defs)
s = src.read_text()
for k in only:
    line = 'INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80133C80.data", %s);' % k
    assert line in s, k
    s = s.replace(line, defs[k])
src.write_text(s)
h = hdr.read_text()
for k in only:
    h2 = re.sub(r"^extern [A-Za-z0-9_]+ %s(\[4\])?;.*\n" % k, "", h, flags=re.M)
    assert h2 != h, k
    h = h2
hdr.write_text(h)
