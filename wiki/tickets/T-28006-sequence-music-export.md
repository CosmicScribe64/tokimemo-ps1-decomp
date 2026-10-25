---
id: T-28006
title: Read sequence music and preserve rendering dependencies
status: In Progress
assignee: sequence-asset-agent
created: 2026-10-25
updated: 2026-10-25
links: ["[[asset-export]]", "[[assets-media]]", "[[tickets/T-28002-media-asset-export]]"]
---

## Goal

Handle the original SEQ music assets with source-backed event and instrument/bank metadata. Prepare reusable modern sequence exports and the evidence needed for later WAV rendering. This extends the user's existing music/general-asset request; it adds no engine project or integration.

## Acceptance criteria

- [ ] Inventory every original SEQ with source/hash/extent and coverage status; establish native header, event, tempo, loop and program semantics from primary/source evidence.
- [ ] Implement a bounded lossless reader and deterministic structured export, plus MIDI where the source semantics permit. Preserve raw bytes, stable identities, opaque/unsupported events and exact timing metadata.
- [ ] Prove sequence-to-bank/program dependencies where possible; unresolved runtime selection remains explicit. Samples or MIDI alone do not establish rendered game music.
- [ ] Implement and verify WAV music rendering only after instrument, pitch/envelope, ordering and loop dependencies are understood. Distinguish software audition from hardware fidelity; unresolved rendering requirements keep the ticket open.
- [ ] Run invented synthetic tests and local original-input validation in Docker; extracted events, MIDI, audio and game text remain ignored under .asset-cache/.
- [ ] Resolve independent Standards/correctness review before Done; preserve original inputs and matching-game/build behavior. No Godot, translation or replacement art.

## Notes

The existing media checkpoint lists 63 SEQ files; this count is a starting inventory, not proof that each is supported. The initial bounded assignment is reader/JSON/MIDI and dependency evidence; rendering follows only when justified. Root alone applies live files, updates shared wiki and commits.

## Comments

2026-10-25: split a disjoint sequence lane from T-28002 so music work can proceed while image/text proof repairs continue. No new host dependencies are authorized; reuse Docker images. The agent prepares isolated ignored packets and actual validation.
