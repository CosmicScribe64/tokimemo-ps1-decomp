# T-25122: main func_8004A1A0 (628 B, 157 words)

Result: no match. `src/main/80049FF0.c` keeps plain `INCLUDE_ASM` (no guard: neither body is under 30 rows).

## Start state

T-25010 draft (`drafts/T-25010/main/func_8004A1A0.c`), whole-unit `funcdiff --resolve --version c`: 89 rows,
157/157 words.

## What changed, and why (each change measured on its own)

1. **First loop: `load_tpage_buf_lock(tp + col + row * 32, ...)`** (it was `tp + row * 32 + col`). In loop 2, the range
   `tp + col` and the index pointer tie at priority 103.3. uopt breaks a tie by bit number, so `tp + col` has to be
   numbered first, and that happens when it appears first as a subexpression of loop 1. Effect: the tp counter in
   `$s3` and the index pointer in `$s4`, the original order of the loop-2 increments, and `addu s0,tp,row<<5`
   operand order. 89 -> 70 rows (with change 2).
2. **Loop 2: `if (n <= 0x40) continue; printf(...)`** in place of `if (n > 0x40) printf(...)`. `w` and `row * 0x78`
   swapped `$s5`/`$s6`. The trace showed `w` with savings 242 over N=21 blocks (D=6, priority 40.33) and
   `row*0x78` with 110/3 = 36.67. The `continue` label adds one uopt block in `w`'s range: N=22, D=7, priority
   34.57, so `row*0x78` is coloured first (`$s5`) and `w` gets `$s6`, as in the original. `if (..) {} else printf`
   does the same. This is natural C, not a FAKE.
3. **Header order `unk_00, x, y, w, h`** (from the original's ugen temporary ring: `t9` = 0x1000000, then `t2` = x,
   `t3` = y, `t4` = w<<7, `t5` = mflo, with `t0`/`t1` held by uopt). y comes right after x. The original's
   `sh t3,6(v0)` in the last delay slot was moved there by as1. Result: 64 rows (`func_8004A1A0.c`). Loops 1 and 2 now
   match register for register. The rest of the diff is the header and the ring numbers that follow from it.

## First divergence left (header colouring)

The original has map `$a0`, 0x80 `$a2`, 0x78 `$t0`, `&D_80122120` `$t1` and 0x1000 `$v1`, and row is also `$v1`.
The build has map `$a2`, 0x80 `$t0`, 0x78 `$t1`, `&D_80122120` `$t2` and 0x1000 `$t3`.
Traced allocator (regdiff `draft.log`): map (priority 7) is coloured before tex (3.33) and cannot take `$a0`
because tex's incoming `$a0` is live in the entry block. 0x1000 cannot take `$v1` because row (`$v1`) is live in
the same block (uopt interference is per basic block). In the original, 0x1000 and row both use `$v1`, so **the
original header is two uopt blocks**. 0x1000 (stores 0x1C/0x1E) sits in block A with the bg def. The map def,
at least one bg store and `row = 0` sit in block B. 0x80, 0x78 and `&D_80122120` span both blocks.
Proof: `func_8004A1A0_boundary.c` puts a late-folded `do {} while ((u32)slot < 0);` (FAKE) before
`bg->unk_20 = 0` and gets every uopt colour of the original (regdiff: map a0, 0x80 a2, 0x78 t0, D_80122120 t1,
0x1000 v1, row v1). The ugen temporary ring then matches too (x t2, y t3, ..., loop-1 tp t3).
Equivalent splitters, same colours: an empty `if (slot) {}` at the same place (vI1), `if ((u32)slot < 0) {}`,
and a call (diagnostic only).

**Why the boundary body is still 159 words:** after any of these boundaries, uopt emits no `unal` ucode for the
map pointer (in `vF` it emits `unal 2; unal 1d` before and after map's `ixa`, and ugen turns that into
`.noalias a2,v0` / `.noalias a2,sp`). Without the `.noalias`, as1 cannot forward the two `lw tX,0x74(sp)` slot
reloads of the cells/index bases past the map stores. That costs 2 extra words. as1 also cannot sink the y store or
hoist the map/cells/index computations as the original does, and the extra block shifts the cells/index spill
homes by 4 (0x54/0x50 against 0x58/0x54).

## Negatives added

- `row = 0x1000` (row as the constant's variable): constant-propagated, no change.
- Bare labels (`L1:;`): no block. `goto L1; L1:`: 2-4 extra words.
- `do { map stores } while (0)`: 161 words. `if (count) {}` / `if (count > 0x40) {}`: 158 words.
- Constant-trip loops over r/g/b or scrollx/scrolly: not unrolled (164/165 words).
- `bg->unk_14` moved after the boundary: `&D_80122120` then takes `$a1` (slot is dead there), 161 words.
- `(u32)D_80122120 + slot * 0x10` / `(u32)&D_80122120[slot]` under the boundary: no `unal` either.
- `map->base`/`map->index` before `cellw` under the boundary: still a reload after an a0 store (158 words).
- A boundary at the top of the function (before bg): `unal` is emitted, but the do-while costs code and slot
  loses `$a1` (162-164 words).
- `s32` return type (header changed temporarily, then reverted), with and without the boundary: no change.

## Next hypothesis

Find the block-splitting construct for which uopt still emits `unal` for the map `ixa` in block B. Facts: `unal` is
emitted in the entry block, in loop bodies (loop-2 induction pointers) and in the block after a do-while at the very
top (vR1), but not in a block after a split when the bg pointer (`$v0`) was defined before the split. That holds
even when bg is no longer used in B (vH1). Read uopt's `unal` emission rule (n64decomp/ido reconstruction,
uoptemit/uoptreg) before more source trials. Then test the construct at the `bg->unk_20` position with
`func_8004A1A0_boundary.c` as the base. The loop part and the ring are already exact.

## Tools (scratch, in `scripts/`; run in Docker with TOKIMEMO_SCRATCH pointing at a copy of these)

- `sbs.py FUNC body.c [-q]`: side-by-side of expected and NON_MATCHING-built words, using a body file.
- `gdump.py FUNC body.c`: ugen binasm in emission order, before as1 (needs `as1cap.py`): ring order, `.alias`/`.noalias`.
- `udump.py FUNC body.c`: uopt output ucode (needs `ugencap.py`): `unal`, labels, block structure.
These patch `cc.ido_frame_env` in-process to add a capture shim. No project tool or build step is changed.
