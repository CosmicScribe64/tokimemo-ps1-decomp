# T-25102: TAIIKU func_8013E894 (243 words, 972 bytes)

Result: matched. Default C in `src/ovl/TAIIKU/80138950.c`, commit "T-25102: match TAIIKU:func_8013E894".

## Start state

`INCLUDE_ASM`. Best earlier body: `drafts/T-24521/TAIIKU/func_8013E894/typed-arrays.c`, 224 rows, 245 words
against 243. It already had the right frame, real x/y arrays and the shared loop counter. Its first
structural difference was an extra `move a0,v0` at word 106.

## What closed it, in order

Rows are `funcdiff --resolve --version c` counts on the whole unit.

| Step | Change | Rows |
| --- | --- | --- |
| start | T-24521 body | 224 |
| v4 | draw loop passes `(s16)yp[0]` directly; `first_y` is a variable of the middle loop only | 168 |
| v10 | row offset from the counter: `xp = x + (i << 1)` in plain `for` loops, no `row` copy | 168 (243 words, middle loop structure exact) |
| v11 | `(x[0] + x[2]) >> 1` operand order | 160 |
| v12-v14 | one scalar slot for the removed `row` | 122 (frame homes exact again) |
| v19-v22 | middle and draw loops read `xp[0]`/`xp[1]` directly, no `first_x`/`second_x` locals | 60 |
| v23 | `yp[1] + xp[1] + middle1` operand order | 58 |
| v24 | one more declared scalar before `angle` (angle home 0x78) | 44 |
| m1 | call `func_80086204` (Rot_2D without a prototype, implicit int) | 0, MATCH |
| final | the extra scalar is a real `ball_x` for the shift loop | 0, MATCH; default build MATCH |

## Mechanisms

- The extra `move a0,v0` was not an allocator choice. In `cc -S`, ugen emits `mul $4,$2,2; sll $24,$4,2;
  move $4,$24` for the row offset. as1 turns the `mul` by 2 into a copy and folds the factor into the
  shift (`move a0,v0; sll t4,a0,3`). The target's `or a0,s1,zero` at the loop top is the same copy taken
  from the counter itself. So the original computes `i * 2` from `i`, with no `row` variable.
- `i * 2` in a counted loop gets strength-reduced by uopt into a second induction variable (`s0 += 2`,
  compare against 4). `i << 1` is not strength-reduced, so the counter stays `s1 += 1` and the offset is
  recomputed. func_80139AB0 in this file needed the same spelling.
- In the middle loop, the target keeps `yp[0]` in `$v0` across the stores to `xp[0]`. So `yp[0]` is a
  local (`first_y`). `xp[0]` and `xp[1]` are re-read after their own stores in the source. uopt forwards
  the stored value, and the resulting expression range is what the target has in `$t1`. A `first_x` local
  had priority 220 against 183 for the `xp` range and swapped `$t0`/`$t1`.
- The last 44 rows were one swap: the x-pointer range of the translate, negate and shift loops got `$v0`
  (priority 476.7, coloured second), the camera x and the shifted ball x got `$v1`. The target has the
  reverse. `func_80086204` is an implicit int call, as in `func_8013C4E4` of this file. With it the four
  rotations return int, and uopt treats `$v0` as written in the block where the x pointer starts.
  `$v0` is then not free for that range, and the camera x and ball x get it.

## Negatives (measured)

- `row = i = 0` chain or `i = 0; row = i;`: constant-propagated, no change (224).
- `row = i` at the loop top: uopt strength-reduces (368); `row = i; i++;` before the address: 343.
- Plain `for` loops with `x + i * 2`: strength-reduced, 312.
- Separate draw-loop variables for both x and y: 234. Draw loop with a separate `draw_y` only: 234.
- No `first_y` local at all (`yp[0]` everywhere): uopt reloads `yp[0]` after the `xp[0]` store (169).
- Swapping the translate statements (y first): 242. Pointer-arithmetic spellings `*(x + i)` in the 4-loops:
  no change. A separate counter for the 4-element loops: 121.
- One combined coordinate array (`s32 pt[2][4]` or `s32 pts[8]`): 324.

## Rows now

0. Clean rebuild 27/27 sha1 OK; `check_headers.py`, `migrate_globals.py --check` and
`sync_protos.py --check-branch` (0 new, 15 known) pass.

## Tools used

`tools/regdiff.py -v` with a copy of the existing traced uopt (`/private/tmp/t24047-uopt/variants/trace`)
placed in a scratch `r9-uopt/variants/trace`, and a scratch `cc -S` script following the listing recipe in
[[notes/matching-04]] (preprocess with `-DNON_MATCHING`, drop `INCLUDE_ASM`/`INCLUDE_RODATA`, run
`/opt/ido/5.3/cc -S` with `cc.IDO_CFLAGS` minus `-c` and `cc.ido_frame_env`).
