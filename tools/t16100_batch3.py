"""T-16100 batch 3 evidence review and serial full-TU scoring; Docker only."""
import hashlib,json,re,shutil,subprocess,sys
from pathlib import Path
sys.path.insert(0,'tools')
import funcloc,funcdiff,funcdiff_verify as proof,order_search
BASE=Path('drafts/T-16100/batch-3'); ROWS=json.loads(Path('drafts/T-16100/batch-3.json').read_text()); LOG=[]
def command(args):
    if args[0]=='gcc':args=[('-Idrafts/T-14500/baseline/include' if x=='-Iinclude' else x) for x in args]
    p=subprocess.run(args,capture_output=True,text=True)
    LOG.append('$ '+' '.join(args)+'\n'+('[Preprocessed C retained in full-context.c]\n' if args[0]=='gcc' else p.stdout)+p.stderr)
    if p.returncode:raise ValueError(p.stdout+p.stderr)
    return p.stdout
proof.command=command
NOTES={
3:'The candidate list sp74 is indexed after the loop fills entries; one-word storage and independent sp75/sp76 aliases remain invalid. The 44-byte table copy also has array-scaled tail arithmetic. Needs a unified candidate buffer and eleven-word table.',
6:'sp68 is walked to sp468: original offsets differ by 0x400 bytes (256 words), while the draft reserves sixteen words. A second byte lookup still uses synthetic global sp. This requires reconstruction of the complete work buffer, not a frame-size guess.',
8:'The two copied 52-byte tables fit 13 words, but sp70/sp74/spA4/spA8 are uninitialized aliases of their final entries and later indexed reads still use global sp. Needs folding aliases into those arrays.',
14:'The 21-byte initializer at sp2C feeds individual byte offsets sp2E..sp3B. Independent s32 aliases are uninitialized; must replace them with a byte table and verified signed loads.',
15:'Projection outputs span 0x90..0x9F in four 4-byte vertices; independent s32 aliases at halfword offsets are invalid and sp54+&sp90 scales by s16. Needs a four-element screen-coordinate array.',
16:'sp3C is read without a write on the default switch branch after func_80046500; sp48 is an ordinary scalar spill with assignments and is not itself invalid. Need recover the default branch value.',
17:'Stack-address arithmetic mixes a scalar base with byte offsets. Preserve unresolved until vector/position element sizes and output buffer are reconstructed.',
18:'Address taken from a scalar stack local is indexed. Needs the complete local record/array extent, not a pointer cast alone.',
19:'Stack scratch synthesis is not proven by compilation. Need inspect callee writes and bound the buffer before reducing or accepting it.',
20:'Stack-address arithmetic reaches beyond a scalar stack object. Requires reconstruction of coordinate aggregate and alias uses.',
21:'Synthesized stack locals include copied or indirect outputs. The real aggregate extent must follow assembly and callees; retain unresolved.',
22:'Synthesized stack locals need byte/halfword widths and complete initializer bounds reconciled. Retain unresolved rather than accepting a compiler-sized scratch.',
24:'Indexed eight-byte stack position records start at sp78. Scalar sp78/sp7A aliases and source positions need unified record array.',
25:'Four projected points at 0x90..0x9F are independent uninitialized aliases. Needs s16 coordinate pairs and pointer walk termination inside the array.',
27:'Four projected points at 0x80..0x8F are independent uninitialized aliases. Needs s16 coordinate pairs and pointer walk termination inside the array.',
33:'The 46-byte short table is evidence-backed, but the draft retains a sixteen-word selection buffer whose bound depends on arg1. Buffer capacity is not established by this function alone.',
38:'func_80090270 writes via sp2C, then the draft reads independent sp44 without initialization. Recover the actual output aggregate and its +0x18 member.',
40:'The 48-byte and 36-byte copies are bounded, but both selector reads subtract offsets inconsistent with their local objects and one still references global sp. Do not fix solely with an array cast.',
43:'sp40[7] is justified: three words plus a final halfword copy is 14 bytes and the threshold loop tests indices below seven. Remaining sp30 is a 14-byte halfword table stored in four words, and loop boundary may index seven; keep that issue explicit.',
50:'Target has one 68-byte copy, while draft performs overlapping 64-byte plus tail and 68-byte copies. Remove duplicate only after checking exact source and destination arithmetic; retained unresolved here.',
54:'The 192-byte stack copy matches 48 function-pointer words, but the subsequent indexed call still reads synthetic global sp instead of sp2C. Replace with the local table after signature verification.'}
# Explicit repairs tied to element width/copy bounds, not to the audit spelling.
def repair(i,b):
    notes=[]
    if '&D_800E6280 +' in b or '+ &D_800E6280' in b:
        b=b.replace('&D_800E6280 +','(u8 *)&D_800E6280 +').replace('+ &D_800E6280','+ (u8 *)&D_800E6280')
        notes.append('GameState arithmetic changed to explicit byte-address addition: target addu adds unscaled index/byte stride; GameState* addition multiplies by sizeof(GameState). Existing member uses remain available.')
    sizes={22:{'sp2C':11},28:{'sp44':10,'sp58':10,'sp6C':10},29:{'sp44':10,'sp58':10,'sp6C':10},30:{'sp68':8,'sp58':8},13:{'sp54':11,'sp3C':11},26:{'sp30':13},42:{'sp34':11,'sp1C':12},45:{'sp54':11}}
    for name,n in sizes.get(i,{}).items():
        b=b.replace('s32 '+name+'[16];','s16 '+name+'['+str(n)+'];')
        b=b.replace(name+'[0] = (s32)', '*(s32 *)'+name+' = (s32)')
        notes.append(name+': copied halfword table uses signed lh and doubled element index in target; changed sixteen-word scratch to s16['+str(n)+'].')
    for name,n in {21:{'sp68':3}}.get(i,{}).items():
        b=b.replace('s32 '+name+'[16];','s32 '+name+'['+str(n)+'];')
        notes.append(name+': complete '+str(n*4)+'-byte word initializer and matching indexed loop bound replace oversized sixteen-word scratch.')
    if i==22:
        b=b.replace('s16 sp2C[11];','u16 sp2C[11];')
        notes.append('Target table read is lhu, so use unsigned halfwords for this threshold table.')
    if i==3:
        b=b.replace('    s32 sp75;\n','').replace('    s32 sp76;\n','').replace('s32 sp74[1];','u8 sp74[11];').replace('s32 sp3C[10];','s32 sp3C[11];')
        b=b.replace('&sp74 +','sp74 +').replace('var_a0 = sp75;','var_a0 = sp74[1];').replace('var_a1 = sp76;','var_a1 = sp74[2];')
        b=b.replace('&sp3C + 0x24','(u8 *)sp3C + 0x24').replace('&D_80148EC4 + 0x24','(u8 *)&D_80148EC4 + 0x24')
        notes.append('Target fills byte candidate list at +0x74, bounded by eleven characters; +0x75/+0x76 are byte entries. Initializer table is 40+4 bytes, so reserve eleven words and use byte tail offsets.')
    if i==8:
        for alias,expr in [('sp70','sp44[11]'),('sp74','sp44[12]'),('spA4','sp78[11]'),('spA8','sp78[12]')]:
            b=b.replace('    s32 '+alias+';\n','')
            b=re.sub(r'\b'+alias+r'\b',expr,b)
        b=b.replace('*(s32 *)((unsigned char *)(sp + (D_801342D0 * 4)) + (120))','sp78[D_801342D0]').replace('*(s32 *)((unsigned char *)(sp + (D_801342D0 * 4)) + (68))','sp44[D_801342D0]')
        notes.append('Two 52-byte copies at stack+0x78/+0x44 form thirteen-word tables; +0xA4/+0xA8/+0x70/+0x74 are entries eleven/twelve. Indexed stack loads use those arrays.')
    if i==11:
        b=b.replace('&sp44 + 0x24','(u8 *)sp44 + 0x24').replace('&D_80149E90 + 0x24','(u8 *)&D_80149E90 + 0x24').replace('&D_80149EBC + 0x24','(u8 *)&D_80149EBC + 0x24')
        notes.append('Both copied stack tables span 44 bytes; preserve eleven-word objects and correct final-word tail from pointer-to-array/scalar scaling to byte offset +0x28.')
    if i==14:
        for alias in ['sp34','sp38','sp3A','sp33','sp2E','sp39','sp2F','sp31','sp36','sp37','sp3B']:
            b=b.replace('    s32 '+alias+';\n','')
            b=re.sub(r'\b'+alias+r'\b','sp2C['+str(int(alias[2:],16)-0x2C)+']',b)
        b=b.replace('s32 sp2C[6];','u8 sp2C[21];')
        notes.append('Target copies twenty bytes plus one byte to stack+0x2C and reads lbu at offsets +2..+15; fold independent uninitialized aliases into u8[21].')
    if i==17:
        for name,n in [('sp60',10),('sp4C',10),('sp3C',8)]:
            b=b.replace('s32 '+name+'['+str(n//2)+'];','s16 '+name+'['+str(n)+'];')
            b=b.replace('*(&'+name+' + var_v1)','*(s16 *)((u8 *)'+name+' + var_v1)').replace('*(&'+name+' + var_v1_2)','*(s16 *)((u8 *)'+name+' + var_v1_2)')
        notes.append('Initializer extents 20/20/16 bytes and target lh plus byte index stepping two establish halfword tables; explicit byte additions preserve those offsets.')
    if i==23:
        b=b.replace('s32 spA4;','s32 spA4[2];').replace('s32 sp5C;','s32 sp5C[18];').replace('s32 sp48;','s32 sp48[5];')
        b=b.replace('spA4 = D_', 'spA4[0] = D_').replace('sp48 = D_', 'sp48[0] = D_').replace('= sp48;', '= sp48[0];').replace('&(&sp48)[1]','&sp48[1]')
        b=b.replace('var_v0 = &spA4;','var_v0 = spA4;').replace('(u32) &spAC','(u32) (spA4 + 2)').replace('var_v0_2 = &sp5C;','var_v0_2 = sp5C;').replace('(u32) &spA4','(u32) (sp5C + 18)')
        for ptr in ['var_v0','var_v0_2','var_s0','var_v1_3','var_s0_2']:b=b.replace(ptr+' += 4;',ptr+' += 1;')
        notes.append('Target initializers are 8/72/20 bytes at +0xA4/+0x5C/+0x48. Reconstruct 2/18/5 word arrays and correct pointer walks from four words to one word per target +4 bytes.')
    if i==7:
        b=b.replace('    s32 sp3C;\n','').replace('    s32 sp38;','    s32 sp38[6];')
        b=b.replace('sp38 = D_', 'sp38[0] = D_').replace(' = sp38;', ' = sp38[0];').replace(' = sp3C;', ' = sp38[1];')
        b=b.replace('&(&sp38)[2]','&sp38[2]')
        notes.append('Six word loads/stores at offsets 0..0x14 from target stack+0x38 form s32[6]; sp3C is entry one, not an uninitialized scalar.')
    if i==10:
        for off in ['44','48','4C','50','54','58']:b=b.replace('    s32 sp'+off+';\n','')
        b=b.replace('{\n','{\n    s32 sp44[6];\n',1)
        for off,idx in [('58',5),('54',4),('50',3),('4C',2),('48',1),('44',0)]:b=re.sub(r'\bsp'+off+r'\b(?!\[)', 'sp44['+str(idx)+']', b)
        b=b.replace('s32 sp44[0][6]','s32 sp44[6]').replace('(&sp44[0])','sp44')
        notes.append('Six consecutive word locals 0x44..0x58 plus indexed selector store form a six-word array; fold aliases into elements.')
    if i==35:
        b=b.replace('s16 sp84;','s16 sp84[20];').replace('s16 sp5C;','s16 sp5C[20];')
        b=b.replace('var_s2 = &sp84;', 'var_s2 = sp84;').replace('var_s3 = &sp5C;', 'var_s3 = sp5C;')
        notes.append('Each stack initializer copies 36+4 bytes and later pointers step halfwords: two s16[20] arrays replace two scalars.')
    if i==37:
        b=b.replace('s32 sp64[4];','s16 sp64[8];')
        b=b.replace('&sp64 + var_s3','(u8 *)sp64 + var_s3')
        notes.append('Four-word local copy is 16 bytes; target index is already a byte stride, so use byte base rather than pointer-to-array scaling.')
    if i==44:
        b=b.replace('s32 sp7C[1];','u8 sp7C[16];').replace('s32 sp5C[1];','s16 sp5C[16];').replace('(u8) *(&sp7C + temp_hi)','sp7C[temp_hi]')
        notes.append('func_80053418 writes sixteen bytes to a0 and sixteen halfwords to a1; target uses unscaled lbu from stack+0x7C. Reconstruct both output arrays.')
    if i==49:
        b=b.replace('s32 sp4C[16];','s32 sp4C[6];').replace('    s32 sp50;\n','').replace('sp50 =','sp4C[1] =')
        notes.append('Six buffer addresses occupy stack+0x4C..0x60; +0x50 is element one. Six-element call count and word stores establish bound.')
    if i==19:
        for name in ['sp3C','sp7C','spBC']:
            b=b.replace('u8 '+name+';', 'u8 '+name+'[64];').replace('&'+name,name)
        notes.append('Three zero-fill loops span 0x40 bytes at stack+0x3C/+0x7C/+0xBC; reconstruct u8[64] string buffers. sp108 capacity remains unresolved without func_800764AC output bound.')
    if i==51:
        b=b.replace('s32 sp2C[16];','s16 sp2C[16];').replace('s32 sp4C[1];','u8 sp4C[16];')
        b=b.replace('GameState *var_v1;','Rec38 *var_v1;').replace('var_v1 = &D_800E6280;','var_v1 = D_800E6280.unk_1BC;').replace('var_v1->unk_1BC[0].','var_v1->').replace('var_v1 += 0x38;','var_v1 += 1;')
        notes.append('func_80053418 writes sixteen s16 outputs to a1 and bytes to a0; target iterates records by 0x38 bytes, so use GsRec38* increment rather than GameState* +0x38.')
    if i==54:
        b=b.replace('(*(s32 (**)(s32))((unsigned char *)(sp + (D_800B1AF6 * 4)) + (44)))(0x80);','((s32 (*)(s32))sp2C[D_800B1AF6])(0x80);')
        notes.append('Target indexed lw uses stack+0x2C after the 192-byte copy; dispatch through sp2C instead of synthetic global sp.')
    return b,notes

def process(i,row):
    global LOG
    unit,name=row['scope'].split(':'); seed=Path(row['selected_path']); out=BASE/unit/name; out.mkdir(parents=True,exist_ok=True)
    old=(seed/'body.c').read_text(); b,notes=repair(i,old); text=(seed/'context.c').read_text()
    if old not in text:raise ValueError('preserved body not present in context: '+str(seed))
    text=text.replace(old,b,1)
    for f,data in [('body.c',b),('context.c',text),('declarations.h',(seed/'declarations.h').read_text() if (seed/'declarations.h').exists() else '')]:(out/f).write_text(data)
    c=funcloc.locate(name,row['src']); asm=Path(c.asm).read_text() if hasattr(c,'asm') else ''
    if not asm:
        candidates=list(Path('asm').rglob(name+'.s')); candidates=[p for p in candidates if (('/'+unit+'/') in str(p) if unit!='main' else '/main/' in str(p))];asm=candidates[0].read_text() if candidates else ''
    # Compact evidence records preserve target instruction addresses and access widths.
    evidence=[re.sub(r'/\* \S+ (\S+) \S+ \*/',r'\1:',line).strip() for line in asm.splitlines() if re.search(r'\b(?:lbu?|lhu?|lw|sb|sh|sw|addiu|addu|sll|multu)\b',line)]
    evidence=evidence[:20]+[line for line in evidence[20:] if re.search(r'\b(?:lh|lhu|lb|lbu|sll)\b|\$sp, 0x',line)][:20]
    (out/'assembly-evidence.txt').write_text('\n'.join(evidence)+'\n')
    flags=[]
    for flag in row['flags']:
        k=flag['kind']; msg=NOTES.get(i,'')
        if k=='inferred-copy-layout':msg=('Word-block typedef describes a raw copy, not a recovered semantic layout. Target word-copy loop and tail widths are recorded in assembly-evidence.txt; retain opaque copy view until field identities are recovered. '+msg).strip()
        if k=='gamestate-address-arithmetic':msg='Target uses byte offset/index addition to GameState base; explicit byte-base repair prevents sizeof(GameState) scaling. '+msg
        if k=='synthesized-stack-local' and i in [13,26,28,29,30,42,45,49,7,10,51]:msg='Reconstructed copy/access extent and element width; see repair notes. '+msg
        if not msg:msg='Address arithmetic references stack locals beyond their declared scalar/one-element object. Full buffer/field reconstruction remains unresolved; compilation alone does not justify the view.'
        flags.append(dict(flag,outcome='repaired' if notes and k!='inferred-copy-layout' and i not in NOTES else ('justified-false-positive' if i==43 and flag['detail']=='sp40[7]' else 'unresolved'),explanation=msg))
    review=dict(scope=row['scope'],selected_path=str(seed),archive_path=str(out),changed=b!=old,reviewed=True,repair_notes=notes,flags=flags,notes=NOTES.get(i,''),assembly_evidence=str(out/'assembly-evidence.txt'),current_types='include/main_api.h and include/game.h; compilation uses frozen baseline/include closure',matching_claim=False)
    LOG=[]; result=dict(scope=row['scope'],src=row['src'],path=str(out),archive_path=str(out),status='failed',score=None,source_base='48362009',header_base='drafts/T-14500/baseline/include',matching_claim=False,changed=b!=old)
    try:
        obj=order_search.compile_variant(c,text,out/'work',False)
        exp,err=funcdiff.expected_object(c)
        if err:raise ValueError(err)
        want=funcdiff.resolved_functions(exp,[name],c)[name]; got=funcdiff.resolved_functions(obj,[name])[name]
        diff=funcdiff.diff_lines(want,got); score=0 if funcdiff.keys(want)==funcdiff.keys(got) else len(diff)
        result.update(status='candidate-zero' if score==0 else 'scored',score=score,compile_verified=True)
        (out/'difference.txt').write_text('\n'.join(diff)+'\n')
    except BaseException as e:result['reason']=str(e)
    pre=out/'work/guarded.c'
    if pre.exists():shutil.copyfile(pre,out/'full-context.c')
    shutil.rmtree(out/'work',ignore_errors=True)
    (out/'diagnostics.txt').write_text('\n'.join(LOG)+'\n'+str(result.get('reason','Full TU compiled; score '+str(result['score'])))+'\n')
    provenance=dict(ticket='T-16100',original_attempt=str(seed),original_sha256=hashlib.sha256(old.encode()).hexdigest(),source_base='48362009',frozen_header_root='drafts/T-14500/baseline/include',context_source=str(seed/'context.c'),repair='tools/t16100_batch3.py',compiler='IDO 5.3 via tools/docker.sh; full translation unit',matching_claim=False)
    (out/'provenance.json').write_text(json.dumps(provenance,indent=2)+'\n');(out/'review.json').write_text(json.dumps(review,indent=2)+'\n');(out/'result.json').write_text(json.dumps(result,indent=2)+'\n')
    print(i,row['scope'],result['status'],result['score'],flush=True)
for i,r in enumerate(ROWS):
    if len(sys.argv)>1 and str(i) not in sys.argv[1:]:continue
    process(i,r)
BASE.mkdir(parents=True,exist_ok=True)
for kind in ['result','review']:
    (BASE/('catalog.jsonl' if kind=='result' else 'review.jsonl')).write_text(''.join(json.dumps(json.loads(p.read_text()),sort_keys=True)+'\n' for p in sorted(BASE.glob('*/*/'+kind+'.json'))))
