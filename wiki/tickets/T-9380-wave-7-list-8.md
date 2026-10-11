---
id: T-9380
title: Wave 7: list 8
status: Done
assignee: wave7-agent-8
created: 2026-10-10
updated: 2026-10-11
links: []
---

## Goal
Match as many functions as possible from wave-7 list 8 (36 files).

## Acceptance criteria
- [x] Matches verified with funcdiff; 27/27 OK build (clean rebuild, headers OK, globals OK, `--check-branch` OK)
- [x] Inline review against CODING_STANDARDS.md

## Notes
27 functions matched, about 5,800 bytes (`ninja progress`: grand total 4585/6958 functions, 670416 bytes): BUNKA_SD `func_801345BC`; DATE `func_8013FA6C`, `func_8013A344`, `func_8013D174`, `func_80136A74`, `func_8014E38C`, `func_80137920` plus nine twins (`func_80137984`, `func_801379E8`, `func_80137B14`, `func_80137C2C`, `func_80137C90`, `func_80138470`, `func_8013872C`, `func_8013882C`, `func_80138890`); SHOUGATU `func_80133D98`; NAME_ENT `func_8014A848`, `func_80146BCC`; EN_NICHI `func_801383A0`, `func_80137E2C`; RPG_BAT `func_8013A2F4`, `func_8013D59C`, `func_801439E0`; ENDING `func_80137160`; TT `func_8014AFB8`, `func_8014D168`. Drafts behind `NON_MATCHING`: EN_NICHI `func_80137C58`, TT `func_8014CC04`, `func_8014B088`. Data islands: DATE `8014CC40`, `80132000`; NAME_ENT `80144AD0`; EN_NICHI `80136B10`. Patterns: [[matching-notes]] "Wave 7, list 8". T-0018 rows: 21 ([[data/t0018-cases]]). Tooling bug for the report: `data_island.py` omits pointer-only labels (NAME_ENT/80144AD0).

## Comments
- 2026-10-11 inline review against CODING_STANDARDS.md: C89 only (declarations at block tops, `/* */` comments); every fakematch carries a `FAKE` comment with reason and T-9380 (`func_8013FA6C`, `func_8013A344`, `func_80133D98`, `func_80137E2C`, `func_80137160`, `func_8014D168`, `func_80146BCC`, `func_8014E38C`, `func_801345BC`); the three `NON_MATCHING` blocks carry the ticket id and the default build matches; main-exe symbols added only through `sync_protos.py --add-global`/`--write`, the two file overrides (`D_80121317`, `D_80121808` in DATE) explain themselves, `check_headers.py` and `--check-branch` pass; no game data staged. No open findings.
