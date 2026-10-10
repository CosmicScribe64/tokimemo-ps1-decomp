---
id: T-3051
title: Review the low-confidence object boundaries and the orphan rodata chunks
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[tickets/T-0500-per-file-game-rodata-data-bss-split]]", "[[source-files]]"]
---

## Goal

29 text boundaries of `config/objects/*.txt` are `choice/N` (the cut lies between two rodata users, several 16-aligned function starts fit, the call graph picked one) and three rodata chunks are orphans that stay asm (BUNKASAI `8015F730-8015FF70`, `8015FF70-80160380`: second and third chunk of text object `801511E0`; TACO `8015DA90-8015DAA0`: second chunk of `8013DB80`). Find better evidence: static helpers only called from one side, `.data` ownership (each object's `.data` also follows link order), O.BIN names, matched C that only builds on one side.

## Acceptance criteria

- [ ] Each `choice/N` boundary confirmed or moved, with the evidence written into [[source-files]].
- [ ] The three orphans owned (hidden text boundaries found) or explained.

## Notes

The build cannot check a `choice/N` cut: every candidate links the same bytes, and only functions without rodata sit between the users. A wrong cut only matters for static scope and for which file a function is written in.

## Comments
