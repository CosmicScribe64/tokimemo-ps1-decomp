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

2026-10-25 bounded reader integration: accepted v5 is applied after independent Standards and Sol 6.1 correctness reviews resolved all parser/validator findings. Four new source/test files preserve all 34,953 existing tracked files. The coherent suite passes 493 tests without skips; ten music test methods and broader independent malformed/collision/forgery cases pass. Installed default and opt-in MIDI CLIs validate and repeat identically on all 63 originals (1,277,012 bytes), retaining 177 entries and 1,008 aliases with exact structured reconstruction. There are 138 complete event streams / 260,108 events and 138 provisional MIDI previews; 39 streams stay raw-only. Header tempo interpretation/native timing, remaining events, banks and WAV rendering remain open in this accepted version. All source/raw/event/ID data match reviewed exports; moving output directories changes only embedded output paths and their dependent JSON checksums/sizes. Exact hashes, commands and caveats are in [[assets-media]]. Keep In Progress.

2026-10-25 extension review: isolated v6 adds native FF51/CC1 evidence and 176 MIDI previews but is not applied. Independent native checks support header/meta MPQN units from the complete PPQN/scheduler/tick-rate chain, with header BPM rounding and meta-event truncation preventing exact native-timing claims. One P2 defect remains: FF51 must retain the native cached FF status for implicit following meta events; clearing it incorrectly marks one YUUEN entry unsupported. Native replay of that entry completes with 1,760 events and 22 tempo events, two implicit. A new isolated v7 must repair cache semantics and refresh full source/SMF/delta proofs; v5 remains the published reader until independent re-verification. Exact review is ignored `T-28006/music-v6-correctness-review.md` and its proof JSON. Bank/WAV work remains open.
