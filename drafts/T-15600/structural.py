from pathlib import Path
import sys,re,json
sys.path.insert(0,'tools')
import bulk_m2c as bulk,autotweak,funcloc,funcdiff,order_search
for unit,func in [('RENSYU','func_801320E0'),('RPG_BAT','func_80155A70'),('SHOUGATU','func_80137144')]:
 old=Path('/ref/drafts/T-14500')/unit/func;row=json.loads((old/'result.json').read_text());src=row['src'];original=Path(src).read_text();body=(old/'body.c').read_text();decl=(old/'declarations.h').read_text().splitlines();variants={}
 if unit=='RENSYU':
  body=Path('build/t15600/func_801320E0/canonical-record/body.c').read_text();decl=[d for d in decl if 'D_800E68EE' not in d]
  body=body.replace('D_800E6280.unk_66C[(u16) D_800E6374 >> 0xC].unk_02','*(u16 *)&D_800E6280.unk_66C[(u16) D_800E6374 >> 0xC].unk_02');variants['packed-halfword']=body
 elif unit=='SHOUGATU':
  body=body.replace('            D_800E7380 += 1;\n            if ((u32) D_800E7380 >= 0x401U)', '            if (D_800E7380++ >= 0x401U)');variants['postincrement']=body
 else:
  variants['signed-status']=body.replace('(D_8015EDF0 & 0x80000000)','(D_8015EDF0 < 0)')
  start=body.index('    if (');end=body.index('    if (D_8015EDF0 == 1)',start)
  prefix='''    if (!(D_8015ED98 & 0xFF) && !(D_8015EDC4 & 0xFF) && !(D_8015EDF0 & 0xFF)) {
        if (D_8015EDF0 & 0x80000000) {
            if (((s32) D_8015EDA0 / 10) < (D_8015EDA0 - D_8015ED94)) {
                func_8013D030();
                D_8015EDF0 = 1;
            }
        }
    }
'''
  variants['nested-conditions']=body[:start]+prefix+body[end:]
 c=funcloc.locate(func,src);expected,err=funcdiff.expected_object(c);assert not err,err;want=funcdiff.resolved_functions(expected,[func],c)[func]
 for name,draft in variants.items():
  folder=Path('build/t15600')/func/name;text=autotweak.rewrite_guard(bulk.insert_draft(original,func,draft,decl),func,draft)
  try:
   obj=order_search.compile_variant(c,text,folder,False);got=funcdiff.resolved_functions(obj,[func])[func];diff=funcdiff.diff_lines(want,got);score=0 if funcdiff.keys(want)==funcdiff.keys(got) else len(diff)
   (folder/'comparison.txt').write_text('\n'.join(diff));(folder/'body.c').write_text(draft);print(unit,name,score,flush=True)
  except Exception as exc:print(unit,name,str(exc),flush=True)
