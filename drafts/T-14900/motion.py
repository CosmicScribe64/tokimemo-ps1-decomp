from pathlib import Path
import sys,re,json
sys.path.insert(0,'tools')
import bulk_m2c as bulk,autotweak,funcloc,funcdiff,order_search
for func in ['func_80148B08','func_801454F0','func_8013AF1C']:
 old=Path('/ref/drafts/T-14500/RPG_BAT')/func;row=json.loads((old/'result.json').read_text());src=row['src'];original=Path(src).read_text();body=(old/'body.c').read_text();decl=(old/'declarations.h').read_text().splitlines()
 if func.endswith('AF1C'):
  body=body.replace('&D_800E6354','D_800E6280.unk_0D4');decl=[d.replace('extern s8 D_801213','extern u8 D_801213') for d in decl if 'D_800E6354' not in d]
 c=funcloc.locate(func,src);expected,err=funcdiff.expected_object(c);assert not err,err;want=funcdiff.resolved_functions(expected,[func],c)[func]
 variants={}
 if func.endswith('AF1C'):variants['unsigned-bytes']=body
 else:
  draft=body.replace('{','{\n    RpgMotion20 *motion = (RpgMotion20 *)&D_8015EC50;',1)
  for a,b in [('D_8015EC50','motion->dx'),('D_8015EC54','motion->dy'),('D_8015EC60','motion->rect.unk_08'),('D_8015EC64','motion->rect.unk_0C'),('D_8015EC68','motion->rect.unk_10'),('D_8015EC6C','motion->rect.unk_14')]:
   draft=draft.replace(a,b)
  draft=draft.replace('(RpgMotion20 *)&motion->dx','(RpgMotion20 *)&D_8015EC50');variants['motion-pointer']=draft
  if func.endswith('48B08'):
   # Direct aggregate member accesses retain the same recovered storage and aliases.
   variants['motion-direct']=draft.replace('    RpgMotion20 *motion = (RpgMotion20 *)&D_8015EC50;\n','').replace('motion->','((RpgMotion20 *)&D_8015EC50)->')
 for name,draft in variants.items():
  folder=Path('build/t14900')/func/name;text=autotweak.rewrite_guard(bulk.insert_draft(original,func,draft,decl),func,draft)
  try:
   obj=order_search.compile_variant(c,text,folder,False);got=funcdiff.resolved_functions(obj,[func])[func];diff=funcdiff.diff_lines(want,got);score=0 if funcdiff.keys(want)==funcdiff.keys(got) else len(diff)
   (folder/'comparison.txt').write_text('\n'.join(diff));(folder/'body.c').write_text(draft);print(func,name,score,flush=True)
  except Exception as exc:print(func,name,str(exc),flush=True)
