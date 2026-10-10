---
id: T-6010
title: Wave 4: list 1
status: Done
assignee: claude
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Wave 4 batch agent 1: match as many of the 127 listed functions (39860 bytes) as possible in the 35 owned C files (main 800451D0; overlays BUNKAKEN, BUNKASAI, BUNKA_SD, DATE, ENDING, EVENT, GEKO, GYOZI, RPG_BAT, SHOUGATU, TACO, TAIIKU, TEL, TT). Work list: scratchpad `wave4-list-1.txt`.

## Acceptance criteria

- [x] Work list processed in order, time-boxed per function.
- [x] Clean build (`rm -rf asm build; configure.py; ninja`): 27 of 27 OK, `build/headers.ok`.
- [x] T-0018 cases appended to [[data/t0018-cases]]; new patterns in [[matching-notes]].
- [x] Inline code review against CODING_STANDARDS recorded below.

## Notes

Result: 17 functions (3644 bytes) turned into C in the owned files (16 from the list, 3644 - 232 = 3412 bytes, plus TEL `func_801360E4` from the byte queue); `ninja progress` grand total 3539 -> 3556 of 6958 (448016 of 2279368 bytes), clean build 27 of 27 OK, `build/headers.ok`, `build/globals.ok`. 9 T-0018 rows added to [[data/t0018-cases]]. Method, patterns and the families that did not match: [[matching-notes]], section "Wave 4 list 1 (T-6010)". Fakematches: two, both marked `FAKE` (TT `func_8013EFAC` `s32 pad`, TACO `func_80151C30` `s32 pad[4]`). One `MAIN_API_OVERRIDE` (DATE 8015A190.c, `D_80122CD0` as `u32`, reason in the file).

## Comments

Inline review against CODING_STANDARDS (2026-10-10):
- Matches verified: every kept function was compared with `expected/` objects built from the all-`INCLUDE_ASM` tree (`funcdiff.py --resolve`) and the full `ninja` ends with all 27 sha1 checks OK. One early stale reference (DATE `80149310.o` copied after a funcdiff build had compiled my edit) was caught by the DATE sha1 and the object rebuilt from the asm tree.
- No `NON_MATCHING` blocks; unmatched functions stay `INCLUDE_ASM` (section 6).
- C89: declarations at block top, `/* */` comments, no inline/stdbool; bit-field structs use `u8`/`u32` members; one `goto` (TT `func_80132000`) with a comment, three paths join one block.
- Fakematches: two `FAKE` comments with reason and ticket (section 7); no other trick: no dummy statements, forced registers or odd casts. The `(u32)`/`U` suffixes and the `*arg2++` argument are ordinary C.
- Declarations: new overlay symbols went through `include/ovl/<NAME>.h` and `tools/sync_protos.py --fix` (main-exe symbols are declared once in `include/main_api.h`; one override, explained in the file). `sync_protos.py --check-branch` OK (0 new), `migrate_globals.py --check` OK. `--fix` and `--write` drop the comment line above `D_800EAFA0` in `main_api.h`; I restored it each time (tooling bug, below).
- Types and names: placeholders kept; no renames; new local types (`Ev44`, `Bits64B8`, `GyoziBits32`/`GyoziBits8`) are documented with sizes.
- No game data staged (`asm/`, `build/`, `expected/` untouched in the commits).
- Wiki: ticket, kanban, log, [[matching-notes]], [[data/t0018-cases]] updated.

Tooling notes for the orchestrator: (1) `tools/sync_protos.py --fix`/`--write` delete the comment line `/* 0x44-byte records like D_8011ECD0 ... (T-5000). */` above `extern u8 D_800EAFA0[];` in `include/main_api.h`; the line is restored by hand. (2) `tools/dupes.py --apply --check --unit TEL --func func_801360E4` printed "plans 0" for a byte-identical twin of the matched `func_80135FFC` (the matched count it prints, 3539, was the baseline), so the twin was copied by hand. (3) `tools/permute.py` takes `--unit` only after the subcommand (`permute.py all --unit EVENT func_X`); `permute.py --unit EVENT func_X` fails with an argparse error.
