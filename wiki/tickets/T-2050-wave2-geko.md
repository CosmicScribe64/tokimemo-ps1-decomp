---
id: T-2050
title: Wave 2: GEKO
status: Done
assignee: claude
created: 2026-10-09
updated: 2026-10-09
links: ["[[matching-notes]]", "[[decompile-workflow]]"]
---

## Goal
Match as many remaining functions of overlay GEKO (src/ovl/GEKO.c) as possible; target at least 60.

## Acceptance criteria

- [x] Matches verified with funcdiff and a clean `ninja` (27/27 OK)
- [x] T-0018 cases recorded in [[data/t0018-cases]]
- [x] Inline code review against CODING_STANDARDS.md

## Notes

## Comments
- Result: 176 GEKO functions matched (367 -> 191 INCLUDE_ASM), 8 T-0018 rows added. Clean rebuild (`rm -rf asm build`, configure, ninja): 27 of 27 OK, headers OK. `ninja progress` grand total 1077/6962.
- Inline code review against CODING_STANDARDS.md: C89 only (no `//`, declarations at block top), shared externs in include/ovl/GEKO.h with one type each (check_headers passes), forward prototypes in the header, no fakematches (no FAKE needed), no NON_MATCHING, no game data staged. `*(s16 *)0x801CE158` style absolute casts follow the batch A note for addresses outside the overlay. No open findings.
- Tooling bug: tools/m2c.py picks the first sorted `asm/ovl/*` hit, wrong for names shared by two overlays (see matching-notes, Wave 2: GEKO).
