---
id: T-8060
title: Wave 5: list 6
status: Done
assignee: wave-5 agent 6
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal
Match as many functions as possible from wave-5 work list 6 (37 files, 148 functions).

## Acceptance criteria

- [x] Matched functions verified with ninja (27/27 OK) and funcdiff
- [x] Inline review against CODING_STANDARDS.md recorded

## Notes
Result: 29 functions, 5980 bytes matched (GEKO 7, TT 3, TACO 5, EVENT 3, SHOUGATU 2, SHUGAKU 1, KANGEI 1, TEL 1, BUNKA_SD 2, DATE 1, EN_NICHI 1, RPG_BAT 1, TAIIKU 1). Patterns in [[matching-notes]], section "Wave 5, list 6 (T-8060)"; 18 unmatched cases appended to [[data/t0018-cases]] (register-order gaps: `$v1`/`$a0`/`$a1`/`$a3`/`$a2` where IDO picks other registers), 3 toolchain notes (DATE2 `% 2 != 0` folding, ETC constant-address pointer, EN_NICHI unroll shape).
FAKEs: the `(u8)` cast of T-2020 (5 functions), `TtXY` pad and `fake_pad` (TT `func_8013A810`, `func_8013A8B8`), unused `TcPos` locals (TACO `func_80158AB0`, `func_80158CDC`, `func_80158DBC`, `func_80156F50`), `^ 0` (BUNKA_SD `func_8013261C`).
Header changes: `include/ovl/{GEKO,TT,TACO,EN_NICHI,BUNKA_SD,TAIIKU,TEL,EVENT}.h` appended declarations only; `D_800CA23C[]`, `func_80050B54`, `func_80051010`, `func_80069128` added to `include/main_api.h`; TACO `func_8015ACCC` declared `s32` (callers read `$v0`, found by `sync_protos.py --check-branch`).

## Comments
- 2026-10-10 inline review against CODING_STANDARDS.md: C89 only (declarations at block top, `/* */` comments); every trick carries a `FAKE` comment with ticket id; no SDK structs redefined; main-exe symbols declared once (`main_api.h`), overlay-local ones appended to the overlay header; fixed-width types; game-state fields through `D_800E6280`, `migrate_globals.py --check` OK; `sync_protos.py --check-branch` OK; no game data staged; commits small with ticket id. No open findings.
