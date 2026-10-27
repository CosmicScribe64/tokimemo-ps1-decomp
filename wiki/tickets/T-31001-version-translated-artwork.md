---
id: T-31001
title: Version and publish translated artwork and review history
status: In Review
assignee: root
created: 2026-10-27
updated: 2026-10-27
links: ["[[tickets/T-31000-create-translated-image-assets]]"]
---

## Goal

Publish the user-requested translated PNG assets and their review history in Git. This explicitly supersedes the earlier local-only treatment for these translated outputs. Original extracted images and comparisons containing originals stay ignored.

## Acceptance criteria

- [x] Snapshot published translated revisions with source/output identities and current-revision selection.
- [x] Preserve review decisions without promoting drafts to accepted runtime assets.
- [x] Verify copied bytes/dimensions and exclude original extracts/previews.
- [ ] Commit only this scope, push and verify the remote tip.
- [ ] Independent review before Done; publication itself is not visual/runtime acceptance.

## Snapshot

`translations/en/graphics/artwork/` contains59 current assets,97 PNGs across9 published batch-manifest revisions, and71 unchanged user review events. Every PNG's SHA256 and dimensions were checked against its published manifest and original dimensions; every recorded review output is present. Original extracts, comparisons and raw palette/table dumps are excluded. Live gallery files and ongoing port changes remain untouched. This is a versioned backup, not a new technical or visual acceptance claim.
