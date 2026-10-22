# T-25059: ETC exam ranking generator

`func_801480D0` in `src/ovl/ETC/80146400.c` matches all 702 instructions / 2,808 bytes in the default C build. The complete translation unit also needs a `void` forward declaration before its already matched caller.

The corrected T-24049 body reproduced 709 instructions, aligned cost 859, and a 216-byte frame. A logging-only uopt replay reproduced all 709 instructions exactly. T-24513's pointer-walk, return-type and debug-call controls were already exhausted; this work followed counter continuity and stack homes instead.

## Source changes and measurements

| Candidate | Instructions | Aligned cost | Evidence |
| --- | ---: | ---: | --- |
| Corrected full-TU baseline | 709 | 859 | Target has 702 instructions and a 208-byte frame. |
| Both tie loops reuse generation counter as inner counter | 690 | 700 | Removes the separate saved next-index range. |
| Selection loop uses the same counter roles | 705 | 621 | Restores the later induction structure. |
| Separate first-noise scalar | 705 | 595 | Restores the target frame and four saved registers. |
| Target evaluation order, conditional layout and scalar homes | 704 | 526 | First generation block matches. |
| Second random product assigned to existing `a` | 704 | 434 | Restores the product in v1. |
| Increment tie-change count before decrementing score | 704 | 430 | Restores peeled and unrolled store scheduling. |
| Rank curve and top-score shift share `offset` | 705 | 249 | Recovers their target t0 lifetime. |
| Lower-bound adjustment explicitly computes `1 - minimum` | 701 | 53 | Prevents distribution into each array addition. |
| Direct player-rank accesses replace cached `last` | 702 | 12 | Preserves the reload after updating the other 13 ranks. |
| Final player correction uses `j` | 702 | 0 | Gives the final adjustment its target a3 carrier. |

The generation expressions retain both random calls per sample and the signed divisor. Reversing the product operands recovers the observed evaluation order. The selector's nested conditional puts the case-9 body before case 1. Scalar placement follows the target's stack homes: noise at 188, angle at 164, offset at 172 and the values array at 104, all relative to the 208-byte frame. The corrected eleven-record bonus bounds from T-24049 remain.

## Controls and retained FAKE

Late-zero next-index expressions changed unrolling and did not recover the missing structure. A one-element noise array improved an intermediate candidate but was unnecessary after the natural scalar lifetimes were restored. A switch, other offset carriers and commuting the outer score sum all regressed the corresponding candidate.

The old `last` scalar no longer carries a value. Removing its declaration changes stack homes and gives aligned cost 42; renaming it to unused `pad` retains the match. The scalar order and unused word are marked FAKE in source. No artificial runtime identity, compiler exception, shared-header change or assembly fallback remains.

## Verification and review

Run in the existing Docker image with the repository at `/work` and the ignored disc inputs mounted read-only:

```sh
python3 tools/funcdiff.py --resolve ETC:func_801480D0
ninja -j2 build/ovl/ETC.ok
python3 tools/check_headers.py
python3 tools/migrate_globals.py --check
python3 tools/sync_protos.py --check-branch
```

The literal comparison reports the default C build and equality for all 2,808 linked bytes. ETC SHA1 is `681c93b6e57c885f86e074c885a8bf6c3cca2319`. Header and global checks pass; the prototype check reports zero new and 15 known disagreements. The file now contains no `INCLUDE_ASM` or `NON_MATCHING` guards.

Root independently reviewed the final source against the standards and scope, including initialized locals, fourteen-entry loops, direct rank updates, PRNG calls and the marked frame word. No findings remain. The ticket stays In Review until root's independent full 27-binary integration gate.

Private complete-TU candidates, measurements and phase captures remain in `build/t25059/`. The matching checkpoints are `tail-delta-j/unit.c` and its minimized `pad-last/unit.c`; generated binaries and original instruction listings are not committed.
