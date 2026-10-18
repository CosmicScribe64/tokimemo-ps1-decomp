#!/usr/bin/env python3
"""Reproduce the small DATE constant-local draft's pre-as1 stage observation.

Run in Docker from the repository root. This emits a listing, an object and
a diagnostic diff under ignored build/T-24011, without production edits.
"""
from pathlib import Path
import re
import subprocess
import sys
import tempfile

sys.path.insert(0, str(Path('tools').resolve()))
from cc import IDO_CFLAGS, ido_frame_env


def main():
    root = Path('build/T-24011').resolve()
    root.mkdir(parents=True, exist_ok=True)
    body = Path(__file__).with_name('small-constant-local.c').read_text()
    name = 'func_801449D4'
    decls = '\n'.join('void ' + symbol + '(void);' for symbol in sorted(
        set(re.findall(r'func_801[0-9A-F]+', body)) - {name}))
    source = root / 'small-constant-local.c'
    source.write_text('#include "common.h"\n#include "ovl/DATE.h"\n'
                      + decls + '\n' + body)
    with tempfile.TemporaryDirectory(dir=root) as temporary:
        flags = [('-I' + str(Path('include').resolve())) if flag == '-Iinclude'
                 else flag for flag in IDO_CFLAGS if flag != '-c'] + ['-S']
        subprocess.run(['/opt/ido/5.3/cc'] + flags + ['-o',
                       str(root / 'small-constant-local.s'), str(source)],
                       env=ido_frame_env('5.3', temporary), cwd=root, check=True)
    obj = root / 'small-constant-local.o'
    subprocess.run([sys.executable, 'tools/cc.py', str(source), str(obj),
                    'ido', '5.3'], check=True)
    result = subprocess.run([sys.executable, 'tools/funcdiff.py', '--built',
                             str(obj), '--no-build', '--resolve', '--unit',
                             'DATE', name], capture_output=True, text=True)
    text = result.stdout + result.stderr
    (root / 'small-constant-local.diff').write_text(text)
    print('diagnostic resolved +/- rows = ' + str(sum(
        line.lstrip().startswith(('-', '+')) for line in text.splitlines())))
    return 1 if 'ERROR' in text or 'missing in ' in text else 0


if __name__ == '__main__':
    sys.exit(main())
