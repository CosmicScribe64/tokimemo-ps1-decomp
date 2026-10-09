---
id: T-1050
title: "Batch I: overlays KANGEI, SHUGAKU"
status: In Progress
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[overlays]]"]
---

## Goal

Decompile functions in `src/ovl/KANGEI.c` and `src/ovl/SHUGAKU.c` (IDO 5.3 with the frame pass), smallest first, aiming for at least 40 matches. Per-overlay externs and types go in `include/ovl/<NAME>.h`.

## Acceptance criteria

- [ ] Functions matched (funcdiff MATCH), all 27 sha1 checks OK
- [ ] New failure patterns noted in [[matching-notes]]
- [ ] Code-review gate run inline, findings resolved

## Notes

## Comments
