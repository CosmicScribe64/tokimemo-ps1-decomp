---
id: T-30000
title: Plan Phase 4 port modernization
status: Done
assignee: orchestrator
created: 2026-10-26
updated: 2026-10-26
links: ["[[phase4]]", "[[phase3]]", "[[widescreen-settings]]", "[[tickets/T-30001-modern-widescreen]]", "[[tickets/T-30002-modern-settings]]", "[[tickets/T-30004-modern-achievements]]", "[[tickets/T-30003-player-customization]]"]
---

## Goal

Mark Phase 3 complete in `ROADMAP.md` and `wiki/status.md`, clean up the Kanban board so only active Phase 4 tickets remain open, and publish a comprehensive, source-backed Phase 4 modernization plan in `wiki/phase4.md` covering widescreen presentation (`T-30001`), modern desktop settings and localization/voice pack selectors (`T-30002`), native achievements (`T-30004`), and the gender-aware player customization stretch goal (`T-30003`).

## Acceptance criteria

- [x] `ROADMAP.md` and `wiki/status.md` record Phase 3 (Godot port) complete with full verification metrics and mark Phase 4 (Modernize the port) in progress.
- [x] `wiki/kanban.md` is tidied with all historical Phase 1, Phase 2, Asset Export (`T-28000`–`T-28007`), and Phase 3 (`T-29000`–`T-29018`) tickets moved to `Done` and synchronized with ticket frontmatter (`0` status mismatches in `tools/wiki.py lint`).
- [x] `wiki/phase4.md` defines the Phase 4 architecture, non-overlapping implementation packets, milestone order (`P4.0`–`P4.4`), widescreen safe-frame aperture contracts, settings/audio/input/translation-pack schemas, deterministic achievement evaluation, and testing/review gates.
- [x] `T-30001`, `T-30002`, and `T-30004` are updated to `Ready` with concrete file ownership and testable acceptance criteria; `T-30003` remains in `Backlog` as the documented stretch milestone `P4.4`.
- [x] `tools/wiki.py catalog` and `tools/wiki.py lint` pass in Docker with `0` broken links and `0` status mismatches.

## Comments

2026-10-26: Created after Phase 3 completion (`1cf4d1185` on `origin/main`) and Kanban board cleanup (`142` historical tickets closed to `Done`). Grounded in the completed `[[widescreen-settings]]` research (`[[tickets/T-30005-widescreen-settings-feasibility]]`) and the shipped Phase 3 Godot 4.7.2 architecture (`[[phase3]]`).
