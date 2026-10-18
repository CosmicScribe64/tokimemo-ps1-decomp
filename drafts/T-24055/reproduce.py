"""Prepare three source-only ETC probes; run through tools/docker.sh."""
import json
from pathlib import Path
out = Path('build/T-24055-probes')
out.mkdir(parents=True, exist_ok=True)
for label in ('model', 'fill', 'scroll'):
    body = Path('drafts/T-24055', label + '.c').read_text()
    (out / (label + '-selected.json')).write_text(json.dumps([[label, body]]))
