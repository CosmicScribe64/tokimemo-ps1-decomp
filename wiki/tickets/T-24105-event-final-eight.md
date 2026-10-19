---
id: T-24105
title: Close out the final 8 unmatched functions of the EVENT overlay
status: In Progress
priority: high
type: code
created: 2026-10-18
updated: 2026-10-18
---

## Summary

Only 8 `INCLUDE_ASM` functions remain in the entire `EVENT` overlay:
1. `EVENT:func_80114B0C` (`src/ovl/EVENT/8010FE10.c` — last remaining function in `8010FE10.c`, twin of `DATE:func_80137154`)
2. `EVENT:func_800FC040` (`src/ovl/EVENT/800FBBF0.c` — last remaining function in `800FBBF0.c`, twin of `GEKO:func_8013BB3C`)
3. `EVENT:func_800FEAE8` and `EVENT:func_800FEDC8` (`src/ovl/EVENT/800FDC20.c` — last 2 functions in `800FDC20.c`)
4. `EVENT:func_801022C0` (`src/ovl/EVENT/801011A0.c` — last remaining function in `801011A0.c`)
5. `EVENT:func_801035BC` (`src/ovl/EVENT/801024B0.c` — last remaining function in `801024B0.c`)
6. `EVENT:func_8010F55C` and `EVENT:func_8010F74C` (`src/ovl/EVENT/8010BB90.c` — last 2 functions in `8010BB90.c`)

Close as many of these final 8 `EVENT` functions as possible in `build/scratch/t24105/` and promote all `score=0` matches to `src/ovl/EVENT/`.
