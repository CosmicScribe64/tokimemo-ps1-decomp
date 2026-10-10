---
id: T-8070
title: Wave 5: list 7
status: Done
assignee: wave5-agent-7
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal
Match as many functions as possible from wave-5 list 7 (37 files, 169 functions).

## Acceptance criteria
- [x] Matched functions committed, 27/27 OK
- [x] Inline code review against CODING_STANDARDS.md

## Notes
34 functions matched, about 6860 bytes (grand total 4001/6958 functions, 521056/2279368 bytes, 22.9%). Method and new idioms: [[matching-notes]] section "Wave 5, list 7". T-0018 rows appended: 20 ([[data/t0018-cases]]).

Matched: DATE `func_801378BC`, `func_801386C8`, `func_801381B4`, `func_801395B4`, `func_80137B78`, `func_8013F544`, `func_8013D2C8`, `func_8013D23C`, `func_801387B8`, `func_80140ECC`, `func_80137A4C`, `func_80135A34`, `func_8013B910`, `func_8013A464`, `func_801380E4`; OMIMAI `func_801337EC`; SHOUGATU `func_80142784`; main `func_8004111C`, `k_disp_inc2`, `func_8007C784`, `normal_date_two_select_main`, `normal_date_three_select_main`; TT `func_8013E320`; OLH `func_80134824`, `func_80134B3C`, `func_80134C28`, `func_80136960`, `func_80136A30`, `func_80136B00`; BUNKAKEN `func_8013BFE0`; ETC `func_801413FC`; GYOZI `func_80140330`; GEKO `func_801346A0`; TAIIKU `func_801419F8`.

Tooling: no bug found. Note for the queue: DATE/ENDING `func_80132000` start with a one-nop pad but are not flagged `P`.

## Comments
Code review (inline, CODING_STANDARDS section 13): every match verified by `funcdiff.py` and the full `ninja` (27/27 OK, headers OK, globals OK, `sync_protos.py --check-branch` OK); no `NON_MATCHING`; C89 declarations at block top; each fakematch has a `FAKE` comment (unused frame slot `pad` locals in GEKO `func_801346A0`, DATE `func_8013A464`, main `k_disp_inc2`, `func_8007C784`; one-line `RECT` stores in `func_8004111C`; the undeclared 8-byte table at GameState+0x758 in nine DATE spots); header edits: `D_801217E4..` not added, `main_api.h` gained `func_8004ECB4`, `func_80052000`, `func_8004F984`, `func_80050B54`, `func_800443A0`, `D_800F54B2`, `D_80122D30`, `D_800CA2A4..6` and the `s32` return of the two `*_select_main`; `MAIN_API_OVERRIDE_D_80122EB8` (u32 selector) in BUNKAKEN. No findings open.
