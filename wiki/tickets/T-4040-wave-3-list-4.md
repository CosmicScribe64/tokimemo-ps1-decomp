---
id: T-4040
title: Wave 3: list 4
status: Done
assignee: wave-3 agent 4
created: 2026-10-09
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal
Match the 119 functions (25412 bytes) of wave-3 work list 4 (main 8006CB30 and overlays BUNKAKEN, DATE, ENDING, EVENT, GEKO, KANGEI, RPG_BAT, SHOUGATU, TACO, TAIIKU, TT), best first.

## Acceptance criteria
- [x] Work list done or time-boxed out; T-0018 shapes reverted to INCLUDE_ASM with a row in wiki/data/t0018-cases.md
- [x] Clean build, 27/27 OK, build/headers.ok
- [x] Inline code review against CODING_STANDARDS.md recorded below

## Notes
Result: 44 functions matched (4924 bytes): 40 of the 119 on the list (4624 of 25412 bytes) and 4 siblings from `queue.py --by bytes` in the same files. 28 register-order rows appended to [[data/t0018-cases]]; patterns and the other failures are in [[matching-notes]], section "Wave 3, list 4 (T-4040)". Five FAKE comments: three index views of consecutive globals (DATE `func_80151C54`, `func_80152CE0`, `func_80156B20`), two unused-local frames (KANGEI `func_80138640`, TACO `func_801438F0`).
Top non-T-0018 blockers: frame size of functions with spilled locals and tables (a leading scalar grows the frame by 8 where the original is 4 lower), loop unroll decisions (`p != &D_xxx` loops), store scheduling of consecutive globals.

## Comments
Inline review (no sub-agents), 2026-10-10, against the CODING_STANDARDS checklist:
- Matches verified: every kept function was compared with `funcdiff.py --resolve` against the all-INCLUDE_ASM objects and the clean build (`rm -rf asm build; configure.py; ninja`) ends with all 27 sha1 lines OK and `build/headers.ok`. String functions (`S`) were checked by the overlay sha1, since `funcdiff.py` cannot resolve `.rodata`.
- No `NON_MATCHING` blocks added; failed attempts were reverted to `INCLUDE_ASM` in place.
- C89: declarations at block top, `/* */` comments only; no inline, bool or designated initializers.
- Types and headers: new main-exe symbols went into `include/main_api.h` in address order, overlay symbols and prototypes into `include/ovl/<NAME>.h`; no overrides added; unused speculative declarations removed; `tools/check_headers.py` passes in ninja.
- Fakematches: five, each with a `FAKE` comment (see Notes). The `u8` local, post-increment, `/ -64` and counter-first loop forms are plain C.
- Placeholders kept (`func_XXXXXXXX`, `D_XXXXXXXX`); renamed callees use the O.BIN names already in `config/obin_renames.txt`.
- Wiki: ticket, kanban, log, matching-notes section and t0018 rows updated. No game data staged.
Findings: none open.
