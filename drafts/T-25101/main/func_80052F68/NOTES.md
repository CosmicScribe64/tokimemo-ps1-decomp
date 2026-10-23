# T-25101 main func_80052F68: 69 rows, not matched

Result: no match. Best body `body.c` here: `funcdiff --resolve` 69 rows ("compared NON_MATCHING C"), 300 words against 301. Over the 30-row guard limit, so `src/main/8004F870.c` keeps the plain `INCLUDE_ASM` and `include/main_api.h` is unchanged.

## Start state

No guard in the tree. Archived drafts (T-14500, T-16000, T-16100, T-17000) are m2c pointer walks at 604 rows. T-12050 reported 327 rows from a scratch body that was not kept: "target peels two then unrolls four, draft rolls".

## What the function is

It builds a per-girl score for girls 1..10 into `arg1[1..10]` through a stack `s16 val[11]` at sp+0x48:

1. `val[k] = arg0[k] * 4; val[k] += g->unk_1BC[k].unk_0A * W[k];` IDO unrolls this loop: two peeled trips, then one group of four.
2. Stress `g->unk_11C.unk_02` is clamped to 140, and `val[2] += D_800B437C[s / 10]`.
3. For each girl, bits 7..1 of two masks `L[i]`, `H[i]` select the stats `(&g->unk_0FC)[7 - m].unk_02`. A stat is divided by 10, or by 20 on the dead `m == 7` arm, then capped at 14. The `L` arm adds `t + t / 2` of `D_800B436C[idx]`; the `H` arm adds `t`.
4. `sum = bytes 1..35 of g->unk_1BC[m].unk_14`. If `sum >= 4`, add `sum * 10`, else set the score to 0.
5. Copy to `arg1`. This loop is unrolled the same way.

Tables: `W` = s8[12] at 0x800B4348, `L` = u8[12] at 0x800B4354, `H` = u8[12] at 0x800B4360. The target's loops walk them from entry 1 with displacement 0 (`la a3,0x800B434B` then `lb 0(a3)`). That needs base symbols at those three addresses. `D_800B4349[k - 1]` puts -1 in the displacement, and `(D_800B4349 - 1)[k]` or `((s8 *)D_800B4341)[k + 7]` stop strength reduction. Splat has no label at the bases, so a default-C match also needs `D_800B4348`, `D_800B4354` and `D_800B4360` in `config/symbol_addrs_main.txt` (wiki/data-types.md, "Linking"). The draft declares them in place of the unused `D_800B4349[11]`, `D_800B4355[10]` and `D_800B4361[10]`; check_headers accepts that.

## Measured steps (funcdiff --resolve rows)

| Change | Rows |
| --- | ---: |
| natural rewrite, one `i`/`j` for every loop | 354 |
| own counter for loop 1 (a counter assigned again later keeps a loop rolled) | 300 |
| frame: 8 scalars before `val`, 8 bytes below it (frame 0x80, val 0x48) | 295 |
| base symbols `W[k]`/`L[i]`/`H[i]`; loop-2 outer counter used only there (gives `bne` against 11); loop-2 inner counter shared with loop 3 outer and loop 4 (gives `slti 7`); stress in its own variable | 239 → 235 |
| autotweak: `GameState *g` for every access, `t` inlined, one extra local in the `H` arm | 95 → 91 |
| `pv = val; pv[k] = arg0[k] * 4; val[k] += ...` (FAKE) | loop 1 exact |
| stress and the loop-3 sum share one variable; the `L` and `H` arms use separate variables | 69 |

## First divergence left

Word 0x0c, `addiu v1,a0,3`, against `lui a3` in the draft. This is the as1 schedule of B0, a consequence of the next point. The real difference is the register of the constant 10. The target has `li a0,10` at the end of loop 1's preheader and uses a0 for every `/ 10` and for `* 10`. Its loop counters are j=v0 and i=v1, and the stress is in v0. The draft gives 10 to v1 and the outer counter to a0, and the loop-3 inner counter follows (a0 against v1). The `li` placement is an effect, not the cause. The ugen listing (`cc -S`) of the draft has `li $3,10` at the head of the block after the clamp. as1's global filler moves it into the stress load-delay slot. With 10 in a0, which is dead after the loop-1 IV setup, the same filler can place it in B0 instead. So what is left is the priority or colouring order of the constant-10 range against the loop-2 counters. A second effect: with separate variables in the two arms, the `H`-arm value takes t0. The target has t1, the same register as the `L` arm.

## Negatives (do not repeat)

- Loop-1 bound spellings: `<= 10`, `!= 11`, `11 > k`, `++k`, `10U`/`(u32)` casts, do-while and while forms all give the same code. `k - 1 < 10`, `10 - k >= 0`, `k <= n` with `n = 10`, `while (k++ < 10)`, a 0-based loop with `[k + 1]`, a derived `j = k + 1`, dual counters `k`/`n`, and offset bases `(arg0 + 1)[k]` either keep a counter or put constants into displacements.
- A variable or a reused dead `arg0` holding 10 is constant-propagated: no change.
- s16/u8/u16 loop-1 counters stop the unroll.
- Loop-1 temporaries (`t = arg0[k] * 4; val[k] = t;`, `t = W[k]`): 402-414 rows.
- `val[k] = val[k] + R * W` picks the right LFTR pointer but swaps the multiply operands; `+=` keeps `R * W`.
- `like`/`dislike` locals for `L[i]`/`H[i]`: 247 rows. A `7 - m` variable: 310 rows. A `t` variable for `D_800B436C[v]`: 133-167 rows.
- Stress spellings (ternary, `x /= 10`, `>`, assignment in the test): 91 at best.
- The declaration order of the seven counters and values: 42 orders, all 69 rows.
- autotweak (300 s, `--score-jobs 2`) from the 91- and 69-row bodies: no further edit.

## Next hypothesis

Get the uopt allocation trace (regdiff needs the traced uopt, wiki/uopt-register-allocation.md; not built in this environment). Compare the constant-10 range's priority, edge count and constrained flag with the outer and inner loop-2 counters. In the target, 10 must be coloured after both counters: either the counters are constrained with a higher priority, or the 10 range spans more blocks. The trace shows which. Then look for the source shape that gives the `L` and `H` arms one variable (t1 in both) while keeping that order. A likely candidate is one more live range in the inner loop that the original had. Separate arm variables and autotweak's extra local each moved 10 down one register, which supports this.
