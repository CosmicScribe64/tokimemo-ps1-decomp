"""Write the selected SHUGAKU source input; use tools/docker.sh."""
import json
from pathlib import Path
out = Path('build/T-24053-probes')
out.mkdir(parents=True, exist_ok=True)
(out / 'reproduce.json').write_text(json.dumps([
    ['selected', Path('drafts/T-24053/selected.c').read_text()]
]))
