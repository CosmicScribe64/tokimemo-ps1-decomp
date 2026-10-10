---
id: T-2000
title: "Wave 2: EVENT"
status: Done
assignee: claude
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Match as many functions as possible in `src/ovl/EVENT.c` (target at least 60).

## Acceptance criteria

- [x] At least 60 functions matched, verified with `ninja` (27 of 27 OK) and `funcdiff.py`.
- [x] T-0018 cases recorded in `wiki/data/t0018-cases.md`.
- [x] Clean rebuild OK; inline review against CODING_STANDARDS.md recorded below.

## Notes

Result: 421 of 781 EVENT functions matched (360 `INCLUDE_ASM` left, 198 of them blocked by the queue). Method: m2c drafts for every queue function, bulk insert of the assignment and call-only bodies, `ninja` sha1 plus `funcdiff.py` per function to find failures, failed ones reverted. Details in [[matching-notes]] (section Overlay batch EVENT). 11 rows appended to [[data/t0018-cases]]. `ninja progress` grand total 1322/6962 functions, 115096/2279368 bytes (all worktree-local, before merge with the other waves).

## Comments

- 2026-10-09 inline review against CODING_STANDARDS.md: C89 only, no comments added, declarations at block top (m2c output checked: no `var_`/`temp_` left); no `NON_MATCHING`, no `FAKE` needed (raw-address loads `*(s16 *)0x801Cxxxx` are plain C, explained in matching-notes); new externs only in `include/ovl/EVENT.h`, which now includes `game.h` (`tools/check_headers.py` OK, 138 unused declarations removed); K&R prototypes for unknown callees as the existing file; no game data staged (`asm/`, `build/`, `expected/` untracked); clean rebuild `rm -rf asm build; configure; ninja` ends 27 of 27 OK. No open findings.
