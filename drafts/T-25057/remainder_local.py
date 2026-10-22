"""Avoid a second remainder expression range when preserving its local."""
from pathlib import Path
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe
probe.ROOT = Path('build/T-25057')
name = 'func_801398EC'
base = (probe.ROOT/name/'record-word-read/body.c').read_text()
base = base.replace('                if (tile < 0) {}\n', '')
base = base.replace('                D_801217D0[i].unk_00 = 0;', '                if ((u32)i < 0) { tile = 0; }\n                D_801217D0[i].unk_00 = 0;')
variants = {
 'local-index-arm': base,
 'local-early-ref': base.replace('        if (tmp == 0)', '        tile = (u32)i < 0;\n        if (tile < 0) {}\n        if (tmp == 0)'),
}
for label in sys.argv[1:]:
    probe.probe(name, label, variants[label])
