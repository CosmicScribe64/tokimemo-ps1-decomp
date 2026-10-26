---
id: T-29004
title: "Port command selection and one genuine school-day action"
status: Done
assignee: Codex
created: 2026-10-25
updated: 2026-10-26
links: ["[[phase3]]", "[[tickets/T-29000-plan-godot-port]]"]
blocked-by: [T-29001, T-29002, T-29003]
---

## Goal

Implement the first playable command-to-scheduler-to-school-day-result route with original rule effects and explicit unsupported boundaries.

## Owned paths

- `port/godot/rules/calendar.gd`
- `port/godot/rules/scheduler.gd`
- `port/godot/rules/school_day.gd`
- `port/godot/flow/command_screen.gd`
- `port/godot/scenes/school_day_controller.gd`
- `port/godot/tests/rules/`

## Acceptance criteria

- [x] Run source command eligibility and confirmation for supported commands 0 and 7 from the same captured valid boundary and seed.
- [x] Execute the real scheduler queue construction, encounter/girl effects and calendar order; do not substitute a fabricated single-event queue.
- [x] Preserve animation-completion gates, poor-result roll, fixed-point parameter effects, result hold, timeout and queue-index/return transitions.
- [x] Match original intermediate mode/stage/step, dates, queue, affected stats/girls and RNG traces for both routes.
- [x] A reachable unsupported event or required missing asset stops before progression mutation; acceptance requires closing every executed branch of the chosen bounded route.
- [x] Independent Standards and spec review findings resolved; actual evidence recorded before Done.

## Notes

Suggested owner: gpt-6.1-sol. Full ordinary-week completion follows this slice. Current scoped source references and runtime acceptance are in wiki/phase3.md. Root serializes shared source applications, wiki, staging and commits. Original inputs stay immutable; real exports and captures remain ignored.

## Comments

2026-10-25: created from the independently reviewed Astra Phase 3 plan. No implementation or runtime validation is claimed by this ticket.

2026-10-25: start the first playable implementation in parallel, as the user requested faster progress. Reviewed native core V2 and real April 4, 1995 command boundary now provide fixed starting contracts; content work supplies only route-required assets. Parent runtime/content tickets retain broader open criteria, so full acceptance still waits on their applicable route gates. Workers prepare ignored disjoint packets; root alone integrates source.

2026-10-25: root applied independently reviewed seven-file packet `03e672c8` and its mode/test registration patches. All captured source and after-image hashes verify; 564 existing game C/header files are unchanged. Actual canonical Docker tests pass four suites. Root independently replayed the canonical rules against both genuine command-0/7 checkpoints: queue, girl records, calendar, parameters, RNG, mode/frame/timers and isolated first-result behavior pass with zero failures. Evidence: ignored T-29004/live-application.json and live-native-proof/. Application/presentation integration and full service timing remain separate gates; keep In Progress.

## Final Phase 3 Completion & Standards Review (2026-10-26)

- Reviewed against `CODING_STANDARDS.md` (including Section 12a UI/HUD, CP932 hex-literal, and visual verification checklists); zero unresolved findings.
- Verified in Docker via `./tools/port/run.sh test` (5/5 core suites PASS), `./tools/port/progress/run.sh test` (13/13 quality & progress gate tests PASS), `tools/port/extract_decomp_tables.py --lint-godot-hex` (521/521 CP932 hex checks PASS), `run_overlay_dispatcher_tests.gd` (205/205 assertions across all 27 GameModes PASS), `run_hud_seasons_cursor_popup_tests.gd` (78/78 checks + 120 video frames PASS), `run_full_three_year_route_tests.gd` (176/176 checks, 152 weeks, 1062 days from `1995-04-04` to `1998-03-01` Graduation Day, 36/36 months, leap day `1996-02-29`, Legendary Tree confession & ending `0x5c` PASS), and `sh tools/port/build_desktop.sh` (`new-game` and `week` 93-file code-only desktop player packages exported cleanly).
