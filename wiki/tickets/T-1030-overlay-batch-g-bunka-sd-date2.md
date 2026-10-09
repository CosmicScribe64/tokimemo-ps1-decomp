---
id: T-1030
title: "Batch G: overlays BUNKA_SD, DATE2"
status: In Progress
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[overlays]]"]
---

## Goal

Decompile functions in `src/ovl/BUNKA_SD.c` and `src/ovl/DATE2.c` (IDO 5.3 with the frame pass), smallest first, at least 40 matches. Per-overlay externs go in `include/ovl/<NAME>.h`.

## Acceptance criteria

- [ ] At least 40 functions matched (funcdiff MATCH), all 27 sha1 checks OK
- [ ] New failure patterns noted in [[matching-notes]]
- [ ] Code-review gate run inline, findings resolved

## Notes

Reuse C from other `src/ovl/*.c` where an identical function is already matched.

## Comments
