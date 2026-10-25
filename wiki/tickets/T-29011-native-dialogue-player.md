---
id: T-29011
title: "Implement source-backed dialogue control playback"
status: In Progress
assignee: parallel-port-worker
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]", "[[tickets/T-29008-assemble-playable-development-app]]"]
---

## Goal

Consume structured original dialogue through the native talk state/control sequence, using stable IDs, exact control/substitution data and explicit voice/completion gates without translating text.

## Owned paths

port/godot/dialogue/; port/godot/tests/dialogue/; tools/port/dialogue_content.py

## Acceptance criteria

- [ ] Implement a bounded runnable module from current applied source, with precise supported routes and fail-closed unsupported behavior.
- [ ] Use the existing native session, arithmetic, input and external-content boundaries; coordinate shared contracts instead of changing another worker's files.
- [ ] Original assets, dialogue, saves, numeric table payloads, screenshots and traces remain ignored; commit only code, synthetic cases and source/proof descriptions.
- [ ] Run targeted synthetic and original-emulator checks in Docker, record matching checkpoints and genuine limits, and produce actual visual proof where applicable.
- [ ] Resolve independent Standards/spec findings before Done; partial code or evidence does not complete the whole ticket.

## Notes

User explicitly requested more agents and chats to speed up the port. This is a disjoint parallel lane. Workers prepare immutable ignored packets under .claude/orchestrator/state/T-29011/candidate; root alone writes canonical source/wiki/Git/build. No actual translation, replacement art, copied fan translation, invented palette or silent missing-event fallback. Engine/runtime/data contracts are in [[phase3]] and port/godot/PORTING.md. Full Phase 3 remains open.
