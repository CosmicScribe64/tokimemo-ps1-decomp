# T-24043 larger MASTER handler draft

This archive gives the next matching pass a corrected source baseline for `func_80139CC8` and records the bounded controls already tried. Production retains its assembly fallback. There are no newly verified C bytes.

## Source corrections

`MASTER/func_80139CC8/body.c` starts from the T-18600 typed/rerolled draft and current `DisplayFixActor44`/`Fix32` fields. The five-byte array is real: case 7 exchanges old elements 0 and 4 using two saved bytes. The old draft overwrote element 4 before reading it for element 0. The probability is read after RNG and reloaded after shuffle calls, preserving possible global mutation. All array accesses stay within the declared object for the target's bounded indices.

Threshold comparisons, palette/fill/shuffle counters, and selector expressions now use ordinary control flow and shared locals with disjoint lifetimes. Removed m2c spill self-assignments are compiler artifacts. The final draft has the original 0x48 frame and only the saved return address, without padding. Its scalar probability-byte view is marked FAKE because it controls compiler promotion; it follows the existing MASTER T-10630 recipe. No header, config, data ownership, or production source changed.

## Measurements and controls

Run `tools/docker.sh python3 drafts/T-24043/measure.py`. It compiles a private full-TU copy with the real C body substituted for the target fallback, then compares resolved instructions. It does not claim a default C match.

| Candidate | Diagnostic cost | Printed rows |
| --- | ---: | ---: |
| Corrected typed draft | 542 | 552 |
| Actor pointer | 540 | 550 |
| GameState pointer | 550 | 560 |
| Natural thresholds and loops | 489 | 521 |
| Natural form plus GameState pointer | 485 | 517 |
| Scalar probability-byte view | 350 | 532 |
| Real selector local, target frame | 346 | 528 |
| Increment-before-store fill (archived) | 342 | 524 |

Costs are aligned differing-instruction diagnostics. Printed rows can be larger because equal-length functions use positional evidence. They are not linked-byte proofs and are not directly comparable to the older T-18600 678-row report.

Other rejected controls: dead spill-assignment removal and merged club selectors leave cost 542; merging counters alone scores 571. A separate threshold local plus pointer fill scores 425 and swapping byte-load order 441. Explicit alpha local leaves 342 unchanged. Early exit in case 9 scores 357, with a selector byte view 355; reversed advance condition scores 347. No broad permutation was run.

A single private full-TU tentative `u8 D_8013C788[5];` definition scores the same 342/524 and retains the extra swap LUI. Its object symbol is COMMON. A separate initialized five-zero-byte definition places the symbol in `.data`, removes the local swap instruction differences, and scores 357/375 overall. Other mismatches remain, and function length changes make the diagnostic totals misleading as a ranking of this local fix. This establishes an effect on the swap, not a full solution or an implemented data island. Both private definitions are excluded from this archive.

## Remaining blockers

The archived live spill homes are counter +0x38, swap count +0x34 and first index +0x40; the target uses +0x34, +0x30 and +0x3C. Reordering real declarations did not move these homes. The swap's two external-array stores retain one extra LUI, and the two byte loads schedule differently. Case 8's alpha/RNG setup begins a register rotation; case 9 advances through a different branch layout. Later record loops retain shifted registers and base-sharing choices. These are concrete source/code-generation differences, not a proven compiler or data-ownership blocker.

Ignored worktree evidence is under `build/master24043/`: stage logs, per-candidate diagnostics, `archive-proof.log`, and `production-audit.log`. Only source and provenance are committed; no generated assembly or game data is included.
