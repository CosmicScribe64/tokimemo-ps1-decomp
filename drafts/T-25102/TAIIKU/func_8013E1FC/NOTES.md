# T-25102: TAIIKU func_8013E1FC (423 words, 1688 bytes)

Result: matched. Default C in `src/ovl/TAIIKU/80138950.c`, commit "T-25102: match TAIIKU:func_8013E1FC". No FAKE.

## Start state

`INCLUDE_ASM`. Best earlier body: `drafts/T-24521/TAIIKU/func_8013E1FC/typed-arrays.c` (548 diagnostic rows, frame
208 against 200, declared `next_angle`). The new donor was `func_8013E894` of this file, matched earlier in this
package. The rotated phase of E1FC is the E894 loop with the camera angle left out.

## What closed it, in order

Rows are `funcdiff --resolve --version c` counts on the whole unit.

| Step | Change | Rows |
| --- | --- | --- |
| v1 | E894 body for both phases, declared `next_angle` | 399 (frame 224) |
| v2 | rotated phase: `angle * 16` and `(angle + 1) * 16` | 404 (frame 208; loop end now exact) |
| v4 | straight phase: `angle++` at the end of the loop | 304 (angle slot 0x78 and 0x60 exact) |
| v6 | two declared scalars less (11 in all) | 214 (frame 200) |
| v11 | `x[1] = x[0] = -320; x[3] = x[2] = -240;` | 52 |
| v12 | `scale` and `ball_x` back, camera locals out (still 11 scalars) | 48 |
| v13 | `y[2] = y[0]; y[3] = y[1];` before the midpoints | 26 |
| v14 | no `xp`/`yp` row pointers: `x[i << 1]`, `x[(i << 1) + 1]`; `camera_x`/`camera_y` locals back | 12 |
| v18 | both outer loops as `for (angle = 0; angle < 64; angle++)` | 0, MATCH |
| final | comments, long lines wrapped | 0, MATCH; default build MATCH, TAIIKU sha1 OK |

## Mechanisms

- Frame. The target has one counter home at 0x78 for both phases and a second rotated-phase home at 0x60. Both
  are compiler temporaries below the declared locals, not declared variables: with `angle++` at the loop end,
  uopt spills the straight-phase counter into the first temporary slot, and the rotated phase's two
  strength-reduced values `angle * 16` (slot 0x78 again) and `(angle + 1) * 16` (0x60) follow. Linear function
  test replacement gives the `bne t4, 0x410` end. A declared `next_angle` can never reach 0x60 (v1, and T-24521's
  0x74). With the temporaries fixed, the frame fixes the declared count: 11 scalars plus `x[4]` and `y[4]`.
- Straight-phase counter. `angle + 1` is computed once (a1) for `y[1]` and for the increment. With `angle++` at
  the loop end, uopt stores it at the boundary before the first call (the scale-loop preheader, as the target).
  The value loaded for the loop test is then carried into the next head in v0 (`move v0,zero` at entry, no store).
  With the increment written mid-body (v2), the head reloads the home and the entry stores zero.
- Constants. Chained stores keep -320 and -240 in a0/a3 for two stores each; `x[1] = x[0]` order gives the
  target's register numbering (`x[0] = x[1]` was 78 rows).
- y copies first. ugen hands out temporaries from a FIFO list. The target's free-list order fits the y copies
  emitted before the two midpoint sums (t9, t2 for the copies, then t3, t4, t5, t7).
- Row pointers. With `xp`/`yp` locals read by the straight-phase draw loop, uopt drops the `.noalias $16,$3` mark
  between the y and x walks of the rotated-phase translate loop (cc -S listing), and as1 keeps the y load after
  the x store. In a bisect, removing that draw loop, passing constants to its call, or plain indexing each
  bring the mark back. E894 has one phase and matches with pointers; E1FC needs indexing.
- `for` line. `angle = 0;` on its own line puts `move v0,zero` before the hoisted loop invariants (as1 groups by
  source line). The `for` line holds both the init and the invariants, and as1 then places the move last.
- `func_80086204` (Rot_2D without a prototype, implicit int) as in E894: the void `Rot_2D` gives 102 rows.

## Negatives (measured)

- `angle += 16` with `Rot_2D(angle + 16)`: uopt CSEs the sum with the increment, 329 rows.
- `y[1] = ++angle << 3` at the top: 404 rows.
- Dropping `ball_x` and `first_y` instead of `xp`/`yp`: 392 rows. Dropping the camera locals with indexing: frame
  192, 126 rows. Indexing only in the straight-phase draw loop, pointers elsewhere: frame 208, 338 rows.
- `scale` folded into the product: `multu v0,t7` operand order (+4 rows).

## Rows now

0. Clean rebuild 27/27 sha1 OK; `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch`
pass.

## Tools used

`funcdiff --resolve`, `regdiff.py` with a copy of the traced uopt (`/private/tmp/t24047-uopt/variants/trace`) in a
scratch `r9-uopt/variants/trace`, a scratch `cc -S` listing (recipe in [[notes/matching-04]]) to read the
`.noalias` marks, and a register-masked opcode diff to separate structure from allocation.
