from pathlib import Path
import sys,json,re
sys.path.insert(0,'tools')
import bulk_m2c as bulk, autotweak
archive=Path('/scratch/drafts-14500/drafts/T-14500')
if len(sys.argv)>2:
    archive=archive/sys.argv[2]
rows=[json.loads(p.read_text()) for p in (archive/sys.argv[1]).glob('*/result.json')]
rows=[r for r in rows if r['status']=='candidate-zero']
rows=[r for r in rows if re.search(r'INCLUDE_ASM\([^;\n]*\b'+re.escape(r['func'])+r'\b',Path(r['src']).read_text())]
for r in rows:
    folder=archive/r['unit']/r['func']
    path=Path(r['src']); old=path.read_text()
    body=(folder/'body.c').read_text()
    decls=(folder/'declarations.h').read_text().splitlines()
    placed=bulk.insert_draft(old,r['func'],body,decls)
    plain=autotweak.rewrite_guard(placed,r['func'],body)
    path.write_text(plain)
Path('build/t14500').mkdir(exist_ok=True)
Path('build/t14500/rows.json').write_text(json.dumps(rows,indent=2))
print('Placed',len(rows),'candidates',sum(r['size'] for r in rows),'bytes')
for path in {Path(r['src']) for r in rows}:
    text=path.read_text().replace('/* bulk_m2c declarations (T-11190) */\n/* end bulk_m2c declarations */\n','')
    path.write_text(text)
