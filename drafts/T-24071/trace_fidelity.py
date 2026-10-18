"""Audit approved unchanged tracer on pinned57 input; Docker only."""
from pathlib import Path
import argparse
import hashlib
import json
import sys
sys.path.insert(0, 'tools')
from trailing_pad import Elf


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


source = Path('build/etc24071/replay/buttons/source.c')
stock = source.with_suffix('.o')
input_sha = '18513169b0051a22a8833ec96928c814cbf9f3e1c53135757be76bb0318a99e0'
object_sha = '9eadc551036a57ba4c3329d6c40b164c8e8d0e5cce7f4a9f11d24514d5265a62'
trace_root = Path('build/etc24071/trace57')
logged = trace_root / 'baseline.o'
trace_source = Path('/scratch/variants/trace.c')
tracer = Path('/scratch/variants/trace')
assert sha(source) == input_sha, 'input changed'
assert sha(trace_source) == '59767ece4ae04ffd19ed97909d3f4099ca46e08035a2fecf59c599a8c704c75c', 'tracer source changed'
assert sha(tracer) == '0fdbd5deb0fe03670b77ce878a0cd0efe98a8aa62ada0fdff1ef00634c73164b', 'tracer binary changed'
a, b = stock.read_bytes(), logged.read_bytes()
assert a == b, 'whole object differs; do not interpret trace'
assert sha(stock) == object_sha, 'stock changed'
x, y = Elf(a), Elf(b)
sections = {}
for name in ['.text', '.rel.text', '.rodata', '.rel.rodata']:
    l = x.sections[x.find(name)]['data']
    r = y.sections[y.find(name)]['data']
    assert l == r, name
    sections[name] = len(l)
meta = {str(p): sha(p) for p in [source, stock, logged, trace_source, tracer]}
meta['equal_sections'] = sections
(trace_root / 'audit.json').write_text(json.dumps(meta, indent=2) + '\n')
print('PASS pinned same-input whole object and code/data/relocations identical.')
