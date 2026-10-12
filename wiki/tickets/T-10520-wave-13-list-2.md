---
id: T-10520
title: Wave 13: list 2
status: Done
assignee: wave-13 batch agent 2
created: 2026-10-12
updated: 2026-10-12
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal
Match as many functions as possible from wave-13 list 2 (59 functions, 30 files).

## Acceptance criteria
- [x] Matched functions committed; build 27/27 OK
- [x] Inline review against CODING_STANDARDS.md recorded

## Notes
Details: [[matching-notes]], section "Wave 13, list 2 (T-10520)".

## Comments

Result: 10 of 59 functions matched (2496 bytes): GEKO `func_80139D54`, SHOUGATU `func_80138D2C`, `func_80139514`, `func_80137D0C`, TACO `func_8013474C`, OMIMAI `func_801331B0`, TAIIKU `func_8013A63C`, `func_8013ACBC`, `func_8013C4E4`, main `func_8007A354`. `ninja progress` grand total 5109 -> 5119 of 6958 functions (915,548 -> 917,172 bytes). 17 guarded near misses and 5 T-0018-style rows are in [[matching-notes]] (section "Wave 13, list 2 (T-10520)") and [[data/t0018-cases]].

Inline review (CODING_STANDARDS section 13): matches verified by the unit sha1 in `ninja` (27/27 OK, `build/headers.ok`, `build/globals.ok`); every `NON_MATCHING` block is guarded with `T-10520` and its default build matches; C89 only (declarations at block top); main-exe symbols added through `sync_protos.py --add-global`/`--write` (no hand-written duplicates), overrides carry their reason (`TACO.h`, `VALEN.h`, `EVENT.h`); fakematches carry a `FAKE` comment (SHOUGATU `func_80138D2C`, TACO `func_8013474C`, TAIIKU `func_8013C4E4` padding locals); no game data or generated files staged. `sync_protos.py --check-branch` and `migrate_globals.py --check` clean. No findings.

Tooling observations: `funcdiff.py --resolve` can hide a one-instruction reorder behind a relocation line (TAIIKU `func_801368B8`) and once reported MATCH for a longer function (TAIIKU `func_80137228`); the unit sha1 caught both.
