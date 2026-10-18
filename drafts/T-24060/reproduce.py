"""Write two SHUGAKU source-only probe inputs; run through tools/docker.sh."""
import json
from pathlib import Path
out = Path('build/T-24060-probes')
out.mkdir(parents=True, exist_ok=True)
for label in ('phase', 'selector'):
    body = Path('drafts/T-24060', label + '.c').read_text()
    (out / (label + '-selected.json')).write_text(json.dumps([[label, body]]))
