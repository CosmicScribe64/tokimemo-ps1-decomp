---
id: T-1040
title: "Batch H: overlays OPTION, ENDING"
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[overlays]]"]
---

## Goal

Decompile functions in `src/ovl/OPTION.c` and `src/ovl/ENDING.c` (IDO 5.3 with the frame pass), smallest first, aiming for at least 40 matches. Per-overlay externs and types go in `include/ovl/<NAME>.h`.

## Acceptance criteria

- [x] Functions matched (funcdiff MATCH), all 27 sha1 checks OK. Result: 41 (ENDING 29, OPTION 12)
- [x] New failure patterns noted in [[matching-notes]]
- [x] Code-review gate run inline, findings resolved

## Notes

Headers: `include/ovl/OPTION.h`, `include/ovl/ENDING.h` (main-exe names as they appear in the overlay asm). Pad stubs: `src/ovl/pad/pad_ENDING_*.s`. Failure patterns in [[matching-notes]] (section Overlay batch H).

## Comments

- Code review (inline, CODING_STANDARDS section 13 checklist, no sub-agents): matches verified with `funcdiff.py` and `ninja` (27 of 27 sha1 OK); no `NON_MATCHING`, no fakematch, no `FAKE` needed; C89 only (grep for `//`, `inline`, `bool` clean); headers have include guards and fixed-width types; RECT from `include/libgpu.h`, no SDK type redefined; placeholder names kept; no generated asm or game data staged (only `src/ovl/*.c`, `include/ovl/*.h`, `src/ovl/pad/*.s` stubs, wiki). Findings: none open. Spec axis: goal of at least 40 matches met (41).
