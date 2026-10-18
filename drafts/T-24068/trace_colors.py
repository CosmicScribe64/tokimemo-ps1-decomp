"""Generate only the parent-approved bit0/color1..3 private logging diagnostic."""
from pathlib import Path
import hashlib
import json
import subprocess
import sys

if len(sys.argv) > 1:
    if sys.argv[1:] != ['--audit']:
        raise SystemExit('use no arguments to generate or --audit after the traced compile')
    sys.path.insert(0, 'tools')
    from trailing_pad import Elf
    root = Path('build/etc24068/color-cost')
    stock = Path('build/etc24068/baseline/source.o')
    logged = root / 'baseline.o'
    input_source = Path('build/etc24068/baseline/source.c')
    if hashlib.sha256(input_source.read_bytes()).hexdigest() != '9fd845874a7b9de9ce82a9b6eb65413c719d4b2c0cbebbd695d32a1e7cabad88':
        raise SystemExit('pinned baseline input changed')
    a, b = stock.read_bytes(), logged.read_bytes()
    if a != b:
        raise SystemExit('whole object differs; do not trust trace')
    x, y = Elf(a), Elf(b)
    sections = {}
    for name in ['.text', '.rel.text', '.rodata', '.rel.rodata']:
        left, right = x.sections[x.find(name)]['data'], y.sections[y.find(name)]['data']
        if left != right:
            raise SystemExit('section differs: ' + name)
        sections[name] = len(left)
    meta = json.loads((root / 'generator.json').read_text())
    for p in [stock, logged, Path('build/etc24068/baseline/source.c')]:
        meta[str(p)] = hashlib.sha256(p.read_bytes()).hexdigest()
    meta['equal_sections'] = sections
    (root / 'audit.json').write_text(json.dumps(meta, indent=2) + '\n')
    print('PASS same-input whole object and code/data/relocations identical.')
    raise SystemExit(0)

source = Path('/scratch/variants/trace.c')
expected = '59767ece4ae04ffd19ed97909d3f4099ca46e08035a2fecf59c599a8c704c75c'
if hashlib.sha256(source.read_bytes()).hexdigest() != expected:
    raise SystemExit('pinned existing trace source changed')
text = source.read_text()
for label, phase, cost in [('L47187c', 'caller-entry', False),
                           ('L47190c', 'caller-cost', True),
                           ('L4719cc', 'saved-entry', False),
                           ('L471afc', 'saved-cost', True)]:
    needle = label + ':\n'
    if text.count(needle) != 1:
        raise SystemExit('label changed: ' + label)
    fmt = 'T24068_COLOR ' + phase + ' bit=0 color=%u mask=%08x/%08x forbidden=%u'
    values = 's2, MEM_U32(s5 + 40), MEM_U32(s5 + 44), (MEM_U32(s5 + 40) & (0x80000000u >> s2)) != 0'
    if cost:
        fmt += ' cost=%.9g best=%.9g'
        values += ', (double)f2.f[0], (double)f20.f[0]'
    logging = 'if (MEM_U16(MEM_U32(s5) + 2) == 0 && s2 >= 1 && s2 <= 3)\n    fprintf(stderr, "' + fmt + '\\n", ' + values + ');\n'
    text = text.replace(needle, needle + logging)
root = Path('build/etc24068/color-cost')
root.mkdir(parents=True, exist_ok=True)
out = root / 'trace-colors.c'
out.write_text(text)
binary = root / 'trace-colors'
subprocess.run(['cc', '-std=c11', '-Os', '-fno-strict-aliasing',
    '-I/scratch/recomp', '-o', str(binary), str(out),
    '/scratch/recomp/build/5.3/libc_impl.o',
    '/scratch/recomp/build/5.3/version_info.o', '-lm'], check=True)
meta = {str(p): hashlib.sha256(p.read_bytes()).hexdigest() for p in [source, out, binary]}
(root / 'generator.json').write_text(json.dumps(meta, indent=2) + '\n')
print('Private logging-only binary generated; whole-object fidelity required before using evidence.')
