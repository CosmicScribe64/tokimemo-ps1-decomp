---
id: T-1050
title: "Batch I: overlays KANGEI, SHUGAKU"
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[overlays]]"]
---

## Goal

Decompile functions in `src/ovl/KANGEI.c` and `src/ovl/SHUGAKU.c` (IDO 5.3 with the frame pass), smallest first, aiming for at least 40 matches. Per-overlay externs and types go in `include/ovl/<NAME>.h`.

## Acceptance criteria

- [x] Functions matched (funcdiff MATCH or only a renamed callee symbol), all 27 sha1 checks OK. Result: 105 (KANGEI 49, SHUGAKU 56), goal was at least 40
- [x] New failure patterns noted in [[matching-notes]] (section Overlay batch I)
- [x] Code-review gate run inline, findings resolved

## Notes

Headers: `include/ovl/KANGEI.h`, `include/ovl/SHUGAKU.h`. Callees use the applied main-exe names (`bg_read_sub2`, `load_palette`, `don_wait`, ...), so `funcdiff.py` shows a symbol-name DIFF for calls the overlay asm still spells with the old name; the link is identical (sha1 OK). `ninja progress`: grand total 309/6962 functions, KANGEI 49/152, SHUGAKU 56/182.

## Comments

- Code review (inline, CODING_STANDARDS section 13 checklist, no sub-agents): matches verified with `funcdiff.py` and `ninja` (all overlay sha1 OK, 26 of 26 overlays plus main); no `NON_MATCHING`, no fakematch, no `FAKE` needed; C89 only (grep for `//`, `inline`, `bool` clean); headers have include guards, fixed-width types, struct offsets documented; placeholders kept for unknown names; no generated asm or game data staged (only `src/ovl/*.c`, `include/ovl/*.h`, wiki). Findings: none open. Spec axis: goal of at least 40 matches met (105).
