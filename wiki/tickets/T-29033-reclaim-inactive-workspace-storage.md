---
id: T-29033
title: Reclaim inactive workspace storage
status: In Review
assignee: root
created: 2026-10-26
updated: 2026-10-26
links: ["[[status]]"]
---

## Goal

Relieve disk exhaustion without losing active port work, translations, originals or unique historical worktree changes.

## Acceptance criteria

- [x] Identify large storage users and safe cleanup targets.
- [x] Preserve branch refs and unique dirty/untracked files before removing old checkouts.
- [x] Verify recovered space and both local pages.
- [ ] Independently review cleanup evidence before closure.

## Progress

Initial free space1.7GiB. Removing unused Docker build cache older than24hours reclaimed7.178GB; free space rose to8.4GiB. Images and running containers were retained. Sibling worktrees consume approximately12GB and include dirty historical source files; translation checkout is excluded from cleanup. Main asset cache20GB and evidence13GB are preserved pending separate provenance-aware decisions.

Cleanup removed31 archived obsolete worktrees and the final clean recovered T25019 checkout, retaining branch refs; archives preserve unique worktree files and omit only original files verified identical to the main originals. User explicitly approved deleting superseded outputs after automatic review raised the risk:82 old text/music export caches and header rehearsal directories were removed (7,695,366,841 logical bytes). Six historical compiler-experiment images were removed. Nine stale test/emulator containers were stopped; both pages and the progress watcher remain running. Free space rose to approximately21GiB before removing the final checkout. Exact inventories, archive hashes and commands are local under T-29033. Independent cleanup review is deferred because the user requested all agents stop; this ticket remains In Review rather than Done.
