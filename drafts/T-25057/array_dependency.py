"""Keep the memory-local remainder loop-dependent with a late-zero identity."""
from pathlib import Path
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe
probe.ROOT = Path('build/T-25057')
name = 'func_801398EC'
base = (probe.ROOT/name/'remainder-array/body.c').read_text()
variants = {
 'array-index-zero': base.replace('(D_800E6280.unk_10F8 >> 2)', '((D_800E6280.unk_10F8 + ((u32)i < 0)) >> 2)'),
}
for label in sys.argv[1:]:
    probe.probe(name, label, variants[label])
