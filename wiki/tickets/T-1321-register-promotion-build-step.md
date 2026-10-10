---
id: T-1321
title: Build step for the register-promotion gap
status: Done
assignee: opus-agent (o-t0018)
created: 2026-10-09
updated: 2026-10-10
links: ["[[tickets/T-0018-ugen-temp-register-order]]", "[[tickets/T-1320-tooling-work-queue-and-blocker-detector]]", "[[matching-notes]]", "[[toolchain]]", "[[data/t0018-cases]]", "[[tickets/T-3000-rematch-rv-functions-with-cvt-pass]]", "[[tickets/T-3001-shared-constant-registers]]", "[[tickets/T-3002-remaining-promotion-shapes]]"]
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

- [x] A rule that reproduces the original's register choice: `tools/cvt_pass.py` (ucode, around uopt). Of the 40 recorded rows with C written (mostly `promo`), 19 match with it (14 of them only with it); the rest fail on other shapes (T-3002) or switch placement (see Comments). Not all rows: recorded as open.
- [x] No regression in the matched functions; clean build 27 of 27 sha1 OK (four existing C bodies changed: two FAKE masks removed, two switches on a local copy).
- [x] Documented in [[toolchain]], [[matching-notes]], CODING_STANDARDS 7a.

## Comments
- 2026-10-09 (T-3100, [[original-compiler]] sections 4-5): key findings for this build step.
  - The premise "IDO promotes only in loops" is wrong. IDO 5.3 promotes a global scalar in straight-line code once it has enough references. `if (D==1) { D++; g(); D += D; }` and the matching `switch` give the original's exact shape: `lbu v1`, reload into `$v1` after the call, `addiu tN,v1,k`, and for a switch `move v0,v1` in the first delay slot.
  - The original promotes at lower counts: one switch selector (O.BIN `olh_main`, `func_8013A40C`) or one compare plus `D++` after a call (`func_8005A560`). So the change needed is the promotion decision (hypothesis: priority >= 0 instead of > 0, or no entry-load cost), not a register rename.
  - Corpus: switch chains on main-exe globals are `$v1` in 262 functions and `$v0` in 25; on overlay-defined data `$v0` in 45 and `$v1` in 24; jump-table switches `$v0` in 138 and `$v1` in 28; if-chains `$v0` in 108 and `$v1` in 11.
  - Overlay data is a counterexample to "every scalar". `func_8013AE80` (GYOZI) matches stock IDO on `switch (D_801474B8)`. Decide between "overlay data were struct members" and "only externally defined globals are promoted" first (rule 7).
  - Rejected: `-Wo,-regr,N` (uopt caller-saved pool). At 7 or 8 it gives `$v1` selectors without promoting, and 194 of 1647 matched functions regress (per-function object compare, immediates masked). `-rege`, `-nomultibbunroll`, `-unrolllimit`, `-no_r23`, `-pic2`, `-fortran_lang`, `-f77alias`, `-dwopcode` and `-dowhyuncolor` have no effect. `-zdbug:6` aborts in the recomp.
  - Suggested implementation: a one-comparison patch of uopt's allocation threshold in a copy of the recompiled uopt, located via the uopt reconstructions. Gate on all `promo` rows, the 1647 matched functions and 27 of 27 sha1. The `reverse` rows are not explained by this rule.

