---
id: T-1070
title: "Batch K: overlays NAME_ENT, TT"
status: In Progress
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[overlays]]"]
---

## Goal

Decompile functions in `src/ovl/NAME_ENT.c` and `src/ovl/TT.c` (IDO 5.3 with the frame pass), smallest first, aiming for at least 40 matches. Per-overlay externs and types go in `include/ovl/<NAME>.h`.

## Acceptance criteria

- [ ] Functions matched (funcdiff MATCH), all 27 sha1 checks OK
- [ ] New failure patterns noted in [[matching-notes]]
- [ ] Code-review gate run inline, findings resolved

## Notes

## Comments
