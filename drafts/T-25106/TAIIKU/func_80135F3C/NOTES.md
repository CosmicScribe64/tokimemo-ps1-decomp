# T-25106: TAIIKU func_80135F3C (matched)

Result: matched, 419 words / 1,676 bytes, default build C (no guard). Source:
`src/ovl/TAIIKU/80133C80.c`, commit `T-25106: match TAIIKU:func_80135F3C`. The commit also adds
`void func_80135F3C(void);` to `include/ovl/TAIIKU.h` for the earlier caller (case 6 of the
scene switch).

## Start state

Production was `INCLUDE_ASM`. The newest archived body, `drafts/T-24521/TAIIKU/func_80135F3C/typed.c`,
was recorded at 543 rows and is missing the first-selection store. I wrote a fresh body from the asm,
using the same four table copies (`TaiWords11`, two `TaiikuS11`, `TaiFlags58`). Measured in the full
unit with `funcdiff --resolve --version c`: 510 diff lines, 413 of 419 words.

## Steps (one lever each)

| step | change | funcdiff lines | words |
|---|---|---|---|
| 0 | fresh body, `DISP(46 + x)` records, `code` local, `for (j = i; j >= 0; j--)` | 510 | 413 |
| 1 | inner loop `for (j = 1; j <= i + 1; j++)` over runner `i + 1 - j` | 400 | 423 |
| 2 | records indexed from 46 (`(&DISP(46))[x]`), two final loops | 289 | 418 |
| 3 | duplicate test as `while (rec(cpu[i]) == rec(R[i+1-j]) \|\| rec(cpu[i]) == rec(D_801491D8))`, no `code` local in the nested loop | 56 | 419 |
| 4 | no `code` local anywhere (flag lookups read the record twice) | 12 | 419 |
| 5 | unused third scalar `pad` (FAKE) | 0, MATCH | 419 |

Why each step works:

1. The original guards the inner loop with `blez (i + 1)` and keeps a byte counter from 36 up to
   `(i + 1) * 36` (`slt; beqz`). `j = i; j >= 0` gives `bltz`. `j = 0; j < i + 1` gives a `bne`
   exit (uopt turns the `<` into `!=`). `j = 1; j <= i + 1` gives the original's guard, counter and
   `slt` exit. `i + 1` is shared with the outer `i++`, so `s7` holds `i + 1` as in the original.
2. Every record product in the original is `multu` with the shared 0x44 constant (`s3`). Indexing
   from record 46 keeps the u8 index unsigned (as in the matched `func_80137C44`). In the same step
   the final `>> 3` loop and the flag lookups became two separate loops. As one loop with
   explicit lookups after it, IDO unrolled the `>> 3` loop. As two loops, the first stays rolled,
   with the original's record-pointer exit against `D_8011EDE0`, and the second unrolls. The same
   step also brought back the original's four table addresses in `s6/s4/s7/fp` (before it,
   `&rival_codes` and `&flags` were folded into `sp` offsets). I did not test which of the two
   changes did that.
3. In the original the first record value is loaded after the inner-loop guard (`blez` first), and
   that one product uses shifts. That pattern is uopt PRE hoisting the record read out of the inner
   loop. A `code` local assigned before the loop puts the load before the guard. With the tests
   written on the records directly, PRE makes the original's `v1` value, and all instructions line
   up.
4. The `code` local merged the then-branch value and the four final values into one variable
   (`v1`). As separate CSE expressions they take `a0` (then-branch, with the record address in
   `v1`) and `v0, v1, a0, a1` for the four lookups. The promoted player byte then moves from `a1`
   to `a2`.
5. With `code` gone the tables sat 4 bytes high (`sp+244` against `sp+240`): the original has 12
   bytes of scalar slots above the copies. An unused `s32 pad;` after `i`, `j` restores them. FAKE.

## Negatives measured here

- `for (j = 0; j < i + 1; j++)` with `R[i - j]` or `cpu[i - j - 1]`: 439 lines (`bne` exit).
- A third scalar used as the cpu-loop counter (`k`): 85 lines; used for the final loops: 86 lines.
  The slot has to be unused.
- One final loop plus four explicit lookups: the `>> 3` loop is unrolled completely.

## Reproduce

`tools/docker.sh python3 tools/funcdiff.py func_80135F3C --unit TAIIKU --resolve --version default`
prints MATCH.
