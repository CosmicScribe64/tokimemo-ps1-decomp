---
id: T-6070
title: "Wave 4: list 7"
status: Done
assignee: wave4-agent-7
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Match as many as possible of the 127 functions (38952 bytes) of wave 4 list 7, in the files main 80043510, 8004C3F0, 80059710 and the overlays BUNKA_SD, DATE, ENDING, ETC, EVENT, GEKO, GYOZI, KANGEI, OPTION, RPG_BAT, SHOUGATU, TACO, TAIIKU, TT.

## Acceptance criteria

- [x] Every function in the list matched or reverted to INCLUDE_ASM with a note
- [x] `ninja` ends with 27 of 27 OK and build/headers.ok
- [x] Inline review against CODING_STANDARDS.md recorded below

## Notes

Work in worktree w4-7 (branch w4-7). T-0018 cases go to [[data/t0018-cases]].

## Comments

### Result (2026-10-10)
24 of the 127 listed functions matched (5,520 of 38,952 bytes), in 11 files; the rest stay `INCLUDE_ASM` (this list held the leftovers of waves 2 and 3). After `rm -rf asm build; configure.py; ninja`: 27 of 27 sha1 OK, `build/headers.ok`, `build/globals.ok`; `sync_protos.py --check-branch` OK (no new finding), `migrate_globals.py --check` OK. Eleven rows added to [[data/t0018-cases]]. Patterns, near misses and the blockers: [[matching-notes]], section "Wave 4, list 7 (T-6070)".

Matched: RPG_BAT `func_8013D1D0`, `func_8013D290`; TT `func_80142DA8`, `func_80144CA8`; OPTION `func_80132B30`, `func_8013AC30`, `func_801325A0`; ENDING `func_80133C10` and the eight loaders `func_80134EF0`, `func_80135330`, `func_80135660`, `func_80135770`, `func_80135880`, `func_80135990`, `func_80135AA0`, `func_80135BB0`; TACO `func_80137A60`, `func_80137BD8`, `func_80137CBC`, `func_80143574`, `func_80143730`, `func_80143814`, `func_8015B2AC`; main `func_8004500C`.

Main findings: statements on one source line are scheduled as a unit (two `FAKE`s); `u16` parameters in a prototype replace casts and give the original's `lhu` reload of spilled temps (`func_8004435C` in `main_api.h`); `for (i = 0, p = ...; ...)` changes the register order; implicit-int dispatchers need no `return`; GameState members read as scalars in the original cost one register in about 15 functions (not fixable without a `keep` line in `config/migrate_globals.txt`, outside this wave's edit rights).

Fakematches, all marked `FAKE` in the source with this ticket: `r.w = ...; r.h = ...;` on one line (TT `func_80142DA8`); the copy `i = arg0;` (TACO `func_80137CBC`); `s32 pad;` and `p[4] = ...; p[5] = ...;` on one line (TACO `func_80143574`); `s32 pad[7];` (TACO `func_8015B2AC`).

### Inline review against CODING_STANDARDS.md (13)
- Matches verified: every function by `funcdiff.py --resolve` on its own object and by the sha1 of its binary after a clean rebuild (27/27 OK).
- No `NON_MATCHING` blocks added; unmatched functions are plain `INCLUDE_ASM` (every near miss was reverted).
- C89: declarations at block top, `/* */` comments only, no C99 forms; the one `s16 t` style declaration inside a block was not kept.
- Types: fixed-width typedefs; new struct `Arrs4x8` (OPTION) has offsets implied by four `s16[8]` members and a size comment.
- Headers (8a): main-exe symbols added to `include/main_api.h` only (`D_800B5BC8` as `u8`, `func_8004435C` with `u16 arg0..arg3`); overlay externs in `include/ovl/ENDING.h` (49 loader words), `include/ovl/TACO.h` (`D_8015F440[]` and four prototypes of functions defined later in the same file). `tools/check_headers.py` and `sync_protos.py --check-branch` pass; no overrides added.
- Fakematches: listed above, each with a `FAKE` comment, reason and ticket. The `*(s16 *)0x801E2404` form in the eight ENDING loaders is the idiom of the matched twin `func_80134DE0`, not a trick.
- Implicit-int functions without a trailing `return` (`func_80137BD8`, `func_80137A60`, `func_8013AC30`, `func_8004500C`) carry a comment that says why.
- No game data staged; commits are small, each starts with `T-6070:`.
