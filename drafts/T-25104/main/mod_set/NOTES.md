# T-25104: main mod_set (195 words, 780 B)

Matched. `src/main/8004C3F0.c` now compiles `mod_set` as plain C in the default build
(`funcdiff --resolve --version default`: MATCH, 780 bytes; all 27 SHA1 checks pass).

## Start state

`INCLUDE_ASM`. Old drafts: T-24006 typed m2c body (aligned cost 295, 198 against 195 words),
T-24301 byte-pointer draft (about 220 rows), T-24600 typed scratch recovery (278/197 words, clut and
the record pointer in the wrong registers). None was reused: a fresh body built from the target and
the `mod_set_gpu` donor shape was 195 words on the first compile (116 raw rows, mostly stack offsets).

## What closed, one cause each

1. **Donor loop shape** (`mod_set_gpu`): `u8 rows/cols/w`, `data = rows * frame * cols + p->unk_04`,
   do-while loops with `cols = p->unk_0C` reloaded at the latch and `rows = p->unk_0D` after the
   inner loop. Gives the right instruction count at once.
2. **Tile-size switch.** `switch (w) { case 8: case 16: default: }` gives the target block order
   (default arm, case 8, case 16). The m2c nested `if (w != 8)` puts case 16 before case 8; an
   `if / else if` chain inverts the branches.
3. **x and y positions as expressions.** `sp.unk_04 = x + col * w` (strength-reduced by uopt into
   the `s1 += w` induction) and `sp.unk_06 = row * w + y` (hoisted). `row` must be `s32`: with
   `u32 row` uopt orders the product `w, row` (`multu s4,a2`); `y + row * w` gets `multu a2,s4` but
   loads y first (t7/t8 swapped).
4. **h is an expression, not a local.** `sp.unk_0A = p->unk_0E[1]` before the 0x1000 constants
   puts its load in `t0`; a `u8 h` local takes `a3` (uopt), not `t0`.
5. **Priority conversion.** `s32 pri` and a u16 prototype `void func_80098B80(Rec24 *, void *, u16)`
   (GsSortSprite): uopt keeps the `cvtl(pri)` range in s7 (`lhu s7` in the outer loop) and the call
   keeps its own `andi a2,s7,0xffff`. `u16 pri` folds the conversion into `lhu a2` at the call and
   pri loses its register to cx; an explicit `& 0xFFFF` is hoisted and spends one ring slot (t9).
   The prototype is in `main_api.h`; `func_80043510` keeps the implicit view (override).
6. **Frame.** Declaration order `padA[4], row, sp, p, col, data, word, uv, clut, bright, rows, cols,
   w, padB[7], mode, padC[1]` gives frame 0xE8, row 0xD4, sp 0xB0, mode 0x70. FAKE pads.
7. **Switch arms on one line each.** Cases 3-5 store mode before the attribute only when the two
   assignments share a source line (as1 per-line scheduling). FAKE layout.
8. **Mask and shift in one statement** (`clut = (*data & 0xFF00) >> 8`, same for uv and word). This
   was the last 10 rows: with the split donor form (`clut = *data & 0xFF00; ... clut >>= 8;`) clut
   has 2 uses + 2 defs (uopt save 400 over 3 units, priority 133) and outranks the x position
   (103) and col (78), so x/col/clut sit in s2/s3/s1. The one-statement form has 1 def + 1 use
   (priority 67, between col and w at 64) and ugen still computes the mask into the variable's
   register (`andi s3,v0,0xff00; srl t1,s3,8; move s3,t1`), so the code is identical except for the
   colours. uv and word take the same form; only clut changes colour.

## Tools used

The uopt trace binary (wiki/uopt-register-allocation.md) was rebuilt in a scratch dir with
`tools/docker.sh` and `regdiff -v`; its priorities (save / units from `compute_save` in
n64decomp/ido `uoptreg2.c`) pointed straight at clut. decomp-permuter is not useful here: its
pretty printer splits the one-line switch arms.

## Negatives (measured)

- record fields through `((ModRec14 *)D_801217A0)[idx]` instead of `p`: same allocation.
- `col = 0` inside `if (cols != 0)`: as1 no longer fills the `beqz` delay slot (196 words).
- an explicit `xpos` local (inside or before the `if`): 103 or 78, no change in order.
- `u16 clut` (71 rows), `s32 clut` (no change), `u32 w` / `s32 w` (other rows change).
- empty `if (col) {}` tests raise col but add units to the x range; not needed.

## For the siblings (not started here)

`mod_set_gpu` (17-row guard) has the same symptom ("clut still outranks the x counter and data")
with the split mask/shift form; try `uv = (*data & 0xFF0000) >> 16;` etc. there first.
`mod_trans_vram` (47 rows, raw tile word in the wrong register) may also react to the
one-statement form.
