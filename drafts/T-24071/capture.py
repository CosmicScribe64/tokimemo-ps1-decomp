"""Copy stock uopt stage files and verify unchanged output; run in Docker."""
from pathlib import Path
import shutil
import subprocess
import sys

sys.path.insert(0, 'tools')
import cc
import cvt_pass as cv
from trailing_pad import Elf

root = Path('build/etc24071/capture57').resolve()
root.mkdir(parents=True, exist_ok=True)

if len(sys.argv) > 1 and sys.argv[1] == '--uopt':
    original_call = subprocess.call
    captured_output = []

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
        captured_output.append(files[1])
        shutil.copyfile(files[0], root / 'pre.B')
        result = original_call(command, *args, **kwargs)
        if result == 0:
            shutil.copyfile(files[1], root / 'raw.O')
        return result

    subprocess.call = capture
    result = cv.run_uopt(sys.argv[2], sys.argv[3:])
    if result == 0:
        shutil.copyfile(captured_output[-1], root / 'post.O')
    sys.exit(result)

production = Path('src/ovl/ETC/80143240.c')
original = production.read_bytes()
assert Path('build/etc24071/replay/buttons/source.o').exists()
import hashlib
assert hashlib.sha256(Path('build/etc24071/replay/buttons/source.c').read_bytes()).hexdigest() == '18513169b0051a22a8833ec96928c814cbf9f3e1c53135757be76bb0318a99e0'
base = Path('build/etc24071/replay/buttons')
original_environment = cc.ido_frame_env


def environment(version, tmp, extra_shims=None):
    return original_environment(version, tmp, dict(extra_shims or {},
        uopt=str(Path(__file__).resolve())))


cc.ido_frame_env = environment
out = root / 'stock.o'
cc.main(['cc.py', str(base / 'source.c'), str(out), 'ido', '5.3'])
assert (base / 'source.o').read_bytes() == out.read_bytes(), 'whole-object mismatch'
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
