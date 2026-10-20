"""T-24501/24570: replay one diagnostic record-assignment hypothesis in Docker.
No production edit or literal-match claim. Outputs stay under build/scratch.
"""
import queue,sys,tempfile
from pathlib import Path
sys.path.insert(0,'tools')
import funcdiff as f,funcdiff_verify as p
name='func_80054E6C';c=f.locate(name,'main');expected,_=f.expected_object(c);want=f.resolved_functions(expected,[name])[name]
base=Path(c.src).read_text();a=base.index('void '+name);z=base.index('\n}',a)+2;body=base[a:z]
old='*(s32 *)&D_800E7D10.metrics[i][slot] = (&D_800E6280.unk_0F8)[i + 1];'
new='*(GsRec0FC *)&D_800E7D10.metrics[i][slot] =\n            ((GsRec0FC *)((u8 *)&D_800E6280 + 0xFC))[i];'
assert old in body
for tag,trial in [('baseline',body),('record_assignment',body.replace(old,new))]:
    Path('build/scratch').mkdir(parents=True,exist_ok=True)
    folder=Path(tempfile.mkdtemp(prefix='metric-'+tag+'-',dir='build/scratch'))
    src=folder/'variant.c';src.write_text(base.replace(body,trial));(folder/'body.c').write_text(trial)
    obj,_=p.compile_unit(c,folder,source=src);got=f.resolved_functions(obj,[name])[name]
    (folder/'resolved.diff').write_text('\n'.join(f.diff_lines(want,got)))
    (folder/'got.txt').write_text('\n'.join(r[1] for r in got))
    print(tag,'cost',f.diff_score(want,got),'words',len(got),'frame',[r[1] for r in got if 'addiu sp,sp,-' in r[1]],flush=True)
