"""T-16100 batch 2: serial evidence review and full-TU draft-only repairs; Docker only."""
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path
sys.path.insert(0, 'tools')
import cc
import funcdiff
import funcloc
import object_layout
import trailing_pad

BASE = Path('drafts/T-16100/batch-2')
ROWS = json.loads(Path('drafts/T-16100/batch-2.json').read_text())
NOTES = [
 'Mixed GameState pointer walks and byte displacements; record stride 4 belongs to unk_69C. Multiple aliases and predecessor reads need coherent aggregate rewrite, not a cast-only fix.',
 'Loop adds an unscaled index to GameState address and then accesses menu bytes; requires unk_10A5 indexing and review of Rec24 byte offsets throughout.',
 'GameState+index is incorrectly structure-scaled; menu byte stores at 0x10A9 and adjacent fields need typed reconstruction across both loops.',
 'The 0x38 girl stride is bytes, not GameState objects. Shared var_v1 and byte stores need a Rec38-based rewrite.',
 '68-byte memcpy loop has 0x44 stride table origin; local block is an exact opaque copy representation, not an identified record type. Retained pending shared record recovery.',
 '68-byte copied record plus GameState+index menu updates: copy extent is supported but GameState arithmetic remains invalid.',
 'Several scalar stack fragments participate in by-value transform calls; independent names do not establish their aggregate ABI. Needs coordinated TcPos/SDK reconstruction.',
 'sp9C and spAC indexed with an 8-byte stride indicate two coordinate arrays. Declared scalar bases and fixed adjacent slots must be unified before safe repair.',
 'sp52 is synthesized at a halfword interior while sp60[8] and sp5C describe transform storage; current by-value call reconstruction remains unresolved.',
 '88-byte copied table is indexed by a byte row/column encoding. Reconstruct as u8[88] and use byte indexing; keep exact opaque copy extent.',
 'sp54/sp68 each have nine signed halfword elements copied through offsets 0..16; naming alone is a detector false positive. Other pointer-walk scaling still warrants later work.',
 'sp7C[8] has documented eight-entry scratch purpose, but GameState* byte-stride walks through unk_54C remain incorrect and require one coherent rewrite.',
 '0x24-byte copy then words at +0x24 and +0x28 requires 44-byte destination. Repair tail byte arithmetic and local extent; replace raw sp lookup with sp34 indexing.',
 'Copy blocks 0x78/0x34 represent save payload sections, not independently recovered structs; exact extents alone do not justify invented game types.',
 'sp48[12] covers twelve halfwords used by the three repeated groups; synthesized name is not itself bad. Preserve the evidence-backed array.',
 'Many synthesized s32 locals at halfword offsets overlap and RECT-like storage is split. Needs explicit packed structs and SDK ABI evidence, not individual renamed scalars.',
 'spA4/sp90 initializations write five words into s16 scalars, sp70 writes eight words, sp44 copies 40. Real overlapping stack storage defect; large coordinated reconstruction deferred.',
 'Assembly stores and loads indexed halfwords with stride 2. Existing s32[16] and s32-scaled additions are wrong; use s16[16] and element indexing.',
 '0x75E record stride 8 and byte at +0x761 identify unk_75E[index].unk_03. Repair store directly through established aggregate.',
 'sp70[16] is used alongside split s16 coordinate locals. Name flag alone is insufficient; aggregate extent and aliases remain unresolved.',
 'Three 0x1120-byte scratch planes are based on scalar sp90; current stack allocation cannot represent those buffers. Needs complete plane layout/element-width reconstruction.',
 'sp3C[16]/sp54[16] overlap original offsets and halfword aliases; must reconstruct copied coordinate structures rather than keep independently oversized arrays.',
 'sp78[16] and sp98[1] overlap logical stack slots and unresolved sp references remain. Needs actual copied table and element type reconstruction.',
 'Read at 0x758 + 8*index equals unk_75E[index-1].unk_02. Established GameState documentation confirms one-based record indexing; repair direct member read.',
 '240-byte copy and address calculation index*20 identify byte table. Repair s32-array-pointer scaling to explicit byte stride.',
 'Scalar sp6C/sp8C/sp80 fragments coexist with subroutine_arg extern artifacts. Aggregate/by-value ABI remains unresolved.',
 '36-byte copied record plus sp78[1] and raw sp indexing do not define a valid full local table; exact copy alone is insufficient.',
 'Girl stride 0x38 followed by +0x1C8 targets Rec38.unk_0C; local pointer alias spans several views and requires typed reconstruction.',
 'sp24 is a TcPos-by-value return fragment. Needs canonical TcPos return signature/usage at all call sites.',
 'spDA is intentionally uninitialized byte in original default arm; exact 54/66/66-byte copies already recovered. Preserve explicit diagnostic; flag is justified false positive.',
 '84-byte copy is supported, but sp8F[1]/sp7A[1] are byte-interior aliases in wider stack copies. Needs shared typed stack representation.',
 '40 copied words plus one trailing word total 164 bytes. Repair extent, byte tail, and indexed byte address; retain unresolved wider table semantics.',
 '0x30/0x24 byte memcpy extents exist but destinations sp6C/sp9C are scalars. Needs coordinate/SDK array layout recovery.',
 'Final table is 40 copied bytes plus one halfword. Repair 42-byte local and tail, and replace raw sp table reads with real local indexing.',
 'Nine and ten words are copied into ten-word buffers; inspect trailing word and source types before adopting a named layout. Preserve copy representation.',
 'sp28[16]/sp30[16] overlap and 44-byte copy targets local scalar. Need one actual stack record; no declaration-only rename.',
 'Constant 2*4 byte GameState arithmetic addresses unk_1228[8] and unk_1228[40]. Repair direct established array fields.',
 'sp54[6] has six word stores and six-entry accesses; synthesized naming is a detector false positive, though other source quality is outside these flags.',
 'Four four-word tables are copied at 0x64/0x80/0x90/0xA0; repair sp64[4], one-past-end bound, word pointer increment, and raw sp lookups to the actual arrays.',
 'KANGEI duplicates OMIMAI copied record and overlapping sp28/sp30 buffers; same coordinated record repair needed.',
 'sp88[4] is a copied four-word pointer table. Repair byte-stride lookup, unify four flags at sp5C into one four-word array, and use one-past-end bound with word increments.',
 'sp3C/sp3A represent overlapping halfword/word position fragments; subroutine_arg artifacts remain and need TcPos ABI recovery.',
 'Copy is 0x2D0 bytes loop + word + halfword = 726 bytes; strings indexed at byte stride 33. Reconstruct a 726-byte char array and correct byte tail/base.',
 '44-byte memcpy at sp30 is eleven word entries and original girl loop has eleven iterations. Repair sp30[11], keep separate byte sp63, and use Rec38 pointer iteration.',
 '60-byte copied table stored in sp4C[16] is an exact block but buffer extra element has no identified need; preserve unresolved table type.',
 'Shift/subtract computes girl index*0x38 and +0x1C8 is Rec38.unk_0C. Repair established GameState record field.',
 '156-byte copy is supported but raw sp uses remain instead of sp30 indexing. Opaque block alone is insufficient to claim valid full stack layout.',
 '68-byte global-to-global copy is exact extent but no known record struct; retained opaque copy with explicit unresolved semantic type.',
 '184-byte copy exists but raw sp dispatch/pointer reads remain; needs function-pointer/coordinate table reconstruction.',
 '208-byte block copy plus one trailing word needs 212-byte local; dispatch through stack+index must become sp2C function pointer indexing.',
 '64/68-byte global copies have exact extents, opaque block types are not established semantic layouts; retain until shared record type recovered.',
 'sp38[16] and sp3A overlap halfword fields of copied storage. Need actual halfword record and bounds.',
 '148-byte copy exists but raw sp accesses remain; preserve unresolved dispatch table representation.',
 '252-byte copy exists but raw sp accesses remain; preserve unresolved dispatch table representation.',
 'GameState pointer advances by 4 bytes while C advances by four whole objects; use GsRec69C pointer and direct queue members throughout.'
]

