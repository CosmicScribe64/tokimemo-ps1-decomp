---
id: T-20620
title: "Scheduling family: as1 ordering"
status: In Progress
assignee: cg-sched agent
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[data/t0018-cases]]", "[[matching-notes]]", "[[register-cheatsheet]]", "[[toolchain]]"]
---

## Goal

Get ahead of the compiler-limited tail for one family: instruction order that as1 (the assembler's scheduler) or
ugen produces differently from the original. Rows `sched`/`schedule` (79) and `hoist` (22) in [[data/t0018-cases]],
for example the `li a0, imm` before `lui at, hi` order at 37 known sites (T-12090, T-12500), constants emitted
first in a block (TT `func_8014A2B4`), and the sprite load/store order (main `dialogue_speak_part`).

## Acceptance criteria

- [ ] Sched/hoist rows of still-unmatched functions collected; the differing instruction pairs extracted with
      `tools/funcdiff.py` and grouped by pattern.
- [ ] For each large pattern: the source shape that controls it (statement order, line grouping, argument order,
      temporaries) or a uniform build rule, tested with small scratch compiles; a build rule only with 0 matched
      functions changed (licensing rule, [[toolchain]]).
- [ ] Matches in files the wave 3 conveyor does not own, committed one by one; busy-file candidates listed with
      their recipe.
- [ ] A T-20620 section in [[matching-notes]], a [[register-cheatsheet]] row per new recipe, rows/Update paragraphs
      in [[data/t0018-cases]].
- [ ] Clean rebuild 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch`.

## Notes

Worktree `tokimemo-wt/cg-sched`, branch `cg-sched` (from `w2-good`). Time box about 3 hours of experiments.

## Comments
