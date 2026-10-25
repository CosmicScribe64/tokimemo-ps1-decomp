---
id: T-28003
title: Read original game inputs and inventory assets
status: In Progress
assignee: asset-coordinator
created: 2026-10-24
updated: 2026-10-24
links: ["[[asset-export]]", "[[disc-layout]]", "[[tickets/T-0002-disc-extraction-and-exe-identification]]"]
---

## Goal

Reuse read-only disc/image/archive readers for original inputs and deterministic asset inventories, including explicit coverage of XA/STR/CDDA gaps. Output under ignored .asset-cache, never alter disc/game/raw inputs.

## Acceptance criteria

- [x] Formats and control semantics backed by source/primary references; unknown cases explicit.
- [x] Working bounded exporter/reader with deterministic outputs, source hashes/locations and coverage manifest.
- [x] Synthetic tests and local real-input validation run in Docker; no copyrighted assets or game text committed.
- [x] Independent standards/correctness review resolved before Done.
- [x] Preserve current decomp/build behavior; engine-specific code and full translation remain out of scope.

## Comments

2026-10-24: User authorized agents for asset extraction/readers, especially translation-ready text and reusable modern exports. No direct Godot work. Root owns shared tracker/index/log and commits.

2026-10-24: Dispatched parallel chat 01a16f34-9559-7a53-80cf-6152fc4b9ca6. Engine-independent tools only; default real outputs under ignored .asset-cache; root integrates source/tests/docs.

2026-10-24: Implemented `tools/assets/input.py`; [[assets-input]] records supported paths,
API, SHA-256/location metadata and limits. Real original ZIP and raw-track inventory both verified
1,372 records: 1,355 readable files, 16 Form2 XA/STR records, one beyond-track CDDA reference.
The CLI manifest at `.asset-cache/media/original-inventory-final/manifest.json` retains all records.
ZIP's other members are listed without claiming CDDA payload coverage. CUE files as direct inputs,
cooked ISO, CHD, 7z, multi-session and multi-extent joining remain unsupported; unsafe/ambiguous
CUE selection is rejected, interleaving/multi-extent records are explicit unsupported entries.

Docker input tests: 7 passed; all owned media/input/manifest tests: 31 passed. Independent standards
and spec review resolved malformed-sector handling, directory-only subheader fallback, framed
directory hashes, CUE ambiguity, ISO bounds/flags and resource closure. No changes to existing
input extractors, build, original data or raw sources. Root owns final tracker/index/log transition.

2026-10-24 source-only checkpoint: input/manifest/CLI sources and synthetic tests are included in
`.asset-cache/staging/media-reviewed-20261024-v1/`; the isolated complete suite passed 64 tests.
The snapshot contains no original game data. [[assets-media]] records its exact hash manifest.
Remaining unsupported input formats keep this ticket In Progress.

2026-10-24 cached-entry continuation: raw-disc and ZIP readers now initialize the bounded sector
reader lazily when `read()` receives an already validated inventory entry before `entries()`.
Source path, byte size and integer LBA are checked; existing sector/Form2 restrictions remain.
Synthetic raw and ZIP cached-entry cases pass in the complete 106-test Docker suite. Independent
review found no blocker. No additional input formats are claimed; status remains In Progress.
