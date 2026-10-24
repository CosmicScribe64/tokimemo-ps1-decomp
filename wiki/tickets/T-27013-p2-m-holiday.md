---
id: T-27013
title: Phase 2 naming and port spec for the holiday screen, appointments and telephone
status: In Review
assignee: Sonnet analyst (p2a-m-holiday)
created: 2026-10-24
updated: 2026-10-24
links: ["[[tickets/T-27000-phase-2]]", "[[spec/m-holiday]]", "config/renames/proposals/m-holiday.tsv"]
---

## Goal

Roadmap phase 2 (readable code), port-first scope, for unit `m-holiday` = `src/main/80075320.c` (the mode 0x46 handler): name its functions and data, name the fields and enums it relies on, and write the system spec the Godot port needs. This pass writes tables and wiki pages only; no source, header or symbol file is edited.

## Acceptance criteria

- [x] `config/renames/proposals/m-holiday.tsv`: 140 rows (28 `func`, 22 `data`, 4 `field`, 73 `const`, 12 `comment`, 1 `file`); every unnamed function of the file is named (28 of 41; 11 O.BIN names and the patch name `appointment_launch` stay with comment rows, `func_80075320` keeps the core name `mode_holiday`); all 22 data labels of the object's `.data` named; 2 rows marked HYPOTHESIS; header line as in `core.tsv`.
- [x] No key or name collides with the tables of the other merged units (checked against core, h-cdio, h-graphics, m-dates-a/b, m-hud, m-scheduler, o-rpgbat, o-taco, o-tt).
- [x] `wiki/spec/m-holiday.md`: stages, state with the new names, rules (icon availability, the activity, the club flows, the appointment place question, the telephone), data tables, connections, hardware layer at module level, quirks, open questions; linked from `wiki/spec/index.md`.
- [x] Emulator evidence for the main flows (see Notes).
- [x] No game text, assets, asm or screenshots committed; no edits under `src/`, `include/` or `config/symbol*` files.
- [ ] Code review against `CODING_STANDARDS.md` (docs-only change) and merge of the table by the phase 2 merge step.

## Notes

- Prefixes: main has no unit prefix; each module picks a subsystem prefix and keeps the O.BIN style. This unit uses `holiday_` (O.BIN: `holiday_tel*`, `holiday_club*`), `appointment_` (patch name `appointment_launch`) and `magazine_` (O.BIN: `magazine_exit`, `magazine_base_show`). Constants: `HOLIDAY_STAGE_*`, `HOLIDAY_SD_*`, `UI_SE_*` (bank-5 effects of the day scenes), `HUD_ICON_*` (extends the m-hud `HudIcon` rows), `MAGAZINE_HINT_*`.
- Method: read the matched C and its callers and callees; decoded the data tables of the object and the neighbouring tables it reads; compared with the tables of core, m-hud, m-scheduler, m-dates-a/b and dropped or renamed rows that those units already made (commands: `WeekdayCommand`; girl flag bits 2 and 18, club flag bit 24 and the remedial flag are theirs).
- Emulator (tools/emu, GDB session on a title-menu save state; the script stops at the main loop, forces mode 0x46 and pokes pad, cursor, calendar, appointment, club and girl values; states and logs stay in the gitignored `build/emu/`): icon availability at stage 0 (`icon_can_use_set` 9, 0xD, 4); command 0 end to end (sounds 3 and 0x500, `main_stage_set(5)`, background sector 0x3FFB, SD sector 0xA20F, `UI_SE_ACTIVITY_GOOD` and `parameter_up_down(0, 1, 0)` at frame 32, `day_event_next` at frame 65); phone-book item 12 (current girl 0xB, tones 0x504 then seven digit tones every 8 frames, TEL entered at stage 1 with slot 0xB); the button dial shortcut (ring 0,1,2,1,0,2,0, tokimeki bonus, straight to the hand-over); the appointment question with two places (options 1,3,5,0xFF; choosing item 1 left 5,3,5,0xFF; `main_state_set(0x50)`); the first club activity on a Sunday (`main_state_set(0x60, club)`, girl flag bit 18 set); the Sunday join list (stage 7, then stage 5); the sports-club third Sunday (cursor on icon 4, stage 6, `main_state_set(0x73)`).
- Findings worth reviewing first: (1) appointment-word place values are 0-based, not place + 1 as `spec/core` says; (2) `appt_place_0..3` are chosen place, true place and two decoys, so the date place question is a memory test; (3) mode 0x60 starts at the first club activity of a club (core calls it a camp); (4) the phone slot 11 stands for the rival; (5) the information magazine hint is gated by the secret-code latch `D_800B3CFC`; (6) a holiday command gives 4x the gains of a school day for 1x the cost.
- Open questions are in section 15 of [[spec/m-holiday]].

## Comments
