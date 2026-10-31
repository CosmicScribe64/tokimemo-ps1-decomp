---
id: T-28000
title: Reusable asset readers and modern exports
status: Done
assignee: asset-coordinator
created: 2026-10-24
updated: 2026-10-26
links: ["[[asset-export]]", "[[disc-layout]]", "[[tickets/T-0002-disc-extraction-and-exe-identification]]"]
---

## Goal

Engine-independent local asset extraction/export tooling for the decomp. Accept original game input, retain byte provenance and support modern formats with reproducible manifests. No Godot project, engine integration or translation execution.

## Acceptance criteria

- [ ] Formats and control semantics backed by source/primary references; unknown cases explicit.
- [ ] Working bounded exporter/reader with deterministic outputs, source hashes/locations and coverage manifest.
- [ ] Synthetic tests and local real-input validation run in Docker; no copyrighted assets or game text committed.
- [ ] Independent standards/correctness review resolved before Done.
- [ ] Preserve current decomp/build behavior; engine-specific code and full translation remain out of scope.

## Comments

2026-10-24: User authorized agents for asset extraction/readers, especially translation-ready text and reusable modern exports. No direct Godot work. Root owns shared tracker/index/log and commits.

2026-10-25: Phase 2 is Done/pushed; root captured the current immutable source/assembly witness described in [[asset-export]]. Existing text (Sol 6.1) and media (Luna) chats resume isolated source-evidence rebasing. Profiles and comprehensive corpus coverage are still unverified for this new witness; all asset tickets remain open. Root retains live integration/review ownership.

2026-10-25: reviewed current-source text rebase applied and verified: 437 canonical asset tests, live source/serialization replay, and exact 38,696-row preservation. No C/header/config/build-tool bytes changed. The source witness remains immutable; 1,889 unresolved text extents and media/image corpus work remain open. [[assets-text]], [[asset-export]].

2026-10-25: reviewed media source rebase and two ETC SD exports applied after current-text integration. Coherent suite: 441 canonical asset tests, zero skips; original native/palette/PNG/review proofs pass. Static corpus is 359 image sources / 3,480 regions, with 73 unresolved NBN sources and further background/movie/image-text work. No game source/build changes or complete-coverage claim. [[assets-media]].

2026-10-25 publication: reviewed text/media/OPENXA checkpoint commits `f499eaac8`, `f2e0164ad` and `0d8f98a32` are pushed to `origin/main`; an independent remote query confirms `0d8f98a3244ba452473629263daa49de077344b8`. Before publication, root rehashed all 27 unchanged matching binaries and verified that committed changes affect only reviewed asset tools/tests/wiki. Coherent suite: 441 tests, zero skips. No rebuild was needed because game source/build files are unchanged. Unreviewed BS and RPG candidates were excluded; all asset tickets remain In Progress.

2026-10-25: integrated the reviewed five-view RPG initial-buffer reader. Current text catalog totals 38,701 units while preserving all old rows; five additions stay flagged, ready rows remain 31,346, and 1,889 unknown extents remain. Coherent asset suite: 453 tests, zero skips. Source/original-byte/delta/serialization proofs pass; game source/build files are unchanged. No runtime or complete-coverage claim. [[assets-text]].

2026-10-25: integrated the independently reviewed native shooter display-input proof. Twelve existing physical extents gain original-instruction-backed contexts, reducing unknown extents to 1,877 while preserving all 38,701 units, readiness, payloads and null targets. The coherent suite passes 478 tests without skips. All 34,945 tracked files outside the exact eight-file packet are unchanged by application. The separate four-BS exporter and music ticket were pushed through `d14f83d05c195d2ea5abe20d63fa1bf3c3a5a887`, independently confirmed on origin/main. All asset tickets remain open. [[assets-text]], [[assets-media]], [[asset-export]].

2026-10-25: four additional reviewed seasonal backgrounds bring the static catalog to 367 sources / 3,488 regions. Reviewed sequence tooling now reconstructs all 63 original packages and exports structured JSON, with 138 opt-in provisional MIDI entries and 39 explicit unsupported streams. The coherent asset suite passes 493 tests without skips; actual CLI/export/catalog/native proofs pass. Text remains at 38,701 units / 1,877 unknown extents. Bank/WAV dependencies, 73 NBN sources, 227 background geometries and other image/text/movie coverage stay open. [[assets-media]], [[assets-text]], [[asset-export]], `tickets/T-28006-sequence-music-export`.

2026-10-25: reviewed v9 closes current-corpus SEQ event decoding: all 177 entries / 337,797 events and 177 alternate MIDI auditions, with lossless package reconstruction, stable IDs/aliases and installed CLI/native/SMF proofs. The coherent suite passes 498 tests without skips. Native tempo units are source-proved; scheduler quantization, bank binding and WAV rendering remain separate open work. Text and static-image totals are unchanged, and the NBN candidate is held for actual safety/pixel/metadata repairs. Overall asset completion remains false. [[assets-media]], `tickets/T-28006-sequence-music-export`.

2026-10-25: integrated sixteen independently reviewed native label/ranking input contexts after the earlier twelve shooter contexts. All 38,701 unit IDs, payloads, control data, readiness and null targets persist; 1,861 physical extents remain semantically unknown. The coherent current suite passes 514 tests without skips. Actual original-byte/native/context/full-delta and JSONL/TSV replay remain the publication gates; no runtime/glyph or comprehensive-coverage claim. [[assets-text]], [[asset-export]].

2026-10-25: reviewed bank provenance v3 adds original-ISO-bound conditional sequence/bank requests with null playback assignments. All 63 sequence identities are checked, with 53 requests across 45 paths and 18 paths still without candidates. The coherent installed suite passes 528 tests without skips. No event/source/alias changes, native WAV rendering or playback parity claim. The media owner is investigating a disjoint source-backed VAB metadata slice; NBN v2 and conservative SPEAKTST integration remain separately in review/preparation. [[assets-media]], `tickets/T-28006-sequence-music-export`.

2026-10-25: reviewed SPEAKTST layout inventory adds 16,702 flagged candidates and preserves all 38,701 existing rows, yielding 55,403 export rows with ready rows unchanged. FF controls, unknown tokens and opaque metadata remain lossless; runtime/voice/character semantics remain open. Executable/overlay unknown extents stay 1,861. The coherent installed suite passes 550 tests without skips; full normal/optimized text, byte reconstruction and JSONL/TSV replay pass. All asset tickets remain In Progress. [[assets-text]], [[asset-export]].

2026-10-25: integrated four original prologue NBN sources with 24 canonical native-cell previews and separate diagnostic masks. Actual current export/native pixel/old-record/manual-review/catalog/gallery checks pass; static coverage is 371 sources / 3,512 regions with 69 generic NBN and 227 BS geometries still open. The coherent suite passes 560 tests without skips. Text remains 55,403 rows / 1,861 unknown usage classifications; no runtime/whole-corpus or soundtrack completion is claimed. [[assets-media]], [[asset-export]].

2026-10-25: applied the independently reviewed V2 bank-local instrument reader; declared full VH+VB size is retained and checked. Actual installed exports validate all 63 pairs, with 60 supported banks / 709 programs / 1,721 tones / 1,158 nonzero-ID sample slots / 8,038,016 bytes and three explicit unsupported shapes. The coherent suite passes 567 tests without skips. Rates, runtime bank selection, synthesis/loop playback and WAV rendering remain unproved. No original input changed; exports remain ignored. [[assets-media]].

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
