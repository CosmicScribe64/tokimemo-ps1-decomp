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
- [ ] Preserve all unique worktree changes; historical research bookkeeping exception remains documented below.
- [x] Verify recovered space and both local pages.
- [ ] Independently review cleanup evidence before closure.

## Progress

Initial free space1.7GiB. Removing unused Docker build cache older than24hours reclaimed7.178GB; free space rose to8.4GiB. Images and running containers were retained. Sibling worktrees consume approximately12GB and include dirty historical source files; translation checkout is excluded from cleanup. Main asset cache20GB and evidence13GB are preserved pending separate provenance-aware decisions.

Cleanup removed31 archived obsolete worktrees and the final clean recovered T25019 checkout, retaining branch refs; archives preserve unique worktree files and omit only original files verified identical to the main originals. User explicitly approved deleting superseded outputs after automatic review raised the risk:82 old text/music export caches and header rehearsal directories were removed (7,695,366,841 logical bytes). Six historical compiler-experiment images were removed. Nine stale test/emulator containers were stopped; both pages and the progress watcher remain running. Free space rose to approximately21GiB before removing the final checkout. Exact inventories, archive hashes and commands are local under T-29033. Independent cleanup review is deferred because the user requested all agents stop; this ticket remains In Review rather than Done.

Final worktree cleanup removed the remaining 11 obsolete checkouts. Only main and the completed translation worktree remain registered. Both detached tips have explicit local archive refs; branch-backed tips remain reachable. Current port candidates, translation/image-review data and original inputs remain in place.

Preservation exception: the intended fresh backup loop did not execute before this 11-tree removal. Ten trees had no tracked modifications. The old external-research tree's three research files already exist in committed `drafts/preserved-worktrees-2026-10-15/external-research/`; its exact uncommitted wiki index/board/log edits were not captured. Equality between the removed research files and the retained historical snapshot was not verified before deletion. This limitation is recorded in `T-29033/remaining-worktree-notes/README.md`; this batch must not be described as freshly archived in full. Main and the translation checkout were not affected.

Old matching scratch and T245 temporary checkouts are archived with a verified byte comparison of184,789 files and12 links (2,626,666,626 file bytes), then removed. The compressed recovery archive occupies approximately992MiB; its checksum, member inventory and comparison result are under T-29033. Current port temporary evidence and both web containers plus watcher are retained. Both pages returned HTTP200. No development or agents resumed.
