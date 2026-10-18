"""Parent-approved private bb3 mask-writer observations; run in Docker only."""
from pathlib import Path
import hashlib
import json
import subprocess
import sys

root = Path('build/etc24068/mask-writer')
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
/* Native logging only: no emulated state or allocator choice is changed. */
static void t24068_writer(uint8_t *mem, uint32_t bb, uint32_t stmt,
        uint32_t pc, const char *phase) {
    const char *name = "func_80140AE4";
    if (MEM_U32(0x1001c8d0) != 13 || MEM_U16(bb + 8) != 3) return;
    for (unsigned i = 0; i < 13; i++)
        if (MEM_U8(0x1001c4d0 + i) != (uint8_t)name[i]) return;
    fprintf(stderr, "T24068_WRITER %s pc=%08x bb=3 stmt=%08x op=%u dtype=%u field20=%08x field24=%08x mask=%08x/%08x\n",
        phase, pc, stmt, stmt ? MEM_U8(stmt) : 0,
        stmt ? MEM_U8(stmt + 1) : 0,
        stmt ? MEM_U32(stmt + 20) : 0, stmt ? MEM_U32(stmt + 24) : 0,
        MEM_U32(bb + 52), MEM_U32(bb + 56));
}
'''
assert text.count('#include "header.h"') == 1
text = text.replace('#include "header.h"', '#include "header.h"\n' + helper)


def insert(old, new):
    global text
    assert text.count(old) == 1, old
    text = text.replace(old, new)


insert('L468bd4:\n', 'L468bd4:\nt24068_writer(mem, MEM_U32(sp + 308), 0, 0x468bd4, "before");\n')
insert('MEM_U32(t5 + 56) = t6;\nv0 = MEM_U32(sp + 308);',
       'MEM_U32(t5 + 56) = t6;\nt24068_writer(mem, MEM_U32(sp + 308), 0, 0x468bd4, "after");\nv0 = MEM_U32(sp + 308);')
insert('L469748:\n', 'L469748:\nt24068_writer(mem, MEM_U32(sp + 308), fp, 0x469748, "before");\n')
insert('MEM_U32(s0 + 44) = t4;\ngoto L46a880;',
       'MEM_U32(s0 + 44) = t4;\nt24068_writer(mem, MEM_U32(sp + 308), fp, 0x469748, "after");\ngoto L46a880;')
insert('L46a230:\n', 'L46a230:\nt24068_writer(mem, MEM_U32(sp + 308), fp, 0x46a230, "before");\n')
insert('MEM_U32(s0 + 48) = t4;\n// bdead c19c0023',
       'MEM_U32(s0 + 48) = t4;\nt24068_writer(mem, MEM_U32(sp + 308), fp, 0x46a230, "after");\n// bdead c19c0023')
out, binary = root / 'trace-writers.c', root / 'trace-writers'
out.write_text(text)
subprocess.run(['cc', '-std=c11', '-Os', '-fno-strict-aliasing', '-I/scratch/recomp',
    '-o', str(binary), str(out), '/scratch/recomp/build/5.3/libc_impl.o',
    '/scratch/recomp/build/5.3/version_info.o', '-lm'], check=True)
(root / 'generator.json').write_text(json.dumps({str(p): sha(p) for p in [source, out, binary]}, indent=2) + '\n')
print('Separate logging-only binary generated; audit before interpretation.')
