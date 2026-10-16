#!/usr/bin/env python3
"""Replay frozen T15900 source alternatives through default full-TU IDO compilation.
Run in Docker from the original48362009 checkout/header baseline.
Source bodies and contexts are immutable inputs; no production files are edited.
"""
import hashlib,json,shutil,subprocess,sys
from pathlib import Path
sys.path.insert(0,'tools')
import funcloc,funcdiff,order_search,funcdiff_verify
ROOT=Path('drafts/T-14500/repairs/t15900')
logs=[]
def command(args):
    p=subprocess.run(args,capture_output=True,text=True)
    logs.append('$ '+' '.join(args)+'\n'+p.stdout+p.stderr)
    if p.returncode:
        raise ValueError(p.stdout+p.stderr)
    return p.stdout
funcdiff_verify.command=command
rows=[]
for folder in sorted(ROOT.glob('*/*')):
    if not (folder/'result.json').exists():continue
    logs.clear()
    r=json.loads((folder/'result.json').read_text())
    c=funcloc.locate(r['func'],r['src'])
    text=(folder/'context.c').read_text()
    obj=order_search.compile_variant(c,text,folder/'work',False)
    shutil.copyfile(folder/'work/guarded.c',folder/'full-context.c')
    expected,problem=funcdiff.expected_object(c)
    assert not problem,problem
    want=funcdiff.resolved_functions(expected,[r['func']],c)[r['func']]
    got=funcdiff.resolved_functions(obj,[r['func']])[r['func']]
    score=0 if funcdiff.keys(want)==funcdiff.keys(got) else len(funcdiff.diff_lines(want,got))
    assert score==r['score'],(r['scope'],r['score'],score)
    (folder/'diagnostics.txt').write_text('\n'.join(logs))
    r['preprocessed_context_hash']=hashlib.sha256((folder/'full-context.c').read_bytes()).hexdigest()
    (folder/'result.json').write_text(json.dumps(r,indent=2)+'\n')
    rows.append(r)
    shutil.rmtree(folder/'work')
    print(r['scope'],score,flush=True)
(ROOT/'catalog.jsonl').write_text(''.join(json.dumps(r,sort_keys=True)+'\n' for r in rows))
assert len(rows)==13
