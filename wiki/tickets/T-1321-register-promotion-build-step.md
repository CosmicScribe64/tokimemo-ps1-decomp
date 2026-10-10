---
id: T-1321
title: Build step for the register-promotion gap
status: Done
assignee: opus-agent (o-t0018)
created: 2026-10-09
updated: 2026-10-09
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

- [x] A rule that reproduces the original's register choice: `tools/cvt_pass.py` (ucode, around uopt). Of the 40 `promo` rows with C written, 15 match with it; the rest fail on other shapes (T-3002) or switch placement (see Comments). Not all rows: recorded as open.
- [x] No regression in the matched functions; clean build 27 of 27 sha1 OK (four existing C bodies changed: two FAKE masks removed, two switches on a local copy).
- [x] Documented in [[toolchain]], [[matching-notes]], CODING_STANDARDS 7a.

## Comments

2026-10-09 (opus-agent, branch o-t0018). Result: the gap is not a loop-only allocator. Two IDO details cause it: cfe widens every unsigned 8/16-bit load (`LOD L; CVT J<-L`) and uopt allocates the widened expression instead of the global; and uopt's global copy propagation merges a switch temporary into its unsigned global. `tools/cvt_pass.py` (uopt shim in `tools/cc.py`, tests `tools/test_cvt_pass.py`, 26 tests incl. real-IDO snippets) removes the widening for globals accessed only as unsigned and narrower than a word, keeps IDO's `==`/`!=` operand order and unsigned loads, and sets `OPTN zcopy 0` for procedures with an unsigned-global switch temporary. Evidence, rejected alternatives (uopt `-zmovc`, threshold patch prototype, `-nordstore`, vreg tricks) and batch guidance: [[matching-notes]] "Unsigned-load conversion pass (T-1321)".
- Matched corpus: everything still matches except `bustup_speech`/`bustup_wink` (FAKE masks removed, natural C matches), ETC `func_80145FF0`/`func_80145960` (switch on a local copy, the original's `$v0` form). Clean build `rm -rf asm build; configure.py; ninja`: 27/27 OK. `ninja progress` 1168 -> 1207 of 6962.
- Newly matched in the build: 26 ETC/TACO functions that need the pass (80-300 bytes) and 13 larger R-flagged RPG_BAT/TACO functions (540-804 bytes) that match with plain C (detector false positives). 14 more pass-only matches exist as scratch C for main-exe and small-overlay files owned by other agents (`func_8005B798`, `func_80060B78`, `func_80061688`, `func_80062634`, `func_80070F80`, `func_800725B0`, `func_800728A4`, `func_80072BC0`, `uwasa_main`, NAME_ENT `func_80132134`/`func_8013C4A8`/`func_80144AD0`/`func_80145A74`, EN_NICHI `func_8013216C`); not applied here to avoid conflicts; T-3000.
- Detector precision (sample of 148 flagged functions with C): 40 need the pass, 19 match without it, 89 differ for other reasons: 68% of the resolved ones. No pass-needed match above 300 bytes yet; the larger pass candidates found (TACO `func_8013760C`, ETC `func_8013CCE4`, RPG_BAT `func_80153784`) fail on other differences.
- uopt patch question (coordinator): a scratch prototype of uopt's promotion threshold (`adjsave > 0` to `>= 0` plus the two split comparisons of `globalcolor`, byte patch of the recompiled binary, never committed and deleted) does not reproduce the original (it promotes compare constants and addresses, breaks `func_80145960` and DATE2 `func_8013279C`) and is not needed: the ucode pass gives the original's code without changing IDO. No patch is proposed.
- Code review (inline, CODING_STANDARDS 13): matches verified by the clean build; no `NON_MATCHING` added; C89, fixed-width types; new `include/ovl/RPG_BAT.h` has guards and includes `game.h`; `tools/check_headers.py` OK; the `(u32)` casts in ETC `func_801460C4` are commented (the counters are probably `u32`); one agent draft that passed a stale fourth argument (RPG_BAT `func_80151984`) was not applied; the pass meets 7a (uniform, documented in its docstring and [[toolchain]], evidence in [[matching-notes]], fails loudly with `cvt_pass.py: ...`, synthetic unit tests, replaceable). Finding: main-exe symbols used only by these overlays were declared in the overlay headers (existing TEL/GEKO practice, check passes); left as is. No open findings. Done.
