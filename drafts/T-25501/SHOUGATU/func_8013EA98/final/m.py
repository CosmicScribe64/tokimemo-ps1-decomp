from pathlib import Path
import sys,re,json,subprocess
sys.path.insert(0,'tools')
import funcdiff as fd,funcdiff_verify as fv
name='func_8013EA98'; c=fd.locate(name,'SHOUGATU')
label=sys.argv[1]; d=Path('build/ea98')/label; d.mkdir(parents=True,exist_ok=True)
body=Path(sys.argv[2]).read_text()
source=Path(c.src).read_text()
pat=re.compile(r'#ifdef NON_MATCHING\n(?:(?!#ifdef).)*?#else\nINCLUDE_ASM\("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013E330", func_8013EA98\);\n#endif\n',re.S)
m=pat.search(source)
if m: source=source[:m.start()]+body+'\n'+source[m.end():]
else:
    needle='INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013E330", func_8013EA98);'
    if needle in source: source=source.replace(needle,body)
    else:
        k=source.index('void func_8013EA98(void) {'); e=source.index('\n}\n',k)+3
        source=source[:k]+body+source[e:]
for a in sys.argv[3:]:
    if a.startswith('--pre='): source=Path(a[6:]).read_text()+source
(d/'body.c').write_text(body)
s=d/'unit.c';s.write_text(source)
obj,_=fv.compile_unit(c,d,source=s,non_matching=False)
e,_=fd.expected_object(c);want=fd.resolved_functions(e,[name],c)[name];got=fd.resolved_functions(obj,[name],c)[name]
rows=fd.diff_lines(want,got);(d/'diff.txt').write_text('\n'.join(rows)+'\n')
dump=subprocess.check_output(['mips-linux-gnu-objdump','-drz','--disassemble='+name,obj],text=True);(d/'dis.txt').write_text(dump)
fr=[l.split('\t')[-1] for l in dump.splitlines() if 'addiu\tsp,sp,-' in l]
print(label,'words',len(want),len(got),'rows',len(rows),'cost',fd.diff_score(want,got),fr)
