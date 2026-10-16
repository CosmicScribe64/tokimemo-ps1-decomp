---
id: T-16500
title: Whole-tree twin sweep after every merge
status: In Review
assignee: claude-tooling
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-16200-matching-wave-2]], [[decompile-workflow]], [[build-system]]
---

## Goal

`tools/dupes.py` and `tools/neardupes.py` copy matched C onto byte-identical and near-identical unmatched twins, but
they were run per package (`--files`), so a donor matched in one package never reached its twin in another. Add
`tools/twin_sweep.py`: one command that plans every exact, near and lenient copy over the whole tree (dry run by
default), and applies only to the C files it is given, each copy verified with the unit sha1 (the existing `--check`).

## Acceptance criteria

- [x] `tools/twin_sweep.py` prints `target <- donor`, mode (exact / near / lenient) and bytes for every planned copy
- [x] `--apply --files <list>` writes only into the listed C files and keeps a copy only when the unit sha1 holds
- [x] known candidates confirmed or explained: TAIIKU func_801406A8, GYOZI func_8013506C, GEKO func_8013E22C, TACO func_801347F4
- [x] unit tests in `tools/test_twin_sweep.py`; full tool test suite passes
- [x] short section in `wiki/decompile-workflow.md`; nothing under `src/` or `drafts/` edited
- [x] one-line command for the orchestrator to run after merges

## Notes

Rules for this ticket: no edits under `src/` (matching agents own the C files), at most 2 compile jobs, no push or merge.

### What was built

- `tools/twin_sweep.py` (tests `tools/test_twin_sweep.py`, 32 tests): loads the asm once, runs the `dupes.py` planner (exact) and the `neardupes.py` planner (near) over every unit, then both again with `--lenient` for the targets still unplanned. Prints `PLAN <mode> <unit>/<target> <- <unit>/<donor>  <bytes> B  <file>  [notes]`, a summary, and with `-v` every skip with its reason. Dry run by default.
- `--apply` needs `--files` (list, `@list.txt`, src paths, or `ALL`) and honours `--exclude`; every copy is verified by the unit sha1 through `dupes.apply_all` (batch per unit and mode, revert, retry one by one); a unit that does not build before the sweep is skipped; HELD plans (jump table or rodata outside the function file) are never applied.
- Small changes in `tools/dupes.py`: plans carry `src_unit`, `build_ok` takes `NINJA_ARGS` (`--jobs`, default 2), `apply_plan` accepts an optional replacement `pattern`.
- Found while applying: a target that is the `#else INCLUDE_ASM` arm of an `#ifdef NON_MATCHING` draft got its copy inside the `#else` arm, which leaves the stale draft and fails `check_headers.py` when the draft's signature differs. The sweep now replaces the whole block (restored when the build rejects the copy); `--keep-drafts` gives the old behaviour.

### Dry run on w2-good (`7f081d79`, 12 s idle, 70 s under load 30)

6958 functions, 5953 matched, 1005 unmatched. 4 planned copies, 944 bytes, in 4 files:

| mode | target | donor | bytes |
| --- | --- | --- | --- |
| exact | GYOZI func_8013506C | SHOUGATU func_8013315C | 172 (data island, 15-line draft) |
| exact | TAIIKU func_801406A8 | main initModelingData_init | 144 (26-line draft) |
| lenient | GEKO func_8013E22C | EVENT func_8010564C | 484 |
| lenient | TACO func_801347F4 | main initModelingData_init | 144 (24-line draft) |

All four known candidates confirmed. Near: 0 plans (`neardupes.py --report` agrees: 1 skip, ETC func_80144AC0). No plan: ETC func_80144AC0, KANGEI func_801370A0, EVENT func_80104C3C (donor body names a symbol outside the relocation sequence, 1712 B) and EVENT func_800FF320 (donor C not found, 708 B). One group has no matched member (BUNKA_SD func_801336AC, func_80136B3C, func_80137CAC, 576 B); one wider-shape twin (GYOZI func_8013E308, 372 B) is not handled.

### Apply check (scratch clone of the worktree, `src/` of the branch untouched)

`twin_sweep.py --apply --files ALL` in a clone: 1 of 4 kept, 144 bytes: **TAIIKU func_801406A8** (the donor C matches once the stale draft is gone). Rejected: GYOZI func_8013506C (sha1 mismatch: the original uses a0 and v1 where the copy gets v1 and v0), GEKO func_8013E22C (sha1 mismatch, EVENT declares `D_801217D0` as `Rec24[]`, GEKO as `u8[]`), TACO func_801347F4 (header conflict: the donor's `void f(s32 *)` against `void f(s32)` in `TACO.h`). Without the draft handling all four were rejected (TAIIKU and TACO by the header check against the draft).

### Command for the orchestrator after merges

`tools/docker.sh python3 tools/twin_sweep.py --apply --files ALL --exclude <C files running agents own>` (drop `--apply --files ALL --exclude ...` for the dry run); see [[decompile-workflow]], "Twin sweep after merges".

Tests: `tools/docker.sh python3 -m unittest discover -s tools -p 'test_*.py'` 1048 tests OK (pytest is not in the image).

## Comments
