# T-25115: main _sprite_set_light_effect1 (1,124 bytes, 282 words)

Result: MATCH in default C (`funcdiff --resolve --version default`), committed as
`T-25115: match main:_sprite_set_light_effect1`.

## Start state

Plain INCLUDE_ASM. Best known body: `drafts/T-24501/main/_sprite_set_light_effect1.c`
(T-24501, resolved edit cost 374). Put under the guard and measured: 233 positional rows.

## What the target showed

- After the last call, uopt loads r (0x34), g (0x30), b (0x2C), the packet (0x38) and arg4 into
  t2, t3, t1, a2, t0, with the constants 3 in a3 and 5 in v0. Because t0-t3 hold uopt ranges,
  ugen's ring in every earlier block is t6..t9, t4, t5 (the draft used t0..t5 too).
- The black edge is one `div zero,zero,v0` with three `mflo` into v1, a0, a1: three values, one
  per colour, each used twice (vertex 1 and 2). The draft had one shared `temp_lo` with an (s8)
  cast (sign extension the target lacks) and only r had a second use, so only r got a register.
- Homes are 0x38..0x28; the draft had them one word higher (0x3C..0x2C).

## Changes, in order, with rows

1. `p->r1 = r * 0 / 5; p->g1 = g * 0 / 5; ...` per colour and per vertex (no shared temp, no
   casts), a register-only `w` declared first: 233 -> 7 rows. Left: arg8/frame-counter load order
   (3 rows) and b1/g1/r1 store order (4 rows).
2. `((s32)D_800E6280.unk_10F8 + arg8)`: 7 -> 4. Spelling `arg8 + D` alone changes nothing (uopt
   canonicalises the u32 sum both ways).
3. The three stores of each vertex on one line (later a comma statement, setRGB1/setRGB2 field
   order): 4 -> 0. as1 schedules per line and emits b, g, r while r keeps v1.
4. `w = s * 8 / 4096;` in one statement also matches (the two-step form was not needed).

## Negatives (measured)

- Reversed source order `p->b1 = ...; p->g1; p->r1` (one statement each): store order right but
  b takes v1 (4 rows).
- Named locals `dr/dg/db` assigned before the stores: same 4 rows (copy propagated).
- All six edge stores on one line: 4 rows (as1 interleaves the two vertices).
- `w` declared after `s`: 11 rows (frame homes shift).
