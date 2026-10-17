---
id: T-20620
title: "Scheduling family: as1 ordering"
status: Done
assignee: cg-sched agent
created: 2026-10-16
updated: 2026-10-17
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
- 2026-10-17: `rm -rf build && configure.py && ninja` gave 27 of 27 SHA1 checks OK (agent run); re-checked in the review worktree: `ninja` up to date, `check_headers` headers OK, `sync_protos --check-branch` branch check OK (0 new disagreements, 15 known), `migrate_globals --check` globals OK; grand total 6301/6958 (74.5%). No build pass or tool changed, so the tool test suite was not needed; matched functions changed: 0.
- 2026-10-17: Code review against `CODING_STANDARDS.md` (Opus, merge reviewer), diff `w2-good...cg-sched`. Scope: C changes are `src/ovl/TACO/80149F90.c` (`func_8014B8B4`) and `include/ovl/TACO.h` (prototype `void` to `s32`); the rest is wiki. `busy_files.sh` lists neither file, so no conveyor-owned file was edited; the three owned-file recipes (EVENT `func_8010B614`, GYOZI `func_8013E638`, main `func_80055A38`) stay in [[matching-notes]] for their owners. The one match is plain C (C89, declarations at the block top, `/* */` comments); its `s32`-without-`return` form carries an explaining comment and this ticket, as the sibling `func_8014E5F8` (T-19003), so no `FAKE` is needed and the header now carries the definition's type (no override). No `NON_MATCHING` block added, no build pass changed (section 7a not touched), no game data, asm or binaries in the diff, commits small with ticket id and trailer. Wiki: ticket/kanban/index/log agree, the negative results are recorded with their evidence. Findings: none open. Ticket Done.
