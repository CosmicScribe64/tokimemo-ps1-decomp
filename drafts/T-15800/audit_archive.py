#!/usr/bin/env python3
"""Check completeness and current hashes of all T-15800 compiling C alternatives."""
from pathlib import Path
import hashlib,json,re,sys
sys.path.insert(0,'/work/tools');import permute
base=Path('/work/drafts/T-15800');rows=[json.loads(x) for x in (base/'catalog.jsonl').read_text().splitlines()];assert len(rows)==23
for row in rows:
 assert row['status']=='compiled' and isinstance(row['score'],int) and row['compiled_instruction_count']>1,row['scope']
 folder=Path('/work')/row['path']
 for name in ['body.c','declarations.h','context.c','preprocessed.c','diagnostics.txt','provenance.json']:
  assert (folder/name).exists(),(row['scope'],name)
 for name,digest in row['hashes'].items():assert hashlib.sha256((folder/name).read_bytes()).hexdigest()==digest,(row['scope'],name)
 body=(folder/'body.c').read_text();assert 'M2C_ERROR' not in body and not re.search(r'\b(?:INCLUDE_ASM|asm|__asm__)\s*\(',body),row['scope']
 context=(folder/'context.c').read_text();rng=permute.function_range(context,row['func']);assert rng and context[rng[0]:rng[1]].strip()==body.strip(),row['scope']
 assert not re.search(r'INCLUDE_ASM\([^;\n]*\b'+re.escape(row['func'])+r'\b',context),row['scope']
print('23/23 complete compiling alternatives; final body/context/preprocessed hashes consistent; no target ASM/stub placeholders')
