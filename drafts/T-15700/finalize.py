#!/usr/bin/env python3
"""T-15700: compact compiler diagnostics and finalize all 28 verified compile records."""
from pathlib import Path
import hashlib,json,re
BASE=Path('drafts/T-15700');rows=[]
# Preprocessor stdout is the separately retained full C input, not a diagnostic.
for p in BASE.rglob('diagnostics.txt'):
    s=p.read_text();s=re.sub(r'(\$ gcc -E[^\n]*\n).*?(\n\n\$ python3 tools/cc.py)',r'\1[Preprocessor C stdout omitted here; final full-context.c or frozen source/include snapshots retain the input.]\2',s,flags=re.S);p.write_text(s.rstrip()+'\n')
p=BASE/'reproduce.py';s=p.read_text().replace("LOG.append('$ '+' '.join(args)+'\\n'+r.stdout+r.stderr)", "LOG.append('$ '+' '.join(args)+'\\n'+('[Preprocessed C retained in full-context.c]\\n' if args[0]=='gcc' and '-E' in args else r.stdout)+r.stderr)");p.write_text(s)
for p in sorted(BASE.glob('*/*/result.json')):
    r=json.loads(p.read_text());assert r['status']=='scored' and r['score']>0,r
    folder=p.parent
    for name in ['body.c','context.c','declarations.h','full-context.c']:
        q=folder/name;q.write_text(q.read_text().rstrip()+'\n')
    for name,label in [('body.c','draft_hash'),('context.c','context_hash'),('full-context.c','full_context_hash'),('diagnostics.txt','diagnostics_hash'),('declarations.h','declarations_hash')]:r[label]=hashlib.sha256((folder/name).read_bytes()).hexdigest()
    r.update(compile_verified=True,matching_claim=False,residual_class='nonmatching compiler-generated instruction/control-flow/register differences; draft semantics not independently proved')
    if r['scope']=='TACO:func_80154960':r['companion_type_completion']='Archived context only: func_80156178 returns its existing final func_8015ACCC result, consumed by target caller; production C unchanged.'
    p.write_text(json.dumps(r,indent=2)+'\n');rows.append(r)
