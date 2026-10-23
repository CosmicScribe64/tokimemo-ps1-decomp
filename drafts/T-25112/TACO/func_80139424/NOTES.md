# T-25112: TACO func_80139424 (glyph quad), matched

Result: MATCH as default C, 1676 bytes (419 words), `src/ovl/TACO/80139170.c`. Two FAKE markers
(dead early `x3 = 0;`, `s32 pad[2]`) and one FAKE (layout) note for the one-line setUVWH expansions.
Clean rebuild 27/27 sha1 OK.

## Start state

`INCLUDE_ASM`. Best archived body: `drafts/T-24502/TACO/func_80139424/body.c` (ANSI `s32 arg0`,
two x values, chained/`goto` UV stores). In the full unit it measured 282/265 funcdiff rows
(402 words against 419). The T-24600 ledger (`taco-lane/CHECKPOINT.md`, 115/418) was private
under `build/` and is gone.

## What closed it, in causal order (rows = my own instruction diff, full unit)

1. **K&R definition, `u8 arg0;`** (`void func_80139424();` stays in the header). The original keeps
   `andi s1,a0,0xff` at entry; an ANSI `s32` arg0 was homed and reloaded. Prologue then exact.
2. **Frame homes give the variable list.** The caller-save slots 0x58..0x38 are, top-down,
   x0 (ra), y0 (t1), x1 (memory only), y1 (t2), x2 (memory only), y2 (t3), x3 (t5), y3 (t4), w (t0):
   declare `x0, y0, x1, y1, x2, y2, x3, y3` then `w`, with 3 locals above and 4 below (16 slots,
   frame 0x68). Two of the four below are unknown: `s32 pad[2]` (FAKE).
3. **Chain colour stores** `p->b0 = p->g0 = p->r0 = arg3;` give the 6,5,4 order with arg3 in t1.
4. **One full expression per vertex** (as in sibling `func_8013B460`): explicit copies (`x2 = x0`)
   and chains are copy-propagated; four full expressions make uopt keep two vertices in registers
   and the repeated ones as copies, x1/x2 in memory. 382 rows.
5. **Reading the centre directly, no `cx` local** (a local is CSE'd with the sum): 304 rows.
6. **Offset grouped first, `D_800E6280.unk_014[i] + (arg4 - dx)`.** The original computes
   `centre + arg4` twice (t8, t9) with arg4 in a3: uopt CSEs `(c + arg4) -/+ dx` but not
   `c + (arg4 -/+ dx)`, which is re-associated after CSE into the same two sums. 94 rows.
7. **Special glyphs as one comma statement** in setUVWH field order
   (`p->u0 = 0xA6, p->v0 = 0xD3, p->u1 = 0xA6 + w, ...`): the constants 0xD3/0xDF/0xA6 are one
   range each across the three glyph blocks, coloured by first mention; only the macro order
   mentions 0xD3 first and still emits the original's store order. 6 rows.
8. **Letter index assigned in the test**, `if ((letter = arg0 - 'A') < 26)` with `u8 letter`:
   `arg0 - 'A'` becomes a CSE (v0) and the narrowed index its own range (v1), as in the original. 2 rows.
9. **x3 before x0 at equal priority.** regdiff: x0 and x3 both priority 1, coloured by uopt bit;
   statement order x0-first gives the right sum order but x0 the first colour. A dead `x3 = 0;`
   anywhere before the colour stores gives x3 the earlier bit (FAKE). MATCH.

## Negatives (measured, do not repeat)

- `s16` vertex locals (narrowing kept, 441 words); `s16 c` centre local: no change.
- `cx`/`cy` locals with the grouped form: 345 rows (sum CSE'd again).
- Commuted sums (`arg4 + c`), `c - dx + arg4`, `(arg4 - dx) + c` vs `c + (arg4 - dx)`: only the
  last two spellings (equivalent) avoid the CSE.
- Explicit copies or chains for the repeated vertices (`x1 = x3`, `x2 = x0 = ...`): 324 rows.
- All 24 statement orders of the four x expressions: best still 2 rows (sum order vs colour).
- `if (x3) {}` after the x3 statement: 15 rows; dead `x3 = 0;` just before the colour stores or
  just before `dx`: no effect (the early mention must come before the colour stores).
- Constant-pair order permutations (24) and `U` suffixes (16) with separate statements: the
  colour follows mention order, the store order follows statement order; never both.
- Letter compare/index with `& 0xFF` mask forms: an extra ugen temporary, 239-257 rows.

## Tools

Scratch harness (not committed): splice a body into the TU, `objdump`, diff against
`expected/`; regdiff -v needs the traced uopt (copied from the T-25012 scratch build into
`$TOKIMEMO_SCRATCH/r9-uopt/variants/trace`).
