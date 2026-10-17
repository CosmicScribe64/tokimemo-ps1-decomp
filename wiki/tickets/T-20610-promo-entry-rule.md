---
id: T-20610
title: "Promotion family: cvt_pass entry rule"
status: Done
assignee: claude-opus (cg-promo)
created: 2026-10-16
updated: 2026-10-17
links: [[[speedup-research-2026-10-16]], [[data/t0018-cases]], [[toolchain]], [[matching-notes]], tools/cvt_pass.py, tools/test_cvt_pass.py]
---

## Goal

Compiler-gap family `promo` of [[data/t0018-cases]]: IDO promotes (or fails to promote) an unsigned narrow global
into a register differently from the original. Known gap: the entry rule of `tools/cvt_pass.py` (T-5010) removes the
widening `CVT` of a `u8`/`u16` global only when the procedure touches it before its first call, branch or label, so a
global first touched after a call keeps the K&R `CVT` and is not promoted (main `func_800726F0`, `func_80072998`,
T-11670). Find one uniform refinement of the entry rule (or a new per-procedure condition) that predicts the
original's choice, or write up the negative result with numbers.

## Acceptance criteria

- [x] Promo rows of still-unmatched functions collected and classified (first load relative to calls; register vs
      reload per use).
- [x] One uniform candidate rule prototyped in a scratch copy of the pass; measured on the targets and on every
      matched function (0 changed matched functions required).
- [x] Adopted only if it gains at least 3 functions with 0 changed: then tests in `tools/test_cvt_pass.py`, docs in
      [[toolchain]], gained functions matched in plain C (non-busy files). Otherwise a negative result with numbers.
      (Negative: nothing adopted, `tools/` unchanged.)
- [x] T-20610 section in [[matching-notes]]; [[register-cheatsheet]] row for any new recipe.
- [x] Clean rebuild 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` (and the tool
      tests if `tools/` changed).

## Notes

Result: negative for the pass, one plain-C recipe. Details and tables: [[matching-notes]], "T-20610".

- Targets: 36 functions with `promo` rows are still `INCLUDE_ASM` (35 without TT), 120 non-TT R/V-flagged functions
  (146,828 bytes); 29 of them have guarded drafts. Most rows are not entry-rule cases (views, frames, `$a1`/`$v0`
  choices of entry-loaded globals).
- Rules (late global = first touched after the entry), procedures whose uopt input changes / matched functions
  changed: no entry condition 567 / not compiled; stored 80 / 14; compared with a non-zero constant 410 / 18; stored
  and compared with a constant 69 / 0 (gains only `func_800726F0`, no guarded draft; 2 drafts worse);
  latest-definition `_constant_first` at least 4 changed. None adopted.
- Census of the original (constant stores to an unsigned global, store register = load register): entry 45 promoted
  against 13; later 51 against 73, by compare kind 30 against 9 (non-zero constants) and 14 against 61 (zero tests).
- main `func_800726F0` matches in plain C under the production pass: zero tests as truth tests plus `s32 sndisp();`.
  `src/main/8006CB30.c` is owned by the wave 3 conveyor, so the recipe is handed over, not applied.
- main `func_80072998`: FAKE dead entry read gives 4 rows (constant-first order of two compares).

## Comments

### 2026-10-16 start and finish (claude-opus, branch `cg-promo`)

Started 22:37 UTC, ended about 00:57 UTC. Two commits (ticket In Progress, results). Negative result for the pass (no `cvt_pass` entry rule adopted, `tools/` unchanged), one plain-C recipe for main `func_800726F0` handed to the wave 3 conveyor because `src/main/8006CB30.c` is busy. Clean rebuild 27/27 SHA1 OK, headers OK, globals OK, `sync_protos --check-branch` OK (0 new, 15 known). Moved to In Review.

### 2026-10-17 code review (claude-opus, against `CODING_STANDARDS.md`, diff `w2-good...cg-promo`)

Scope: eight wiki files only (this ticket, `kanban.md`, `index.md`, `log.md`, `matching-notes.md`, `register-cheatsheet.md`, `toolchain.md`, `data/t0018-cases.md`). No C file, header, `tools/` file, config or draft changed, so the busy-file rule holds (nothing under `src/` touched; `8006CB30.c` was left alone and the recipe is documented for the conveyor), there is no `NON_MATCHING` or `FAKE` code to check, and no build pass changed (section 7a does not apply: the candidate rules were scratch variants, measured, and none adopted; the regression numbers, 0 matched functions changed by the committed state, are recorded in the ticket and in [[matching-notes]] "T-20610"). Matched count: 0. The `func_800726F0` result is a verification on a scratch copy (`funcdiff --built`, linked bytes equal the target), stated as such, not a claimed match in `src/`. No game data, asm listings or binaries added (the wiki quotes only short instruction fragments, as other pages do). Ticket status, kanban column and index line agree (set to Done with this review). The wiki rows are consistent with each other (cheatsheet row, `toolchain.md` note, `t0018-cases` update and table row all state the same numbers).

No findings. Rebuild and checks run by the merge step (clean `configure.py` + `ninja`, 27/27). Done.
