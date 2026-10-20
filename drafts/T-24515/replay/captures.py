"""Explicit source snapshots for two NAME_ENT small routines."""
from probe import body,probe
name='func_801398EC';base=body(name)
cap=base.replace('    s32 i;','    s32 i;\n    NameRecordF00 *records;\n    u8 selected;\n    u32 frame;')
cap=cap.replace('    tmp = D_8014D0D4 % 3;', '    records = (NameRecordF00 *)D_80123120;\n    selected = D_800E8BEF;\n    frame = D_800E6280.unk_10F8;\n    tmp = D_8014D0D4 % 3;')
cap=cap.replace('((NameRecordF00 *)D_80123120)[D_800E8BEF]', 'records[selected]').replace('((D_800E6280.unk_10F8 >> 2)', '((frame >> 2)')
probe(name,'control',base)
probe(name,'explicit-snapshots',cap)
# A volatile source load fixes its evaluation point, without a volatile local spill.
vcap=cap.replace('selected = D_800E8BEF;', 'selected = *(volatile u8 *)&D_800E8BEF;').replace('frame = D_800E6280.unk_10F8;', 'frame = *(volatile u32 *)&D_800E6280.unk_10F8;')
probe(name,'fixed-read-snapshots',vcap)
name='func_80138C1C';base=body(name)
probe(name,'control',base)
probe(name,'space-second-fixed-read',base.replace('sp1 = D_800B3644[1];', 'sp1 = *(volatile u8 *)&D_800B3644[1];'))
probe(name,'space-both-fixed-read',base.replace('sp0 = D_800B3644[0];', 'sp0 = *(volatile u8 *)&D_800B3644[0];').replace('sp1 = D_800B3644[1];', 'sp1 = *(volatile u8 *)&D_800B3644[1];'))
probe(name,'space-early-dead-read',base.replace('    for (n = 0; n < 8; n++)', '    n = D_800B3644[0];\n    count = D_800B3644[1];\n    for (n = 0; n < 8; n++)',1))
