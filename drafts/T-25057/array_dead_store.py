"""Number the frame with a dead initialization rather than a live discarded read."""
from pathlib import Path
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe
probe.ROOT = Path('build/T-25057')
name = 'func_801398EC'
base = (probe.ROOT/name/'array-conditional/body.c').read_text()
variants = {
 'array-dead-frame': base.replace('        if (tmp == 0)', '        tile[0] = D_800E6280.unk_10F8;\n        if (tmp == 0)'),
}
for label in sys.argv[1:]:
    probe.probe(name, label, variants[label])
