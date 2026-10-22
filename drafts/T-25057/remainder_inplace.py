"""Test the natural reused frame/remainder local instead of one expression."""
from pathlib import Path
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe
probe.ROOT = Path('build/T-25057')
name = 'func_801398EC'
base = (probe.ROOT/name/'precompute-remainder/body.c').read_text()
calc = '        tile = (D_800E6280.unk_10F8 >> 2) % 3;'
variants = {
 'inplace-remainder': base.replace(calc, '        tile = D_800E6280.unk_10F8;\n        tile >>= 2;\n        tile %= 3;'),
 'inplace-local': base.replace(calc, '        tile = D_800E6280.unk_10F8;\n        if (tile < 0) { tile = 0; }\n        tile >>= 2;\n        tile %= 3;'),
}
for label in sys.argv[1:]:
    probe.probe(name, label, variants[label])
