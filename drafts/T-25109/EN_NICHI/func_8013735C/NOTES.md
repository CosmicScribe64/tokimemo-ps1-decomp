# T-25109: EN_NICHI func_8013735C

Result: **match**, 2300 bytes (575 words). The default build compiles the C in
`src/ovl/EN_NICHI/80136B10.c`. `funcdiff --resolve --version default` prints MATCH, and the unit
sha1 is OK. Three marked FAKEs remain (below).

## Start state

INCLUDE_ASM. Best known body: `drafts/T-24516/en-3735c.c`, 552 words against 575, aligned cost 453.

## What changed, in order, and why

The aligned cost (`funcdiff.diff_score`) after each step is in brackets.

1. **Semantics against the asm, case by case** [276]:
   - case 0 computes the slot address once and stores through it;
   - case 3 is the sibling fade loop `for (i = 1; i < 22; i++) rec[96 + i].unk_07 = rec[96].unk_07`, the same as
     func_80136DEC case 2 (IDO peels record 97, then 5 x 4);
   - case 1's fill loop is `for (; n < 5; n++)`: the counter is not reassigned afterwards, so loop 1 is
     peeled plus 4x unrolled and loop 2 stays rolled (T-5020 rule; the old draft's `for (i = n; ...)` had
     the two the other way round);
   - case 4 starts with `if (all five < 0x10) {} else if (C4C[0] == 0xFF && C4C[1] == 0xFF) {}`. The
     original keeps both test chains and the PRE reload block between them.
2. **The empty 0xFF arm.** uopt removes an empty or dead-store arm whole (tested: empty, `;`, a dead local
   or global store, a self-assign, `while (0)`, `goto`, a value-context `&&`/`||`, `switch`). Only a loop
   survives the branch cleanup. `n = 0; do {} while ((u32)n < 0);` keeps the two `bne` and emits
   nothing, because ugen folds the unsigned `< 0` (FAKE). [276, structure now equal there]
3. **unk_F5F of the first result test in v1.** Add a dead `if ((s32)D_800E6280.unk_F5F) {}` in the then-arm
   (FAKE, register-cheatsheet "F5F" row). Before it, the load went to a ugen temporary, so every ugen
   register from case 4 to the end was one slot off. Placed before the whole chain, it hoists the load
   (158). In the then-arm the load stays in place [149].
4. **Case 0/2 slot indexing: `((DisplayActor44 *)D_8011ECD0 + 98)[D_80139C30]`.** The original multiplies
   the case 0/2 index by 0x44 with `sll 4; addu; sll 2` (one register). Case 1 multiplies by the hoisted
   0x44 register (`li a3,68; multu`). uopt gives every `mpy x, 68` of the function one constant range
   (`constinreg` optype 5: a constant that is not a power of two). So `[x + 98]` (index expression ->
   `mpy`) in cases 0/2 pulled their multiply into case 1's register. A plain variable index on a record-98
   base pointer is an `ixa` with stride 68: no constant operand, ugen expands it. This was checked in a
   mini harness: `(base + 98)[x]`, `p = base + 98; p[x]` and the 2D view `((u8 (*)[0x44])D)[x][k]`
   all give shifts, while `&...[x][k]`, `[x + 98]`, `x * 0x44`, `x * 0x44U`, `(s32)x`, `(s16)x`, `j = x` and the
   `DisplayActor44 D_8011ECD0[]` override all give multu. [76]
5. **Case 0 store order 26, 2A, 01.** The ugen ring shows that the original evaluates the unk_2A load before
   the constant 4; as1 then moves `sb 4` into the load delay slot. [60]
6. **Allocation order of the hoisted constants** (all 60 remaining rows were `t1`/`t2` swapped). In the
   draft, 0xFF (9 uses, saving 51, D 6, priority 8.5) outranked 0x8011ECD0 (8 uses, saving 31, D 4,
   priority 7.75). The original has the address in `t1` and 0xFF in `t2`. A never-true two-term loop
   between case 1's loops, `do {} while ((u32)n < 0 && (u32)i < 0);`, emits no code. It adds blocks to
   the 0xFF range only, which raises its denominator to 7 (priority 7.29). [0, MATCH] A one-term loop adds
   too few blocks (still 60). Two one-term loops, a nested pair, `||`, or two `(u32)i < 0` loops also match.

## Negatives (measured)

- The empty-arm forms listed in step 2 (all removed by uopt).
- Loop 2 as `for (i = n; i < 5; i++)` unrolls it (301). `for (;;) { if (n >= 5) break; ... }` (99), a
  trailing folded loop after loop 2 (85). `while`, `if (n < 5) do ... while` and a one-line body are
  neutral (60).
- The F5F dead test before the case-4 chain (158), or in the B arm, `(u32)D_800E6280.unk_F5F < 0` (129).
- autotweak `--time 300 --score-jobs 2` on the 60-row body: 309 edits, no gain (cost 60 -> 60).

## Controls on the final body

- Without the F5F test: 573/574 words, structure differs (ring offset returns).
- Without the case-1 folded loop: 60 rows, register swap t1/t2 (step 6).
- Without the B-arm loop: the two 0xFF tests vanish (structure).
