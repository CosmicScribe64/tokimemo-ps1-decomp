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

2026-10-25 actual app integration: root applied the frozen five-file `app-v1` packet after both review axes resolved the initial-checkpoint coverage finding. Canonical five rule suites and 26 focus checks pass. Both headless schedules 0/1/3 and the actual Xvfb/Mesa app commit 243 steps; all 15 original checkpoints compare the full progress record, parameters, girls, queue, calendar, flags, RNG, mode/stage/step/substep and three mode timers. The first-result subset retains its ten earlier comparisons. The April 9 holiday is rejected transactionally at April 8/index 8 with equal before/after hashes; it is not skipped. Root actual proof and screenshots are under `live-week-proof/` and `.asset-cache/port-week-current/`; the named canonical clip is published to the existing visual page. Native HUD, audio, other commands and holiday continuation remain open. Smoke timeout is 90 seconds for PNG recording, separate from the 601-iteration logical bound.

2026-10-25: the rest-command owner continues from the sealed first-result packet toward actual ordinary-rest-week continuation, with source/cache/date/queue/RNG semantics and original comparisons required. Root granted a bounded emulator lease after T-29013 released and fresh Docker inspection found no emulator. All successor changes remain isolated until current app/save integration is verified; no shared app changes or full-week claim yet.

## Rest weekdays integrated (2026-10-25)

Root applied the five-file reviewed packet `7a7b095dc7d28744443ea41c0d06a1e6a71cde1794df524077af52c37b801418`. Canonical rest now continues through April 8 in 335 steps. Installed checks pass: 109 ordinary-week, 111 first-result, 26 focus, 16 continuing-save, 37 codec, 50 existing-save and 28 rest-save checks, plus bank/New Game and shared suites. Fresh Mesa playback matches the headless full trace, final state, RNG and checkpoints exactly. Native comparison reproduces all 29 control segments and 150 actor rows; three original 30-poll SD waits remain one preloaded-cache poll, with clock differences explicit. Proof: `.claude/orchestrator/state/T-29009/rest-week-app-v1/canonical-proof/acceptance.json`.

Holiday scene integration and native HUD/audio remain open. Original first-result behavior is retained when requested explicitly. User-requested removal of default developer text is assigned to the HUD successor; this packet preserves the old development layout.
