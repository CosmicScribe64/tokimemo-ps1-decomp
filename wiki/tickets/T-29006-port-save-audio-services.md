---
id: T-29006
title: "Implement stable-boundary saves and ordered audio services"
status: In Progress
assignee: port-service-agent
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]", "[[tickets/T-29000-plan-godot-port]]"]
blocked-by: [T-29001, T-29002, T-29003]
---

## Goal

Provide versioned boundary saves and deterministic media command/completion services for the first activity without invented instrument or voice mappings.

## Owned paths

- `port/godot/services/save_service.gd`
- `port/godot/services/audio_service.gd`
- `port/godot/tests/services/`

## Acceptance criteria

- [ ] Save semantic state, behavior/content versions, checkpoint, native RNG/clocks and queued events at explicit stable boundaries; preserve fixed-point values.
- [ ] Validate before replacing live state; publish atomically with backup and explicit migrations; reject corruption, truncation and unsupported versions.
- [ ] Save/reload has the same continuation as uninterrupted replay for supported boundaries; native-card import and suspend-anywhere stay deferred.
- [ ] Order start/end/cancel/error events through the session service phase; mute changes audibility without advancing or stalling story rules incorrectly.
- [ ] Use only validated clip/tone/sequence dependencies; no guessed rates, General MIDI substitution or same-stem bank inference. Declare missing sound/fidelity and block required unproved timing.
- [ ] Independent Standards and spec review findings resolved; actual evidence recorded before Done.

## Notes

Suggested bounded owner: gpt-6-luna for save/schema/service tests, escalating native audio scheduling to Sol. A fixed BGM recording is not proof of all runtime audio commands. Full soundtrack acceptance remains later Phase 3 work. Root serializes shared source applications, wiki, staging and commits. Original inputs stay immutable; real exports and captures remain ignored.

## Comments

2026-10-25: reviewed v2 save codec and playable-session adapter are integrated in the canonical project. Both independent reviews pass; actual canonical replay in `.claude/orchestrator/state/T-29006/live-save-proof/` passes 37 codec/filesystem checks and 50 app checks, including all 80+40+243+163 continuation state/event hash pairs after command/result reloads. The adapter preserves controller policy, native checkpoints, RNG, clocks, input and owned service state; rejects unsafe boundaries and validates before installation with rollback on failed resource rebinding. This proves supported-boundary continuation, not suspend-anywhere, power-loss durability or native memory-card import. User-facing save controls and ordered audio remain open; the ticket stays In Progress.

2026-10-25: created from the independently reviewed Astra Phase 3 plan. No implementation or runtime validation is claimed by this ticket.

2026-10-25: user explicitly requested additional parallel port work. Start bounded save container/validation and audio lifecycle code against frozen session contracts while gameplay/presentation/content workers continue. Full acceptance retains the applicable runtime/content/continuation gates; no native-card import or guessed BGM mapping. Existing external worker prepares an ignored packet; root alone integrates.

## Reviewed rest-command/save integration (2026-10-25)

Root applied the exact five-file command7 app packet and two-file save-v4 correction after independent Standards and original Spec re-verification. Actual canonical execution passes 111 route checks, 26 focus checks, 15 native week checkpoints, 28 rest save checks, 50 existing study save checks, 37 codec checks and five shared suites. Fresh Mesa rest playback reaches iteration 163; its full trace, final state, RNG and stop boundary equal the reviewed original-backed route. Both save findings are resolved: rest draw identities restore correctly and authenticated command capabilities survive restore. Evidence: `.claude/orchestrator/state/T-29008/command7-save-live/acceptance.json`; final review `.claude/orchestrator/state/T-29006/v4/final-spec-resolution.json`.

This remains a captured-boundary development app: study supports the bounded ordinary week, rest stops at its first result. New Game linkage, full rest-week progression, native HUD/audio and held save UI shortcuts remain separate open work. No game C/header/build changes or translation changes.

## Queued F5/F9 save controls integrated (2026-10-25)

Integrated `port/godot/app/boundary_save_controls.gd`, `port/godot/tests/services/run_save_controls.gd`, `port/godot/tests/services/run_keyboard_controls.gd`, `tools/port/tests/save_controls_keyboard.py`, `tools/port/tests/test_save_user_data_paths.py` and `tools/port/run_playable.sh` from `.claude/orchestrator/state/T-29010/composed-app-v2/`. Input handlers enqueue F5/F9 requests without storage I/O inside simulation steps; saves and loads execute between committed steps and rebind live HUD presentation via `_present_committed`. Canonical Docker verification passes 30 write checks, 38 read checks across separate engine containers, 7 X11 keyboard delivery/recovery checks and 7 launcher user-data containment cases. Holiday save/restore controller reattachment and ordered audio remain open.

## Holiday boundary save/restore reattachment integrated (2026-10-25)

Extended `port/godot/services/playable_save_adapter.gd`, `port/godot/app/boundary_save_controls.gd`, and `port/godot/app/playable_boot.gd` from `.claude/orchestrator/state/T-29013/app-integration-v2/` so boundary saves and F5/F9 reloads support `normal_holiday` (`mode 0x46`, selection stage 1 and result stage 6), reattaching `holiday_controller` and refreshing `HolidayHudAdapter` (`_present_committed`) without mutating RNG or continuation hashes (verified across 1,135 checks in `run_sunday_app_integration_tests.gd`). Ordered audio service integration remains open.
