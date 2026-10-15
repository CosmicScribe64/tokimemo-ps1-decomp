"""T15800 NAME_ENT callee-ABI reconstruction and ordinary full-TU context preparation."""
from pathlib import Path
import json,os,re,sys
sys.path.insert(0,'/work/tools')
import bulk_m2c as bulk,m2c,autotweak
os.chdir('/work/build/t15800/root')
O=Path('/work/drafts/T-15800');B=Path('/scratch/drafts-14500/drafts/T-14500')
for f in ['func_80140F90','func_80141820']:
 d=O/'NAME_ENT'/f
 raw=(B/'NAME_ENT'/f/'m2c.c').read_text()
 decl,body=m2c.split_draft(raw,f)
 body=re.sub(r'func_80054E6C\([^;]*\);','func_80054E6C();',body)
 original=Path('src/ovl/NAME_ENT/80139740.c').read_text();headers=bulk.header_text('.','NAME_ENT')
 body,known,problem=bulk.cleanup(decl,body,bulk.collect_vars(headers+'\n'+original));assert not problem,problem
 decls=sorted(known.values())+bulk.forward_decls(original,f,body,headers,known)
 # These globals already exist in the frozen overlay and common headers.
 decls=[x for x in decls if not bulk.prototype_in(headers,bulk.decl_name(x) or '')] 
 (d/'body.c').write_text(body)
 (d/'declarations.h').write_text('\n'.join(decls)+'\n')
 plain=autotweak.rewrite_guard(bulk.insert_draft(original,f,body,decls),f,body)
 (d/'context.c').write_text(plain)
 p=json.loads((d/'provenance.json').read_text());p['callee_abi_reconstruction']='func80054E6C takes no incoming arguments: all argument registers are defined before first use in target callee; existing NAME_ENT and ETC source calls noarg. Remove spurious m2c unsetargument placeholders, without fabricated values.'
 (d/'provenance.json').write_text(json.dumps(p,indent=2)+'\n')
