---
id: T-0005
title: Build system & checksum matching
status: In Review
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[overview]]", "[[build-system]]", "configure.py", "objdiff.json", "README.md"]
---

## Goal

Build system (OK build) that assembles/compiles and verifies output checksum against the original.

## Acceptance criteria

- [x] `configure.py` generates `build.ninja`; `tools/docker.sh ninja` reproduces `SLPM_86.053` with sha1 `e823bd84...3b26` (`build/SLPM_86.053.bin: OK`).
- [x] `include/include_asm.h`, `include/common.h`, and `src/game.c` (831 `INCLUDE_ASM`) exercise the C path (cpp, cc1 2.7.2-psx, maspsx, as).
- [x] `objdiff.json` present; README.md and [[build-system]] document the steps.

## Notes

Verified from a clean tree (`rm -rf asm build`): split, build, sha1 OK, second `ninja` is a no-op. Follow-ups: [[tickets/T-0009-progress-report-script]], [[tickets/T-0011-game-code-file-boundaries-and-compiler]].

## Comments

- 2026-10-09: moved to In Review. Awaiting code-review against CODING_STANDARDS.md.
