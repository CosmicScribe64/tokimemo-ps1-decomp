---
id: T-2040
title: "Wave 2: ETC"
status: In Progress
assignee: claude
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[data/t0018-cases]]", "[[overlays]]"]
---

## Goal

Decompile functions in `src/ovl/ETC.c` (IDO 5.3 with the frame pass), smallest first, from `queue.py --files ETC`. Target: at least 60 matches. Externs and types go in `include/ovl/ETC.h`.

## Acceptance criteria

- [x] Functions matched (funcdiff MATCH or, for constant-address loads, equal instruction words and ETC sha1 OK), all 27 sha1 checks OK. Result: 190 new matches, 197 of 387 now C (goal 60)
- [x] T-0018 failures recorded in [[data/t0018-cases]] (4 rows), new patterns in [[matching-notes]] (section Wave 2: ETC)
- [ ] Code-review gate run inline, findings resolved

## Notes

## Comments
