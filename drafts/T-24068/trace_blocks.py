"""Parent-approved private bit0/bb3,11 forbidden-mask observations; Docker only."""
from pathlib import Path
import hashlib
import json
import subprocess
import sys

root = Path('build/etc24068/block-mask')
root.mkdir(parents=True, exist_ok=True)


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


if sys.argv[1:] == ['--audit']:
    sys.path.insert(0, 'tools')
    from trailing_pad import Elf
    source = Path('build/etc24068/baseline/source.c')
    if sha(source) != '9fd845874a7b9de9ce82a9b6eb65413c719d4b2c0cbebbd695d32a1e7cabad88':
        raise SystemExit('pinned baseline input changed')
    stock, logged = Path('build/etc24068/baseline/source.o'), root / 'baseline.o'
    a, b = stock.read_bytes(), logged.read_bytes()
    if a != b:
        raise SystemExit('whole object differs; do not trust observations')
    x, y = Elf(a), Elf(b)
    sections = {}
    for name in ['.text', '.rel.text', '.rodata', '.rel.rodata']:
        l, r = x.sections[x.find(name)]['data'], y.sections[y.find(name)]['data']
        if l != r:
            raise SystemExit('section differs: ' + name)
        sections[name] = len(l)
    meta = json.loads((root / 'generator.json').read_text())
    meta.update({str(p): sha(p) for p in [source, stock, logged]})
    meta['equal_sections'] = sections
    (root / 'audit.json').write_text(json.dumps(meta, indent=2) + '\n')
    print('PASS same-input whole object and code/data/relocations identical.')
    raise SystemExit(0)
if sys.argv[1:]:
    raise SystemExit('use no arguments to generate or --audit after compilation')
source = Path('/scratch/variants/trace.c')
if sha(source) != '59767ece4ae04ffd19ed97909d3f4099ca46e08035a2fecf59c599a8c704c75c':
    raise SystemExit('pinned existing trace source changed')
text = source.read_text()
helper = r'''
/* Native logging predicate only; no emulated state is changed. */
static int t24068_scope(uint8_t *mem) {
    const char *name = "func_80140AE4";
    if (MEM_U32(0x1001c8d0) != 13) return 0;
    for (unsigned i = 0; i < 13; i++)
        if (MEM_U8(0x1001c4d0 + i) != (uint8_t)name[i]) return 0;
    return 1;
}
'''
assert text.count('#include "header.h"') == 1
text = text.replace('#include "header.h"', '#include "header.h"\n' + helper)
condition = 't24068_scope(mem) && MEM_U16(MEM_U32(a2) + 2) == 0 && (MEM_U16(a0 + 8) == 3 || MEM_U16(a0 + 8) == 11)'
before = '''if (CONDITION)
    fprintf(stderr, "T24068_BLOCK before bb=%u class=%u precolor=%u source=%08x/%08x lr=%08x/%08x\n",
        MEM_U16(a0 + 8), a3, a1, MEM_U32(a0 + (a3 << 3) + 44), MEM_U32(a0 + (a3 << 3) + 48), MEM_U32(a2 + 40), MEM_U32(a2 + 44));
'''.replace('CONDITION', condition).replace('/%08x\n"', '/%08x\\n"')
after = '''if (CONDITION)
    fprintf(stderr, "T24068_BLOCK after bb=%u class=%u precolor=%u source=%08x/%08x lr=%08x/%08x\n",
        MEM_U16(a0 + 8), a3, a1, MEM_U32(a0 + (a3 << 3) + 44), MEM_U32(a0 + (a3 << 3) + 48), MEM_U32(a2 + 40), MEM_U32(a2 + 44));
'''.replace('CONDITION', condition).replace('/%08x\n"', '/%08x\\n"')
first, last = 'L46dfb0:\n', 'MEM_U32(a2 + 44) = t6;\n// bdead 1 sp = sp + 0x8;'
assert text.count(first) == 1 and text.count(last) == 1
text = text.replace(first, first + before).replace(last, 'MEM_U32(a2 + 44) = t6;\n' + after + '// bdead 1 sp = sp + 0x8;')
out, binary = root / 'trace-blocks.c', root / 'trace-blocks'
out.write_text(text)
subprocess.run(['cc', '-std=c11', '-Os', '-fno-strict-aliasing', '-I/scratch/recomp',
    '-o', str(binary), str(out), '/scratch/recomp/build/5.3/libc_impl.o',
    '/scratch/recomp/build/5.3/version_info.o', '-lm'], check=True)
(root / 'generator.json').write_text(json.dumps({str(p): sha(p) for p in [source, out, binary]}, indent=2) + '\n')
print('Separate logging-only binary generated; audit before interpretation.')
