# T-25104: main mod_trans_vram (188 words, 752 B)

Not matched. `src/main/8004C3F0.c` keeps `INCLUDE_ASM` (47 rows is above the guard limit).
Best body: `mod_trans_vram.c` here, 47 rows (`funcdiff --resolve --version c`, 187 against 188
words). Controls: `control-63-cse-raw.c` (63 rows), `control-45-rows-reuse.c` (45 rows, reuses the
`u8 rows` bound as a flags temporary; not adopted, it is a FAKE-only form).

## Start state

T-24501 body (`drafts/T-24501/main/mod_trans_vram.c`): 226 rows, 196 words, explicit `sp78`
shadow of the cell size, frame homes wrong (buffer 9C, ratio 9B, row 19C).

## What closed (226 -> 47 rows), one cause each

1. **Donor shape.** `mod_set_gpu` (same file, 17-row guard) gives the loop shape: `u8 rows/cols`
   locals, `data = rows * frame * cols + p->unk_04`, `u32 row/col`, do-while with `cols = p->unk_0C`
   reloaded in the inner loop and `rows = p->unk_0D` inside `if (cols != 0)`. `uv`/`bank` locals for
   the tile-word masks. 212 rows.
2. **No masks on the LoadSquare arguments.** The u16 prototype supplies the conversion; uopt hoists
   one `andi` per invariant argument and the call keeps its own, as in the target. An explicit
   `& 0xFFFF` or `(u16)` adds a third `andi`.
3. **One `size` variable, no shadow.** The target's else-arm `li t1,16; sw t1,0x78; li a1,16` is
   uopt's live-range split of a single variable (uopt output: `str home; rlod home`), not a second
   variable. The outer piece only becomes profitable when the column offset is written
   `col * size` (uopt strength-reduces it into the `s1 += size` induction) instead of a separate
   `xoff` accumulator. With `xoff` the whole range stays in memory.
4. **Frame.** Declaration order `col, data, row, uv, bank, p, buf[256], ratio, rows, cols, xh, ypos,
   size` gives frame 0x1A0 and every home (row 194, buf 88, ratio 87, size 78, p spill 74). `u8`
   locals take one byte; `p` lives in a compiler spill slot below the declared locals. No pads.
5. **Hoisted temporaries in target order** (s4 ratio, s5 w, s6 h, s7 xbase, fp ybase).
   `ypos` as a local (computed before the inner loop) puts the y temporary last; `xh = x / 2` as a
   local (any placement, now once at the top) puts the x base after w/h. Found by the permuter
   (`xh`), confirmed by placement controls (5 placements, all 47).
6. **`uv = *data; uv &= 0xFF0000;`** puts a uopt colour in `t0`, so ugen's pool loses `t0` for the
   whole function exactly as in the target (63 -> 47 rows): every row before word 123 now matches.

## First divergence left (word 123, loop head)

Target: `lw t0,0(s2)`, `and a2,t0` (uv), `and v1,t0` (bank), `andi t8,t0,0xff` (flags); uv shifted
through `srl t6,a2,16; move a2,t6`. Best: raw word in `v1` (the `uv` variable's first web),
masked uv forwarded into a ugen temporary, bank in `t0`. Everything after is ugen numbering in the
same block plus one word (the uv shift goes straight into a2).

The q3 form (`uv = *data & 0xFF0000; bank = *data & 0xFF000000; uv >>= 16;` and
`p->unk_00[bank >> 24]`, flags `*data & 0xFF`) has the target's uv/bank instructions exactly, but
the raw word is the `*data` CSE temporary in `v0` and no uopt range uses `t0`, so the ugen pool
shifts the whole function (172 rows).

uopt output (dumped with `.scratch` shim around tools/cvt_pass.py run_uopt; decoded with its
parser): in the loop block, declared locals never take v0 (cols, a global v0 range, shares the
block), the `*data` CSE temporary does (finer interference), params a0..a3 are blocked, and locals
are coloured by priority. The target therefore needs a raw-word holder that cannot take v0 and is
coloured after `bank` (v1): a local variable with lower priority than `bank`, which no tested
source produces.

## Negatives (measured, do not repeat)

- frame/idx/y/ypos/bank/uv reused as the raw word; frame as the column counter (frame then s3).
- declared `tile = *data` in 40+ statement/order/source combinations (tile copy-propagated away,
  or coloured before bank, or bank forwarded); `*data++` forms (increment moves before the call).
- for-loop forms (same code as do-while, except rows reload per row: 53 rows).
- loop bounds read directly from the record (`col < p->unk_0C`, `row < p->unk_0D`), all 12
  combinations; rows/cols as s32/u32/u16.
- void or s32-return views of get_fnt; s32 return of mod_trans_vram.
- empty tests inside the inner loop body (any variable): break LICM, 182 words.
- flags spellings `(u8)*data`, `*data`, `(s32)*data & 0xFF`, `(u8)(*data & 0xFF)`.
- permuter rounds (900 s and 600 s at -j 2): best are FAKE-only (`rows = *data` reuse, 45 rows;
  constant-mask chains that do not compile in the build pipeline).

## Next hypothesis

Find what makes the target's raw-word holder a lower-priority local than `bank`: a local that is
also referenced outside the inner-loop block without a call in its range (so it has more blocks
and a lower priority), for example a value tested or assigned in the outer loop before the inner
loop starts. Check each candidate in the uopt output (raw word must be `str R8`, bank `str R3`,
uv `str R6`) before measuring rows.
