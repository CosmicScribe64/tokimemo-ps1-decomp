"""Measure dispatcher reload hypotheses in a copied TU, Docker only."""
from pathlib import Path
import sys,json
sys.path.insert(0,'tools')
import funcdiff as fd,order_search
name='func_8007132C';c=fd.locate(name,'main');base=Path(c.src).read_text()
needle=next(l for l in base.splitlines() if 'INCLUDE_ASM' in l and name in l)
body=Path('drafts/T-23003/main/func_8007132C/hand.c').read_text()
variants={'baseline':body}
for mode in ['field','offset','alternating']:
 s=body
 for field in ['03E','03F','040']:
  old='D_800E6280.unk_'+field
  if mode=='field':s=s.replace(old,'(*(u8 *)&'+old+')')
  elif mode=='offset':s=s.replace(old,'(*(u8 *)((u8 *)&D_800E6280 + 0x'+field+'))')
  else:
   parts=s.split(old);s=parts[0]+''.join((old if i%2==0 else '(*(u8 *)&'+old+')')+x for i,x in enumerate(parts[1:]))
 variants[mode]=s
root=Path('build/t24024');root.mkdir(parents=True,exist_ok=True)
e,problem=fd.expected_object(c)
if problem:raise RuntimeError(problem)
want=fd.resolved_functions(e,[name],c)[name]
for label,s in variants.items():
 out=root/label;obj=order_search.compile_variant(c,base.replace(needle,s),out,False)
 got=fd.resolved_functions(obj,[name])[name]
 record={'variant':label,'aligned_cost':fd.diff_score(want,got),'printed_rows':len(fd.diff_lines(want,got)),'expected_instructions':len(want),'draft_instructions':len(got)}
 (out/'body.c').write_text(s)
 with (root/'results.jsonl').open('a') as f:f.write(json.dumps(record)+'\n')
 print(json.dumps(record),flush=True)
