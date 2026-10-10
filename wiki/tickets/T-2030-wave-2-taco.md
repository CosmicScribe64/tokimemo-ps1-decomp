---
id: T-2030
title: "Wave 2: TACO"
status: Done
assignee: claude
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal
Match as many functions as possible in `src/ovl/TACO.c` (wave 2 batch, branch w2-taco).

## Acceptance criteria
- [x] At least 60 functions matched (70 matched), 27/27 OK in a clean build
- [x] T-0018 cases appended to `wiki/data/t0018-cases.md`
- [x] Code review against CODING_STANDARDS.md recorded below

## Notes
Queue worked smallest first on `--files TACO`. Patterns and leftovers: [[matching-notes]], section "Wave 2 batch TACO". 7 rows appended to [[data/t0018-cases]].

## Comments

- Inline review against CODING_STANDARDS (2026-10-09): C89 only (declarations at block top, no `//`), no NON_MATCHING and no fakematch, no dead code, fixed-width types, struct offsets documented in `include/ovl/TACO.h`, externs declared once (`tools/check_headers.py` passes, unused declarations removed), `INCLUDE_ASM` kept in address order for the rest. Clean rebuild (`rm -rf asm build; configure.py; ninja`): 27 of 27 sha1 OK. No open findings.
