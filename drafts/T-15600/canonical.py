from pathlib import Path
import sys,json
sys.path.insert(0,'tools')
import bulk_m2c as bulk,autotweak,funcloc,funcdiff,order_search
for unit,func,variant,pairs in [('RENSYU','func_801320E0','packed-halfword',[('D_800E738A','D_800E6280.unk_110A'),('D_800E6374','D_800E6280.unk_0F4.h'),('D_800E7374','D_800E6280.unk_10F4')]),('SHOUGATU','func_80137144','postincrement',[('D_800E7389','D_800E6280.unk_1109'),('D_800E7380','D_800E6280.unk_1100')])]:
 row=json.loads((Path('/ref/drafts/T-14500')/unit/func/'result.json').read_text());src=row['src'];original=Path(src).read_text();body=Path('build/t15600',func,variant,'body.c').read_text()
 for old,new in pairs:body=body.replace(old,new)
 decl=[] if unit=='RENSYU' else ['void func_80137818(void);','void func_80137A04(void);','void func_801377CC(void);']
 text=autotweak.rewrite_guard(bulk.insert_draft(original,func,body,decl),func,body);folder=Path('build/t15600')/func/'canonical';c=funcloc.locate(func,src);expected,err=funcdiff.expected_object(c);assert not err;want=funcdiff.resolved_functions(expected,[func],c)[func]
 obj=order_search.compile_variant(c,text,folder,False);got=funcdiff.resolved_functions(obj,[func])[func];diff=funcdiff.diff_lines(want,got);score=0 if funcdiff.keys(want)==funcdiff.keys(got) else len(diff);(folder/'body.c').write_text(body);(folder/'comparison.txt').write_text('\n'.join(diff));print(unit,score)
