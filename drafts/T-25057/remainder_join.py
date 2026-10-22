"""Expose the remainder local before frame/index through a defined merged value."""
from pathlib import Path
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe
probe.ROOT = Path('build/T-25057')
name = 'func_801398EC'
base = (probe.ROOT/name/'record-word-read/body.c').read_text()
base = base.replace('                if (tile < 0) {}\n', '')
base = base.replace('        if (tmp == 0)', '        tile = 0;\n        if (tmp == 0)')
end = base.rfind('    }')
base = base[:end] + '        if (tile < 0) {}\n' + base[end:]
variants = {
 'remainder-join': base,
 'remainder-join-identity': base.replace('        tile = 0;', '        tile = (u32)i < 0;'),
}
for label in sys.argv[1:]:
    probe.probe(name, label, variants[label])
