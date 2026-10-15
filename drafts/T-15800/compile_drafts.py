#!/usr/bin/env python3
"""Compile isolated T-15800 full-TU alternatives serially with frozen source/header context."""
from pathlib import Path
import argparse,hashlib,json,os,re,shutil,subprocess,sys,time
sys.path.insert(0,'/work/tools')
import funcloc,funcdiff,order_search,permute,funcdiff_verify
OUT=Path('/work/drafts/T-15800');ROOT=Path('/work/build/t15800/root');FROZEN=Path('/scratch/drafts-14500');BASE=FROZEN/'drafts/T-14500'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def catalog():
 rows=[]
 for r in json.loads((OUT/'inventory.json').read_text()):
  unit,func=r['scope'].split(':');p=OUT/unit/func/'result.json'
  if p.exists():rows.append(json.loads(p.read_text()))
 (OUT/'catalog.jsonl').write_text(''.join(json.dumps(r,sort_keys=True)+'\n' for r in rows))
def init(row):
 unit,func=row['scope'].split(':');folder=OUT/unit/func;folder.mkdir(parents=True,exist_ok=True);seed=FROZEN/row['selected_path']
 for name in ['body.c','declarations.h','context.c']:
  dest=folder/name
  if not dest.exists() and (seed/name).exists():shutil.copyfile(seed/name,dest)
 if not (folder/'provenance.json').exists():
  (folder/'provenance.json').write_text(json.dumps({'selected_seed':row['selected_path'],'prior_attempts':row['attempts'],'frozen_commit':'4836200960c04659c0705aa566dc4da3b5749e7a','production_src':row['src']},indent=2)+'\n')
 return folder

def run(row):
 folder=init(row);unit,func=row['scope'].split(':');attempts=folder/'attempts';attempts.mkdir(exist_ok=True);n=len(list(attempts.iterdir()))+1;attempt=attempts/('%03d'%n);attempt.mkdir();work=folder/'work';shutil.rmtree(work,ignore_errors=True)
 for name in ['body.c','declarations.h','context.c']:
  if (folder/name).exists():shutil.copyfile(folder/name,attempt/name)
 result={'scope':row['scope'],'unit':unit,'func':func,'src':row['src'],'size':row['size'],'path':str(folder.relative_to(Path('/work'))),'status':'failed','score':None,'compile_attempts':n,'provenance':str((folder/'provenance.json').relative_to(Path('/work'))),'header_snapshot':'drafts/T-14500/baseline/include'}
 messages=[];started=time.monotonic()
 def complete_command(args):
  p=subprocess.run(args,capture_output=True,text=True)
  if p.stderr:messages.append('COMMAND '+repr(args)+'\n'+p.stderr)
  if p.returncode:raise ValueError('command failed: '+repr(args)+'\n'+p.stdout+'\n'+p.stderr)
  return p.stdout
 funcdiff_verify.command=complete_command
 try:
  text=(folder/'context.c').read_text();rng=permute.function_range(text,func)
  if rng is None:raise ValueError('target C definition absent from selected context')
  if re.search(r'INCLUDE_ASM\([^;\n]*\b'+re.escape(func)+r'\b',text):raise ValueError('target still has ASM fallback; not counted as compiling C')
  c=funcloc.locate(func,row['src']);obj=order_search.compile_variant(c,text,work,False)
  got=funcdiff.resolved_functions(obj,[func])[func]
  if got is None:raise ValueError('compiled target function symbol absent')
  result['status']='compiled';result['compiled_instruction_count']=len(got)
  try:
   expected,problem=funcdiff.expected_object(c)
   if problem:raise ValueError(problem)
   want=funcdiff.resolved_functions(expected,[func],c)[func];result['score']=0 if funcdiff.keys(got)==funcdiff.keys(want) else len(funcdiff.diff_lines(want,got))
  except (SystemExit,ValueError,OSError) as err:messages.append('Score unavailable: '+str(err));result['score_note']=str(err)
 except (SystemExit,ValueError,OSError,RuntimeError) as err:
  messages.append(str(err));result['reason']=next((l for l in str(err).splitlines() if 'Error:' in l or 'error:' in l),str(err).splitlines()[0] if str(err) else type(err).__name__)
 message='\n'.join(messages)
 (folder/'diagnostics.txt').write_text(message+'\n');(attempt/'diagnostics.txt').write_text(message+'\n')
 pre=work/'guarded.c'
 if pre.exists():
  shutil.copyfile(pre,folder/'preprocessed.c')
  lines=pre.read_text(errors='replace').splitlines();snips=[]
  for num in sorted(set(int(m) for m in re.findall(r'line (\d+)',message))):
   snips.extend('%d: %s'%(i+1,lines[i]) for i in range(max(0,num-4),min(len(lines),num+3)))
  (folder/'error-context.txt').write_text('\n'.join(snips)+'\n')
 result['hashes']={name:sha(folder/name) for name in ['body.c','declarations.h','context.c','preprocessed.c'] if (folder/name).exists()};result['header_hashes']={str(p):sha(p) for p in sorted(funcloc.includes_of(row['src']))};result['frozen_source_sha256']=sha(ROOT/row['src']);result['compiler_script_sha256']=sha(ROOT/'tools/cc.py');result['dockerfile_sha256']=sha(ROOT/'tools/Dockerfile');result['seconds']=round(time.monotonic()-started,3)
 (folder/'result.json').write_text(json.dumps(result,indent=2)+'\n');(attempt/'result.json').write_text(json.dumps(result,indent=2)+'\n');shutil.rmtree(work,ignore_errors=True);catalog();print(row['scope'],result['status'],result['score'],result.get('reason',''),flush=True)
 return result

def main():
 p=argparse.ArgumentParser();p.add_argument('scopes',nargs='*');p.add_argument('--all',action='store_true');p.add_argument('--init-only',action='store_true');a=p.parse_args();os.chdir(ROOT)
 rows=json.loads((OUT/'inventory.json').read_text())
 for row in rows:
  if a.scopes and row['scope'] not in a.scopes:continue
  folder=init(row)
  if a.init_only:continue
  if a.all and (folder/'result.json').exists() and json.loads((folder/'result.json').read_text())['status']=='compiled':continue
  run(row)
if __name__=='__main__':main()
