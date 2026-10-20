"""T-24511 frame slot topology probes, no declaration-order grid."""
exec(open('drafts/T-24511/probe.py').read().split('for name,body in variants:')[0])
arrays='''    s16 spC0[12];
    s16 spA8[12];
    s16 sp90[12];
    s16 sp78[12];
'''
top=fix.replace(arrays,'').replace('    u32 i;',arrays+'    u32 i;')
variants=[('arrays-first',top),('arrays-first-reuse',top.replace('    s32 byteOffset;\n','').replace('byteOffset','arg1'))]
for name,body in variants:
    out=root/name;out.mkdir(exist_ok=True);(out/'body.c').write_text(body)
    full=draft_fixes.fresh_context(dict(func='func_8013D88C',src=str(c.src),scope='TEL:func_8013D88C'),body,out)
    obj=order_search.compile_variant(c,full,out,False)
    got=funcdiff.resolved_functions(obj,['func_8013D88C'])['func_8013D88C']
    (out/'diff.txt').write_text('\n'.join(funcdiff.diff_lines(want,got)))
    print(json.dumps(dict(name=name,words=len(got),cost=funcdiff.diff_score(want,got),rows=len(funcdiff.diff_lines(want,got)))),flush=True)
