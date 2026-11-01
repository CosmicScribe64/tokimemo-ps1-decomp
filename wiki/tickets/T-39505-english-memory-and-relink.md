---
id: T-39505
title: Give the English build room to grow (memory map and free relink)
status: In Progress
assignee: claude
created: 2026-11-01
updated: 2026-11-01
links: ["[[tickets/T-39500-english-translation-patch]]", "[[english-text-runtime]]"]
---

## Goal

The English build from [[tickets/T-39502-english-text-runtime]] pins every object to its original address, so overlays cannot grow and the main exe has only the 27 KB island. English text is roughly twice the size of the Japanese. Map the RAM the game really uses, and let the English build relink the main exe and overlays freely where that is safe, so English strings and code fit.

## Acceptance criteria

- [ ] RAM map of the whole game (main exe, bss, overlay slot and its tail, the regions above it, file load buffers, stack) from the code's constants and from emulator memory watches, with each region's owner and whether English can use it.
- [ ] Every raw address in data or code that is not a linker symbol is found and either symbolised or proven not to need moving.
- [ ] The English build can relink freely (main exe and overlays); with no English changes it boots and plays the emulator route with screenshots identical to the original.
- [ ] Size limits checked at build time against the RAM map.

## Notes

Part of [[tickets/T-39500-english-translation-patch]].

## Comments
