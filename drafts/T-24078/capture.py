"""Capture stock IDO5.3 backend stages; Docker only, production unchanged."""
from pathlib import Path
import hashlib
import json
import subprocess
import sys

ROOT = Path('/work/build/etc24078/backend604')
ROOT.mkdir(parents=True, exist_ok=True)


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def input_path(args):
    inputs, i = [], 0
    while i < len(args):
        if args[i] in ('-G', '-o', '-t', '-temp'):
            i += 2
        elif args[i].startswith('-'):
            i += 1
        else:
            inputs.append(args[i])
            i += 1
    if len(inputs) != 1:
        raise RuntimeError('unexpected pass input list: ' + repr(inputs))
    return Path(inputs[0])


def wrapper():
    phase, target, args = sys.argv[1], sys.argv[2], sys.argv[3:]
    prefix = ROOT / phase[2:]
    sidecar = Path(args[args.index('-t') + 1])
    prefix.with_suffix('.input').write_bytes(input_path(args).read_bytes())
    prefix.with_suffix('.T').write_bytes(sidecar.read_bytes())
    prefix.with_suffix('.args.json').write_text(json.dumps(args) + '\n')
    if phase == '--ugen':
        status = subprocess.call([target] + args)
    else:
        sys.path.insert(0, '/work/tools')
        import frame_pass
        status = frame_pass.run_as1(target, args)
    if status:
        raise SystemExit(status)
    output = Path(args[args.index('-o') + 1])
    prefix.with_suffix('.output').write_bytes(output.read_bytes())
    prefix.with_suffix('.after.T').write_bytes(sidecar.read_bytes())


def capture():
    sys.path.insert(0, 'tools')
    import cc
    source = Path('build/etc24078/replay/word-coordinates/source.c')
    assert sha(source) == '609467db3b985d285c786a9db47057fbb59b4276268d12d39ed6aab98d30713d'
    original = cc.ido_frame_env
    script = str(Path(__file__).resolve())

    def env(version, temp, extra_shims=None):
        return original(version, temp, {'ugen': script})

    cc.ido_frame_env = env
    try:
        cc.main(['cc.py', str(source), str(ROOT / 'captured.o'), 'ido', '5.3'])
    finally:
        cc.ido_frame_env = original
    baseline = Path('build/etc24078/replay/word-coordinates/source.o')
    if baseline.read_bytes() != (ROOT / 'captured.o').read_bytes():
        raise SystemExit('capture changed full object')
    sys.path.insert(0, 'tools')
    from trailing_pad import Elf
    import frame_pass as fp
    left, right = Elf(baseline.read_bytes()), Elf((ROOT / 'captured.o').read_bytes())
    for name in ['.text', '.rel.text', '.rodata', '.rel.rodata']:
        a = left.sections[left.find(name)]['data']
        b = right.sections[right.find(name)]['data']
        assert a == b, name
        print(name, 'identical', len(a))
    (ROOT / 'capture-fidelity.json').write_text(json.dumps({
        'input': sha(source), 'stock_object': sha(baseline),
        'captured_object': sha(ROOT / 'captured.o'),
        'ugen_input': sha(ROOT / 'ugen.input'), 'ugen_symbols': sha(ROOT / 'ugen.T'),
        'ugen_output': sha(ROOT / 'ugen.output'),
        'ugen_binary': sha('/opt/ido/5.3/ugen'), 'ido_flags': cc.IDO_CFLAGS}, indent=2) + '\n')
    print('PASS: stock capture full object identical; phase interpretation now permitted.')


if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] in ["--ugen", "--as1"]:
        wrapper()
    else:
        capture()
