---
id: T-24104
title: Sweep remaining unmatched functions under ugen_pass and cvt_pass rpar fix and close unlocked near misses
status: In Progress
priority: high
type: code
created: 2026-10-18
updated: 2026-10-18
---

## Summary

With `tools/ugen_pass.py` (`T-24092`) wired into `tools/cc.py` and the `tools/cvt_pass.py` `_constant_first` `rpar` refinement (`T-24103`), scan all remaining unmatched functions and archived drafts across `drafts/` (`T-22004`, `T-22*`, `T-23*`, `T-24*`) to find any additional functions whose instruction diffs drop to `<= 10` rows, and close the resulting near misses in disjoint compilation units (`SHUGAKU/80134050.c`, `SHUGAKU/80139040.c`, `KANGEI/80132080.c`, `ENDING/80133030.c`, `ENDING/80137340.c`).
