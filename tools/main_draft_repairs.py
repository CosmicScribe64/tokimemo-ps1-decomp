"""T-16000: full-TU main draft repair compiler; run only in project Docker."""
import hashlib,json,shutil,subprocess,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import autotweak,bulk_m2c as bulk,funcloc,funcdiff,order_search,funcdiff_verify as proof
NAMES=['func_800476C0','func_80052F68','get_near_menu','func_80054E6C','func_80056BA8','func_8005CC40','func_80062948','func_8006B0C8','parameter_up_down','func_80077900','func_8007A50C','func_8007BBE8','dialogue_render']
BASE=Path('drafts/T-16000')
LOG=[]
rows={r['func']:r for r in json.loads(Path('drafts/T-14500/inventory.json').read_text()) if r['unit']=='main' and r['func'] in NAMES}
def command(args):
 p=subprocess.run(args,capture_output=True,text=True)
 LOG.append('$ '+' '.join(args)+'\n'+('Preprocessed C retained in full-context.c\n' if args[0]=='gcc' and '-E' in args else p.stdout)+p.stderr)
 if p.returncode: raise ValueError(p.stdout+p.stderr)
 return p.stdout
proof.command=command
for name in sys.argv[1:] or NAMES:
 r=rows[name];out=BASE/name;out.mkdir(parents=True,exist_ok=True);seed=Path('drafts/T-14500/main')/name
 if not (out/'body.c').exists():
  (out/'body.c').write_text((seed/'body.c').read_text())
  (out/'declarations.h').write_text((seed/'declarations.h').read_text() if (seed/'declarations.h').exists() else '')
  (out/'baseline-source.c').write_text(Path(r['src']).read_text())
 b=(out/'body.c').read_text();decl=(out/'declarations.h').read_text().splitlines();original=(out/'baseline-source.c').read_text()
 preamble=(out/'preamble.c').read_text() if (out/'preamble.c').exists() else ''
 text=preamble+autotweak.rewrite_guard(bulk.insert_draft(original,name,b,decl),name,b);LOG=[];(out/'context.c').write_text(text)
 result=dict(r,status='failed',score=None,ticket='T-16000',matching_claim=False,path=str(out))
 try:
  assert b.strip(),'empty generated body; reconstruct from preserved raw m2c/target'
  c=funcloc.locate(name,r['src']);obj=order_search.compile_variant(c,text,out/'work',False)
  expected,err=funcdiff.expected_object(c);assert not err,err
  want=funcdiff.resolved_functions(expected,[name],c)[name];got=funcdiff.resolved_functions(obj,[name])[name]
  diff=funcdiff.diff_lines(want,got);score=0 if funcdiff.keys(want)==funcdiff.keys(got) else len(diff)
  result.update(status='candidate-zero' if score==0 else 'scored',score=score,compile_verified=True)
  (out/'diagnostics.txt').write_text(('\n'.join(LOG)+'\nFull translation unit compiled with IDO 5.3. Score '+str(score)).rstrip()+'\n')
  (out/'difference.txt').write_text('\n'.join(diff)+'\n')
 except BaseException as e:
  result['reason']=str(e);(out/'diagnostics.txt').write_text(('\n'.join(LOG)+'\n'+str(e)).rstrip()+'\n')
 pre=out/'work'/'guarded.c'
 if pre.exists():shutil.copyfile(pre,out/'full-context.c')
 shutil.rmtree(out/'work',ignore_errors=True)
 result.update(archive_path=str(out),provenance='Frozen T-14500 initial MAIN census with T-16000 layout/type repair; see README.md', compiler='IDO 5.3, project Docker, full translation unit')
 for file in ['body.c','declarations.h','context.c','baseline-source.c','full-context.c','diagnostics.txt','preamble.c']:
  if not (out/file).exists():continue
  result[file+'_sha256']=hashlib.sha256((out/file).read_bytes()).hexdigest()
 (out/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(name,result['status'],result['score'],flush=True)
(BASE/'catalog.jsonl').write_text(''.join(json.dumps(json.loads(p.read_text()))+'\n' for p in sorted(BASE.glob('*/result.json'))))
