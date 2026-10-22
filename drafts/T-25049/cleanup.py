from pathlib import Path
import sys,re
sys.path.insert(0,'tools')
import funcdiff as f
import funcdiff_verify as v
name='func_80073CB0';c=f.locate(name,'main')
a,err=f.expected_object(c);assert err is None,err
a=f.resolved_functions(a,[name],c)[name]
body=Path("build/T-25049/byte-tail/body.c").read_text()
clean=body
for decl in ("    u8 temp_t3;\n","    u8 temp_t5;\n","    u8 var_t0;\n"):
    clean=clean.replace(decl, "")
variants={"clean-unused":clean,"clean-spill":clean.replace("    s32 sp28;\n", "").replace("    sp28 = var_v1;\n", "")}
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
