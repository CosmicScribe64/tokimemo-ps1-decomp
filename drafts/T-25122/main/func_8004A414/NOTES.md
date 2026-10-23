# T-25122: main func_8004A414 (257 words, 1,024 bytes)

Result: **matched**. The default build compiles the C in `src/main/80049FF0.c`, and `funcdiff --resolve --version default` prints MATCH (1024 bytes). Commit `T-25122: match main:func_8004A414`.

## Start state

`INCLUDE_ASM`, no guard. The best known body was the T-24501 `for_loops` source (`drafts/T-24501/main/func_8004A414.c`). Whole-unit `funcdiff --resolve --version c` gave 142 diff lines (71 rows). The frame (0x98), the homes and the instruction count were already right. The function draws a disc of textured `POLY_GT4` quads, one per grid cell inside the radius.

## What was left, in the target's terms

1. Saved-register colours. The original has y-narrow `s3`, shade 0x80 `s4`, x-narrow `s5`, (s16) width `s6`, (s16) height `s7` and the `GameState` base `fp`. The draft had shade `s3`, x `s4`, y `s5`, base `s6`, width `s7` and height `fp`. The T-24501 trace priorities were shade 240, x/y 200, base 180 and dims 150.
2. Constant registers. The original has 0x60 `a0`, 0xC0 `v0`, 0x50 `v1` and 0xD0 `t0`, with GetTPage's `a0 = 0` in the jal delay slot. The draft had 0x60 `v0`, and its `move a0,zero` was hoisted early.
3. The association of the clut arguments after the two `rand()` calls.

## What closed it

- The constants are exactly `setUVWH(poly, 0xC0, 0x50, 0x10, 0x10)` (u1 = u3 = 0xD0, v2 = v3 = 0x60), and the coordinates are exactly `setXYWH(poly, X - w/2, Y - h/2, (s16)w, (s16)h)`. Writing the PsyQ macro expansions as one comma statement per macro (setXYWH, setUVWH, setRGB0..3), in the macros' own field order, took the diff from 142 to 16 lines. That fixed every saved-register colour, the constant registers and the store order. The statements can be split across lines; it still matches. Marked FAKE (hand expansion of the SDK macros, as elsewhere in the repo).
- The clut: with the `random_x` local and the separate `rand()` statement removed, the two nested `rand()` calls compile the same as before (frame unchanged), so the local was not needed. uopt reorders the operands of these sums, so the source order matters: `g->unk_014[i] + (s16)x + (rand() & 8)` and `(s16)y + g->unk_018[i] + (rand() & 8)` match. Measured: 16 -> 4 lines (y argument) -> 0 lines (x argument).

## Negatives measured here (do not repeat)

- Direct `D_800E6280.unk_014[...]` instead of the `g` pointer on the old body: 349 lines (frame and structure change).
- Nested `rand()` with the unused `random_x` still declared: frame +8 (106 lines).
- Clut y forms `g->unk_018[i] + (rand() & 8) + (s16)y` (10 lines), `(rand() & 8) + g->unk_018[i] + (s16)y` (12), `g->unk_018[i] + (s16)y + (rand() & 8)` (10). Clut x forms `(s16)x + g->unk_014[i] + (rand() & 8)` (6) and `g->unk_014[i] + ((rand() & 8) + (s16)x)` (6). `(rand() & 8) + g->unk_014[i] + (s16)x` and `g->unk_014[i] + (rand() & 8) + (s16)x` leave the 4-line A/L operand swap.

## Method note

The trace uopt (`/scratch/r9-uopt/variants/trace`) was not available on this host. A scratch script, `build/scratch/t25122/ccs.py` (not committed), ran IDO `cc -S` through the cc.py shim environment to read the pre-as1 output. It showed that uopt, not as1, chose the constant colours and the clut operand order.
