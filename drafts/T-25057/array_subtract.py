"""Test equivalent subtraction to bypass commutative sum normalization."""
from pathlib import Path
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe
probe.ROOT = Path('build/T-25057')
name = 'func_801398EC'
base = (probe.ROOT/name/'array-store-first/body.c').read_text()
expr = '(i / 8) * 0x30 + tile[0] * 0x10 + 0x80'
variants = {
 'sum-negative-product': base.replace(expr, '(i / 8) * 0x30 - tile[0] * -0x10 + 0x80'),
 'sum-negative-expression': base.replace(expr, '(i / 8) * 0x30 - (0U - tile[0] * 0x10) + 0x80'),
}
for label in sys.argv[1:]:
    probe.probe(name, label, variants[label])
