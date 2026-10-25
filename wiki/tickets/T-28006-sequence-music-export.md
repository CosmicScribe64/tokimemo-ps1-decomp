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

2026-10-25 accepted v9 integration: all later native-cache, declared-EOT-bound, framing-label and output-proof findings are resolved in the immutable packet. Independent Standards/native reviews and the composed 498-test suite pass without skips. The installed CLIs validate/repeat across all 63 files / 177 entries / 1,008 aliases, decoding 337,797 events (235 tempos, two implicit, and 42 CC1) and producing 177 opt-in MIDI auditions with zero unsupported entries in this corpus. All source/native package bytes and the earlier 138 channel-event streams persist. Native MPQN units are proved through the full timing chain; BPM rounding/quantization prevents exact scheduler/playback parity claims. Safe entry boundaries, truthful output-path relocation accounting and exact after-image/source protection pass. Exact commands, manifests and rendering limits are in [[assets-media]]. Current-corpus event decoding is complete; bank/WAV/loop rendering and general format coverage keep T-28006 In Progress.

2026-10-25: applied reviewed bank provenance v3 after both earlier source-order and original-identity findings were resolved. Five new source/profile/test files preserve all 34,959 existing tracked files. All 63 SEQs join freshly to original ISO path/size/hash; 53 conditional load requests cover 45 paths and leave 18 without candidates. All 177 playback assignments remain null. Per-channel initial programs are VAB indexes; runtime cache/read/constructor/epoch/pack constraints and UNDOKAI2 co-load distinctions persist. Independent native/Standards reviews and 528 coherent installed tests pass without skips. WAV rendering and actual playback remain open. [[assets-media]].

2026-10-25 VAB metadata continuation: the isolated two-file instrument reader remains unmerged after independent review found that it drops and fails to validate VabHdr.fsize against paired VH+VB bytes. All 63 original pairs have matching declared totals; one positive synthetic fixture incorrectly accepts a mismatch. Independent original-bank checks validate 60 supported mappings; PRO_VOL, UNDOKAI and YUUEN remain explicit unsupported inputs, without a corruption claim. The media owner is preparing separate v2 with preserved declared size, extent checks, negative regressions and all 63 inventory statuses. Existing sequence/bank tools and rendering limits are unchanged. Review: ignored T-28006/vab-instruments-v1-review.md. Keep In Progress.

2026-10-25: applied the independently reviewed V2 bank-local instrument reader; declared full VH+VB size is retained and checked. Actual installed exports validate all 63 pairs, with 60 supported banks / 709 programs / 1,721 tones / 1,158 nonzero-ID sample slots / 8,038,016 bytes and three explicit unsupported shapes. The coherent suite passes 567 tests without skips. Rates, runtime bank selection, synthesis/loop playback and WAV rendering remain unproved. No original input changed; exports remain ignored. [[assets-media]].
