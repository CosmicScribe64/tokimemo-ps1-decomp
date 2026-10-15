import sys
from pathlib import Path
sys.path.insert(0,'tools')
import bulk_m2c as bulk,autotweak,funcloc,funcdiff,order_search
f='func_801431D4'; src='src/ovl/BUNKASAI/80142700.c'; base=Path('drafts/T-14500/BUNKASAI')/f
body=(base/'body.c').read_text().replace('D_800E62BE','D_800E6280.unk_03E')
c=funcloc.locate(f,src); expected,err=funcdiff.expected_object(c); assert not err,err
want=funcdiff.resolved_functions(expected,[f],c)[f]
for tag,b in [('canonical',body),('utf8',body.replace('パ'+chr(92)+'x83'+chr(92)*2+'コン占い','パソコン占い'))]:
 out=Path('build/scratch/t15400')/tag; out.mkdir(parents=True,exist_ok=True)
 text=autotweak.rewrite_guard(bulk.insert_draft(Path(src).read_text(),f,b,[]),f,b)
 (out/'candidate.c').write_text(text); (out/'body.c').write_text(b)
 obj=order_search.compile_variant(c,text,out/'work',False)
 got=funcdiff.resolved_functions(obj,[f])[f]; diff=funcdiff.diff_lines(want,got)
 (out/'diff.txt').write_text('\n'.join(diff)+'\n')
 print(tag, 'equal',funcdiff.keys(want)==funcdiff.keys(got),'rows',len(diff),flush=True)
