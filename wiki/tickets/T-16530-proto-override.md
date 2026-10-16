---
id: T-16530
title: Prototype override option for regdiff and funcdiff
status: In Progress
assignee: agent t16530
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-16200-matching-wave-2]]", "[[decompile-workflow]]", "[[uopt-register-allocation]]", "[[matching-notes]]"]
---

## Goal

An agent matching a function often needs to know whether a callee should be `void`, implicit `int` (K&R `f()`) or `s32 f(s32 a0)`
in the file under test (T-16210: the `RPG_BAT_OVERRIDE_func_8013E7C0` int view that matched two functions). Today that means
copying the C file and the header to `build/scratch/`, editing the copy and running `regdiff.py --src`. Add a repeatable
`--proto 'void func_80012345(s32 a0);'` option to `tools/regdiff.py` and `tools/funcdiff.py` so the prototype is replaced for
one compile without editing a header or a `src/` file.

Also make regdiff's lookup of the traced uopt clearer: when the binary is missing, print where it looked and how to provide
it (`TOKIMEMO_SCRATCH/r9-uopt/variants/trace`, recipe in [[uopt-register-allocation]]).

## Acceptance criteria

- [ ] `--proto DECL` (repeatable) in `tools/regdiff.py` and `tools/funcdiff.py`; the override applies to the compile of the file under test only, no file under `src/` or `include/` is edited.
- [ ] Unit tests for the override plumbing (no game data), full tool test suite passes.
- [ ] A clear message for a missing traced uopt (paths tried, `TOKIMEMO_SCRATCH`, `--uopt`, `REGDIFF_UOPT`, recipe page).
- [ ] Documented in `wiki/decompile-workflow.md` (what it does, how to run, limits).
- [ ] Validated on one real T-16210 case if it can be reproduced without editing `src/`.
- [ ] Code review against `CODING_STANDARDS.md` before Done.

## Notes

- Rules for this ticket: no edits under `src/`; drafts/ archives untouched; at most two parallel compile jobs.

## Comments
