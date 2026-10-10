---
id: T-8080
title: Wave 5: list 8
status: Done
assignee: wave-5 agent 8
created: 2026-10-10
updated: 2026-10-10
links: []
---

## Goal
Match as many functions of wave-5 list 8 as possible (GYOZI, 8005A0B0, TEL, NAME_ENT, 800451D0, ...).

## Acceptance criteria
- [x] Matched functions committed, ninja 27/27 OK (clean rebuild, headers OK, globals OK, `sync_protos.py --check-branch` OK)
- [x] Inline review against CODING_STANDARDS.md

## Notes
Result: 56 functions, 16320 bytes matched (progress 3967 -> 4023 functions, 530520 bytes); list functions and non-list functions of the same files. New patterns in [[matching-notes]] ("Wave 5, list 8 (T-8080)"): `s32` return without a value for scene switches, the `*(u8 *)&D_800E6280.field` view as the scalar-global access, local function-pointer tables with the index passed on, stack-slot padding rules for menu arrays and spilled locals. 19 rows added to [[data/t0018-cases]] for the cases left open.

Matched: EVENT `func_80119440`, `func_801146B4`, `func_80117A68`, `func_8011C6C4`, `func_800FDF90`, `func_800FEA38`; GYOZI `func_80137798`, `func_80141550`, `func_80134938`; RPG_BAT eleven scene scripts (`func_80136D50`, `func_801375F8`, `func_80137E74`, `func_80158240`, `func_8015831C`, `func_801583F8`, `func_801584D4`, `func_801585B0`, `func_80158930`, `func_80157AE0`, `func_801480F0`) and `func_80156CC0`; main 8005A0B0 (`func_8005A0B0`, `func_8005A1A0`, `func_8005BAE0`, `func_8005BB84`, `func_8005BC38`, `func_8005C968`, `func_8005D804`, `func_8005E018`, `func_8005E2C0`, `func_8005E454`, `func_8005E5C8`, `func_8005E7F0`, `func_8005EA38`, `func_8005EB74`, `func_8005ECB8`, `func_8005EE00`, `func_8005EF44`, `func_8005F26C`, `func_8005F478`, `func_8005FF68`, `func_80060958`, `pre_xmas_init`), 80059B40 `func_80059E00`, 800789E0 `func_80078A94`; OLH `func_80132884`, `func_80132AE8`; ETC `func_80149BC0`, `func_801495CC`; SHOUGATU `func_80134930`; NAME_ENT `func_80135434`, `func_801362D0`, `func_801385A8`, `func_80137608`, `func_80138338`, `func_80138A7C`.

Header edits (declarations only): `func_80066104` is K&R in `main_api.h` (pre_xmas_init passes a string through $a0), `pre_xmas_init`, `func_8005A1A0`, `func_80059E00` return `s32`, OLH's `func_80132884`/`func_80132AE8` return `s32`, new externs in the overlay headers (`sync_protos.py --fix` moved the main-exe ones).

Tooling: no bugs found. `funcdiff.py` without `--resolve` hides load-width differences (`lb` against `lbu`); `--resolve` is the check to use before `ninja`.

Review (CODING_STANDARDS checklist, inline): C89 only, `/* */` comments, locals at the top of blocks; every trick carries a `FAKE` comment (u8 views of the selector, unused `pad` locals, `(u8)` casts on `func_8002328C`/`func_8005E0E0`, `one = 1`, `u16 idx` plus second argument, `u8 sel` copy); no unmarked oddities left (the RPG_BAT scene functions and the u8 views of `func_8005E5C8` have explanatory comments); main-exe symbols declared once in `main_api.h` (`sync_protos.py --check-branch` OK); no game data, asm or build output committed. No open findings.

## Comments
