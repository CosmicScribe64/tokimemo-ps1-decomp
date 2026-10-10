---
id: T-2040
title: "Wave 2: ETC"
status: Done
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
- [x] Code-review gate run inline, findings resolved

## Notes

Families: pointer-table loaders (constant-address loads), call wrappers, 60 screen setup functions on the 0x44-entry table. 4 FAKE locals (`pad`), listed in [[matching-notes]]. Not matched and recorded: T-0018 rows `func_80142AE8`, `func_801428EC`, `func_80143F24`, `func_80145AF8`; others in the matching-notes section. 190 INCLUDE_ASM left, 141 of them blocked (jump table, string, R/V).

## Comments

- Code review (inline, CODING_STANDARDS section 13 checklist, no sub-agents): final clean rebuild (`rm -rf asm build`, configure, ninja, no `-k`) ends with all sha1 OK (27 `.ok` files incl. headers) and `ninja progress` grand total 1098 of 6962; C89 only (no `//`, no mixed declarations); all non-obvious tricks marked `FAKE` (4 unused locals) or explained (constant-address loads, `D_80120650` base, implicit-int returns in matching-notes); externs in `include/ovl/ETC.h` only (a struct `Rgb555Split` documented with offsets and size), none repeated from `game.h`; placeholders kept for all names; no generated asm, build output or game data staged. Findings: none open.
