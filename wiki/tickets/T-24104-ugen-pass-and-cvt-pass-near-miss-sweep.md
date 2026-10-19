---
id: T-24104
title: Sweep remaining unmatched functions under ugen_pass and cvt_pass rpar fix and close unlocked near misses
status: Done
priority: high
type: code
created: 2026-10-18
updated: 2026-10-18
---

## Summary

With `tools/ugen_pass.py` (`T-24092`) wired into `tools/cc.py` and the `tools/cvt_pass.py` `_constant_first` `rpar` refinement (`T-24103`), scan all remaining unmatched functions and archived drafts across `drafts/` (`T-22004`, `T-22*`, `T-23*`, `T-24*`) to find any additional functions whose instruction diffs drop to `<= 10` rows, and close the resulting near misses in disjoint compilation units (`SHUGAKU/801344B0.c`, `SHUGAKU/80138A60.c`, `ENDING/80137340.c`).

## Result

1. Ran a repo-wide live score sweep (`build/scratch/t24104/sweep_drafts.py`, `build/scratch/t24104/ranked_drafts.txt`) across all remaining `INCLUDE_ASM` functions.
2. Matched and promoted 4 functions (1,584 B) to unguarded default C:
   - `SHUGAKU:func_80134658` (368 B in `src/ovl/SHUGAKU/801344B0.c` — `src/ovl/SHUGAKU/801344B0.c` now 100.0% matched!)
   - `SHUGAKU:func_801392B4` (272 B in `src/ovl/SHUGAKU/80138A60.c`)
   - `ENDING:func_80137BB4` (652 B in `src/ovl/ENDING/80137340.c`)
   - `ENDING:func_80137E40` (292 B in `src/ovl/ENDING/80137340.c`)

