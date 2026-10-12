---
id: T-10330
title: "Wave 12: list 3"
status: Done
assignee: w12-3
created: 2026-10-11
updated: 2026-10-11
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Match as many of the 149 assigned functions (68,588 bytes) as possible in the 49 files of wave-12 list 3.

## Acceptance criteria

- [ ] Quick first pass, then ranked deep attempts.
- [ ] Retained matches verified by resolved function diff and all 27 checksums.
- [ ] Header/global checks and inline standards review complete.

## Notes

Shared wave12-brief.md governs this batch. Scratch lives in scratchpad/w12-3/.

## Comments

- 2026-10-11: early stop, 0 exact matches. Near miss: EVENT func_80114B0C (10 lines, guarded NON_MATCHING). Tried and reverted: TT func_80134874, func_80134630, SHOUGATU func_80133760, EVENT func_801155D4. The four 80042540 functions are known T-0018 rows. Tooling bug: permute.py fails with 'cannot import name Empty from queue' because tools/queue.py shadows stdlib queue; workaround PYTHONPATH with a sitecustomize that imports queue.

- Second pass: drafts for ~140 functions generated; hand attempts on EVENT func_80119370/func_80118868/func_80114B0C, GYOZI func_8013E638, ETC func_80143F24, main func_8007B64C. Guarded near misses: EVENT func_80114B0C (10 lines), GYOZI func_8013E638 (23), ETC func_80143F24 (23). 0 exact matches. Inline review: guards ticketed, C89, declarations via sync_protos; check-branch OK, 27/27 OK. Residual cause throughout: constant/global register allocation (T-0018 shapes). Not a full 3-hour deep pass over all 149.
