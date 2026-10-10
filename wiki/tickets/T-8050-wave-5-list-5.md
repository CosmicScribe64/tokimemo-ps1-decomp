---
id: T-8050
title: Wave 5: list 5
status: Done
assignee: wave-5 agent 5
created: 2026-10-10
updated: 2026-10-10
links: ["[[matching-notes]]", "[[data/t0018-cases]]", "[[decompile-workflow]]"]
---

## Goal
Match as many functions as possible from wave-5 list 5 (37 files, 160 functions, 49,644 bytes; nearly all earlier failures).

## Acceptance criteria
- [x] Matched functions committed, 27/27 OK, headers/globals OK
- [x] Inline review against CODING_STANDARDS.md

## Notes
Result: 24 functions matched by hand (BUNKA_SD 1, ETC 7, EVENT 4, NAME_ENT 9, SHUGAKU 1, TT 2) plus 7 GYOZI twins filled by `dupes.py`; five close near misses stay behind `#ifdef NON_MATCHING` with a ticket id (KANGEI `func_80138D40`, NAME_ENT `func_801454CC` and `func_80140D6C`, TT `func_80147074`, GYOZI `func_8013E724`). Patterns, failures and tool notes: [[matching-notes]], section "Wave 5, list 5 (T-8050)". Eight new rows in [[data/t0018-cases]].

FAKE markers: EVENT `func_800FCDD8` (unused `idx`, `r == (D * 0)` from the permuter), `func_801073FC` (scalar addressed from the Rec34 table), NAME_ENT `func_8013D62C` (unused `pad` before the buffer).

## Comments
- Review against the CODING_STANDARDS checklist (inline, no sub-agent): C89 only, declarations at the top of each block, `/* */` comments in ASCII, no `//`; the K&R rules (section 2) respected (no casts added for promotion); main-exe symbols declared only through `sync_protos.py` (`--check-branch` clean, `headers OK`, `globals OK`); every `NON_MATCHING` block carries T-8050 and the diff cause; the three tricks are marked FAKE with their reason; no game data, `asm/`, `build/` or `expected/` staged. No open findings.
