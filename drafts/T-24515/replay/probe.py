"""Bounded NAME_ENT source hypotheses; run serially inside Docker."""
from pathlib import Path
import sys,json,re
sys.path.insert(0,'tools')
import autotweak,draft_fixes,funcloc,funcdiff,order_search
ROOT=Path('build/T-24515')
def body(name):
    c=funcloc.locate(name,'NAME_ENT'); text=Path(c.src).read_text()
    g=next(g for g in autotweak.guards(text) if g.func==name)
    a,b=autotweak.definition_span(text,name,g.body,g.else_start)
    return text[a:b]
def probe(name,label,source,protos=()):
    c=funcloc.locate(name,'NAME_ENT'); out=ROOT/name/label;out.mkdir(parents=True,exist_ok=True)
    (out/'body.c').write_text(source)
    full=autotweak.rewrite_guard(Path(c.src).read_text(),name,source)
    if full is None:
        full=draft_fixes.fresh_context(dict(func=name,src=str(c.src),scope='NAME_ENT:'+name),source,out)
    if protos:
        import funcdiff_verify,protoverride
        pre=out/'pre.c';pre.write_text(full)
        full=funcdiff_verify.preprocess_draft(pre,False)
        full,_=protoverride.apply(full,protoverride.parse(protos),allow_defined=True)
    obj=order_search.compile_variant(c,full,out,False)
    expected,problem=funcdiff.expected_object(c)
    assert not problem,problem
    want=funcdiff.resolved_functions(expected,[name],c)[name]
    got=funcdiff.resolved_functions(obj,[name])[name]
    lines=funcdiff.diff_lines(want,got);(out/'diff.txt').write_text('\n'.join(lines))
    result=dict(func=name,label=label,target=len(want),words=len(got),rows=len(lines))
    print(json.dumps(result),flush=True)
    with (ROOT/'results.jsonl').open('a') as f:f.write(json.dumps(result)+'\n')
    return len(lines)
if __name__=='__main__':
    name='func_8013AF78'; base=body(name)
    loop=base.replace('    s32 i;','    s32 *p;').replace('    for (i = 0; i < 3; i++) {\n        sel[i] = 0;\n    }','    p = sel;\n    do {\n        *p++ = 0;\n    } while (p < &sel[3]);')
    variants=[('control',base),('pointer-loop',loop)]
    for width in ['u32','s32']:
        typed=loop
        for v in 'abcd':typed=typed.replace('    u8 '+v+';', '    '+width+' '+v+';')
        variants.append((width+'-locals',typed))
    for label,source in variants:probe(name,label,source)
