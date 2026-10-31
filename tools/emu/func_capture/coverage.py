"""T-39301: coverage of captured hits: which basic blocks of each function no hit executed.

usage: python3 -I coverage.py <capture dir>... --asm <asm dir>

Reads <fn>/hitNNNN.json ("executed": instruction addresses recorded by run.sh --cover) of every capture dir given and the
function's asm (asm/matchings/main/<file>/<fn>.s or asm/*.s: the instruction addresses are in its comments) and prints,
per function: instructions, executed, and the unreached basic blocks (start address, instruction count, label). A basic
block starts at the function start, at a branch/jump target label and after a branch's delay slot. Needs captures made with
--cover. Game data stays under .asset-cache; this prints addresses and counts only.
"""
import glob
import json
import os
import re
import sys

INSN = re.compile(r'/\*\s*[0-9A-F]+\s+([0-9A-F]{8})\s+[0-9A-F]{8}\s*\*/\s*(\S+)')
LABEL = re.compile(r'^\s*(?:jlabel|dlabel)?\s*\.?(L[0-9A-F]{8}):|^\s*jlabel\s+\.?(L[0-9A-F]{8})')
BRANCH = ('b', 'beq', 'bne', 'beqz', 'bnez', 'blez', 'bgtz', 'bltz', 'bgez', 'bltzal', 'bgezal', 'j', 'jr', 'jal', 'jalr',
          'beql', 'bnel', 'bc0f', 'bc0t')


def load_asm(asm_dir):
    funcs = {}
    for path in glob.glob(os.path.join(asm_dir, '*.s')) + glob.glob(os.path.join(asm_dir, 'matchings', 'main', '*', '*.s')):
        name = None
        for line in open(path, errors='ignore'):
            if line.startswith('glabel '):
                name = line.split()[1]
                funcs[name] = []
            elif line.startswith('endlabel '):
                name = None
            elif name:
                m = INSN.search(line)
                if m:
                    funcs[name].append(('insn', int(m.group(1), 16), m.group(2)))
                else:
                    lm = re.search(r'\.?(L[0-9A-F]{8})\b', line)
                    if lm and ':' in line or 'jlabel' in line:
                        funcs[name].append(('label', 0, lm.group(1) if lm else ''))
    return funcs


def blocks(items):
    """List of (start address, [addresses], label)."""
    out = []
    current = None
    pending_label = ''
    after_branch = 0
    for kind, address, text in items:
        if kind == 'label':
            if current:
                out.append(current)
                current = None
            pending_label = text
            continue
        if current is None:
            current = (address, [], pending_label)
            pending_label = ''
        current[1].append(address)
        if after_branch:
            after_branch -= 1
            if after_branch == 0:
                out.append(current)
                current = None
        elif text in BRANCH:
            after_branch = 1
    if current:
        out.append(current)
    return out


def main(argv):
    asm_dir = argv[argv.index('--asm') + 1]
    dirs = [a for a in argv[1:] if not a.startswith('--') and a != asm_dir]
    funcs = load_asm(asm_dir)
    executed = {}
    for d in dirs:
        for fn_dir in glob.glob(os.path.join(d, '*/')):
            name = os.path.basename(os.path.dirname(fn_dir))
            for js in glob.glob(os.path.join(fn_dir, 'hit*.json')):
                hit = json.load(open(js))
                executed.setdefault(name, set()).update(hit.get('executed', []))
    for name in sorted(executed):
        if name not in funcs:
            print('%-30s no asm found' % name)
            continue
        items = funcs[name]
        addresses = [a for k, a, _ in items if k == 'insn']
        seen = executed[name]
        hit = sum(1 for a in addresses if a in seen)
        print('%-30s %4d/%4d instructions executed' % (name, hit, len(addresses)))
        for start, addrs, label in blocks(items):
            if not any(a in seen for a in addrs):
                print('    unreached block 0x%08X (%d instruction(s)) %s' % (start, len(addrs), label))


main(sys.argv)
