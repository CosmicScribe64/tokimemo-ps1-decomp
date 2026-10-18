"""Write one MASTER source-lead probe input; run only via tools/docker.sh."""
import json
from pathlib import Path
out = Path('build/T-24048-probes')
out.mkdir(parents=True, exist_ok=True)
(out / 'reproduce.json').write_text(json.dumps([
    ['selected', Path('drafts/T-24048/selected.c').read_text()]
]))
