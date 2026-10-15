#!/usr/bin/env python3
"""T-15100: serial, archived repairs of nine unaligned aggregate-copy m2c failures.

Run in the existing project Docker image; /ref is the read-only original census.
No production source or immutable initial attempt is modified. Zero scores need proof.
"""
import hashlib,json,re,shutil,sys
from pathlib import Path
sys.path.insert(0,'tools')
import autotweak,bulk_m2c as bulk,funcloc,funcdiff,m2c,order_search
BASE=Path('drafts/T-14500/repairs-unaligned')
ROWS=json.loads(Path('drafts/T-14500/unaligned-repair-candidates.json').read_text())

def aggregate(body,decls,local,symbol,size,types):
    """Replace observed complete copy, including a three-byte tail, by its real byte aggregate."""
    typ='UnalignedBytes%d'%size
    types[typ]='typedef struct { u8 b[%d]; } %s; /* size 0x%X */'%(size,typ,size)
    types[symbol]='extern %s %s;'%(typ,symbol)
    decls=re.sub(r'^.*\b'+symbol+r'\s*;[^\n]*\n?', '',decls,flags=re.M)
    pattern=r'^    (?:\?|s32|u8) '+local+r'(?:\[[^\]]*\])?;'
    if re.search(pattern,body,re.M):body=re.sub(pattern,'    '+typ+' '+local+';',body,flags=re.M)
    else:body=body.replace('{','{\n    '+typ+' '+local+';',1)
    out=[];done=False
    for line in body.splitlines():
        if symbol in line and (local in line or 'M2C_LWL' in line):
            if not done:out.append('    %s = %s;'%(local,symbol));done=True
        else:out.append(line)
    assert done,(local,symbol)
    body='\n'.join(out)+'\n'
    body=re.sub(r'&'+local+r'\b',local+'.b',body)
    return body,decls

def strings(body,items,start,end):
    """Recover ordinary automatic string initializers in original declaration order."""
    for local,pointer in items:
        literal=re.search(r'    '+pointer+r' = ("[^\n]*");',body).group(1)
        body=body.replace('    ? '+local+';','    u8 '+local+'[] = '+literal+';')
        body=re.sub(r'^    \? \*'+pointer+r';\n','',body,flags=re.M)
    a=body.index(start);b=body.index(end,a)
    body=body[:a]+body[b:]
    return body

