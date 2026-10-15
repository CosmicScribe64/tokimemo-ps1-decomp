"""Preserve the unfinished TACO experiment as source-only reviewable artifacts."""
from pathlib import Path
import sys,json,hashlib,shutil
sys.path.insert(0,'tools');import funcloc
source=Path('src/ovl/TACO/801335A0.c');context=source.read_text();func='func_80133738';start=context.index('void '+func+'(void) {');end=context.index('\n#else',start);body=context[start:end]+'\n'
folder=Path('drafts/T-15300/TACO')/func;folder.mkdir(parents=True,exist_ok=True)
(folder/'body.c').write_text(body)
(folder/'context.c').write_text(context)
(folder/'declarations.h').write_text('extern TcPair8 D_8015E238[];\n')
headers={}
for path in sorted(funcloc.includes_of(source)):
 target=folder/'header-context'/path;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(path,target);headers[str(path)]=hashlib.sha256(path.read_bytes()).hexdigest()
for suffix in ['seed','state','structure']:
 p=Path('/tmp/t15300-taco-'+suffix+'-diff.log')
 # Host logs are outside the container mount; copied into build/ before this script.
 p=Path('build/t14500/taco-'+suffix+'-diff.log')
 if p.exists():shutil.copyfile(p,folder/(suffix+'-diagnostics.txt'))
for filename in ['t15300_taco_state.py','t15300_taco_structure.py']:
 target=folder/'reproduce'/filename;target.parent.mkdir(exist_ok=True);shutil.copyfile(Path('build/t14500')/filename,target)
meta={'ticket':'T-15300','unit':'TACO','func':func,'src':str(source),'status':'nonmatching-guarded-archive','promoted':False,'inventory_coverage_bytes':308,'literal_differing_instruction_positions':8,'literal_comparison_mode':'NON_MATCHING C (full translation unit)','base_checkout_commit':Path('build/t14500/archive-head.txt').read_text().strip(),'body_sha256':hashlib.sha256(body.encode()).hexdigest(),'context_sha256':hashlib.sha256(context.encode()).hexdigest(),'headers_sha256':headers,'remaining':'selector stack home +0x2C instead of +0x28 (4 positions); lookup temporary registers t1/t0 instead of t2/t1 (4 positions)','default_production':'INCLUDE_ASM retained; archived source is not production C'}
(folder/'result.json').write_text(json.dumps(meta,indent=2)+'\n')
(folder/'README.md').write_text('''# TACO func_80133738 unfinished structural probe

This source-only archive is nonmatching and is not used by the default build. The production source retains its original INCLUDE_ASM. `context.c` is the exact guarded translation unit that produced `structure-diagnostics.txt`; `header-context/` freezes its quoted header closure. The frozen first-attempt seed remains in T-14500's TACO catalog.

The m2c seed split one saved selector into a dead spill local and a separate selector, and its inferred scalar table pointer incorrectly scaled the index by 32 rather than 8. The experiment restores one selector initialized before calls, models the six eight-byte lookup entries using existing TcPair8, and replaces byte offset 0x4B8 with existing TcActor element 8's field at 0x78. These are ordinary data/flow corrections, not padding or forced registers.

The result still differs at eight instruction positions: four selector spills/reloads use stack +0x2C rather than +0x28; four lookup instructions use adjacent temporary registers. It is not a literal match. No further variants were tested after the user requested a handoff. Reordering the two genuine local declarations is an untested next lead; no result is claimed.

`seed-diagnostics.txt` records the canonical-global normalization issue. `state-diagnostics.txt` records the normalized raw seed; `structure-diagnostics.txt` records the latest compiled variant. `reproduce/` contains the actual local transformation scripts, which run in the project Docker image against a checkout with the guarded seed installed. No assembly files, original executables, or binary build outputs are included.
''')
source.write_text(Path('build/t14500/taco-production.c').read_text())
print('Archived unfinished TACO source/context and',len(headers),'headers; restored production source')
