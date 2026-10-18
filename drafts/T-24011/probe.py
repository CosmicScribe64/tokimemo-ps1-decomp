#!/usr/bin/env python3
"""Compile the retained DATE source drafts without editing production sources.

Run in Docker from the repository root. Outputs stay under build/T-24011.
These are diagnostic comparisons, not production C-match verification.
"""
from pathlib import Path
import argparse
import re
import subprocess
import sys
from diagnostic import report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--standalone', action='store_true',
                        help='compile one function per object instead of the full unit')
    args = parser.parse_args()
    output = Path('build/T-24011')
    output.mkdir(parents=True, exist_ok=True)
    drafts = Path(__file__).resolve().parent
    names = ('func_801449D4', 'func_80144E20')
    bodies = {name: (drafts / (name + '.c')).read_text() for name in names}
    declarations = 'extern u8 D_8015BF48;\n' + '\n'.join(
        'void ' + name + '(void);' for name in sorted(set(re.findall(
            r'func_801[0-9A-F]+', '\n'.join(bodies.values()))) - set(names)))
    prefix = '#include "common.h"\n#include "ovl/DATE.h"\n' + declarations + '\n'
    subprocess.run(['ninja', '-j1', 'build/headers.ok'], check=True)
    if args.standalone:
        units = {name: prefix + body for name, body in bodies.items()}
    else:
        source = Path('src/ovl/DATE/80141EE0.c').read_text()
        for name, body in bodies.items():
            fallback = ('INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80141EE0", '
                        + name + ');')
            if source.count(fallback) != 1:
                raise SystemExit('production fallback changed: ' + name)
            source = source.replace(fallback, body)
        source = source.replace('#include "ovl/DATE.h"',
                                '#include "ovl/DATE.h"\n' + declarations)
        units = {'dispatchers': source}
    for label, source in units.items():
        path = output / (label + '.c')
        obj = output / (label + '.o')
        path.write_text(source)
        subprocess.run([sys.executable, 'tools/cc.py', str(path), str(obj),
                        'ido', '5.3'], check=True)
        targets = (label,) if args.standalone else names
        for name in targets:
            result = subprocess.run([sys.executable, 'tools/funcdiff.py',
                                     '--built', str(obj), '--no-build', '--resolve',
                                     '--unit', 'DATE', name], capture_output=True, text=True)
            try:
                report(result, name, output / (name + (
                    '-standalone' if args.standalone else '-fulltu') + '.diff'))
            except ValueError as error:
                print(error, file=sys.stderr)
                return 1
    return 0


if __name__ == '__main__':
    sys.exit(main())
