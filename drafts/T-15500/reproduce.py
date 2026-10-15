import sys
from pathlib import Path
sys.path.insert(0,'tools')
import bulk_m2c as bulk,autotweak,funcloc,funcdiff,order_search
f='func_8013FEB4'; src='src/ovl/BUNKASAI/8013FD90.c'
b=Path('drafts/T-14500/BUNKASAI/'+f+'/body.c').read_text().replace('D_800E738A','D_800E6280.unk_110A').replace('D_800E71DF','D_800E6280.unk_F5F').replace('D_800E6679 |= 0x20','D_800E6280.unk_1BC[10].unk_0C.f.b13 = 1')
out=Path('build/scratch/t15500');out.mkdir(exist_ok=True);(out/'body.c').write_text(b)
text=autotweak.rewrite_guard(bulk.insert_draft(Path(src).read_text(),f,b,[]),f,b)
(out/'candidate.c').write_text(text);c=funcloc.locate(f,src);e,err=funcdiff.expected_object(c);assert not err,err
obj=order_search.compile_variant(c,text,out/'work',False);want=funcdiff.resolved_functions(e,[f],c)[f];got=funcdiff.resolved_functions(obj,[f])[f];diff=funcdiff.diff_lines(want,got);(out/'diff.txt').write_text('\n'.join(diff));print('equal',funcdiff.keys(want)==funcdiff.keys(got),'rows',len(diff))
