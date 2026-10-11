---
id: T-9140
title: Wave 6: list 4
status: Done
assignee: w6-4
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal
Match as many functions as possible from wave-6 work list 4 (36 files, 158 functions).

## Acceptance criteria
- [x] Clean build 27/27 OK, headers OK
- [x] Inline review against CODING_STANDARDS.md recorded

## Notes
Worktree w6-4 (branch w6-4), not merged.

## Comments

### Result (2026-10-10)
28 functions matched (7,804 bytes) from the list: GYOZI `func_8013B43C`; main `strSync`, `func_8007B8F8`, `func_8007B734`, `func_8007BF04`, `func_80056284`, `func_80054AF4`; OLH 13 help pages; DATE2 `func_80132C60`; EVENT `func_8010A7E0`, `func_8010AAF8`; RPG_BAT `func_8014E1A4`, `func_8013D0D4`; BUNKA_SD `func_80132000`; ENDING `func_8013B9F0`; BUNKASAI `func_8013FD90`. `ninja progress` grand total 4295/6958 (591,468 bytes). 22 rows added to [[data/t0018-cases]]; idioms and the failed experiments in [[matching-notes]], "Wave 6, list 4 (T-9140)". Clean rebuild (`rm -rf asm build; configure; ninja`) 27/27 OK, `build/headers.ok`, `build/globals.ok`; `sync_protos.py --check-branch` and `migrate_globals.py --check` OK. Branch w6-4, not merged.

### Review against CODING_STANDARDS.md (inline, 2026-10-10)
- Match rule: every function was verified with `funcdiff.py --resolve` and by the unit sha1 line of a full `ninja`; no `NON_MATCHING` blocks added (the one that existed, `strSync`, is gone).
- C89: declarations at block top, `/* */` comments only, no C99. Checked in the diff of all 28 bodies; `func_8007BF04` uses an ANSI `s16` parameter like its neighbour `func_8007BE94`.
- Fakematches, all marked `FAKE` with reason and ticket: `func_80056284` (the `D_800E7D16 = 0` store in the `for` init), `func_8013FD90` (`(&D_80122EB8)[1]` to reach `D_80122EBC` through one symbol). Findings: none open; both are listed in [[matching-notes]] for a later real-aggregate fix.
- Declarations (section 8a): new main-exe scalars (`D_80125D64..9C`, `D_80125D14..28`, `D_80125D3A/3C`, `D_800E7D13/15/16`, `D_80094748`, `D_8009476C`, `D_800B0929`) are in `include/main_api.h` once, `sync_protos.py --check` and `check_headers.py` pass; overlay-local ones went to `OLH.h`/`EVENT.h`; `func_8007BF04` is `s16` in the header (type from the definition); the OLH/NAME-style `void` prototypes became `s32` where the definition returns through a bare `return`. `D_800E7D13` is a symbol-file entry (`config/symbol_addrs_main.txt`, main only), documented; `D_8013A430..43C` in `DATE2.h` changed from `u8` to `s16` (the matched stores are `sh`).
- Naming: placeholders kept; GameState fields written as `D_800E6280.unk_XXXX` (no old names, `globals.ok`).
- No game data, `asm/`, `build/`, `expected/` staged; commits small, ticket id first, trailer present.
- Wiki: ticket, kanban, log and index updated; no open findings.
