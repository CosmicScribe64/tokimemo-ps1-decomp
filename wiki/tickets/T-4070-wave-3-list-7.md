---
id: T-4070
title: "Wave 3: list 7"
status: Done
assignee: wave3-agent-7
created: 2026-10-09
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Match as many as possible of the 104 functions (24660 bytes) of wave 3 list 7, in the files 80042A00, 8004C3F0, 80053650, 80061710 and the overlays BUNKAKEN, BUNKASAI, DATE, DATE2, EVENT, GEKO, GYOZI, KANGEI, OPTION, RPG_BAT, SHOUGATU, SHUGAKU, TACO, TAIIKU, TT.

## Acceptance criteria

- [x] Every function in the list matched or reverted to INCLUDE_ASM with a note
- [x] `ninja` ends with 27 of 27 OK and build/headers.ok
- [x] Inline review against CODING_STANDARDS.md recorded below

## Notes

Work in worktree w3-7 (branch w3-7). T-0018 cases go to [[data/t0018-cases]].

## Comments

### Result (2026-10-10)
42 of the 104 listed functions matched (7160 of 24660 bytes), in 17 files; the rest stay `INCLUDE_ASM`. After `rm -rf asm build; configure.py; ninja`: 27 of 27 sha1 OK, `build/headers.ok`, `ninja progress` grand total 2847/6958 functions, 312204/2279368 bytes. New patterns and the list of unmatched shapes: [[matching-notes]], section "Wave 3, list 7 (T-4070)". Eight rows added to [[data/t0018-cases]].

Main findings: one base symbol (record or array) for the globals stops IDO's load hoist (six functions); bit-field access reproduces the `sll/bltz` flag test and the `D |= 4` entry (two functions); an extra register local declared first moves the next slot (GYOZI `func_80143E08`).

Fakematches, all marked `FAKE` in the source with this ticket: `s32 pad[2]` in OPTION `func_80134D38`, `func_80134B6C`, `func_80134DD0`, `func_8013539C`, `func_80135034`; `(n ^ 0)` in `func_801324E8`; the repeated mask in SHUGAKU `func_80132D14`; the two no-op `& 0xFF` in main `srn_init`.

### Inline review against CODING_STANDARDS.md (13)
- Matches verified: every function by `funcdiff.py --resolve` on its own object (string relocations only differ for EVENT/SHOUGATU functions with literals) and by the sha1 of its binary; clean rebuild 27/27 OK.
- No `NON_MATCHING` blocks added; unmatched functions are plain `INCLUDE_ASM`.
- C89: declarations at block top, `/* */` comments only (checked the diff for `//`), no C99 forms. The bit-field typedefs are used only because the match needs them (section 8).
- Types: fixed-width typedefs; new structs (`GyoziRec44`, `BitsB14`, the local `FnTblN`) carry offsets or size comments.
- Headers (8a): main-exe symbols were added to `include/main_api.h` only; overlay symbols to `include/ovl/<NAME>.h`; overrides `MAIN_API_OVERRIDE_D_800E6636` (DATE, `s16` array) and `MAIN_API_OVERRIDE_D_80122EC8` (EVENT, 40-entry table) have a reason on the define; `srn_init` got `u32 *` as its second parameter in `main_api.h` (the definition uses it as a pointer). `tools/check_headers.py` passes. Unused declarations from abandoned attempts were removed.
- Fakematches: listed above, each with a `FAKE` comment, reason and ticket.
- Commits: one batch of related matches per commit, `T-4070:` prefix, Co-Authored-By line, only `src/` and `include/` staged (wiki files in the final commit); no game data.
- No findings left open.
