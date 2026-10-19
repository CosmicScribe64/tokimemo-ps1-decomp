---
id: T-24112
title: Close remaining 8 unmatched functions in NAME_ENT/80139740.c
status: Done
priority: high
type: code
created: 2026-10-18
updated: 2026-10-19
depends_on:
  - T-24106
---

## Summary

Attack and close the remaining 8 `INCLUDE_ASM` functions in `src/ovl/NAME_ENT/80139740.c`:
- `NAME_ENT:func_8013AF78` (`score=105`, 296 B, `drafts/T-22023/NAME_ENT/func_8013AF78/body.c`)
- `NAME_ENT:func_8014068C` (`score=191`, 1,240 B, `drafts/T-22023/NAME_ENT/func_8014068C/body.c`)
- `NAME_ENT:func_801398EC` (`score=253`, 492 B, `drafts/T-22023/NAME_ENT/func_801398EC/body.c`)
- `NAME_ENT:func_8013B1D8` (`score=254`, 836 B, `drafts/T-22023/NAME_ENT/func_8013B1D8/body.c`)
- `NAME_ENT:func_8013A9C8` (`score=266`, 732 B, `drafts/T-22023/NAME_ENT/func_8013A9C8/body.c`)
- `NAME_ENT:func_80140F90` (`score=561`, 2,192 B, `drafts/T-20900/NAME_ENT/func_80140F90/body.c`)
- `NAME_ENT:func_80139F30` (`score=577`, 2,712 B, `drafts/T-17900/NAME_ENT/func_80139F30/body.c`)
- `NAME_ENT:func_80141820` (`score=599`, 2,056 B, `drafts/T-20900/NAME_ENT/func_80141820/body.c`)

## Results

- Matched and promoted 6 of 8 functions (+9,568 B linked / +9,636 B asm) in `src/ovl/NAME_ENT/80139740.c`:
  - `NAME_ENT:func_80139F30` (2,712 B)
  - `NAME_ENT:func_8013A9C8` (732 B)
  - `NAME_ENT:func_8013B1D8` (836 B)
  - `NAME_ENT:func_8014068C` (1,172 B linked / 1,240 B asm)
  - `NAME_ENT:func_80140F90` (2,192 B)
  - `NAME_ENT:func_80141820` (1,924 B)
- Code review passed against `CODING_STANDARDS.md`.
