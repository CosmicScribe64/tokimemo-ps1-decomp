---
id: T-24134
title: Wave 2 closer sweep across GEKO (12), OPTION (10), TEL (2), BUNKAKEN (1), and OMIMAI (1)
status: In Progress
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24126
---

## Summary

Attack the remaining 26 actual `INCLUDE_ASM` functions in `GEKO`, `OPTION`, `TEL`, `BUNKAKEN`, and `OMIMAI`:
- **`BUNKAKEN` (1 remaining)**: `func_8013B510` (`src/ovl/BUNKAKEN/8013B510.c`)
- **`OMIMAI` (1 remaining)**: `func_8013388C` (`src/ovl/OMIMAI/801331B0.c`)
- **`TEL` (2 remaining)**: `func_80136724` (`src/ovl/TEL/80132000.c`), `func_8013D88C` (`src/ovl/TEL/8013CCD0.c`)
- **`OPTION` (10 remaining)**: `func_80132000`, `func_801320C0`, `func_80132384`, `func_801326BC`, `func_80132BA8`, `func_80133010` (`src/ovl/OPTION/80132000.c`), `func_80139A30`, `func_80139E3C`, `func_80139F8C` (`src/ovl/OPTION/801389A0.c`), `func_8013A7EC` (`src/ovl/OPTION/8013A550.c`)
- **`GEKO` (12 remaining)**: `func_80132B24`, `func_80133024`, `func_80133308` (`801323E0.c`), `func_80133C50` (`801338F0.c`), `func_80134808` (`80134040.c`), `func_801358CC`, `func_80135DC0`, `func_801367EC`, `func_80136C20` (`80134D10.c`), `func_801420E4` (`80141AF0.c`), `func_80142C9C`, `func_80144198` (`80142760.c`)