def repair(row,raw):
    decls,body=m2c.split_draft(raw,row['func']);types={};notes=[];override=''
    unit=row['unit'];func=row['func']
    if unit=='KANGEI':
        for local,symbol,size in [('sp13C','D_80139FAC',220),('sp54','D_8013A088',231)]:body,decls=aggregate(body,decls,local,symbol,size,types)
        body=body.replace('temp_v1->unk0','*(s16 *)temp_v1').replace('*(&D_80139F38 + (var_s0_2 * 2))','D_80139F38[var_s0_2]').replace('&D_80139F10 + (var_s0_2 * 4)','&D_80139F10[var_s0_2]')
        decls=re.sub(r'^extern \? D_80139F(?:10|38);\n','',decls,flags=re.M)
        types['positions']='typedef struct { s16 x; s16 y; } UnalignedPosition4; /* size 4 */\nextern UnalignedPosition4 D_80139F10[10];\nextern s16 D_80139F38[10];'
        notes.append('220/231-byte tables and ten four-byte position pairs from observed copies/strides.')
    elif unit in ('BUNKAKEN','BUNKASAI'):
        symbol='D_80155F90' if unit=='BUNKAKEN' else 'D_80160520'
        body,decls=aggregate(body,decls,'sp3C',symbol,11,types)
        body=body.replace('? *var_v0;', 'u8 *var_v0;').replace('? *var_v1;', 'u8 *var_v1;')
        notes.append('Eleven-byte local copy: two words plus three-byte tail.')
    elif unit=='GYOZI':
        body=strings(body,[('sp64','temp_t7'),('sp4C','temp_t2'),('sp3C','temp_t8')],'    temp_t7 = ','    func_800BCDF0(')
        notes.append('Three ordinary automatic string initializers recover word copies and unaligned tails.')
    elif unit=='SHOUGATU' and func.endswith('350F8'):
        body=strings(body,[('sp84','temp_t7'),('sp6C','temp_t2'),('sp5C','temp_t8')],'    temp_t7 = ','    M2C_MEMCPY_ALIGNED(&sp38')
        body,decls=aggregate(body,decls,'sp38','D_80143E5C',35,types);notes.append('Three automatic strings and genuine seven-by-five-byte fortune table.')
    elif unit=='SHOUGATU' and func.endswith('359FC'):
        body,decls=aggregate(body,decls,'sp28','D_80143E80',35,types);notes.append('Genuine seven-by-five-byte fortune table (24+8+3 copied bytes).')
    elif unit=='SHOUGATU':
        body,decls=aggregate(body,decls,'sp40','D_801435F8',3,types)
        body=body.replace('= sp40;','= sp40.b[0];').replace('= sp41;','= sp40.b[1];')
        body=body.replace('***D_80143AF4', '***(u8 ***)D_80143AF4').replace('**D_80143AF4','**(u8 ***)D_80143AF4').replace('**D_80143AF8','**(u8 **)D_80143AF8')
        notes.append('Three copied flag bytes; sp41 is the second byte of the same aggregate.')
    elif unit=='ENDING':
        body=body.replace('    ? sp3C;', '    UnalignedBytes27 sp3C;').replace('    ? *temp_t7;\n','')
        a=body.index('    temp_t7 = ');b=body.index('    var_a0 = 0;',a)
        body=body[:a]+'    sp3C = D_8013C0D0;\n'+body[b:]
        body=re.sub(r'^.*M2C_LWL.*\n','',body,flags=re.M).replace('&sp3C','sp3C.b')
        types['UnalignedBytes27']='typedef struct { u8 b[27]; } UnalignedBytes27; /* size 0x1B */'
        types['D_8013C0D0']='extern UnalignedBytes27 D_8013C0D0;'
        # Target callers consume v0; the same integer-return view already exists in main/8007C030.c.
        body,n=re.subn(r'(\s*)dialogue_render\(([^\n]*)\);\n\s*D_80122CD0 = M2C_ERROR\([^\n]*\);',r'\1D_80122CD0 = dialogue_render(\2);',body)
        assert n==9,n
        override='#define MAIN_API_OVERRIDE_dialogue_render /* T-15100: nine observed callers consume v0; existing main/8007C030.c integer ABI view. */\n'
        types['dialogue']='s32 dialogue_render(s32, s32, s32, s32, s32, void *, void *, s32);'
        notes.append('Real 27-byte copied string aggregate and nine consumed dialogue_render integer returns (existing main-source ABI view).')
    elif unit=='GEKO':
        for local,symbol,size in [('sp454','D_80147764',209),('sp420','D_80147838',24),('sp39C','D_80147850',132),('sp318','D_801478D4',132),('sp300','D_80147958',21),('sp2E8','D_80147970',21),('sp74','D_80147988',627),('sp4C','D_80147BFC',37),('sp34','D_80147C24',21)]:body,decls=aggregate(body,decls,local,symbol,size,types)
        body=body.replace('    ? sp43C;','    u8 sp43C[21];').replace('    s16 sp422;\n','').replace('    ? sp41;\n','')
        body=body.replace('sp422 =','*(s16 *)&sp420.b[2] =').replace('&(&sp420[0])[D_800E62BF]','(sp420.b + D_800E62BF * 2)').replace('&(sp420.b[0])[D_800E62BF]', '(sp420.b + D_800E62BF * 2)')
        body=body.replace('&sp43C','sp43C').replace('&sp41','(sp34.b + 13)')
        for local in ['sp300','sp2E8','sp39C','sp318','sp4C','sp34']:
            offset=int(local[2:],16)
            pattern=r'\*\(u8 \*\)\(\(unsigned char \*\)\(sp \+ (.*?)\) \+ \('+str(offset)+r'\)\)'
            body=re.sub(pattern,lambda m:local+'.b['+m.group(1)+']',body)
        # m2c splits the loop's flags-array pointer from its constant offset.
        body=body.replace('? *var_t8_2;','u8 *var_t8_2;').replace('var_t8_2 = &subroutine_arg0;', 'var_t8_2 = sp43C;').replace('var_t8_2 = sp + var_v1_3;','var_t8_2 = sp43C + var_v1_3;')
        body=body.replace('GameState *var_t7;', 'u8 *var_t7;').replace('var_t7 = &D_800E6280', 'var_t7 = (u8 *)&D_800E6280').replace('var_t7->unk_120[0]', 'var_t7[0x120]')
        body=body.replace('*(u8 *)((unsigned char *)var_t8_2 + (1084))', '*var_t8_2')
        body=re.sub(r'\*\(s8 \*\)\(\(unsigned char \*\)var_t8_2 \+ \(1084\)\)', '*var_t8_2',body)
        notes.append('Nine real copied byte tables, 21-byte writable flags and explicit stack aliases; 627-byte tail table.')
    # Raw m2c address arithmetic is in bytes, not GameState-sized elements.
    body=body.replace('(&D_800E6280 +','((u8 *)&D_800E6280 +')
    return decls,body,types,override,notes

