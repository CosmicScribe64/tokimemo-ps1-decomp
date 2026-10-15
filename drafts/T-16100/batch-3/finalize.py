"""Normalize batch-3 review metadata and concise evidence after serial scoring."""
import hashlib,json,re
from pathlib import Path
base=Path('drafts/T-16100/batch-3');rows=json.loads(Path('drafts/T-16100/batch-3.json').read_text())
notes={3:'Byte candidate list and table tail reconstructed. Remaining GameState* record walk increments still need conversion to Rec38 before production use.',8:'Thirteen-word table aliases and indexed stack reads reconstructed. Remaining GameState* record walk increments remain unresolved.',11:'Both table extents are eleven words; byte tail offsets repaired. Opaque block-copy typedefs remain provisional.',14:'The initializer and all byte aliases now share one u8[21] array, using target lbu offsets.',17:'Halfword table extents and byte index additions reconstructed. Independent global pointer strides and coordinate packed-word views remain outside this repair.',19:'Three u8[64] string buffers reconstructed from zero-fill bounds. sp108 result buffer extent remains unresolved pending callee output-capacity evidence.',21:'Three copied words and a three-iteration word-indexed loop justify s32[3].',22:'Five copied words plus a halfword and target lhu index*2 justify u16[11].',23:'Stack initializers reconstructed as s32[2], s32[18], and s32[5]. Existing GameState* +4 walks still scale incorrectly and remain unresolved.',28:'Three copied 20-byte tables read by lh/index*2 justify s16[10]. Raw 0x44 copy type remains provisional.',29:'Three copied 20-byte tables read by lh/index*2 justify s16[10]. Raw 0x44 copy type remains provisional.',30:'Two copied 16-byte tables read by lh/index*2 justify s16[8]. Raw 0x44 copy type remains provisional.',37:'Sixteen-byte initializer and target lh with index stepping two justify s16[8]; byte-base addition prevents pointer-to-array scaling.',42:'Copied tables are s16[11] and s16[12]. Separate sp1E remains an uninitialized alias of sp1C[1], requiring a follow-up.',43:'sp40[7] has exactly fourteen initialized bytes (three words plus final halfword), indexed by a loop bounded by seven; this flag is justified. Adjacent sp30 and possible boundary index remain unresolved.',4:'sp58[16] is exactly a 64-byte copy, indexed via sll index,2 and lw (8013D674..8013D67C); this synthesized-array flag is justified. GameState byte-base arithmetic repaired. String buffer spAC[1] still needs reconstruction.'}
repaired_scopes={0,1,3,5,7,8,9,10,11,12,13,14,17,21,22,28,29,30,31,32,35,37,44,45,47,49,51,52}
for i,row in enumerate(rows):
 unit,name=row['scope'].split(':');out=base/unit/name
 result=json.loads((out/'result.json').read_text());review=json.loads((out/'review.json').read_text())
 if i in notes:review['notes']=notes[i]
 review['review_basis']='Read selected draft and target stack/copy/access instructions; checked aggregate width against current main_api.h and game-state wiki. Full TU uses frozen census header closure.'
 for flag in review['flags']:
  kind=flag['kind'];flag['outcome']='unresolved'
  if kind=='gamestate-address-arithmetic':
   flag['outcome']='repaired';flag['explanation']='Explicit u8* base makes addition use target byte offsets instead of sizeof(GameState). '+review.get('notes','')
  elif kind=='synthesized-stack-local' and ((i==4 and flag['detail']=='sp58[16]') or (i==43 and flag['detail']=='sp40[7]')):
   flag['outcome']='justified-false-positive';flag['explanation']=review['notes']
  elif kind in ['stack-address-arithmetic','synthesized-stack-local'] and i in repaired_scopes:
   flag['outcome']='repaired';flag['explanation']=' '.join(review['repair_notes'])+' '+review.get('notes','')
  elif kind=='inferred-copy-layout':
   flag['explanation']='Opaque word-block type has only a copy-extent interpretation; semantic field layout remains unresolved. '+review.get('notes','')
  elif i in notes:flag['explanation']=notes[i]
 review['outcome']='partial-repair' if review['changed'] and any(f['outcome']=='unresolved' for f in review['flags']) else ('repaired' if review['changed'] else 'reviewed-unresolved')
 if not review['changed'] and all(f['outcome']=='justified-false-positive' for f in review['flags']):review['outcome']='justified-false-positive'
 # Keep sparse target evidence, no binary opcode words or wholesale generated assembly.
 ev=(out/'assembly-evidence.txt').read_text().splitlines();ev=[re.sub(r'/\* \S+ (\S+) \S+ \*/',r'\1:',line).strip() for line in ev];ev=ev[:20]+[l for l in ev[20:] if re.search(r'\b(?:lh|lhu|lb|lbu|sll)\b|\$sp, 0x',l)][:20]
 (out/'assembly-evidence.txt').write_text('\n'.join(ev)+'\n')
 d=out/'difference.txt'
 if d.exists():
  lines=[re.sub(r'^([+-]@row \d+) [0-9a-fA-F]{8}\s*;',r'\1 ;',l) for l in d.read_text().splitlines()];d.write_text('\n'.join(lines[:80])+('\n[Excerpt limited to 80 differing rows; score retains complete resolved diff count.]' if len(lines)>80 else '')+'\n')
 result['initial_score']=next((a['score'] for a in row['alternatives'] if a['path']==row['selected_path']),None)
 result['artifact_sha256']={p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in out.iterdir() if p.is_file() and p.name not in ['result.json','review.json']}
 (out/'result.json').write_text(json.dumps(result,indent=2)+'\n');(out/'review.json').write_text(json.dumps(review,indent=2)+'\n')
results=[json.loads(p.read_text()) for p in sorted(base.glob('*/*/result.json'))];reviews=[json.loads(p.read_text()) for p in sorted(base.glob('*/*/review.json'))]
(base/'catalog.jsonl').write_text(''.join(json.dumps(r,sort_keys=True)+'\n' for r in results if r['status'] in ['scored','candidate-zero']))
(base/'review.jsonl').write_text(''.join(json.dumps(r,sort_keys=True)+'\n' for r in reviews))
print('Reviewed',len(reviews),'Compiled',sum(r['status'] in ['scored','candidate-zero'] for r in results),'Changed',sum(r['changed'] for r in reviews),'Failed',[r['scope'] for r in results if r['status']=='failed'])
