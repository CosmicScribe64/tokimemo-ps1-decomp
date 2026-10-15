from pathlib import Path
import sys,re,json
sys.path.insert(0,'tools')
import bulk_m2c as bulk,autotweak,funcloc,funcdiff,order_search
for func in ['func_801454F0','func_80148B08','func_8013AF1C']:
 old=Path('/ref/drafts/T-14500/RPG_BAT')/func;row=json.loads((old/'result.json').read_text());src=row['src'];original=Path(src).read_text();body=(old/'body.c').read_text();decl=(old/'declarations.h').read_text().splitlines()
 if func.endswith('AF1C'):
  body=body.replace('&D_800E6354','D_800E6280.unk_0D4');decl=[d for d in decl if 'D_800E6354' not in d]
 c=funcloc.locate(func,src);expected,err=funcdiff.expected_object(c);assert not err,err;want=funcdiff.resolved_functions(expected,[func],c)[func]
 variants={'baseline':body}
 pointer=body.replace('{','{\n    RpgBatWords3 *state = &D_8015EDB0;',1).replace('D_8015EDB0.unk_00','state->unk_00').replace('D_8015EDB4','state->unk_04').replace('D_8015EDB8','state->unk_08');variants['state-pointer']=pointer
 if func.endswith('AF1C'):
  variants['game-pointer']=body.replace('{','{\n    GameState *gs = &D_800E6280;',1).replace('D_800E6280.unk_0D4','gs->unk_0D4')
 for name,draft in variants.items():
  folder=Path('build/t14900')/func/name;text=autotweak.rewrite_guard(bulk.insert_draft(original,func,draft,decl),func,draft)
  try:
   obj=order_search.compile_variant(c,text,folder,False);got=funcdiff.resolved_functions(obj,[func])[func];diff=funcdiff.diff_lines(want,got);score=0 if funcdiff.keys(want)==funcdiff.keys(got) else len(diff)
   (folder/'comparison.txt').write_text('\n'.join(diff));(folder/'body.c').write_text(draft);print(func,name,score,flush=True)
  except Exception as exc:print(func,name,str(exc),flush=True)
