# T-25141 TACO func_8013FBF0: matched

Result: matched, 1040 bytes, default C in `src/ovl/TACO/8013F250.c` (`funcdiff --resolve
--version default` prints MATCH; no INCLUDE_ASM line left for the function).

## Start state

Production was plain INCLUDE_ASM. Best known body: `drafts/T-24502/TACO/func_8013FBF0/body.c`
(typed SPRT draft). Measured as the whole-unit NON_MATCHING compile: 84 diff rows, frame 72
against 96, the colour multiplies in the wrong registers (`mflo t4` for the target's `mflo s0`),
the second sprite's y0 chain evaluated in the wrong order and three UV constant swaps.

## What closed it (one change at a time, rows after each)

1. Frame and layout: `s32 pad[6]` before the declared locals (frame 0x60, g/b spill homes stay
   at 0x42/0x41), the first sprite's UV stores on one line, `setWH` for the third sprite.
   84 -> 80.
2. Colour word: `(r << 16) + (g << 8) + b` on the u8 channels, without the `& 0xFF` that m2c
   printed. The masks in the target are the u8 truncations of `r *= value` (ugen emits
   `mul s0,s0,v1; and t4,s0,255; move s0,t4`; as1 folds the move into the later shift). The
   explicit masks added three redundant `and`s: they cost ring slots and let the products go to
   temporaries. `cc -S` of both versions shows it. 80 -> 51.
3. Second sprite y0: the shift written first,
   `((0xFF - D_8015EDB4->unk84[1]) >> 2) + D_800E6280.unk_018[D_8011ECA0] + 0x18`. cfe swaps
   the operands of `+` by weight, so writing the shift first makes ugen read the array first
   (target ring: idx t4, sll t5, addu t6, lh t7, then D_8015EDB4 t8). 51 -> 4.
4. UV stores of the second and third sprite on one line (as the first sprite and
   `func_8013F874`). 4 -> 0.

The cleaned body (casts dropped, K&R unsigned `-x >> 2`, `state > 0xC0` thresholds, `setWH`
for all three sprites, `(SPRT *)`/`(s32)` casts as in the siblings) also matches.

## Negatives measured

- `s32 r` with u8 g/b: 126 rows (value gets a second, converted range in t0).
- s32 r, g, b: 82 rows (word spills `sw` instead of `sb`).
- Separate s32 product locals `rr = r * value`: 96 rows (substituted into the call).
- Sprite 2 y0 with the array first and a lighter shift: `0xFFU - x`, `0xFF - (u32)x`,
  `(u32)(...) / 4`: all 51 rows; `(u8)~x >> 2`: 48 rows with other breakage.
- A `u32 dy` local for the shift (the func_8013F9C0 pattern): 59 rows, dy becomes a uopt range
  in v0.

## FAKEs left

`s32 pad[6]` (frame extent, original locals unknown) and the one-line UV stores; both named in
the comment above the function with T-25141.

## Tooling used

Fast loop: `tools/cc.py <copy> <obj> ido 5.3 -DNON_MATCHING` plus `funcdiff --built <obj>
--no-build` (about 14 s per variant against 65 s for the plain funcdiff), and a `cc -S`
wrapper around `cc.ido_frame_env` for the ugen listing.