def repair(i, body):
    if i == 9:
        body = body.replace('s32 sp68[22];', 'u8 sp68[88];')
        body = body.replace('*(&sp68 + ', '*(sp68 + ')
    elif i == 12:
        body = body.replace('s32 sp34[10];', 's32 sp34[11];')
        body = body.replace('(&sp34 + 0x24)', '((u8 *)sp34 + 0x24)').replace('(&D_801494C4 + 0x24)', '((u8 *)&D_801494C4 + 0x24)')
        body = body.replace('*(s32 *)((unsigned char *)(sp + (D_801491EC * 4)) + (52))', 'sp34[D_801491EC]')
    elif i == 17:
        body = body.replace('s32 sp4C[16];', 's16 sp4C[11];')
        body = body.replace('*(&sp4C[0] + var_t9)', 'sp4C[var_s2]').replace('*(&sp4C[0] + var_t1_2)', 'sp4C[var_s0_2]')
    elif i == 18:
        body = body.replace('var_t6 = &D_800E6280 + (D_800E71DE * 8);', 'var_t6 = (u8 *)&D_800E6280.unk_75E[D_800E71DE];')
        body = body.replace('*(s8 *)((unsigned char *)var_t6 + (1889)) = 1;', '*(s8 *)((unsigned char *)var_t6 + (3)) = 1;')
    elif i == 24:
        body = body.replace('&sp30 + (((u16) D_800E6374 >> 0xC) * 0x14)', '(u8 *)sp30 + (((u16) D_800E6374 >> 0xC) * 0x14)')
    elif i == 23:
        body = body.replace('*(u8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1880))', 'D_800E6280.unk_75E[D_800E71DE - 1].unk_02')
    elif i == 31:
        body = body.replace('s32 sp2C[40];', 's32 sp2C[41];')
        body = body.replace('(&sp2C + 0x9C)', '((u8 *)sp2C + 0x9C)').replace('(&D_80121A34 + 0x9C)', '((u8 *)&D_80121A34 + 0x9C)')
        body = body.replace('var_t6 + &sp2C', 'var_t6 + (u8 *)sp2C')
        body = body.replace('*(s32 *)((unsigned char *)(&spD0) + (4)) =', 'spD4 =')
    elif i == 33:
        body = body.replace('s32 sp34[10];', 'u16 sp34[21];')
        body = body.replace('(&sp34 + 0x24)', '((u8 *)sp34 + 0x24)').replace('(&D_8015B984 + 0x24)', '((u8 *)&D_8015B984 + 0x24)')
        for off, name in [(188,'spBC'), (104,'sp68'), (272,'sp110')]:
            body = body.replace('*(s32 *)((unsigned char *)(sp + var_v0) + (%s))' % off, name+'[D_800CA2F8]')
        body = body.replace('*(u16 *)((unsigned char *)(sp + (D_800CA2F8 * 2)) + (52))', 'sp34[D_800CA2F8]')
    elif i == 36:
        body = body.replace('    u8 *temp_v0;\n', '').replace('            temp_v0 = (2 * 4) + &D_800E6280;\n', '')
        for offset in [4672,4800,4676,4804,4680,4808,4668,4796]:
            body = body.replace('*(s32 *)((unsigned char *)temp_v0 + (%s)) = -1;' % offset, 'D_800E6280.unk_1228[%s] = -1;' % ((offset+8-0x1228)//4))
    elif i == 42:
        body = body.replace('s32 sp2C[181];', 'u8 sp2C[726];')
        body = body.replace('(&sp2C + 0x2D0)', '(sp2C + 0x2D0)').replace('(&date_place_names + 0x2D0)', '((u8 *)&date_place_names + 0x2D0)')
        body = body.replace('+ &sp2C', '+ sp2C')
    elif i == 38:
        body = body.replace('s32 sp64[16];', 's32 sp64[4];').replace('    s32 sp74[1];\n', '')
        body = body.replace('var_s2 += 4;', 'var_s2 += 1;').replace('var_s2 != &sp74', 'var_s2 != &sp64[4]')
        for offset,name in [(160,'spA0'),(144,'sp90'),(128,'sp80')]:
            body = body.replace('*(s32 *)((unsigned char *)(sp + var_s1_2) + (%s))' % offset, name+'[var_s1_2 / 4]')
    elif i == 40:
        body = body.replace('    s32 sp6C[1];\n', '').replace('    s32 sp68;\n', '').replace('    s32 sp64;\n', '').replace('    s32 sp60;\n', '').replace('    s32 sp5C;', '    s32 sp5C[4];')
        body = body.replace('var_s0_2 = &sp5C;', 'var_s0_2 = sp5C;').replace('var_s0_2 += 4;', 'var_s0_2 += 1;').replace('var_s0_2 != &sp6C', 'var_s0_2 != &sp5C[4]')
        body = body.replace('*(var_s2 + &sp88)', 'sp88[var_s2 / 4]')
        for j,name in reversed(list(enumerate(['sp5C','sp60','sp64','sp68']))):
            body = body.replace(name+' = 1;', 'sp5C[%s] = 1;' % j)
        body = body.replace('sp5C = D_8015F000;', 'sp5C[0] = D_8015F000;')
    elif i == 43:
        body = body.replace('s32 sp30[16];', 's32 sp30[11];').replace('GameState *var_a0;', 'Rec38 *var_a0;')
        body = body.replace('var_a0 = &D_800E6280;', 'var_a0 = D_800E6280.unk_1BC;').replace('var_a0->unk_1BC[0].unk_0C.w', 'var_a0->unk_0C.w').replace('var_a0 += 0x38;', 'var_a0 += 1;')
    elif i == 45:
        body = body.replace('*(s32 *)((unsigned char *)(&D_800E6280 + ((var_t6 - var_s0) * 8)) + (456))', 'D_800E6280.unk_1BC[var_s0].unk_0C.w')
    elif i == 49:
        body = body.replace('s32 sp2C[52];', 's32 sp2C[53];')
        body = body.replace('(&sp2C + 0xCC)', '((u8 *)sp2C + 0xCC)')
        body = body.replace('(*(s32 (**)(s32))((unsigned char *)(sp + (D_800B1AF6 * 4)) + (44)))(0x80)', '((s32 (*)(s32))sp2C[D_800B1AF6])(0x80)')
    elif i == 54:
        body = body.replace('GameState *var_a0;', 'GsRec69C *var_a0;').replace('var_a0 = &D_800E6280;', 'var_a0 = D_800E6280.unk_69C;')
        body = body.replace('var_a0->unk_69C[0].unk_00', 'var_a0->unk_00')
        body = body.replace('*(u8 *)((unsigned char *)var_a0 + (1688))', 'var_a0[-1].unk_00').replace('*(u8 *)((unsigned char *)var_a0 + (1684))', 'var_a0[-2].unk_00')
        body = body.replace('var_a0 += 4;', 'var_a0 += 1;')
        body = body.replace('u8 *var_a0_2;', 'GsRec69C *var_a0_2;').replace('(var_v0_2 * 4) + &D_800E6280', '&D_800E6280.unk_69C[var_v0_2]')
        body = body.replace('*(u8 *)((unsigned char *)var_a0_2 + (1692))', 'var_a0_2->unk_00').replace('var_a0_2 += 4;', 'var_a0_2 += 1;')
        body = body.replace('(u32) &D_800E6300', '(u32) &D_800E6280.unk_69C[32]')
    return body

def run(row, i):
    unit, name = row['scope'].split(':')
    seed = Path(row['selected_path']); folder = BASE / unit / name; folder.mkdir(parents=True, exist_ok=True)
    old = (seed/'body.c').read_text(); body = repair(i, old); changed = body != old
    decl = (seed/'declarations.h').read_text() if (seed/'declarations.h').exists() else ''
    context = (seed/'context.c').read_text()
    if changed:
        assert old.strip() in context, 'archived body absent from context'
        context = context.replace(old.strip(), body.strip(), 1)
    for filename, data in [('body.c',body), ('declarations.h',decl), ('context.c',context)]: (folder/filename).write_text(data)
    c = funcloc.locate(name, row['src'])
    paths = list(Path('asm').rglob(name+'.s'))
    paths = [p for p in paths if ('/main/' in str(p) if unit=='main' else '/ovl/'+unit+'/' in str(p))]
    asm = paths[0].read_text() if len(paths)==1 else ''
    evidence = [re.sub(r'/\* \w+ (\w+) \w+ \*/', r'/* \1 */', s) for s in asm.splitlines() if any(x in s for x in ('$sp', 'sll ', 'addu ', 'addiu ', 'lhu ', 'lh ', 'lb ', 'lbu '))]
    evidence = evidence[:24]
    (folder/'evidence.txt').write_text('Target: '+str(paths)+'\nReview: '+NOTES[i]+'\n\n'+'\n'.join(evidence)+'\n')
    result = dict(scope=row['scope'],src=row['src'],size=row['size'],path=str(folder),archive_path=str(folder),ticket='T-16100',matching_claim=False,status='failed',score=None,changed=changed,source_base='48362009')
    log=[]; work=folder/'work'; work.mkdir(exist_ok=True)
    try:
        def command(args):
            p=subprocess.run(args,capture_output=True,text=True)
            log.append('$ '+' '.join(args)+'\n'+('[Preprocessed full TU retained]\n' if '-E' in args else p.stdout)+p.stderr)
            if p.returncode: raise ValueError(p.stdout+p.stderr)
            return p.stdout
        pre=command(['gcc','-E','-P','-Idrafts/T-14500/baseline/include','-Iinclude','-I'+os.path.dirname(c.src),'-D__sgi','-D__attribute__(x)=','-DINCLUDE_ASM(F,N)=INCLUDE_ASM(F,N)','-DINCLUDE_RODATA(F,N)=INCLUDE_RODATA(F,N)',str(folder/'context.c')])
        (folder/'full-context.c').write_text(pre)
        obj=work/'guarded.o'
        command(['python3','tools/cc.py',str(folder/'full-context.c'),str(obj),'ido','5.3'])
        includes={n for _,n in cc.INCLUDE_ASM_RE.findall(pre)}
        trailing_pad.pad_functions(str(obj), str(c.src), includes)
        obj.write_bytes(object_layout.trim(obj.read_bytes(),object_layout.source_extent(c.src)))
        expected,err=funcdiff.expected_object(c)
        if err:raise ValueError(err)
        want=funcdiff.resolved_functions(expected,[name],c)[name];got=funcdiff.resolved_functions(str(obj),[name])[name]
        diff=funcdiff.diff_lines(want,got);score=0 if funcdiff.keys(want)==funcdiff.keys(got) else len(diff)
        concise = [re.sub(r'^([+-])[0-9a-fA-F]{8}\s*', r'\1', s) for s in diff[:80]]
        (folder/'difference.txt').write_text('Difference rows: '+str(len(diff))+'; first 80 mnemonic/relocation rows; opcode words omitted.\n'+'\n'.join(concise)+'\n')
        result.update(status='candidate-zero' if score==0 else 'scored',score=score,compile_verified=True)
    except BaseException as exc: result['reason']=str(exc)
    shutil.rmtree(work,ignore_errors=True)
    (folder/'diagnostics.txt').write_text('\n'.join(log)+'\n'+result.get('reason','Full translation unit compiled and scored.')+'\n')
    header_hashes={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(Path('drafts/T-14500/baseline/include').rglob('*.h'))}
    provenance=dict(selected_path=str(seed),original_result=json.loads((seed/'result.json').read_text()),source_base='48362009',headers='drafts/T-14500/baseline/include first, current include fallback',frozen_header_sha256=header_hashes,selected_context_sha256=hashlib.sha256((seed/'context.c').read_bytes()).hexdigest(),body_sha256=hashlib.sha256(body.encode()).hexdigest(),original_body_sha256=hashlib.sha256(old.encode()).hexdigest(),assembly_path=[str(p) for p in paths],notes=NOTES[i])
    (folder/'provenance.json').write_text(json.dumps(provenance,indent=2)+'\n')
    result['review_outcome']='repaired-with-unresolved-remainder' if changed else ('justified-false-positive' if i in [10,14,29,37] else 'unresolved')
    (folder/'result.json').write_text(json.dumps(result,indent=2)+'\n')
    review=dict(scope=row['scope'],path=str(folder),changed=changed,outcome=result['review_outcome'],flags_reviewed=row['flags'],explanation=NOTES[i],evidence_path=str(folder/'evidence.txt'),compile_status=result['status'],score=result['score'])
    (folder/'review.json').write_text(json.dumps(review,indent=2)+'\n')
    print(row['scope'],result['status'],result['score'],result['review_outcome'],flush=True)

def finalize():
    headers={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(Path('drafts/T-14500/baseline/include').rglob('*.h'))}
    results=[];reviews=[]
    for i,row in enumerate(ROWS):
        unit,name=row['scope'].split(':');folder=BASE/unit/name;seed=Path(row['selected_path'])
        result=json.loads((folder/'result.json').read_text());review=json.loads((folder/'review.json').read_text());prov=json.loads((folder/'provenance.json').read_text())
        paths=prov['assembly_path'];lines=Path(paths[0]).read_text().splitlines() if len(paths)==1 else []
        evidence=[re.sub(r'/\* \w+ (\w+) \w+ \*/',r'/* \1 */',s) for s in lines if any(x in s for x in ('$sp','sll ','addu ','addiu ','lhu ','lh ','lb ','lbu '))][:24]
        (folder/'evidence.txt').write_text('Target: '+str(paths)+'\nReview: '+NOTES[i]+'\n\nSelected target mnemonic/operand excerpts (opcode words omitted):\n'+'\n'.join(evidence)+'\n')
        diff=folder/'difference.txt'
        if diff.exists() and not diff.read_text().startswith('Difference rows:'):
            lines=diff.read_text().splitlines();diff.write_text('Difference rows: '+str(len(lines))+'; first 80 mnemonic/relocation rows; opcode words omitted.\n'+'\n'.join(re.sub(r'^([+-])[0-9a-fA-F]{8}\s*',r'\1',s) for s in lines[:80])+'\n')
        prov.update(notes=NOTES[i],frozen_header_sha256=headers,selected_context_sha256=hashlib.sha256((seed/'context.c').read_bytes()).hexdigest())
        for filename in ['body.c','declarations.h','context.c','full-context.c','diagnostics.txt']:
            assert (folder/filename).exists(),(folder,filename)
            result[filename.replace('.','_')+'_sha256']=hashlib.sha256((folder/filename).read_bytes()).hexdigest()
        review['explanation']=NOTES[i]
        review['flag_outcomes']=[dict(flag=f,outcome=result['review_outcome'],explanation=NOTES[i]) for f in row['flags']]
        for filename,data in [('provenance.json',prov),('result.json',result),('review.json',review)]:
            (folder/filename).write_text(json.dumps(data,indent=2)+'\n')
        results.append(result);reviews.append(review)
    (BASE/'catalog.jsonl').write_text(''.join(json.dumps(r,sort_keys=True)+'\n' for r in results if r['status'] in ['scored','candidate-zero']))
    (BASE/'review.jsonl').write_text(''.join(json.dumps(r,sort_keys=True)+'\n' for r in reviews))
    summary=dict(reviewed=len(reviews),compiled=sum(r['status']=='scored' for r in results),changed=sum(r['changed'] for r in results),zero=sum(r['score']==0 for r in results),false_positive=sum(r['review_outcome']=='justified-false-positive' for r in results),changes=[dict(scope=r['scope'],old=json.loads((Path(ROWS[i]['selected_path'])/'result.json').read_text())['score'],new=r['score']) for i,r in enumerate(results) if r['changed']])
    (BASE/'summary.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps(summary,indent=2))

if __name__=='__main__' and '--finalize' in sys.argv:
    finalize()
elif __name__=='__main__':
    for i,row in enumerate(ROWS):
        if len(sys.argv)>1 and str(i) not in sys.argv[1:]:continue
        run(row,i)
    results=[json.loads(p.read_text()) for p in sorted(BASE.glob('*/*/result.json'))]
    reviews=[json.loads(p.read_text()) for p in sorted(BASE.glob('*/*/review.json'))]
    (BASE/'catalog.jsonl').write_text(''.join(json.dumps(r,sort_keys=True)+'\n' for r in results if r['status'] in ('scored','candidate-zero')))
    (BASE/'review.jsonl').write_text(''.join(json.dumps(r,sort_keys=True)+'\n' for r in reviews))
