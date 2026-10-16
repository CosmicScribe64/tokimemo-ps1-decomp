from pathlib import Path
import sys,re,json
sys.path.insert(0,'tools')
import bulk_m2c as bulk,autotweak,funcloc,funcdiff,order_search
for unit,func in [('RENSYU','func_801320E0'),('RPG_BAT','func_80155A70'),('SHOUGATU','func_80137144')]:
 old=Path('/ref/drafts/T-14500')/unit/func;row=json.loads((old/'result.json').read_text());src=row['src'];original=Path(src).read_text();body=(old/'body.c').read_text();decl=(old/'declarations.h').read_text().splitlines()
 c=funcloc.locate(func,src);expected,err=funcdiff.expected_object(c);assert not err,err;want=funcdiff.resolved_functions(expected,[func],c)[func]
 variants={'baseline':(body,decl)}
 if unit=='RENSYU':
  canonical=body.replace('(*(&D_800E68EE + (((u16) D_800E6374 >> 0xC) * 4)) & 0xF)','(D_800E6280.unk_66C[(u16) D_800E6374 >> 0xC].unk_02 & 0xF)');canonical_decl=[d for d in decl if 'D_800E68EE' not in d];variants['canonical-record']=(canonical,canonical_decl)
  pointer=canonical.replace('{','{\n    GameState *gs = &D_800E6280;',1)
  for a,b in [('D_800E6280','gs[0]'),('D_800E738A','gs->unk_110A'),('D_800E6374','gs->unk_0F4.h'),('D_800E7374','gs->unk_10F4')]:pointer=pointer.replace(a,b)
  pointer=pointer.replace('GameState *gs = &gs[0];','GameState *gs = &D_800E6280;').replace('gs[0].','gs->');variants['game-pointer']=(pointer,canonical_decl)
 elif unit=='RPG_BAT':
  pointer=body.replace('{','{\n    RpgBatWords4 *state = &D_8015EE00;',1).replace('D_8015EE04','(u32)state->unk_04');variants['state-pointer']=(pointer,decl)
 else:
  pointer=body.replace('{','{\n    GameState *gs = &D_800E6280;',1).replace('D_800E7389','gs->unk_1109').replace('D_800E7380','gs->unk_1100');variants['game-pointer']=(pointer,decl)
 for name,(draft,declarations) in variants.items():
  folder=Path('build/t15600')/func/name;text=autotweak.rewrite_guard(bulk.insert_draft(original,func,draft,declarations),func,draft)
  try:
   obj=order_search.compile_variant(c,text,folder,False);got=funcdiff.resolved_functions(obj,[func])[func];diff=funcdiff.diff_lines(want,got);score=0 if funcdiff.keys(want)==funcdiff.keys(got) else len(diff)
   (folder/'comparison.txt').write_text('\n'.join(diff));(folder/'body.c').write_text(draft);print(unit,func,name,score,flush=True)
  except Exception as exc:print(unit,func,name,str(exc),flush=True)
