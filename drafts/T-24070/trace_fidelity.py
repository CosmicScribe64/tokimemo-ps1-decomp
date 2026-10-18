"""Audit parent-approved existing unchanged tracer on pinned39 input; Docker only."""
from pathlib import Path
import hashlib
import json
import sys
sys.path.insert(0, 'tools')
from trailing_pad import Elf


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


source = Path('build/etc24070/direct-caps/source.c')
stock = Path('build/etc24070/direct-caps/source.o')
logged = Path('build/etc24070/trace39/baseline.o')
trace_source = Path('/scratch/variants/trace.c')
tracer = Path('/scratch/variants/trace')
assert sha(source) == '73c73dcbc9e8f137a3caaff17210e08cb79005644c46e3a60058be18f0ad0f44', 'input changed'
assert sha(trace_source) == '59767ece4ae04ffd19ed97909d3f4099ca46e08035a2fecf59c599a8c704c75c', 'tracer source changed'
assert sha(tracer) == '0fdbd5deb0fe03670b77ce878a0cd0efe98a8aa62ada0fdff1ef00634c73164b', 'tracer binary changed'
a, b = stock.read_bytes(), logged.read_bytes()
assert a == b, 'whole object differs; do not interpret trace'
assert sha(stock) == 'd597f00bf1b5f61de61d0245f98e27a7c928b75891768e4cd3be052f65eae61e', 'stock changed'
x, y = Elf(a), Elf(b)
sections = {}
for name in ['.text', '.rel.text', '.rodata', '.rel.rodata']:
    l = x.sections[x.find(name)]['data']
    r = y.sections[y.find(name)]['data']
    assert l == r, name
    sections[name] = len(l)
meta = {str(p): sha(p) for p in [source, stock, logged, trace_source, tracer]}
meta['equal_sections'] = sections
Path('build/etc24070/trace39/audit.json').write_text(json.dumps(meta, indent=2) + '\n')
print('PASS pinned same-input whole object and code/data/relocations identical.')
