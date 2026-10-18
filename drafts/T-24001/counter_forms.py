"""Reproduce T-24001 source-only counter experiments, inside Docker only.

Run: tools/docker.sh python3 drafts/T-24001/counter_forms.py [variant ...]
Reads the current guarded source and generated original asm; writes only to
.scratch/T-24001. The standalone object has three alignment nops after the
function, so its adiff tail is ignored. Final proof uses funcdiff --resolve.
"""
from pathlib import Path
import subprocess, sys
root = Path('.scratch/T-24001')
root.mkdir(parents=True, exist_ok=True)
base = Path('src/main/8006CB30.c').read_text()
start = base.index('/* T-23003: the 3-bit')
end = base.index('\n#else\nINCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006D7EC)', start)
source = '#include "common.h"\n#include "game.h"\n' + base[start:end] + '\n'
block = '''    idx = 0;
    if (day == monthDays) {
        D_800E6280.unk_69C[0].unk_00 = 0xFE;
        idx++;
        day = 0;
        monthOff = 1;
    }
    day++;'''
variants = {
'base': block,
'else-inits': '''    if (day == monthDays) {
        idx = 0;
        D_800E6280.unk_69C[0].unk_00 = 0xFE;
        idx++;
        day = 0;
        monthOff = 1;
    } else {
        idx = 0;
    }
    day++;''',
'switch-bool': block.replace('if (day == monthDays) {', 'switch (day == monthDays) { case 1:').replace('    }\n    day++;', '        break;\n    }\n    day++;'),
'unsigned-view': block.replace('idx++;', '(*(u32 *)&idx)++;'),
'byte-view': block.replace('idx++;', '(*(u8 *)&idx)++;'),
'half-view': block.replace('idx++;', '(*(u16 *)&idx)++;'),
'index-pointer': block.replace('idx++;', '((s32 *)((u8 *)&idx))[0]++;'),
'comma-increment': block.replace('idx++;', 'idx += monthOff + 1;'),
'zero-copy': block.replace('idx = 0;', 'idx = monthOff;'),
}
variants.update({
'array-counter': block,
'union-counter': block,
'struct-counter': block,
'char-store': block.replace('D_800E6280.unk_69C[0].unk_00 = 0xFE;', '*(u8 *)((u8 *)&D_800E6280 + 0x69C) = 0xFE;'),
'word-store': block.replace('D_800E6280.unk_69C[0].unk_00 = 0xFE;', '*(&D_800E6280.unk_69C[0].unk_00) = 0xFE;'),
'pointer-store': block.replace('D_800E6280.unk_69C[0].unk_00 = 0xFE;', '*(u8 *)((s32)&D_800E6280.unk_69C[0] + idx) = 0xFE;'),
'bitwise-store': block.replace('D_800E6280.unk_69C[0].unk_00 = 0xFE;', 'D_800E6280.unk_69C[0].unk_00 = ~idx - 1;'),
'idx-in-test': block.replace('if (day == monthDays)', 'if ((idx = 0) == day - monthDays)'),
})

variants.update({
'while-break': block.replace('if (day == monthDays)', 'while (day == monthDays)').replace('        monthOff = 1;', '        monthOff = 1;\n        break;'),
'for-break': block.replace('idx = 0;\n    if (day == monthDays)', 'for (idx = 0; day == monthDays;)').replace('        monthOff = 1;', '        monthOff = 1;\n        break;'),
'goto-path': block.replace('if (day == monthDays) {', 'if (day != monthDays) goto skip;\n    {').replace('    day++;', 'skip:\n    day++;'),
'post-condition': block.replace('if (day == monthDays)', 'if (!(day != monthDays))'),
})

for name in sys.argv[1:] or variants:
    text = source.replace(block, variants[name])
    if name == 'array-counter':
        text = text.replace('s32 idx;', 's32 idx_arr[1];')
        text = __import__('re').sub(r'\bidx\b', 'idx_arr[0]', text)
    if name in ('union-counter', 'struct-counter'):
        keyword = 'union' if name == 'union-counter' else 'struct'
        text = text.replace('s32 idx;', keyword + ' { s32 value; } counter;')
        text = __import__('re').sub(r'\bidx\b', 'counter.value', text)
    c = root / (name + '-try.c'); obj = root / (name + '.o'); dis = root / (name + '.dis')
    c.write_text(text)
    result = subprocess.run(['python3','tools/cc.py',str(c),str(obj),'ido','5.3'], capture_output=True,text=True)
    if result.returncode:
        print(name, 'ERROR', result.stderr[-800:], flush=True); continue
    with dis.open('w') as out:
        subprocess.run(['mips-linux-gnu-objdump','-dz',str(obj)], stdout=out, check=True)
    diff = subprocess.run(['python3','drafts/T-23003/adiff.py', str(dis), 'asm/nonmatchings/main/8006CB30/func_8006D7EC.s','1','2'], capture_output=True,text=True)
    print(name, diff.stdout, flush=True)
