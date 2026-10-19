---
id: T-24140
title: Wave 3 closer sweep across GEKO (12), OPTION (8), TEL (2), and OMIMAI (1)
status: In Progress
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24134
---

## Summary

Attack the remaining unmatched functions across `GEKO` (12), `OPTION` (8), `TEL` (2), and `OMIMAI` (1):
- `OMIMAI` (1 remaining in `src/ovl/OMIMAI/80132000.c` — closing brings **`OMIMAI` to 100.0% matched!**): `func_8013388C` (test `keepproof.py` on `D_800E71E8` / `D_800E71DF` and `<expr> * 0` `ugen` subtree discard!)
- `OPTION` (8 remaining): `func_80132384` (2-instruction gap in `80132000.c` — test occupying `$s5..$s8` or `((i << 2) + i) * 4 - 0x58` so `ldc 20` is not hoisted to `$s5`!), `func_80132BA8`, `func_8013488C`, `func_80135DD8`, `func_8013827C` (`80132000.c`), `func_80139A30`, `func_80139E3C`, `func_80139F8C` (`801389A0.c`)
- `TEL` (2 remaining — closing both brings **`TEL` to 100.0% matched!**): `func_80136724` (`80132000.c`), `func_80139E3C` (`801389D0.c`)
- `GEKO` (12 remaining): `func_80132B24`, `func_80133024`, `func_80133308` (`801323E0.c`), `func_80133C50` (`801338F0.c`), `func_80134808` (`801341B0.c`), `func_801367EC`, `func_80136C20` (`80134D10.c`), `func_80139398` (`80137E10.c`), `func_8013F894` (`8013DC70.c`), `func_801406CC` (`80140190.c`), `func_80142C9C` (`80142C20.c`), `func_80144198` (`80143D30.c`)
