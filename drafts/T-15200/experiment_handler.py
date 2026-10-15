"""T-15200 serial reconstruction probes. Run in Docker; mount original archive at /ref read-only."""
from pathlib import Path
import sys,re,json
sys.path.insert(0,'tools');import bulk_m2c as bulk,autotweak,funcloc,funcdiff,order_search
f='func_80118010';src=Path('src/ovl/EVENT/8010FE10.c');original=src.read_text();base=Path('/ref/drafts/T-14500/EVENT')/f
body=(base/'body.c').read_text();decl=(base/'declarations.h').read_text()
body=body.replace('func_8004A8EC(3, D_800B0860)','func_8004A8EC(3)').replace('func_8004C250(0x34, D_800B0860)','func_8004C250(0x34)')
body=body.replace('D_80124358 == 3','*(s16 *)&D_80124358 == 3').replace('(&D_801243A4)[D_800EECBC]','((s16 *)&D_801243A4)[D_800EECBC]')
body=body.replace('    s16 sp32;', '    EventStateView *g;\n    s16 sp32;').replace('    temp_v0 =', '    g = (EventStateView *)D_800B0860;\n    temp_v0 =',1)
a=body.index('            temp_v1 =');b=body.index('            func_8004C250',a)
body=body[:a]+'''            g->unk_1A4[g->unk_EE6].unk_02 -= 1;
            g->unk_1A4[g->unk_EE6].unk_06 -= 2;
            g->unk_1A4[g->unk_EE6].unk_0A += 5;
'''+body[b:]
body=re.sub(r'    u8 \*temp_v1[^;]*;\n','',body)
for x,y in {'D_800B1746':'g->unk_EE6'}.items():body=body.replace(x,y)
c=funcloc.locate(f,'EVENT');exp,problem=funcdiff.expected_object(c);want=funcdiff.resolved_functions(exp,[f],c)[f]
bodies = {}
failures = []
def score(name,body):
    bodies[name] = body
    if len(sys.argv)>1 and name not in sys.argv[1:]:return

    dest=Path('build/t15200')/name;dest.mkdir(parents=True,exist_ok=True)
    placed=bulk.insert_draft(original,f,body,decl.splitlines());plain=autotweak.rewrite_guard(placed,f,body)
    (dest/'body.c').write_text(body);(dest/'context.c').write_text(plain);(dest/'declarations.h').write_text(decl)
    try:
        obj=order_search.compile_variant(c,plain,dest/'work',False);got=funcdiff.resolved_functions(obj,[f])[f];diff=funcdiff.diff_lines(want,got);n=0 if funcdiff.keys(want)==funcdiff.keys(got) else len(diff)
        (dest/'comparison.txt').write_text('\n'.join(diff)+'\n');print(name,n,flush=True)
    except (SystemExit, ValueError, OSError) as e:
        failures.append(name)
        print(name,e,flush=True)
score('18010-records',body)
# Scalar declaration before address-taken local avoids artificial lifetime/frame placement.
b=body.replace('    EventStateView *g;\n    s16 sp32;\n    s32 temp_v0;','    s32 temp_v0;\n    s16 sp32;\n    EventStateView *g;')
score('18010-scalar-first',b)
# Narrow function result in sibling convention; target masks explicitly and uses temporary chain.
b=b.replace('    s32 temp_v0;','    u8 temp_v0;')
score('18010-byte-result',b)
# Use direct scalar selector loads outside record accesses, as in target.
b=b.replace('g->unk_EE6','D_800B1746')
score('18010-direct-selector',b)
b=body.replace('    s32 temp_v0;', '    u8 temp_v0;')
b=b.replace('    u32 temp_t2;\n','').replace('    u32 temp_t8;\n','').replace('        temp_t8 = temp_v0 & 0x7F;\n','').replace('temp_t8','(temp_v0 & 0x7F)')
b=b.replace('    temp_t2 = func_8002328C(g->unk_EE6) & 0x7F;\n','    temp_v0 = func_8002328C(g->unk_EE6);\n').replace('temp_t2 < 2U','(temp_v0 & 0x7F) < 2U')
score('18010-reused-result',b)
b=b.replace('    EventStateView *g;\n','').replace('    g = (EventStateView *)D_800B0860;\n','').replace('g->','((EventStateView *)D_800B0860)->')
score('18010-inline-state',b)
b=bodies['18010-reused-result'].replace('func_8004C250(0x34)','func_8004C250()')
score('18010-no-phantom',b)
b=b.replace('D_800B0F45','g->pad_6CB[0x1A]').replace('D_800B1AF4','((EventMovieState *)&D_800B1AF0)->pad04').replace('D_800B1AF9','((EventMovieState *)&D_800B1AF0)->unk_09')
b=b.replace('*(u8 *)((unsigned char *)(&D_800B0860[g->unk_EE6]) + (1749))','g->pad_6CB[g->unk_EE6 + 0xA]')
score('18010-all-state',b)
# Direct result compound expression, same as matched siblings using the uchar.
b=b.replace('    u8 temp_v0;','    u32 temp_v0;')
score('18010-all-state-word',b)
b=bodies['18010-all-state']
a=b.index('            g->unk_1A4');z=b.index('            func_8004C250',a)
updates=''
for member,op in [('unk_02','-= 1'),('unk_06','-= 2'),('unk_0A','+= 5')]:
    updates+='            r = (EventStateView *)(D_800B0860 + g->unk_EE6 * 0x34);\n            r->unk_1A4[0].'+member+' '+op+';\n'
