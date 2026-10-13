---
id: T-10710
title: Wave 15: list 1
status: Done
assignee: w15-1
created: 2026-10-12
updated: 2026-10-12
links: ["[[matching-notes]]", "[[data/t0018-cases]]", "[[gcc-objects]]"]
---

## Goal
Match as many functions as possible from wave15-list-1 (about 100 functions, 39 files).

## Acceptance criteria
- [x] Every function attempted (m2c draft, build, funcdiff or regdiff); the six functions of 1 KB and more only got the m2c review and a row in [[data/t0018-cases]].
- [x] ninja 27/27 OK, `build/headers.ok`, `build/globals.ok`, `sync_protos.py --check-branch` OK.
- [x] Inline review against CODING_STANDARDS.md.

## Notes
Matched (4 functions, 1,068 bytes):
- main `menu_check_2` (164 bytes): record view + `MenuSelection5`, `sel = -1;` first.
- GYOZI `func_8013E0A4` (164): twin of EVENT `func_8010F4B8`.
- TACO `func_8015CCC8` (412): `i * 0x88 + (u8 *)D_8015EDB4` element address (offset first).
- ETC `func_8013E094` (328): locals `base/e1/e2` plus one FAKE `s32 pad` for the frame offset.

Guarded near misses kept under `#ifdef NON_MATCHING` with the `INCLUDE_ASM` fallback: main `menu_check_1`, `menu_set`, `initModelingData_init` (7 lines); NAME_ENT `func_80140B20` (14); OPTION `func_80139878` (6); BUNKA_SD `func_8013987C` (6); RPG_BAT `func_80153A60` (registers only, 41/41); TAIIKU `func_80147210` (21); TACO `func_8015B1B8` (frame offset only); TT `func_8013D150` (99/99), `func_8013E498` (130/130). Existing near misses of earlier waves were re-checked without change: TT `func_8014C5F8`, TACO `func_8015C0A0`, GYOZI `func_8013506C`, VALEN `func_80133670`, OPTION `func_80139F8C`.

Blocked groups (rows in [[data/t0018-cases]]): shared `lui $at` over several words of one original struct while other files use the words as scalars (RPG_BAT `func_8014F278`, `func_8014EF8C`, `func_8014E994`; `InitMouse` has no data range); libgte assembly (TACO `func_8015D0D0`, DATE `func_8015A640`, `func_8015A980`); GameState field vs scalar base registers (SHOUGATU `func_801347E0`, main `func_8006BA40`); the `return 0` placed before the last store (main `func_80042808`, `func_8004284C`, `func_80042908`, `func_80042940`).

Tooling observation: `funcdiff.py` compiles without `NON_MATCHING`, so a guarded draft prints `MATCH` from the `INCLUDE_ASM` fallback; `regdiff.py` compiles the guarded variant.

## Comments
Inline review (CODING_STANDARDS): no macros added; every non-obvious trick has a FAKE or NOTE comment (ETC pad, NON_MATCHING notes); new main-exe symbols declared through `sync_protos.py --add-global`; the pointer-parameter change of `initModelingData_init`/`func_80058F0C` was written through `sync_protos.py --write` plus the alias line; overlay declarations appended to `include/ovl/TT.h`; no game data or generated files committed. Findings: none open.
