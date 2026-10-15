---
id: T-12900
title: Small OPTION and main audio matching
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Maximize verified code bytes with bounded attempts on src/ovl/OPTION/*.c and src/main/80079B10.c. Based on origin/main 95c3fc9f in codex/small-option-main.

## Acceptance criteria

- Literal linked-byte funcdiff proof for every enabled C function.
- Affected binary SHA1 matches; normal default build remains exact.
- No unowned source or generated asm changes; review against CODING_STANDARDS.md before Done.

## Notes

Queued cx6/cx8 lists are leads only. Reuse full translation units for scoring. Docker only, one compiler at a time. Parent T-12900; first bounded pass T-12901.

## Comments

- 2026-10-15: Claimed the previously unstarted OPTION and main audio files. No matches claimed yet.
