"""T-25051 full-TU source-only probe harness; run from repository root in Docker."""
import sys,pathlib,re
sys.path.insert(0,'/work/tools')
import cc,funcdiff as fd,funcdiff_verify as fv
p=pathlib.Path('build/t25051');p.mkdir(exist_ok=True)
base=fv.preprocess_draft('src/ovl/TACO/8013B170.c');m=re.search(r's32 func_8013B460\([^;]*?\)\s*\{',base);start=m.start();depth=0;end=None
for j in range(base.index('{',start),len(base)):
 if base[j]=='{':depth+=1
 if base[j]=='}':
  depth-=1
  if depth==0:end=j+1;break
body=base[start:end];pre,post=base[:start],base[end:]
w=fd.resolved_functions('expected/build/ovl/TACO/src/ovl/TACO/8013B170.o',['func_8013B460'])['func_8013B460']
def run(name,b):
 src=p/(name+'.c');obj=p/(name+'.o');src.write_text(pre+b+post);cc.compile_ido(str(src),str(obj),'5.3');got=fd.resolved_functions(str(obj),['func_8013B460'])['func_8013B460'];(p/(name+'.diff')).write_text('\n'.join(fd.diff_lines(w,got)));print(name,len(got),fd.diff_score(w,got),flush=True)
uv='p->u0 = 0, p->v0 = 0, p->u1 = 0xFF, p->v1 = 0, p->u2 = 0, p->v2 = 0xF0, p->u3 = 0xFF, p->v3 = 0xF0;'
