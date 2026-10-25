---
id: T-29004
title: "Port command selection and one genuine school-day action"
status: In Progress
assignee: Codex
created: 2026-10-25
updated: 2026-10-25
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

- [ ] Run source command eligibility and confirmation for supported commands 0 and 7 from the same captured valid boundary and seed.
- [ ] Execute the real scheduler queue construction, encounter/girl effects and calendar order; do not substitute a fabricated single-event queue.
- [ ] Preserve animation-completion gates, poor-result roll, fixed-point parameter effects, result hold, timeout and queue-index/return transitions.
- [ ] Match original intermediate mode/stage/step, dates, queue, affected stats/girls and RNG traces for both routes.
- [ ] A reachable unsupported event or required missing asset stops before progression mutation; acceptance requires closing every executed branch of the chosen bounded route.
- [ ] Independent Standards and spec review findings resolved; actual evidence recorded before Done.

## Notes

Suggested owner: gpt-6.1-sol. Full ordinary-week completion follows this slice. Current scoped source references and runtime acceptance are in wiki/phase3.md. Root serializes shared source applications, wiki, staging and commits. Original inputs stay immutable; real exports and captures remain ignored.

## Comments

2026-10-25: created from the independently reviewed Astra Phase 3 plan. No implementation or runtime validation is claimed by this ticket.

2026-10-25: start the first playable implementation in parallel, as the user requested faster progress. Reviewed native core V2 and real April 4, 1995 command boundary now provide fixed starting contracts; content work supplies only route-required assets. Parent runtime/content tickets retain broader open criteria, so full acceptance still waits on their applicable route gates. Workers prepare ignored disjoint packets; root alone integrates source.
