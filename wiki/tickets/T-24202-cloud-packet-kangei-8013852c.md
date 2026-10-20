---
id: T-24202
title: Cloud packet: KANGEI func_8013852C
status: In Progress
assignee: claude (cloud session)
created: 2026-10-20
updated: 2026-10-20
links: [[[matching-notes]], [[tickets/T-24054-short-function-count]], [[tickets/T-24122-shugaku-final-3-and-small-overlays-sweep]], src/ovl/KANGEI/80137B90.c]
---

## Goal

Match `KANGEI:func_8013852C` (180 B, `src/ovl/KANGEI/80137B90.c`) in a cloud session from a match packet
(recipe: wiki/cloud-matching.md on branch `cloud/t24200-taco-8013b460`). The guarded draft was at 2 rows.

## Acceptance criteria

- [ ] Unchanged file compiles in the cloud to sha1 `0ae36c0a925e1ee093ab51867f0c007aa64fbc2a`.
- [ ] func_8013852C matches (plain C) or the guard is updated with the remaining rows.
- [ ] Local session proves the result with the full linked build.

## Notes

## Comments
