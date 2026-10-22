"""Annotate and verify the matching source and removal controls, Docker only."""
from pathlib import Path
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe
probe.ROOT = Path('build/T-25057')
name = 'func_801398EC'
source = (probe.ROOT/name/'sum-negative-expression/body.c').read_text()
source = source.replace('/* T-24515 source-only indexed-record reconstruction; not a verified match. */', '/* T-25057: record display flags and animated tile coordinates. */')
source = source.replace('    u32 tile[1];', '    u32 tile[1]; /* FAKE: T-25057, memory local orders remainder before frame allocation. */')
source = source.replace('if ((u32)D_801217D0[i].unk_00 < 0) {}', 'if ((u32)D_801217D0[i].unk_00 < 0) {} /* FAKE: T-25057, erased test orders record induction. */')
source = source.replace('tile[0] = D_800E6280.unk_10F8;', 'tile[0] = D_800E6280.unk_10F8; /* FAKE: T-25057, dead initialization orders frame before index; removed by uopt. */')
source = source.replace('- (0U - tile[0] * 0x10) + 0x80;', '- (0U - tile[0] * 0x10) + 0x80; /* FAKE: T-25057, unsigned identity preserves row-before-tile evaluation. */')
source = source.replace('if (tile[0] < 0) {}', 'if (tile[0] < 0) {} /* FAKE: T-25057, erased use retains remainder range. */')
(Path('drafts/T-25057')/'func_801398EC-matched.c').write_text(source)
variants = {
 'annotated-match': source,
 'remove-tail-use': source.replace('                if (tile[0] < 0) {} /* FAKE: T-25057, erased use retains remainder range. */\n', ''),
 'remove-record-use': source.replace('        if ((u32)D_801217D0[i].unk_00 < 0) {} /* FAKE: T-25057, erased test orders record induction. */\n', ''),
 'remove-dead-frame': source.replace('        tile[0] = D_800E6280.unk_10F8; /* FAKE: T-25057, dead initialization orders frame before index; removed by uopt. */\n', ''),
 'remove-array': source.replace('u32 tile[1];', 'u32 tile;').replace('tile[0]', 'tile'),
 'remove-subtraction': source.replace('- (0U - tile[0] * 0x10)', '+ tile[0] * 0x10'),
}
for label in sys.argv[1:]:
    probe.probe(name, label, variants[label])
