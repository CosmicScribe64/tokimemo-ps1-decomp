"""Causal NAME_ENT byte lifetime controls; run serially in Docker."""
from pathlib import Path
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe as old
old.ROOT = Path('build/T-25062')
old.ROOT.mkdir(parents=True, exist_ok=True)
name = 'func_8013AF78'
base = old.body(name)
variants = {'baseline': base}
for label in sys.argv[1:]:
    old.probe(name, label, variants[label])
