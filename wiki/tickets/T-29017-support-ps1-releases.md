---
id: T-29017
title: "Support known PS1 releases in the content importer"
status: In Progress
assignee:
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]", "[[versions]]", "[[tickets/T-29003-port-content-adapter]]", "[[tickets/T-29015-desktop-development-player]]"]
---

## Goal

Support known PS1 releases in the content importer. User requirement recorded 2026-10-25; implementation and platform verification remain open.

## Acceptance criteria

- [x] Identify and independently validate Rev 1, Shokai Genteiban, Rev 2, Rev 4 and PlayStation the Best using the existing version catalog and original files.
- [x] Separate container recognition from release support; use per-release paths, hashes, executable/overlay table locations and source maps rather than relaxing the Best-release hash checks.
- [x] Audit differing script/media/table files and expose equivalent semantic IDs in the installed pack. Keep version-specific content and evidence; do not silently replace files with another release.
- [x] Define which behavior differences are preserved by release profile versus canonical Best behavior; record and test any deliberate normalization before promising equivalent playthroughs.
- [ ] Run complete import and required-route validation for each supported disc, then representative New Game, dialogue/audio, events, endings and save/reload checks.
- [x] Keep unsupported or unknown PS1 revisions explicit until verified. Other platform editions are outside this request.

## Progress Notes (2026-10-25)

- Implemented `tools/port/release_catalog.py` and `tools/port/test_release_catalog.py` (`19/19` tests passing) cataloging all 5 known Japanese PS1 releases (`best_slpm_86053`, `rev4_slps_00065_v143`, `rev2_slps_00065_v125`, `rev1_slps_00065_v110`, `shokai_slps_00064_v110`) with exact executable and 27-overlay SHA-1 tables, ISO-9660 PVD Volume Set ID disambiguation (`SLPS00065VER110` vs `SLPS00064VER110`), `SYSTEM.CNF` boot target verification, and container inspection across extracted directories, `.cue`, `.bin`/`.img`, `.iso`, and `.zip`.
- Integrated multi-release profile identification into `port/godot/services/original_game_import_backend.gd` with `port/godot/tests/app/run_multi_release_import_tests.gd` (`56/56` assertions passing in Docker), keeping PlayStation the Best (`SLPM-86053`) as the sole canonical extraction target while emitting explicit release-specific diagnostics (`ERR_REV4_UNSUPPORTED`, `ERR_REV2_UNSUPPORTED`, `ERR_REV1_UNSUPPORTED`, `ERR_SHOKAI_UNSUPPORTED`) for non-Best discs without relaxing canonical SHA-1 checks.

## Evidence and scope

The existing input reader supports extracted directories, raw MODE2/2352 data tracks and ZIP-contained tracks. Some formats require normalization and some sources lack audio/sector dependencies; a recognized container is not proof of complete game coverage. `wiki/versions.md` catalogs five PS1 discs: Rev 1 and Shokai have identical game files, later releases differ in executable/overlay and some data bytes, with moved LBAs. No new release support is claimed by this ticket.
