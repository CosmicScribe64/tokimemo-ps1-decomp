---
id: T-20800
title: "Consolidate the per-unit redraft drivers into tools/redraft.py"
status: In Progress
assignee: claude-opus (tr-redraft-tool)
created: 2026-10-17
updated: 2026-10-17
links: ["[[decompile-workflow]]", "[[exhaustive-drafts]]"]
---

## Goal

The types-first passes (T-16550, T-16560, T-17000 .. T-18800) wrote 21 per-unit drivers
(`drafts/T-<n>/redraft.py`, 17,000 lines, each a copy of the last with a few rewrites added). The register-order
agent found that most archived drafts no longer compile against the current headers. A refresh of every
remaining function needs one tool: `tools/redraft.py`.

## Acceptance criteria

- [ ] Every `drafts/T-1[5-9]*/redraft.py` and README read; the union of proven rewrites ported as named variants
  (label fold, pointer-step, actor-offset, matrix-copy, by-value struct args, field-symbols, stack-table struct
  copies, record/array rewrites) plus `tools/draft_fixes.py`.
- [ ] `tools/redraft.py UNIT:func ...`, `--unit U`, `--all-unmatched`; `--jobs N` (max 2), TT skipped by default,
  resumable; each variant scored as a full translation unit through `tools/exhaustive_drafts.py`.
- [ ] Output `drafts/<TICKET>/<UNIT>/<func>/{body.c,context.c,result.json}`; `result.json` has `scope`, `score`,
  `census_score`, `variant`, `provenance`.
- [ ] Unit tests `tools/test_redraft.py` on synthetic input (no game data); full tool suite passes.
- [ ] Docs in [[decompile-workflow]].
- [ ] Validation on 10 functions in 3 units, scores against their current best drafts.
- [ ] Code review against `CODING_STANDARDS.md` before Done.

## Notes

Worktree `tokimemo-wt/tr-redraft-tool`, branch `tr-redraft-tool`, base `w2-good`. Touches `tools/`, `wiki/` and a new
`drafts/T-20800/` folder only; never `src/`, `include/` or an existing drafts archive. At most 2 parallel compile
jobs.

## Comments
