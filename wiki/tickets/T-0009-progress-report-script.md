---
id: T-0009
title: Progress reporting script
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[overview]]"]
---

## Goal

A script (tools/, Python 3, Docker) that reports decompilation progress: INCLUDE_ASM count versus C functions, bytes matched, per segment.

## Acceptance criteria

- [x] `tools/progress.py` prints functions and bytes decompiled per segment, exits non-zero on failure.
- [x] Documented in [[build-system]].

## Notes

Count `INCLUDE_ASM` lines in `src/` against function sizes from the splat `nonmatching` headers.

## Comments

- 2026-10-09: implemented `tools/progress.py` + `ninja progress`; output `game 10/831 functions, 160/284404 bytes (0.1%)` after the T-0011 getters. Review (code-review, medium): checks added that every split function is either `INCLUDE_ASM` or has a C body and has a size header; `progress` no longer depends on the sha1 check. Segment granularity: only `game` exists as a C segment today.
