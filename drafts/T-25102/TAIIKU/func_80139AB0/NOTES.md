# T-25102: TAIIKU func_80139AB0 (204 words, 812 bytes)

Result: matched. Default C in `src/ovl/TAIIKU/80138950.c`, commit "T-25102: match TAIIKU:func_80139AB0".

## Start state

`INCLUDE_ASM`. Earlier bodies: T-16550 m2c redraft (237 rows), T-16552 index form (99 lines, outer loop
strength-reduced), T-24521 explicit/narrow pair loops (228 and 194 rows). All used scalar or offset views.

## What closed it, in order

| Step | Change | Rows |
| --- | --- | --- |
| v1 | clean body, grid as `(j - 2) * (i * 4 + 80)` | 224 (inner loop fully unrolled, products folded) |
| v2 | `(i * 4 + 80) * j - (i * 8 + 160)` | 242 (inner loop unrolled by 2, but the outer loop and the even half strength-reduced) |
| v3 | the same with `u32` counters | 221 (unsigned counters do not block strength reduction) |
| v4 | `(j - 2) * ((i << 2) + 80)` with shifts | 167 (outer loop no longer strength-reduced; inner still fully unrolled) |
| v5 | shifts, `w * j - ((i << 3) + 160)` | 270 (unrolled by 2 with `multu`, but the constant is added inside the loop) |
| v6 | shifts, invariant first: `-160 - (i << 3) + ((i << 2) + 80) * j` | 9 |
| v11 | one more declared scalar (`pad`) | 5 |
| final | `D_801499B0` defined in C (`TaiikuPair D_801499B0[1]`) instead of `INCLUDE_RODATA` | MATCH |

## Mechanisms

1. **Shifts are not induction expressions.** uopt strength-reduces `i * 4`, `i * 8` and the linear sums of `i`
   into induction registers; `i << 2` and `i << 3` are recomputed from `i` every trip, as in the original
   (`sll s4,v0,2` at the loop top, filled into the `bne` delay slot).
2. **Body size decides the unroll factor.** `(j - 2) * w` is small enough for a full unroll of the 4-trip loop;
   the longer `invariant + w * j` body is unrolled by 2 and keeps `multu w, j` and `multu w, j + 1`.
3. **Invariant first.** `-160 - (i << 3) + w * j` lets uopt hoist the invariant left operand
   (`li t9,-160; subu t5,t9,a0`), so the loop body is a single `addu` per store.
4. **Counter reuse.** One `i` for all four loops: a fresh counter for the 80-trip loop or the small loops
   makes uopt unroll them (by 4, or fully).
5. **Spill slot.** With `i, j` the spill of `i + 1` lands at sp+0x54; any third declared scalar moves it to
   sp+0x50 with the same 0x60 frame. An argument local (`x = D_800E6280.unk_10F4; func_800AE120(x);`) also
   matches; the unused `pad` is used and marked FAKE because neither is known.
6. **as1 `lui $at` sharing.** as1 reuses one `lui $at` for stores to different offsets only when the symbol is
   defined in the compiled C, and only within one 16-byte-aligned chunk of the section. Observed groups in
   this function: D_80149968 offsets {0x1C,0x1E} {0x14} {0x1A,0x18} {0x00,0x04} {0x0C,0x10} (section offsets
   0x28..0x47); D_801499A0 all six; D_80149958 three; D_80122DD0 (extern) never. `D_801499B0` was an
   `INCLUDE_RODATA` piece, so the C saw an undefined extern and gave its two `sh` two `lui`s, and the
   scheduler then moved the first store up. Defining it in C closes the last 5 rows. The whole tree still
   builds with all 27 sha1 OK.

## Negatives

- Line grouping of the two `D_801499B0` stores and the call, a chained store, the volatile `D_801499B2` for
  the second store, the stores swapped: 5 or 7 rows (the cause is the extern symbol, not the schedule).
- A real index local `k = (i << 2) + j` in the inner loop: 193 rows. `k = i << 2` in the outer loop: 75 rows.
- A separate grid counter: frame 0x78 and the 80-trip loop unrolled by 4.
