---
id: T-0500
title: Split game rodata, data and bss per source file
status: In Progress
assignee: claude
created: 2026-10-09
updated: 2026-10-09
links: ["[[source-files]]", "[[tickets/T-0012-game-file-boundaries-and-shift-jis]]", "[[tickets/T-0301-sdk-rodata-data-split]]"]
---

## Goal

The game code is split into 29 files ([[source-files]]) but `.rodata`, `.data` and `.bss` are still one subsegment each. Split them to match the files, so that string literals and statics can move into the C files when functions are decompiled.

## Acceptance criteria

- [ ] rodata/data/bss subsegments named after the `src/main/<addr>` file that owns them, in link order, with the evidence per boundary written into [[source-files]].
- [ ] `ninja` ends with `build/SLPM_86.053.bin: OK` and all 27 sha1 checks pass.

## Notes

Why T-0012 did not split rodata even for the clear cases: (1) of the 8 rodata object starts that code references exactly, 6 coincide with a file boundary and 2 do not (see [[source-files]]), so a rodata split would give 6 cuts for 28 files and the pieces between would still be unattributed; (2) only about 80 game functions reference `.rodata` directly and most of its 0x3EE0 bytes (strings reached through `.data` pointer tables, tables in the 0x800B2000-0x800B2300 region) have no code reference to attribute them to a file; (3) the rodata/data/bss of the game and of the SDK libs interleave from 0x800B23B0 on, which needs [[tickets/T-0301-sdk-rodata-data-split]] first; (4) a wrong cut would still match the sha1, so there would be no build signal to catch a bad ownership claim. Naming a partial split would therefore assert unproven ownership.

Why T-0012 did not do it: only about 80 game functions reference `.rodata` directly (strings are mostly reached through `.data` pointer tables), and the per-file `.data`/`.bss` blocks interleave with shared globals defined in other files, so a cut needs symbol-level ownership (who defines a symbol), not just who uses it.

Starting evidence (`tools/game_boundaries.py`): rodata objects start 16-aligned after a zero run of at least 5 bytes: `800AF370`, `800AF390`, `800AFB00`, `800AFB70`, `800AFE00`, `800AFE10`, `800B1590`, `800B2000` map onto the file boundaries; `.data`/`.bss` symbols used by a single file are 94% in file order. The libpress/libcd/libsnd/libgpu strings from about `0x800B23B0` (see [[tickets/T-0301-sdk-rodata-data-split]]) must be separated first. Pointer tables in `.data` that point into rodata give string ownership. Lines in splat's own output (`.asciz` comment hex) make string extents exact.

## Comments

- 2026-10-09 T-1340: rodata chunk islands for jump-table functions exist (`.rodata` subsegment named like a C file; [[build-system]]). New boundary evidence: a jump table that ends in zero words up to a 16-byte boundary ends an original object (70 cases in the overlays); IDO's per-object order is [strings][tables][padding]. A per-object split of the C files would allow several islands per file.