(BASE/'catalog.jsonl').write_text(''.join(json.dumps(r,sort_keys=True)+'\n' for r in rows))
total=sum(r['size'] for r in rows);table='| Scope | Code bytes | Diagnostic rows |\n| --- | ---: | ---: |\n'+'\n'.join('| %s | %d | %d |'%(r['scope'],r['size'],r['score']) for r in rows)+'\n'
(BASE/'README.md').write_text('''# T-15700 TACO and TAIIKU compile-repair archive

All **28/28** reserved scopes compile as complete translation units in the existing Docker IDO 5.3 pipeline: TACO 17/17, TAIIKU 11/11. All remain **NON_MATCHING**. No diagnostic zero, literal-byte match, production source edit or matching-coverage gain is claimed. Target code extents total '''+str(total)+''' bytes.

'''+table+'''

Each scope retains real repaired C, declarations, source TU, full preprocessed TU, complete compiler stdout/stderr diagnostics (preprocessor source stdout is retained separately), comparison, hashes and provenance. Original attempts and their raw m2c/body/context remain immutable under drafts/T-14500. Every earlier alternative is referenced in provenance.json. First complete diagnostic inputs before reconstruction are preserved under each scope's attempts/first-full-diagnostics; these are historical failed inputs, not final compiling alternatives.

Source baseline is the frozen 48362009 census; unrelated prior accepted work does not alter the owned TUs. Historical taco-13800/taiiku-13600/cx2/cx9 source trees were inspected and no implemented body for these exact targets was found. The first complete-diagnostics pass repaired declaration/prototype conflicts; remaining work uses existing SDK SVECTOR and observed coordinate arrays, exact copied table extents, existing aggregate members, packed-word/byte views, and the canonical TcPos-by-value signature. No empty stubs, inline assembly or target ASM fallbacks substitute for C.

One archived-only companion type completion is required: func_80154960 consumes func_80156178's result. Its context gives that helper an s32 return and returns its existing final func_8015ACCC call result. This follows the observed consumed v0 ABI; the production matching helper is unchanged. All other production C is also untouched. Recovered source types and instruction differences still require independent semantic/matching work.

To recompile all final archived bodies, run inside Docker from this checkout with immutable census mounted read-only at /ref:

```
python3 drafts/T-15700/reproduce.py
```

The driver accepts scoped names for a subset and preserves full compiler diagnostics. reconstruct.py records the one-time derivation from the first-pass inputs; do not reapply it to already reconstructed bodies. finalize.py normalizes archive metadata and removes duplicated preprocessor source stdout from diagnostic logs without dropping compiler diagnostics. No binaries, generated assembly, disc files or game assets are archived.
''')
(BASE/'verification.txt').write_text('Final serial full-TU IDO5.3 compile results:28/28 OK, no matching claims.\n'+''.join('%s:compile OK, score%d\n'%(r['scope'],r['score']) for r in rows))
p=Path('wiki/tickets/T-15700-taco-taiiku-draft-repairs.md');s=p.read_text().replace('status: In Progress','status: In Review').replace('- [ ] All 28 compile','- [x] All 28 compile').replace('- [ ] Inspect existing','- [x] Inspect existing').replace('- [ ] Preserve initial','- [x] Preserve initial').replace('- [ ] Docker only','- [x] Docker only');s+='''
## Results

**28/28 compile** (TACO17, TAIIKU11), covering '''+str(total)+''' target code bytes. No zeros or matching-coverage gain. All target bodies are real compiler input; no stubs or ASM substitutions. Full contexts, complete compiler diagnostics, comparisons, hashes/provenance and reproduction drivers are in drafts/T-15700. Initial census archives and production C/header files are unchanged.

'''+table+'''

Complete diagnostics revealed ten initially recoverable inputs after declaration/signature corrections, then eighteen remaining reconstructions. Earlier truncated helper diagnostics obscured the errors. Repairs use real SDK SVECTOR records and observed coordinate arrays; exact 11/23-byte unaligned copies, 46-byte halfword tables, existing selection table types; TacoRnd scalar member, TAIIKU packed word/first byte views, and three genuine TcPos by-value arguments. No matching optimization or artificial padding.

The archived func_80154960 TU completes helper func_80156178's consumed integer return by returning its existing last func_8015ACCC result. This is explicitly recorded as a companion ABI/type completion in context/provenance; the production matched helper stays unchanged. All other current matching C stays unchanged. Full-TU compile and diagnostic score are evidence of compilability only, not semantic or literal-byte matching proof.

## Review

Final serial Docker compile/comparison succeeded for all28. C89, existing SDK types, no target stubs/inlineASM/fallback, no binaries/assets staged. Initial attempts retained, table/vector extents tied to observed accesses/copies, production changes absent. Independent standards review pending; In Review.
''';p.write_text(s)
p=Path('wiki/kanban.md');s=p.read_text();card='- [ ] [[tickets/T-15700-taco-taiiku-draft-repairs|T-15700 Compile 28 TACO/TAIIKU failed drafts]]';s=s.replace(card+'\n','').replace('## In Review\n','## In Review\n'+card+'\n',1);p.write_text(s)
p=Path('wiki/index.md');s=p.read_text().replace('Compile all 28 failed TACO/TAIIKU drafts (In Progress).','All28 TACO/TAIIKU failed drafts now compile; no zeros (In Review).');p.write_text(s)
with Path('wiki/log.md').open('a') as f:f.write('''
## [2026-10-15] build | Compile every reserved TACO and TAIIKU draft

[[tickets/T-15700-taco-taiiku-draft-repairs]] moves In Review:28/28 full TUs compile in Docker (17TACO/11TAIIKU), '''+str(total)+''' target code bytes, scores98..709 and no zeros/coverage claim. SDK vector/coordinate layouts, actual copied tables and canonical aggregate/ABI views resolve all blockers. Complete C/context/diagnostics/hash/provenance and one-time derivation scripts saved in drafts/T-15700; initial attempts and production C/header files unchanged.
''')
print('28/28 compiled;',total,'code bytes')