def process(row):
    folder=BASE/row['unit']/row['func'];folder.mkdir(parents=True,exist_ok=True)
    old=Path('/ref/drafts/T-14500')/row['unit']/row['func'];raw=(old/'m2c.c').read_text();original=Path(row['src']).read_text();headers=bulk.header_text('.',row['unit']);messages=[];body='';decl_lines=[];result=dict(row,status='failed',score=None,ticket='T-15100',variant='unaligned-aggregate-repair')
    try:
        decls,body,types,override,notes=repair(row,raw)
        body,known,problem=bulk.cleanup(decls,body,bulk.collect_vars(headers+'\n'+original))
        if problem:raise RuntimeError(problem)
        # Preserve the recovered aggregate declarations rather than scalar inference.
        for key in types:known.pop(key,None)
        decl_lines=list(types.values())+sorted(known.values())+bulk.forward_decls(original,row['func'],body,headers,known)
        c=funcloc.locate(row['func'],row['src']);expected,problem=funcdiff.expected_object(c)
        if problem:raise RuntimeError(problem)
        want=funcdiff.resolved_functions(expected,[row['func']],c)[row['func']]
        for attempt in range(12):
            text=autotweak.rewrite_guard(bulk.insert_draft(original,row['func'],body,decl_lines),row['func'],body)
            text=override+text
            (folder/'body.c').write_text(body);(folder/'declarations.h').write_text('\n'.join(decl_lines)+'\n');(folder/'context.c').write_text(text)
            try:
                obj=order_search.compile_variant(c,text,folder/'work',False);got=funcdiff.resolved_functions(obj,[row['func']])[row['func']];diff=funcdiff.diff_lines(want,got);score=0 if funcdiff.keys(want)==funcdiff.keys(got) else len(diff)
                result.update(status='candidate-zero' if score==0 else 'scored',score=score,compile_attempts=attempt+1)
                result.pop('reason',None)
                (folder/'comparison.txt').write_text('\n'.join(diff)+'\n');messages.append('Full-TU IDO 5.3 compile succeeded; diagnostic comparison only.');break
            except (SystemExit,ValueError,OSError) as exc:
                msg=str(exc);messages.append(msg)
                fixed=bulk.cheap_fix(msg,decl_lines,known)
                if fixed is not None and fixed!=decl_lines:decl_lines=fixed;continue
                more=bulk.source_fix(msg,body,decl_lines,row['func'],original,headers)
                if more is None and "redeclaration of '%s'"%row['func'] in msg:
                    repaired=bulk.with_return_type(body,row['func'],'s32')
                    if repaired!=body:more=repaired,decl_lines
                if more is None:result['reason']=bulk.first_error(msg) or msg;break
                body,decl_lines=more
        result['notes']=notes
    except BaseException as exc:result['reason']=str(exc);messages.append(str(exc))
    pre=folder/'work'/'guarded.c'
    if pre.exists():shutil.copyfile(pre,folder/'full-context.c')
    shutil.rmtree(folder/'work',ignore_errors=True)
    result['matching_claim']=False
    result['archive_path']=str(folder)
    result['context_sha256']=hashlib.sha256((folder/'context.c').read_bytes()).hexdigest() if (folder/'context.c').exists() else None
    result['residual_class']='nonmatching instruction, control-flow, stack-layout or inferred-type differences' if result['status']=='scored' else 'compiler failure'
    result['environment_hash']=hashlib.sha256(Path('/ref/drafts/T-14500/environment.json').read_bytes()).hexdigest()
    result['harness_hash']=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    if body:result['body_sha256']=hashlib.sha256(body.encode()).hexdigest()
    (folder/'diagnostics.txt').write_text('\n\n'.join(messages)+'\n');(folder/'result.json').write_text(json.dumps(result,indent=2)+'\n')
    (folder/'provenance.json').write_text(json.dumps(dict(source_base='3d581b25',census_base='48362009',original_attempt=str(old).replace('/ref/',''),original_result=row),indent=2)+'\n')
    print(row['scope'],result['status'],result.get('score'),result.get('reason',''),flush=True)
    return result

if __name__=='__main__':
    BASE.mkdir(parents=True,exist_ok=True)
    for row in ROWS:
        if len(sys.argv)>1 and row['scope'] not in sys.argv[1:]:continue
        process(row)
    results=[json.loads(p.read_text()) for p in sorted(BASE.glob('*/*/result.json'))]
    (BASE/'catalog.jsonl').write_text(''.join(json.dumps(r,sort_keys=True)+'\n' for r in results))
