#!/usr/bin/env python3
"""adiff.py MINE.dis ORIG.s [maxhunks] : opcode-aligned comparison of a built function against the original asm."""
import re, sys, difflib
mine_lines = []
for l in open(sys.argv[1]):
    m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', l)
    if m:
        mine_lines.append((int(m.group(2), 16), m.group(3).strip()))
orig = []
for l in open(sys.argv[2]):
    m = re.match(r'\s*/\* [0-9A-F]+ [0-9A-F]+ ([0-9A-F]{8}) \*/\s+(.*)', l)
    if m:
        b = bytes.fromhex(m.group(1))
        orig.append((int.from_bytes(b, 'little'), m.group(2).strip()))
def key(w):
    op = w >> 26
    if op == 0:
        return ('R', w & 0x3F)
    if op == 1:
        return ('B1', (w >> 16) & 0x1F)
    return ('I', op)
def regs(w):
    op = w >> 26
    if op == 0:
        return ((w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31)
    if op in (2, 3):
        return ()
    return ((w >> 21) & 31, (w >> 16) & 31)
ka = [key(w) for w, _ in orig]
kb = [key(w) for w, _ in mine_lines]
sm = difflib.SequenceMatcher(None, ka, kb, autojunk=False)
maxh = int(sys.argv[3]) if len(sys.argv) > 3 else 12
struct = 0
regonly = 0
hunks = []
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag == 'equal':
        for k in range(i2 - i1):
            wo = orig[i1 + k][0]
            wm = mine_lines[j1 + k][0]
            if regs(wo) != regs(wm):
                regonly += 1
    else:
        struct += max(i2 - i1, j2 - j1)
        hunks.append((tag, i1, i2, j1, j2))
print('orig %d instr, mine %d instr; structural-diff instr %d in %d hunks; reg-differences in aligned instr %d' % (len(orig), len(mine_lines), struct, len(hunks), regonly))
for tag, i1, i2, j1, j2 in hunks[:maxh]:
    print('--- hunk %s orig[%d:%d] mine[%d:%d]' % (tag, i1, i2, j1, j2))
    for t in orig[max(0, i1 - 2):i1]:
        print('   ', t[1][:50])
    for t in orig[i1:min(i2, i1 + 8)]:
        print('  -', t[1][:60])
    for t in mine_lines[j1:min(j2, j1 + 8)]:
        print('  +', t[1][:60])

if len(sys.argv) > 4:
    n = int(sys.argv[4])
    c = 0
    print('--- first %d aligned register differences' % n)
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'equal':
            for k in range(i2 - i1):
                wo = orig[i1 + k][0]; wm = mine_lines[j1 + k][0]
                if regs(wo) != regs(wm):
                    print('%5d  %-44s | %s' % (i1 + k, orig[i1 + k][1][:44], mine_lines[j1 + k][1][:40]))
                    c += 1
                    if c >= n:
                        sys.exit(0)
