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

2026-10-25 canonical integration: applied the independently reviewed six-file focus-v2 packet. Actual installed tests pass 26 focus checks, ten original-game command-0 state/effect comparisons and identical state/event hashes with 0/1/3 rendered frames per step. The rendered app commits 80 steps and reaches the first result using eight RNG draws. Evidence is `.claude/orchestrator/state/T-29008/live-*`; captures are `.asset-cache/port-app-current-v2/` and are already present in the local visual viewer. This remains a captured-boundary development app: native HUD, command-7 content integration, week presentation, audio and New Game are not complete. Ordinary-week presentation integration is now assigned to the existing T-29009 owner.

App integration runs in parallel with bounded module reviews. Exclusive paths avoid the rules, presentation and content workers. Root serializes all canonical applications and commits. All development tools and tests run in Docker.

2026-10-25 next integration lane: the dialogue owner's frozen v2 is under review; that worker now prepares a separate command-7 application candidate against the committed week app. It must preflight the complete v2 media pack, keep command-specific frame/resource dictionaries distinct, validate native resource requests and prove the actual first rest result against the original 0/7 fixture. Command 0's full-week proof must stay unchanged. The production media importer remains held for its clean-CLI import repair, so no app enablement is accepted yet.
