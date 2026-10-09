---
id: T-1070
title: "Batch K: overlays NAME_ENT, TT"
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[overlays]]"]
---

## Goal

Decompile functions in `src/ovl/NAME_ENT.c` and `src/ovl/TT.c` (IDO 5.3 with the frame pass), smallest first, aiming for at least 40 matches. Per-overlay externs and types go in `include/ovl/<NAME>.h`.

## Acceptance criteria

- [x] Functions matched (funcdiff MATCH), all 27 sha1 checks OK. Result: 76 (NAME_ENT 53, TT 23); goal was at least 40
- [x] New failure patterns noted in [[matching-notes]]
- [x] Code-review gate run inline, findings resolved

## Notes

Headers: `include/ovl/NAME_ENT.h`, `include/ovl/TT.h` (externs and prototypes use the old main-exe names; no O.BIN rename applies to the callees used). New patterns in [[matching-notes]] (section Overlay batch K). Pad stubs added under `src/ovl/pad/` for object-boundary nops.

## Comments

- Code review (inline, CODING_STANDARDS section 13 checklist, no sub-agents): matches verified with `funcdiff.py` (MATCH, or pad-only diff plus a passing `ninja build/ovl/<NAME>.ok`) and a full `ninja` (27 of 27 sha1 OK); no `NON_MATCHING`, no fakematch, no `FAKE` needed; C89 only (grep for `//`, `inline`, `bool` clean); headers have include guards, fixed-width types, `TtRec2` documents offsets and size; placeholders kept for all names; no generated asm or game data staged (only `src/ovl/*.c`, `include/ovl/*.h`, `src/ovl/pad/*.s`, wiki). Findings: none open. Spec axis: 76 matched against the goal of 40.
