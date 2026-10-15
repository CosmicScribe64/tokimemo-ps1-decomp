from pathlib import Path
import sys,re,json
sys.path.insert(0,'tools')
import bulk_m2c as bulk,autotweak,funcloc,funcdiff,order_search
for func in ['func_801454F0','func_8013AF1C']:
 old=Path('/ref/drafts/T-14500/RPG_BAT')/func;row=json.loads((old/'result.json').read_text());src=row['src'];original=Path(src).read_text();body=(old/'body.c').read_text();decl=(old/'declarations.h').read_text().splitlines()
 if func.endswith('AF1C'):
  body=body.replace('&D_800E6354','D_800E6280.unk_0D4');decl=[d.replace('extern s8 D_801213','extern u8 D_801213') for d in decl if 'D_800E6354' not in d]
 c=funcloc.locate(func,src);expected,err=funcdiff.expected_object(c);assert not err,err;want=funcdiff.resolved_functions(expected,[func],c)[func]
 variants={}
 # Replace decompiler register-based pseudo-locals by ordinary source-level lifetime reuse.
 draft=re.sub(r'    s32 temp_v[01](?:_\d)?;\n','',body)
 draft=re.sub(r'\btemp_v[01](?:_\d)?\b','counter',draft).replace('{','{\n    s32 counter;',1);variants['one-counter-local']=draft
 if func.endswith('454F0'):
  draft=re.sub(r'    s32 temp_v[01](?:_\d)?;\n','',body);draft=re.sub(r'\btemp_v0(?:_\d)?\b','delay',draft);draft=re.sub(r'\btemp_v1(?:_\d)?\b','distance',draft).replace('{','{\n    s32 delay;\n    s32 distance;',1);variants['role-locals']=draft
 for name,draft in variants.items():
  folder=Path('build/t14900')/func/name;text=autotweak.rewrite_guard(bulk.insert_draft(original,func,draft,decl),func,draft)
  try:
   obj=order_search.compile_variant(c,text,folder,False);got=funcdiff.resolved_functions(obj,[func])[func];diff=funcdiff.diff_lines(want,got);score=0 if funcdiff.keys(want)==funcdiff.keys(got) else len(diff)
   (folder/'comparison.txt').write_text('\n'.join(diff));(folder/'body.c').write_text(draft);print(func,name,score,flush=True)
  except Exception as exc:print(func,name,str(exc),flush=True)
