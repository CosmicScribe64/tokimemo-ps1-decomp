"""T-25604 scratch: compile src/ovl/NAME_ENT/80142A80.c as is with the rev1 delta (../fresh-r1/rev1.py)
and align func_80143580 with build/T-25604/ref/rev1.s (local copy of the gitignored versions/ asm).
    tools/docker.sh python3 drafts/.../final_rev1.py"""
import sys, os
from pathlib import Path
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, 'tools'); sys.path.insert(0, HERE); sys.path.insert(0, os.path.join(HERE, '..', 'fresh-r1'))
import funcloc, order_search, score2, rev1
c = funcloc.locate(score2.NAME, "NAME_ENT")
obj = order_search.compile_variant(c, rev1.apply(Path(c.src).read_text()), Path("build/T-25604/final-rev1"), False)
d = score2.parse_dis(score2.dis(obj)); R = score2.parse_splat(score2.REV1)
h, pairs = score2.compare(d, R)
print("rev1: draft", len(d), "copy", len(R), "structural hunks", h, "register substitutions", sum(pairs.values()), dict(pairs))
# word check: every rev1 instruction without a relocation operand must equal the compiled word
import re, subprocess
words = []
out = subprocess.run(["mips-linux-gnu-objdump", "-dr", "--disassemble=" + score2.NAME, obj], capture_output=True, text=True).stdout
lines = out.splitlines()
for i, l in enumerate(lines):
    m = re.match(r"\s+[0-9a-f]+:\s+([0-9a-f]{8})\s", l)
    if m:
        rel = i + 1 < len(lines) and "R_MIPS" in lines[i + 1]
        words.append((int(m.group(1), 16), rel))
ref = []
for l in open(score2.REV1):
    m = re.match(r"\s*/\* [0-9A-F]+ [0-9A-F]+ ([0-9A-F]{8}) \*/\s+(\S+)\s*(.*)$", l)
    if m and not m.group(2).startswith('.'):
        ref.append((int(m.group(1), 16), bool(re.search(r"%|jal|func_|jtbl|D_|\.L|\. \+", m.group(3)) or m.group(2) in ('b', 'j', 'beq', 'bne', 'beqz', 'bnez', 'bgez', 'bltz', 'blez', 'bgtz'))))
bad = [k for k, ((w, r), (x, q)) in enumerate(zip(words, ref)) if not r and not q and w != x]
print("rev1 words", len(words), len(ref), "unrelocated mismatches", len(bad), bad[:10])
