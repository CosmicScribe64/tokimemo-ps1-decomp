# T-25121: ETC func_80132C3C (four POLY_FT4 quads, 222 words)

## Start state

Plain `INCLUDE_ASM`. Best earlier body: `drafts/T-24012/func_80132C3C.c` (T-24012), 105 rows in the
single-function compile (180 "cost" in T-24012's metric): frame 0x40 against 0x70, spill +4, a second load of
`D_8014DB80` for the first colour store, every coordinate register off, the return value moved from v1.

## Result

Guarded `NON_MATCHING` body in `src/ovl/ETC/80132880.c`, 15 rows (`funcdiff --resolve --version c`), all of them
four 4-use constant registers in the coordinate block (0x20, 0xA0 for u1/u3, 0x128, 0xA8: draft t0/t1/t2/t3 =
0xA0u/0x128/0x20/0xA8, original 0x20/0xA0u/0x128/0xA8). Instruction count, order, frame and every other
register match.

## What closed rows, and why (one lever at a time)

1. **Frame 0x70, spill at 0x38.** Every constant-propagated or dead scalar keeps a frame slot (measured: three
   locals assigned constants and folded into stores grew the frame by 8 and moved the spill). The original has
   13 words above the `y` spill slot; `s32 pad[13]` declared before `y` reproduces both (FAKE; the original
   probably had about 13 coordinate locals that uopt folded away).
2. **One load of `D_8014DB80` for the 12 colour stores.** With an ordinary global, uopt copy-propagates
   `shade = D_8014DB80` into the first store and emits a second `lw` (t9). A `volatile` view in
   `include/ovl/ETC.h` (FAKE) gives exactly one `lw v0` and keeps the store-back. The same view fixes the tail:
   `lw v0; addiu v0,v0,2` (no separate promoted-global range in v1). A `static`/defined global removes the
   store-back instead (measured, rejected).
3. **Return value in v0 without `move v0,v1`.** Every `return var` form puts the counter in v1 (the return copy
   in the exit block interferes). `s32` with no `return` plus a dead `if (next) {}` (RPG_BAT func_8013F350
   recipe, T-9720) keeps it in v0 and gives `or v0,zero,zero` in the branch.
4. **0x18 shared by x0/x2 (sh) and u0/u2 (sb).** cfe types a constant by the store: s16 store J, u8 store L, so
   literal or local-variable forms give two registers. Measured: a `u16` view on x0 or an `s8` view on u0 joins
   them; an uopt-folded sum (`0x18 + i`, i = 0) also gives J. 0xA0 must stay split (x J in a2, u1 L in t1), so
   only u0/u2 are J: `*(s8 *)&p->u0` on polys 0 and 2 (FAKE).
5. **Store order and the 8-use constant order (0x18 a1 before 0xA0 a2).** One line per PsyQ macro expansion
   (setXY4 / setUVWH as comma expressions, x0 first): as1 reorders stores inside a line, and the hoisted
   constants are numbered in ucode order, so x0 must come before x1 in the source. One statement per store in
   the target's store order (the T-24012 layout) gives the stores right but 0xA0 before 0x18 (40 rows).
6. **y2 before y0 (v1/a0).** Compute `y2 = y + 0xB3` before `y0 = y + 0x13`; equal priority, lower bit first.
7. **0x13 in v0, 0xB3 and the `y` reload in a3.** uopt splits a basic block after the statement at which the
   block holds 16 indirect stores (`istr`, counted before dead-store elimination; `str` to locals does not
   count; the counter restarts at calls). Constants first used in a block are loaded at its head. In our
   build the block after the last tpage call holds tpage store + XY0 + UV0, so the UV0 constants start before
   the call result is stored and cannot take v0. The original splits after XY0. Either an empty test
   `if (p0) {}` after XY0 (FAKE, used) or seven dead stores before XY0 (removed by uopt, still counted)
   reproduce it: 35 to 15 rows.

## Negatives added (measured, do not repeat)

- IDO does not unroll or promote a `POLY_FT4 *poly[4]` array with per-poly loops; the straight-line form is right.
- `return var` in every spelling (`+=`, chained, early `return 0`, separate local) keeps v1 + `move`.
- `(s16)`/`u8`/`u32` shade locals, `+ 0`, `| 0`, `*(volatile s32 *)&D` (address form `lui/addiu/lw 0`).
- Chained assignments in UV0 or XY1 (2-chains): ugen temporary t9 for the constant, no phantom uopt range.
- `u0 = x0` read-backs, `y1 = y0` read-backs: real `lh` reloads, no forwarding.
- Locals for coordinates (`s32`, `s16`, `u16`, `u8`, `s8`): the store conversion types the folded constant, no
  sharing.
- Empty-test boundaries at any other statement, alone or with the XY0/UV0 one: 15 rows at best.
- Width/height locals (`w`, `h`): folded sums become J and 0xA0 joins x and u (89 rows).
- Splitting UV1/UV3 into two lines, with or without a boundary between them: 19-22 rows.
- A local `u = 0x20` for poly 1/3 u0: propagated and rematerialized (34 rows).

## First divergence left

Row 99: `li t0,160` (draft) against `li t1,160` (original). The four 4-use constants have equal priority 1.5
(4 uses, 2 live units, denominator 2) and are coloured in bit order 0xA0u (UV0), 0x128 (XY1), 0x20 (UV1), 0xA8
(UV1). The original colours 0x20 first. A real early use of 0x20 in the UV0 block (an extra `sb` to p1->u0 before
UV0) gives exactly the original colours, so either the original uses 0x20 earlier in code that leaves no
instruction, or 0x20 has a higher priority than 0xA8 (for example a variable range with its definition in the
UV1 block: savings 5 over 2 units = 2.5), while 0xA8 stays a 1.5 constant.

## Next hypothesis

Find a code-free construct that makes 0x20 a higher-priority range than 0xA8 without touching 0xA0u/0x128: a
local assigned 0x20 in the UV1 block whose constant uopt does not propagate (a multiply-defined local: one
local for u0 of all four quads, `u = 0x18` / `u = 0x20` alternating, with the s8 views), or a 3+ chain that
leaves a phantom uopt range in the UV0/XY1 block (T-11840 note). Check the R9 units of 0x20 with
`regdiff -v` after each try; the target needs 0x20 allocated before bit 153.
