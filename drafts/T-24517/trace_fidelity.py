"""Attribute the exact structural checkpoint with stock/traced fidelity first. Docker only."""
from pathlib import Path
from types import SimpleNamespace
import sys,os,subprocess,json,time
sys.path.insert(0,'tools')
import regdiff,funcdiff,trailing_pad
name='func_801363F4';ctx=funcdiff.locate(name,'MASTER');w=Path('build/t24517/tracer');w.mkdir(parents=True,exist_ok=True)
src='build/t24517/checkpoint/variant.c';t=time.time()
c=regdiff.compile_draft(os.getcwd(),name,src,'/scratch/variants/trace',str(w/'traced'))
if c.returncode:raise SystemExit(c.log[-4000:])
stock=w/'stock.o'
subprocess.run([sys.executable,'tools/uopt_eval.py','compile',c.base,str(stock),'--uopt','/opt/ido/5.3/uopt'],check=True,stdout=(w/'stock.log').open('w'),stderr=subprocess.STDOUT)
def sections(p):
 return {s['sname']:bytes(s['data']) for s in trailing_pad.Elf(Path(p).read_bytes()).sections if s['sname'] in ('.text','.rel.text','.rodata','.rel.rodata','.data','.rel.data')}
assert sections(stock)==sections(c.obj),'stock/traced exact sections differ'
ref=funcdiff.resolved_functions('build/t24517/checkpoint/guarded.o',[name],ctx)[name]
got=regdiff.resolved(c.obj,name)
assert funcdiff.keys(ref)==funcdiff.keys(got),'trace target differs from exact checkpoint'
(w/'fidelity.json').write_text(json.dumps(dict(exact_sections_equal=True,target_checkpoint_equal=True,sections=sorted(sections(stock)),production_compiler_changed=False),indent=2)+'\n')
e,_=funcdiff.expected_object(ctx);want=regdiff.resolved(e,name,ctx)
args=SimpleNamespace(src=src,verbose=False)
with (w/'report.txt').open('w') as out:
 regdiff.report(args,name,ctx,src,c,got,want,(t,t,time.time()),lambda x:out.write(x+'\n'))
print((w/'fidelity.json').read_text());print('\n'.join((w/'report.txt').read_text().splitlines()[:28]))
