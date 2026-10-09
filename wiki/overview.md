---
type: overview
updated: 2026-10-09
---

# Overview

Goal: a byte-matching decompilation of the PS1 executable(s) of Tokimeki Memorial - Forever with You (Japan, PlayStation the Best). Rebuilding the original binary(ies) exactly from C and assembly source.

## Stack

- Target: Sony PlayStation, MIPS R3000 (little-endian, GTE coprocessor)
- SDK: PsyQ, 1995-era libs (see [[psyq-sdk]] and [[tickets/T-0006-identify-psyq-libs-sdk-version]])
- Tooling: splat (splitting), IDO 5.3 via asm-processor for the game code (partial match, see [[matching-notes]]), maspsx + gcc kept for SDK C, all run in Docker only (see [[tickets/T-0003-docker-toolchain-image]])
- Build: see [[tickets/T-0005-build-system-and-checksum-matching]]

## Facts

- Disc layout, extraction: [[disc-layout]]
- Main executable (SLPM_86.053, entry, memory map, bss): [[executable]]
- Overlays (26 headerless .EXN, load address unverified): [[overlays]]
- Compiler and tools: [[toolchain]] (IDO 5.3 + asm-processor for game code, splat 0.50.0)
- Libraries linked and SDK era (libpress, libcd, libsnd, libspu, libgpu, libapi; PsyQ 3.6-4.0 era): [[psyq-sdk]]
- Build and sha1 check (OK build, all `INCLUDE_ASM`): [[build-system]]

## Raw sources (immutable)

disc/ (extracted game files, not committed), the game zip, and anything under raw/ (e.g. `raw/disc-findings.md`). The wiki never modifies these.
