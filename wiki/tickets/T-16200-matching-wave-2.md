---
id: T-16200
title: Matching wave 2 (resume wave 1, then 24 new packages)
status: In Progress
assignee: claude-orchestrator
created: 2026-10-15
updated: 2026-10-15
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

- [ ] Every package reviewed, merged or explicitly dropped, with its ticket Done
- [ ] `w2-int` clean 27/27, headers, globals, `sync_protos --check-branch`
- [ ] main updated; blockers collected for the next tooling round

## Comments
