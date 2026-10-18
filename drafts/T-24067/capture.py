"""Copy stock uopt stage files and verify unchanged output; run in Docker."""
from pathlib import Path
import shutil
import subprocess
import sys

sys.path.insert(0, 'tools')
import cc
import cvt_pass as cv
from trailing_pad import Elf

root = Path('build/etc24067/archive/ucode').resolve()
root.mkdir(parents=True, exist_ok=True)

if len(sys.argv) > 1 and sys.argv[1] == '--uopt':
    original_call = subprocess.call

    def capture(command, *args, **kwargs):
        files = []
        i = 1
        while i < len(command):
            if command[i] in cv.VALUED:
                i += 2
            elif command[i].startswith('-'):
                i += 1
            else:
                files.append(command[i])
                i += 1
        if len(files) < 2:
            raise RuntimeError('missing uopt input/output')
        shutil.copyfile(files[0], root / 'pre.B')
        result = original_call(command, *args, **kwargs)
        if result == 0:
            shutil.copyfile(files[1], root / 'raw.O')
        return result

    subprocess.call = capture
    cv.main(sys.argv)

production = Path('src/ovl/ETC/80140F50.c')
original = production.read_bytes()
subprocess.run([sys.executable, 'drafts/T-24067/replay.py', '--variant', 'indexed'], check=True)
base = Path('build/etc24067/archive/indexed')
original_environment = cc.ido_frame_env


def environment(version, tmp, extra_shims=None):
    return original_environment(version, tmp, dict(extra_shims or {},
        uopt=str(Path(__file__).resolve())))


cc.ido_frame_env = environment
out = root / 'stock.o'
cc.main(['cc.py', str(base / 'source.c'), str(out), 'ido', '5.3'])
a, b = [Elf(p.read_bytes()) for p in [base / 'source.o', out]]
for name in ['.text', '.rel.text', '.rodata', '.rel.rodata']:
    left = a.sections[a.find(name)]['data']
    right = b.sections[b.find(name)]['data']
    if left != right:
        raise SystemExit('capture changed ' + name)
    print(name, 'identical', len(left))
if production.read_bytes() != original:
    raise SystemExit('production changed')
print('Copied ucode only; no compiler bytes or options changed. Production unchanged.')
