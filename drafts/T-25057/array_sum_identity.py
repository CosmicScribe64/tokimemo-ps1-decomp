"""Hide the plain memory-variable operand behind a late-folded zero identity."""
from pathlib import Path
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe
probe.ROOT = Path('build/T-25057')
name = 'func_801398EC'
base = (probe.ROOT/name/'array-store-first/body.c').read_text()
variants = {
 'tile-late-add': base.replace('tile[0] * 0x10', '(tile[0] + ((u32)i < 0)) * 0x10'),
 'row-late-add': base.replace('(i / 8) * 0x30', '((i / 8) * 0x30 + ((u32)i < 0))'),
}
for label in sys.argv[1:]:
    probe.probe(name, label, variants[label])
