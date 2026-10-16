---
id: T-20600
title: "Register-order family: recipe sweep"
status: In Progress
assignee: claude-opus (cg-regorder)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[data/t0018-cases]]", "[[register-cheatsheet]]", "[[matching-notes]]"]
---

## Goal

Re-apply the source recipes that later solved single register-order gaps across the whole `regorder` family of
[[data/t0018-cases]] (793 rows). Part of getting ahead of the compiler-limited tail estimated in
[[speedup-research-2026-10-16]].

Recipes to sweep, in order of fit (each from a matched function):
- integer-return view of a `void` callee whose call result lands in `$v0` (T-16210, T-20001); prefer the plain header
  type over an override (CODING_STANDARDS 8a);
- reading `GameState` or a global through a local pointer `GameState *g` (T-16920, T-16930);
- `u16` vs `s16` typing of stored constants so a constant joins the later compare's range (T-11840);
- zero-store chain pairs on one line (T-11850);
- a dead read of a selector before the first call (T-11690);
- a natural extra local declared first to fix an 8-byte frame (T-16930);
- data defined in the same unit for a shared `lui` (T-16551).

## Acceptance criteria

- [ ] Target list: functions still `INCLUDE_ASM` or guarded with `regorder` rows, minus files owned by the wave 3
  conveyor, ranked by size and draft closeness.
- [ ] Each target tried for about 15 minutes with the recipes; every match committed in plain C (FAKE marked).
- [ ] Rows / Update paragraphs in [[data/t0018-cases]]; a T-20600 section in [[matching-notes]]; a
  [[register-cheatsheet]] row for any new recipe.
- [ ] Recipes for functions in busy files reported for the conveyor.
- [ ] Clean rebuild 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch`.

## Notes

Worktree `tokimemo-wt/cg-regorder`, branch `cg-regorder`, base `w2-good`. At most 2 parallel compile jobs, no
permuter farm. Never edit a C file listed by the conveyor's busy-files script.

## Comments
