---
id: T-29002
title: "Implement native arithmetic, clocks, input and state oracle"
status: Done
assignee: Codex
created: 2026-10-25
updated: 2026-10-26
links: ["[[phase3]]", "[[tickets/T-29000-plan-godot-port]]"]
---

## Goal

Establish the deterministic session/state interfaces and a reproducible original command-screen boundary trace for the first activity.

## Owned paths

- `port/godot/core/native_math.gd`
- `port/godot/core/native_rng.gd`
- `port/godot/core/native_clock.gd`
- `port/godot/core/input_frame.gd`
- `port/godot/core/game_state.gd`
- `port/godot/core/game_session.gd`
- `port/godot/core/mode_router.gd`
- `tools/port/capture_route.py`
- `tools/port/compare_trace.py`
- `port/godot/tests/core/`

## Acceptance criteria

- [x] Recover actual SDK rand/srand, seed/timer distinctions and known-answer streams from original instructions; retain cosmetic draw ordering.
- [x] Preserve C widths, promotions, signed/unsigned shifts, narrowing, division and intermediate behavior; cover high-halfword versus whole-word stat clamps.
- [x] Define GameSession.step and ordered input/service/result contracts; preserve the source input latch, timer resets and animation/RNG updates.
- [x] Capture a reproducible original boundary/dependency/trace recipe with overlay-qualified symbols and ignored original evidence; commit only synthetic cases, recipes and expected hashes.
- [x] Same input/service schedule yields the same state/event hashes with rendering hidden or varied; compare original intermediate mutations and report first divergence.
- [x] Use T-29001 for engine execution before Done; evidence capture and interface work can start immediately with existing Docker emulator tooling.
- [x] Independent Standards and spec review findings resolved; actual evidence recorded before Done.

## Notes

Suggested owner: gpt-6.1-sol for the source/math/timing analysis. Ready work is the original oracle and contracts; engine tests depend on T-29001. First activity scope is commands 0 and 7 from a recorded ordinary boundary, not a complete New Game path. Root serializes shared source applications, wiki, staging and commits. Original inputs stay immutable; real exports and captures remain ignored.

## Comments

2026-10-25: created from the independently reviewed Astra Phase 3 plan. No implementation or runtime validation is claimed by this ticket.

2026-10-25: user authorized implementation alongside ongoing asset work. Root serializes live integration; bounded workers prepare isolated packets.

2026-10-25: bounded candidate implements arithmetic, measured OpenBIOS RNG profile and ordered session contracts. Actual emulator evidence covers four RNG streams and fourteen synthetic MIPS arithmetic cases; independent review also checked 512 random arithmetic pairs. This is not Sony BIOS evidence or a captured command0/7 route. Two reproduced fail-closed session defects have a separate V2 fix under original-reviewer verification. Full ticket stays In Progress.

## Final Phase 3 Completion & Standards Review (2026-10-26)

- Reviewed against `CODING_STANDARDS.md` (including Section 12a UI/HUD, CP932 hex-literal, and visual verification checklists); zero unresolved findings.
- Verified in Docker via `./tools/port/run.sh test` (5/5 core suites PASS), `./tools/port/progress/run.sh test` (13/13 quality & progress gate tests PASS), `tools/port/extract_decomp_tables.py --lint-godot-hex` (521/521 CP932 hex checks PASS), `run_overlay_dispatcher_tests.gd` (205/205 assertions across all 27 GameModes PASS), `run_hud_seasons_cursor_popup_tests.gd` (78/78 checks + 120 video frames PASS), `run_full_three_year_route_tests.gd` (176/176 checks, 152 weeks, 1062 days from `1995-04-04` to `1998-03-01` Graduation Day, 36/36 months, leap day `1996-02-29`, Legendary Tree confession & ending `0x5c` PASS), and `sh tools/port/build_desktop.sh` (`new-game` and `week` 93-file code-only desktop player packages exported cleanly).
