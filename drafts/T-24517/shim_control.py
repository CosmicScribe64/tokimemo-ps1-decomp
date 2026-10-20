"""One-process control attributing sentinel influence; production tools unchanged. Docker only."""
from pathlib import Path
import sys,json
sys.path.insert(0,'tools')
import cc,funcdiff,trailing_pad,object_layout
name='func_801363F4';c=funcdiff.locate(name,'MASTER');s=Path('build/t24517/checkpoint/guarded.c')
w=Path('build/t24517/sentinel-control');w.mkdir(parents=True,exist_ok=True);out=w/'without-ugen-shim.o'
cc.SHIMS.pop('ugen')
cc.main(['cc.py',str(s),str(out),'ido','5.3'])
includes={n for _,n in cc.INCLUDE_ASM_RE.findall(s.read_text())};trailing_pad.pad_functions(str(out),str(c.src),includes);out.write_bytes(object_layout.trim(out.read_bytes(),object_layout.source_extent(c.src)))
e,_=funcdiff.expected_object(c);want=funcdiff.resolved_functions(e,[name],c)[name];current=funcdiff.resolved_functions('build/t24517/checkpoint/guarded.o',[name],c)[name];actual=funcdiff.resolved_functions(str(out),[name],c)[name]
result=dict(control='omit ugen shim only in this process',target_rows=len(actual),current_score=funcdiff.diff_score(want,current),control_score=funcdiff.diff_score(want,actual),changed_rows=[i for i,(a,b) in enumerate(zip(current,actual)) if a[0]!=b[0]],production_changed=False)
def sections(path):
 return {s['sname']:bytes(s['data']) for s in trailing_pad.Elf(Path(path).read_bytes()).sections if s['sname'] in ('.text','.rel.text','.rodata','.rel.rodata','.data','.rel.data')}
result['exact_sections_equal']=sections('build/t24517/checkpoint/guarded.o')==sections(out)
assert result['exact_sections_equal'],'enabled/omitted-shim sections differ'
(w/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result),flush=True)
