"""Separate a memory-backed local from the frame expression without volatile."""
from pathlib import Path
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe
probe.ROOT = Path('build/T-25057')
name = 'func_801398EC'
base = (probe.ROOT/name/'precompute-remainder/body.c').read_text()
variants = {
 'remainder-array': base.replace('    u32 tile;', '    u32 tile[1];').replace('tile =', 'tile[0] =').replace('tile *', 'tile[0] *').replace('if (tile <', 'if (tile[0] <'),
 'remainder-address': base.replace('        tile =', '        if ((u32)&tile < 0) {}\n        tile ='),
}
for label in sys.argv[1:]:
    probe.probe(name, label, variants[label])
