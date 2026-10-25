---
id: T-29017
title: "Support known PS1 releases in the content importer"
status: Backlog
assignee:
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]", "[[versions]]", "[[tickets/T-29003-port-content-adapter]]", "[[tickets/T-29015-desktop-development-player]]"]
---

## Goal

Support known PS1 releases in the content importer. User requirement recorded 2026-10-25; implementation and platform verification remain open.

## Acceptance criteria

- [ ] Identify and independently validate Rev 1, Shokai Genteiban, Rev 2, Rev 4 and PlayStation the Best using the existing version catalog and original files.
- [ ] Separate container recognition from release support; use per-release paths, hashes, executable/overlay table locations and source maps rather than relaxing the Best-release hash checks.
- [ ] Audit differing script/media/table files and expose equivalent semantic IDs in the installed pack. Keep version-specific content and evidence; do not silently replace files with another release.
- [ ] Define which behavior differences are preserved by release profile versus canonical Best behavior; record and test any deliberate normalization before promising equivalent playthroughs.
- [ ] Run complete import and required-route validation for each supported disc, then representative New Game, dialogue/audio, events, endings and save/reload checks.
- [ ] Keep unsupported or unknown PS1 revisions explicit until verified. Other platform editions are outside this request.

## Evidence and scope

The existing input reader supports extracted directories, raw MODE2/2352 data tracks and ZIP-contained tracks. Some formats require normalization and some sources lack audio/sector dependencies; a recognized container is not proof of complete game coverage. `wiki/versions.md` catalogs five PS1 discs: Rev 1 and Shokai have identical game files, later releases differ in executable/overlay and some data bytes, with moved LBAs. No new release support is claimed by this ticket.
