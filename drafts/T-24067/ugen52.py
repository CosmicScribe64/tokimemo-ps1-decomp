"""Same-input IDO5.2 backend diagnostic; Docker only, no production changes."""
from pathlib import Path
import hashlib
import json
import shutil
import subprocess
import sys

ROOT = Path('/work/build/etc24067/ugen52')
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
    source = Path('build/etc24067/archive/continue/source.c')
    original = cc.ido_frame_env
    script = str(Path(__file__).resolve())

    def env(version, temp, extra_shims=None):
        return original(version, temp, {'ugen': script, 'as1': script})

    cc.ido_frame_env = env
    try:
        cc.main(['cc.py', str(source), str(ROOT / 'captured.o'), 'ido', '5.3'])
    finally:
        cc.ido_frame_env = original
    baseline = Path('build/etc24067/ugen-state/stock.o')
    if baseline.read_bytes() != (ROOT / 'captured.o').read_bytes():
        raise SystemExit('capture changed full object')
    (ROOT / 'capture-fidelity.json').write_text(json.dumps({
        'input': sha(source), 'stock_object': sha(baseline),
        'captured_object': sha(ROOT / 'captured.o'),
        'ugen_input': sha(ROOT / 'ugen.input'), 'ugen_symbols': sha(ROOT / 'ugen.T'),
        'ugen_output': sha(ROOT / 'ugen.output')}, indent=2) + '\n')
    print('PASS: stock capture full object identical.')


def stage(version):
    fidelity = json.loads((ROOT / 'capture-fidelity.json').read_text())
    if sha(ROOT / 'ugen.input') != fidelity['ugen_input'] or sha(ROOT / 'ugen.T') != fidelity['ugen_symbols']:
        raise SystemExit('captured stage input changed')
    args = json.loads((ROOT / 'ugen.args.json').read_text())
    directory = ROOT / ('stage-' + version)
    directory.mkdir(exist_ok=True)
    source, table, output = directory / 'input.O', directory / 'input.T', directory / 'output.G'
    shutil.copyfile(ROOT / 'ugen.input', source)
    shutil.copyfile(ROOT / 'ugen.T', table)
    args[args.index('-o') + 1] = str(output)
    args[args.index('-t') + 1] = str(table)
    args[args.index('-temp') + 1] = str(directory / 'scratch')
    original_input = str(input_path(args))
    args[args.index(original_input)] = str(source)
    if version == '53':
        command = ['/opt/ido/5.3/ugen']
    else:
        if (ROOT / 'stage-53/object.o').read_bytes() != (ROOT / 'as1.output').read_bytes():
            raise SystemExit('stock stage fidelity must pass before5.2')
        if sha('/scratch/ido5.2/usr/lib/ugen') != '43db097dc6b462d7741b511c612f8d3de02aa2f5b713cab13ac0ce14e193e006':
            raise SystemExit('existing5.2 backend changed')
        if sha('/scratch/ido5.2/usr/bin/qemu-irix') != '9dc3d30412961dedcb60d45201061458b8051af0a1930e3d3732940c0c372675':
            raise SystemExit('existing5.2 runtime changed')
        command = ['/scratch/ido5.2/usr/bin/qemu-irix', '-silent', '-L',
                   '/scratch/ido5.2', '/scratch/ido5.2/usr/lib/ugen']
    subprocess.run(command + args, check=True)
    (directory / 'command.json').write_text(json.dumps(command + args) + '\n')
    print('Backend stage completed:', version)


def assemble(versions=('53', '52')):
    sys.path.insert(0, 'tools')
    import frame_pass
    import funcdiff as fd
    result = {}
    for version in versions:
        directory = ROOT / ('stage-' + version)
        table = directory / 'as1.T'
        shutil.copyfile(directory / 'input.T', table)
        args = json.loads((ROOT / 'as1.args.json').read_text())
        args[args.index('-o') + 1] = str(directory / 'object.o')
        args[args.index('-t') + 1] = str(table)
        source = str(input_path(args))
        args[args.index(source)] = str(directory / 'output.G')
        status = frame_pass.run_as1('/opt/ido/5.3/as1', args)
        if status:
            raise SystemExit(status)
        obj = directory / 'object.o'
        if version == '53':
            if obj.read_bytes() != (ROOT / 'as1.output').read_bytes():
                raise SystemExit('same-stage53 object differs from captured stock as1 output')
        name = 'func_80141ABC'
        candidate = fd.locate(name, 'ETC')
        expected, error = fd.expected_object(candidate, False)
        if error:
            raise RuntimeError(error)
        want = fd.resolved_functions(expected, [name], candidate)[name]
        got = fd.resolved_functions(obj, [name])[name]
        (directory / 'target.diff').write_text('\n'.join(fd.diff_lines(want, got)) + '\n')
        result[version] = {'instructions': len(got), 'cost': fd.diff_score(want, got),
            'rows': len(fd.diff_lines(want, got)), 'object_sha': sha(obj),
            'binasm_sha': sha(directory / 'output.G'), 'symbols_sha': sha(directory / 'input.T')}
    result['matching_claim'] = False
    (ROOT / 'result.json').write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps(result, indent=2))


if __name__ == '__main__':
    if sys.argv[1] in ['--ugen', '--as1']:
        wrapper()
    elif sys.argv[1:] == ['capture']:
        capture()
    elif len(sys.argv) == 3 and sys.argv[1] == 'stage' and sys.argv[2] in ['53', '52']:
        stage(sys.argv[2])
    elif sys.argv[1:] == ['assemble']:
        assemble()
    elif sys.argv[1:] == ['assemble-stock']:
        assemble(('53',))
    else:
        raise SystemExit('use capture, stage 53|52, assemble-stock, or assemble')
