#!/usr/bin/env python3
"""Generate or verify the external print-only AS1 diagnostic; run in Docker."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools'))
import funcdiff
import trailing_pad

SOURCE_SHA256 = '905927751a79aba82d332bdce254b2b30a4312ae14a4108897e11b48487d17a1'


def replace_once(text, old, new):
    if text.count(old) != 1:
        raise ValueError('unexpected diagnostic anchor count: ' + repr(old))
    return text.replace(old, new)


def edit_function(text, name, edit):
    anchor = 'static uint32_t ' + name + '('
    start = text.index(anchor, text.index(anchor) + 1)
    end = text.index('\n}\n', start) + 3
    return text[:start] + edit(text[start:end]) + text[end:]


def instrument(source):
    if hashlib.sha256(source.encode()).hexdigest() != SOURCE_SHA256:
        raise ValueError('AS1 reference differs from the pinned reconstruction')
    old = 't3 = MEM_U32(s3 + 68);\n//nop;\nt6 = t3 << 26;'
    new = ('t3 = MEM_U32(s3 + 68);\n'
           'fprintf(stderr, "XBBSCAN dst=%u src=%u flags=%08x weight=%u\\n", '
           'MEM_U32(s4+64), MEM_U32(s3+64), t3, MEM_U16(s3+72));\n'
           '//nop;\nt6 = t3 << 26;')
    source = replace_once(source, old, new)

    def scanner(body):
        body = replace_once(body, 'L42a44c:\n', 'L42a44c:\n'
                            'fprintf(stderr, "XBBRESULT dst=%u src=%u candidate=%u\\n", '
                            'MEM_U32(s6+64), MEM_U32(s1+64), v0);\n')
        stages = (
            ('L42a15c', 'slot=%u op=%08x sched=%u/%u limit=%u',
             's2, MEM_U32(sp+120), MEM_U8(s0+33), MEM_U8(s0+34), MEM_U32(sp+200)'),
            ('L42a1b4', 'slot=%u movable=%u', 's2, v0'),
            ('L42a314', 'slot=%u graph_reject=%u', 's2, v0'),
            ('L42a364', 'slot=%u path_accept=%u', 's2, v0'),
        )
        for label, message, values in stages:
            body = replace_once(body, label + ':\n', label + ':\n'
                                'fprintf(stderr, "XBBDETAIL dst=%u src=%u ' + message +
                                '\\n", MEM_U32(s6+64), MEM_U32(s1+64), ' + values + ');\n')
        return body

    source = edit_function(source, 'func_42a028', scanner)
    return edit_function(source, 'func_429928', lambda body: replace_once(
        body, 'L429a48:\n', 'L429a48:\n'
        'fprintf(stderr, "XBBPATHBARRIER dst=%u src=%u block=%u flags=%08x\\n", '
        'MEM_U32(MEM_U32(sp+156)+64), MEM_U32(MEM_U32(sp+152)+64), '
        'MEM_U32(a1+64), v0);\n'))


def sections(path):
    names = {'.text', '.rel.text', '.rodata', '.rel.rodata', '.data', '.rel.data'}
    return {s['sname']: bytes(s['data'])
            for s in trailing_pad.Elf(path.read_bytes()).sections if s['sname'] in names}


def verify(args):
    args.out.mkdir(parents=True, exist_ok=True)
    results = []
    for label in args.labels:
        prefix = args.captures / label
        output = args.out / (label + '.o')
        command = [str(args.binary), '-elf', '-G', '0', '-p0', '-EL', '-g0', '-O2',
                   '-R', '-xbbdbg', '8', str(prefix.with_suffix('.G')), '-o', str(output),
                   '-t', str(prefix.with_suffix('.T'))]
        with (args.out / (label + '.log')).open('w') as log:
            subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, check=True)
        stock = prefix.with_suffix('.o')
        reference = sections(stock)
        if not reference or sections(output) != reference:
            raise ValueError(label + ' code/data/relocation sections differ from stock')
        want = funcdiff.resolved_functions(stock, [args.function])[args.function]
        got = funcdiff.resolved_functions(output, [args.function])[args.function]
        if want is None or got is None or funcdiff.keys(want) != funcdiff.keys(got):
            raise ValueError(label + ' target words differ from stock')
        results.append(dict(label=label, sections=sorted(reference),
                            target_instructions=len(got), all_equal=True))
    result = dict(results=results, matching_claim=False)
    (args.out / 'result.json').write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps(result))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest='command', required=True)
    generate = commands.add_parser('instrument')
    generate.add_argument('source', type=Path)
    generate.add_argument('output', type=Path)
    check = commands.add_parser('verify')
    check.add_argument('--binary', type=Path, required=True)
    check.add_argument('--captures', type=Path, default=Path('build/t24047/switch-metadata'))
    check.add_argument('--labels', nargs='+', default=['default', 'case12'])
    check.add_argument('--function', default='func_80135F4C')
    check.add_argument('--out', type=Path, default=Path('build/t24047/as1-tracer-fidelity'))
    args = parser.parse_args()
    if args.command == 'instrument':
        args.output.write_text(instrument(args.source.read_text()))
        print('AS1 print-only diagnostic source generated')
    else:
        verify(args)


if __name__ == '__main__':
    try:
        main()
    except (OSError, ValueError, subprocess.CalledProcessError) as error:
        print('as1_trace.py: ' + str(error), file=sys.stderr)
        raise SystemExit(2)
