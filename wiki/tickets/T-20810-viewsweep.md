---
id: T-20810
title: "viewsweep: exhaustive return-type and callee-view sweep"
status: Done
assignee: tr-viewsweep agent
created: 2026-10-17
updated: 2026-10-17
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]", "[[matching-notes]]", "drafts/T-20810/README.md", "[[tickets/T-16530-proto-override]]", "[[tickets/T-20600-regorder-recipe-sweep]]"]
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

- [x] `tools/viewsweep.py` (CLI in [[decompile-workflow]]), `tools/test_viewsweep.py`, `allow_defined` in
      `tools/protoverride.py`, `edit=` in `tools/funcdiff_verify.py`; nothing under `src/` or `include/` is touched by a sweep.
- [x] Unit tests with synthetic units (no game data): definition and K&R definition rewrite, callee defined in the same
      file, refusal of data, lattice generation and order, budget cut-off, UNTESTED retry with a fake compiler.
- [x] Full tool suite passes (`python3 -m unittest discover -s tools -p 'test_*.py'`).
- [x] First use over the guarded drafts and the wave 3 restore drafts; table logged. The one MATCH is kept as a verified
      patch (`drafts/T-20810/patches/`), not committed to `src/`: the rule "nothing under `src/` or `include/`" won over
      "commit any MATCH as plain C" (checks run with the patch applied: `ninja` sha1 OK, `check_headers`,
      `migrate_globals --check`, `sync_protos --check-branch`; no clean rebuild). Applying it is a follow-up (see Comments).
- [x] [[decompile-workflow]] section, first row 'Do first: run viewsweep' in [[register-cheatsheet]], [[matching-notes]] entry.
- [x] Code review against `CODING_STANDARDS.md` before Done (2026-10-17, see Comments).

## Notes

Worktree `tokimemo-wt/tr-viewsweep`, branch `tr-viewsweep` from `w2-good`. At most 2 parallel compile jobs. Only
`tools/`, `wiki/` and new `drafts/T-20810/` are touched.

- **Design.** `tools/protoverride.py` gains `apply(..., allow_defined=True)` (a defined function's definition head takes the
  declaration's return type, an empty parameter list follows its `()`/`(void)`, a definition with parameters keeps them;
  data is refused in both modes), `definitions()`, `current_view()`, `split_declaration()`. `funcdiff_verify.compile_unit`
  gains `edit=` (callable on the preprocessed text) and `source=` (a scratch copy to preprocess). `tools/viewsweep.py`:
  lattice (own x callee views, callees from the asm `jal` targets, `void` only when no function of the file uses the
  result), ordering by `draft_fixes` hints, `Sweep` (budget, `-j` threads, serial retry, UNTESTED, early stop at 0 rows,
  anchor on the best compiling own edit when the unedited unit does not compile, early end when a compiler error names no
  function of the lattice), scoring like `order_search.make_text_scorer`, literal-byte proof with an isolated-unit
  fallback, recipe (definition, header, guard, callers and their units), `--guarded`, `--targets`, `--drafts`, `--body`
  (with the draft's `declarations.h`), `--src`, `--json`, `--resume`.
- **Rules kept.** Nothing under `src/` or `include/` is edited by the tool or in this branch (the one match is a patch in
  `drafts/T-20810/patches/`, see below); no existing `drafts/` archive touched; at most 2 parallel compile jobs.
- **Tests.** `tools/test_viewsweep.py` (76) and 16 new `AllowDefined` tests in `tools/test_protoverride.py`. Full suite
  `python3 -m unittest discover -s tools -p 'test_*.py'`: 1,281 tests OK (3 skipped).
- **Validation (lever taken out of scratch copies under `build/scratch/`, `--src`).** RPG_BAT `func_80149C5C` 6 compiles,
  TACO `func_8014B8B4` 4, TAIIKU `func_80145370` 5, RPG_BAT `func_8013B5B0` 41, RPG_BAT `func_8013E54C` 2; all reach 0 rows and
  the literal-byte proof.
- **First use (committed tool, `-j 2`).** Guarded drafts: 101 targets, 996 compiles, 846 s, 1 match, 7 improved, 93 no gain,
  0 UNTESTED. Archived bodies: 29 swept (+2 already C or guarded), 157 compiles, 114 s: 16 no gain, 3 improved, 10 failed
  (body does not compile in the tree). Table and JSON in `drafts/T-20810/`. Estimated 2 to 5 new matches; 1 found.
- **The match.** NAME_ENT `func_80134B94` (804 B): `void func_800AE090();` to `s32 func_800AE090();` in the file's
  override, guard removed. With the change applied: `ninja` up to date (NAME_ENT.bin and every other unit sha1 OK),
  `funcdiff.py --resolve` MATCH, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` OK. Not committed
  to `src/`: the ticket rules forbid editing `src/` and `include/`; the patch is `drafts/T-20810/patches/NAME_ENT_func_80134B94.patch`
  (apply, re-run the checks, commit as `T-NNNN: match func_80134B94`). A clean rebuild (`rm -rf build`) was not repeated for it:
  one overlay file differs from the base tree whose build is 27/27.

## Comments

2026-10-17 (agent): In Review. Open for the reviewer: the patch is not applied in `src/` (rules); the
`UNTESTED` handling reports deterministic compile errors too (a `void` view that a use elsewhere in the file rejects is
filtered now, a draft that fails for another reason ends the target as `failed`).

2026-10-17 (review, claude-opus): Done. Checked `w2-good...tr-viewsweep` against `CODING_STANDARDS.md`.
- Scope: only `tools/`, `wiki/` and new `drafts/T-20810/` changed; nothing under `src/`, `include/`, `config/`, `asm/`
  or `disc/`; no game data or binaries (`drafts/` holds names, row counts, edit strings and one source patch).
- Standards: Python 3, module docstrings, paths as arguments, non-zero exit when a target cannot be swept (section 11);
  all new `tools/` files ASCII; no trailing whitespace; all 6 commits start with `T-20810:`. The patch adds no new FAKE
  and keeps a one-line reason in the `MAIN_API_OVERRIDE_` define (8a).
- Tests: full tool suite through `tools/docker.sh`, 1,281 tests OK (3 skipped); `tools/test_viewsweep.py` (76) and the 16
  `AllowDefined` tests in `tools/test_protoverride.py` use synthetic units and a fake compiler.
- Use: `viewsweep.py NAME_ENT:func_80134B94 -j 1` found the match in 3 compiles (10 rows to 0, `s32 func_800AE090();`), ran the
  literal-byte proof (MATCH, 804 bytes) and printed the recipe and the units to re-check; the patch applies cleanly.
- Docs: numbers in `wiki/decompile-workflow.md`, `wiki/matching-notes.md` and `drafts/T-20810/` agree with
  `viewsweep-results.jsonl` (101 guarded: 996 compiles, 1 match, 7 improved, 93 no gain; 29 restores: 157 compiles, 16 no gain,
  3 improved, 10 failed). The "5 of the 7 T-20600 matches (3,772 B)" figure adds up (1,296 + 624 + 720 + 864 + 268).
- Findings: one small, fixed: the acceptance criterion said any MATCH is committed as plain C while the branch keeps it as a
  patch; the criterion now says what was done. No blocking findings.
- Follow-up for the next matcher (not done here, `src/` is outside this ticket): apply
  `drafts/T-20810/patches/NAME_ENT_func_80134B94.patch`, run the clean build and the three checks, commit as
  `T-NNNN: match func_80134B94` (804 B). Until then the conveyor should skip that guarded draft.
