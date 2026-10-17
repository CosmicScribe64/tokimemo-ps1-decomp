---
id: T-20620
title: "Scheduling family: as1 ordering"
status: In Review
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

- [x] Sched/hoist rows of still-unmatched functions collected; the differing instruction pairs extracted with
      `tools/funcdiff.py` and grouped by pattern.
- [x] For each large pattern: the source shape that controls it (statement order, line grouping, argument order,
      temporaries) or a uniform build rule, tested with small scratch compiles; a build rule only with 0 matched
      functions changed (licensing rule, [[toolchain]]).
- [x] Matches in files the wave 3 conveyor does not own, committed one by one; busy-file candidates listed with
      their recipe.
- [x] A T-20620 section in [[matching-notes]], a [[register-cheatsheet]] row per new recipe, rows/Update paragraphs
      in [[data/t0018-cases]].
- [x] Clean rebuild 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch`.

## Notes

Worktree `tokimemo-wt/cg-sched`, branch `cg-sched` (from `w2-good`). Time box about 3 hours of experiments.

## Comments

2026-10-16 (agent): results, details in [[matching-notes]] "T-20620".
- Matched (committed): TACO `func_8014B8B4` (972 bytes), implicit `int` like its sibling `func_8014E5F8`.
- Verified in scratch copies of conveyor-owned files (not edited): EVENT `func_8010B614` (100 bytes), GYOZI
  `func_8013E638` (236), main `func_80055A38` (196). Recipes: byte-view store and byte-view later read for the
  `li a0` / `lui at` order; init statements and loop head on one line for the preheader order.
- The `li`-first family of T-9020: 32 of 37 sites matched by now; 5 left in 4 owned files.
- Order-only near misses with no C form: BUNKASAI `func_80151364`, main `file_checksum_ok`, NAME_ENT `func_8013BB24`,
  main `dialogue_speak_part`, TACO `func_801511C0`, GEKO `func_80136C20`, ETC `func_80146400` (original issues a later
  line's instruction first), VALEN `func_80134030` (original does not hoist the default case's `lui`s).
- No uniform as1 rule: `-O1`, `-O3`, `-p1`, `-g1..3` and flattened `.loc` lines tested through an as1 shim; none fixes
  the near misses without breaking them or the rest. No build pass changed. IDO 4.1's as1 is the untested lead (needs
  the private compilers back).
- Review pending.
