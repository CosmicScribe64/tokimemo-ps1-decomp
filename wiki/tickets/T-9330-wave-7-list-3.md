---
id: T-9330
title: Wave 7: list 3
status: Done
assignee: w7-3
created: 2026-10-10
updated: 2026-10-10
links: ["[[matching-notes]]", "[[decompile-workflow]]"]
---

## Goal
Match as many functions as possible from wave-7 work list 3 (35 files, 152 functions).

## Acceptance criteria
- [x] Matched functions committed, build 27/27 OK
- [x] Inline review against CODING_STANDARDS.md

## Notes
Result: 17 functions, 6,244 bytes; 23 rows added to [[data/t0018-cases]]; patterns in [[matching-notes]] "Wave 7, list 3 (T-9330)".

## Comments

### Review (inline, CODING_STANDARDS.md)
- Match rule: all 17 functions compared with `funcdiff.py` and by the sha1 of the full build (27/27 OK, headers OK, globals OK); no NON_MATCHING blocks added; the failed attempts were reverted to INCLUDE_ASM.
- C dialect: C89, `/* */` comments only, declarations at block starts; K&R definitions (`func_80133F00`, `func_80134C1C`) only where the original passes narrow arguments raw.
- Fakematches: seven marked `FAKE` (unused leading `pad` in `func_80136700`, `u8` view of `D_80121603`, pointer-as-expression macro in `func_80134C1C`, doubled index variable in `func_80133F00`, store on the `for` line in `func_801334BC`, double test in `func_801493D4`, two `D_800B0A04`-relative accesses in `func_80108F50`).
- Declarations (8a): main-exe symbols added with `sync_protos.py --add-global`/`--fix` (main_api.h), overlay-local ones appended to `include/ovl/*.h`; `check_headers` and `sync_protos --check-branch` pass; the `D_80121808` EVENT override was not needed in the end.
- Data: `D_8015DDD0` is now defined in DATE/8014A1B0.c (T-9010 island); two symbols were appended to `config/symbol_addrs.txt` for TACO `func_801490B4`.
- No findings open.
