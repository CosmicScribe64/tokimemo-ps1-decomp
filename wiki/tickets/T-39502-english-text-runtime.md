---
id: T-39502
title: Add the English text runtime behind a build flag
status: In Progress
assignee: claude
created: 2026-11-01
updated: 2026-11-01
links: ["[[tickets/T-39500-english-translation-patch]]"]
---

## Goal

An English build of the decomp, selected by a flag, with a proportional Latin font, a text encoding for English and pixel-width layout, while the default build still matches 27/27.

## Acceptance criteria

- [ ] `configure.py` English variant builds the executable and overlays into a separate output folder without the SHA-1 gate; the default build still prints OK for all 27.
- [ ] English encoding and proportional font designed, documented and implemented in `text_system.c`; size and memory limits checked at build time.
- [ ] One English string shown correctly on the title menu in the emulator.

## Notes

Part of [[tickets/T-39500-english-translation-patch]].

## Comments
