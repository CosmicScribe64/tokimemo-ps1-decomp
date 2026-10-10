---
id: T-6020
title: Wave 4: list 2
status: Done
assignee: claude
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Wave 4 batch agent 2: match as many of the 151 listed functions (41284 bytes) as possible in the 34 owned C files (main 8004E500, 80053650, 80061710; overlays BUNKASAI, BUNKA_SD, DATE, DATE2, ENDING, ETC, EVENT, GEKO, GYOZI, KANGEI, NAME_ENT, RPG_BAT, SHOUGATU, SHUGAKU, TACO, TAIIKU, TEL, TT). Work list: scratchpad `wave4-list-2.txt`.

## Acceptance criteria

- [x] Work list processed in order, time-boxed per function (13 of 151 listed functions matched, 2580 bytes).
- [x] Clean build (`rm -rf asm build; configure.py; ninja`): 27 of 27 OK, `build/headers.ok`.
- [x] T-0018 cases appended to [[data/t0018-cases]]; new patterns in [[matching-notes]].
- [x] Inline code review against CODING_STANDARDS recorded below.

## Notes

Result: 13 functions (2580 bytes) turned into C; `ninja progress` grand total 3539 -> 3552 of 6958 (446952 of 2279368 bytes), clean build 27 of 27 OK, `build/headers.ok`, `build/globals.ok`. Method and patterns: [[matching-notes]], section "Wave 4 list 2 (T-6020)". T-0018 rows added to [[data/t0018-cases]]: 6. One `FAKE` (an unused 4-byte local for a spill slot, NAME_ENT `func_80147950`). Tooling bug found: `tools/sync_protos.py --write`/`--fix` drops the comment line above `extern u8 D_800EAFA0[];` in `include/main_api.h` each run (restored by hand).

## Comments

Inline review against CODING_STANDARDS (2026-10-10):
- Matches verified with `funcdiff.py --resolve` against expected objects taken from an unmodified build, and by the full `ninja` (27 of 27 sha1 OK); one early false MATCH (expected object copied after the first edit) was caught by the GYOZI overlay sha1 and the function reverted.
- No `NON_MATCHING` blocks; unmatched functions stay `INCLUDE_ASM` (section 6). C89 only, `/* */` comments.
- Section 8a: new main-exe symbols declared once in `include/main_api.h` (via `sync_protos.py`); `sync_protos.py --check-branch` reports 0 new disagreements; `migrate_globals.py --check` OK; GameState fields written as `D_800E6280.unk_xxx`.
- Section 8: EVENT `func_800F7F44` uses the `Rec34` table element instead of three first-symbol globals; the one `*(s16 *)&D_801207B0` view is a width view of a documented symbol.
- Section 7: the only fakematch is the `s32 unused;` local in NAME_ENT `func_80147950`, marked `FAKE` with reason and ticket.
- Wiki: ticket/kanban match, log entry appended, index updated, T-0018 rows appended (never edited).
