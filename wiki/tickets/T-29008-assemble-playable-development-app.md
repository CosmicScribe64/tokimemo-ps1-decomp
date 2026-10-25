---
id: T-29008
title: "Assemble the captured-boundary playable development app"
status: In Progress
assignee: port-app-agent
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]", "[[tickets/T-29004-school-day-playable-rules]]", "[[tickets/T-29005-native-scene-presentation]]", "[[tickets/T-29003-port-content-adapter]]"]
---

## Goal

Join the reviewed session, first-activity rules, presentation and external content adapters into a runnable development application. Start from the genuine captured command boundary; this is not a complete New Game route or full Phase 3 acceptance.

## Owned paths

- `port/godot/app/playable_boot.gd`
- `port/godot/app/playable.tscn`
- `port/godot/tests/app/`
- `tools/port/run_playable.sh`

## Acceptance criteria

- [ ] Use the existing rule/controller/renderer modules and actual external route packs; no copied rules, ignored production dependencies or source asset payload in res:// or Git.
- [ ] Load a validated external development boundary, tables and mandatory media; do not manufacture an ordinary state or silently substitute missing resources.
- [ ] Real keyboard/controller inputs use the native command mapping and the fixed-step input/service phases.
- [ ] Enable only commands whose mandatory assets are verified; missing command-7 background must stop before mutation until supplied.
- [ ] Resource and actor completion events advance through committed session steps; render frequency, Image objects and host I/O do not change the deterministic state/event hashes.
- [ ] Reach the first supported result and stop at the declared boundary before unsupported continuation. Capture actual screenshots/clips in a watched port folder.
- [ ] Record original-state/effect and rendered-comparison evidence, source/profile limits and both independent review axes before Done. Missing original HUD/audio/New Game stay explicit.

## Notes

App integration runs in parallel with bounded module reviews. Exclusive paths avoid the rules, presentation and content workers. Root serializes all canonical applications and commits. All development tools and tests run in Docker.
