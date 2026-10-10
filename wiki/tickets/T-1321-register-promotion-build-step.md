---
id: T-1321
title: Build step for the register-promotion gap (deferred)
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[tickets/T-0018-ugen-temp-register-order]]", "[[tickets/T-1320-tooling-work-queue-and-blocker-detector]]", "[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Make IDO 5.3 produce the original's register promotion of global scalars (the T-0018 gap) with a uniform build step or a patched uopt, so the functions flagged R or V by `tools/queue.py` can be decompiled. Deferred: no rule is known yet; this ticket holds the evidence and the case list.

## Evidence

- The original keeps every load of a scalar global in one register across straight-line code (the next load after a call goes into the same register); IDO 5.3 and 7.1 do this only inside loops. In 189 of 195 functions that re-load a byte global every load uses one register ([[matching-notes]], "Register promotion of globals (T-0018)").
- No uopt option, pass mix, flag or C shape reproduces it. A binasm rewrite fails the uniform-rule test of CODING_STANDARDS 7a; next experiments are listed in [[matching-notes]] (read uopt's promotion priority in the ido-decomp sources, an older MIPS uopt, patching one weight in a copy of uopt).
- Detector ([[tickets/T-1320-tooling-work-queue-and-blocker-detector]], `tools/queue.py`): of the 6248 functions still `INCLUDE_ASM` (2231464 bytes), 1305 are flagged R or V (853844 bytes): 953 R, 739 V, 387 both. None of the 714 matched functions is flagged. Recorded cases: 41 of 42 found by the detector. Re-count with `tools/docker.sh python3 tools/queue.py --summary`.

## Rule for batch agents

When a function is skipped because of this gap, append one row to [[data/t0018-cases]] (file, function, category, symptom). Never edit or delete rows. The table calibrates the detector (`queue.py --calibrate`) and is the test set for any future fix: when a fix lands, every `promo` row must match.

## Acceptance criteria

- [ ] A rule (or a uopt patch) that reproduces the original's register choice on all `promo` rows of [[data/t0018-cases]].
- [ ] No regression in the 714+ matched functions; 27 of 27 sha1 OK.
- [ ] Documented in [[toolchain]] and [[matching-notes]]; passes CODING_STANDARDS 7a if it is a build pass.

## Comments
- 2026-10-09 (T-3100, [[original-compiler]] sections 4-5): key findings for this build step.
  - The premise "IDO promotes only in loops" is wrong. IDO 5.3 promotes a global scalar in straight-line code once it has enough references. `if (D==1) { D++; g(); D += D; }` and the matching `switch` give the original's exact shape: `lbu v1`, reload into `$v1` after the call, `addiu tN,v1,k`, and for a switch `move v0,v1` in the first delay slot.
  - The original promotes at lower counts: one switch selector (O.BIN `olh_main`, `func_8013A40C`) or one compare plus `D++` after a call (`func_8005A560`). So the change needed is the promotion decision (hypothesis: priority >= 0 instead of > 0, or no entry-load cost), not a register rename.
  - Corpus: switch chains on main-exe globals are `$v1` in 262 functions and `$v0` in 25; on overlay-defined data `$v0` in 45 and `$v1` in 24; jump-table switches `$v0` in 138 and `$v1` in 28; if-chains `$v0` in 108 and `$v1` in 11.
  - Overlay data is a counterexample to "every scalar". `func_8013AE80` (GYOZI) matches stock IDO on `switch (D_801474B8)`. Decide between "overlay data were struct members" and "only externally defined globals are promoted" first (rule 7).
  - Rejected: `-Wo,-regr,N` (uopt caller-saved pool). At 7 or 8 it gives `$v1` selectors without promoting, and 194 of 1647 matched functions regress (per-function object compare, immediates masked). `-rege`, `-nomultibbunroll`, `-unrolllimit`, `-no_r23`, `-pic2`, `-fortran_lang`, `-f77alias`, `-dwopcode` and `-dowhyuncolor` have no effect. `-zdbug:6` aborts in the recomp.
  - Suggested implementation: a one-comparison patch of uopt's allocation threshold in a copy of the recompiled uopt, located via the uopt reconstructions. Gate on all `promo` rows, the 1647 matched functions and 27 of 27 sha1. The `reverse` rows are not explained by this rule.
