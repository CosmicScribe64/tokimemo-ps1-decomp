---
id: T-24129
title: Close remaining near misses in TACO and ETC (wave 2)
status: In Progress
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24120
  - T-24121
---

## Summary

Attack the remaining functions in `TACO` (23 functions) and `ETC` (16 functions) using the newly discovered `uopt` CSE priority boost (`if (expr) {}`), `Uadd` vs `Uixa` byte-pointer cast, `*(s32 *)((u8 *)&sym)` copy-prop stopper, and `bittab` seeding tricks:
- `TACO:func_8014F820` (`src/ovl/TACO/80149F90.c`, `score=4`, 100/100 insns)
- `TACO` 1-function files: `80133870.c` (`func_80133E98`), `80138890.c` (`func_80138B2C`), `8013B170.c` (`func_8013B460`), `80143E80.c` (`func_80144094`), `80153AA0.c` (`func_80153B38`), plus `80139170.c`, `8013B990.c`, `8013DB80.c`, `8013F250.c`, `801447D0.c`, `80149F90.c`
- `ETC`: `func_80146D4C` (`80146400.c`, 264/264 insns), `func_80149F48` (`801492C0.c`, 141/142 insns), `func_80144DF0` (`80143240.c`), `func_8013FFAC` (`8013FF80.c`, 5 rows), `func_80142B80` (`80142B80.c`), `8013BEF0.c`, `80132880.c`
