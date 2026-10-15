"""Remove empty generated declaration markers after normalization."""
from pathlib import Path
import json
for name in {r['src'] for r in json.loads(Path('build/t14500/rows.json').read_text())}:
 p=Path(name);p.write_text(p.read_text().replace('/* bulk_m2c declarations (T-11190) */\n/* end bulk_m2c declarations */\n',''))
