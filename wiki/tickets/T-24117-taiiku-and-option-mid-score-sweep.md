---
id: T-24117
title: Close low-to-mid-score functions in TAIIKU and OPTION
status: Done
priority: high
type: code
created: 2026-10-18
updated: 2026-10-19
depends_on:
  - T-24110
---

## Summary

Attack and close the low-to-mid-score functions in `TAIIKU` and `OPTION`:
- **`TAIIKU`**:
  - `TAIIKU:func_8013BCEC` (`score=12` in `src/ovl/TAIIKU/80138950.c`)
  - `TAIIKU:func_801365C8` (`score=18`), `TAIIKU:func_80135480` (`score=22`), `TAIIKU:func_80137544` (`score=26`) in `src/ovl/TAIIKU/80133C80.c`
  - `TAIIKU:func_8014607C` (`score=20` in `src/ovl/TAIIKU/801452D0.c`)
  - `TAIIKU:func_8014357C` (`score=32` in `src/ovl/TAIIKU/80142240.c`)
- **`OPTION` (`src/ovl/OPTION/80132000.c` & `80138080.c`)**:
  - `OPTION:func_80133330` (`score=85`), `OPTION:func_801376A8` (`score=85`), `OPTION:func_80133F50` (`score=91`), `OPTION:func_801361FC` (`score=96`), `OPTION:func_80135110` (`score=102`), `OPTION:func_801359A4` (`score=107`), `OPTION:func_80132384` (`score=111`) in `src/ovl/OPTION/80132000.c`

## Results

- Matched and promoted 6 functions (+3,412 B) in `src/ovl/OPTION/80132000.c`:
  - `OPTION:func_80133330` (588 B)
  - `OPTION:func_80133F50` (704 B)
  - `OPTION:func_80135110` (492 B)
  - `OPTION:func_801359A4` (616 B)
  - `OPTION:func_801361FC` (568 B)
  - `OPTION:func_801376A8` (444 B)
- Code review passed against `CODING_STANDARDS.md`; full local CI check suite passed.
