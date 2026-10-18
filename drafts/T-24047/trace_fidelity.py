#!/usr/bin/env python3
"""Verify the external diagnostic uopt against stock output; run in Docker."""
import argparse
import json
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools'))
import funcdiff
import trailing_pad


def sections(path):
    names = {'.text', '.rel.text', '.rodata', '.rel.rodata', '.data', '.rel.data'}
    return {s['sname']: bytes(s['data'])
            for s in trailing_pad.Elf(path.read_bytes()).sections
            if s['sname'] in names}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path,
                        default=Path('build/t24047-replay/candidate.c'))
    parser.add_argument('--reference', type=Path,
                        default=Path('build/t24047-replay/guarded.o'))
    parser.add_argument('--scratch', type=Path, default=Path('/scratch'))
    parser.add_argument('--out', type=Path,
                        default=Path('build/t24047/tracer-fidelity'))
    args = parser.parse_args()
    args.out.mkdir(parents=True, exist_ok=True)
    outputs = {}
    variants = [('stock', Path('/opt/ido/5.3/uopt'), False),
                ('patched', args.scratch / 'recomp/build/5.3/out/uopt', False),
                ('traced', args.scratch / 'variants/trace', True)]
    for label, binary, trace in variants:
        out = args.out / (label + '.o')
        command = [sys.executable, str(ROOT / 'tools/uopt_eval.py'), 'compile',
                   str(args.source), str(out), '--uopt', str(binary)]
        if trace:
            command.append('--trace')
        with (args.out / (label + '.log')).open('w') as log:
            subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, check=True)
        outputs[label] = out
    stock = sections(outputs['stock'])
    for label in ('patched', 'traced'):
        if sections(outputs[label]) != stock:
            raise ValueError(label + ' code/data/relocations differ from stock')
    name = 'func_80135F4C'
    reference = funcdiff.resolved_functions(args.reference, [name])[name]
    actual = funcdiff.resolved_functions(outputs['traced'], [name])[name]
    if reference is None or actual is None or funcdiff.keys(reference) != funcdiff.keys(actual):
        raise ValueError('traced target differs from the preferred full-TU reference')
    result = dict(sections=sorted(stock), all_equal=True,
                  target_instructions=len(actual), matching_claim=False)
    (args.out / 'result.json').write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps(result))


if __name__ == '__main__':
    try:
        main()
    except (OSError, ValueError, subprocess.CalledProcessError) as error:
        print('trace_fidelity.py: ' + str(error), file=sys.stderr)
        raise SystemExit(2)
