---
id: T-20800
title: "Consolidate the per-unit redraft drivers into tools/redraft.py"
status: In Review
assignee: claude-opus (tr-redraft-tool)
created: 2026-10-17
updated: 2026-10-17
links: ["[[decompile-workflow]]", "[[exhaustive-drafts]]", "drafts/T-20800/README.md"]
---

## Goal

The types-first passes (T-16550, T-16560, T-17000 .. T-18800) wrote 21 per-unit drivers
(`drafts/T-<n>/redraft.py`, 17,000 lines, each a copy of the last with a few rewrites added). The register-order
agent found that most archived drafts no longer compile against the current headers. A refresh of every
remaining function needs one tool: `tools/redraft.py`.

## Acceptance criteria

- [x] Every `drafts/T-1[5-9]*/redraft.py` and README read; the union of proven rewrites ported as named variants
  (label fold, pointer-step, actor-offset, matrix-copy, by-value struct args, field-symbols, stack-table struct
  copies, record/array rewrites) plus `tools/draft_fixes.py`.
- [x] `tools/redraft.py UNIT:func ...`, `--unit U`, `--all-unmatched`; `--jobs N` (max 2), TT skipped by default,
  resumable; each variant scored as a full translation unit through `tools/exhaustive_drafts.py`.
- [x] Output `drafts/<TICKET>/<UNIT>/<func>/{body.c,context.c,result.json}`; `result.json` has `scope`, `score`,
  `census_score`, `variant`, `provenance`.
- [x] Unit tests `tools/test_redraft.py` on synthetic input (no game data); full tool suite passes.
- [x] Docs in [[decompile-workflow]].
- [x] Validation on 10 functions in 3 units, scores against their current best drafts.
- [ ] Code review against `CODING_STANDARDS.md` before Done.

## Notes

Worktree `tokimemo-wt/tr-redraft-tool`, branch `tr-redraft-tool`, base `w2-good`. Touches `tools/`, `wiki/` and a new
`drafts/T-20800/` folder only; never `src/`, `include/` or an existing drafts archive. At most 2 parallel compile
jobs.

## Comments

2026-10-17 (claude-opus, tr-redraft-tool): `tools/redraft.py` (driver, scheduler, archive index, finish step),
`tools/redraft_rewrites.py` (the union of the type rewrites, read from the headers of the unit through `Env(unit)`) and
`tools/redraft_extras.py` (the shapes of one overlay: matrix copies, TcPos by value, RECT locals, menu arrays, MASTER
re-roll). All 21 drivers `drafts/T-16550` .. `drafts/T-18800` and their READMEs were read; the 31 functions that exist in
more than one version were compared, and the later variants were merged in (the fold after `fix_shared_hi` of T-17700 as
`typed-late`, `indexed_his` of T-17000, `hi_exact` of T-17300, per-unit record views for EVENT, GYOZI and MASTER, the
display-base walk, the scaled-index temporary of T-18300). Tests: `tools/test_redraft.py`, 95 tests on synthetic headers,
asm and C; the whole tool suite (`unittest discover -s tools -p 'test_*.py'`) passes, 1,279 tests, 3 skipped.
All 1,020 archived best bodies run through the rewrites without an error.

Validation, ten functions in GEKO, DATE2 and main, default variants, `drafts/T-20800/`: nine better than the census and
one equal; against the best archived draft three better (`GEKO:func_80144198` 69 to 62, `GEKO:func_80134228` 16 to 8,
`main:func_80054140` 106 to 48, all `gs-local`), six equal, one worse (`DATE2:func_80136860`, 164 against 188). None at 0. The archived bodies still
compile and score what they did. Opt-in variants and an un-driven unit (RENSYU 3 functions, VALEN not reached by
`--limit`) were run once in scratch: RENSYU `func_80132390` 364 to 337.
Details: [[decompile-workflow]] ("Redraft tool"). Ready for code review against `CODING_STANDARDS.md`.
