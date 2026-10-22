from pathlib import Path
import sys
sys.path.insert(0, 'tools')
import funcdiff as f
import funcdiff_verify as v
name='func_8006D138'
c=f.locate(name,'main')
expected, problem=f.expected_object(c)
assert problem is None,problem
out=Path('build/t25040-root-replay')
base=Path(c.src).read_text()
marker='INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006D138);'
assert base.count(marker)==1
src=out/'full.c'
src.write_text(base.replace(marker,Path('drafts/T-25040/snapshot-near.c').read_text()))
obj,_=v.compile_unit(c,out,source=src)
a=f.resolved_functions(expected,[name],c)[name]
b=f.resolved_functions(obj,[name],c)[name]
print('target',len(a),'candidate',len(b),'cost',f.diff_score(a,b))
print('\n'.join(f.diff_lines(a,b)))
