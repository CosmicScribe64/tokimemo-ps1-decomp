---
id: T-4030
title: "Wave 3: list 3"
status: Done
assignee: agent (w3-3)
created: 2026-10-09
updated: 2026-10-10
links: ["[[matching-notes]]", "[[decompile-workflow]]", "[[data/t0018-cases]]"]
---

## Goal

Match the 118 functions (25936 bytes) of wave-3 work list 3 in main-exe files 80046500, 80049FF0, 8004AD60, 8004E500, 800563F0, 80058D20, 80059710 and overlay files of DATE, ENDING, EVENT, GEKO, GYOZI, KANGEI, NAME_ENT, RPG_BAT, SHOUGATU, SHUGAKU, TACO, TAIIKU, TT. Work in worktree w3-3 (branch w3-3). Not merged or pushed.

## Acceptance criteria

- [x] Work list processed in order, time-boxed per function (41 matched: 38 list functions, 5464 bytes, plus 3 from the byte queue, 380 bytes)
- [x] Clean build (`rm -rf asm build; configure.py; ninja`, no -k): 27 of 27 OK, build/headers.ok
- [x] T-0018 cases appended to `wiki/data/t0018-cases.md` (15 rows, tagged w3-3)
- [x] Inline review against CODING_STANDARDS.md recorded below

## Notes
Result: `ninja progress` grand total 2805 -> 2846 of 6958 functions (+41), 310888 of 2279368 bytes. Matched: GYOZI 7 (`func_80143110`, `80143314`, `801433E0`, `80143520`, `801435C4`, `80143940`, `801439F0`), ENDING 3, TT 1, TAIIKU 4, DATE 4, SHOUGATU 5, GEKO 2, NAME_ENT 5, KANGEI 1, RPG_BAT 1, EVENT 1, main 7. Patterns, open gaps and a `funcdiff.py` bug (wrong overlay picked for names that exist in several overlays: prints MATCH for any body): [[matching-notes]], section "Wave 3, list 3 (T-4030)".

Fakematches (both marked `FAKE` in the C): ENDING `func_801336A8` (`s32 pad` above the RECT), TAIIKU `func_80138A34` (`s32 pad` second word above the table).

New declarations: main-exe symbols in `include/main_api.h` only (arrays and scalars at 0x8007E7D0, 0x800E36xx, 0x800E643E/6476, 0x800E7xxx, 0x800EECB4, 0x80122640; prototypes `func_80051A68`, `func_80059938`, `func_80084D3C`, `func_800AE090`, `func_801394EC` is overlay-local in `GEKO.h`), overlay symbols in `include/ovl/<NAME>.h` (new structs: `RpgStat` in RPG_BAT.h).

## Comments
Inline code review (2026-10-10, no sub-agents) against CODING_STANDARDS.md, over `git diff f69eece -- src include`:
- Match rule (1): every match proved by the clean-build sha1 of its object/overlay; no `NON_MATCHING` blocks added.
- C89 (2): declarations at block tops, `/* */` comments only, no `//`, no `inline`/`bool`.
- Types (8): fixed-width types, struct offsets documented (`RpgStat`), unknown fields `unk_XX`/`pad_XX`.
- Shared externs (8a): all main-exe symbols are declared once in `include/main_api.h` (`tools/check_headers.py` passes: `build/headers.ok`); no override added, the TAIIKU/pad experiments with overrides were reverted.
- Fakematches (7): two, both `FAKE`-marked with reason and ticket (above); the `level` copy in `func_8004AC18`, the `kind` byte in `func_8013E410`, the `sx/sy` locals in `RangeMouse` and the grouped globals (`RpgStat`, `D_800E6476[]`, `D_800E643E[]`) are used variables/real declarations that IDO rules reward, explained in comments or in the notes section.
- INCLUDE_ASM (6): reverted attempts restored to the original `INCLUDE_ASM` line in place.
- No game data, `asm/`, `build/`, `expected/` staged. Wiki: ticket, kanban, log, matching-notes and t0018-cases updated; no page added, so `index.md` unchanged.
No open findings.
