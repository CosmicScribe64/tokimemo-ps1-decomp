---
id: T-25013
title: "NEXT STEP: compiler-level levers (uopt/ugen/as1 emulation rules) to unblock the last ~119 functions"
status: Ready
priority: high
type: tooling
created: 2026-10-21
updated: 2026-10-21
depends_on:
  - T-25012
links: [[[status]], [[plan-remaining]], [[build-step-gaps]], [[as1-scheduler-rule]], [[matching-notes]], [[tickets/T-25012-class-c-d-reconstruction]], [[tickets/T-25011-class-a-closers-as1-levers]], [[tickets/T-24929-uopt-zcopy-scope]]]
---

## Why this is the next step

Source-level work is exhausted. After the 2026-10-19/21 sessions (T-24142, T-25011, T-25012) every guarded near miss was swept (autotweak 280-1,940 variants each, permuter 300-600 s, order_search, hand variants) and the unguarded functions that were reconstructed from the asm stop 70-100 rows away for the same reasons. `tools/twin_sweep.py` finds no matched twin for any of the 119 unmatched functions. Further hand work with the current build pipeline will keep producing guards, not matches. What is left needs the build pipeline to model the original compiler better, one uniform rule at a time (the way T-24929 did for ETC `func_8013CCE4`).

## Rules (do not skip)

- A build-step change is allowed only as ONE uniform rule that (a) has evidence it models the original compiler, (b) changes no already-matched function (compare every object before and after; `drafts/T-24929/objcmp.py` does this), and (c) passes 27/27 SHA1. No per-function or per-file options (CODING_STANDARDS section 7a, [[plan-remaining]] ground rules).
- Where the rules live: `tools/cvt_pass.py`, `tools/ugen_pass.py`, the frame pass and `tools/cc.py`. Existing precedents: T-24929 (switch compares), T-24103 (`_constant_first` before `rpar`), the cvt pass NO_WIDEN helpers.
- Use `tools/regdiff.py` with `TOKIMEMO_SCRATCH=$PWD/build/scratch/<ticket>` and the traced uopt copied to `$TOKIMEMO_SCRATCH/r9-uopt/variants/trace` (source: `/private/tmp/t24047-uopt/variants/trace`; if that copy is gone, rebuild it from the instrumented uopt of T-9800, see [[ending-allocator-trace]]). Check `regdiff` before trusting `score.py`'s `struct` count, which masks registers.

## Levers identified (one candidate target each, evidence in the linked notes)

1. **Keep a redundant store of a promoted global.** main `func_8006A044`: the original keeps F64[1] live in `$v0` after every OR (`or t5,v0,zero; ori; andi v0`) and re-stores it; ours drops the redundant stores (12 instructions missing, about 87 rows). Also helps ETC `func_8013F054` (8 instructions missing). Notes: `drafts/T-25012/main/func_8006A044/`, `drafts/T-25012/ETC/func_8013F054/`.
2. **8-byte shift of the locals area.** main `func_80062948`: the original's locals sit 8 bytes lower (0x70/0x80/0xA0/0xC0 in a 0xD0 frame; ours 0x78/0x88/0xA8/0xC8) and an unused local only grows the frame. Find what the original compiler reserves there (outgoing-argument area or a spilled temporary) and model it. Notes: `drafts/T-25012/main/func_80062948/`. Related: NAME_ENT `func_80138C1C`/`func_801398EC` frame work (T-24132), TAIIKU `func_8013E1FC` (T-24142 `ox`/`oy` hole, best draft was in the lost scratch; the findings are in [[log]] 2026-10-19).
3. **No CSE of relational booleans.** ENDING `func_801387B0` (about 80 rows): the original recomputes `xor; sltiu` for each of four `0xC - (flag == 1)` compares while sharing the `lbu`/`andi`; uopt CSEs the boolean from the second compare on.
4. **as1 block-weight / delay-slot picks.** KANGEI `func_8013852C`, KANGEI `func_80134724` (4 rows), VALEN-style dispatch `sll` slot picks, GEKO `func_80136C20` and `func_801367EC` (selector `lui` should fill the branch slot, block weight 7 but the scan stops at +6), TACO `func_801511C0`/`func_8013B460`, GYOZI `func_80140E0C`/`func_8013907C`. See [[as1-scheduler-rule]]: the visit-order / weight pass is the lever to trace.
5. **Register colouring order.** Unconstrained uopt ranges are coloured in first-mention order, not priority (regdiff finding, T-25012). GYOZI `func_801412A4` (ugen temporaries t7 vs t5), BUNKA_SD `func_80134D94`/`func_80139290`, GEKO `func_80144198`-style param-in-register (`sel` kept in `$a2` instead of spilled), `D_800E7384`-style direct-symbol reads (`keepproof.py` says NOT PROVEN for BUNKA_SD `func_80134D94` and MASTER `func_80132444`).

## Method

1. Pick one lever. Take its smallest target, trace uopt/ugen/as1 on the original-shaped source and find the first divergence from the target asm.
2. Write the rule as a pass or a pass extension with a unit test in `tools/` (see `tools/test_cvt_pass.py`).
3. Rebuild everything; run the object-comparison tool over all units; run the full gate (headers, `migrate_globals.py --check`, `ninja`, 27/27).
4. Only then promote the targets the rule unblocks (each still needs funcdiff MATCH), one commit per rule.
5. Record the evidence in [[matching-notes]] (`## T-NNNN: ...`) and update [[build-step-gaps]], [[status]] and the log.

## Notes for the agent who picks this up

Cheap models stalled on this class (three Sonnet 5.5 runs, zero matches); use the strongest model available. Do not start with the source-level sweep tools again: the work they can do is done.
