"""T-24511 second bounded source pass: pointer web separation and init order."""
exec(open('drafts/T-24511/probe.py').read().split('for name,body in variants:')[0])
base=fix
variants=[]
old='''        flags = &D_800E6280;
        i = 0;
        available = sp90;
        xs = spC0;
        ys = spA8;
        steps = sp78;'''
new='''        i = 0;
        available = sp90;
        xs = spC0;
        ys = spA8;
        steps = sp78;
        flags = &D_800E6280;'''
variants.append(('init-order',base.replace(old,new)))
# Distinct C objects for the flag-induction web and the ordinary GameState base.
for name,body in [('s32-index',base.replace('    u32 i;', '    s32 i;')),('coordinate-s32',base.replace('    s16 x;', '    s32 x;').replace('    s16 y;', '    s32 y;'))]:
    variants.append((name,body))
# Keep separate page-one pointer lifetime to avoid global spill coupling.
page=base.replace('    s16 *steps;', '    s16 *steps;\n    s16 *xs1;\n    s16 *ys1;\n    s16 *steps1;')
prefix,tail=page.split('    } else {\n        set_kanji_string(-0x80, -0x52',1)
for before,after in [('xs','xs1'),('ys','ys1'),('steps','steps1')]:
    import re
    prefix=re.sub(r'\b'+before+r'\b',after,prefix)
# Correct replacement renamed the declarations too; restore all six declarations.
prefix=prefix.replace('    s16 *xs1;\n    s16 *ys1;\n    s16 *steps1;\n    s16 *xs1;', '    s16 *xs;\n    s16 *ys;\n    s16 *steps;\n    s16 *xs1;')
variants.append(('page-pointer-webs',prefix+'    } else {\n        set_kanji_string(-0x80, -0x52'+tail))
for name,body in variants:
    out=root/name;out.mkdir(exist_ok=True);(out/'body.c').write_text(body)
    full=draft_fixes.fresh_context(dict(func='func_8013D88C',src=str(c.src),scope='TEL:func_8013D88C'),body,out)
    obj=order_search.compile_variant(c,full,out,False)
    got=funcdiff.resolved_functions(obj,['func_8013D88C'])['func_8013D88C']
    (out/'diff.txt').write_text('\n'.join(funcdiff.diff_lines(want,got)))
    print(json.dumps(dict(name=name,words=len(got),cost=funcdiff.diff_score(want,got),rows=len(funcdiff.diff_lines(want,got)))),flush=True)
