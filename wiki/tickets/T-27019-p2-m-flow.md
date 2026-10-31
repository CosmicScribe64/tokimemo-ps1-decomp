---
id: T-27019
title: Phase 2 naming and spec for m-flow (boot, main loop, mode machine, overlay loader, title, pad, memory card)
status: Done
assignee: Sonnet 5.5 analyst (workflow p2a)
created: 2026-10-24
updated: 2026-10-24
links: `[tickets/T-27000-phase-2`, `spec/m-flow`]
---

## Goal

Name the game-logic functions and globals of the main-exe flow unit and write the port spec for it. This pass writes tables and wiki pages only: no change to `src/`, `include/` or the symbol files.

Files: `src/main/80041000.c`, `800420D0.c`, `80042540.c`, `80042A00.c`, `800789E0.c`, `80061710.c` and `80053650.c` (115 functions): graphics bring-up and `game_state_init` with the new-game data, the executable entry and frame loop, the mode dispatcher and setters, pad and mouse polling with the soft reset and the secret code, the overlay loader with its sector and entry tables and the TT loader, the title screen with its 3D card intro and the start mode, the memory-card layer with the system-file and game-save images.

## Acceptance criteria

- [x] `config/renames/proposals/m-flow.tsv`: one row per rename (kind scope old new note), a comment row for each O.BIN-named function touched, hypotheses marked, one name per (unit, address), no collision with names in the core, h-graphics, h-cdio, m-hud, m-scheduler, m-dates-a tables or the sibling worktree tables; `tools/rename.py plan` on the func, data, field and type rows reports no problem.
- [x] `wiki/spec/m-flow.md`: what the unit does, state and data with the new names, rules as coded, connections, open questions; linked from `wiki/spec/index.md`.
- [x] Facts only, no game text, no asm, no screenshots, nothing from `disc/`, `versions/` or `build/`.
- [x] Review (an Opus pass checks the table and the spec against the C; no `src/` change, so no `CODING_STANDARDS.md` findings).
- [ ] Merge the table into the rename tool input (not part of this branch).

## Notes

- Evidence: the matched C and every caller in `src/`; the core table and spec (branch p2a-core, merged into the base), the h-graphics, h-cdio, m-hud, m-scheduler and m-dates-a tables, the sibling m-sysmenu table (for the shared `CardResult` names), and the emulator (`tools/emu`: states made with `boot` to the opening and to the title; probes: mode byte and stage writes through a whole title run, the loader read and state changes, the pad code, the soft reset, the card check frames, initial values of the calendar, player and girl records).
- Main findings: the title is a 3D intro (13 photographs over a still) with a 601-frame wait, a flash, a logo and a two-item menu, and an 18-step pad code at the menu jumps to the opening movie; the overlay sector and entry tables are decoded in full (48 overlay modes, 24 disc files, EVENT and GYOZI are not in them); the loader skips the read when the same file is resident, so overlay statics survive mode changes; the memory-card layer is a poll-per-frame state machine over two banks with a byte-sum check, and both save images are listed field by field; the entries of modes 0x74-0x76 in core's `MainMode` rows are in a different order than the loader code has them (question 1 of the spec).
- Naming: core names are used as given; new prefixes `boot_`, `timer_`, `sys_`, `pad_`, `mouse_`, `ovl_`, `title_`, `card_`, `mdec_` (`card_` instead of the ADR row's `memory_card_`; `timer_` moves to the 800420D0 row, see spec section 1). Ten rows carry HYPOTHESIS marks (three from the first pass, seven added after review: the five overlay-folder `OvlLba` constants the ADR map keeps as `confirm`, `CARD_RESULT_IO_ERROR` and `extra_mode_flags`). The hardware calls (libgs, libgpu, libcard, libpress) are left as `func_` placeholders; the two MDEC helpers and the card events are named at module level.
- Not done: no SDK placeholder was renamed; the alias labels inside actors and layers that the title writes (`D_801206xx`, `D_801220E0`) stay for element access; the frame at which the card jobs end in a real save was not run (the emulator's card check keeps retrying).

## Comments

- 2026-10-24 (Sonnet 5.5): table (324 rows: 87 func, 48 data, 44 field, 1 type, 10 comment, 14 file, 120 const; 3 HYPOTHESIS rows) and spec written, In Review.
- 2026-10-24 (Opus 5.5, review round 1 fix): the review blocked on missing HYPOTHESIS marks. Added them to `OVL_LBA_SUMMER_CAMP`, `OVL_LBA_CULTURE_FESTIVAL_TOUR`, `OVL_LBA_GIRL_ENCOUNTERS`, `OVL_LBA_NEW_YEAR_CHRISTMAS`, `OVL_LBA_MISC_SCENES`, `CARD_RESULT_IO_ERROR` and `extra_mode_flags` (table, and spec sections 3.4, 3.7, 6.1, 7). Wording fixes from the non-blocking findings: `card_file_exists` open counts (9 and 17 opens at most), the pad code window (15 frames between steps, 241 frames set), the title idle clock restarting on every up or down press, the callers of `pad_update`, the unnamed display call in TT stage 2, and the `pad_raw_read` no-device path. Merge notes added as spec open questions 11 and 12. Table still 324 rows, 10 with HYPOTHESIS; `tools/rename.py plan` on the func, data, field and type rows reports no problem. Back to In Review.
- 2026-10-24 (Opus 5.5, review round 2, approved, no findings): the one blocking finding is resolved. Commit `0cdb9358c` adds HYPOTHESIS marks to all seven flagged rows of `config/renames/proposals/m-flow.tsv`: `OVL_LBA_SUMMER_CAMP`, `OVL_LBA_CULTURE_FESTIVAL_TOUR`, `OVL_LBA_GIRL_ENCOUNTERS`, `OVL_LBA_NEW_YEAR_CHRISTMAS`, `OVL_LBA_MISC_SCENES`, `CARD_RESULT_IO_ERROR` and the `extra_mode_flags` data row. Each note starts with HYPOTHESIS: and says which part is unproven and which part (disc file, LBA, modes) is fact. The table has 324 rows (325 lines with the header), every line has 5 tab-separated columns, and 10 rows carry HYPOTHESIS. In `spec/m-flow` the marks appear on the 15 affected rows of the section 3.4 mode table (with a paragraph under it), on the extras-menu claim in TT stage 0, on `CARD_RESULT_IO_ERROR` in 6.1 and on the `extra_mode_flags` row in section 7. The touched wording was checked against the C and the C backs it: `card_file_exists` opens at most 9 times for a save file (1 open plus 8 retries) and 17 times for the system file (1 plus 16); `pad_code_update` sets the timer to 0x10 on a match and decrements it in the same call, so the next step must match within 15 frames, and after the final match the timer counts up from 0x0F until the u8 wraps, giving 241 frames with the flag set; the title menu idle clock restarts on every pad up or down press. The branch changes only the table and wiki files: nothing under `src/`, `include/` or `config/symbol*` (checked against the merge base). Set Done. The last acceptance item (merging the table into the rename tool input) stays open and is not part of this branch.
