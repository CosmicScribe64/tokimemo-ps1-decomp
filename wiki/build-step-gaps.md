---
type: analysis
updated: 2026-10-17
---

# What the build is missing

Summary of what the toolchain (IDO 5.3 + frame_pass, cvt_pass, trailing_pad, `-nokpicopt`, `-cckr`) still cannot
reproduce, so the last functions do not block a full match. Sources: [[data/true-gaps]] (T-20710 census),
[[ido-52-evaluation]] (T-20700), the T-20600/T-20610/T-20620 family passes, [[uopt-register-allocation]] (T-9800).

## Census (T-20710, 2026-10-17)

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
| ugen block-copy state (5) | The one uniform compiler difference found: the original, like IDO 4.1, does not switch a struct copy to state F at a block start; IDO 5.3/5.2 do (6 reproducers in tools/ido_eval_cases/ugen_copy_state/). Swapping in 4.1's ugen changes 1,307 matched functions for other reasons, so it is not usable. | A narrow patch to the recompiled 5.3 ugen's block-copy state logic, like the uopt patch tooling (T-9800). Needs reverse engineering of ugen's rotation code; test on the 35 matched functions with F-state copies and the 5 census cases. |
| ugen temporary numbering (6) | Open; may share a cause with the block-copy state. | Re-check after a ugen block-copy patch prototype. |

## Next steps

1. Prototype the ugen block-copy patch (research, private, not in the build until it passes the adoption rule:
   uniform, evidenced, every matched function unchanged).
2. Keep the conveyor on source recipes; the census lists two recipes ready to apply (EVENT func_8010B614, GYOZI
   func_8013E638) and two functions with no recipe pass yet (GEKO func_80134228, func_80135AC8).
3. Re-run the census after the next round to measure what is left in each row.
