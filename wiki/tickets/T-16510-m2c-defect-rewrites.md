---
id: T-16510
title: Detect and rewrite recurring m2c draft defects
status: In Progress
assignee: claude
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-16200-matching-wave-2]], [[decompile-workflow]], [[exhaustive-drafts]]
---

## Goal

Agents keep fixing the same m2c output defects by hand: the shared-store `goto` after switch cases, Shift-JIS strings printed as hex escapes, the wrong post-increment compare (`D += 1; if (D == n)`), and `void` drafts whose callers read `$v0` or whose original is implicit-int. Add detectors, and mechanical rewrites where the fix is mechanical, as a post-processing step usable on any draft body (`tools/draft_fixes.py`, optional flag in `tools/m2c.py`).

## Acceptance criteria

- [ ] Detectors for: shared-store goto/label after switch cases, Shift-JIS hex-escape strings, post-increment compare rewrite; report-only: void draft whose callers read `$v0`, s32-without-return candidates (T-16210 rule).
- [ ] Unit tests from real before/after pairs in the notes; full tool test suite passes.
- [ ] Detector hit counts over the `drafts/T-14500` selected bodies.
- [ ] Rewrites applied to the top 30 hits by score, rescored with full-translation-unit scoring (at most 2 jobs); score changes and new zero-score drafts reported; improved alternatives saved under `drafts/T-16510/` with provenance.
- [ ] Short section in `wiki/decompile-workflow.md`.
- [ ] No edits under `src/`, no in-place edits of existing `drafts/` archives.

## Notes

Source fixes: `wiki/matching-notes.md`, T-12701, T-13201, T-15300, T-15400, T-15600, T-16210.

## Comments
