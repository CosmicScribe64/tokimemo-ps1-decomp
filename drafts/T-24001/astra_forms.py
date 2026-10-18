"""Reproduce T-24001 Astra counter hypotheses inside Docker only.

Run tools/docker.sh python3 drafts/T-24001/astra_forms.py [variant ...].
Reads the guarded source and generated original asm; saves transient sources,
objects and diagnostics under .scratch/T-24001-astra. Alignment nops after
func_8006D7EC are not function differences; final proof uses funcdiff --resolve.
The float-union case is retained as a compiler-error diagnostic, not a match.
"""
from pathlib import Path
import re, subprocess, sys
root = Path('.scratch/T-24001-astra')
root.mkdir(parents=True, exist_ok=True)
base = Path('src/main/8006CB30.c').read_text()
start = base.index('/* T-23003: the 3-bit')
end = base.index('\n#else\nINCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006D7EC)', start)
source = '#include "common.h"\n#include "game.h"\n' + base[start:end] + '\n'
variants = {
'float-init': source.replace('idx = 0;', '*(float *)&idx = 0.0f;'),
'union-float': source.replace('s32 idx;', 'union { s32 i; float f; } counter;').replace('idx = 0;', 'counter.f = 0.0f;'),
'signed-bitfield': source.replace('s32 idx;', 'struct { signed int i : 32; } counter;'),
'unsigned-bitfield': source.replace('s32 idx;', 'struct { unsigned int i : 32; } counter;'),
'volatile-init': source.replace('idx = 0;', '*(volatile s32 *)&idx = 0;'),
'empty-dynamic': source.replace('        idx++;\n        day = 0;', '        if (day) { }\n        idx++;\n        day = 0;'),
'empty-counter': source.replace('        idx++;\n        day = 0;', '        if (idx) { }\n        idx++;\n        day = 0;'),
}
variants.update({
'do-lt': source.replace('        idx++;\n        day = 0;', '        do { idx++; } while (idx < 1);\n        day = 0;'),
'do-ne': source.replace('        idx++;\n        day = 0;', '        do { idx++; } while (idx != 1);\n        day = 0;'),
'while-lt': source.replace('        idx++;\n        day = 0;', '        while (idx < 1) { idx++; }\n        day = 0;'),
'goto-loop': source.replace('        idx++;\n        day = 0;', 'counter_loop:\n        idx++;\n        if (idx < 1) goto counter_loop;\n        day = 0;'),
'do-sign': source.replace('        idx++;\n        day = 0;', '        do { idx++; } while (idx < 0);\n        day = 0;'),
'do-body': source.replace('        D_800E6280.unk_69C[0].unk_00 = 0xFE;', '        do {\n        D_800E6280.unk_69C[0].unk_00 = 0xFE;').replace('        monthOff = 1;\n    }', '        monthOff = 1;\n        } while (idx < 1);\n    }'),
})
block = '    idx = 0;\n    if (day == monthDays) {\n        D_800E6280.unk_69C[0].unk_00 = 0xFE;\n        idx++;\n        day = 0;\n        monthOff = 1;\n    }\n    day++;'
variants.update({
'for-inner': source.replace(block, '    idx = 0;\n    if (day == monthDays) {\n        for (idx = 0; idx < 1; idx++) {\n        D_800E6280.unk_69C[0].unk_00 = 0xFE;\n        day = 0;\n        monthOff = 1;\n        }\n    }\n    day++;'),
'for-outer': source.replace(block, '    for (idx = 0; idx < 1; idx++) {\n        if (day != monthDays) break;\n        D_800E6280.unk_69C[0].unk_00 = 0xFE;\n        day = 0;\n        monthOff = 1;\n    }\n    day++;'),
'for-shared-init': source.replace(block, '    idx = 0;\n    if (day == monthDays) {\n        for (; idx < 1; idx++) {\n        D_800E6280.unk_69C[0].unk_00 = 0xFE;\n        day = 0;\n        monthOff = 1;\n        }\n    }\n    day++;'),
'for-inner-ne': source.replace(block, '    idx = 0;\n    if (day == monthDays) {\n        for (idx = 0; idx != 1; idx++) {\n        D_800E6280.unk_69C[0].unk_00 = 0xFE;\n        day = 0;\n        monthOff = 1;\n        }\n    }\n    day++;'),
})
block = '    if (D_800E6280.unk_1BC[11].unk_14[0] != 0) {\n        D_800E6280.unk_1BC[11].unk_14[0] -= 1;\n    }'
variants.update({
'switch-timer': source.replace(block, '    switch (D_800E6280.unk_1BC[11].unk_14[0]) {\n    case 0: break;\n    default:\n        D_800E6280.unk_1BC[11].unk_14[0] -= 1;\n        break;\n    }'),
'switch-timer-default-first': source.replace(block, '    switch (D_800E6280.unk_1BC[11].unk_14[0]) {\n    default:\n        D_800E6280.unk_1BC[11].unk_14[0] -= 1;\n        break;\n    case 0: break;\n    }'),
})
compile_failed = False
for name in sys.argv[1:] or variants:
    text = variants[name]
    if name in ('union-float', 'signed-bitfield', 'unsigned-bitfield'):
        text = re.sub(r'\bidx\b', 'counter.i', text)
        if 'bitfield' in name:
            text = text.replace('&counter.i', '(s32 *)&counter')
    c = root / (name + '.c'); obj = root / (name + '.o'); dis = root / (name + '.dis')
    c.write_text(text)
    result = subprocess.run(['python3','tools/cc.py',str(c),str(obj),'ido','5.3'], capture_output=True,text=True)
    if result.returncode:
        compile_failed = True
        print(name, 'ERROR', result.stderr[-800:], flush=True); continue
    with dis.open('w') as out:
        subprocess.run(['mips-linux-gnu-objdump','-dz',str(obj)], stdout=out, check=True)
    diff = subprocess.run(['python3','drafts/T-23003/adiff.py', str(dis), 'asm/nonmatchings/main/8006CB30/func_8006D7EC.s','2','3'], capture_output=True,text=True)
    print(name, diff.stdout, flush=True)

sys.exit(1 if compile_failed else 0)
