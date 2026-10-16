---
id: T-16200
title: Matching wave 2 (resume wave 1, then 24 new packages)
status: Done
assignee: claude-orchestrator
created: 2026-10-15
updated: 2026-10-16
links: [[stopping-point-2026-10-15]], [[exhaustive-drafts]], [[matching-breakthroughs]]
---

## Goal

Match as many of the remaining game functions as possible, starting from the T-14500 compiling-draft census.
Baseline main `868c879a`: 5,945/6,958 functions, 1,452,192/2,279,368 bytes (63.7%). TT is excluded (diman8, issue #2).

## Plan

- Integration branch `w2-int` (worktree `../tokimemo-wt/w2-int`), ref `w2-good` = last verified merge.
- 24 new file-disjoint packages (T-16210..T-16440, branches `w2-p01`..`w2-p24`), ranked by expected matched bytes per
  agent-hour from census draft scores and guarded-draft notes; 882 non-TT functions / 739,240 bytes in 128 packages total.
- Four wave-1 packages resumed (`w2-r08`, `w2-r09`, `w2-r11`, `w2-r13`; tickets T-11640, T-11650, T-11670, T-11690):
  merge the old branch (about 20 committed matches) onto the new main, then continue.
- One Opus matcher per package in its own worktree, commit after every match; a Sonnet agent reviews each branch
  against CODING_STANDARDS.md and merges it into `w2-int` with a clean 27/27 rebuild, then pushes fast-forward to main.
- A usage gate checks the plan limits before each package starts.

## Acceptance criteria

- [x] Every package reviewed, merged or explicitly dropped, with its ticket Done
- [x] `w2-int` clean 27/27, headers, globals, `sync_protos --check-branch`
- [x] main updated; blockers collected for the next tooling round

## Comments

### 2026-10-16: wave closed early, results

User decision during the run: switch matching to Sonnet first with an Opus closer for near misses, and close this
wave early (let running packages finish, start nothing new). The setup script refused the 21 packages that had not
started (`w2-p04`..`w2-p24`); no branch, worktree or ticket exists for them, and they return to the next inventory.

Seven packages merged into `w2-int` and pushed to main, each reviewed against CODING_STANDARDS.md by the merge agent
(tickets Done): T-11640 (10 new + 6 wave-1 matches), T-11650 (8 + 8), T-11670 (1 + 3), T-11690 (1 + 2),
T-16210 (5 of 5, 5,072 bytes), T-16220 (3), T-16230 (4 of 4, 1,620 bytes). Wave-2 matches: 37 functions / 24,352
bytes; with the resumed wave-1 work the total moved from 5,945/6,958 (63.7%) to 5,995/6,958, 1,483,916/2,279,368
bytes (65.1%). Final check on `w2-int` 9a960bdf: clean rebuild 27/27, headers OK, globals OK, `sync_protos
--check-branch` 0 new / 15 known. Cheat sheet: 29 tricks folded in ([[register-cheatsheet]]).

Blockers reported (input for the tooling round T-16500..T-16530 and later rounds):
- Splat scalars hide aggregates: a scalar that is record N of a known stride table (D_80121FD4 = Rec24 #57,
  D_801206D8 = display record 98). type_recovery could flag addresses on a known stride (T-16230).
- Overlay-local prototype overrides (implicit-int vs void callers, counter in $v1 vs $v0) need a supported convention
  in check_headers/sync_protos (T-16210, T-11670); regdiff/funcdiff override option is T-16530.
- Frame homes: originals often have 1-2 more stack homes; only FAKE unused locals reproduce them. A tool that proposes
  home and declaration layouts would help (T-11650, T-11640).
- Chain-assignment store order is not right to left; order_search permutes whole lines only (T-16230).
- autotweak lacks a "GameState *g pointer" edit (T-16230); regdiff should flag ugen moving a spilled reload into an
  argument register, and ranges skipping v0 next to an unused call result (T-16220, T-11670).
- A fast single-function compile harness (under 1 s) would let agents brute-force as1 scheduling (T-16220).
- Known compiler-side gaps unchanged: cvt_pass entry rule for u8 globals first touched after a call; copy
  propagation of loop-invariant byte copies; 4-trip inner loop unroll by 2 with multu (T-11640, T-11650, T-11670).

Research during the wave: [[speedup-research-2026-10-16]] (compiler reverse engineering; faster source recovery).
