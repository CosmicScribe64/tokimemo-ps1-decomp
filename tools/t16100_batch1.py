"""T-16100 batch 1 evidence review and serial frozen full-TU compiler (Docker only)."""
import hashlib,json,re,shutil,subprocess,sys
from pathlib import Path
sys.path.insert(0,'tools')
import funcloc,funcdiff,funcdiff_verify as proof,order_search,trailing_pad
BASE=Path('drafts/T-16100/batch-1');BASE.mkdir(parents=True,exist_ok=True)
ROWS=json.loads(Path('drafts/T-16100/batch-1.json').read_text())
LOG=[]
def command(args):
 p=subprocess.run(args,capture_output=True,text=True);LOG.append('$ '+' '.join(args)+'\n'+(p.stdout if args[0]!='gcc' else '[preprocessed full TU archived]\n')+p.stderr)
 if p.returncode:raise ValueError(p.stdout+p.stderr)
 return p.stdout
proof.command=command

def replace_definition(text,name,body):
 # Definition only: declarations and caller occurrences cannot consume the body.
 pat=re.compile(r'(?m)^[\w *]+\b'+re.escape(name)+r'\([^;{}]*\)\s*\{')
 m=pat.search(text)
 if not m:raise ValueError('No target definition in archived context')
 depth=1;i=m.end()
 while depth:
  if text[i]=='{':depth+=1
  if text[i]=='}':depth-=1
  i+=1
 return text[:m.start()]+body.strip()+text[i:]

def repair(r,b):
 unit,name=r['scope'].split(':');notes=[]
 # Original addu bases never multiply the index by sizeof(GameState)=0x1a90.
 if '&D_800E6280' in b and any(f['kind']=='gamestate-address-arithmetic' for f in r['flags']):
  b=re.sub(r"&D_800E6280(?![.\w])", "((u8 *)&D_800E6280)", b)
  notes.append('Target addu uses the GameState base plus the explicit byte index (sll/multu are already in that index); cast the base before addition to prevent an erroneous extra sizeof(GameState)=0x1A90 scale. Field recovery remains incomplete where this view spans multiple fields.')
 if name=='get_fnt':
  b=b.replace('s32 sp8C[3];','u8 sp8C[0x100];').replace('s32 *var_s2;','u8 *var_s2;')
  b=b.replace('&sp8C','sp8C')
  notes.append('Both target memcpy calls copy 0x100 bytes into sp+0x8C, requiring 256 bytes, not three words. Byte pointer walking matches row stride arg4.')
 if unit=='GEKO' and name=='func_80144198':
  b=b.replace('s32 sp18[16];','s16 sp18[12];').replace('    s16 sp1A;\n','').replace('sp1A =','sp18[1] =')
  notes.append('Target copies six words (0x18 bytes) to sp+0x18, patches sp+0x1A with sh, then uses month<<1 and lh -2. Twelve s16 entries represent that exact copy and patch, eliminating the detached scalar.')
 if unit=='DATE2' and name=='func_801330D4':
  b=b.replace('s32 sp38[16];','s16 sp38[10];').replace('&sp38[0] + var_t6','&sp38[var_t6 / 2]')
  notes.append('Target array base sp+0x38, sh/lh, indices 1..9 via index<<1 establish ten halfwords. Second loop var_t6 already contains a byte offset, divided by two for typed indexing.')
 if unit=='DATE' and name=='func_80152060':
  b=b.replace('    s32 sp30;\n','').replace('s32 sp2C;','s32 sp2C[2];')
  b=re.sub(r'\bsp30\b','sp2C[1]',b);b=b.replace('sp2C =','sp2C[0] =').replace('func_80046290(sp2C,','func_80046290(sp2C[0],')
  notes.append('Two lw/sw copy words to sp+0x2C/+0x30, passed as a0/a1. Use one two-word local so the second copy store initializes the actual second argument.')
 # Known target stack dispatch tables, previously reached through an invented global sp.
 if (unit,name) in {('DATE','func_80155E40'),('DATE','func_80154118'),('EVENT','func_800FD00C'),('GYOZI','func_8013B0C4')}:
  local={'func_80155E40':'sp2C','func_80154118':'sp28','func_800FD00C':'sp30','func_8013B0C4':'sp34'}[name]
  index = "temp_v0" if unit=="GYOZI" else ("D_800B1AF6" if unit=="EVENT" else "D_800E738A")
  call = "((s32 (*)())%s[%s])();" % (local,index) if unit=="GYOZI" else "((s32 (*)(s32))%s[%s])(0x80);" % (local,index)
  b = "\n".join("    "+call if "sp + (" in line and "s32 (**" in line else line for line in b.split("\n"))
  notes.append('The copied word table is indexed from the target stack base before jalr. Replace invented extern sp addressing with that copied local table. Copy-block type remains a representation of observed word copies, not a recovered semantic struct.')
 if (unit,name) in {('TAIIKU','func_8013FC5C'),('RPG_BAT','func_80139E88')}:
  if unit=='TAIIKU':b=b.replace('*(&sp64 + var_s5)','sp64[var_s5]')
  else:b=b.replace('*(var_s2 + &sp80)','sp80[var_s2]')
  notes.append('Target array load uses one sll index by 2 before lw; indexing the s32 array avoids scaling by the whole array size.')
 if unit=='EVENT' and name=='func_800FF320':
  b=b.replace('&sp50 +','((u8 *)sp50) +')
  notes.append('Target indexes the copied word-pair table by selector<<3. Preserve its byte offset instead of multiplying it again by sizeof the entire 32-byte array.')
 if unit=='NAME_ENT' and name=='func_8013AF78':
  # Three flags initialized by a contiguous pointer walk, read independently later.
  b=re.sub(r'    s32 sp(?:24|20|1C);\n','',b).replace('s32 sp28[1];','s32 flags[3];')
  b=b.replace('&sp1C','&flags[0]').replace('&sp28','&flags[3]')
  for old,new in [('sp1C','flags[0]'),('sp20','flags[1]'),('sp24','flags[2]')]:b=re.sub(r'\b'+old+r'\b',new,b)
  notes.append('Target zeroes three consecutive words at sp+0x1C,20,24 and ends at sp+0x28; one three-word flags array connects the initialization to later scalar reads.')
 return b,notes

