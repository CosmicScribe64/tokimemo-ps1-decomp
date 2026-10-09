---
id: T-0009
title: Progress reporting script
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[overview]]"]
---

## Goal

A script (tools/, Python 3, Docker) that reports decompilation progress: INCLUDE_ASM count versus C functions, bytes matched, per segment.

## Acceptance criteria

- [ ] `tools/progress.py` prints functions and bytes decompiled per segment, exits non-zero on failure.
- [ ] Documented in [[build-system]].

## Notes

Count `INCLUDE_ASM` lines in `src/` against function sizes from the splat `nonmatching` headers.

## Comments
