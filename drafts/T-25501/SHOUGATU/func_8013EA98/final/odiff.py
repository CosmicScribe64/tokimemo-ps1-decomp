#!/usr/bin/env python3
"""odiff.py LABEL: instruction diff of build/ea98/LABEL against the target asm (pseudo-ops normalised)."""
import re, difflib, sys
def norm(s):
    s = s.replace('$', '').replace('\t', ' ').strip()
    op, _, args = s.partition(' ')
    s = op + ' ' + args.replace(' ', '')
    s = re.sub(r'^or (\w+),(\w+),zero$', r'move \1,\2', s)
    s = re.sub(r'^or (\w+),zero,zero$', r'move \1,zero', s)
    s = re.sub(r'^addiu (\w+),zero,(\S+)$', lambda m: 'li %s,%d' % (m.group(1), int(m.group(2), 0)), s)
    s = re.sub(r'%(hi|lo)\([^)]*\)', '0', s)
    s = re.sub(r',[0-9a-f]+<[^>]*>$', ',L', s)
    s = re.sub(r'(0x[0-9a-fA-F]+|\d+)', lambda m: str(int(m.group(1), 0)), s)
    s = re.sub(r'^(b|j|jal) .*', r'\1 L', s)
    s = re.sub(r',\.L\w+', ',L', s); s = re.sub(r'^(b\w*) \.L\w+', r'\1 L', s)
    s = re.sub(r'(-?0x[0-9a-fA-F]+|\b\d+)\(', '0(', s)
    return s
a = [norm(l.split('\t', 2)[-1].strip()) for l in open('build/ea98/%s/dis.txt' % sys.argv[1]) if re.match(r'^\s+[0-9a-f]+:\t[0-9a-f]{8} \t', l)]
t = []
for l in open('asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013E330/func_8013EA98.s'):
    m = re.match(r'\s*/\* \w+ \w+ \w+ \*/\s+(\S+)\s*(.*)', l)
    if m: t.append(norm(m.group(1) + ' ' + m.group(2)))
n = 0
for op in difflib.SequenceMatcher(None, t, a, autojunk=False).get_opcodes():
    if op[0] != 'equal':
        n += 1; print(op[0], op[1], t[op[1]:op[2]], '->', a[op[3]:op[4]])
print('blocks', n)
