---
id: T-24096
title: Speedup audit and automation (O.BIN return-type sweep, .data island audit, recipe transforms)
status: Done
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

Implement the three decomp speedup initiatives requested by the user:
1. **Cross-Overlay `O.BIN` Return-Type & `.data` Island Audit (`build/scratch/t24096/audit_speedups.py`):**
   - Audit every `EVENT` `O.BIN` jump-table symbol (`0x8004A000..0x8004E000`) against its `SLPM_86.053` target function's return type (`void` vs `s32`/`s16`/`u16`/`void *`), identifying any unmatched `EVENT` functions that call an `O.BIN` entry whose return type in `include/main_api.h` disagrees with `SLPM_86.053`.
   - Audit all remaining unmatched functions across all units for shared-`%hi` (`lui $at, %hi(sym)`) consecutive stores or loads to `.data` symbols owned by the same object that do not yet have a `.data` island (`tools/data_island.py`).
2. **Batch Idiom Sweep Helper (`build/scratch/t24096/idiom_sweep.py`):**
   - Implement a standalone fast idiom-sweep helper combining `cmp_ior_zero` (`(x | 0) == kU`), `cse_u32_view` (`*(u32 *)&local`), `ptr_mul_zero` (`*= 0`), `pad_sweep` (`s32 pad;`), and `autotweak`/`recipes` transforms (`gstate`, `chain`, `inline_multi`, `byteview`, `emptytest`, `declswap`, `width`) in a single Docker batch run.
3. **Sibling Symbol-Transplant Helper (`build/scratch/t24096/port_sibling.py`):**
   - Build a helper that aligns a matched sibling's `.s` and `.c` against an unmatched target's `.s`, substitutes the aligned global/callee symbol map into the matched sibling's C body (filtering out `%hi` noise and preserving `D_800E6280`), and outputs a ready-to-score draft.

## Acceptance criteria

- [x] Run the `O.BIN` return-type and `.data` island audit across all 404 remaining functions and report actionable hits
- [x] Implement the fast idiom-sweep / sibling-transplant helpers and verify they work
- [x] Log findings in `wiki/log.md`

## Notes

- `.data` island audit found 18 unmatched functions across all units with shared-`%hi` accesses to their own object's `.data`:
  - 6 functions in 6 objects need `tools/data_island.py`: `GYOZI/801418B0` (`func_801421B4`), `MASTER/80139000` (`func_80139CC8`), `RENSYU/80132040` (`func_8013325C`), `SHOUGATU/8013F190` (`func_8013FB28`), `VALEN/80132760` (`func_80132834`), `main/80075320` (`func_800755B4`).
  - 12 functions are in objects that already have `.data` islands (`ETC/80143240` `func_80143A88`, and 11 `TAIIKU` functions in `80133C80`, `80138950`, `80142240`, `801452D0`) and only need their `INCLUDE_RODATA` lines replaced with C definitions.
- `EVENT` `O.BIN` return-type audit identified 2 additional unmatched callers of `s32`-returning `O.BIN` functions: `EVENT/80107FC0:func_80109404` (`func_8004C6B0`) and `EVENT/8010FE10:func_80118B4C` (`func_8004A8EC`).
- Refreshed `build/siblings.json` across all 6,952 functions.
