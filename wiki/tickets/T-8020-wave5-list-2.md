---
id: T-8020
title: "Wave 5: list 2"
status: Done
assignee: wave5-agent-2
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal
Match as many functions as possible from the wave-5 list 2 (36 files, 133 functions).

## Acceptance criteria
- [x] Matches committed, 27/27 OK (clean rebuild after `rm -rf asm build`)
- [x] Inline code review against CODING_STANDARDS.md

## Notes
Result: 21 of 133 functions matched, 6,192 bytes (list expected 35,643). Patterns and the unsolved shapes: [[matching-notes]], section "Wave 5, list 2 (T-8020)"; 17 new rows in [[data/t0018-cases]].

Matched: main `func_800623E4`, `func_80062840`; DATE `func_8014D818`, `func_8014DE10`, `func_8014ECF0`, `func_8014D000`; DATE2 `func_8013775C`; SHUGAKU `func_8013A314`; ENDING `func_80133030`; MASTER `func_80139810`; GEKO `func_80141514`; RPG_BAT `func_80135D90`, `func_80159D50`; ETC `func_801408F0`; NAME_ENT `func_80143498`; OPTION `func_801371FC`, `func_80133AD8`, `func_80136A50`, `func_80135C0C`; BUNKA_SD `func_80134B88`; OLH `func_801332F8`.

FAKEs (3 kinds): DATE2 `func_8013775C` (`0x38U`), MASTER `func_80139810` (two unused locals for the 0x68 frame), the T-7020 loop-body-on-the-`for`-line comments in OPTION (5 functions).

Header edits to existing lines (type changes forced by the C): `func_801332F8`, `func_80159D50` void -> s32 (they fall off the end of an int function). Appended declarations in `include/main_api.h` (`func_8004F984`, `func_8004E93C`, `func_80045288`, `func_80061A3C`, `func_80062634`, `func_8007E99C`, `func_80052000`, `D_801208FB`, `D_800B3CFC`, `D_80120912[]`, ...), `include/ovl/{DATE,OPTION,EVENT,RPG_BAT,OLH,NAME_ENT,MASTER,GEKO,ETC,BUNKA_SD}.h`.

## Comments

### Inline review (CODING_STANDARDS.md, no sub-agents)
- Section 1/6: every function is a plain match; no `NON_MATCHING`; failed attempts reverted to `INCLUDE_ASM` in place.
- Section 2: C89 only (declarations at block top, `/* */` comments), K&R rules applied: `s32` return types where the asm falls off an int function, K&R `()` prototypes for callees whose arguments m2c invented.
- Section 4/8: no renames; new globals keep their splat names; GameState accessed only through fields (`migrate_globals.py --check` reports `globals OK`). Overlay-only data stays in overlay headers; main-range symbols were added to `include/main_api.h` (`sync_protos.py --check-branch`: 0 new disagreements).
- Section 7: the three FAKE kinds above are marked with `FAKE` comments and listed here; the `(u32)` selector cast in BUNKA_SD `func_80134B88` and the `s32` return types are real types, not tricks.
- Section 8a: no duplicate declarations (`check_headers` passes inside ninja).
- Findings: none open.