r=b[:a]+updates+b[z:]
r=r.replace('        if (((EventMovieState *)&D_800B1AF0)->unk_09 == 1) {','        if (((EventMovieState *)&D_800B1AF0)->unk_09 == 1) {\n            EventStateView *r;\n')
score('18010-record-pointer',r)
# Use one existing pointer variable for all record-relative writes, then restore state base.
updates=updates.replace('r =','g =').replace('r->','g->').replace('g->unk_EE6 *','((EventStateView *)D_800B0860)->unk_EE6 *')
r=b[:a]+updates+'            g = (EventStateView *)D_800B0860;\n'+b[z:]
score('18010-reused-pointer',r)
# Byte pointer spelling of the same typed Rec34 access.
updates=''
for off,op in [(0x1A6,'-= 1'),(0x1AA,'-= 2'),(0x1AE,'+= 5')]:
    updates+='            r = D_800B0860 + g->unk_EE6 * 0x34;\n            *(s16 *)(r + 0x%X) %s;\n'%(off,op)
r=b[:a]+updates+b[z:]
r=r.replace('        if (((EventMovieState *)&D_800B1AF0)->unk_09 == 1) {','        if (((EventMovieState *)&D_800B1AF0)->unk_09 == 1) {\n            u8 *r;\n')
score('18010-byte-pointer',r)
b=bodies['18010-all-state']
for typ in ['u8','s32']:
    r=b.replace('        if (((EventMovieState *)&D_800B1AF0)->unk_09 == 1) {','        if (((EventMovieState *)&D_800B1AF0)->unk_09 == 1) {\n            '+typ+' stride;\n')
    r=r.replace('            func_8004A8EC(3);','            func_8004A8EC(3);\n            stride = sizeof(Rec34);')
    for member,op in [('unk_02','-= 1'),('unk_06','-= 2'),('unk_0A','+= 5')]:
        r=r.replace('g->unk_1A4[g->unk_EE6].'+member+' '+op+';', '((EventStateView *)((u8 *)g + g->unk_EE6 * stride))->unk_1A4[0].'+member+' '+op+';')
    score('18010-stride-'+typ,r)
b=bodies['18010-all-state'].replace('func_8004C250();','func_8004C250(sizeof(Rec34));')
score('18010-stride-call',b)
b=bodies['18010-stride-call']
r=b.replace('g->unk_1A4[g->unk_EE6].','((EventStateView *)((u32)g + g->unk_EE6 * sizeof(Rec34)))->unk_1A4[0].')
score('18010-integer-base',r)
r=b.replace('func_8004C250(sizeof(Rec34));','func_8004C250(sizeof(Rec34), g);')
score('18010-call-base',r)
r=bodies['18010-byte-pointer'].replace('func_8004C250();','func_8004C250(sizeof(Rec34));')
r=r.replace('    EventStateView *g;\n','    u8 *r;\n').replace('            u8 *r;\n','').replace('    g = (EventStateView *)D_800B0860;\n','').replace('g->','((EventStateView *)D_800B0860)->')
score('18010-replace-pointer',r)
# Target reserves v0 for the function result after the early helper calls.
original=original.replace('#include "ovl/EVENT.h"','#define func_80118010 T15200_void_declaration\n#include "ovl/EVENT.h"\n#undef func_80118010')
b=bodies['18010-all-state'].replace('void func_80118010','s32 func_80118010')
score('18010-return-register',b)
b=b.replace('func_8004C250();','func_8004C250(sizeof(Rec34));')
score('18010-return-stride',b)
b=bodies['18010-all-state']
# Separate second callee result has a different lifetime from the initial result byte.
b=b.replace('    u8 temp_v0;','    u8 temp_v0;\n    u32 second;')
start=b.index('    D_80094720 =');b=b[:start]+b[start:].replace('    temp_v0 =','    second =',1).replace('(temp_v0 & 0x7F) < 2U','(second & 0x7F) < 2U')
b=b.replace('    EventStateView *g;\n','').replace('    g = (EventStateView *)D_800B0860;\n','').replace('g->','((EventStateView *)D_800B0860)->')
score('18010-two-results',b)
b=b.replace('func_8004C250();','func_8004C250(sizeof(Rec34));')
score('18010-two-results-stride',b)
original=src.read_text()
b=bodies['18010-all-state']
decl='''typedef struct EventStatsRecord34 {
    /* 0x00 */ GsWord unk_00;
    /* 0x04 */ GsWord unk_04;
    /* 0x08 */ GsWord unk_08;
    /* 0x0C */ u8 unk_0C[0x28];
} EventStatsRecord34; /* size 0x34 */
typedef struct EventStatsStateView {
    /* 0x000 */ u8 unk_000[0x1A4];
    /* 0x1A4 */ EventStatsRecord34 unk_1A4[12];
} EventStatsStateView; /* size 0x414 */
'''
for old,new in [('unk_02','unk_00.h[1]'),('unk_06','unk_04.h[1]'),('unk_0A','unk_08.h[1]')]:b=b.replace('g->unk_1A4[g->unk_EE6].'+old,'((EventStatsStateView *)g)->unk_1A4[g->unk_EE6].'+new)
score('18010-word-pairs',b)
b=b.replace('func_8004C250();','func_8004C250(sizeof(Rec34));')
score('18010-word-pairs-stride',b)

if failures:
    raise SystemExit("Failed probes: " + ", ".join(failures))
