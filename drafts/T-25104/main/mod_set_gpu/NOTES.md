# T-25104: main mod_set_gpu (208 words, 804 B)

Not matched. The guard in `src/main/8004C3F0.c` is at 4 rows (was 17, T-25009). Registers,
instruction order and all stack slots except one match. Measured with
`funcdiff mod_set_gpu --unit main --resolve --version c` ("compared NON_MATCHING C").

## Start state

T-25009 guard, 17 rows: clut in `$s4` instead of `$s7` (x counter and data one register off),
the latch increments in the wrong order, and the `p` and `ypos` spill slots 4 bytes off. It used a
FAKE `if (last) {}` to lift `last` into `$s3`.

## What closed, one cause each

1. **Mask and shift in one statement** (`uv = (*data & 0xFF0000) >> 16`, same for page and clut;
   the mod_set lesson). clut drops below the x counter and data in uopt priority. With the
   `if (last) {}` FAKE still in, other registers shifted, so the FAKE was removed: 17 -> 8 rows,
   with every register right.
2. **Latch order `col++; data++;`**. The original increments col, then the x counter, then data.
   With `data++; col++;` data came before the x counter. 8 -> 6 rows.
3. **`ypos = row * 8 + y + 0x168;` inside the inner loop, just before `s->y0`**. uopt hoists it
   into the inner preheader, after the x counter in expression order, so its spill temporary moves
   from 0x64 to 0x60, as in the original. 6 -> 4 rows.

## How the spill temporaries are laid out (measured, used for the rows above)

- Declared scalars take homes top-down (trace: row -12, s -16, ... clut -36, tpage -38,
  last -40, bright -44, mode -48, ypos -52, rows -53, cols -54, p -60; final offset = 0xA8 + home).
- The compiler temporaries sit below the last home, in this order: copy-propagated `p` expression
  (spilled), then the hoisted preheader items in expression-table (bit) order: x counter IV
  (in `$s4`, its slot is never used), ypos (spilled), pri*4 (spilled). Two observations agree with
  this order: ypos in the outer loop gives p 0x68, ypos 0x64, (x IV) 0x60, pri4 0x5C; ypos in
  the inner loop gives p 0x68, (x IV) 0x64, ypos 0x60, pri4 0x5C.
- The uopt source (n64decomp/ido `uopttemp.c` gettemp/findbbtemps, `uoptreg1.c` makelivranges)
  supports this: temps are created in node order, and preheader inserts are taken in bit order.

## First divergence left

`sw t8,0x68(sp)` against `sw t8,0x6C(sp)` (rows 22, 54, 123, 164: the store of `p` and its
three reloads). The original keeps `p` in 0x6C, the slot of p's own declared home here. Two ways
fit the original frame (0xA8, ypos 0x60, pri4 0x5C):

- (T1) `p` is a real variable that is not copy-propagated, so it is spilled at its home 0x6C,
  and one more frame word sits between the homes and the temporaries. Measured control:
  `control-7-rows-p-home.c`. An extra local after p plus FAKE `if (&p) {}` at the end of the
  function give the right frame and every slot. Two things are left: the latch load of p goes into
  a ugen temporary (`$t4`) where the original has a uopt colour (`$a3`), because an address-taken p
  is no longer a register candidate, and the ring shift after that load.
- (T2) the declared homes end at 0x70 (one word fewer, e.g. `u8 rows, cols` at the top and a
  single pad), so the p expression temporary is 0x6C. That needs one more temporary before ypos;
  without it the frame shrinks to 0xA0 (measured, offsets from the frame top: p -60, x IV -64,
  ypos -68, pri4 -72; the original has p -60, ypos -72, pri4 -76).

## Negatives (measured, do not repeat)

- `p = D; p += idx;` (is_incr does not hold for it), `(void)&p` (cfe drops it), `if (&p) {}`
  right after the def (frame right, but the ugen ring moves from the switch on: 86 rows).
- Ways to make p a non-propagated variable: `idx = GetWorkBase(...)`, `idx = p->unk_08; switch (idx)`,
  zcopy off (scratch experiment only, cvt_pass copy) and `ModRec14 *pa[1]`. All of them give
  frame 0xA0, or put p in `$v1`/`$a0` at entry (the original uses `$t8` and a reload).
- x0 spellings (`x + col * 8 + 0xA0`, `col * 8 + x + 0xA0`, `x + (col + 20) * 8`,
  `0xA0 + x + col * 8`), a `xpos` local (s32/u32 in the inner or outer loop; s16/u16 change code):
  no extra temporary.
