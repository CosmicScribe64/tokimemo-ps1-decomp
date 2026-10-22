"""Keep the remainder local across a late-eliminated alternate definition."""
from pathlib import Path
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe
probe.ROOT = Path('build/T-25057')
name = 'func_801398EC'
base = (probe.ROOT/name/'record-word-read/body.c').read_text()
variants = {
 'remainder-phi': base.replace('                D_801217D0[i].unk_00 = 0;', '                if (tile < 0) { tile = 0; }\n                D_801217D0[i].unk_00 = 0;'),
 'remainder-phi-no-tail': base.replace('                if (tile < 0) {}\n', '').replace('                D_801217D0[i].unk_00 = 0;', '                if (tile < 0) { tile = 0; }\n                D_801217D0[i].unk_00 = 0;'),
}
for label in sys.argv[1:]:
    probe.probe(name, label, variants[label])
