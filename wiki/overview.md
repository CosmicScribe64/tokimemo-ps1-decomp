---
type: overview
updated: 2026-10-31
---

# Overview

A byte-matching decompilation of the PS1 executables of Tokimeki Memorial, Forever with You (Japan, PlayStation the Best). It is finished: all 27 binaries rebuild from C and assembly source, and every game function is in C ([[status]]).

## Stack

- Target: Sony PlayStation, MIPS R3000 (little-endian, GTE coprocessor)
- SDK: PsyQ, 1995-era libs (see [[psyq-sdk]] and [[tickets/T-0006-identify-psyq-libs-sdk-version]])
- Tooling: splat for splitting, IDO 5.3 through asm-processor for the game code with a few build passes that reproduce the original compiler ([[toolchain]], [[matching-notes]]), and maspsx with gcc for SDK C. All of it runs in Docker ([[tickets/T-0003-docker-toolchain-image]]).
- Build: see [[tickets/T-0005-build-system-and-checksum-matching]]

## Facts

- Disc layout, extraction: [[disc-layout]]
- Main executable (SLPM_86.053, entry, memory map, bss): [[executable]]
- Overlays (26 headerless .EXN): [[overlays]]
- Compiler and tools: [[toolchain]] (IDO 5.3 + asm-processor for game code, splat 0.50.0)
- Libraries linked and SDK era (libpress, libcd, libsnd, libspu, libgs, libgpu, libgte, libetc, libapi, libc; mixed PsyQ 3.3-3.6 vintages): [[psyq-sdk]]
- Build and SHA-1 check: [[build-system]]

## Raw sources (immutable)

disc/ (extracted game files, not committed), the game zip, and anything under raw/ (e.g. `raw/disc-findings.md`). The wiki never modifies these.
