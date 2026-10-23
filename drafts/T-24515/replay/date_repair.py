"""Repair stale word-width date assignment using existing GsWord bit fields."""
from pathlib import Path
from probe import probe
name='func_80138C1C'
base=Path('build/T-24515/func_80138C1C/known-end-fixed-space/body.c').read_text()
start=base.index('    D_800E6280.unk_0F8 =')
fields='''    ((GsWord *)&D_800E6280.unk_0F8)->date.month = D_8014CC90 + 1;
    ((GsWord *)&D_800E6280.unk_0F8)->date.day = D_8014CC94 + 1;
    ((GsWord *)&D_800E6280.unk_0F8)->date.unk_09 = D_8014CC98 + 1;
    D_800E6280.unk_1BC[0].unk_10.date.month = D_8014CC9C + 1;
    D_800E6280.unk_1BC[0].unk_10.date.day = D_8014CCA0 + 1;
    D_800E6280.unk_1BC[0].unk_10.date.unk_09 = D_8014CCA4 + 1;
}'''
bit=base[:start]+fields
probe(name,'typed-date-stores',bit)
# One reused explicit endpoint below the text buffer models the original end register.
end=bit.replace('    u8 buf[0x18];', '    u8 buf[0x18];\n    u8 *end;')
for sym,length in [('D_8014CC74',6),('D_8014CC7C',6),('D_8014CC84',10)]:
    end=end.replace('    for (p = '+sym+';', '    end = &'+sym+'['+str(length)+'];\n    for (p = '+sym+';')
    end=end.replace('p < &'+sym+'['+str(length)+']', 'p < end').replace('p != &'+sym+'['+str(length)+']', 'p != end')
probe(name,'explicit-ends',end)
