---
id: T-10820
title: Wave 16: list 2
status: Done
assignee: wave-16 agent 2
created: 2026-10-12
updated: 2026-10-12
links: []
---

## Goal
Match functions on wave-16 list 2 (40 files, ~60 functions).

## Acceptance criteria
- [x] Every listed function attempted; matches committed
- [x] ninja 27/27 OK, code review recorded

## Notes
Matched (4 functions, 852 bytes): VALEN `func_80132274` (switch dispatch shell, hand copy of GYOZI `func_80138784`), SHOUGATU `func_80136078` (function-pointer table copy plus the T-4090 double-test FAKE), GEKO `func_8013A2EC` (same table shape; the FAKE was found by decomp-permuter, whose score 20 was a real match), RPG_BAT `func_801528DC` (u8 pointer view of the s8 GameState field `unk_110C` and the swapped loop-compare operands).
Near misses kept under `#ifdef NON_MATCHING` with the INCLUDE_ASM fallback: RPG_BAT `func_80153A60` (41 register lines -> 4), TT `func_80145370` (10 -> 4), GEKO `func_80137580` (56 -> 22); older near misses of earlier waves re-tested without improvement: TAIIKU `func_8013AE14`, `func_8013CF88`, `func_80137228`, `func_8013C780`, `func_801368B8`, `func_80136DA4`, TT `func_80147FD0`, BUNKA_SD `func_8013987C`, ETC `func_80143F24`, VALEN `func_80133670`, EVENT `func_8010B614`, main `func_8007A868` (note updated).
Every other function got an m2c draft (scratchpad `w16-2/m2c/`), a build of a cleaned draft where it compiled and a funcdiff; recipes, dead ends and the permuter findings are in [[matching-notes]] ("Wave 16, list 2"), 16 rows in [[data/t0018-cases]]. Not attempted beyond the m2c draft: the three hand-written rotation helpers (DATE `func_8015A7E0`, TACO `func_8015CF30`, `func_8015D270`), the large TT/TAIIKU/MASTER bodies listed under "Loops" and the 600+-byte `func_8013230C`, `make_color_bar`, `func_80042CB0`, `func_80145550`, `func_800764AC`.
Code review (inline, CODING_STANDARDS checklist): matched code is plain C except two FAKE-tagged empty double tests (T-4090/T-10820); table typedefs are file-local, new prototypes (`func_8013A2EC`, `func_8013A668`) are appended to `include/ovl/GEKO.h`, no main-exe symbol declared outside `main_api.h`; every NON_MATCHING block carries T-10820 and the INCLUDE_ASM fallback; no game data committed. `sync_protos --check-branch`, `migrate_globals --check`, `check_headers`, `ninja` (all objects OK, `headers.ok`, `globals.ok`).

## Comments
