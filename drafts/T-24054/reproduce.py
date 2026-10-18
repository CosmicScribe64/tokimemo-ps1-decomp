#!/usr/bin/env python3
"""T-24054: reproduce the closest KANGEI source draft; run in Docker, no production writes."""
from pathlib import Path
import subprocess


def main():
    name = 'func_8013852C'
    source = Path('src/ovl/KANGEI/80137B90.c').read_text()
    anchor = 'INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", %s);' % name
    if source.count(anchor) != 1:
        raise SystemExit('expected one assembly fallback; production may have advanced')
    folder = Path('build/T24054/replay')
    folder.mkdir(parents=True, exist_ok=True)
    scratch = folder / 'source.c'
    scratch.write_text(source.replace(anchor, Path('drafts/T-24054/body.c').read_text()))
    subprocess.run(['python3', 'tools/viewsweep.py', 'KANGEI:' + name,
                    '--src', str(scratch), '--budget', '1', '-j', '1',
                    '--json', str(folder / 'result.json')], check=True)


if __name__ == '__main__':
    main()
