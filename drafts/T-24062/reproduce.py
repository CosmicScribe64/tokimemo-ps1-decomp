#!/usr/bin/env python3
"""T-24062: replay archived source checkpoints in Docker; production stays untouched."""
from pathlib import Path
import re
import subprocess
import sys
sys.path.insert(0, 'tools')
import autotweak


def main():
    original = Path('src/main/80079B10.c').read_text()
    folder = Path('build/T24062/replay')
    folder.mkdir(parents=True, exist_ok=True)
    names = ['func_8007B64C', 'func_80079C70', 'func_8007BBE8']
    for name in names:
        body = Path('drafts/T-24062/' + name + '.c').read_text()
        source = original
        if name == 'func_8007B64C':
            guard = next(g for g in autotweak.guards(source) if g.func == name)
            source = source[:guard.start] + 'T24062_BODY' + source[guard.end:]
            source = source.replace('(&D_80125CC0)', 'QUEUE_BASE')
            source = re.sub(r'\bD_80125CC0\b', 'D_80125CC0[0]', source)
            source = re.sub(r'\bD_80125CC2\b', 'D_80125CC0[1]', source)
            source = source.replace('QUEUE_BASE', 'D_80125CC0').replace('T24062_BODY', body)
            source = '#define D_80125CC0 D_80125CC0_scalar_unused\n#define D_80125CC2 D_80125CC2_scalar_unused\n' + source
            source = source.replace('#include "game.h"', '#include "game.h"\n#undef D_80125CC0\n#undef D_80125CC2\nextern u16 D_80125CC0[33];', 1)
        else:
            anchor = 'INCLUDE_ASM("asm/nonmatchings/main/80079B10", ' + name + ');'
            if source.count(anchor) != 1:
                raise SystemExit('expected one fallback for ' + name)
            source = source.replace(anchor, body)
            if name == 'func_8007BBE8':
                declaration = '''
typedef struct AudioBankView50 {
    /* 0x00 */ s32 unk_00[16];
    /* 0x40 */ u8 unk_40[16];
} AudioBankView50; /* size 0x50, scratch target-evidenced view */
void func_8007BBE8();
'''
                source = source.replace('#include "game.h"', '#include "game.h"\n' + declaration, 1)
        scratch = folder / (name + '.c')
        scratch.write_text(source)
        subprocess.run(['python3', 'tools/viewsweep.py', 'main:' + name, '--src', str(scratch),
                        '--budget', '1', '-j', '1', '--json', str(folder / (name + '.json'))], check=True)


if __name__ == '__main__':
    main()
