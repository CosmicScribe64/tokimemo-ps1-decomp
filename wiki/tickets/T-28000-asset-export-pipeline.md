---
id: T-28000
title: Reusable asset readers and modern exports
status: In Progress
assignee: asset-coordinator
created: 2026-10-24
updated: 2026-10-25
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
