# T-25110: TACO func_8013EAA4 (not matched, 4 rows)

Result: guarded in `src/ovl/TACO/8013DB80.c` (`#ifdef NON_MATCHING`, default build stays
INCLUDE_ASM). Full-TU `funcdiff --resolve --version c`: 491/491 words, 4 differing words (cost 8).
`body.c` here is the guarded body. The four tables it reads are now declared in
`include/ovl/TACO.h` (`extern u8 D_8015F560[8], D_8015F568[8], D_8015F570[8], D_8015F578[8];`).

## Start state

T-24502 `coordinate.c`: 96 differing words (cost 186), frame 0xF0 against 0x110, 84 of them
sp offsets, plus the first-grid init swap (2), the first CLUT registers (8) and the final-grid
latch order (2).

## What changed, in order (each measured in the full TU)

1. **Frame = declared slots + uopt temporaries (96 -> 29).** IDO puts declared locals top-down in
   declaration order (unused scalars and arrays keep their slots), then uopt's temporaries
   (spill homes of PRE/CSE values and induction temporaries) directly below the last declared
   slot. The target's declared region is 0x110..0x70: two slots above the MATRIX, MATRIX at
   0xE8, 32 bytes, SVECTOR v 0xC0, pt[4] 0xB0, TcPos pos 0xA8, four slots, depth 0x94, flag 0x90,
   one slot, kind 0x88, phase 0x84, three slots, height 0x74, one slot. The draft's eleven other
   scalars fill the eleven single slots exactly; the 32-byte gap is `s32 pad[8]` (FAKE).
   Temporaries in the target: 0x6C height*2 (loop-2 step), 0x68 unused, 0x64 particle pointer,
   0x60 dist*4, 0x5C clut, 0x58 third-grid Y, 0x54 point offset (o*4, shared by all three grids),
   0x50 unused.
2. **Third grid as a plain index loop (29 -> 27).** `for (o = 0; o < 2; o++) { v.vy = pos.y +
   o * 0x100 - 0x80; ...}`: uopt strength-reduces o*0x100 and o*4 and replaces the test by
   `Y != 0x200` itself. The declared Y accumulator is gone (a declared variable keeps its declared
   slot, the target's Y lives in the temporary area). Fixes the latch order rows 404/405.
3. **First grid as a plain index loop (27 -> 13).** `v.vy = pos.y + o * 0x80 - 0x40;` adds the
   loop-1 induction temporary the target has (it takes 0x58 in our build and moves the point
   offset to 0x54) and fixes the init swap rows 59/60. The E778-style
   `for (y0 = 0, o = 0; ...) ... y0 += 0x80;` also fixes the swap, but has no such temporary
   (point offset stays at 0x58, 16 rows).
4. **CLUT row as its own statement after the frame if/else (13 -> 4).**
   `row = D_8015F568[kind];` between the `if (D_8015F560[kind] != 0) frame = ...; else frame = 0;`
   and `GetClut(frame * 0x10, row + 0x1E0)` gives the target's v0/a2/a3 (kind a3, row address v0,
   frame a2). `row` before the if: 17 rows. Ternary in place of `frame`: same as if/else.
5. Loop 2 as `v.vy = pos.y + o * height * 2 - height;` (no `y1` local) gives the same 4 rows; the
   `y1` and `y0` slots became `pad_b`/`pad_a` (FAKE).

## First divergence left (rows 374, 377, 402, 406)

The third grid's Y temporary (o*0x100; init `sw zero`, two loads, one store) is at sp+0x64,
the particle pointer's spill home, instead of sp+0x58. Everything else matches, including all
other homes.

Mechanism (IDO 7.1 uopt decompilation, github.com/n64decomp/ido, `spilltemps` in
`src/uopt/uoptreg2.c`, `gettemp`/`findbbtemps` in `src/uopt/uopttemp.c`): uopt gives each PRE item
in `coloreditems` a temporary in bit order, first fit over the temporaries already made, skipping
a temporary held by a lower-bit item whose region shares a graph node. Our Y takes the particle
pointer's temporary because the pointer is dead after the cosine call. Measured: when the pointer
stays live past the third grid (FAKE `if (particle) {}` after the third AddPrim, or anywhere after
the grid), Y moves to 0x58 and the point offset stays at 0x54, exactly as in the target, but two more
temporaries appear (frame 0x118, 94 rows). So the target has something that holds 0x64 during
the third grid only and comes before Y, or keeps the pointer busy there without extra temporaries.

## Negatives (all full TU, on the 4-row body unless noted)

- Loop 3 spellings: `o * 0x80 * 2`, `o * 2 * 0x80`, `o <= 1`, `o != 2`, do-while: 4 rows.
  `(o << 8)` 97, `o * 0x100 + pos.y - 0x80` 5, `j * (size * 2)` 4, explicit `y1 = size * 2` step
  138, outer/inner counters swapped (`pt[o * 2 + j]`) 219, separate inner counter 274, v.vy or v.vz
  inside the inner loop 93-235, `o = 0` before the cosine call 98, `v.vz` before `size` 18,
  declared accumulator (`for (y1 = 0, o = 0; ...)`) 9.
- Loop 2 explicit accumulator `y1 += height * 2`: 112. One variable for height and size: 133.
  `s16 size` 51, `s16 height` 142.
- Particle: direct `D_8015EC70[arg0].` reads instead of the pointer: 4 (same). Empty tests of the
  pointer: before the third grid 5, inside it 223, after it or at the end frame +8 (94).
  Dead `particle->unk12;` is removed (4). `if (j * size * 2) {}` 108, `if (size * 2) {}` 9.
- clut written inline twice: 4 (same).
- decomp-permuter, 15 min single thread from the 4-row body (stripped function, base 103 =
  padding nop + the stack rows): nothing better.

## Next hypothesis

Find the item that holds sp+0x64 during the third grid in the original. Candidates by the
`spilltemps` rule: a PRE item with a lower bit than the Y expression (first used earlier in
the source) whose region covers the third grid but not the code before the cosine call. A
traced uopt that prints `spilltemps` decisions (bit, region nodes, chosen temploc) would settle
it in one compile: dump them for this body and for the `if (particle) {}` control and compare.
`dump_ucode.py` + `homes.py` here print the homes of a guarded TU (run in Docker:
`python3 drafts/T-25110/TACO/func_8013EAA4/dump_ucode.py <guarded.c> x <out.O>` then
`python3 drafts/T-25110/TACO/func_8013EAA4/homes.py <out.O> -1 272`; ucode offset + 0x110 = sp).
