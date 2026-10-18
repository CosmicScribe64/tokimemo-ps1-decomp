---
id: T-24097
title: Match EVENT:func_8011DDB8 (3,992 B) and DATE/EVENT movie/scene siblings (func_8013FD6C/func_8011B848)
status: In Progress
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

Following the 4-for-4 completion of [[tickets/T-24089-date-event-movie-sibling-pairs]] (+11,536 B across `src/ovl/DATE/801461B0.c` and `src/ovl/EVENT/8011C650.c`), close out the final remaining function in `src/ovl/EVENT/8011C650.c` plus the next `DATE`/`EVENT` high-similarity twin pair:

1. `EVENT:func_8011DDB8` (**3,992 B** in `src/ovl/EVENT/8011C650.c`) — the ONLY remaining unmatched function in `src/ovl/EVENT/8011C650.c`, with matched sibling `DATE:func_80147958` (4,044 B in `src/ovl/DATE/801461B0.c`) at **0.953** opcode similarity! Note that `func_8011DDB8` also calls `func_8004AFB8` in `case 40`, where `s32 func_8004AFB8()` is already overridden at the top of `src/ovl/EVENT/8011C650.c`.
2. `DATE:func_8013FD6C` (**1,436 B** in `src/ovl/DATE/8013E7B0.c`) and `EVENT:func_8011B848` (**1,460 B** in `src/ovl/EVENT/8010FE10.c`) — a **2,896 B** twin cluster (ratio 0.925) with matched sibling `DATE2:func_80135AA4` (`src/ovl/DATE2/80133440.c`).

## Acceptance criteria

- [ ] Match `EVENT:func_8011DDB8` (3,992 B) in `src/ovl/EVENT/8011C650.c` to `score=0`, making `src/ovl/EVENT/8011C650.c` 100% matched
- [ ] Match `DATE:func_8013FD6C` (1,436 B in `src/ovl/DATE/8013E7B0.c`) and `EVENT:func_8011B848` (1,460 B in `src/ovl/EVENT/8010FE10.c`) to `score=0`
- [ ] Verify with `tools/funcdiff.py --resolve`, review against `CODING_STANDARDS.md`, and log in `wiki/log.md`
