---
id: T-16530
title: Prototype override option for regdiff and funcdiff
status: Done
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

- [x] `--proto DECL` (repeatable) in `tools/regdiff.py` and `tools/funcdiff.py`; the override applies to the compile of the file under test only, no file under `src/` or `include/` is edited.
- [x] Unit tests for the override plumbing (no game data), full tool test suite passes.
- [x] A clear message for a missing traced uopt (paths tried, `TOKIMEMO_SCRATCH`, `--uopt`, `REGDIFF_UOPT`, recipe page).
- [x] Documented in `wiki/decompile-workflow.md` (what it does, how to run, limits).
- [x] Validated on one real T-16210 case if it can be reproduced without editing `src/`.
- [x] Code review against `CODING_STANDARDS.md` before Done.

## Notes

- Rules for this ticket: no edits under `src/`; drafts/ archives untouched; at most two parallel compile jobs. Nothing under `src/`, `include/` or `drafts/` was changed.
- **Design:** `tools/protoverride.py` works on the preprocessed unit (the text regdiff and funcdiff's `NON_MATCHING` compile already build with `gcc -E -P`): every top-level declaration of the named function is removed and the given one is written where the function was first mentioned (declaration place, or in front of the first caller when the file never declared it). It does not rely on the `*_OVERRIDE_` macros, so it works for any symbol. A small scanner (strings, char literals, K&R definitions, pragmas) finds the top-level statements. Refused: unparsable or non-function text, a function the unit defines, a name that is not in the unit, a declaration that shares a statement with other declarators. `commit_hint` greps `include/` for the `*_OVERRIDE_<symbol>` guard and prints how to keep a matching view (CODING_STANDARDS.md 8a).
- **regdiff:** `--proto` (repeatable) goes to `compile_draft(..., protos=)`; the report prints one `proto override` line per function and, on a MATCH, the keep-it hint. `find_uopt` replaces the inline lookup: order `--uopt`, `$REGDIFF_UOPT`, `/scratch/r9-uopt/variants/trace` (+ `$TOKIMEMO_SCRATCH/...` when the container has it); a given path is never replaced; the missing-binary message lists each path with its source, says when `/scratch` is not mounted, and names the recipe (`wiki/uopt-register-allocation.md`); a non-executable file is reported as such.
- **funcdiff:** `--proto` uses `funcdiff_verify.compile_unit(c, dir, protos, non_matching=)` (new; `compile_guarded` is now a wrapper). `NON_MATCHING` is defined only for a guarded function or `--version c`, so an unguarded file is compiled as the build would; the literal-byte link proof still decides MATCH. `--proto` with `--built` is refused; `check_fresh` is skipped (the production object is not used).
- **Validation on the T-16210 case** (RPG_BAT `func_801454F0`, callee `func_8013E97C`; traced uopt from the session scratch, `TOKIMEMO_SCRATCH` mounted):
  - regdiff, tree as is (file has the int override): MATCH 498/498. `--proto 'void func_8013E97C(s32 a, s32 b, s32 c);'`: 10 instructions differ, `$v0->$v1 x14`, first divergence at draft insn 89 (the counter `D_8015EDB8` in `$v0`), the T-16210 symptom.
  - temp copy `build/scratch/t16530/801454F0_nooverride.c` (define and `s32 func_8013E97C();` deleted, header says `void`): DIFF as above; with `--proto 's32 func_8013E97C();'` (replaces the header's `void ...`): MATCH 498/498.
  - funcdiff `--resolve RPG_BAT:func_801454F0`: baseline MATCH (linked 2000 bytes); `--proto 's32 func_8013E97C();'` MATCH; `--proto 'void ...'` DIFF (rows 89..94, 290...). `main:func_8006EA40 --proto 's32 func_80053418();'` DIFF (T-11670: the original needs `void`); guarded `main:func_80072998 --proto 's32 func_80068938();'` takes the `NON_MATCHING` path, DIFF. Refusals checked: defined function, unknown name, `--built`.
  - Cost: 20 to 60 ms for the rewrite of an 83 to 115 KB unit; regdiff lookup 2.0 s and traced compile 1.1 s with an override (2.8 s and 1.1 s without), funcdiff about 2 s.
- **Sweep of the scanner:** all 443 `src` C files preprocessed: 6,081 function definitions found (equal to `srcscan.defined_functions` on the three K&R-heavy files checked), no stray text between statements, and 1,769 real declarations fed back as `--proto` reproduce the unit unchanged (token for token) in all cases.
- **Tests:** `tools/test_protoverride.py`, 50 tests (parse, rewrite incl. K&R definitions, pragmas, literals, refusals, guard hints, `find_uopt`, regdiff and funcdiff plumbing, `compile_unit`). `pytest` is not in the image: the suite runs as `tools/docker.sh python3 -m unittest discover -s tools -p 'test_*.py'`: 1,066 tests OK (3 skipped) without `TOKIMEMO_SCRATCH`. With the traced uopt mounted three `test_bulk_m2c` tests fail (`ExactRescore.test_without_rescore_it_stays_a_near_miss`, `Pipeline.test_order_stage_flag`, `Pipeline.test_threshold_routes_to_climb`: autotweak's regdiff advisor is on when the binary exists); reproduced at the base commit with my changes stashed, so not caused by this ticket.
- **Follow-ups:** data views (`MAIN_API_OVERRIDE_D_...`) are not covered (function declarations only); `test_bulk_m2c` should isolate itself from `REGDIFF_UOPT`/the scratch mount; funcdiff has no `--src` for a scratch copy (regdiff does).

## Comments

### Code review (2026-10-16)

Reviewed `git diff w2-good...t16530-proto-override` against `CODING_STANDARDS.md` (checklist, section 13).

- **Scope:** 12 files, all under `tools/` and `wiki/`. No file under `src/`, `include/` or `drafts/` changed; no game data, asm, object or binary committed; added text is ASCII with no `//` comments. Four implementer commits plus one review commit, ticket id in every message, `Co-Authored-By` trailer present.
- **Tests:** `tools/docker.sh python3 -m unittest -q tools/test_protoverride.py`: 50 tests OK. Full suite without `TOKIMEMO_SCRATCH`: `python3 -m unittest discover -s tools -p 'test_*.py'` OK (3 skipped, 90 s). Tests use synthetic text only.
- **Tool run on real input** (RPG_BAT `func_801454F0`, callee `func_8013E97C`; worktree stayed clean after every run):
  - `funcdiff.py --resolve ... --proto 's32 func_8013E97C();'`: MATCH, 2000 linked bytes, with the keep-it hint naming `RPG_BAT_OVERRIDE_func_8013E97C`.
  - the same with `--proto 'void func_8013E97C(s32 a, s32 b, s32 c);'`: DIFF at rows 89 to 94 (`lui v1` expected, `lui v0` built), the T-16210 symptom.
  - refusals: a function the file defines (`func_801454F0`), an unknown name, a data declaration, and `--built` each give a clear message and no verdict.
  - `regdiff.py ... --proto 's32 func_8013E97C();'` with the traced uopt mounted: MATCH 498/498; with the void declaration: 10 instructions differ, `$v0->$v1 x14`, first divergence at draft insn 89. The two-option example from `wiki/decompile-workflow.md` runs as written.
  - `regdiff.py` without a scratch mount: exit message lists the path tried with its source, says `/scratch` is not mounted, names the recipe page.
- **Edge cases tried on `protoverride.apply`:** function-pointer parameters, a callee only mentioned in `INCLUDE_ASM(...)` or `#pragma weak`, a string that holds `f(`, a shared declarator statement (refused), trailing text after the declaration (refused). All behave as the docs say.
- **Docs:** the new `wiki/decompile-workflow.md` sections match the code (options, output lines, limits, lookup order); the pointers in `wiki/matching-notes.md` and `wiki/uopt-register-allocation.md` are correct; index, kanban card and log were consistent with status In Review before this review.
- **Finding 1 (fixed, 178b4619):** `tools/test_protoverride.py` imported `argparse` without using it.
- **Notes, no action needed:** (a) the keep-it hint still says to add the `#define` when the file already has the override; harmless. (b) `autotweak.py` has its own uopt lookup that does not use `regdiff.find_uopt` (default path only, same order). (c) `wiki/decompile-workflow.md` front matter has three `sources:` keys; this predates the ticket. (d) A `static` or struct-typed declaration is replaced by the plain `--proto` text; not reachable for real units because a static function is defined in the file and so refused.
- **Follow-ups already in Notes:** the three `test_bulk_m2c` tests that fail only with the traced uopt mounted (they fail the same at the base commit), data views, funcdiff `--src`.

Result: no open findings; ticket moved to Done.
