#!/usr/bin/env python3
"""Replay T-25050 source controls in Docker without changing production C.

Printed counts are resolved diagnostic lines. Literal byte proof and the
affected MAIN checksum are separate commands documented in README.md.
"""
import argparse
from pathlib import Path
import sys, subprocess
sys.path.insert(0,'tools')
import funcdiff
source=Path('src/main/80075320.c').read_text()
start=source.index('s32 func_80075C84(void)')
end=source.index('\ns32 func_80075FA0',start)
current=source[start:end]
body=Path('drafts/T-25050/baseline.c').read_text()
choices=('baseline','reload','intsum','intsum_flag','score_alias','loop_alias','loop_volatile','natural_index','split_init','position_root','position_address','self_store','position_use','late_zero','late_integer','late_branch','late_load','cursor_read','row_pointer','late_store','increment_order','selector_reuse','natural_reload','natural_unsigned','score_assignment','grouped_index','index_last','narrow_counters','record_word','random_local','current','remove_raw_use','integer_alias','remove_tail_store','inline_random','reverse_increments')
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('control',choices=choices)
variant=parser.parse_args().control
if variant=='reload':
    body=body.replace('D_800B6998++;','D_800B6998 = *(volatile u8 *)&D_800B6998 + 1;')
elif variant in ('intsum','intsum_flag','score_alias','loop_alias','loop_volatile','natural_index','split_init','position_root','position_address','self_store','position_use','late_zero','late_integer','late_branch','late_load','cursor_read','row_pointer','late_store','increment_order','selector_reuse','natural_reload','natural_unsigned','score_assignment','grouped_index','index_last','narrow_counters','record_word','random_local'):
    body=body.replace('D_800B6990[D_800B6998]', '*(u8 *)((u32)D_800B6990 + D_800B6998)')
    if variant!='intsum':
        body=body.replace('        switch (', '        if (D_800E6280.unk_F88) {}\n        switch (',1)
    if variant=='score_alias':
        body=body.replace('*(s32 *)&D_800E6280.unk_1BC[i].unk_00', '*(s32 *)(u32)&D_800E6280.unk_1BC[i].unk_00')
    if variant=='loop_alias':
        body=body.replace('k = D_800B6998 + 7;', 'k = *(u8 *)(u32)&D_800B6998 + 7;')
    if variant=='loop_volatile':
        body=body.replace('k = D_800B6998 + 7;', 'k = *(volatile u8 *)&D_800B6998 + 7;')
    if variant=='split_init':
        body=body.replace('k = D_800B6998 + 7;', 'k = D_800B6998;\n                k += 7;')
    if variant in ('natural_index','natural_reload','natural_unsigned','grouped_index'):
        body=body.replace('    u32 k;\n','').replace('                k = D_800B6998 + 7;\n','')
        body=body.replace('j++, k++','j++').replace('k % 7','(D_800B6998 + j + 7) % 7')
        if variant in ('natural_unsigned','grouped_index'):
            body=body.replace('s32 j;', 'u32 j;')
        if variant=='grouped_index':
            body=body.replace('(D_800B6998 + j + 7)', '(D_800B6998 + 7 + j)')
    if variant=='position_root':
        body=body.replace('(u32)D_800B6990 + D_800B6998', '(u32)&D_800B6998 - 8 + D_800B6998')
    if variant=='position_address':
        body=body.replace('        switch (', '        if ((u32)&D_800B6998) {}\n        switch (',1)
    if variant=='self_store':
        body=body.replace('k = D_800B6998 + 7;', 'D_800B6998 = D_800B6998;\n                k = D_800B6998 + 7;')
    if variant=='position_use':
        body=body.replace('k = D_800B6998 + 7;', 'if (D_800B6998) {}\n                k = D_800B6998 + 7;')
    if variant=='late_zero':
        body=body.replace('k = D_800B6998 + 7;', 'k = *((u8 *)&D_800B6998 + ((u32)i < 0)) + 7;')
    if variant=='late_integer':
        body=body.replace('k = D_800B6998 + 7;', 'k = *(u8 *)((u32)&D_800B6998 + ((u32)i < 0)) + 7;')
    if variant in ('late_branch','cursor_read','row_pointer'):
        body=body.replace('k = D_800B6998 + 7;', 'k = D_800B6998;\n                if ((u32)i < 0) { k = 0; }\n                k += 7;')
    if variant=='late_load':
        body=body.replace('k = D_800B6998 + 7;', 'if ((u32)i >= 0) { k = D_800B6998 + 7; }')
    if variant=='cursor_read':
        body=body.replace('j++, k++','j++').replace('k % 7','k++ % 7')
    if variant=='row_pointer':
        body=body.replace('    u32 k;', '    u32 k;\n    u8 *row;')
        body=body.replace('for (j = 0;', 'row = D_800B3778[i];\n                for (j = 0;').replace('j++, k++','j++, k++, row++').replace('D_800B3778[i][j]','*row')
    if variant in ('late_store','increment_order','selector_reuse','natural_reload','natural_unsigned','score_assignment','grouped_index','index_last','narrow_counters','record_word','random_local'):
        body=body.replace('                }\n            }\n        }\n    }\n    return 0;', '                }\n                if ((u32)i < 0) { D_800B6998 = 0; }\n            }\n        }\n    }\n    return 0;')
    if variant in ('increment_order','selector_reuse','score_assignment','index_last','narrow_counters','record_word','random_local'):
        body=body.replace('j++, k++','k++, j++')
    if variant=='selector_reuse':
        body=body.replace('switch (D_800E6280.unk_F88 & 0x70)', 'switch (k = D_800E6280.unk_F88 & 0x70)')
    if variant=='score_assignment':
        score='*(s32 *)&D_800E6280.unk_1BC[i].unk_00'
        body=body.replace(score+' += rand() & 0x7FFF;', score+' = '+score+' + (rand() & 0x7FFF);')
    if variant=='index_last':
        body=body.replace('    s32 i;\n    s32 j;\n    u32 k;', '    u32 k;\n    s32 j;\n    s32 i;')
    if variant=='narrow_counters':
        body=body.replace('s32 i;', 's16 i;').replace('s32 j;', 's16 j;')
    if variant=='record_word':
        body=body.replace('*(s32 *)&D_800E6280.unk_1BC[i].unk_00', '((s32 *)&D_800E6280.unk_1BC[i])[0]')
    if variant=='random_local':
        body=body.replace('    u32 k;', '    u32 k;\n    s32 random;')
        body=body.replace('                        *(s32 *)&D_800E6280.unk_1BC[i].unk_00 += rand() & 0x7FFF;', '                        random = rand();\n                        *(s32 *)&D_800E6280.unk_1BC[i].unk_00 += random & 0x7FFF;')
