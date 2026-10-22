"""Keep the promoted memory remainder conditional and order its discovery."""
from pathlib import Path
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe
probe.ROOT = Path('build/T-25057')
name = 'func_801398EC'
base = (probe.ROOT/name/'record-word-read/body.c').read_text()
base = base.replace('    u32 tile;', '    u32 tile[1];').replace('tile =', 'tile[0] =').replace('tile *', 'tile[0] *').replace('if (tile <', 'if (tile[0] <')
variants = {
 'array-conditional': base,
 'array-early-zero': base.replace('        if (tmp == 0)', '        tile[0] = 0;\n        if (tmp == 0)'),
 'array-early-frame': base.replace('        if (tmp == 0)', '        tile[0] = 0;\n        if (D_800E6280.unk_10F8 < 0) {}\n        if (tmp == 0)'),
}
for label in sys.argv[1:]:
    probe.probe(name, label, variants[label])
