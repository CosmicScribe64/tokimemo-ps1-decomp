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
