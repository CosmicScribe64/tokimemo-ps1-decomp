"""Run named NAME_ENT source controls inside project Docker, serially."""
from pathlib import Path
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe as old
old.ROOT = Path('build/T-25057')
old.ROOT.mkdir(parents=True, exist_ok=True)
name = 'func_801398EC'
base = Path('drafts/T-24515/func_801398EC.c').read_text()
named = base.replace('    s32 i;', '    s32 i;\n    u32 tile;').replace('                D_801217D0[i].unk_00 = 0;', '                tile = (D_800E6280.unk_10F8 >> 2) % 3;\n                D_801217D0[i].unk_00 = 0;').replace('((D_800E6280.unk_10F8 >> 2) % 3) * 0x10', 'tile * 0x10')
variants = {
    'current': old.body(name),
    'indexed': base,
    'remainder-test': named.replace('                D_801217D0[i].unk_0E', '                if (tile) {}\n                D_801217D0[i].unk_0E'),
    'remainder-late-test': named.replace('tile * 0x10 + 0x80;', 'tile * 0x10 + 0x80;\n                if (tile) {}'),
}
if __name__ == '__main__':
    for label in sys.argv[1:]:
        old.probe(name, label, variants[label])
