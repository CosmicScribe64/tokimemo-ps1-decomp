"""Install a guarded fresh near-match seed without changing existing C."""
from pathlib import Path
import sys,json
sys.path.insert(0,'tools');import bulk_m2c as bulk
unit,func,ticket=sys.argv[1:];folder=Path('/scratch/drafts-14500/drafts/T-14500')/unit/func;r=json.loads((folder/'result.json').read_text());assert not r['guarded'];p=Path(r['src']);s=p.read_text();s=bulk.insert_draft(s,func,(folder/'body.c').read_text(),(folder/'declarations.h').read_text().splitlines()).replace('NON_MATCHING: T-11190','NON_MATCHING: '+ticket);p.write_text(s)
