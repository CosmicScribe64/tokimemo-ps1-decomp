"""Install a guarded T-15000 seed from the frozen candidate archive."""
from pathlib import Path
import sys,json
sys.path.insert(0,'tools')
import bulk_m2c as bulk
archive=Path('/scratch/drafts-14500/drafts/T-14500')
history=json.loads((archive/'historical-attempts.json').read_text())
if isinstance(history,dict):
 for key,value in history.items():
  if '80135CE0' in str(key) or '80135CE0' in str(value): print('PRIOR',str(key),str(value)[:1200])
folder=archive/'KANGEI/func_80135CE0';p=Path('src/ovl/KANGEI/801355F0.c');s=p.read_text()
s=bulk.insert_draft(s,'func_80135CE0',(folder/'body.c').read_text(),(folder/'declarations.h').read_text().splitlines()).replace('NON_MATCHING: T-11190','NON_MATCHING: T-15000')
p.write_text(s)