- ypos spellings in the T2 layout (`y + row * 8 + 0x168`, `row * 8 + (y + 0x168)`, `<< 3`,
  split `ypos = row * 8 + y` / `+ 0x168` at the use, u32/s16/u16): no extra temporary, or code changes.
- ypos as an inline expression without a local: frame 0xA0, 74 rows.
- moving `u8 rows, cols` next to tpage/last (mode moves).
- Permuter 900 s at -j 1 on the 4-row body: best only re-parenthesises (no score change).

## Strongest lead (measured last): (T2) with `x + 0xA0` as its own hoisted item

In the T2 layout (`u8 rows; u8 cols; s32 padB;` above `row`, nothing after `ypos` but `p`),
writing `s->x0 = (s16)(x + 0xA0) + col * 8;` gives the original frame and slot map exactly:
frame 0xA8, p 0x6C, (x + 0xA0) 0x68, IV 0x64, ypos 0x60, pri4 0x5C. The cast keeps `x + 0xA0` as a
separate subtree, which LICM hoists before the IV, and that is the missing temporary. The code is
wrong, though. The cast adds `sll/sra`, `x + 0xA0` is spilled, and SR builds the IV from `col * 8`
alone (`move s4,zero`, with the add inside the loop). In the original, `x + 0xA0` is computed
straight into `$s4`, and the IV starts from it. So the original has `x + 0xA0` as a hoisted item of
its own, with no cvt between it and `+ col * 8`. Its slot is reserved but never used, just like the
IV's. Measured without success: `(u32)`/`(s32)` casts, parenthesised or reordered sums, an `xpos`
local (inner or outer loop, s32/u32), `x + 0xA0 + (s32)(col * 8)` (SR of col*8 only, frame 0xA0),
and an empty `if (x + 0xA0) {}` (the item appears at 0x64, registers shift).

## Next hypothesis

Find a spelling of the x position that cfe and uopt do not reassociate into `(x + col * 8) + 0xA0`,
so that LICM hoists `x + 0xA0` as an item and SR then takes it as the IV start. Apply it in the T2
layout. Ideas not yet measured: x through a K&R-promoted or narrower parameter type with no code
cost, `x + 0xA0` as a shared subexpression used twice in the body (for example the original may have
computed the clip or x1 of the sprite from the same base), or the base taken from a field or global.
(T1), a non-propagated register-candidate `p`, stays as the fallback. Every idx reuse tried
colours p at entry.

## T-25201: matched

`mod_set_gpu` is default-build C (804 bytes), 27/27 SHA1. Neither T1 nor T2 as written above was the
answer. Two changes together:

1. **The loop bounds read from the record in `for` conditions**, with no `rows`/`cols` locals:
   `for (row = 0; row < p->unk_0D; row++) for (col = 0; col < p->unk_0C; col++, data++)`, and
   `data = p->unk_0D * frame * p->unk_0C + p->unk_04`. uopt keeps the two counts in v1/v0 as
   partial-redundancy items: the column count is reloaded at the inner latch, the row count when the
   inner loop ends. The original's `rows = p->unk_0D` inside `if (cols != 0)` was this placement.
   Measured: two temporaries then sit between p's home and ypos (sp+0x68, 0x64; the x IV is one).
2. **p not copy-propagated anywhere**: a FAKE dead store `idx = 0;` right after `p = ...`. The
   assignment of p is then not available below it, so every use reads p from its own home. The
   latch use goes to `$a3` across the latch and the inner exit (as in the original), not to a ugen
   temporary as with the address-taken control. The store must be in the entry block: at the
   `data` line instead (`idx = p->unk_0D * frame * p->unk_0C`) p is coloured `$v1` at entry (96 diff
   lines).
3. `s32 xpos` declared before `ypos` (`xpos = x + 0xA0 + col * 8; s->x0 = xpos;`) adds the home word
   that puts p's home at sp+0x6C.

Measured on the way: without the dead store, the `for` form uses two slots for p (a copy-propagated
temporary for most uses and p's home for the two latch conditions; uopt does not copy-propagate into
a `fjp`/`tjp` whose successor is a loop's first block, uoptcopy.c). Implicit `int search_tpage_multi()`
misses 51 lines, so the u16 view stays (file scope; `mod_set` still matches with it).

Source for the rules (IDO uopt decomp in the T-25108 scratch copy): uoptcopy.c (copy propagation is
refused when the assignment is not available at the block, the expression changes in the block, it
contains an `ilod`, or the use is a loop-latch branch), uoptreg2.c `spilltemps` (PRE temporaries in
bit order; a later item reuses an earlier item's temporary when their regions share no block),
uopttemp.c `gettemp` (block-local CSE temporaries at emit time, reused when free).
