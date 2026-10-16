---
id: T-20610
title: "Promotion family: cvt_pass entry rule"
status: In Progress
assignee: claude-opus (cg-promo)
created: 2026-10-16
updated: 2026-10-16
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

- [ ] Promo rows of still-unmatched functions collected and classified (first load relative to calls; register vs
      reload per use).
- [ ] One uniform candidate rule prototyped in a scratch copy of the pass; measured on the targets and on every
      matched function (0 changed matched functions required).
- [ ] Adopted only if it gains at least 3 functions with 0 changed: then tests in `tools/test_cvt_pass.py`, docs in
      [[toolchain]], gained functions matched in plain C (non-busy files). Otherwise a negative result with numbers.
- [ ] T-20610 section in [[matching-notes]]; [[register-cheatsheet]] row for any new recipe.
- [ ] Clean rebuild 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` (and the tool
      tests if `tools/` changed).

## Notes

## Comments
