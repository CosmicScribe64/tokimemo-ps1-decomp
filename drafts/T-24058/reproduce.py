#!/usr/bin/env python3
"""T-24058: reproduce three concrete SHOUGATU negatives in Docker, scratch only."""
from pathlib import Path
import subprocess
import sys
sys.path.insert(0, 'tools')
import autotweak


def main():
    source = Path('src/ovl/SHOUGATU/80134120.c').read_text()
    guard = next(g for g in autotweak.guards(source) if g.func == 'func_801347E0')
    body = Path('drafts/T-24058/shougatu.c').read_text()
    parameter = body.replace('func_801347E0(void)', 'func_801347E0(s32 girl)').replace('    s32 girl;\n', '')
    global_girl = body.replace('    s32 girl;\n', '').replace(
        '    girl = D_800E6280.unk_75D;\n    D_800E6280.unk_F5F = girl;\n    func_800847B8(girl);',
        '    D_800E6280.unk_F5F = D_800E6280.unk_75D;\n    func_800847B8(D_800E6280.unk_F5F);')
    global_girl = global_girl.replace('    girl = D_800E6280.unk_F5F;\n', '').replace('(girl == 6)', '(D_800E6280.unk_F5F == 6)')
    folder = Path('build/T24058/replay')
    folder.mkdir(parents=True, exist_ok=True)
    for label, variant in [('baseline', body), ('callback-parameter', parameter), ('global-girl', global_girl)]:
        scratch = folder / (label + '.c')
        scratch.write_text(source[:guard.start] + variant + source[guard.end:])
        subprocess.run(['python3', 'tools/viewsweep.py', 'SHOUGATU:func_801347E0', '--src', str(scratch),
                        '--budget', '1', '-j', '1', '--json', str(folder / (label + '.json'))], check=True)


if __name__ == '__main__':
    main()
