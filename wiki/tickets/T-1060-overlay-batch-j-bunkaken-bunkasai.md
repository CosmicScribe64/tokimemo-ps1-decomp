---
id: T-1060
title: "Batch J: overlays BUNKAKEN, BUNKASAI"
status: In Progress
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[overlays]]"]
---

## Goal

Decompile functions in `src/ovl/BUNKAKEN.c` and `src/ovl/BUNKASAI.c` (IDO 5.3 with the frame pass), smallest first, at least 40 matches. Per-overlay externs go in `include/ovl/<NAME>.h`.

## Acceptance criteria

- [ ] At least 40 functions matched (funcdiff MATCH), all 27 sha1 checks OK
- [ ] New failure patterns noted in [[matching-notes]]
- [ ] Code-review gate run inline, findings resolved

## Notes

## Comments
