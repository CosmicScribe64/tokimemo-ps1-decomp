---
id: T-24100
title: Convert TAIIKU .data INCLUDE_RODATA definitions to C and close TAIIKU/OPTION near misses
status: Done
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on:
  - T-24087
  - T-24096
---

## Context

The [[tickets/T-24096-speedup-audit-and-automation]] `.data` island audit (`build/scratch/t24096/audit_speedups.py`) found that 11 unmatched functions across `src/ovl/TAIIKU/80133C80.c`, `src/ovl/TAIIKU/80138950.c`, `src/ovl/TAIIKU/80142240.c`, and `src/ovl/TAIIKU/801452D0.c` already have `.data` islands configured in `config/overlays/TAIIKU.yaml` (`data_ranges`), and their `.data` variables used `INCLUDE_RODATA` in the `.c` files instead of C definitions.

## Results

- **`TAIIKU:func_80142400` (`src/ovl/TAIIKU/80142240.c`, 1,036 B)**: matched (`score=0`) and promoted to unguarded default C by sharing `i` across `case 0`'s 6 loops and `case 3`'s 2 loops (raising `i`'s `uopt` priority above `iv_ptr`'s $20.5$ so `$s0/$s1` color in the target order) while retaining `s32 pad;` for the 192-byte frame.
- **`D_8014A130[3]` (`src/ovl/TAIIKU/80142240.c`)**: replaced `INCLUDE_RODATA` with `s32 D_8014A130[3] = { 0 };` in C, reducing `TAIIKU:func_8014357C` from 34 rows to 22 rows.
- **`OPTION:func_801394F4` (`src/ovl/OPTION/801389A0.c`, 900 B)**: added guarded `#ifdef NON_MATCHING` draft at 14 differing rows (down from 56 rows).

## Acceptance criteria

- [x] Replace `INCLUDE_RODATA` `.data` lines with byte-identical C definitions in `TAIIKU` where needed and close as many `TAIIKU` and `OPTION/801389A0.c` functions as possible to `score=0` in `build/scratch/t24100/`
- [x] Promote all `score=0` functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [x] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`
