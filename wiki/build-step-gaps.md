---
type: analysis
updated: 2026-10-20
---

# What the build is missing

Summary of what the toolchain (IDO 5.3 + frame_pass, cvt_pass, trailing_pad, `-nokpicopt`, `-cckr`) still cannot
reproduce, so the last functions do not block a full match. Sources: [[data/true-gaps]] (T-20710 census),
[[ido-52-evaluation]] (T-20700), the T-20600/T-20610/T-20620 family passes, [[uopt-register-allocation]] (T-9800).

## Census (T-20710, 2026-10-17)

This is a historical census, not the current remaining-function inventory. [[tickets/T-24092-ugen-block-copy-state-pass]] subsequently implemented the block-copy direction correction in `tools/ugen_pass.py`, enabled by `tools/cc.py`. Its ticket records 17 passing tests, no regressions across 6,337 then-matched functions, and 12 newly promoted functions (6,748 bytes). These historical checks were not rerun for this documentation correction.

87 guarded non-TT drafts: 9 registers only, 8 order only, 3 both, 15 within two unpaired instructions, 52 still
structurally off. For the 55 drafts within six unpaired instructions (32,308 bytes) the first divergence is:

| Component | Cases | Detail |
| --- | ---: | --- |
| uopt | 28 | range set 15, priority 7, colour order 4, tie 2 |
| as1 order | 12 | instruction from a later source line issued first |
| ugen | 11 | block-copy state 5, temporary numbering 6 |
| other | 4 | operand order 1, local layout 1, known recipe not yet applied 2 |

## Gap by gap

| Gap | Status | What would close it |
| --- | --- | --- |
| uopt tie-break | Closed as a build step: only 2 true ties (EVENT func_80114B0C, TACO func_80134A18), both first-use order; every tie variant tried changes 661-1,724 matched functions (T-9800). | Source recipes only. |
| uopt range set / priority / colour order (26) | Open. Not tie-breaks; no uniform rule found. Most register-order gaps solved so far turned out to be source shapes (T-16920, T-20600, T-20610). | Keep applying the recipe catalogue; re-census after the next matching round to see what is left. |
| as1 order (12) | IDO 4.1's as1 equals 5.3's on 6,335 of 6,336 matched functions (T-20700); as1 options change nothing useful (T-20620). Both schedule per source line. | Source line grouping (T-20620 recipes); no build step expected. |
| ugen block-copy direction state (historical 5) | Addressed by T-24092 and the enabled `ugen_pass.py`. The cause is an uninitialized stack read in `f_eval_mov`, with preceding branch/store helpers affecting forward/backward copy direction for unaligned copies larger than 32 bytes. | Use the current compiler pipeline. Do not repeat the old whole-ugen swap or prototype request. A remaining copy mismatch needs a fresh reproducer; a 32-byte unrolled-copy register mismatch is not automatically this issue. |
| ugen temporary numbering (historical 6) | Current status requires remeasurement after T-24092 and subsequent source matches. | Diagnose the first divergence with the current pipeline; do not assume the old census still names unmatched functions. |

## Next steps

1. Keep matching with the current enabled passes and source recipes. Check current INCLUDE_ASM status before reusing any target from the October 17 census.
2. Preserve exact current-pipeline reproductions for remaining scheduler, range-allocation and copy-register failures.
3. Re-run the census over the remaining functions before proposing another uniform compiler correction.
