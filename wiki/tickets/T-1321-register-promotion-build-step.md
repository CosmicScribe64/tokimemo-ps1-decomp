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
