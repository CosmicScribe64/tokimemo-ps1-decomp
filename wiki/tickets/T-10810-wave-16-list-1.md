---
id: T-10810
title: Wave 16: list 1
status: Done
assignee: w16-1
created: 2026-10-12
updated: 2026-10-13
links: ["[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal
Match as many functions as possible from wave16-list-1 (37 functions, 23 files).

## Acceptance criteria
- [x] Every function attempted (m2c draft, build, funcdiff or regdiff); the libgte helpers and the blocked groups only got the m2c review and a row in [[data/t0018-cases]].
- [x] ninja 27/27 OK, `build/headers.ok`, `build/globals.ok`, `sync_protos.py --check-branch` OK.
- [x] Inline review against CODING_STANDARDS.md.

## Notes
Matched (4 functions, 1,064 bytes):
- TAIIKU `func_80147210` (264 bytes): `for` with a comma-separated init `(i, v, a, d)` and `(i + 1) * 8` inline; the same body as a do-while was right in the wrong registers.
- TAIIKU `func_80147120` (240): every access through `((Tk1C *)D_8014A400)[i]` hoists `D_80120896` and keeps the frame at 0x30 (a named pointer or `y` local costs 8 bytes of frame).
- EN_NICHI `func_80134F2C` (280) and twin `func_80135044` (280): the record pointer stays the file's `EN_OBJ(arg0)` expression (a local `EnObj *e` adds a home slot, frame 0x38), and `unk_28` is stored before `unk_2C` for the ugen temporary order.

Guarded near misses (`#ifdef NON_MATCHING`, `INCLUDE_ASM` fallback): EN_NICHI `func_80136A2C` (6 lines: end pointer and the constant 4 swap a1/a2), main `func_80055A38` (10 register fields), TACO `func_8015B1B8` (8 lines, ugen t2/t3 in the struct copy), TACO `func_8015BF70` (about 20 lines).

Blocked groups (rows in [[data/t0018-cases]]): return-0 delay-slot order in main `func_80042808`/`8004284C`/`80042908`/`80042940` (retried with five more forms); shared `lui at` without an owning data range (`InitMouse`, `menu_bar_color`); the RPG_BAT trio, where the build already provides the interior symbols and only `include/ovl/RPG_BAT.h` blocks a struct definition; libgte assembly (`func_8015A640`, `func_8015A980`, `func_8015D0D0`); gcc division expansion (TT `func_8014D260`).

Details of the idioms: [[matching-notes]], section "Wave 16, list 1".

## Comments
Inline review (CODING_STANDARDS): no macros added (the `EN_OBJ` macro is the file's existing FAKE with its comment); new types are `Tk1C` (TAIIKU, next to `Lim3`) and file-local nothing else; overlay declarations appended to `include/ovl/EN_NICHI.h` (`D_80139A2C`, `D_80139228`) and `include/ovl/TACO.h` (`D_80161E20`, `D_80161E60`); `D_800B3CF8` declared through `sync_protos.py --add-global`; every guarded draft carries a NON_MATCHING note with the remaining difference; no game data or generated files committed. Findings: none open.
