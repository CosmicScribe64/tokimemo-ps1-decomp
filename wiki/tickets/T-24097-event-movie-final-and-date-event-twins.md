---
id: T-24097
title: Match EVENT:func_8011DDB8 (3,992 B) and DATE/EVENT movie/scene siblings (func_8013FD6C/func_8011B848)
status: Done
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

Following the 4-for-4 completion of [[tickets/T-24089-date-event-movie-sibling-pairs]] (+11,536 B across `src/ovl/DATE/801461B0.c` and `src/ovl/EVENT/8011C650.c`), close out the final remaining function in `src/ovl/EVENT/8011C650.c` plus `EVENT` scene siblings in `src/ovl/EVENT/8010FE10.c`:

1. `EVENT:func_8011DDB8` (**3,992 B** in `src/ovl/EVENT/8011C650.c`) — the ONLY remaining unmatched function in `src/ovl/EVENT/8011C650.c`, with matched sibling `DATE:func_80147958` (4,044 B in `src/ovl/DATE/801461B0.c`) at **0.953** opcode similarity.
2. `EVENT:func_80118868` (**232 B** in `src/ovl/EVENT/8010FE10.c`), `EVENT:func_8011C164` (**656 B** in `src/ovl/EVENT/8010FE10.c`), and `EVENT:func_8011B848` (**1,460 B** in `src/ovl/EVENT/8010FE10.c`).

## Acceptance criteria

- [x] Match `EVENT:func_8011DDB8` (3,992 B) in `src/ovl/EVENT/8011C650.c` to `score=0`, making `src/ovl/EVENT/8011C650.c` 100% matched
- [x] Match `EVENT:func_80118868` (232 B) and `EVENT:func_8011C164` (656 B) in `src/ovl/EVENT/8010FE10.c` to `score=0` and diagnose `EVENT:func_8011B848`
- [x] Verify with `tools/funcdiff.py --resolve`, review against `CODING_STANDARDS.md`, and log in `wiki/log.md`

## Results

- Matched **3 functions (+4,880 B)** in `EVENT`:
  - `EVENT:func_8011DDB8` (3,992 B in `src/ovl/EVENT/8011C650.c` + `include/ovl/EVENT.h` — **`src/ovl/EVENT/8011C650.c` is now 100.0% matched!**): `MATCH`
  - `EVENT:func_80118868` (232 B in `src/ovl/EVENT/8010FE10.c`): `MATCH`
  - `EVENT:func_8011C164` (656 B in `src/ovl/EVENT/8010FE10.c`): `MATCH`
- Diagnosed `EVENT:func_8011B848` (1,460 B): in `tools/cvt_pass.py`, `_constant_first` marks `str mt=MT_R` into `$a1` (`r5`) right before `rpar` (outgoing call argument 2 at the end of the function) as `"other"`, preventing `_constant_first` from swapping the 5 earlier `D_800B1AF6 == 0x25` comparisons (`360/365` instructions match). Handed off to [[tickets/T-24103-cvt-pass-rpar-and-event-8011b848]].
