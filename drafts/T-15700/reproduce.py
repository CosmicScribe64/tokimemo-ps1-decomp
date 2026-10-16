#!/usr/bin/env python3
"""T-15700 serial full-TU draft repairs; run in Docker with immutable census at /ref."""
import hashlib,json,re,shutil,subprocess,sys
from pathlib import Path
sys.path.insert(0,'tools')
import autotweak,bulk_m2c as bulk,funcloc,funcdiff,funcdiff_verify as proof,order_search
BASE=Path('drafts/T-15700');ROWS=json.loads((BASE/'reservation.json').read_text())
LOG=[]
def command(args):
    r=subprocess.run(args,capture_output=True,text=True);LOG.append('$ '+' '.join(args)+'\n'+('[Preprocessed C retained in full-context.c]\n' if args[0]=='gcc' and '-E' in args else r.stdout)+r.stderr)
    if r.returncode:raise ValueError(r.stdout+r.stderr)
    return r.stdout
proof.command=command

def process(row):
    global LOG
    unit,func=row['scope'].split(':');folder=BASE/unit/func;folder.mkdir(parents=True,exist_ok=True)
    old=Path('/ref/drafts/T-14500')/unit/func;original=Path(row['src']).read_text()
    if func=='func_80154960':
        original=original.replace('void func_80156178(void)', 's32 func_80156178(void)').replace('    func_8015ACCC(6, 6, D_801604D4, D_801604D4, pos, 0xFF, 0xF0);', '    return func_8015ACCC(6, 6, D_801604D4, D_801604D4, pos, 0xFF, 0xF0);')
    headers=bulk.header_text('.',unit)
    body=(folder/'body.c').read_text() if (folder/'body.c').exists() else (old/'body.c').read_text()
    decl=(folder/'declarations.h').read_text().splitlines() if (folder/'declarations.h').exists() else ((old/'declarations.h').read_text().splitlines() if (old/'declarations.h').exists() else [])
    decl=[d for d in decl if d.strip()!='extern s32 sp;' or __import__('re').search(r'\bsp\b',body)]
    result=dict(scope=row['scope'],src=row['src'],size=row['size'],status='failed',score=None,matching_claim=False,archive_path=str(folder),source_base='48362009',ticket='T-15700');LOG=[]
    try:
        c=funcloc.locate(func,row['src']);expected,err=funcdiff.expected_object(c)
        if err:raise ValueError(err)
        want=funcdiff.resolved_functions(expected,[func],c)[func]
        for attempt in range(10):
            placed=bulk.insert_draft(original,func,body,decl);text=autotweak.rewrite_guard(placed,func,body)
            (folder/'body.c').write_text(body);(folder/'declarations.h').write_text('\n'.join(decl)+'\n');(folder/'context.c').write_text(text.rstrip()+'\n')
            try:
                obj=order_search.compile_variant(c,text,folder/'work',False);got=funcdiff.resolved_functions(obj,[func])[func];diff=funcdiff.diff_lines(want,got);score=0 if funcdiff.keys(want)==funcdiff.keys(got) else len(diff)
                result.update(status='candidate-zero' if score==0 else 'scored',score=score,compile_attempts=attempt+1);(folder/'comparison.txt').write_text('\n'.join(diff)+'\n');break
            except (SystemExit,ValueError,OSError) as exc:
                msg=str(exc);fixed=bulk.cheap_fix(msg,decl,{})
                if fixed is not None and fixed!=decl:decl=fixed;continue
                # Only signature repairs; undefined stack aliases need actual reconstruction.
                more=None if re.search(r"'sp\w*' undefined",msg) else bulk.source_fix(msg,body,decl,func,original,headers)
                if more is None and "redeclaration of '%s'"%func in msg:
                    typ=bulk.prototype_in(headers,func);new=bulk.with_return_type(body,func,typ[0] if typ else 's32')
                    if new!=body:more=new,decl
                if more:body,decl=more;continue
                result['reason']=next((s.strip() for s in msg.splitlines() if 'Error:' in s),'compiler failure; see full diagnostics');break
    except BaseException as exc:result['reason']=str(exc)
    pre=folder/'work'/'guarded.c'
    if pre.exists():shutil.copyfile(pre,folder/'full-context.c')
    shutil.rmtree(folder/'work',ignore_errors=True)
    (folder/'diagnostics.txt').write_text('\n\n'.join(LOG)+'\n')
    for name in ['body.c','context.c','declarations.h']:
        if (folder/name).exists():result[name.replace('.','_')+'_sha256']=hashlib.sha256((folder/name).read_bytes()).hexdigest()
    (folder/'result.json').write_text(json.dumps(result,indent=2)+'\n');(folder/'provenance.json').write_text(json.dumps(dict(original_attempt='drafts/T-14500/'+unit+'/'+func,prior_attempts=row['attempts'],source_base='48362009',repair_base='a445c4e2'),indent=2)+'\n')
    print(row['scope'],result['status'],result['score'],result.get('reason',''),flush=True)
for row in ROWS:
    if len(sys.argv)>1 and row['scope'] not in sys.argv[1:]:continue
    process(row)
results=[json.loads(p.read_text()) for p in sorted(BASE.glob('*/*/result.json'))];(BASE/'catalog.jsonl').write_text(''.join(json.dumps(r,sort_keys=True)+'\n' for r in results))
