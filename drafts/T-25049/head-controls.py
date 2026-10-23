from pathlib import Path
import sys,re
sys.path.insert(0,'tools')
import funcdiff as f
import funcdiff_verify as v
name='func_80073CB0';c=f.locate(name,'main')
a,err=f.expected_object(c);assert err is None,err
a=f.resolved_functions(a,[name],c)[name]
body=Path("build/T-25049/inline-switch/body.c").read_text()
expr="D_800E6280.unk_69C[D_800E6280.unk_71C]"
variants={}
variants["pointer-empty"]=body.replace("    temp_a0 =", "    if (&"+expr+") {}\n    temp_a0 =",1)
variants["byte-tail"]=body.replace("    sp28 = var_v1;", "    if (temp_a0) {}\n    sp28 = var_v1;",1)
p=body.replace("    s32 sp28;", "    GsRec69C *record;\n    s32 sp28;").replace("    temp_a0 =", "    record = &"+expr+";\n    temp_a0 =",1)
p=p.replace(expr+".unk_03", "record->unk_03",1).replace(expr+".unk_01", "record->unk_01",1)
variants["explicit-pointer"]=p
variants["pointer-tail"]=p.replace("    sp28 = var_v1;", "    if (record) {}\n    sp28 = var_v1;",1)
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
