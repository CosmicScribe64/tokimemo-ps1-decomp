---
id: T-2080
title: "Wave 2: overlays TAIIKU, RPG_BAT"
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[overlays]]"]
---

## Goal

Decompile functions in `src/ovl/TAIIKU.c` and `src/ovl/RPG_BAT.c` (IDO 5.3, frame pass, `-Wo,-nokpicopt`), smallest first, aiming for at least 60 matches. First retry the three `NON_MATCHING` TAIIKU functions left from T-0017. Work list: `tools/queue.py --next 40 --files TAIIKU,RPG_BAT`.

## Acceptance criteria

- [x] Functions matched (funcdiff MATCH), all 27 sha1 checks OK. Result: 108 (RPG_BAT 61, TAIIKU 47 new); goal was at least 60
- [x] New failure patterns noted in [[matching-notes]] (section Wave 2); 10 T-0018 cases appended to [[data/t0018-cases]]
- [x] Code-review gate run inline, findings resolved

## Notes

The three TAIIKU functions left under `NON_MATCHING` by T-0017 (`func_8013815C`, `func_801446A0`, `func_80146FA0`) were already plain C when this ticket started (T-0017 moved them out once `-Wo,-nokpicopt` matched them), so there was nothing to retry. New header `include/ovl/RPG_BAT.h`; `include/ovl/TAIIKU.h` gained externs, prototypes and three small struct types (`TaiikuPair`, `TaiikuRec`, `TaiikuBig`). Patterns and blockers: [[matching-notes]], section "Wave 2 (T-2080)". Scratch helpers (not committed) lived in `build/scratch/`: an m2c wrapper that draws from one overlay, a `funcdiff` wrapper that folds `sym+offset` relocations to absolute addresses, and an auto-draft/auto-revert loop.

## Comments

- Code review (inline, CODING_STANDARDS section 13 checklist, no sub-agents): matches verified with a normalised `funcdiff.py` and the 27 sha1 checks of a clean `ninja` (the abs-address reads of TAIIKU `func_80142180`/`func_80145210` differ from the original only in the relocation record, the linked bytes are identical); no new `NON_MATCHING`, no new fakematch (the three `FAKE` comments in TAIIKU.c are from T-0017); C89 only (no `//`, `inline`, `bool`; declarations at block top; all compiled by cfe); externs are in the overlay headers with one type per symbol and `tools/check_headers.py` passes; struct layouts documented with offsets and sizes; placeholders kept; unused externs added by drafts were removed; only `src/ovl/TAIIKU.c`, `src/ovl/RPG_BAT.c`, their headers and wiki files are changed. Findings: none open. Spec axis: 108 matched against a goal of 60.
