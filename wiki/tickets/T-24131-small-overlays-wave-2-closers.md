---
id: T-24131
title: Wave 2 closer sweep across KANGEI, RENSYU, MASTER, EN_NICHI, ENDING, and VALEN
status: Done
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24122
---

## Summary

Attack the remaining unmatched functions across the 6 small-to-medium overlays (`KANGEI`, `RENSYU`, `MASTER`, `EN_NICHI`, `ENDING`, `VALEN`).

## Results (+5 functions, +2,084 B; `src/ovl/EN_NICHI/80132000.c` at 100.0% matched!)

- `KANGEI:func_80135004` (`src/ovl/KANGEI/80134120.c`, 220 B): `MATCH`
- `EN_NICHI:func_80132EE8` (`src/ovl/EN_NICHI/80132000.c`, 488 B): `MATCH`
- `EN_NICHI:func_8013358C` (`src/ovl/EN_NICHI/80132000.c`, 596 B — **`src/ovl/EN_NICHI/80132000.c` 100.0% matched, 39/39 functions!**): `MATCH`
- `MASTER:func_8013B804` (`src/ovl/MASTER/80139000.c`, 428 B): `MATCH`
- `ENDING:func_8013759C` (`src/ovl/ENDING/80137340.c`, 352 B): `MATCH`
- Discovered three new `uopt` + `ugen` idioms:
  1. `uopt` Register-Candidate Constant LIFO Ordering (`$t0 -> $t1 -> $t2 -> ...`).
  2. `ugen` `<expr> * 0` subtree discard (counted by `uopt` during `bittab` creation and `adjsave` priority calculation, then discarded by `ugen` with zero instructions emitted).
  3. `uoptinput` `==`/`!=` operand swap via `(v | 0) == C` or `+ (<non_inv> * 0)`.
- Improved `#ifdef NON_MATCHING` guard for `KANGEI:func_8013852C` to 2 rows (`src/ovl/KANGEI/80137B90.c`).
- Remaining functions across `KANGEI` (4), `RENSYU` (3), `MASTER` (6), `EN_NICHI` (2), `ENDING` (5), and `VALEN` (4) continue in [[tickets/T-24138-small-overlays-wave-3-closers]].

## Review (`CODING_STANDARDS.md`)

- Verified `check_headers.py`, `ninja build/ovl/KANGEI.ok build/ovl/EN_NICHI.ok build/ovl/MASTER.ok build/ovl/ENDING.ok build/ovl/VALEN.ok`, and `funcdiff.py --resolve`. Zero findings.
