# T-25136: TACO func_80145650 (matched)

Result: MATCH, 1908 bytes (477 words), default build compiles the C in `src/ovl/TACO/801447D0.c`; clean rebuild 27/27 sha1 OK.

## Start state

Production was plain `INCLUDE_ASM`. The newest draft (`drafts/T-24502/TACO/func_80145650/body.c`) measured 200 rows in the full unit (`funcdiff --resolve --version c`). First divergence: the second 32-byte matrix copy's transfer register, `$t8` where the original has `$t5` (source `$t3` in both).

## What was measured

A scratch single-function harness (`build/scratch/f650/`, removed by the clean rebuild) compiled the body with `tools/cc.py` and an extra `ugen` shim that logs the ucode ops `tools/ugen_pass.py` uses for its state (`str`/`istr`/`fjp`/`mov`). It can also force the B/F state per copy.

1. Copies 2-4 are in state F in the original (transfer = source + 2 in the ring); copy 1 is in state B. In the draft, all four are B, because the last memory op before each `Umov` is the direct stack store `input.vz = 0`. Forcing BFFF dropped the draft to 92 rows, with the first divergence moving to the tail (row 224). So the cause is confirmed.
2. The tail: `q + arg0[2]->x0` evaluates the field first. Writing `arg0[2]->x0 + q` (and likewise for x1, y2, y3) makes the compiler evaluate the call expression first, as the original does. With BFFF forced, that left only the frame: 0x80 instead of 0x88.
3. `s32 x, y` in place of `s16` restores the 0x88 frame. With BFFF forced, that gave 0 rows.
4. The F state without forcing: `tools/ugen_pass.py` gives F after an `istr` immediately before the copy, or for every later copy once a signed `<` jump against a positive constant comes after an `istr`. Stores through `SVECTOR *v = &input` become direct `str` in uopt (no change). Moving the copy before the input stores changes the code (as1 does not reorder them). A vertex loop is not unrolled (396 words). So an empty `if (... > 0) {}` after the vertex-0 stores (the first `istr`s) sets the flag and emits no code.
5. Colouring: `if (x > 0) {}` alone gives x, angle, y in s2..s4 (target x, y, angle); `if (y > 0) {}` gives y, angle, x. A test that reads both (`x + y`, `x | y`, `x > 0 && y > 0`, two ifs) gives 0 rows. Kept `if (x + y > 0) {}` (FAKE, one jump).

The m2c casts (`(s16)`, `(s32)`) were removed; POLY_G4 matches the caller's `POLY_G4 *polys[3]`. The return type stays `s32` (the caller sees an implicit int declaration, so `void` is a redeclaration error).

## Negatives added

- Pointer stores to the input (`SVECTOR *v = &input`): uopt folds them to direct `str`, state stays B.
- Copy before the input stores: as1 keeps the order, 407 rows.
- `for (i = 1; i < 4; i++)` vertex loop: not unrolled.
- An empty test reading only one of x, y or angle/arg2/arg1: wrong saved-register order (22-470 rows).

## Left

Nothing for this function. The FAKE stands for an original-compiler block-copy state that the current `ugen_pass.py` model only gives after an indirect store or a signed positive-bound jump.
