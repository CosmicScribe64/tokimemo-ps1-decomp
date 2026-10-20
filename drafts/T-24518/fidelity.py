"""Check stock/traced uopt fidelity on this exact private ETC translation unit."""
from pathlib import Path
import subprocess
import sys
sys.path.insert(0, 'tools')
import funcdiff
import object_layout
import trailing_pad
import funcloc

stem=sys.argv[1] if len(sys.argv)>1 else 'shared-signed'
scope=sys.argv[2] if len(sys.argv)>2 else 'ETC:func_8013FFAC'
unit,fn=scope.split(':')
context=funcloc.locate(fn,unit)
source=Path('build/T-24518')/stem/'guarded.c'
reference=Path('build/T-24518')/stem/'guarded.o'
out=Path('build/T-24518')/('fidelity-'+stem);out.mkdir(parents=True,exist_ok=True)
objects=[]
for label,binary,trace in [('stock','/opt/ido/5.3/uopt',False),('trace','/scratch/variants/trace',True)]:
 obj=out/(label+'.o')
 cmd=['python3','tools/uopt_eval.py','compile',str(source),str(obj),'--uopt',binary]
 if trace:cmd.append('--trace')
 with (out/(label+'.log')).open('w') as log:subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
 includes={name for _,name in __import__('cc').INCLUDE_ASM_RE.findall(source.read_text())}
 trailing_pad.pad_functions(str(obj),str(context.src),includes)
 obj.write_bytes(object_layout.trim(obj.read_bytes(),object_layout.source_extent(str(context.src))))
 objects.append(obj)
def sections(path):
 wanted={'.text','.rel.text','.rodata','.rel.rodata','.data','.rel.data'}
 return {s['sname']:bytes(s['data']) for s in trailing_pad.Elf(path.read_bytes()).sections if s['sname'] in wanted}
assert sections(objects[0])==sections(objects[1]),'stock/traced section bytes differ'
want=funcdiff.resolved_functions(str(reference),[fn])[fn]
for obj in objects:
 got=funcdiff.resolved_functions(str(obj),[fn])[fn]
 assert funcdiff.keys(want)==funcdiff.keys(got),'diagnostic target differs from exact stock candidate'
print('Exact candidate fidelity: stock/traced code, relocations, data and target words agree')
