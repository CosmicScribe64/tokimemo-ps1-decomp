---
id: T-29006
title: "Implement stable-boundary saves and ordered audio services"
status: Backlog
assignee:
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

2026-10-25: created from the independently reviewed Astra Phase 3 plan. No implementation or runtime validation is claimed by this ticket.
