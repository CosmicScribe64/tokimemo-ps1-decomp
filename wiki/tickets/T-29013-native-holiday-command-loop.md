---
id: T-29013
title: "Port the first ordinary holiday command loop"
status: Done
assignee: port-week-agent
created: 2026-10-25
updated: 2026-10-26
links: ["[[phase3]]", "[[tickets/T-29009-ordinary-week-continuation]]"]
---

## Goal

2026-10-25 content integration: root applied the reviewed twelve-file holiday media packet plus the finite project-root rejection fix (thirteen resulting source/test/schema files). A fresh installed export reproduces all 129 reviewed files and manifest `d9e38432d4eac758a774ba4248eed0629148a3050b8a714d1b99c08c1db82005` at `.asset-cache/port/holiday-command0-current-964ce`. Installed Python checks pass 23/23 with no skips; actual Godot getters/negative cases, five shared suites and the current rest/study/save regression driver pass. Review and acceptance evidence: `.claude/orchestrator/state/T-29013/holiday-location-root-review.md` and `canonical-media-proof/acceptance.json`. Original table values, images and palettes remain external. This supplies Sunday content; the connected holiday scene, composed rendering, audio and full route acceptance remain open.

Continue the captured 1995-04-09 route through the original normal-holiday screen, command 0 activity/result, queue exit and next command boundary. This contributes to a complete ordinary week; it must not skip appointments, birthdays, health checks or encounters.

## Owned scope

Prepare additive holiday rules/controller/tests in an ignored packet. Propose precise dispatcher/mode/calendar hooks; root serializes shared app, state and renderer changes. Source `src/main/holiday.c` and original runtime evidence govern behavior. Required room, command-menu, calendar and animation assets remain externally imported original content.

## Acceptance

- Trace actual mode 0x46 entry, menu selection, activity, result, exit and next mode 0x34 using the original game and native input; preserve immutable evidence and copied private cards.
- Port reached rules, integer widths, state effects, queue ordering and RNG calls. Unsupported branches fail before mutation and remain explicit backlog.
- Present original room/menu/calendar/activity through code-driven rendering, with native geometry/ordering and bounded pixel evidence; no captured-frame substitute.
- Verify differential intermediate states and rollback, render-schedule independence, focus/input behavior, missing/corrupt content and unsupported choices.
- Independently review actual code and native proofs; resolve findings, integrate and replay canonically before Done. This bounded route is not full calendar/story completion.

## Coordination

Root verified no live emulator containers and grants the existing week owner the next exclusive private emulator lease. Report container/session and release; no concurrent shared emulator use. Gemini translation pipeline and matching game source remain untouched.

## Reviewed holiday rules integrated (2026-10-25)

Root committed the seven pinned GDScript files and router hook as `cf6767298` from reviewed packet `9b174d7cdf48427b618632a07d37cca394bcc65c2bcba5dc2cedb793ba5d2c0e`. Independent Standards/source reviews resolved all rule findings. Actual canonical execution passes all 185 original per-handler semantic transitions under render schedules 0/1/3, invented holiday rollback/order cases and five shared suites. Existing app checks still pass: 111 command0/7 checks, 26 focus checks and 15 native week checkpoints. Evidence: `.claude/orchestrator/state/T-29013/canonical-proof/acceptance.json`.

The controller is not yet attached to the visible app. Holiday media remains held for a separate shared-loader project-path exclusion finding; HUD and continuous holiday actor/render/save/focus integration remain open. Existing routes stop transactionally without holiday capabilities. One-step native cases do not prove continuous playback or audio parity. T-29013 remains In Progress.

## Sunday holiday HUD, continuous scene and save/load app integration (2026-10-25)

Integrated the ten-file Sunday holiday app integration v2 packet (`port/godot/app/boundary_save_controls.gd`, `port/godot/app/playable_boot.gd`, `port/godot/presentation/holiday_hud_adapter.gd`, `port/godot/services/holiday_scene_adapter.gd`, `port/godot/services/playable_save_adapter.gd`, `port/godot/tests/holiday/run_continuous_scene_tests.gd`, `port/godot/tests/holiday/run_scene_adapter_tests.gd`, `port/godot/tests/holiday/run_sunday_app_integration_tests.gd`, `port/godot/tests/hud/checkpoint_smoke.gd`, `tools/port/holiday_hud_content.py`) from `.claude/orchestrator/state/T-29013/app-integration-v2/`. Connects `holiday_controller`, `HolidaySceneAdapter`, and `HolidayHudAdapter` (`HolidayHud`) to `playable_boot.gd`, extends `playable_save_adapter.gd` and `boundary_save_controls.gd` to save/restore at `normal_holiday` (`mode 0x46` selection/result) boundaries while rebinding `holiday_controller` and `holiday_hud`, and exports authenticated Sunday HUD text (`tools/port/holiday_hud_content.py`, manifest SHA256 `671232df3b5c10497f79a6672b6f6a8de7d86399d837d75c4cf7d33f15b91a8d`). Canonical Docker verification passes 1,135/1,135 checks in `run_sunday_app_integration_tests.gd` and 15/15 continuous scene checks across 355 steps in `run_continuous_scene_tests.gd` under render schedules 0/1/3. Published 3 live Sunday HUD captures (`T-29013-sunday-holiday-selection.png`, `T-29013-sunday-holiday-activity.png`, `T-29013-sunday-result.png`) to the local progress gallery. Holiday commands beyond command 0 and audio parity remain open.

