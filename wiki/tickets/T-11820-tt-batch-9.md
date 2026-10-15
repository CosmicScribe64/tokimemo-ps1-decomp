---
id: T-11820
title: TT batch 9: Opus on never-attempted TT functions, the four skipped drafts and func_801333D4
status: Done
assignee: claude (diman8 fork)
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-11810-tt-batch-8]]", "[[decompile-workflow]]", "[[data/t0018-cases]]"]
---

## Goal
Ninth TT batch (issue CosmicScribe64/tokimemo-ps1-decomp#2), stacked on T-11810 (PR #31). Stays off the wave-1 TT files
(`80133970.c`, `80142D40.c`, `8013C6D0.c`). Per T-11810, no more passes on the closest guarded drafts (their remaining
differences are IDO limits); one Opus pass on:
1. TT functions no agent has attempted (bulk_m2c drafts exist): `func_80149704`, `func_80136830`, `func_8013F3B4`,
   `func_80146000`, `func_801410A0`, `func_80138808`.
2. The four T-11810 pass B did not reach: `func_80142AF0`, `func_8014B274`, `func_801487D0`, `func_80146E10`.
3. `func_801333D4` (m2c cannot draft it: case labels inside else-if arms), structure by hand.

## Acceptance criteria

- [x] Pass done; matches one per commit, near misses guarded with `T-11820`
- [x] Inline review against `CODING_STANDARDS.md`
- [x] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results
TT: 242/303 -> 245/303 (59.1% -> 60.6%). One Opus pass, about 54 minutes, 343k tokens.
- Matched: `func_8013F3B4` (468 bytes; twin of `func_80135870`), `func_801333D4` (804; switch written by hand, the
  existing rodata island was enough, no config change), `func_8014B274` (416; twin of `func_8014B088`).
- Guarded: `func_80149704` (6 instructions), `func_80136830` (17 lines), `func_80146000` (7 instructions).
- Not kept: `func_801410A0` (48 lines), `func_80138808` (145), `func_801487D0` (103). Not reached: `func_80142AF0`,
  `func_80146E10`.
- `include/ovl/TT.h`: new guarded block `OVL_TT_T11820_TABLES` (`D_80155E14`, `D_80155E20`, `D_80155D32`, `D_80155D38`,
  `D_80155D3E`, `D_80152AF0`, `D_80152AF4`).

## Review
Inline, against CODING_STANDARDS.md: full rebuild 27/27; `funcdiff --resolve` MATCH for all three; prototype and global
checks clean; the one new trick (empty `if (q) {}` in the guarded `func_80149704` draft) carries FAKE with T-11820;
header block guarded; wave files, `tools/` and `config/` untouched. No findings.

## Comments
