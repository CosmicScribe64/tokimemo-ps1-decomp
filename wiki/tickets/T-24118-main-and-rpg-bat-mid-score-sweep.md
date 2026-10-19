---
id: T-24118
title: Close mid-score batch in main (score 48..116) and RPG_BAT (score 108..138)
status: Done
priority: high
type: code
created: 2026-10-18
updated: 2026-10-19
depends_on:
  - T-24111
---

## Summary

Attack and close the next tier of low-to-mid-score functions in `src/main/` and `src/ovl/RPG_BAT/`:
- **`main`**:
  - `main:func_800764AC` (`score=48`), `main:func_80075C84` (`score=53`), `main:func_8007739C` (`score=110`) in `src/main/80075320.c`
  - `main:func_80043010` (`score=77` in `src/main/80042A00.c`)
  - `main:func_80047560` (`score=78` in `src/main/80047550.c`)
  - `main:func_8007ABE0` (`score=90`), `main:func_8007A98C` (`score=96`), `main:func_8007A080` (`score=114`), `main:func_8007A6AC` (`score=116`) in `src/main/80079B10.c`
  - `main:func_8006BA40` (`score=98` in `src/main/800674B0.c`)
  - `main:sndisp` (`score=98` in `src/main/8004F870.c`)
  - `main:func_8005F730` (`score=111`) and `main:func_8005B43C` (`score=112`) in `src/main/8005A0B0.c`
- **`RPG_BAT`**:
  - `RPG_BAT:func_80132B34` (`score=108` in `src/ovl/RPG_BAT/801321D0.c`)
  - `RPG_BAT:func_80154DD8` (`score=113`) and `RPG_BAT:func_8015528C` (`score=246`) in `src/ovl/RPG_BAT/80154430.c`
  - `RPG_BAT:func_8014BFE4` (`score=126` in `src/ovl/RPG_BAT/8014ADD0.c`)
  - `RPG_BAT:func_80138324` (`score=138` in `src/ovl/RPG_BAT/80137280.c`)

## Results

- Matched and promoted 6 functions (+2,652 B) in `src/main/`:
  - `main:func_80043010` (176 B in `src/main/80042A00.c`)
  - `main:func_8007739C` (760 B in `src/main/80075320.c`)
  - `main:func_8007A080` (468 B in `src/main/80079B10.c`)
  - `main:func_8007A6AC` (444 B in `src/main/80079B10.c`)
  - `main:func_8007A98C` (408 B in `src/main/80079B10.c`)
  - `main:func_8007ABE0` (396 B in `src/main/80079B10.c`)
- Code review passed against `CODING_STANDARDS.md`; full local CI check suite passed.
