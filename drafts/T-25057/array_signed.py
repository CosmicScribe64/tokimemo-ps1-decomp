"""Test signed coordinate arithmetic on the promoted memory remainder."""
from pathlib import Path
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe
probe.ROOT = Path('build/T-25057')
name = 'func_801398EC'
base = (probe.ROOT/name/'array-store-first/body.c').read_text()
variants = {
 'array-signed': base.replace('    u32 tile[1];', '    s32 tile[1];').replace('if (tile[0] < 0)', 'if ((u32)tile[0] < 0)'),
 'array-signed-use': base.replace('tile[0] * 0x10', '(s32)tile[0] * 0x10'),
}
for label in sys.argv[1:]:
    probe.probe(name, label, variants[label])
