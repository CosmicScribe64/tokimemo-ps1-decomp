"""Causal exit-liveness and expression-order controls on indexed NAME_ENT records."""
from pathlib import Path
from probe import probe
name='func_801398EC';base=Path('build/T-24515/func_801398EC/global-index-loop/body.c').read_text()
ret=base.replace('void func_801398EC(void)', 's32 func_801398EC(void)')
probe(name,'integer-fallthrough',ret,['s32 func_801398EC(void);'])
order=base.replace('(i / 8) * 0x30 + ((D_800E6280.unk_10F8 >> 2) % 3) * 0x10', '((D_800E6280.unk_10F8 >> 2) % 3) * 0x10 + (i / 8) * 0x30')
probe(name,'modulo-first',order)
probe(name,'integer-modulo-first',order.replace('void func_801398EC(void)', 's32 func_801398EC(void)'),['s32 func_801398EC(void);'])
