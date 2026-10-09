---
id: T-0008
title: Determine overlay load address and split overlays
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[overview]]"]
---

## Goal

Umbrella ticket: find where the 26 `CDROM/EXEDIR/*.EXN` overlays are loaded and split them with splat. See [[overlays]].

## Acceptance criteria

- [ ] Load address and slot size confirmed from the main exe loader (string `EXEDIR`).
- [ ] Per-overlay splat configs and build rules; each overlay rebuilds byte-identical.
- [ ] Symbols the main exe leaves undefined (`build/undefined_syms_auto.txt`) resolved through overlay symbols.

## Notes

Guess: load near 0x80130000, 0x30000 wide. Split into child tickets per overlay once the address is known. `O.BIN` is not code.

## Comments
