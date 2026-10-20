"""Repair stale NAME_ENT loop lifetimes before allocation tuning."""
from probe import body,probe
name='func_80138C1C';base=body(name)
reuse=base.replace('count','n').replace('    s32 n;\n    s32 n;', '    s32 n;\n    s32 pad;')
probe(name,'reuse-clear-copy-index',reuse)
end=reuse.replace('p != &D_8014CC8E','p != &D_8014CC84[10]')
probe(name,'known-third-end',end)
fixed=end.replace('sp0 = D_800B3644[0];', 'sp0 = *(volatile u8 *)&D_800B3644[0];').replace('sp1 = D_800B3644[1];', 'sp1 = *(volatile u8 *)&D_800B3644[1];')
probe(name,'known-end-fixed-space',fixed)
# Restore target destination-store order: increment before the second store.
ordered=fixed.replace('            D_800E6280.unk_0D4[n * 2 + 1] = p[1];\n            n++;', '            n++;\n            D_800E6280.unk_0D4[n * 2 - 1] = p[1];').replace('            D_800E6280.unk_0DC[n * 2 + 1] = p[1];\n            n++;', '            n++;\n            D_800E6280.unk_0DC[n * 2 - 1] = p[1];').replace('            D_800E6280.unk_0E4[n * 2 + 1] = p[1];\n            n++;', '            n++;\n            D_800E6280.unk_0E4[n * 2 - 1] = p[1];')
probe(name,'preincrement-second-store',ordered)
