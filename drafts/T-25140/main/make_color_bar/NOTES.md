# T-25140: main make_color_bar (1,072 bytes)

Result: matched in literal default C (`funcdiff --resolve --version default`: MATCH, 1,072 bytes). No FAKE, no header change.

## Start state

`INCLUDE_ASM`. The best known body was the T-24501 draft (`drafts/T-24501/main/make_color_bar.c`): `Pix2Out` pair stores through a walked pointer `p++`, with separate counters `j = 0` and `i = -1` stepped by 2. Same instruction count, 186 positional rows, resolved edit cost 330.

## First divergence

regdiff on the T-24501 draft: the row pointer `p` is range bit 0 and takes `$v0`. In the target `p` is `$t0`, and `$v0`/`$a1` hold the masked values `j & 0x1F` and `i & 0x1F`. Leaf ranges here are unconstrained, so uopt colours them in bit order, not by priority. A declared pointer variable gets an early bit. For `p` to be coloured after the masked values, it has to be a value that uopt creates late, such as a strength-reduced induction temporary.

## Change

Write every store as an index of the pair counter, `p[k + 16 * row].r1 = k * 2; ... .r0 = k * 2 - 1;`, in a `for (k = 0; k < 16; k++)` loop. uopt strength-reduces `k * 2` (the target's `$v1`), `k * 2 - 1` (the target's `$a2`, which also takes the loop test through linear-function test replacement: `slti at,a2,0x1F`) and the address `p + k * 4` (`$t0`), and then deletes `k`. All three loops and the two alpha loops (`k != 128`, unrolled 4x by IDO) match.

## Measurements (toy translation unit, then the full unit)

| variant | rows | edit cost |
|---|---|---|
| T-24501: walked `p++`, counters `i`/`j` | 186 | 330 |
| indexed `p[k]` with `i`/`j` kept as counters | 247 | 422 |
| `Pix2Out *p` as the parameter itself (no `p = arg0` copy), `k * 2` values | 260 | 468 |
| `void *arg0`, `p = arg0`, `k * 2` and `k * 2 - 1` values | 0 | 0 |

A toy file holding only `common.h`, the `Pix2Out` typedef and the function, compiled with `tools/cc.py ... ido 5.3`, reproduced the full-unit score exactly (186/330 for the T-24501 body) in about 8 seconds. The function is a leaf with no globals, so the toy result carries over. The full-unit `--version c` and `--version default` runs confirmed the match.

## Negatives

- Separate `i`/`j` induction counters (walked or indexed pointer) give `p` an early bit, and with it the wrong colour order.
- A copy-free `Pix2Out *` parameter is far worse (260 rows): the `p = arg0` copy is part of the original.
