# T-24923 OPTION:func_801326BC (src/ovl/OPTION/80132000.c)

Status: **not matched**. `INCLUDE_ASM` restored. Best draft rows: 134 (pre-flight m2c)
to 149 (the new bodies); all far over the ~30-row guard limit.

## What the target does (from `asm/ovl/OPTION/nonmatching/OPTION/80132000/func_801326BC.s`)

- Early-out on `D_8013D300 == 0`.
- Three byte stores of `D_800E6280.unk_03A` to `D_801220E0..E2`.
- If `D_800E6280.unk_10F8 & 1`: `D_801220DC += 1; D_801220DE -= 1;` (separate s16 symbols).
- `i = 0; n = D_8013D308 + 1;`
- If `n >= 0x101`:
  - **Loop 1** copies 128 words (0x200 bytes) from `0x801CBEF0 + (D_8013D304 % 4) * 0x200`
    into `D_800E6280.unk_163C[0..127]`. The induction is a pointer `v0 = &D_800E6280 + i`,
    compared with `0x800E6480` (= `&D_800E6280.unk_1BC[1].unk_0C`, base + 0x200), and the
    store is `sw 0x1638(v0)` after `v0 += 4` (i.e. dest 0x163C + i). `v1` is a separate
    source byte offset (`i`). Signed `% 4` is recomputed inside the loop every pass.
  - **Loop 2**, only if `n - 0x100 >= 0`, for `m = 0 .. n-0x100`: per iteration copies
    `src + ((D_8013D304 + 1) % 4) * 0x200 + i` at +0x40 -> `p + 0x167C`,
    at +0xE0 -> `p + 0x171C`, and, only if `m + 0x60 < 0x80`, at +0x180 -> `p + 0x17BC`.
    Note the first copy's row scale is `sll 7; sll 2` (two shifts), the second and third are
    one `sll 9`: the first expression was written `((x) * 0x80 * 4)`, the others `* 0x200`.
    The signed `% 4` is recomputed for every copy, in-loop.
  - `D_8013D308 = n;` then if `n >= 0x129`: `D_8013D304 += 1; D_8013D308 = 0;`.
  - `load_palette(D_800E6280.unk_163C, 0x1A, 1, 1, 0);` and return. The ticket draft's
    `load_palette(&D_800E6280, ...)` was wrong (the argument is `0x800E78BC` = unk_163C).
- Else `D_8013D308 = n;`.

The ticket's pre-flight description ("9-word copy", destinations `&D_800E6280.unk_1BC[0].unk_0C`)
is not what the asm says: the loop is 0x200 bytes and the destinations are `unk_163C + i`
(0x167C/0x171C/0x17BC for loop 2). `0x800E6480` is the loop bound, not a destination.

## What was measured (Docker, ido 5.3, full NON_MATCHING TU)

1. m2c draft / first clean rewrite: 134 rows (`funcdiff --resolve`), i.e. no gain over the ticket.
2. `pal = D_8013D304` local + pointer-form loops: 134-146 rows. Loop 1 is instruction-for-
   instruction the target **except** that uopt hoists `(pal % 4) * 0x200` into the preheader
   (target recomputes the signed modulo inside the loop). Loop 2's row base is likewise hoisted
   and shared by all three copies.
3. Direct `D_8013D304` reads + **symbol-based** destination stores
   (`*(s32 *)((u8 *)&D_800E6280 + 0x163C + i)`) + pointer comparison
   `((u8 *)&D_800E6280 + i) < ...unk_1BC[1].unk_0C`: this fixes the two global defects —
   one `lw` of `D_8013D304` for the whole function, signed `% 4` recomputed in-loop per copy,
   and the final `D_8013D304 += 1` reuses the loaded register. Rows: 149. The remaining
   difference is the induction: uopt keeps `&D_800E6280` in a register and recomputes
   `addu v0,a3,v1` each pass instead of strength-reducing to the target's `v0 += 4` /
   `sltu v0,a0` / `sw 0x1638(v0)` and it adds a redundant `addu` in the delay slot.
4. Pointer-form loop with direct global reads: target-like induction, but the pointer store
   makes uopt treat the loop stores as aliasing and it reloads `D_8013D304` three times in
   loop 2 (and rotates loop 1, putting `v0 += 4` at the top).
5. `D_801220DC/DE` as direct `extern s16` declarations matches the target's relocation form;
   uopt/as1 hoists the `lh D_801220DE` above `sh D_801220DC` (target keeps source order).
   Using `((SpriteSlot24 *)&D_801220D0)->unk_0C/unk_0E` preserves the order but emits
   `12(at)` / `14(at)` immediate offsets that `funcdiff --resolve` reports as real differences,
   and spends two extra ugen ring slots (t2..t4 instead of t0..t2).

## Exact next hypothesis

The target's loop shape is the strength-reduced form of a source that both compares
`&D_800E6280 + i` against `&D_800E6280.unk_1BC[1].unk_0C` and stores at `+0x163C`, while the
`D_8013D304` load survives all stores and the modulo is not hoisted. Try to get all three at
once:

- make uopt keep the symbol-based store form (which is what lets the global load survive:
  a pointer store kills it), and force the pointer induction by writing the loop condition
  on an explicit `p = (u8 *)&D_800E6280` that is *also* the address base of the comparison
  and of the store — but with the store expression still rooted at the symbol, e.g. via a
  typedef/field view that the alias analysis can see through; or
- find the source spelling that stops uopt's loop optimizer from hoisting `(pal % 4) * 0x200`
  while keeping the load in a local (one candidate not tried: a second `if (pal) {}`-style
  use, or an address expression whose `i` term is not the outermost addend);
- the first copy's `* 0x80 * 4` and the others' `* 0x200` are confirmed necessary spellings
  for the target's `sll 7; sll 2` / `sll 9`.

`D_801220DC/DE` need the direct-symbol form for the relocation words; the remaining
load-before-store there is an as1 ordering question (try both increments on one source line,
or an alias-forcing common base).

## Files

- `drafts/T-24923/body.c` — variant 3 above (best global/load behaviour, 149 rows).
- `drafts/T-24923/body_pform.c` — variant 2 (target's loop 1 induction, hoisted modulo, 146 rows).
- `src/ovl/OPTION/80132000.c` — unchanged (`INCLUDE_ASM` restored).
