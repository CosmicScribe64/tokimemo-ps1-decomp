"""Test lexical frame-definition order while preserving defined runtime flow."""
from pathlib import Path
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe
probe.ROOT = Path('build/T-25057')
name = 'func_801398EC'
base = (probe.ROOT/name/'precompute-remainder/body.c').read_text().replace('    u32 tile;', '    u32 tile;\n    u32 frame;')
variants = {
 'scalar-goto-order': base.replace('        tile = (D_800E6280.unk_10F8 >> 2) % 3;', '        goto read_frame;\ncalculate_tile:\n        tile = (frame >> 2) % 3;\n        goto tile_ready;\nread_frame:\n        frame = D_800E6280.unk_10F8;\n        goto calculate_tile;\ntile_ready:;'),
}
for label in sys.argv[1:]:
    probe.probe(name, label, variants[label])
