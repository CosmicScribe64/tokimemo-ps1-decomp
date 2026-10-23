# T-25112: TACO func_80139FB8 (matched)

Result: matched in plain C, no FAKE, 1,480 bytes (370 instructions). The body is in
`src/ovl/TACO/80139170.c`; `funcdiff --resolve --version default` prints MATCH.

## Start state

`INCLUDE_ASM`. Best earlier body: `drafts/T-24502/TACO/func_80139FB8/body.c` (FAKE zero subtree,
preheader and +i sine shared). Measured here as a NON_MATCHING guard in the full unit: 179
`funcdiff --resolve` rows, 370 words, frame right.

## Measurements, one change at a time

| Body | Rows |
| --- | --- |
| T-24502 body (FAKE, shared preheader/+i sine) | 179 |
| + mirrored-angle result written into the +i scale variable (`var_v0_3` -> `var_v0_2`) | 169 |
| Same, all scale variables merged | 169 (neutral) |
| Clean body (no FAKE, one sine variable per site) + that sharing | 192 |
| + one sine variable for the front, +i and back sites | 163 |
| + `for (i = 1; i < 20; i++)` instead of the m2c `do { } while (i != 20)` | MATCH |

Loop forms on the final body: `for` with `i < 20` or `i != 20`, with an explicit `step += 0x1000`
or `i * 0x1000 / 40`: all MATCH. `do/while` with `i != 20` or `i < 20`: 163.

## What each change fixed (regdiff, instrumented uopt trace)

1. Mirrored half: the original has the input `pos + D` in v1 and the angle result in v0. m2c's
   separate `var_v0_3` (priority 15) loses v0 to the input expression (priority 20). Writing the
   result into the scale variable (priority 23.3 after the merge, coloured first) gives v0 to it.
   Writing it into `d` or `angle` instead costs 10 rows.
2. Sine: one variable for the front, +i and back sites has priority 4.8 (below the radius at 4.889,
   above the constants), so it takes s5 after angle s0, quotient s1, index s2, step s3, radius s4.
   The -i sine stays its own variable (priority 10, s1, shared with the quotient).
3. Constants: in the do/while form the trace gives 3 priority 3.5, 40 3.444, 0x100000 and 96 2.5
   (both in the four scale else blocks) and the table address 2.333 (uses in the four call blocks,
   9 blocks). So 3 takes s6, 40 s7, 0x100000 fp, and the table address falls back to `lui/%lo`.
   The `for` form changes the loop's block layout (IDO's guarded top test, folded) and with it these
   ranges; it gives 40 s6, 3 s7 and the table address fp, as in the original.

## Negatives added

- `(z + 0x100) << 12` for the scale: `addiu; sll`, and 96 moves into a register (different code).
  `(z + 0x100) * 0x1000` is folded by uopt to the same code as `(z << 12) + 0x100000`.
- A local `u8 *tbl = D_8015F4B0`: copy-propagated, no change.
- do/while in any test spelling: 163 rows (constant colours above).

## Lesson for other functions

An m2c `do { } while` of a constant-trip loop can be a `for` loop in the original, and the two give
different constant and table-address colours in IDO (same instructions). Try the `for` form when
the s registers of hoisted constants are permuted. `func_80139424` in this file was not looked at.
