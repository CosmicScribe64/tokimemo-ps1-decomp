---
id: T-11550
title: TT: Opus near-miss pass with order_search, autotweak and regdiff
status: Done
assignee: claude (diman8 fork)
created: 2026-10-14
updated: 2026-10-14
links: ["[[tickets/T-11540-tt-order-search-pass]]", "[[decompile-workflow]]", "[[register-cheatsheet]]", "[[data/t0018-cases]]"]
---

## Goal
One Opus agent on the closest guarded TT drafts, using every solver now in `main`: `tools/autotweak.py` (cheat-sheet
edits, order stage T-11200, regdiff hints T-11220), `tools/order_search.py`, `tools/regdiff.py`, and reasoning.
Targets: `func_8013E850` (2 lines), `func_80144A04` (3), `func_8013C1FC` (4), `func_80140624` (9), `func_801392A0` (11),
`func_80142350` (11), `func_801330D8`, `func_80140920`. Bundled with T-11540 in one PR.

## Acceptance criteria

- [x] Each target matched or its guard comment updated
- [x] Inline review against `CODING_STANDARDS.md`
- [x] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results
TT: 206/303 -> 211/303 (44.7% -> 48.0%). About 45 minutes, 277k tokens.
- Matched: `func_8013E850` (972 bytes; nested `for (;;)` loops instead of restart/reset gotos), `func_8013C1FC` (544;
  typed ordering-table index), `func_80140624` (764; indexed table loops sharing the counter), `func_801330D8` (432; no
  `t`/`x` locals), `func_80140920` (924; reloads in case 0x52, `pt` after `vy`, FAKE `vx`/`vy`).
- Improved: `func_80142350` 11 -> 2 lines (autotweak: FAKE `u8 temp` reload moves the constant 1 to a1).
- Unchanged: `func_80144A04` (3 lines), `func_801392A0` (11 lines); causes narrowed, see [[data/t0018-cases]].
- Which tools helped: regdiff hints (`func_801330D8`, `func_80140920`), autotweak (`func_80142350`), hand reasoning with a
  5-second scratch compile loop for the rest; order_search was not needed.

## Review
Inline, against CODING_STANDARDS.md: full rebuild 27/27, `funcdiff --resolve` MATCH for all five, prototype and global
checks clean; new fakematches carry `FAKE` with T-11550 on their line; guard removals checked by `git diff`. No findings.

## Comments
