---
id: T-24202
title: Cloud packet: KANGEI func_8013852C
status: In Review
assignee: claude (cloud session)
created: 2026-10-20
updated: 2026-10-20
links: [[[matching-notes]], [[tickets/T-24054-short-function-count]], [[tickets/T-24122-shugaku-final-3-and-small-overlays-sweep]], src/ovl/KANGEI/80137B90.c]
---

## Goal

Match `KANGEI:func_8013852C` (180 B, `src/ovl/KANGEI/80137B90.c`) in a cloud session from a match packet
(recipe: wiki/cloud-matching.md on branch `cloud/t24200-taco-8013b460`). The guarded draft was at 2 rows.

## Acceptance criteria

- [x] Unchanged file compiles in the cloud to sha1 `0ae36c0a925e1ee093ab51867f0c007aa64fbc2a`.
- [x] func_8013852C matches (plain C) or the guard is updated with the remaining rows.
- [ ] Local session proves the result with the full linked build.

## Notes

Result: not matched, still 2 rows (as1 places `lui $at, %hi(D_80139ADC)` above `addu`). Guard comment updated to T-24202;
compiled object of the file unchanged (sha1 `0ae36c0a...`). Analysis and sweep coverage: [[matching-notes]] (T-24202 section).
In Review: code review against CODING_STANDARDS.md and local linked-build proof still due.

## Comments
