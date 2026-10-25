---
id: T-29009
title: "Port ordinary-week day-end and weekend continuation"
status: In Progress
assignee: parallel-port-worker
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]", "[[tickets/T-29008-assemble-playable-development-app]]"]
---

## Goal

Extend the reviewed first-activity boundary through source-backed ordinary weekday/day-end and weekend behavior, with real emulator comparison and explicit unsupported stops.

## Owned paths

port/godot/rules/day_end.gd; port/godot/rules/week_end.gd; port/godot/rules/ordinary_week.gd; port/godot/tests/week/

## Acceptance criteria

- [ ] Implement a bounded runnable module from current applied source, with precise supported routes and fail-closed unsupported behavior.
- [ ] Use the existing native session, arithmetic, input and external-content boundaries; coordinate shared contracts instead of changing another worker's files.
- [ ] Original assets, dialogue, saves, numeric table payloads, screenshots and traces remain ignored; commit only code, synthetic cases and source/proof descriptions.
- [ ] Run targeted synthetic and original-emulator checks in Docker, record matching checkpoints and genuine limits, and produce actual visual proof where applicable.
- [ ] Resolve independent Standards/spec findings before Done; partial code or evidence does not complete the whole ticket.

## Integration checkpoint

2026-10-25: root applied final reviewed packet `4e3cb548`, preserving the exact composed controller and unchanged D state. Existing mode30/31 and rule registration were retained once; only mode32 and the week suite were added. Five actual canonical Docker suites pass. Fresh canonical replay passes all 28 original checkpoints: 20 dispatcher/cache/week-end frame/timer checks, six isolated remaining-result checks and two holiday rollback checks. All 564 existing game C/header files remain unchanged. Evidence is ignored T-29009/live-application.json and live-native-proof/. Actual app cache/HUD service joins, holiday continuation and complete timing/rendering parity remain open; keep In Progress.

## Notes

User explicitly requested more agents and chats to speed up the port. This is a disjoint parallel lane. Workers prepare immutable ignored packets under .claude/orchestrator/state/T-29009/candidate; root alone writes canonical source/wiki/Git/build. No actual translation, replacement art, copied fan translation, invented palette or silent missing-event fallback. Engine/runtime/data contracts are in [[phase3]] and port/godot/PORTING.md. Full Phase 3 remains open.
