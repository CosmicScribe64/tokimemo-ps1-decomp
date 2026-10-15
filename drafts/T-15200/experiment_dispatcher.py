"""T-15200 serial reconstruction probes. Run in Docker; mount original archive at /ref read-only."""
from pathlib import Path
import sys,re,json
sys.path.insert(0,'tools')
import bulk_m2c as bulk,autotweak,funcloc,funcdiff,order_search
base=Path('/ref/drafts/T-14500/EVENT/func_801103A0')
out=Path('build/t15200');out.mkdir(parents=True,exist_ok=True)
original=Path('src/ovl/EVENT/8010FE10.c').read_text(); func='func_801103A0'
body=(base/'body.c').read_text(); decl=(base/'declarations.h').read_text()
body=body.replace('void func_801103A0(void)', 's32 func_801103A0(void)').replace('return;', 'return 0;')
body=re.sub(r'func_80011E28\((0x[0-9A-F]+)[^;]*\);',r'func_80011E28(\1);',body)
body=body.replace('func_80011DFC(1);','func_80011DFC();\n    return 1;')
body=body.replace('*(&D_800B0A34 + (D_800B1746 * 0x34))','((EventStateView *)&D_800B0860)->unk_1A4[D_800B1746].pad2E[2]').replace('*(&D_800B0A36 + (D_800B1746 * 0x34))','((EventStateView *)&D_800B0860)->unk_1A4[D_800B1746].pad2E[4]').replace('D_800B0A5E','((EventStateView *)&D_800B0860)->unk_1A4[1].pad23[3]')
body=body.replace('(D_800B0944 & 2)','((EventStateView *)&D_800B0860)->unk_0E4.f.b1').replace('!(D_800B1AE4 & 1)','!((CharFlags *)&D_800B1AE4)->b0')
a=body.index('            var_v0 =');b=body.index('            if ((sp2B < 3U)',a)
body=body[:a]+'''            if (!((D_800B0895 = (D_800B0896 == 12 && D_800B0897 == 10)) ||
                  (D_800B0895 = (D_800B0896 == 12 && D_800B0897 == 8)) ||
                  (D_800B0895 = (D_800B0896 == 12 && D_800B0897 == 7)))) {
                goto block_448;
            }
'''+body[b:]
body=re.sub(r'    u8 var_v0[^;]*;\n','',body)
decl='\n'.join(x for x in decl.splitlines() if not any(s in x for s in ['D_800B0A34','D_800B0A36','D_800B0A5E','D_800B0944']))
c=funcloc.locate(func,'EVENT'); exp,problem=funcdiff.expected_object(c)
if problem:raise RuntimeError(problem)
want=funcdiff.resolved_functions(exp,[func],c)[func]
bodies = {}
failures = []
def score(name,body,decl):
    bodies[name] = body
    if len(sys.argv)>1 and name not in sys.argv[1:]:return

    placed=bulk.insert_draft(original,func,body,decl.splitlines());plain=autotweak.rewrite_guard(placed,func,body)
    plain=plain.replace('#include "ovl/EVENT.h"','#define func_801103A0 T15200_void_declaration\n#include "ovl/EVENT.h"\n#undef func_801103A0')
    dest=out/name;dest.mkdir(exist_ok=True)
    (dest/'body.c').write_text(body);(dest/'declarations.h').write_text(decl);(dest/'context.c').write_text(plain)
    try:
        obj=order_search.compile_variant(c,plain,dest/'work',False);got=funcdiff.resolved_functions(obj,[func])[func]
        diff=funcdiff.diff_lines(want,got);n=0 if funcdiff.keys(want)==funcdiff.keys(got) else len(diff)
        (dest/'comparison.txt').write_text('\n'.join(diff)+'\n');(dest/'result.json').write_text(json.dumps({'score':n,'matching_claim':False})+'\n');print(name,n,flush=True)
    except (SystemExit, ValueError, OSError) as e:
        failures.append(name)
        print(name,e,flush=True)
score('typed-bools',body,decl)
# Consistent state pointer removes per-field type contradictions and exposes aliasing.
gbody=body.replace('    u8 sp2B;','    EventStateView *g;\n    u8 sp2B;').replace('    temp_v0 =','    g = (EventStateView *)&D_800B0860;\n    temp_v0 =',1)
gbody=gbody.replace('((EventStateView *)&D_800B0860)->','g->')
repls={'D_800B0895':'g->unk_035','D_800B0896':'g->unk_036','D_800B0897':'g->unk_037','D_800B095E':'g->unk_0FE','D_800B1746':'g->unk_EE6','D_800B1AF5':'g->unk_1295','D_800B1AF6':'g->unk_1296'}
for addr in range(0x800B0E2A,0x800B0E75):repls['D_%08X'%addr]='g->unk_5CA[0x%X]'%(addr-0x800B0E2A)
for x,y in repls.items():gbody=gbody.replace(x,y)
gdecl='\n'.join(x for x in decl.splitlines() if not any(s in x for s in repls))
score('state-bools',gbody,gdecl)
gbody=gbody.replace('    return 1;\n','')
# Return value remains whatever the no-event helper leaves in v0, as in target.
gbody=gbody.replace('    sp2B = temp_v0 & 0x7F;\n    sp29 = temp_v0 & 0x80;', '    sp29 = temp_v0 & 0x80;\n    sp2B = temp_v0 & 0x7F;')
score('state-mask-order',gbody,gdecl)
# Remove only jumps whose destination is reached by leaving enclosing blocks.
def trim_jumps(text):
    for m in list(re.finditer(r'\bgoto (block_\d+);',text))[::-1]:
        target=re.search(r'^'+m[1]+r':',text[m.end():],re.M)
        if target and re.fullmatch(r'[\s}]*(?:block_\d+:[\s}]*)*',text[m.end():m.end()+target.start()]):text=text[:m.start()]+text[m.end():]
    for lab in re.findall(r'^(block_\d+):',text,re.M):
        if not re.search(r'goto '+lab+r';',text):text=re.sub(r'^'+lab+r':\n','',text,flags=re.M)
    return text
b=trim_jumps(gbody)
score('state-fallthrough',b,gdecl)
# Original's date condition can be one Boolean chain including selector and remaining flags.
a=b.index('        if (D_801243B0 == 8) {');z=b.index('            if ((sp2B < 3U)',a)
# Keep exact evaluation order; independent short-circuit structure is already represented.
# Scalar result first affects observed stack-byte placement without introducing padding.
b=b.replace('    EventStateView *g;\n    u8 sp2B;\n    u8 sp29;\n    s32 temp_v0;', '    s32 temp_v0;\n    u8 sp2B;\n    u8 sp29;\n    EventStateView *g;')
score('state-scalar-layout',b,gdecl)

if failures:
    raise SystemExit("Failed probes: " + ", ".join(failures))
