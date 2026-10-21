# T-24904 GEKO:func_80136C20 — NOTES

Outcome: **not matched, 4 rows unchanged.** No src edit; guard left as it was
(`#ifdef NON_MATCHING` in `src/ovl/GEKO/80134D10.c`, 4 rows). Files here are
scratch variants + the generator; nothing outside `drafts/T-24904/` changed.

## The diff

```
-@row 63 3c0e800e  ; lui t6,0x0        (target: branch delay slot of `bnez t3`)
+@row 63 00000000  ; nop
-@row 65 00000000  ; nop              (target: load delay of `lbu t0, D_80145090`)
+@row 65 3c0e800e  ; lui t6,0x0
```

The `lui $t6, %hi(D_800E71DF)` is the switch selector (`D_800E6280.unk_F5F`).
Target puts it in the delay slot of `bnez $t3` (the `D_80145088` test); the
build puts it in the load delay of `lbu $t0, D_80145090` and leaves a `nop`.

## Fresh as1 trace of the current draft

Captured with `drafts/T-24900-as1/{cap,xtr,fn}.py` and the traced as1
(`/private/tmp/t24047-uopt/variants/as1-trace`, copied to
`build/as1tr/bin/as1-x2`; runs in Docker). Weights:

```
b6w4 b7w4 b8w1 b9w4 b10w4 b11w7 b12w10 casesw13 b28..31w7 b32w1
```

and the decisive lines:

```
XBBSCAN dst=8 src=10 ... weight=4
XBBRESULT dst=8 src=10 candidate=1        (lui $t0, %hi(D_80145090))
XBBSCAN dst=8 src=11 flags=fffff800 weight=7     <- no detail: scan stopped
XBBSCAN dst=10 src=11 weight=7
XBBRESULT dst=10 src=11 candidate=2      (lui $t6, %hi(D_800E71DF) -> the built output)
```

So: bb8 (weight 1, holds the `D_80145088` test and its stall) never tries
bb11 (weight 7, switch head) because 7 >= 1 + 6. bb10 (weight 4, holds the
`D_80145090` test) does reach bb11 and takes the `lui`, which is the row we see.
This confirms the recorded rule; nothing new about the mechanism.

## Variants tried (all in this directory, one docker run per batch)

| file | form | len | rows |
| --- | --- | --- | --- |
| v0 | current guard | 151 | 4 |
| v1 | early return on `D_80145088 != 0`, then early return on `D_80145090 == 0` | 151 | 16 |
| v2 | `if (D_80145088 == 0) { ... if/else 090 ... } else bustup` | 151 | 4 |
| v3 | `if / else if / else` chain | 151 | 16 |
| v4 | `goto` for both tests, labels at the end | 151 | 14 |
| v5 | outer `if` with early return inside, not else | 151 | 4 |
| v6 | `while (D_80145088 == 0)` instead of `if` | 151 | 4 |
| v7 | `switch (D_80145088) { case 0: ... default: bustup }` | 151 | 4 |
| v8 | outer switch + inner `switch (D_80145090)` | 151 | 4 |
| v9 | truthiness tests (`if (D_80145088)`, `if (!D_80145090)`) | 151 | 16 |
| v10 | `goto` + shared tail label | 151 | 14 |

Every form that keeps the byte-identical instruction stream (v0, 2, 5, 6, 7, 8)
gives the same 4 rows: the CFG, hence the blocks, block numbers and weights are
identical, so as1 makes the same choice. The forms that reorder the uniform-call
block (v1, 3, 9) or move the returns to the end (v4, 10) change bytes and score
14-16 rows.

## What this means

The weight of bb11 can only change if the CFG changes, and the CFG is fixed by
the target bytes (same branches, same targets). Two consequences:

1. The "one level lighter head" idea is not reachable from C while keeping the
   instruction stream: no source spelling of this CFG moved bb11's weight.
2. The remaining explanation is that in the original ugen emitted
   `lui $t6, %hi(D_800E71DF)` in bb8 and only the `lbu` in bb11 (a split
   address/load pair), so as1 had nothing to move.

## Next concrete hypothesis

Find a source form where ugen splits the selector address from its load, i.e.
the `%hi` lands in the `D_80145088` block and `lbu t6, %lo(...)(t6)` stays in the
switch block. Candidates to test next (each must add no instruction):

- read `D_800E6280.unk_F5F` through a `GameState *` local (`gs->unk_F5F`) declared
  before the `D_80145088` test - uopt may keep the hi part live across the test;
- two uses of the byte where the first is dead by as1 (`(void)D_800E6280.unk_F5F;`
  marked FAKE) - cheap to try, but risks an extra lbu;
- alternatively confirm the split-pair hypothesis on a matched function that
  shows a `lui %hi` in one block and the `%lo` load in another with no as1 move
  (grep matched .s for `lui` + a load of the same global in the next block).

If none of these works in 20 minutes, treat it as a scheduling case for a human
(closest open item in `wiki/as1-scheduler-rule.md`).