2026-10-09 (opus-agent, branch o-t0018). Result: the gap is not a loop-only allocator. Two IDO details cause it: cfe widens every unsigned 8/16-bit load (`LOD L; CVT J<-L`) and uopt allocates the widened expression instead of the global; and uopt's global copy propagation merges a switch temporary into its unsigned global. `tools/cvt_pass.py` (uopt shim in `tools/cc.py`, tests `tools/test_cvt_pass.py`, 26 tests incl. real-IDO snippets) removes the widening for globals accessed only as unsigned and narrower than a word, keeps IDO's `==`/`!=` operand order and unsigned loads, and sets `OPTN zcopy 0` for procedures with an unsigned-global switch temporary. Evidence, rejected alternatives (uopt `-zmovc`, threshold patch prototype, `-nordstore`, vreg tricks) and batch guidance: [[matching-notes]] "Unsigned-load conversion pass (T-1321)".
- Matched corpus: everything still matches except `bustup_speech`/`bustup_wink` (FAKE masks removed, natural C matches), ETC `func_80145FF0`/`func_80145960` (switch on a local copy, the original's `$v0` form). Clean build `rm -rf asm build; configure.py; ninja`: 27/27 OK. `ninja progress` 1168 -> 1207 of 6962.
- Newly matched in the build: 26 ETC/TACO functions that need the pass (80-300 bytes) and 13 larger R-flagged RPG_BAT/TACO functions (540-804 bytes) that match with plain C (detector false positives). 14 more pass-only matches exist as scratch C for main-exe and small-overlay files owned by other agents (`func_8005B798`, `func_80060B78`, `func_80061688`, `func_80062634`, `func_80070F80`, `func_800725B0`, `func_800728A4`, `func_80072BC0`, `uwasa_main`, NAME_ENT `func_80132134`/`func_8013C4A8`/`func_80144AD0`/`func_80145A74`, EN_NICHI `func_8013216C`); not applied here to avoid conflicts; T-3000.
- Detector precision (sample of 148 flagged functions with C): 40 need the pass, 19 match without it, 89 differ for other reasons: 68% of the resolved ones. No pass-needed match above 300 bytes yet; the larger pass candidates found (TACO `func_8013760C`, ETC `func_8013CCE4`, RPG_BAT `func_80153784`) fail on other differences.
- uopt patch question (coordinator): a scratch prototype of uopt's promotion threshold (`adjsave > 0` to `>= 0` plus the two split comparisons of `globalcolor`, byte patch of the recompiled binary, never committed and deleted) does not reproduce the original (it promotes compare constants and addresses, breaks `func_80145960` and DATE2 `func_8013279C`) and is not needed: the ucode pass gives the original's code without changing IDO. No patch is proposed.
- Code review (inline, CODING_STANDARDS 13): matches verified by the clean build; no `NON_MATCHING` added; C89, fixed-width types; new `include/ovl/RPG_BAT.h` has guards and includes `game.h`; `tools/check_headers.py` OK; the `(u32)` casts in ETC `func_801460C4` are commented (the counters are probably `u32`); one agent draft that passed a stale fourth argument (RPG_BAT `func_80151984`) was not applied; the pass meets 7a (uniform, documented in its docstring and [[toolchain]], evidence in [[matching-notes]], fails loudly with `cvt_pass.py: ...`, synthetic unit tests, replaceable). Finding: main-exe symbols used only by these overlays were declared in the overlay headers (existing TEL/GEKO practice, check passes); left as is. No open findings. Done.

2026-10-09 merge with main (per-object layout, 2734 matched). The pass changes 26 of main's matched functions (20 `$v0` switches on unsigned globals, 4 promotions the original does not make, 2 FAKE masks it would make unnecessary); no rule on the C or the ucode separates the original's `$v0` and `$v1` switches (same C). Per the adoption gate it is taken out of the build (`tools/cc.py` keeps an `extra_shims` hook; tool and tests stay). Added a constant-first rule for `==`/`!=` (DATE `func_8013E184`, GEKO `func_8013F02C`). Ported the 13 plain-C matches into the per-object files; the 40 pass-only matches stay scratch. Clean build 27/27, `check_headers` OK, `ninja progress` 2747/6958 (main 2734 + 13). Detector retune left to [[tickets/T-3000-rematch-rv-functions-with-cvt-pass]], which now also holds the open decision (encode the original's non-promotable selectors in C, e.g. as struct or array members, then enable the pass).

2026-10-10 (T-5010): the pass is in the build. Two rules added (entry: only variables touched before the procedure's first call, branch or label; compare: the `CVT` goes only where the value is compared or switched on). With them it changes 17 of 3649 matched functions, 16 on unit-private data plus ETC `func_80145960`, all rewritten as `FAKE` local copies; the 40 pass-only bodies of this ticket are matched in the build. [[tickets/T-5010-t0018-register-order-second-attempt]].
