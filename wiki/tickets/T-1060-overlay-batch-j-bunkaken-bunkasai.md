---
id: T-1060
title: "Batch J: overlays BUNKAKEN, BUNKASAI"
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[overlays]]"]
---

## Goal

Decompile functions in `src/ovl/BUNKAKEN.c` and `src/ovl/BUNKASAI.c` (IDO 5.3 with the frame pass), smallest first, at least 40 matches. Per-overlay externs go in `include/ovl/<NAME>.h`.

## Acceptance criteria

- [x] At least 40 functions matched, all 27 sha1 checks OK. Result: 77 new (BUNKAKEN 39, BUNKASAI 38; progress 39/201 and 39/207 with one earlier empty stub)
- [x] New failure patterns noted in [[matching-notes]]
- [x] Code-review gate run inline, findings resolved

## Notes

## Comments

- Code review (inline, CODING_STANDARDS section 13 checklist, no sub-agents): matches verified by the linked sha1 (`ninja build/ovl/BUNKAKEN.ok`, `BUNKASAI.ok`, full `ninja` 27 of 27 OK) plus funcdiff for non-pad functions; no `NON_MATCHING`, no fakematch; C89 only (grep for `//`, `inline`, `bool` clean); headers have include guards and fixed-width types; pad stubs contain only `nop`; names are placeholders; no generated asm or game data staged. Findings: none open. Spec axis: at least 40 matched, 77 reached.
