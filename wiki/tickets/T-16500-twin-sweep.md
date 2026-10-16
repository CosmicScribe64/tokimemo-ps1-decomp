---
id: T-16500
title: Whole-tree twin sweep after every merge
status: Done
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
- [x] `--apply --files <list>` writes only into the listed C files (plus the header declarations a copy needs) and keeps a copy only when the unit sha1 holds
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

- 2026-10-16 (review, CODING_STANDARDS.md checklist): diff w2-good...t16500-twin-sweep touches only `tools/` (`twin_sweep.py`, `test_twin_sweep.py`, small changes in `dupes.py` and `neardupes.py`) and wiki pages; nothing under `src/`, `drafts/`, `asm/` or `disc/`; no game data, asm or binaries (the tests use the synthetic repos of `test_dupes.py`/`test_neardupes.py`). Python 3 only, runs through `tools/docker.sh`, docstring with usage and limits on every new function that matters. Checked: (1) tests: `test_twin_sweep.py` 32 OK, whole tool suite 1048 OK (3 skipped), `test_dupes`/`test_neardupes`/`test_check_headers` OK; (2) dry run on the branch reproduces the report: 6958 functions, 4 plans / 944 B (exact 2 / 316 B, lenient 2 / 628 B), 4 functions without a plan, `--apply` without `--files` exits 2, the tree stays clean; (3) apply in a scratch clone with `--apply --files ALL --unit TAIIKU`: `OK TAIIKU: 1 copies`, `applied 1 of 1 copies (144 bytes)`, the 26-line NON_MATCHING block of `func_801406A8` is replaced by the donor C, `check_headers.py` passes, one prototype added to `include/ovl/TAIIKU.h` (clone deleted, branch `src/` untouched); (4) ticket, kanban card and `wiki/log.md` agree, log is append-only. Findings and fixes: (a) the docstring said a run takes minutes (measured 10 s idle, about a minute under load) -> corrected; (b) with `--unit`/`--files` the summary lines "groups without a matched member" and "wider shape" are whole-tree numbers and read as filtered -> labelled "whole tree"; (c) the workflow page said `--apply` never edits "a file" by accident and pointed to "the last section" -> says "C file", names the headers a copy edits (`include/ovl/<NAME>.h`, `include/main_api.h`) and names the section; (d) the acceptance line "writes only into the listed C files" now says "plus the header declarations a copy needs". Not findings, noted: the tool exits 0 when copies are rejected or a unit is skipped (both are printed in the `applied N of M ...` line; the orchestrator should read it); pre-existing duplicate `sources:` keys in the frontmatter of `wiki/decompile-workflow.md` are not part of this ticket. Result: tool works as the ticket says; the real gain on w2-good is TAIIKU `func_801406A8` (144 B), the other three plans need hand work (see Notes). Status Done.

2026-10-16 (orchestrator): merged into `w2-int`; `twin_sweep.py --apply --files ALL` applied 1 of 4 plans,
TAIIKU `func_801406A8` (144 bytes, linked MATCH, 27/27). GEKO `func_8013E22C` and TACO `func_801347F4` were rejected
as predicted; GYOZI `func_8013506C` needs register work.
