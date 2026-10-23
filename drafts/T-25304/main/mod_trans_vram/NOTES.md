# T-25304: main mod_trans_vram (second pass, 188 words, 752 B)

Matched. `src/main/8004C3F0.c` compiles the C in the default build (`funcdiff --resolve --version
default`: MATCH, 752 bytes). Commits: `T-25304: match main:mod_trans_vram`, then a cleanup commit
(s32 counters, no redundant casts, same bytes).

## Start state

T-25104 best body (`drafts/T-25104/main/mod_trans_vram/mod_trans_vram.c`), 47 rows, 187 against
188 words. First divergence at word 123 (inner loop head): the tile word in `v1` instead of `t0`,
uv and bank colours swapped. The first pass concluded that the original needs a range coloured `t0`
and looked for a raw-word local with a lower priority than `bank`.

## What I measured

- Reproduced 47 rows with the T-25104 body. regdiff -v (traced uopt): uv local `v0`, `*data` CSE
  `v1`, bank `t0`; uopt basic blocks split at the calls (bb15 = loop head up to `get_fnt`).
- Dumped uopt's input and output ucode (regdiff's `compile_draft` with a wrapper that also copies
  uopt's output; rendered with `cvt_pass.parse`). In bb15, `a0..a3` are written in argument order
  (`str` + `rpar`), so a range that lives past them cannot take an argument register; uv in the
  q3 form is precoloured `a2` because it is passed itself. In the T-25104 body the uv range is
  coloured `v0` but ugen forwards it into a temporary, so a uopt colour need not appear in the code.
- The q3 form (`uv = *data & 0xFF0000; ...; uv >>= 16`) gives the target's uv/bank code, raw CSE `v0`,
  no range in `t0`: 172 rows, as T-25104 measured.

## What closed (47 -> 0 rows), one cause

1. **Mask and shift in one statement for uv and bank, as in the siblings** (`mod_set_gpu`,
   `mod_set`): `uv = (*data & 0xFF0000) >> 16; bank = (*data & 0xFF000000) >> 24;`, and
   `p->unk_00[bank]`. Alone: 172 rows (no range in `t0`).
2. **flags as its own local**: `flags = *data & 0xFF;` passed to get_fnt. `u32 flags` (declared last): 60 rows;
   `u8 flags` (declared last): 10 rows. uopt colours: raw CSE `v0`, uv `a2`, bank `v1`, flags `t0` (it lives past
   the `a0..a3` writes). `t0` then leaves ugen's pool for the whole function (the else-arm `li t1,16`),
   and the final code loads the tile word into `t0`, with flags in a ugen temporary (`andi t8,t0,0xff`).
   This is the range the first pass was looking for: not the raw word but the flags byte.
3. **Frame**: `u8 flags` declared after `u8 cols` (or after rows/ratio) takes a byte next to the
   other u8 locals; declared last it adds a frame word and moves the p spill slot from 0x74 to 0x70
   (the 10 rows). 0 rows.

Cleanup on top, same bytes: `s32 col`, `s32 row`, `s32 xh`, and `col * size / 2 / ratio`,
`size / 2 / ratio` without casts (the u8 ratio promotes to unsigned int under -cckr, so the
divisions stay `divu`).

## Negatives added

- for loops with the bounds read from the record (the `mod_set_gpu` shape), with or without the
  rows/cols locals, latch `data++` in the body or in the for header: 188 rows (frame, p home and
  register use change).
- xh inlined as `x / 2 / ratio`: 143 rows; ypos inlined into the LoadSquare call: 175 rows.
- `u8 flags` declared before `u8 ratio`: 18 rows.
- `tile = *data;` with `uv`/`bank` from tile and `tile &= 0xFF` (or `tile & 0xFF`) as the flags:
  170-174 rows (tile takes `v0`, no range in `t0`).

## Tools used

- A scratch scorer (not kept): each body put into the full unit and scored with
  `order_search.make_text_scorer(..., resolve=True)` (funcdiff rows), two threads; about 15 s a body.
- ucode dump: `regdiff.compile_draft` with its uopt wrapper changed to run uopt with `subprocess` and
  copy uopt's output file next to the saved input, then `cvt_pass.parse` to list the records of the
  procedure (`mt=3` locations are registers, by hardware number).
