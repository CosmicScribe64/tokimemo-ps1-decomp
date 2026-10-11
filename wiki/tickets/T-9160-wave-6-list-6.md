---
id: T-9160
title: Wave 6: list 6
status: Done
assignee: wave-6 agent 6
created: 2026-10-10
updated: 2026-10-10
links: []
---

## Goal

Match as many functions as possible from wave-6 list 6 (36 files, 137 functions).

## Acceptance criteria

- [x] Matched functions verified with funcdiff and a clean 27/27 build
- [x] Inline review against CODING_STANDARDS.md recorded

## Notes
Branch w6-6, not merged. 20 functions matched (progress 4267 -> 4287 functions): SHOUGATU `func_80136178`, `func_80132E8C`, `func_80136BD4`; SHUGAKU `func_8013A9E0`; TACO `func_801571E8`, `func_801572D8`, `func_801589B0`, `func_80158B84`, `func_8013515C`, `func_80134EBC`; TAIIKU `func_80143AC8`, `func_80144768`, `func_801448D4`, `func_80144C70`; main `week_day`, `func_8006B900`; EVENT `func_801016BC`; GEKO `func_8013D9C4`; OPTION `func_80139F04`; EN_NICHI `func_80132ADC`. 35 rows added to [[data/t0018-cases]]. Data islands made: TAIIKU/80142240, OPTION/801389A0, EN_NICHI/80132000 (configs `config/overlays/{TAIIKU,OPTION,EN_NICHI}.yaml`). Patterns: [[matching-notes]], "Wave 6, list 6 (T-9160)".

## Comments

### Review (inline, against CODING_STANDARDS.md)
- Matches: each function checked with `funcdiff.py --resolve` and the unit sha1; final `rm -rf asm build; configure.py; ninja` ends with all 27 `OK`, `headers OK`, `globals OK`; `sync_protos.py --check-branch` OK; `migrate_globals.py --check` OK.
- C89 and comments: declarations at block top, `/* */` only. Two `FAKE` comments (TACO `func_801571E8`, `func_801572D8`: three stores on one line, found by the permuter, as1 orders the stores of one source line together). No `NON_MATCHING` blocks.
- Headers: `TcPos`/`Tri3S`/`EnNichiPos`/`TaiikuU16x2`-style types documented with offsets; `include/ovl/TACO.h` gains the prototypes `func_80158CDC`/`func_80158DBC` (s16 arguments, they are what makes IDO sign-extend the call arguments); `include/ovl/TAIIKU.h`, `OPTION.h`, `EN_NICHI.h`, `RPG_BAT.h`: appends only; SHOUGATU `func_80083440` override defined in the .c with its reason (CODING_STANDARDS 8a).
- Deviations to know: `src/main/800737A0.c` forward-declares `week_day_main`/`week_day_exit` (defined later in the same file) and `src/main/800674B0.c` declares `D_800B66E0[]`, `D_800B66F4[]`, `D_800B6708` locally; `check_headers.py` accepts both, but they are main-exe symbols that section 8a wants in `main_api.h` (the tool offers no way to add them there).
- Data: owned variables defined in C with their original (zero) initialisers; pieces that were bigger than the variable are covered by one definition per word.
- Findings: none open.
