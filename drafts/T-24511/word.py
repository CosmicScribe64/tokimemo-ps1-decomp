"""T-24511 explicit packed-word read placement; reuse existing coordinate lifetime."""
exec(open('drafts/T-24511/probe.py').read().split('for name,body in variants:')[0])
body=fix.replace('    u32 i;', '    s32 i;').replace('    s16 x;', '    s32 x;')
a,b=body.split('                    default:\n',1)
b,tail=b.split('            } else {\n                switch (i)',1)
b=b.replace('                        if (arg2 == D_800E6280.unk_03F)', '                        x = *(u32 *)D_800E6280.unk_F64;\n                        if (arg2 == D_800E6280.unk_03F)')
b=b.replace('                    if (arg2 == D_800E6280.unk_03F)', '                    x = *(u32 *)D_800E6280.unk_F64;\n                    if (arg2 == D_800E6280.unk_03F)')
b=b.replace('((*(u32 *)D_800E6280.unk_F64) <<', '(x <<')
body=a+'                    default:\n'+b+'            } else {\n                switch (i)'+tail
# Reject accidental duplicated capture caused by indentation-substring replacement.
body=body.replace('                        x = *(u32 *)D_800E6280.unk_F64;\n                        x = *(u32 *)D_800E6280.unk_F64;', '                        x = *(u32 *)D_800E6280.unk_F64;')
for name,source in [('word-capture',body),('word-capture-chain',body.replace('                }\n                if (i == 3)', '                } else if (i == 3)'))]:
    out=root/name;out.mkdir(exist_ok=True);(out/'body.c').write_text(source)
    full=draft_fixes.fresh_context(dict(func='func_8013D88C',src=str(c.src),scope='TEL:func_8013D88C'),source,out)
    obj=order_search.compile_variant(c,full,out,False)
    got=funcdiff.resolved_functions(obj,['func_8013D88C'])['func_8013D88C']
    (out/'diff.txt').write_text('\n'.join(funcdiff.diff_lines(want,got)))
    print(json.dumps(dict(name=name,words=len(got),rows=len(funcdiff.diff_lines(want,got)))),flush=True)
