---
id: T-20810
title: "viewsweep: exhaustive return-type and callee-view sweep"
status: In Progress
assignee: tr-viewsweep agent
created: 2026-10-17
updated: 2026-10-17
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]", "[[matching-notes]]", "[[tickets/T-16530-proto-override]]", "[[tickets/T-20600-regorder-recipe-sweep]]"]
---

## Goal

One lever closed 19 of the 102 wave 3 blocker notes: how the file types the function itself (an `s32` definition with
no return, implicit int, so `$v0` is live at the exit) or a callee (K&R `s32 f();` or `void f();`, kept through
`*_OVERRIDE_` guards). Every use was found by hand: `--proto` takes one callee per run and refuses a function the file
defines, a probe that fails on a concurrent `build.ninja` rewrite is silently untested (T-20600), and
`draft_fixes.py` reports `s32-no-return` but never scores it. `tools/viewsweep.py` tries the whole lattice (own type x
every callee view, singles, best-own x callees, pairs) on the full translation unit in memory, ranks it, and prints the
recipe when a variant reaches 0 rows. It is step 1 of the type pass and of every wave 4 matcher.

## Acceptance criteria

- [ ] `tools/viewsweep.py` (CLI in [[decompile-workflow]]), `tools/test_viewsweep.py`, `allow_defined` in
      `tools/protoverride.py`, `edit=` in `tools/funcdiff_verify.py`; nothing under `src/` or `include/` is touched by a sweep.
- [ ] Unit tests with synthetic units (no game data): definition and K&R definition rewrite, callee defined in the same
      file, refusal of data, lattice generation and order, budget cut-off, UNTESTED retry with a fake compiler.
- [ ] Full tool suite passes (`python3 -m unittest discover -s tools -p 'test_*.py'`).
- [ ] First use over the guarded drafts and the wave 3 restore drafts; any MATCH committed as plain C with the checks
      (clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch`); table logged.
- [ ] [[decompile-workflow]] section, first row 'Do first: run viewsweep' in [[register-cheatsheet]], [[matching-notes]] entry.
- [ ] Code review against `CODING_STANDARDS.md` before Done.

## Notes

Worktree `tokimemo-wt/tr-viewsweep`, branch `tr-viewsweep` from `w2-good`. At most 2 parallel compile jobs. Only
`tools/`, `wiki/` and new `drafts/T-20810/` are touched.

## Comments
