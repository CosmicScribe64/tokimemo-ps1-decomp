---
id: T-11050
title: Big functions (Sonnet) 28
status: Done
assignee: sonnet-s28
created: 2026-10-13
updated: 2026-10-13
links: []
---

## Goal
Match big functions in DATE/80132000, main 8007C030, EVENT/8010FE10 and main 8006CB30 (34 functions, list astra28-list.txt).

## Acceptance criteria
- [x] ninja 27/27 OK, headers and globals OK
- [x] inline review against CODING_STANDARDS.md

## Notes
No function matched (0 bytes). Attempted drafts: EVENT `func_80114C5C` (423/432 insns, ~230 register differences), `func_8011B848`, `func_80119110`; permuter on DATE `func_8013C7D0`, `func_80140374` and main `normal_date_three_select_init` found nothing. All source reverted to INCLUDE_ASM / the existing NON_MATCHING guards; details and the new idioms in [[matching-notes]] section "T-11050". The TT overlay was not touched.

Inline review against CODING_STANDARDS.md: no C, header, config or tool change; only wiki pages edited.

## Comments
