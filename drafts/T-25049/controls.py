from pathlib import Path
import sys,re
sys.path.insert(0,'tools')
import funcdiff as f
import funcdiff_verify as v
name='func_80073CB0';c=f.locate(name,'main')
a,err=f.expected_object(c);assert err is None,err
a=f.resolved_functions(a,[name],c)[name]
body=Path('drafts/T-25049/recovered.c').read_text()
clean=body.replace('                var_t0 = D_800B6890[3][var_v1];\n                goto block_55;', '                D_801230D0 = D_800B6890[3][var_v1];\n                break;').replace('                var_t0 = (*D_800B68A8)[var_v1];\nblock_55:\n                D_801230D0 = (s32) var_t0;', '                D_801230D0 = D_800B68A8[0][var_v1];').replace('                var_t0 = D_800B68A8[0xA][var_v1];\n                goto block_55;', '                D_801230D0 = D_800B68A8[10][var_v1];\n                break;')
variants={'unshared-stores':clean}
inline=clean
for temp in ('temp_t3','temp_t5','temp_t6','temp_t9'):
    pat=r'        '+temp+r' = ([^;]+);\n        switch \('+temp+r'\)'
    inline=re.sub(pat,r'        switch (\1)',inline)
variants['inline-switch']=inline
variants['signed-byte']=inline.replace('u8 temp_a0;', 's32 temp_a0;')
for key,b in variants.items():
    out=Path('build/T-25049')/key;out.mkdir(exist_ok=True)
    (out/'body.c').write_text(b)
    src=out/'full.c';base=Path(c.src).read_text()
    marker='INCLUDE_ASM("asm/nonmatchings/main/800737A0", func_80073CB0);'
    if marker in base:
        base=base.replace(marker,b)
    else:
        start=base.index('void func_80073CB0(void) {')
        end=base.index('\nu8 func_800741B8',start)
        base=base[:start]+b+'\n'+base[end:]
    src.write_text(base)
    obj,_=v.compile_unit(c,out,source=src)
    got=f.resolved_functions(obj,[name],c)[name]
    print(key,len(a),len(got),f.diff_score(a,got),flush=True)
    (out/'diff.txt').write_text('\n'.join(f.diff_lines(a,got)))
