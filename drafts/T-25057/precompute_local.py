"""Keep the precomputed remainder local across a late-eliminated assignment."""
from pathlib import Path
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe
probe.ROOT = Path('build/T-25057')
name = 'func_801398EC'
base = (probe.ROOT/name/'precompute-remainder/body.c').read_text()
base = base.replace('                if (tile < 0) {}\n', '')
variants = {
 'precompute-local': base.replace('        if (tmp == 0)', '        if ((u32)i < 0) { tile = 0; }\n        if (tmp == 0)'),
 'precompute-local-same': base.replace('        if (tmp == 0)', '        if ((u32)i < 0) { tile = tile + ((u32)i < 0); }\n        if (tmp == 0)'),
}
for label in sys.argv[1:]:
    probe.probe(name, label, variants[label])
