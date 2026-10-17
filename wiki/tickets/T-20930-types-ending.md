---
id: T-20930
title: "Targeted type pass: ENDING"
status: In Progress
assignee: agent (ty-ending)
created: 2026-10-17
updated: 2026-10-17
links: ["[[data-types]]", "[[build-step-gaps]]", "[[speedup-research-2026-10-16]]", "[[ending-family-results]]", "[[tickets/T-18300-types-first-ending]]"]
---

## Goal

A second, targeted type pass for ENDING, one of the least-matched overlays. The first types-first pass
(T-18300) left 13 unmatched functions far from a match. Find the shared structures the remaining functions
use (work structs, menu/option records, callback tables, stack tables, bit-fields) from the asm access
patterns and the matched siblings, declare them, move the matched code onto them (byte-identical), and
redraft the unmatched functions with `tools/redraft.py`, keeping the drafts in `drafts/T-20930/`.

Scope: the C files under `src/ovl/ENDING/` and `include/ovl/ENDING.h` (additions only in `include/game.h`).
Not `src/ovl/TT/*` (issue #2) and not main `dialogue_render` in `src/main/8007C030.c` (issue #45).

## Acceptance criteria

- [ ] New ENDING types declared with offset and size comments, matched code uses them, 27/27 sha1 OK.
- [ ] The unmatched ENDING functions redrafted, drafts and `result.json` (scope, score, census_score) in `drafts/T-20930/`.
- [ ] Paired score changes (census or T-18300 draft vs new draft) reported here and in [[ending-family-results]].
- [ ] Types documented in [[data-types]] and [[ending-family-results]].
- [ ] Clean rebuild 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

## Comments
