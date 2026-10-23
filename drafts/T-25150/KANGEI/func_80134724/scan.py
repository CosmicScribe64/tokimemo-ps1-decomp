"""scan.py : find jump-table switch heads (sltiu at,rX,N; beqz at,L; slot) in the built ELFs
and classify the range-check delay slot vs the default block's first instruction."""
import subprocess, re, glob, sys
from collections import Counter
elfs = sorted(glob.glob('build/ovl/*.elf')) + ['build/SLPM_86.053.elf']
stats = Counter()
for elf in elfs:
    out = subprocess.run(['mips-linux-gnu-objdump', '-d', '-z', '--no-show-raw-insn', elf],
                         capture_output=True, text=True).stdout
    ins = []  # (addr, text, func)
    func = None
    for l in out.splitlines():
        m = re.match(r'^([0-9a-f]+) <(.*)>:', l)
        if m: func = m.group(2); continue
        m = re.match(r'^\s*([0-9a-f]+):\s+(\S+)\s*(.*)$', l)
        if m: ins.append((int(m.group(1), 16), m.group(2), m.group(3).replace(' ', ''), func))
    at = {a: i for i, (a, _, _, _) in enumerate(ins)}
    import struct
    words = set()
    dump = subprocess.run(['mips-linux-gnu-objdump', '-s', '-R' if False else '-s', elf], capture_output=True, text=True).stdout
    sec = None
    for l in dump.splitlines():
        if l.startswith('Contents of section'): sec = l.split()[-1].rstrip(':'); continue
        if sec in ('.text',) or sec is None: continue
        parts = l.split()
        for w in parts[1:5]:
            if len(w) == 8:
                try: words.add(struct.unpack('<I', bytes.fromhex(w))[0])
                except ValueError: pass
    tg = Counter()
    for (a2, op2, ar2, fn2) in ins:
        m2 = re.search(r'([0-9a-f]{8})<', ar2)
        if m2 and (op2.startswith('b') or op2 == 'j'): tg[int(m2.group(1), 16)] += 1
    for i in range(len(ins) - 3):
        a, op, ar, fn = ins[i]
        if op != 'sltiu' or not ar.startswith('at,'): continue
        sel = ar.split(',')[1]
        b = ins[i + 1]
        if b[1] != 'beqz' or not b[2].startswith('at,'): continue
        tgt = int(re.match(r'at,([0-9a-f]+)', b[2]).group(1), 16)
        slot = ins[i + 2]
        disp = slot[1] == 'sll' and slot[2] == '%s,%s,0x2' % (sel, sel)
        j = at.get(tgt)
        if j is None: continue
        first = ins[j]
        kind = 'disp' if disp else 'other'
        stats[kind] += 1
        if disp and first[1] not in ('lui', 'lw', 'lh', 'lhu', 'lb', 'lbu', 'sw', 'sh', 'sb', 'jal', 'beq', 'bne', 'j', 'jr', 'sll' if False else 'x'):
            dst = first[2].split(',')[0]
            print('%-14s %-14s %08x sel=%s default: %-6s %-22s table=%d branches=%d prev=%s' % (elf.split('/')[-1], fn, a, sel, first[1], first[2], tgt in words, tg[tgt], ins[j-2][1]+' '+ins[j-2][2][:20]))
print(stats, file=sys.stderr)
