---
id: T-39303
title: Install one disc pack of the original files the game reads (M2)
status: Ready
assignee:
created: 2026-10-30
updated: 2026-10-30
links: ["[[port-completion-plan]]", "[[disc-layout]]"]
---

## Goal

`setup_content` installs one pack of the original files the game reads (path, LBA, size, SHA-256) from the user's disc, and the engine's CD reads use it. This is layer 4 of [[port-completion-plan]].

## Acceptance criteria

- [ ] The pack is built from the user's disc only, verified by hashes, and never committed.
- [ ] `cd_read_sectors` and related engine calls read from it by LBA.
- [ ] Existing content roles keep working.

## Comments
