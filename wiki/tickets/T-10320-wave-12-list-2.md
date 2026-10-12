---
id: T-10320
title: Wave 12: list 2
status: Done
assignee: wave-12 batch agent 2
created: 2026-10-11
updated: 2026-10-12
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal
Match as many functions as possible from wave-12 list 2 (139 functions, 48 files).

## Acceptance criteria
- [x] Matched functions committed; build 27/27 OK
- [x] Inline review against CODING_STANDARDS.md recorded

## Notes
Details, idioms and blockers: [[matching-notes]], section "Wave 12, list 2 (T-10320)". T-0018 rows: [[data/t0018-cases]].

Matched (6, 2,640 bytes): TT `func_8013BB28` (488), TACO `func_80148D6C` (628), `func_801483B0` (308), `func_8013E4FC` (636), `func_8013DDE4` (256), main `func_800536AC` (324).
Near misses kept under `NON_MATCHING` (17): BUNKAKEN `func_801428D0`, RPG_BAT `func_8014AB84`, `func_8014AC74`, `func_8015A744`, GYOZI `func_8013506C`, `func_80137188`, DATE `func_8014DCCC`, `func_8014AF88`, TACO none left, TT `func_801436BC`, `func_8013C0A8`, `func_8013A3E0`, `func_8013BE9C`, OPTION `func_80137E38`.
Five list entries (`func_801404FC`, `func_801407D4`, `func_80140364`, `func_80140028`, `func_8013F9C0`) are defined in TACO/8013F250.c, which the list does not own; not touched.

## Comments
2026-10-12 inline review against CODING_STANDARDS.md (no sub-agent): matches verified with `funcdiff.py --resolve` and the unit SHA1 checks of `ninja` (27 units OK, `build/headers.ok`, `build/globals.ok`); `sync_protos.py --check-branch` 0 new disagreements, `migrate_globals.py --check` OK. Every `NON_MATCHING` block is guarded, names T-10320, and the default build matches. C89 only (declarations at block top, `/* */` comments). Fakematches carry `FAKE` comments with reason: main `func_800536AC` (unused 8-byte local for the 0x58 frame), TACO `func_8013E4FC` and `func_8013DDE4` (loop entry on one source line, second base spelled from the end label), TACO `func_801483B0` (invariant store inside the loop). New shared declarations: `D_800F54B6`, `D_800D9320`, `D_80122840`, `D_80122B10`, `D_80122660`, `place_init2`-style names were NOT added; `func_8009B430` is guarded in main_api.h with a documented DATE override (3 arguments). Existing `Tc14` is reused for the TACO particle records. No game data or asm staged. Findings: none open.
