"""T-39301: check the engine modules' C_SIGNATURES against the C they port.

usage: python3 -I tools/port/check_engine_signatures.py --repo <repo root>

For every engine/<c_file>.gd (modules, not _helpers) it reads C_FILE and C_SIGNATURES and finds each function's definition
in that C file (ANSI or K&R), then checks the argument count and the return type (void/non-void and the width: u8 s8 u16
s16 s32 u32, pointers as ptr). A function the C declares non-void cannot be marked void to skip v0: a function that
falls off the end of an s32 function says so with "ret_undefined": "<reason>". libc (_libc.gd) is checked against the
prototypes in include/ when one exists. Exit code 1 on any mismatch.

Signature override (T-39312): a function whose callers pass arguments that the C definition does not take (an unprototyped
callee that ignores them, where adding the parameter changes the matching build) lists them with "args_extra": N and a
mandatory "args_extra_reason": "<why>"; the expected count is then the C count plus N.
"""
import argparse
import glob
import os
import re
import sys

TYPES = {'void': 'void', 'u8': 'u8', 's8': 's8', 'u16': 'u16', 's16': 's16', 'u32': 'u32', 's32': 's32', 'int': 's32',
         'unsigned': 'u32', 'char': 's8', 'short': 's16', 'long': 's32', 'uint': 'u32'}


def c_return(tokens):
    text = ' '.join(tokens)
    if '*' in text:
        return 'ptr'
    words = [w for w in re.split(r'\s+', text.replace('*', ' ')) if w and w not in ('static', 'extern', 'const', 'volatile', 'struct')]
    return TYPES.get(words[-1] if words else 's32', 's32?')


def find_definition(source, name):
    # ANSI: type name(args) {   K&R: type name(a, b)\n type a;\n {
    pat = re.compile(r'^([A-Za-z_][\w \t\*]*?)[ \t\*]\**%s\s*\(([^;{]*?)\)\s*(\n[^{;]*;\s*)*\{' % re.escape(name), re.M | re.S)
    for m in pat.finditer(source):
        head = m.group(1).strip()
        if head.split()[-1] in ('return', 'else', 'if', 'while', 'switch'):
            continue
        params = m.group(2).strip()
        count = 0 if params in ('', 'void') else len([p for p in params.split(',') if p.strip()])
        text = source[m.start():m.start() + len(m.group(0))]
        before = text[:text.index(name)]
        return c_return(before.split()) if '*' not in before else 'ptr', count
    return None


def parse_module(path):
    text = open(path).read()
    c_file = re.search(r'const C_FILE: String = "([^"]+)"', text)
    block = re.search(r'const C_SIGNATURES: Dictionary = \{(.*?)\n\}', text, re.S)
    sigs = {}
    if block:
        for m in re.finditer(r'"(\w+)": \{([^}]*)\}', block.group(1)):
            body = m.group(2)
            args = re.search(r'"args": \[([^\]]*)\]', body)
            ret = re.search(r'"ret": "(\w+)"', body)
            count = len([a for a in args.group(1).split(',') if a.strip()]) if args else 0
            extra = re.search(r'"args_extra": (\d+)', body)
            reason = re.search(r'"args_extra_reason": "([^"]+)"', body)
            sigs[m.group(1)] = (count, ret.group(1) if ret else '?', '"ret_undefined"' in body, int(extra.group(1)) if extra else 0, bool(reason))
    return (c_file.group(1) if c_file else ''), sigs


def main():
    p = argparse.ArgumentParser()
    p.add_argument('--repo', required=True)
    args = p.parse_args()
    errors = 0
    checked = 0
    for path in sorted(glob.glob(os.path.join(args.repo, 'port/godot/engine/*.gd'))):
        base = os.path.basename(path)
        if base == 'engine.gd':
            continue
        c_file, sigs = parse_module(path)
        if c_file.startswith('src/'):
            source = open(os.path.join(args.repo, c_file)).read()
        else:
            source = '\n'.join(open(f, errors='ignore').read() for f in glob.glob(os.path.join(args.repo, 'include', '**', '*.h'), recursive=True))
        for name, (count, ret, undefined, extra, reason) in sorted(sigs.items()):
            found = find_definition(source, name)
            if found is None:
                if c_file.startswith('src/'):
                    print('%s: %s not found in %s' % (base, name, c_file))
                    errors += 1
                continue
            checked += 1
            c_ret, c_count = found
            if extra and not reason:
                print('%s: %s lists args_extra without args_extra_reason' % (base, name))
                errors += 1
            if c_count + extra != count:
                print('%s: %s takes %d argument(s) in the C, C_SIGNATURES lists %d' % (base, name, c_count, count))
                errors += 1
            if c_ret == 'void' and ret != 'void':
                print('%s: %s is void in the C, C_SIGNATURES says %s' % (base, name, ret))
                errors += 1
            elif c_ret != 'void' and ret == 'void':
                print('%s: %s returns %s in the C; C_SIGNATURES says void (use the type and "ret_undefined" with a reason if it falls off the end)' % (base, name, c_ret))
                errors += 1
            elif c_ret != 'void' and c_ret != ret and not (c_ret == 'ptr' and ret in ('ptr', 'u32')):
                print('%s: %s returns %s in the C, C_SIGNATURES says %s' % (base, name, c_ret, ret))
                errors += 1
    print('%d function signature(s) checked, %d error(s)' % (checked, errors))
    sys.exit(1 if errors else 0)


main()
