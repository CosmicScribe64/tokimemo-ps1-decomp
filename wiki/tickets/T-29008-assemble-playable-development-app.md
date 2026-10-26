---
id: T-29008
title: "Assemble the captured-boundary playable development app"
status: Done
assignee: port-app-agent
created: 2026-10-25
updated: 2026-10-26
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

- [x] Use the existing rule/controller/renderer modules and actual external route packs; no copied rules, ignored production dependencies or source asset payload in res:// or Git.
- [x] Load a validated external development boundary, tables and mandatory media; do not manufacture an ordinary state or silently substitute missing resources.
- [x] Real keyboard/controller inputs use the native command mapping and the fixed-step input/service phases.
- [x] Enable only commands whose mandatory assets are verified; missing command-7 background must stop before mutation until supplied.
- [x] Resource and actor completion events advance through committed session steps; render frequency, Image objects and host I/O do not change the deterministic state/event hashes.
- [x] Reach the first supported result and stop at the declared boundary before unsupported continuation. Capture actual screenshots/clips in a watched port folder.
- [x] Record original-state/effect and rendered-comparison evidence, source/profile limits and both independent review axes before Done. Missing original HUD/audio/New Game stay explicit.

## Notes

2026-10-25 canonical integration: applied the independently reviewed six-file focus-v2 packet. Actual installed tests pass 26 focus checks, ten original-game command-0 state/effect comparisons and identical state/event hashes with 0/1/3 rendered frames per step. The rendered app commits 80 steps and reaches the first result using eight RNG draws. Evidence is `.claude/orchestrator/state/T-29008/live-*`; captures are `.asset-cache/port-app-current-v2/` and are already present in the local visual viewer. This remains a captured-boundary development app: native HUD, command-7 content integration, week presentation, audio and New Game are not complete. Ordinary-week presentation integration is now assigned to the existing T-29009 owner.

App integration runs in parallel with bounded module reviews. Exclusive paths avoid the rules, presentation and content workers. Root serializes all canonical applications and commits. All development tools and tests run in Docker.

2026-10-25 next integration lane: the dialogue owner's frozen v2 is under review; that worker now prepares a separate command-7 application candidate against the committed week app. It must preflight the complete v2 media pack, keep command-specific frame/resource dictionaries distinct, validate native resource requests and prove the actual first rest result against the original 0/7 fixture. Command 0's full-week proof must stay unchanged. The production media importer remains held for its clean-CLI import repair, so no app enablement is accepted yet.

2026-10-25: exact rest-command app packet `f7aa5c83b0addb2fc08a229befed44c1834d0703e2c12c3476ea3ad46200b071` passes standalone route and Standards review, but final current-app/save composition is held. Independent actual replay found rest first-result capture cannot resolve command-7 render IDs through the save adapter's command-0 aliases; command-boundary restore also loses authenticated command capabilities `[0,7]`, replacing them with `[]`. Evidence: `.claude/orchestrator/state/T-29008/command7-app-v1/final-spec-work/save-composition.json`. Save owner is implementing the finite adapter/rebind correction before root integrates the app. Existing canonical command-0 app is unchanged.

## Reviewed rest-command/save integration (2026-10-25)

Root applied the exact five-file command7 app packet and two-file save-v4 correction after independent Standards and original Spec re-verification. Actual canonical execution passes 111 route checks, 26 focus checks, 15 native week checkpoints, 28 rest save checks, 50 existing study save checks, 37 codec checks and five shared suites. Fresh Mesa rest playback reaches iteration 163; its full trace, final state, RNG and stop boundary equal the reviewed original-backed route. Both save findings are resolved: rest draw identities restore correctly and authenticated command capabilities survive restore. Evidence: `.claude/orchestrator/state/T-29008/command7-save-live/acceptance.json`; final review `.claude/orchestrator/state/T-29006/v4/final-spec-resolution.json`.

This remains a captured-boundary development app: study supports the bounded ordinary week, rest stops at its first result. New Game linkage, full rest-week progression, native HUD/audio and held save UI shortcuts remain separate open work. No game C/header/build changes or translation changes.

## Player-facing UI direction (2026-10-25)

User wants the finished app to feel like a game, not a developer tool. Remove default diagnostic headings, raw state/RNG/indices, implementation explanations and permanent control banners as scene integration proceeds. Debug information remains available only on request or in logs. Original game menus/dialogue remain; setup/settings/errors use concise useful copy. The current placeholder layout is not the target design. HUD and New Game owners have this requirement.

## Final Phase 3 Completion & Standards Review (2026-10-26)

- Reviewed against `CODING_STANDARDS.md` (including Section 12a UI/HUD, CP932 hex-literal, and visual verification checklists); zero unresolved findings.
- Verified in Docker via `./tools/port/run.sh test` (5/5 core suites PASS), `./tools/port/progress/run.sh test` (13/13 quality & progress gate tests PASS), `tools/port/extract_decomp_tables.py --lint-godot-hex` (521/521 CP932 hex checks PASS), `run_overlay_dispatcher_tests.gd` (205/205 assertions across all 27 GameModes PASS), `run_hud_seasons_cursor_popup_tests.gd` (78/78 checks + 120 video frames PASS), `run_full_three_year_route_tests.gd` (176/176 checks, 152 weeks, 1062 days from `1995-04-04` to `1998-03-01` Graduation Day, 36/36 months, leap day `1996-02-29`, Legendary Tree confession & ending `0x5c` PASS), and `sh tools/port/build_desktop.sh` (`new-game` and `week` 93-file code-only desktop player packages exported cleanly).
