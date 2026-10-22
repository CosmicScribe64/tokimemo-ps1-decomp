"""Introduce the existing record induction through an erased exact-width read."""
from pathlib import Path
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe
probe.ROOT = Path('build/T-25057')
name = 'func_801398EC'
base = (probe.ROOT/name/'index-first/body.c').read_text()
variants = {
 'record-word-read': base.replace('        if (tmp == 0)', '        if ((u32)D_801217D0[i].unk_00 < 0) {}\n        if (tmp == 0)'),
 'record-byte-read': base.replace('        if (tmp == 0)', '        if (D_801217D0[i].unk_0E < 0) {}\n        if (tmp == 0)'),
}
for label in sys.argv[1:]:
    probe.probe(name, label, variants[label])
