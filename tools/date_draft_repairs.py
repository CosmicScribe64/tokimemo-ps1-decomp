#!/usr/bin/env python3
"""T-14600: reproducible archived DATE C repairs. Docker with /ref and /hist read-only."""
import json,re,sys,shutil,hashlib
from pathlib import Path
sys.path.insert(0,'tools')
import autotweak,bulk_m2c as bulk,m2c,funcloc,funcdiff,order_search
BASE=Path('drafts/T-14500/repairs/DATE')
rows=json.loads(Path('drafts/T-14500/date-repair-reservation.json').read_text())
def extract(text,func):
    a=re.search(r'^(?:void|s32) '+func+r'\([^\n]*\)\s*\{',text,re.M).start()
    pos=text.index('{',a); depth=1; end=pos+1
    while depth:
        if text[end]=='{':depth+=1
        if text[end]=='}':depth-=1
        end+=1
    return text[a:end]+'\n'
def historical(row,file):
    text=(BASE.parent/'inputs'/file).read_text()
    return extract(text,row['func'])
for row in rows:
    if len(sys.argv)>1 and row['func'] not in sys.argv[1:]:continue
    func=row['func']; out=BASE/func;out.mkdir(parents=True,exist_ok=True)
    old=Path('/ref/drafts/T-14500/DATE')/func
    body=(old/'body.c').read_text() if (old/'body.c').exists() else ''
    decl=(old/'declarations.h').read_text() if (old/'declarations.h').exists() else ''
    original=Path(row['src']).read_text();note=''
    if func.endswith('3C190'):
        body=body.replace('D_800E6280.unk_1BC[D_800E71DF] = (s32) (D_800E6280.unk_1BC[D_800E71DF] +', '(*(s32 *)&D_800E6280.unk_1BC[D_800E71DF]) = (s32) ((*(s32 *)&D_800E6280.unk_1BC[D_800E71DF]) +')
        body=body.replace('*(s32 *)((unsigned char *)(sp + (D_800CA22C[D_80122CDC] * 4)) + (56))','sp38[D_800CA22C[D_80122CDC]]')
        note='Recover stack array indexing and explicit first-word access to struct.'
    elif func.endswith('3C410'):
        raw=(old/'m2c.c').read_text();body=extract(raw,func)
        body=body.replace('? sp18;', 'typedef struct { u8 b[231]; } DatePlaceWeights;\n    DatePlaceWeights sp18;')
        a=body.index('    M2C_MEMCPY_ALIGNED');b=body.index('    if (arg0',a)
        body=body[:a]+'    sp18 = D_8015B7A0;\n'+body[b:]
        body=body.replace('*(&sp18 + (D_800E71DF * 0x15) + D_800CA2F8)','sp18.b[D_800E71DF * 0x15 + D_800CA2F8]')
        # The observed copy is 0xE4 aligned bytes plus the final three bytes.
        body=body.replace('    typedef struct { u8 b[231]; } DatePlaceWeights;\n','')
        decl='typedef struct { u8 b[231]; } DatePlaceWeights; /* 11 rows of 21 bytes */\nextern DatePlaceWeights D_8015B7A0;\nextern s16 D_800E6392;\nextern u8 D_800E71DF;\n'
        note='Recover 11x21 byte aggregate from 228-byte copy plus unaligned three-byte tail.'
    elif func.endswith('3FD6C'):
        body=body.replace('&D_7FFF + 1','0x8000').replace('(s32) &D_7FFF','0x7FFF')
        decl=decl.replace('extern s32 D_7FFF;\n','');note='Absolute-address mask symbol denotes integer constant; existing scalar union fields retained.'
    elif func.endswith('449D4'):
        body=historical(row,'449-u32.c')
        decl='typedef struct { void (*f[76])(); } DateFnTable76; /* 0x130 */\nextern DateFnTable76 D_8015BF50;\n'
        defs=Path('src/ovl/DATE/80132000.c').read_text();headers=bulk.header_text('.','DATE')
        for name in sorted(set(re.findall(r'\bfunc_[0-9A-F]+\b',body))):
            if name==func or bulk.prototype_in(headers,name):continue
            match=re.search(r'^(?:void|s32|u16|s16|u8|s8) '+name+r'\([^\n]*?\)',defs,re.M)
            if match:decl+=match.group(0)+';\n'
        note='Reuse T-14100 449-u32.c real 76-callback aggregate; unsigned index historical 315-row draft.'
    elif func.endswith('44E20'):
        body=historical(row,'44e20-large1.c');names=['8015C080','8015C0C8','8015C154','8015C214','8015C294','8015C320','8015C3A8','8015C438','8015C4CC','8015C594','8015C630','8015C6B0','8015C738','8015C8D8','8015C94C','8015CA00','8015CAA4','8015CB30','8015CC60','8015CD88','8015CE04','8015CE80'];sizes=[(int(b,16)-int(a,16))//4 for a,b in zip(names,names[1:])]
        decl=''.join('typedef struct { void (*f[%d])(); } DateFnTable%d; /* 0x%X */\n'%(n,n,n*4) for n in sorted(set(sizes)))
        decl+=''.join('extern DateFnTable%d D_%s;\n'%(n,name) for name,n in zip(names,sizes))+'extern u8 D_8015BF48;\n'
        note='Reuse T-14100 full typed 21-aggregate callback selector, historical 631-row draft.'
    elif func.endswith('511B4'):
        a=body.index('    s32 sp2C');b=body.index('    if (D_80122D04',a)
        body=body[:a]+'    FnTbl40 tbl;\n\n    tbl = D_8015EC24;\n    tbl.f[D_800E738A](0x80);\n'+body[b:]
        note='Use existing TU FnTbl40 declaration and genuine 40-callback copy.'
    elif func.endswith(('52268','52E34')):
        body=bulk.with_return_type(body,func,'s32');note='Existing earlier unprototyped call has implicit int ABI; preserve s32 definition.'
    elif func.endswith('55004'):
        a=body.index('    s32 sp2C');b=body.index('    s16 var_a0',a)
        body=body[:a]+'    DateFnTable53 tbl;\n'+body[b:]
        a=body.index('    *(M2cBlockD4');b=body.index('    if (D_800E738A',a)
        body=body[:a]+'    tbl = D_8015F82C;\n    tbl.f[D_800E738A](0x80);\n'+body[b:]
        decl=decl.replace('extern s32 D_8015F82C;','typedef struct { void (*f[53])(); } DateFnTable53; /* 0xD4 */\nextern DateFnTable53 D_8015F82C;');note='Recover genuine 53-callback table instead of undersized integer buffer.'
    decl_lines=decl.strip().split('\n') if decl.strip() else []
    placed=bulk.insert_draft(original,func,body,decl_lines);plain=autotweak.rewrite_guard(placed,func,body)
    (out/'body.c').write_text(body);(out/'declarations.h').write_text(decl);(out/'context.c').write_text(plain)
    (out/'provenance.json').write_text(json.dumps(dict(ticket='T-14600',source_base='48362009',original_attempt='drafts/T-14500/DATE/'+func,note=note,original_result=row),indent=2)+'\n')
    c=funcloc.locate(func,row['src']);result=dict(scope=row['scope'],status='failed',score=None,note=note)
    try:
        expected,problem=funcdiff.expected_object(c)
        if problem:raise RuntimeError(problem)
        obj=order_search.compile_variant(c,plain,out/'work',False)
        want=funcdiff.resolved_functions(expected,[func],c)[func];got=funcdiff.resolved_functions(obj,[func])[func]
        diff=funcdiff.diff_lines(want,got);score=0 if funcdiff.keys(want)==funcdiff.keys(got) else len(diff)
        result.update(status='candidate-zero' if score==0 else 'scored',score=score)
        (out/'diagnostics.txt').write_text('Full-TU IDO 5.3 compile succeeded. Comparison remains NON_MATCHING.\n')
        (out/'comparison.txt').write_text('\n'.join(diff)+'\n')
    except BaseException as exc:
        (out/'diagnostics.txt').write_text(str(exc));result['reason']=str(exc)
    pre=out/'work'/'guarded.c'
    if pre.exists():shutil.copyfile(pre,out/'full-context.c')
    shutil.rmtree(out/'work',ignore_errors=True)
    result['body_sha256']=hashlib.sha256(body.encode()).hexdigest()
    result['context_sha256']=hashlib.sha256(plain.encode()).hexdigest()
    result['residual_class']='frame offsets and register scheduling' if func.endswith('511B4') else 'instruction, control-flow or register allocation differences'
    result['matching_claim']=False
    (out/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(func,result['status'],result['score'],flush=True)
