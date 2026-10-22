#!/usr/bin/env python3
"""Causal ENDING scheduling controls; Docker only, writes ignored build output."""
from pathlib import Path
import sys
sys.path.insert(0, 'tools')
import funcdiff
import order_search
name = 'func_8013A2E8'
c = funcdiff.locate(name, 'ENDING')
src = Path(c.src).read_text()
start = src.index('void '+name+'(void)')
end = src.index('\n#else',start)
body = src[start:end]
anchor = '    if (D_800E6280.unk_0F6.b[1] & 1) {'
flag = 'D_800E6280.unk_0F6.b[1]'
literal = '"一流大学"'
variants = {
 'string_unsigned_false': body.replace(anchor,'    if ((u32)'+literal+' < 0) {}\n'+anchor,1),
 'string_high_unsigned_false': body.replace(anchor,'    if (((u32)'+literal+' & 0xFFFF0000U) < 0) {}\n'+anchor,1),
 'flag_unsigned_false': body.replace(anchor,'    if ((u32)'+flag+' < 0) {}\n'+anchor,1),
 'string_then_flag': body.replace(anchor,'    if ((u32)'+literal+' < 0) {}\n    if ((u32)'+flag+' < 0) {}\n'+anchor,1),
 'flag_then_string': body.replace(anchor,'    if ((u32)'+flag+' < 0) {}\n    if ((u32)'+literal+' < 0) {}\n'+anchor,1),
}
ptr = body.replace('s32 pad;', 'char *pad;',1).replace(anchor,'    pad = '+literal+';\n    if ((u32)pad < 0) {}\n'+anchor,1).replace('func_800AE0F0(D_800CA280, '+literal+');','func_800AE0F0(D_800CA280, pad);',1)
variants['pointer_unsigned_false'] = ptr
for label, test in [('pointer_mask_false', '(u32)('+flag+' & 2) < 0'),
                    ('pointer_mask_truth', flag+' & 2')]:
    variants[label] = ptr.replace(anchor,'    if ('+test+') {}\n'+anchor,1)
chain_end = body.index('    top = 0;')
chain_start = body.index(anchor)
chain = body[chain_start:chain_end]
for label, condition in [('loop_flag_false', '(u32)'+flag+' < 0'),
                         ('loop_top_false', 'top < 0'),
                         ('loop_ptr_false', '(u32)'+literal+' < 0')]:
    prefix = '    top = (u32)'+flag+';\n' if label == 'loop_top_false' else ''
    variants[label] = body[:chain_start]+prefix+'    do {\n'+chain+'    } while ('+condition+');\n'+body[chain_end:]
first_arm_end = chain.index('    } else if')
first_arm = chain[:first_arm_end] + '        goto t25056_chain_done;\n    }\n'
remainder = '    if' + chain[first_arm_end+len('    } else if'):]
for label, condition in [('first_loop_mulzero', flag+' * 0'),
                         ('first_loop_unsigned_false', '(u32)'+flag+' < 0'),
                         ('first_loop_andzero', flag+' & 0')]:
    variants[label] = (body[:chain_start]+'    do {\n'+first_arm+
        '    } while ('+condition+');\n'+remainder+
        't25056_chain_done:\n'+body[chain_end:])
for label, latch in [('carrier_loop', ''),
                     ('carrier_loop_identity', '    if (pad) { pad += (u32)'+flag+' < 0; }\n'),
                     ('carrier_loop_flag_identity', '    if (pad) { '+flag+' += (u32)'+flag+' < 0; }\n')]:
    variants[label] = (body[:chain_start]+'    pad = (u32)'+flag+' < 0;\n    do {\n'+first_arm+
        latch+'    } while (pad);\n'+remainder+
        't25056_chain_done:\n'+body[chain_end:])
score = order_search.make_text_scorer(c,name,resolve=True)
for label in sys.argv[1:] or variants:
    text = src[:start] + variants[label] + src[end:]
    work = Path('build/t25056/probes')/label
    work.mkdir(parents=True,exist_ok=True)
    (work/'full.c').write_text(text)
    print(label,score(text,work,label),flush=True)