if variant in ('current','remove_raw_use','integer_alias','remove_tail_store','inline_random','reverse_increments'):
    body=current
    if variant=='remove_raw_use':
        body=body.replace('        if (D_800E6280.unk_F88) {}\n','')
    elif variant=='integer_alias':
        body=body.replace('D_800B6990[D_800B6998]', '*(u8 *)((u32)D_800B6990 + D_800B6998)')
    elif variant=='remove_tail_store':
        body=body.replace('                if ((u32)i < 0) {\n                    D_800B6998 = 0;\n                }\n','')
    elif variant=='inline_random':
        body=body.replace('    s32 random;\n','').replace('                        random = rand();\n','').replace(' += random & 0x7FFF;', ' += rand() & 0x7FFF;')
    elif variant=='reverse_increments':
        body=body.replace('k++, j++','j++, k++')
out=Path('build/T-25050/replay')/variant
out.mkdir(parents=True,exist_ok=True)
(out/'body.c').write_text(body)
(out/'full.c').write_text(source[:start]+body+source[end:])
subprocess.run([sys.executable,'tools/cc.py',str(out/'full.c'),str(out/'full.o'),'ido','5.3'],check=True)
c=funcdiff.locate('func_80075C84','main')
exp,err=funcdiff.expected_object(c)
assert not err,err
v=funcdiff.resolved_verdict(str(out/'full.o'),exp,'func_80075C84',c)
rows=v[1] or []
(out/'diff.txt').write_text('\n'.join(rows))
print(variant,v[0],len(rows),'diagnostic lines; not literal linked proof')
