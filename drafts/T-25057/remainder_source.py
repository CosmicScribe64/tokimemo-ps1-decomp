"""Test remainder type and lazy placement from the current exact-shape source."""
from pathlib import Path
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe
probe.ROOT = Path('build/T-25057')
name = 'func_801398EC'
base = (probe.ROOT/name/'record-word-read/body.c').read_text()
calc = '                tile = (D_800E6280.unk_10F8 >> 2) % 3;\n'
variants = {
 'signed-remainder': base.replace('    u32 tile;', '    s32 tile;').replace('if (tile < 0)', 'if ((u32)tile < 0)'),
 'precompute-remainder': base.replace(calc, '').replace('        if (tmp == 0)', '        tile = (D_800E6280.unk_10F8 >> 2) % 3;\n        if (tmp == 0)'),
 'remainder-byte': base.replace('    u32 tile;', '    u8 tile;'),
}
for label in sys.argv[1:]:
    probe.probe(name, label, variants[label])
