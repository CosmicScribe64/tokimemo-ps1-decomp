---
type: overview
updated: 2026-10-09
---

# Overview

Goal: a byte-matching decompilation of the PS1 executable(s) of Tokimeki Memorial - Forever with You (Japan, PlayStation the Best). Rebuilding the original binary(ies) exactly from C and assembly source.

## Stack

- Target: Sony PlayStation, MIPS R3000 (little-endian, GTE coprocessor)
- SDK: PsyQ (version TODO, see [[tickets/T-0006-identify-psyq-libs-sdk-version]])
- Tooling: splat (splitting), maspsx (assembler macro post-processing), gcc (PsyQ-era version TODO), all run in Docker only (see [[tickets/T-0003-docker-toolchain-image]])
- Build: see [[tickets/T-0005-build-system-and-checksum-matching]]

## Sections to fill in (TODO, extraction agent)

- Disc layout (disc/, track/file list): TODO
- Main executable name, load address, size, entry point, checksum: TODO
- Overlays / other executables: TODO
- Compiler and flags findings: TODO
- Memory map: TODO
- Libraries linked (libgpu, libgte, libcd, libsnd, ...): TODO

## Raw sources (immutable)

disc/ (extracted game files, not committed), the game zip, and anything under raw/. The wiki never modifies these.
