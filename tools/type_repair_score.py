"""T-14700 serial full-TU compile and resolved scoring of archived type repairs."""
import json,sys,hashlib,shutil,re
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import subprocess, funcdiff_verify as proof
def full_command(args):
    result=subprocess.run(args,capture_output=True,text=True)
    if result.returncode: raise ValueError(result.stdout+result.stderr)
    return result.stdout
proof.command=full_command
import bulk_m2c as bulk, autotweak, funcloc, funcdiff, order_search
base=Path('drafts/T-14500/repairs/types')
rows=json.loads((base/'inventory.json').read_text())
for row in rows:
    if len(sys.argv)>1 and row['func'] not in sys.argv[1:]: continue
    d=base/row['unit']/row['func'];body=(d/'body.c').read_text();decl=(d/'declarations.h').read_text().splitlines()
    original=Path(row['src']).read_text()
    (d/'baseline-source.c').write_text(original)
    (d/'baseline-headers.h').write_text(bulk.header_text('.',row['unit']))
    text=autotweak.rewrite_guard(bulk.insert_draft(original,row['func'],body,decl),row['func'],body)
    (d/'context.c').write_text(text)
    c=funcloc.locate(row['func'],row['src'])
    result=dict(row,status='failed',score=None,base_commit='4836200960c04659c0705aa566dc4da3b5749e7a',ticket='T-14700',production_changed=False)
    try:
        obj=order_search.compile_variant(c,text,d/'work',False)
        expected,problem=funcdiff.expected_object(c)
        if problem:raise RuntimeError(problem)
        want=funcdiff.resolved_functions(expected,[row['func']],c)[row['func']]
        got=funcdiff.resolved_functions(obj,[row['func']])[row['func']]
        score=0 if funcdiff.keys(want)==funcdiff.keys(got) else len(funcdiff.diff_lines(want,got))
        result.update(status='candidate-zero' if score==0 else 'scored',score=score)
        (d/'diagnostics.txt').write_text('Full-TU compile successful. Default resolved score: %d\n'%score)
        (d/'difference.txt').write_text('\n'.join(funcdiff.diff_lines(want,got))+'\n')
    except BaseException as e:
        result['reason']=str(e);(d/'diagnostics.txt').write_text(str(e)+'\n')
    for name in ('body.c','declarations.h','context.c'):
        result[name+'_sha256']=hashlib.sha256((d/name).read_bytes()).hexdigest()
    (d/'result.json').write_text(json.dumps(result,indent=2)+'\n')
    shutil.rmtree(d/'work',ignore_errors=True)
    print(row['scope'],result['status'],result.get('score'),flush=True)
