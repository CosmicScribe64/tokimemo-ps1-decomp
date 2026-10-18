---
id: T-24100
title: Convert TAIIKU .data INCLUDE_RODATA definitions to C and close TAIIKU/OPTION near misses
status: In Progress
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on:
  - T-24087
  - T-24096
---

## Context

The [[tickets/T-24096-speedup-audit-and-automation]] `.data` island audit (`build/scratch/t24096/audit_speedups.py`) found that 11 unmatched functions across `src/ovl/TAIIKU/80133C80.c`, `src/ovl/TAIIKU/80138950.c`, `src/ovl/TAIIKU/80142240.c`, and `src/ovl/TAIIKU/801452D0.c` already have `.data` islands configured in `config/overlays/TAIIKU.yaml` (`data_ranges`), and their `.data` variables currently use `INCLUDE_RODATA` in the `.c` files instead of C definitions. Defining those `.data` variables in C allows `as1` to share `%hi` (`lui $at`) across adjacent `.data` variables and affects `uopt` address register allocation:

- `src/ovl/TAIIKU/80133C80.c`: `func_80134264` (1,416 B), `func_80134BF4` (2,188 B), `func_80135480` (964 B, guarded near-miss)
- `src/ovl/TAIIKU/80138950.c`: `func_80138FC8` (1,100 B), `func_8013BCEC` (524 B, guarded near-miss at 12 rows), `func_8013D114` (528 B, guarded near-miss at 12 rows)
- `src/ovl/TAIIKU/80142240.c`: `func_80142400` (1,148 B, guarded near-miss), `func_8014357C` (760 B, guarded near-miss)
- `src/ovl/TAIIKU/801452D0.c`: `func_80145B58` (1,316 B), `func_8014607C` (648 B, guarded near-miss), `func_80146304` (688 B)
- Plus the remaining 3 functions in `src/ovl/OPTION/801389A0.c` (`func_80139A30` 1,036 B, `func_80139E3C` 200 B, `func_80139F8C` 132 B) to bring `OPTION` to 100%.

## Acceptance criteria

- [ ] Replace `INCLUDE_RODATA` `.data` lines with byte-identical C definitions in `TAIIKU` where needed and close as many `TAIIKU` and `OPTION/801389A0.c` functions as possible to `score=0` in `build/scratch/t24100/`
- [ ] Promote all `score=0` functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [ ] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`
