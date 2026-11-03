---
id: T-39504
title: Script an emulator route through the main screens with screenshots
status: Done
assignee: claude
created: 2026-11-01
updated: 2026-11-02
links: ["[[tickets/T-39500-english-translation-patch]]"]
---

## Goal

A repeatable emulator run that visits the main screens (title, options, name entry, opening, first school day, a date, a phone call, the calendar, status, save and load) and captures screenshots, for any disc image, so the Japanese and English discs can be compared side by side.

## Acceptance criteria

- [x] The route runs headless from a cue path and writes a contact sheet.
- [x] Baseline screenshots from the original disc (kept out of git).
- [x] The route drives an English build with the English letter page as the default name entry page. (`build/emu/route-en-banner`: fresh boot through save/load, 33,649 frames and 146 captures; name-entry, birthday and dialogue fixes visually checked.)

## Notes

2026-11-02: `tools/emu/routes/holiday-magazine.route` adds a focused normal-input magazine visit from a compatible main-route `holiday_1` checkpoint. `build/emu/route-en-magazine-v2` passes in 11 s, captures the complete first issue and returns to the holiday menu. Page-aware `talk` input prevents reopening the magazine after exit. This supplements the main route; it does not establish coverage of later issues or all off-route events.

Part of [[tickets/T-39500-english-translation-patch]]. Emulator: [[emulator]] (section Route).

Files: `tools/emu/route.py` (route parser, builds, runner, contact sheet, compare), `tools/emu/route.lua` (step player in PCSX-Redux), `tools/emu/routes/main.route` (the route), `tools/emu/test_route.py` (20 tests, no game data). `tools/emu/run.sh route ...` runs it; `EMU_CUE=/path/disc.cue` mounts a cue outside the repository read-only (cues inside it: `--iso`/`--cue`, the alias from T-39501).

How it waits: GameState scene bytes (`game_mode` 0x800E7388, `mode_stage`, `mode_step`, `substep`, loader mode 0x800E738E), the menu selection `menu_cursor_0/1` (0x800E7313, stepped with the d-pad in the default keyboard menu mode), the free cursor `cursor_x/y` (name entry, album slot) and the typewriter `text_state` (0x800B3F60: one screenshot per whole page). No fixed frame counts except short gaps after a press.

Findings on the way (original disc, OpenBIOS):
- The title menu is mode 0x12 stage 2 (the title intro is stage 1); options are mode 0x20 (menu ready at step 2); file select 0x13 (stage 2 step 0x41 offers a new album on an empty card, 0x45 picks the album slot: slot i is marked at x = i / 7 * 0x1F - 0x82, y = i % 7 * 16 - 0xB, and the cursor must be inside that cell); name entry 0x14 stage 1 steps 1/3/5/7/9 (family, given, nickname, birthday, confirm), stage 2 Shiori's profile; prologue 0x15 (about 13 600 frames, paced by the script, not by text or voice); the command screen 0x34 (icons 0-7 activities, 11 system settings, 12 calendar, 13 save/load; save is unusable on the first evening); weeks run 0x30, 0x31, 0x40 per day, 0x32 at the week's end; Sunday is 0x46 (icon 8 the phone, 9 the date when one is due, which the menu then preselects); a call is 0x45 (step 0x20 calendar, 0x30 date spot); a date is 0x50.
- Shiori's answer to a date is random: the route calls again the next Sunday (`goto`, three tries). Small timing changes change the answer (one baseline run was refused once, the final one accepted at once).
- The date start (holiday stage 0xFE) waits for a button after its message.
- Slot 1 of the save list holds the album made at the file select, so the save asks to overwrite.
- Voice default is "cut with CROSS" (option value 2); the route sets OFF and the fast text speed.

Baseline (original disc, `build/disc-image/orig/tokimemo.cue`, 2026-11-01): `build/emu/route-jp/`, reason `done`, 27 922 frames, 539 s emulator wall time (about 9 min 15 s with Docker start), 144 screenshots. Checkpoints and frames: title_menu 2951, options 3416, options_text_speed 3460, options_voice 3608, options_voice_off 3700, title_menu_again 3940, file_select_new_album 4836, file_select_slot_1 4943, name_entry_page 5445, name_entry_family/given/nickname 5571/5797/5932, name_entry_birthday 5976, name_entry_confirm 6020, prologue_01..48 6060-19790, after_prologue 19794, command_screen 20050, calendar 20241, system_settings 20464, school_day 20742, week_1_01..18, holiday 22407, phone_book 22550, phone_call_01..07, phone_date_spot 23105, phone_answer_01..08, command_screen_evening 23785, save_menu 24404, save_slot 24626, save_overwrite 24670, save_writing 24689, save_done 24825, holiday_date 25567, date_01..13, after_date 26770, load_slot 27600, load_01..03, loaded 27922 (the command screen of the saved Sunday, 9 April). Every shot was checked by eye.

Not covered: a separate status screen (the command screen's top panel is the status: parameters and stress; the girls' status comes from calling Yoshio, which would use up the Sunday the date needs); the debug menus (title + RIGHT/LEFT held on options); the English run (no English disc yet).

## Comments

- 2026-11-01 claude: implementation and baseline done; moved to In Review. Needs a review against `CODING_STANDARDS.md` (scripts section) before Done.

## Final standards review

Reviewed route parsing, relinked addresses, state resumption, emulator invocation, checkpoint conversion and the Lua wait/talk termination paths. Required waits fail on timeout, a route succeeds only with reason `done`, and incomplete screenshot conversion leaves an explicit error. Talk capture waits for completed text reveal. Resume metadata preserves frame offsets and prior checkpoints; callers must still select a compatible saved state, as the controlled-fixture reports document.

All 21 route tests pass. Rechecked `build/emu/route-en-final-layout/result.json`: reason `done`, 33,649 frames and 146 checkpoints with no missing image files. That historical run verifies the tool's English route through save/load; later per-build receipts identify current-disc captures separately. This review makes no claim that every game route has been played. No unresolved standards findings remain.
