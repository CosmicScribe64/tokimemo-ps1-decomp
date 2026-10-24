---
id: T-27017
title: Phase 2 naming and spec for m-sysmenu (src/main/8005A0B0.c, the player's room screens)
status: In Review
assignee: Sonnet 5.5 analyst (workflow p2a)
created: 2026-10-24
updated: 2026-10-24
links: [[[tickets/T-27000-phase-2]], [[spec/m-sysmenu]]]
---

## Goal

Name every function and global of `src/main/8005A0B0.c` (109 functions: the weekly command screen, calendar check, memory-card data screen, system settings and mascot, club joining, rumor report, Christmas and New Year preparation), the fields and types it relies on, and write the port spec. Phase 2 scope: game logic named and explained, hardware layer at module level only. The work list called the unit "system menus, clubs, rumors, holiday prep".

## Acceptance criteria

- [x] `config/renames/proposals/m-sysmenu.tsv`: one row per rename (kind scope old new note), a comment row for each of the 16 O.BIN-named functions, hypotheses marked `HYPOTHESIS:`; 264 rows (92 func, 27 data, 1 field, 126 const, 16 comment, 2 file). `tools/rename.py plan` on its func, data and field rows: exit 0, 118 rows, 45 files, no collision.
- [x] `wiki/spec/m-sysmenu.md`: what each screen does, its state and data with the new names, the rules (build rules of the command screen, icon table, save and load steps, settings tables, rumor and preparation rules), connections, peculiarities and dead code, prefix requests, open questions; linked from `wiki/spec/index.md`.
- [x] Emulator checks from the HUD unit's saved state of the command screen (1995-04-09): icon to stage mapping, settings flow and frame numbers, calendar, the data screen through save and load, the click spots in mouse-like mode, icon 0 to mode 0x30, icon 4 without a club does nothing, d-pad icon order, click spots drawn on a screenshot.
- [x] Facts only, no game text, no asm, no screenshots, nothing from `disc/`, `versions/`, `build/`. No edit of `src/`, `include/` or symbol files.
- [ ] Review of names, evidence and hypothesis marks (Opus), then merge the table into the rename tool input.

## Notes

- The file is one original object: every screen drawn over the player's room picture. File proposal `src/main/home_room.c`, header `include/main/home_room.h`; new prefixes requested in the spec section 13: `command_screen_`, `room_`, `save_load_` (with Module words rows).
- O.BIN: 16 applied names kept (comment rows). The developer table also holds the unapplied names of this block (`game_mode*`, `data_save_load*`, `system_config_*`, `system_mascot_*`, `uwasa_set`, `pre_xmas_main`, ...) in the same order as the exe functions; they are quoted in the notes as evidence and not used as names (same policy as [[tickets/T-27014-p2-m-dates-a]]).
- Found while reading: weekday club joining (stage 5) cannot be reached because icon 4 is locked for a clubless player; `func_8005A410` has no caller; the click speed change always yields 0x10; the mascot menu's start cursor is one item off; the two mascot click cooldown stamps are never written; a load of a first-day or ending-day save starts a new run (mode 0x14); the memory-card screens run in the emulator against the empty virtual card (the check takes about 480 frames), including save, load and the stage 0xFF reload, which builds the screen twice when the room picture is cached.
- Shared names: core's `mode_command_screen`, `card_start`, `card_stop`, `girl_cleared`, `mascot_choice`, `scratch_words`; HUD's `icon_*`, `mascot_set`, `month_label_strings`, `slot_info_from_system_data`, `room_bg_sectors_weekday` (weekday unit). One cross-unit overlap to settle: `CharFlags.b2` is `club_member` in the holiday table (used, no row here); `GsRec66C.unk_03` gets `quit_flags` here.

## Comments

- 2026-10-24 (workflow p2a): table, spec and this ticket written; status In Review.
