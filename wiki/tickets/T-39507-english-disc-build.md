---
id: T-39507
title: Build the English disc and patch in one command
status: In Progress
assignee: claude
created: 2026-11-01
updated: 2026-11-01
links: ["[[tickets/T-39500-english-translation-patch]]"]
---

## Goal

One command builds the English game and disc: the English exe and overlays, every text container written with the English encoder and build-time word wrap, CD read sizes and LBAs updated in the English build, files relocated on the disc, then the patched track, cue and xdelta patch.

## Acceptance criteria

- [ ] English encoder over the T-39503 writers, emitting the control bytes of [[english-text-runtime]], with word wrap from the font metrics and each window's pixel width.
- [ ] C literals and CD read sizes and LBAs change only in the English build; the default build still matches 27/27.
- [ ] The disc boots in the emulator and shows English on the title, file select and opening screens.
- [ ] A report lists every record that could not be placed or wrapped.

## Notes

Part of [[tickets/T-39500-english-translation-patch]].

## Comments
