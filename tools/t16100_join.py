"""Join complete per-worker reviews and context-qualified draft alternatives."""
import json
import hashlib

from pathlib import Path
from refresh_draft_ledgers import checked
root=Path('drafts/T-16100')
manifest=json.loads((root/'manifest.json').read_text());scopes={r['scope'] for r in manifest}
reviews=[];catalog=[]
for n in range(1,4):
 folder=root/f'batch-{n}'
 batch=json.loads((root/f'batch-{n}.json').read_text());owned={r['scope'] for r in batch}
 rows=[json.loads(x) for x in (folder/'review.jsonl').read_text().splitlines() if x.strip()]
 assert len(rows)==len({r['scope'] for r in rows})==55 and {r['scope'] for r in rows}==owned,(n,'review incomplete')
 reviews.extend(rows)
 for line in (folder/'catalog.jsonl').read_text().splitlines():
  r=json.loads(line);assert r['scope'] in owned
  catalog.append(checked(r))
  folder_path=Path(r.get("path") or r.get("archive_path") or r["repair_path"])
  for filename in ("body.c","declarations.h","context.c","full-context.c","diagnostics.txt"):
   for field in (filename+"_sha256",filename.replace(".","_")+"_sha256"):
    if r.get(field):
     assert hashlib.sha256((folder_path/filename).read_bytes()).hexdigest()==r[field],str(folder_path/filename)

assert len(reviews)==len({r['scope'] for r in reviews})==165 and {r['scope'] for r in reviews}==scopes
out=Path('drafts/T-14500/repairs/t16100');out.mkdir(exist_ok=True)
(out/'catalog.jsonl').write_text(''.join(json.dumps(r,sort_keys=True)+'\n' for r in catalog))
(root/'review.jsonl').write_text(''.join(json.dumps(r,sort_keys=True)+'\n' for r in reviews))
print('Reviewed',len(reviews),'compiled alternatives',len(catalog))
