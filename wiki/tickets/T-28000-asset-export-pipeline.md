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
