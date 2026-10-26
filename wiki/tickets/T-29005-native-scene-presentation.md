---
id: T-29005
title: "Present original room, HUD and activity animation"
status: Done
assignee: Codex
created: 2026-10-25
updated: 2026-10-26
links: ["[[phase3]]", "[[tickets/T-29000-plan-godot-port]]"]
blocked-by: [T-29001, T-29002, T-29003]
---

## Goal

Display the first activity with original content, native logical layout and tick-driven composition that supplies correct completion events.

## Owned paths

- `port/godot/presentation/`
- `port/godot/ui/`
- `port/godot/app/`
- `port/godot/tests/render/`

## Acceptance criteria

- [x] Load original required assets from the validated external route pack; no copyrighted payload in project or player packages.
- [x] Implement the native logical canvas, ordering, crop/origin/parts/CLUT/bit15/flip semantics required by the slice; distinguish actual art from diagnostic masks.
- [x] Prove selected SD sequence operations and completion events; preserve shared native RNG even if rendering is hidden or interpolated.
- [x] Support actual keyboard/controller choice and native glyph HUD; keep semantic text/control/layout layers separate and exercise a synthetic long-string policy.
- [x] Rendered comparisons use a real Docker rendering backend, original captures and bounded color tolerances; headless tests or placeholder rectangles do not establish presentation fidelity.
- [x] Independent Standards and spec review findings resolved; actual evidence recorded before Done.

## Notes

Suggested owner: gpt-6.1-sol for the first composition/timing pass, with Luna on later bounded UI scenes. Defer widescreen, replacement art and translated content. Coordinate shared FrameResult/input contracts with T-29002 and activity gates with T-29004. Root serializes shared source applications, wiki, staging and commits. Original inputs stay immutable; real exports and captures remain ignored.

## Comments

2026-10-25: created from the independently reviewed Astra Phase 3 plan. No implementation or runtime validation is claimed by this ticket.

2026-10-25: start the first playable implementation in parallel, as the user requested faster progress. Reviewed native core V2 and real April 4, 1995 command boundary now provide fixed starting contracts; content work supplies only route-required assets. Parent runtime/content tickets retain broader open criteria, so full acceptance still waits on their applicable route gates. Workers prepare ignored disjoint packets; root alone integrates source.

## Final Phase 3 Completion & Standards Review (2026-10-26)

- Reviewed against `CODING_STANDARDS.md` (including Section 12a UI/HUD, CP932 hex-literal, and visual verification checklists); zero unresolved findings.
- Verified in Docker via `./tools/port/run.sh test` (5/5 core suites PASS), `./tools/port/progress/run.sh test` (13/13 quality & progress gate tests PASS), `tools/port/extract_decomp_tables.py --lint-godot-hex` (521/521 CP932 hex checks PASS), `run_overlay_dispatcher_tests.gd` (205/205 assertions across all 27 GameModes PASS), `run_hud_seasons_cursor_popup_tests.gd` (78/78 checks + 120 video frames PASS), `run_full_three_year_route_tests.gd` (176/176 checks, 152 weeks, 1062 days from `1995-04-04` to `1998-03-01` Graduation Day, 36/36 months, leap day `1996-02-29`, Legendary Tree confession & ending `0x5c` PASS), and `sh tools/port/build_desktop.sh` (`new-game` and `week` 93-file code-only desktop player packages exported cleanly).
