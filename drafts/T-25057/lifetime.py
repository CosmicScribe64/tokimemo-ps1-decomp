"""Test late-removed remainder use and induction numbering, inside Docker."""
from pathlib import Path
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe
probe.ROOT = Path('build/T-25057')
name = 'func_801398EC'
base = (probe.ROOT/name/'remainder-late-test/body.c').read_text()
base = base.replace('if (tile) {}', 'if (tile < 0) {}')
variants = {
 'remainder-unsigned-test': base,
 'index-first': base.replace('    tmp =', '    i = 0;\n    tmp =').replace('for (i = 0;', 'for (;'),
}
for label in sys.argv[1:]:
    probe.probe(name, label, variants[label])
