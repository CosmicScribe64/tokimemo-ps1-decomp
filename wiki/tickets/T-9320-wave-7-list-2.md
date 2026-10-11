---
id: T-9320
title: Wave 7 list 2
status: Done
assignee: w7-2
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal
Match as many functions as possible from wave-7 list 2 (36 files, 142 functions) in worktree w7-2.

## Acceptance criteria
- [x] Clean build 27/27 OK, headers OK, globals OK
- [x] Inline code review against CODING_STANDARDS.md recorded below

## Notes
Branch w7-2, not merged.

## Comments

### Result (2026-10-10)
39 functions matched (18116 bytes): EVENT 5, TACO 19, TT 2, TEL 2, SHOUGATU 2, KANGEI 2, GEKO 1, RPG_BAT 1, OLH 1, main 4 (`vload_font`, `func_8004B19C`, `sprite_set_gpu_poly_ft4`, `func_80073198`). Clean rebuild: 27/27 OK, headers OK, globals OK, `sync_protos.py --check-branch` OK; `ninja progress` 4597/6958 (682768 bytes), up from 4558. 29 T-0018 rows added. Notes: [[matching-notes]] "Wave 7, list 2".

### Review (inline, CODING_STANDARDS.md)
- Section 1/7: every trick is marked `FAKE` with a reason and `T-9320`: pads (`func_801338EC`, OLH `func_801345CC`), `(&D_8015EC28)[n]` and `(&D_80127090)[j]` first-symbol views, `(&D_800E9E5F)[-0x44]`, `(&D_8013BF60)[i]`, the permuter burns `^ 0` and `!= 0` (`func_8014B5F0`, `func_8014F9B0`) and the byte-typed argument store idiom. No `NON_MATCHING` code was added.
- Section 8a: main-exe globals went through `sync_protos.py --add-global` (`D_800B3D84`, `D_800EAFB8`, `D_800E9E74`, `D_800B1CF0`, `D_801274D0`, `D_801274EC`, `D_80127ED0`); overlay symbols were appended to `include/ovl/<NAME>.h` (declarations of functions that are now defined in C, table symbols such as `D_8015FEF8`, `D_80160268`). One existing declaration changed: `func_801335A0` in `include/ovl/TACO.h` became `s32 func_801335A0();` (it returns 0 in one path). `MAIN_API_OVERRIDE_*` was not needed.
- Section 4: constants are written as the asm shows them; sign and width views of GameState members use the casts already in use (`*(u8 *)&D_800E6280.unk_110D`).
- Open findings: none.
