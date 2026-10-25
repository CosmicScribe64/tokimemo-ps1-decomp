---
id: T-29002
title: "Implement native arithmetic, clocks, input and state oracle"
status: Ready
assignee:
created: 2026-10-25
updated: 2026-10-25
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

- [ ] Recover actual SDK rand/srand, seed/timer distinctions and known-answer streams from original instructions; retain cosmetic draw ordering.
- [ ] Preserve C widths, promotions, signed/unsigned shifts, narrowing, division and intermediate behavior; cover high-halfword versus whole-word stat clamps.
- [ ] Define GameSession.step and ordered input/service/result contracts; preserve the source input latch, timer resets and animation/RNG updates.
- [ ] Capture a reproducible original boundary/dependency/trace recipe with overlay-qualified symbols and ignored original evidence; commit only synthetic cases, recipes and expected hashes.
- [ ] Same input/service schedule yields the same state/event hashes with rendering hidden or varied; compare original intermediate mutations and report first divergence.
- [ ] Use T-29001 for engine execution before Done; evidence capture and interface work can start immediately with existing Docker emulator tooling.
- [ ] Independent Standards and spec review findings resolved; actual evidence recorded before Done.

## Notes

Suggested owner: gpt-6.1-sol for the source/math/timing analysis. Ready work is the original oracle and contracts; engine tests depend on T-29001. First activity scope is commands 0 and 7 from a recorded ordinary boundary, not a complete New Game path. Root serializes shared source applications, wiki, staging and commits. Original inputs stay immutable; real exports and captures remain ignored.

## Comments

2026-10-25: created from the independently reviewed Astra Phase 3 plan. No implementation or runtime validation is claimed by this ticket.
