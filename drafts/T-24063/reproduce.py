"""Prepare the corrected random-buffer source probe; run through tools/docker.sh."""
import json
from pathlib import Path
out = Path('build/T-24063-probes')
out.mkdir(parents=True, exist_ok=True)
(out / 'random-selected.json').write_text(json.dumps([
    ['random', Path('drafts/T-24063/random.c').read_text()]
]))
