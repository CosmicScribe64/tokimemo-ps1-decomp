"""Place the record store before the memory-local division to fill its latency."""
from pathlib import Path
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe
probe.ROOT = Path('build/T-25057')
name = 'func_801398EC'
base = (probe.ROOT/name/'array-dead-frame/body.c').read_text()
old = '                tile[0] = (D_800E6280.unk_10F8 >> 2) % 3;\n                D_801217D0[i].unk_00 = 0;'
base = base.replace(old, '                D_801217D0[i].unk_00 = 0;\n                tile[0] = (D_800E6280.unk_10F8 >> 2) % 3;')
variants = {
 'array-store-first': base,
 'array-sum-flipped': base.replace('(i / 8) * 0x30 + tile[0] * 0x10', 'tile[0] * 0x10 + (i / 8) * 0x30'),
}
for label in sys.argv[1:]:
    probe.probe(name, label, variants[label])
