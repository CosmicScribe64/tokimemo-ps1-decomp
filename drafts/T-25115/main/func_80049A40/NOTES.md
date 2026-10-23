# main func_80049A40 (T-25115)

Result: matched, default C, 1456 bytes (372 words); `funcdiff --resolve --version default` MATCH, main sha1 OK.
About 60 minutes from the m2c draft.

## Start state

`INCLUDE_ASM`. No hand draft existed: only the T-14500/T-17000 m2c census (score 589). The other hits for
the name in drafts/ and tickets are callers (the `MAIN_API_OVERRIDE_func_80049A40` views).

## What the function is

POLY_G4 box: `GetWorkBase(0x24)`, `func_8009F054` (SetPolyG4), `SetSemiTrans(p, 1)` or `SetSemiTrans(p, 0)` in an
if/else on `arg6 & 0x80`, a 7-entry switch on `arg6 & 0x7F` (table from 0, entry 0 = default; arms in source order 6, 5, 4, 3,
1, 2, 0/default), then the coordinate block of `_sprite_set_box_shade_tarao` and `AddPrim`.

## Steps (rows from a scratch splice-compile-diff loop, about 2 s per variant)

1. Header: the original reads the coordinate homes with `lw`; an `s16` definition gives `lh` (4 rows). The
   definition is `s32`, so `main_api.h` now says `s32` too. A full rebuild changed no caller unit (the callers
   that pass wide values already use the unprototyped `MAIN_API_OVERRIDE_` view).
2. m2c-shaped draft with `case 0: default:` and comma lines per vertex: 354 rows. Store order in every arm
   was already right; `p` sat in `s0` where the original keeps it in `a2` with `sw/lw 0x28(sp)` around each call.
3. Cause of `s0`: `p` crosses three calls (func_8009F054 and the two SetSemiTrans), so the caller-saved
   colour costs 6 (+0.3 precolour), and the first use of a saved register costs `0.25 * blocks`. The draft had
   21 uopt blocks (5.25 + 0.3 = 5.55, cheaper). A test that only added three blocks flipped `p` to `a2` (24
   blocks). uopt ends a block at a statement boundary once 20 variable loads are counted (`varreflimit`,
   uoptinput.c: checked at LOC/STR ops, not at ISTR; loads of a variable assigned earlier in the same block are
   forwarded and not counted).
4. Where the blocks come from: the colour channels are `s32 r, g, b` computed once after the SetSemiTrans
   if/else (uopt copy-propagates them into each arm, so every arm reloads arg5 and all arms share the same
   expression bits: b in a0 everywhere, r/g/b in v0/v1/a0 in cases 2 and default, constant 10 in a1, arg5 in
   t1), and the black vertices are `r * 0, g * 0, b * 0` (folded to `sb zero`), as in
   `_sprite_set_light_effect1`. Cases 6, 5 and 4 then count 24 loads and each gets a block before its `break`.
   `gs = &D_800E6280;` at the top makes the coordinate lines count `gs` too, which ends a block before `y2`;
   the original reloads arg3 (`lw t1,0x3C`) exactly there.
5. Case 3: the original spends one ugen temporary before each shift (`[t9] srl t3; sll t4; [t5] srl t6; sll
   t7`). `(u8)r * 8` (or `(r & 0xFF) * 8`) reproduces it; plain `r * 8` is two slots short (272 rows, every later
   temporary off). FAKE, marked in the source.

## Negatives (measured)

- Per-arm locals r/g/b in cases 2 and default (63 rows at best) or repeated expressions with no locals
  (361 rows, unsigned divides).
- On the per-arm-locals body: chained zero stores `p->b0 = p->g0 = p->r0 = 0` give 24 blocks but a cfe chain temporary takes v0 (85 rows).
- `colour * 0` on the arg5 expressions instead of on r/g/b raises the b range's priority: b lands in v1 (78).
- One statement per case over four lines: store order wrong. One store per statement: no change.
- Case 3 spellings without the slot: `r << 3`, `(u8)(r * 8)`, `(s32)(r * 8)`, `r * 8 / 1`, `(r + 0) * 8`
  (272 rows); `(r * 8) & 0xFF` puts the slot after the shift (4 rows).
- `u32` arg5 instead of `(u32)` casts: no change.