base_repair=repair
def repair(r,b):
 b,notes=base_repair(r,b);unit,name=r['scope'].split(':')
 def group(text,low,count,label,typ='s16'):
  names=['sp%X'%(low+i*(2 if typ=='s16' else 4)) for i in range(count)]
  for n in names:
   text=re.sub(r'    s(?:16|32) '+n+r';\n','',text)
  text=text.replace('{\n','{\n    '+typ+' '+label+'['+str(count)+'];\n',1)
  for i,n in enumerate(names):text=re.sub(r'\b'+n+r'\b',label+'['+str(i)+']',text)
  return text
 taco={'func_8013DEE4':0x98,'func_8013F468':0x88,'func_801424A8':0x90,'func_8013D838':0x80,'func_801413B8':0x90,'func_80140ADC':0x88,'func_80141B18':0xA0}
 if unit=='TACO' and name in taco:
  b=group(b,taco[name],8,'screen_xy')
  # Original halfword pointer registers advance eight bytes, not eight halfwords.
  for v in re.findall(r'    s16 \*(\w+);',b):
   b=re.sub(r'\b'+v+r' \+= (4|8);',lambda m:v+' += '+str(int(m[1])//2)+';',b)
  notes.append('Target lh/sh read eight consecutive projected screen halfwords at sp+0x%X..+0x%X after func_800AB170 writes through pointers to the region. Reconstruct one s16 screen_xy[8] instead of disconnected s32 synthesized scalars; halfword pointer steps 4/8 in generated C represented target byte steps and are halved. Other matrix, input-vector, and pointer stride issues remain unresolved.'%(taco[name],taco[name]+14))
 if unit=='TACO' and name in ['func_801487A4','func_8014A480']:
  low=0x4C if name=='func_801487A4' else 0x50
  b=group(b,low,3,'transformed_xyz');b=group(b,low+8,3,'input_xyz')
  notes.append('Target sh constructs the three-halfword input at sp+0x%X; func_800A0ECC writes to sp+0x%X, subsequently read by lh at offsets 0,2,4. Reconstruct separate three-halfword input/output arrays so the output call initializes all three consumed components.'%(low+8,low))
 if unit=='TACO' and name=='func_8013DB80':
  b=group(b,0x3C,3,'transformed_xyz','s32');b=group(b,0x4C,3,'input_xyz','s32')
  notes.append('Target sw/lw use three consecutive words starting sp+0x4C as func_800AA460 input and sp+0x3C as output; connect each triplet into an s32[3] so output writes initialize all later consumed components.')
 if unit=='BUNKASAI' and name=='func_8015AF90':
  b=b.replace('s32 sp3C[16];','s32 sp3C[4];').replace('s32 sp4C[16];','s32 sp4C[11];')
  notes.append('Target copies exactly four words to sp+0x3C and 44 bytes to sp+0x4C, followed by the 48-byte local at sp+0x78. Shrink synthesized arrays to four and eleven words; synthetic block typedef semantics remain unresolved.')
 if unit=='main' and name=='func_8005F730':
  for low,label in [(0x44,'heights'),(0x50,'widths'),(0x5C,'positions_x'),(0x68,'positions_y')]:b=group(b,low,5,label)
  notes.append('Target sh fills five halfwords in each region based at sp+0x44,50,5C,68 and passes those bases to menu_set. Reconstruct four five-halfword arrays, including synthesized +2/+4/+6/+8 aliases. No padding or frame-size match is forced.')
 if unit=='RENSYU' and name=='func_8013325C':
  b=b.replace('s32 sp40[16];','s16 sp40[11];')
  notes.append('Target copies five words plus a halfword (22 bytes), then indexes with index<<1 and lh. Represent eleven s16 entries; the former sixteen-word array had both wrong element width and extent.')
 if unit=='EN_NICHI' and name=='func_80136DEC':
  for n in ['spC4','sp98','sp6C']:
   b=b.replace('s16 '+n+';','s16 '+n+'[22];').replace('&'+n+' + 0x24','((u8 *)'+n+') + 0x24')
  for n in ['D_80139C68','D_80139C94','D_80139CC0']:b=b.replace('&'+n+' + 0x24','((u8 *)&'+n+') + 0x24')
  notes.append('Target copies 36 bytes followed by two words to each local (44 bytes total); each scalar destination becomes s16[22]. Trailing +0x24 expressions denote byte offsets, not halfword or scalar-word strides. Existing copy-block typedef is retained as unresolved semantic typing.')
 return b,notes

previous_repair=repair
def repair(r,b):
 b,notes=previous_repair(r,b);unit,name=r['scope'].split(':')
 if unit=='EVENT' and name=='func_8011A8E4':
  for n in ['sp90','sp64','sp38']:
   b=b.replace('s32 '+n+'[10];','u16 '+n+'[21];').replace('&'+n+' + 0x24','((u8 *)'+n+') + 0x24').replace('*(&'+n+' + var_v0)',n+'[var_v0]')
  for n in ['D_80125454','D_80125480','D_801254AC']:b=b.replace('&'+n+' + 0x24','((u8 *)&'+n+') + 0x24')
  notes.append('Target copies 40 bytes plus one halfword to three locals, then reads each with lhu at index<<1. Use u16[21], typed element indexing, and byte offsets for the trailing copy. The copy typedef represents only the 40-byte prefix.')
 if unit=='TAIIKU' and name=='func_8013E1FC':
  for low,label in [(0x9C,'positions_y'),(0xAC,'positions_x')]:
   names=['sp%X'%(low+i*4) for i in range(4)]
   for n in names:b=re.sub(r'    s32 '+n+r';\n','',b)
   b=b.replace('{\n','{\n    s32 '+label+'[4];\n',1)
   for i,n in enumerate(names):b=re.sub(r'\b'+n+r'\b',label+'['+str(i)+']',b)
   b=b.replace('&'+label+'[0] +','((u8 *)'+label+') +')
  notes.append('Target accesses two groups of four words at sp+0x9C..A8 and AC..B8 by pair index<<3 and lw +0/+4. Use s32[4] groups with explicit byte views for the already-byte-scaled pair offset. Remaining pointer loops beyond these sites remain unresolved.')
 return b,notes

def review(r,seed,body,notes,asm):
 lines=(seed/'body.c').read_text().splitlines();out=[]
 for f in r['flags']:
  kind=f['kind'];n=f['line'];excerpt=lines[max(0,n-2):n+1]
  if kind=='gamestate-address-arithmetic':verdict='repaired-byte-scaling';explanation=notes[0]
  elif kind=='inferred-copy-layout':
   verdict='reviewed-copy-evidence-partial';explanation='Target lw/sw copy sequences support the documented block byte width. This synthetic block typedef does not establish the original source type; semantic layout and any adjacent/trailing fields remain unresolved. Preserve the flag.'
  else:
   verdict='unresolved-layout';explanation='Stack accesses and initialization/call sites reviewed against the target. Separate synthesized locals or oversized arrays are not evidence of a valid aggregate. No safe complete layout was established in this bounded pass; preserve the flag and original draft.'
   if notes and r['scope'] in ['main:get_fnt','GEKO:func_80144198','DATE2:func_801330D4','DATE:func_80152060','NAME_ENT:func_8013AF78','TAIIKU:func_8013FC5C','RPG_BAT:func_80139E88','EVENT:func_800FF320']:
    verdict='repaired-local-layout';explanation=' '.join(notes)
  if notes and kind != "gamestate-address-arithmetic":
   verdict = "partially-repaired-layout" if kind != "inferred-copy-layout" else "copy-width-reviewed-layout-partial"
   explanation = " ".join(notes) + " Retained flag: full original layout/typing is not proven."
  out.append(dict(**f,verdict=verdict,explanation=explanation,original_excerpt=excerpt))
 return dict(scope=r['scope'],selected_path=str(seed),changed=body!=(seed/'body.c').read_text(),outcomes=out,repair_notes=notes,assembly_path=str(asm),assembly_sha256=hashlib.sha256(asm.read_bytes()).hexdigest(),current_types='include/game.h and include/main_api.h; frozen baseline include closure used for compilation',remaining_limitations='No production integration or matching claim; synthetic copy typedefs and unproven neighboring stack layouts remain review findings.')

def process(r):
 global LOG
 unit,name=r['scope'].split(':');seed=Path(r['selected_path']);folder=BASE/unit/name;folder.mkdir(parents=True,exist_ok=True)
 orig=(seed/'body.c').read_text();body,notes=repair(r,orig);c=funcloc.locate(name,r['src']);asm=Path(trailing_pad.find_asm(c.src,name));a=asm.read_text()
 rv=review(r,seed,body,notes,asm)
 # Archive only relevant instruction excerpts; original game/disassembly remains untouched.
 evidence=[re.sub(r"/\*\s+[0-9A-Fa-f]+\s+([0-9A-Fa-f]+)\s+[0-9A-Fa-f]+\s+\*/",r"\1:",l) for l in a.splitlines() if any(s in l for s in ['$sp','addu','sll','multu','jalr','%hi(D_800E6280)','%lo(D_800E6280)'])]
 (folder/'assembly-evidence.txt').write_text('\n'.join(evidence)+'\n')
 decl=(seed/'declarations.h').read_text() if (seed/'declarations.h').exists() else ''
 if not re.search(r'\bsp\b',body):decl=re.sub(r'^extern s32 sp;\n','',decl,flags=re.M)
 (folder/'body.c').write_text(body);(folder/'declarations.h').write_text(decl)
 result=dict(scope=r['scope'],src=r['src'],size=r['size'],path=str(folder),archive_path=str(folder),status='failed',score=None,ticket='T-16100',source_base='48362009',matching_claim=False,changed=rv['changed'])
 LOG=[]
 try:
  context=replace_definition((seed/'context.c').read_text(),name,body)
  if not re.search(r'\bsp\b',body):context=re.sub(r'^extern s32 sp;\n','',context,flags=re.M)
  (folder/'context.c').write_text(context)
  # Freeze the include closure; archived full-context supersedes preprocessing where present.
  if (seed/'full-context.c').exists():
   pre=replace_definition((seed/'full-context.c').read_text(),name,body)
   if not re.search(r'\bsp\b',body):pre=re.sub(r'^extern s32 sp;\n','',pre,flags=re.M)
   LOG.append('Frozen archived full-context.c replaced at target definition only.')
  else:
   frozen=Path('drafts/T-14500/baseline/include');assert frozen.is_dir(), 'missing frozen headers'
   pre=command(['gcc','-E','-P','-I'+str(frozen),'-I'+str(Path(c.src).parent),'-D__sgi','-D__attribute__(x)=','-DINCLUDE_ASM(F,N)=INCLUDE_ASM(F,N)','-DINCLUDE_RODATA(F,N)=INCLUDE_RODATA(F,N)',str(folder/'context.c')])
  (folder/'full-context.c').write_text(pre)
  # Reuse the full-TU harness; preprocessing frozen text has no remaining header includes.
  obj=order_search.compile_variant(c,pre,folder/'work',False)
  expected,err=funcdiff.expected_object(c);assert not err,err
  want=funcdiff.resolved_functions(expected,[name],c)[name];got=funcdiff.resolved_functions(obj,[name])[name]
  diff=funcdiff.diff_lines(want,got);score=0 if funcdiff.keys(want)==funcdiff.keys(got) else len(diff)
  (folder/'difference.txt').write_text('\n'.join(diff)+'\n');result.update(status='candidate-zero' if score==0 else 'scored',score=score,compile_verified=True)
 except BaseException as exc:result['reason']=str(exc)
 shutil.rmtree(folder/'work',ignore_errors=True)
 (folder/'diagnostics.txt').write_text('\n'.join(LOG)+'\n')
 prov=dict(ticket='T-16100',source_base='48362009',original_path=str(seed),original_body_sha256=hashlib.sha256(orig.encode()).hexdigest(),compiler='IDO 5.3 via order_search.compile_variant, full translation unit, Docker serial',matching_claim=False)
 (folder/'provenance.json').write_text(json.dumps(prov,indent=2)+'\n');(folder/'review.json').write_text(json.dumps(rv,indent=2)+'\n')
 for f in ['body.c','declarations.h','context.c','full-context.c','diagnostics.txt','assembly-evidence.txt']:
  if (folder/f).exists():result[f+'_sha256']=hashlib.sha256((folder/f).read_bytes()).hexdigest()
 (folder/'result.json').write_text(json.dumps(result,indent=2)+'\n')
 print(r['scope'],result['status'],result['score'],'changed' if rv['changed'] else 'preserved',result.get('reason','')[:100],flush=True)

def catalogs():
 results=[json.loads(p.read_text()) for p in sorted(BASE.glob('*/*/result.json'))];reviews=[json.loads(p.read_text()) for p in sorted(BASE.glob('*/*/review.json'))]
 (BASE/'catalog.jsonl').write_text(''.join(json.dumps(r)+'\n' for r in results));(BASE/'review.jsonl').write_text(''.join(json.dumps(r)+'\n' for r in reviews))
for r in ROWS:
 if len(sys.argv)>1 and r['scope'] not in sys.argv[1:]:continue
 process(r);catalogs()
# Scope-specific limits retained even when the broad arithmetic issue was repaired.
LIMITS={
 'MASTER:func_801326B4':'Eight local 0x34-byte records and a 0x30-byte prefix copy are visible. Existing copied word blocks compile, but converting these to the shared Rec34 layout needs analysis of every field use and the trailing word; this pass preserves them.',
 'MASTER:func_801363F4':'The 0x44-byte aggregate assignment is supported by the target word copy, but overlays expose overlapping record views. MasterPoint2 walks and the corresponding global record type must be reconciled before replacing the block type.',
 'ETC:func_8013FFAC':'sp54 is populated through a six-iteration pointer loop while sp58 is independently assigned. They likely alias one six-word texture-address array; the generated pointer increments still scale as s32 units and need a complete loop rewrite before shrinking the buffer.',
 'KANGEI:func_8013769C':'Selected alternative already preserves unaligned 220-byte and 231-byte text copies. The remaining 0x44-byte word block copies global records; no additional safe semantic struct conversion was established.',
 'VALEN:func_80132834':'sp5C is passed to get_h_yuukou as an s16 array and later reused as the eligible-character pool. The synthesized s32[16] is suspicious; two uses and the callee write extent must be reconciled before assigning an exact halfword extent.',
 'SHOUGATU:func_8013EA98':'sp30 receives 12 bytes plus a trailing halfword; another local sp40 table is supplied through the preserved declaration/context. Halfword indexing and table roles need reconciliation; retain the synthesized word-array warning.',
 'main:func_800654D0':'Four generated s32[16] menu arrays are written mainly through s16 pointers at bases sp60,80,A0,C0. Halfword pointer increments and widened generated stores conflict with that declaration; an exact rewrite of the unrolled fill loops remains outstanding.',
 'TT:func_8013D988':'sp78 and sp2BC are starts of large projected-coordinate grids, and sp298 lies inside the first grid. The one-word locals are invalid bounds; recovering the nested loop dimensions and interior aliases is required before assigning array sizes.',
 'TAIIKU:func_80133CFC':'Multiple scalar-address local copies span 20 and 28 bytes, and the 40-byte prefix at sp44 overlaps synthesized sp48/sp4C aliases. This needs unified table layout recovery; merely enlarging or renaming individual locals would retain the alias defect.',
 'TAIIKU:func_80132BD0':'Existing repaired 23-byte and 23-halfword arrays preserve the target trailing copy sizes. The 40-byte block typedef only describes a prefix; retain that inferred-layout finding until the source records have a shared documented type.',
 'main:func_80055AFC':'The 120-byte and 52-byte copies represent save/restore spans crossing several GameState subfields. Widths alone do not establish a replacement record type, so both inferred copy types remain visible.',
 'main:func_8008647C':'Generated sp6C[16] receives indexed writes, while a later pointer starts from detached sp70. The second word must alias the same actual array; loop bounds and all later consumers need reconstruction before changing its extent.',
 'TACO:func_80144430':'sp80 is declared one word but read through projected-point byte offsets. Its pointer loop and AddPrim coordinate consumers demonstrate a larger aggregate; exact vector count and the interior starting aliases are not fully recovered.',
}
for p in BASE.glob('*/*/review.json'):
 rv=json.loads(p.read_text());r=next(x for x in ROWS if x['scope']==rv['scope']);asm=Path(rv['assembly_path']);text=asm.read_text()
 accesses={}
 for op,off in re.findall(r'\b(lb|lbu|lh|lhu|lw|sb|sh|sw)\s+\$\w+, (0x[0-9A-Fa-f]+)\(\$sp\)',text):accesses.setdefault(off,[]).append(op)
 rv['direct_stack_accesses']={k:sorted(set(v)) for k,v in accesses.items()}
 rv['layout_limit']=LIMITS.get(rv['scope'],'The archived repair addresses only the specific assembly-supported width, alias or stride findings described above; remaining generated pointer types, control flow and semantic record layouts require further reconstruction.')
 for outcome in rv['outcomes']:
  if outcome['verdict'] in ['unresolved-layout','reviewed-copy-evidence-partial']:outcome['explanation']+=' '+rv['layout_limit']
 p.write_text(json.dumps(rv,indent=2)+'\n')
catalogs()
