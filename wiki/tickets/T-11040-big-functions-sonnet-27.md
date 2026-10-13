---
id: T-11040
title: Big functions (Sonnet) 27
status: Done
assignee: sonnet-s27
created: 2026-10-13
updated: 2026-10-13
links: []
---

## Goal
Match big functions in TAIIKU/80138950, main 800674B0, NAME_ENT/80139740 and NAME_ENT/80132000 (53 functions, list astra27-list.txt).

## Acceptance criteria
- [x] ninja 27/27 OK, headers and globals OK
- [x] inline review against CODING_STANDARDS.md

## Notes
Matched 1 function (232 bytes): TAIIKU func_8013AE14 (leftover near miss, `D_80120668 += 0x48`). New guarded near misses: main func_8006B648 (10 lines), TAIIKU func_80139FE4 (register order only, opcodes and frame match), NAME_ENT func_8014231C (28 lines down to 4: eight-record stride-0x44 loop, IDO unrolls it by four). Other attempts (func_80068938, func_8006A044, func_8013C0E4, func_8013B1D8, func_8013A6FC, func_8013CF88, func_8013BB24, func_8013F208, func_8013DF88, func_80135BB4) were reverted; details and idioms in [[matching-notes]] section "T-11040", T-0018 rows in [[data/t0018-cases]]. The TT overlay was not touched. Blocked: main func_80068EC0 needs the overlay-local `D_8012069C` line removed from include/ovl/EVENT.h (a header outside this batch).

Inline review against CODING_STANDARDS.md: the matched function uses existing struct fields and globals; the guarded drafts keep the INCLUDE_ASM fallback and a NON_MATCHING comment with the differing lines; frame padding (`s32 pad[2]` in func_8006B648) is described in the guard comment; no header, tool or config edited; sync_protos --check-branch OK, migrate_globals --check OK, ninja 27/27 OK with headers.ok and globals.ok.

## Comments
