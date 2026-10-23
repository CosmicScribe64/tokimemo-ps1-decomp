# T-25101 main sndi: matched

Result: `sndi` (main, `src/main/8004F870.c`) matches all 1052 bytes as default C; the main SHA1 and the clean 27/27 rebuild pass.

## Start state

No guard in the tree. The best draft was `drafts/T-25027/main/sndi/body.c`: 123 funcdiff rows, 264 words against 263. It used while loops with `i++` in the loop header, a `u8 parity` local and integer-cast cursors `(u8 *)((u32)buf + (u32)i)`, all marked FAKE so that uopt would not strength-reduce the cursor.

## What the target shows

- Each scan has two different base registers. The cursor `v1 = a3 + v0` uses the buffer address (a3), which is also the strcpy argument and is rebuilt after calls. The loop test `t2 = a1 + v0` uses a copy, `move a1,a3`, made in the preheader. Neither address is strength-reduced.
- `i++`, `andi t9,v0,1` and the copied first instructions of the header and the latch are all moves by as1's global filler. The C has a plain `for (...; i++)` with the parity test as an expression.
- The `i = 0` in the found arm (just before the last strcat, spilled across it) and on the exhausted exit is the second loop's `for (i = 0; ...)` init. uopt moves it there. The drafts' explicit resets are not needed.

## Measured steps (sbs rows = differing aligned rows of `.scratch/sbs.py`; 12 = relocation naming only)

| Variant | Words | sbs rows |
| --- | ---: | ---: |
| body.c (T-25027 best) | 264 | 68 (funcdiff 123) |
| N1: natural `for`, `X[i]` and `&X[i + 1]` | 253 | 174 (cursor strength-reduced) |
| F3: `for`, `p = &X[i]` | 253 | 205 (strength-reduced) |
| F1: `for`, `p = X + i`, test `X[i]` | 260 | 64 (no SR; frame 4 off) |
| F5: F1 plus one scalar declared before the arrays | 260 | 22 (only the 3 latch copies missing) |
| G1: `s = X;` then `for (i = 0; s[i] != 0; i++)` | 263 | 20 (preheader order: move before beqz) |
| H1/H3: `for (i = 0, s = X; s[i] != 0; i++)`, `p = X + i` or `p = s + i` | 263 | 12, funcdiff MATCH |

Final source: H3 (`p = s + i`, every scan through `s`), with parameters named as in `sndisp` (`str, y, col`).

## Negatives added

- `&X[i]` or `X[i]` used as the cursor in the loop header: uopt strength-reduces it into a walked pointer (253 words).
- `p = X + i` with the test `X[i]`: no strength reduction, but the test shares the base register (3 words short).
- `for (s = X, i = 0; ...)` and `s = X;` before the loop: `move a1,a3` is scheduled before the `beqz`. With `i = 0` first, the move lands in the delay slot.
- Explicit `i = 0` in the found arm combined with a reset at the join (T-25027 `join-reset.c`): uopt drops the arm's store, 74 sbs rows.

## FAKE left

The buffer sizes 40/40/40/20 are inferred from the frame offsets; the comment names T-25101. The scalar `s` declared before the arrays sets the frame layout and is a real local.
