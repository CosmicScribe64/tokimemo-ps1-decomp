---
id: T-2010
title: "Wave 2: DATE"
status: Done
assignee: claude
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Match the remaining unblocked functions of `src/ovl/DATE.c` (queue flags R/V excluded). DATE2 is a separate file and not part of this ticket.

## Acceptance criteria

- [x] At least 60 functions matched: 278.
- [x] Clean build (`rm -rf asm build`, configure, `ninja` without `-k`): 27 of 27 sha1 OK, header check passes; `ninja progress` grand total 901 -> 1179 of 6962.
- [x] Inline review against CODING_STANDARDS.md done, findings fixed.
- [x] T-0018 cases appended to [[data/t0018-cases]] (9 rows).

## Notes

Result: 278 functions of `src/ovl/DATE.c` matched (INCLUDE_ASM 637 -> 359), in five commits. Method: m2c drafts for the whole queue in one Docker call, batch apply, a relocation-resolved per-function comparison of the built object against `expected/ovl/DATE.o`, revert of the failures, then `ninja` (sha1) before each commit. Patterns and misses: [[matching-notes]] section "Wave 2: DATE (T-2010)". T-0018 data: 9 `regorder` rows in [[data/t0018-cases]]. About 140 unblocked functions remain; the top blocker is register order (`$v0` vs `$t6` first temporary, shared constants, scheduling of two read-modify-write statements).

## Comments

Inline review (CODING_STANDARDS): C89 and `/* */` comments only; no `//`, no dead code, no m2c noise (`/* irregular */`, `/* extern */` lines removed); externs and prototypes only in `include/ovl/DATE.h`, nothing redeclared from `game.h` (`tools/check_headers.py` passes in ninja); callees use the O.BIN names where one exists; the `(u16)`/`(u8)` casts and the `*(T *)0xADDR` reads are explained by a comment at the top of `src/ovl/DATE.c`. Findings: m2c `/* irregular */` comments removed, duplicate inline prototypes moved into the header. No open findings. No fakematches introduced.