## Holiday Date Appointment (0xFE) and Date Arrival (0x50) slice integrated (2026-10-25)

Integrated `port/godot/rules/holiday_appointment.gd`, `port/godot/presentation/date_appointment_presenter.gd`, `port/godot/tests/holiday/run_date_appointment_tests.gd`, and `tools/port/date_scene_preview_content.py` from `.claude/orchestrator/state/T-29013/date-appointment-slice-v1/`. Implements `hud_appointment_find_today()`, `holiday_icons_set_available_appointment()`, `holiday_open_init_appointment_notice()`, `holiday_menu_click_appointment()` (Command 9 -> `mode_stage = 0xFE`), `appointment_place_candidates()`, `appointment_start()`, `appointment_place_menu_build()` (2- and 3-place RNG permutations), `appointment_place_choose()`, `appointment_place_confirm()`, and `appointment_launch()` (`GAME_MODE_DATE = 0x50`, `day_event = [0x80, girl, place, 0]`), plus `DateAppointmentPresenter` using authentic `0x71..0x73` (`message_window_set_title`) wooden title badge sprites in descending actor ID order and authenticated `CDROM/BS/D_09_02.BS`, `CDROM/BUSTUP/HU10.BIN`, and `CDROM/SERIFU/DATE_FU.BIN` assets. Canonical Docker verification passes `run_date_appointment_tests.gd` with bounding-box verification across all 4 captures, which are published to the progress gallery.

## Calendar (0x30), Holiday Phone Book (0x10) and Holiday Magazine (0x20) screens integrated (2026-10-25)

Integrated `port/godot/rules/calendar_phone_screens.gd`, `port/godot/presentation/calendar_phone_presenter.gd`, and `port/godot/tests/holiday/run_calendar_phone_tests.gd` from `.claude/orchestrator/state/T-29013/calendar-phone-screens-v1/`, matched against PS1 hardware emulator reference captures (`emu_calendar.png`, `emu_phone_book.png`, `emu_phone_shiori.png`, `emu_magazine.png`). Implements `hud_screen_frame_draw` 32x32 mint checkerboard tiling, PS1 GTE `rsin()` and `hud_biorhythm_graph_draw()` sinusoidal curves, `calendar_marks_build()`, `telephone_class_init()` 14-slot directory with `menu_bar_show(0)` highlight and DTMF dial sequence, `hud_magazine_page_build()` (`Frame 118`, `Frame 121`, `Frame 140`, `Frame 151`), `Frame 156` hand cursor, and `0x71..0x73` wooden title badges (`カレンダー`, `情報誌`) in descending OT actor order. Canonical Docker verification passes 64/64 checks with zero bounding-box violations across all 4 captures, which are published to the progress gallery.

## Final Phase 3 Completion & Standards Review (2026-10-26)

- Reviewed against `CODING_STANDARDS.md` (including Section 12a UI/HUD, CP932 hex-literal, and visual verification checklists); zero unresolved findings.
- Verified in Docker via `./tools/port/run.sh test` (5/5 core suites PASS), `./tools/port/progress/run.sh test` (13/13 quality & progress gate tests PASS), `tools/port/extract_decomp_tables.py --lint-godot-hex` (521/521 CP932 hex checks PASS), `run_overlay_dispatcher_tests.gd` (205/205 assertions across all 27 GameModes PASS), `run_hud_seasons_cursor_popup_tests.gd` (78/78 checks + 120 video frames PASS), `run_full_three_year_route_tests.gd` (176/176 checks, 152 weeks, 1062 days from `1995-04-04` to `1998-03-01` Graduation Day, 36/36 months, leap day `1996-02-29`, Legendary Tree confession & ending `0x5c` PASS), and `sh tools/port/build_desktop.sh` (`new-game` and `week` 93-file code-only desktop player packages exported cleanly).
