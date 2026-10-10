---
id: T-3000
title: Re-match R/V-flagged functions with cvt_pass.py and retune the T-0018 detector
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[tickets/T-1321-register-promotion-build-step]]", "[[tickets/T-1320-tooling-work-queue-and-blocker-detector]]", "[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

`tools/cvt_pass.py` (T-1321) removes the main cause of the R/V blocker for unsigned globals. Batch agents should treat R/V-flagged functions as ordinary work again, following the guidance in [[matching-notes]] ("Unsigned-load conversion pass (T-1321)", batch-agent guidance), and `tools/queue.py` should stop flagging the shapes the pass now handles.

## Acceptance criteria

- [ ] `queue.py` R/V rules updated: a global that is unsigned (lbu/lhu) and only switched or compared and reloaded after calls is no longer a blocker; keep flagging the shapes listed as open in T-1321.
- [ ] Calibration rerun against the T-1321 sample (`build/t1321` results are scratch; record the numbers in [[matching-notes]]).
- [ ] The `promo` rows of [[data/t0018-cases]] retried; matched ones noted in the ticket (rows are never edited).

## Notes

T-1321 sample: of 148 flagged functions with C written, 40 match only with the pass, 19 matched without it (detector false positives), 89 still differ for other reasons.

Open decision (T-1321 merge): `tools/cvt_pass.py` is not in the build because 20 of main's matched switches on unsigned globals keep `$v0` in the original while the same C gives `$v1` under the pass. Either find the variable property that separates them (T-3100: promotable scalar vs struct/array data) and encode it in the C, then enable the pass (`tools/cc.py` `SHIMS`), or leave the pass out. Scratch C for 40 pass-only matches is listed in [[tickets/T-1321-register-promotion-build-step]].

## Comments
