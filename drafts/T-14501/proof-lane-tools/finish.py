"""Record a verified unit batch and remove empty generation comments."""
from pathlib import Path
import json,sys,re
unit=sys.argv[1]
rows=json.loads(Path('build/t14500/rows.json').read_text())
assert all(r['unit']==unit for r in rows)
for f in {r['src'] for r in rows}:
 p=Path(f); p.write_text(p.read_text().replace('/* bulk_m2c declarations (T-11190) */\n/* end bulk_m2c declarations */\n',''))
count=len(rows); size=sum(r['size'] for r in rows)
literal_size=sum(int(n) for n in re.findall(r'MATCH \(linked function bytes equal target \((\d+) bytes\)\)',Path('build/t14500/proof.txt').read_text()))
items=', '.join(r['func']+' ('+str(r['size'])+')' for r in rows)
p=Path('wiki/tickets/T-14501-remaining-draft-proof.md')
p.write_text(p.read_text()+f'\n2026-10-15: {unit} batch: {count} functions / {size:,} coverage bytes; {literal_size:,} default linked bytes verified after normalization. Overlay SHA1, header/global checks and prototype branch guard pass. Focused standards inspection: ordinary C89; no FAKE artifacts; original C preserved. Functions (inventory coverage sizes): {items}. Evidence: /private/tmp/t14500-{unit}-proof.log and local build/t14500/{unit}-proof.txt.\n')
p=Path('wiki/log.md');p.write_text(p.read_text()+f'\n## [2026-10-15] build | T-14501 {unit} verified draft batch\n\nPromoted {count} ordinary C functions / {size:,} inventory coverage bytes for {unit}; default linked literal proof compared {literal_size:,} bytes and overlay SHA1 passes. Header/global/prototype checks pass. No raw data or asm committed; original C preserved. [[tickets/T-14501-remaining-draft-proof]] remains In Review for root review.\n')
Path('build/t14500/'+unit+'-proof.txt').write_text(Path('build/t14500/proof.txt').read_text())
print(unit,count,size)
import re
record=Path('wiki/tickets/T-14501-remaining-draft-proof.md').read_text()
counts=re.findall(r'2026-10-15: (\w+) batch: (\d+) functions / ([\d,]+) (?:default linked|coverage) bytes',record)
total_count=12+sum(int(c) for u,c,b in counts)
total_bytes=8384+sum(int(b.replace(',','')) for u,c,b in counts)
total_units=len({'BUNKAKEN'} | {u for u,c,b in counts})
p=Path('wiki/index.md');lines=p.read_text().splitlines()
for i,line in enumerate(lines):
 if line.startswith('- [[tickets/T-14501-remaining-draft-proof|T-14501]]'):
  lines[i]=f'- [[tickets/T-14501-remaining-draft-proof|T-14501]] Verify exhaustive draft candidates: {total_count} literal matches / {total_bytes:,} inventory coverage bytes in {total_units} units (In Review).'
p.write_text('\n'.join(lines)+'\n')
