---
id: T-11030
title: Big functions (Sonnet) 26
status: Done
assignee: sonnet-s26
created: 2026-10-13
updated: 2026-10-13
links: []
---

## Goal
Match big functions in DATE/80132000, main 8007C030, EVENT/8010FE10 and main 8006CB30 (36 functions, list astra26-list.txt).

## Acceptance criteria
- [x] ninja 27/27 OK, headers and globals OK
- [x] inline review against CODING_STANDARDS.md

## Notes
Matched 2 functions (1,552 bytes): main `func_80072338`, DATE `func_8013CDF8`. Near misses guarded with NON_MATCHING: main `date_sasoi_judge`, DATE `func_8013C7D0`. Attempted and reverted to INCLUDE_ASM (drafts structurally close, details in [[matching-notes]] section "T-11030"): DATE `func_8013EB8C`, `func_8013FAD0`, `func_801407D0`, `func_8013C190`, `func_8013A94C`; EVENT `func_80117D80`, `func_8011C164`, `func_80119110`; main `func_8006D138`. Not attempted: the 4-9 KB functions (DATE `func_8013272C`, `func_80137154`, main `func_8007EF48`, `func_80081190`, `func_8006D7EC`, `func_8007132C`, EVENT `func_801103A0`). The TT overlay was not touched.

Inline review against CODING_STANDARDS.md: new code uses GameState fields and shared views (`ScheduleEntryFlags`, `ScheduleClubFlags`, `Rec38Flags`); the byte-cast reads of `unk_03F`/`unk_0F6` that stop uopt promoting a global are documented in the notes; DATE.h got one appended view (`DateFlags0F4`); main_api.h got one declaration (`func_8007259C`) and one global (`D_800B6740`, via --add-global); no tool, config or other overlay edited; sync_protos --check-branch OK, migrate_globals --check OK, ninja all OK with headers.ok and globals.ok.

## Comments
