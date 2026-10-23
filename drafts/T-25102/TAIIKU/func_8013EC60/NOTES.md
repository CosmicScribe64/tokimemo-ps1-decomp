# T-25102: TAIIKU func_8013EC60 (448 words, 1788 bytes)

Result: matched. Default C in `src/ovl/TAIIKU/80138950.c`, commit "T-25102: match TAIIKU:func_8013EC60". No FAKE.

## Start state

`INCLUDE_ASM`. Earlier drafts were far off: the T-14500/T-16100/T-16550 m2c bodies scored 784 rows (frame 464), the
T-24521 `typed-arrays.c` body 489 rows (frame 216). The donors were `func_8013E1FC` and `func_8013E894` of this file,
both matched earlier in this package. EC60 is the same two-phase strip drawer:

- straight phase: corners `x = {320, 320, 240, 240}`, `y = {a << 3, (a + 1) << 3, a << 3, (a + 1) << 3}`, each
  corner turned by the camera angle (`func_80086204(camera.angle, &input_x, &input_y, &x[i], &y[i])` with
  `input_x = x[i]; input_y = y[i];`), then the E1FC tail with the camera and scale loops over all four corners;
- between the phases `((TaiikuActor44 *)D_8011ECD0)[102].unk_26 = x[3] + 8;` (`sh` to `D_8012080E`);
- rotated phase: the E894 body with inputs `(0, -320)` and `(0, -240)` and 64 steps of 16.

## What closed it, in order

Rows are `funcdiff --resolve --version c` counts on the whole unit.

| Step | Change | Rows |
| --- | --- | --- |
| v1 | E1FC straight phase as above, E894 rotated phase (`angle = 0; do { ... angle += 16; } while (angle != 0x400);`), E1FC's 11 scalars | 301 (frame 200) |
| v2 | E894's 13 scalars (`xp`, `yp`), pointers in one or both phases | 615 / 381 (frame 208) |
| v4 | rotated phase as `for (angle = 0; angle < 64; angle++)` with `angle * 16` and `angle * 16 + 16` | 40 |
| v6 | `y[0] = angle << 3; y[1] = (angle + 1) << 3; y[2] = y[0]; y[3] = y[1];` instead of chained stores | 0, MATCH |

## Mechanisms

- Rotated-phase counter. With the do-while, `angle` is a variable live across the four calls and takes `$s6` for the
  whole function; the target keeps it in a stack slot (0x78) and gives `$s6` the constant 2 and `$s7`/`$fp` the
  hoisted loop ends. With `angle * 16` uopt strength-reduces the counter into one spilled value at the slot of the
  straight-phase counter, and linear function test replacement gives the `addiu v0,v0,16; bne v0,0x400` end. The
  second corner is `angle * 16 + 16` (`addu; addiu a0,a0,16` in the target), not `(angle + 1) * 16`, which in E1FC
  is a second strength-reduced value with its own slot (0x60).
- Counter slot and colour. At v4 the only rows left were the straight-phase counter in `$a0` instead of `$v0` (its
  two shifted copies one register lower) and the counter spill at 0x74 instead of 0x78. A bisect by removal (scratch
  only) showed that the chained `y[2] = y[0] = angle << 3; y[3] = y[1] = (angle + 1) << 3;` reserve one more spill
  slot: without the y[2]/y[3] stores the counter goes back to 0x78. Writing the copies as their own statements keeps
  the same stores (uopt forwards the values) without the hidden chain temporaries, and the counter colour follows.
  The constant chains `x[1] = x[0] = 320;` stay as in E1FC.

## Negatives (measured)

- Chained y stores in any order (`y[2] = y[0]`, `y[0] = y[2]`): 40 rows (slot 0x74, counter in `$a0`).
- y[2]/y[3] recomputed (`y[2] = angle << 3;`): 26 rows. Copies the other way (`y[2] = ...; y[0] = y[2];`): 8 rows.
- A separate counter variable for the rotated phase: frame 208, 155 rows.
- Removing the actor store or the rotation loop (diagnostic only) does not move the slot.

## Rows now

0. Clean rebuild 27/27 sha1 OK; `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch`
pass.

## Tools used

`funcdiff --resolve`, `regdiff.py -v` with a copy of the traced uopt in a scratch `r9-uopt/variants/trace` (the
range table showed `angle` at priority 21 under the two shift ranges at 30), and a scratch side-by-side script over
funcdiff's resolved rows with registers masked.
