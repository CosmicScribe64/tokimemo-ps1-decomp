# T-24502 carousel lifetime checkpoint

Not matched; production remains original asm. Metrics below are standalone unless explicitly labeled full TU. The corrected stale draft uses u8 table indexing and original -D_8015F4E4 << 10. Baseline282cost, scratch implicit-int font39424 return view264. No production header changes. Target is1480 bytes/370 real instructions; draft also370 real instructions, with standalone alignment372 words; frame0x68 and argument homes agree.

Fresh causal change: extending first-loop sine temp_s5_2 through a zero subtree in the final cosine branch lowers allocator priority10 to3.5. This restores radius arg2 s4, step s3, first-loop sine s5, giving218cost. A subsequent shared preheader/first-loop sine local gives200cost (archived body), but moves that shared range to s7, so the aggregate score is not a clean allocator win. To replay the clearer218 candidate from this body, declare s32 temp_s5_2; change the loop sine definition/call argument and final zero-use from temp_s5 to temp_s5_2, leaving the preheader temp_s5 unchanged.

Original saved registers: angle s0, quotient s1, index s2, step s3, radius s4, sine s5, constant40 s6, constant3 s7, table pointer fp. The218 trace has first-loop sine3.5, radius4.889, step5.167, quotient7.5, second-loop sine10; original prefix/final sine remain wrong at s1, constants3/40 reverse, constant0x100000 takes fp while table address is rematerialized. Table local alone does not help. Raw diffs/traces remain private build/T24502/carousel-*.diff and *-reg.txt.

Bounded lifetime probes on264 baseline: extend into next mirrored branch264; second sine tail264; combined pre/first/second extension264; first+second tail252; initialized next-iteration use266; latch252; loop exit238; final cosine branch218; exit both238. On218, sharing all sine sites246, preheader/first-loop200, first-loop/final252. On200, pointer local200, pointer zero-use246 plus4 real instructions, forty-times-zero200. Three constant-expression probes on218 worsen: forty-in-latch308/380 aligned words, forty-divisor370/376, early scale constant264/408. Do not repeat these grids. Historical shared sx/cx, common scales, index-derived step and shifts already failed independently; see wiki/notes/matching-05.md T-19021.

Full-TU validation: base280, implicit-int262, unshared extended216, shared-preheader198; all370 real words, with no standalone alignment. The improvements transfer, with a two-row alignment difference in standalone scoring. Use full TU for future probes. The source body comments do not alter198. The standalone context remains a small diagnostic convenience only.

Replay the full TU in Docker:

```
python3 drafts/T-24502/TACO/func_80139FB8/replay.py
```

Original font body is still nonmatching and currently void unprototyped; the scratch s32 view is ABI compatible, but any future promotion must use a justified own declaration and prove all callers. No production guard:200 is far beyond the <=30 aligned-row policy.
