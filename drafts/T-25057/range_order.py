"""Introduce erased expression references in allocator traversal order."""
from pathlib import Path
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe
probe.ROOT = Path('build/T-25057')
name = 'func_801398EC'
base = (probe.ROOT/name/'index-first/body.c').read_text()
variants = {
 'record-early': base.replace('        if (tmp == 0)', '        if ((u32)&D_801217D0[i] < 0) {}\n        if (tmp == 0)'),
 'tile-initialized': base.replace('    tmp =', '    tile = 0;\n    tmp ='),
}
for label in sys.argv[1:]:
    probe.probe(name, label, variants[label])
