# T-25124: main func_80046AC8 (matched)

Result: default-build C match, 2696 bytes (671 words + 3 alignment nops). The matched body is in
`src/main/80046500.c` (the draft copy was dropped in review, so there is one source of truth).

## Start state

Best inherited body: `drafts/T-25034/main/func_80046AC8/uv-standard.c` (s32 return, no `return`).
Whole-unit `funcdiff --resolve --version c`: 78 rows. Code between rows 60 and 432 already matched.
Three groups were left:

1. Entry block: hoisted computations in another ugen order. The original numbers row4
   (`(arg0 & 0x20) * 4`) right after `order`, then texture u, texture v, the order offset, and alpha.
   Alpha's `sra`/`andi` sit one temporary higher (t6/t7, not t5/t6).
2. Stack homes: texture v at 0x60 against 0x64, order at 0x6C against 0x7C, and so on.
3. Tile part: the flag test is one temporary higher too, and t2/t4 are swapped in every case
   (ugen free-list order).

## What closed it, in order (sbs = own side-by-side diff lines; rows = funcdiff)

Tools: `regdiff` with a spill-trace uopt (`spillpatch.py` from T-25110 applied to the scratch
`trace.c`, binary `r9/variants/spill`). It prints one `R9TEMP` line per coloured PRE item: bit,
temporary index, displacement, blocked temporaries. I used it together with the `cc -S` ugen
listing (`lst2.py` below).

1. Stack homes are uopt spill temporaries. `spilltemps` hands them out in item bit order: first fit,
   and a temporary is blocked only when a lower-bit item that shares a node holds it (T-25110 rule).
   Bits follow ucode order for expressions read from the source. Expressions that copy propagation
   builds later (from `order`, `alpha`, `row`) get bits after all ucode bits. The original layout
   (order 0x7C, blue 0x70, texture u 0x68, texture v 0x64, order offset 0x5C, alpha 0x58, frame 0xA8)
   needs this:
   - texture u/v written inline in the `func_8009ECB0` call, with no `texture_row` local. Row4 in
     the cases is the expression `(arg0 & 0x20) * 4`. 69 to 67 sbs.
   - `order * 4` inline in AddPrim; `alpha = arg6 >> 28` (variable) tested with `alpha & 0xF`. Both
     are built by copy propagation, so ooff and alpha come last.
   - the v2/v3 endpoint as `row + 0x7F` with `row = (arg0 & 0x20) * 4;` at the top. The endpoint item
     is copy-propagated and moves after texture v (temporary t7). v0/v1 keep the plain expression.
   - nine declared scalars: `green` dropped (inline colours). With 10 the frame is 0xB0. Dropping
     `gs` instead changes the endpoint priority (40 > 36.67): a0/a1 swap.
   - the tile blue needs t0..t2 blocked. `if (order) {}` after the tile loop (FAKE) extends the order
     item into the tile region. Red then takes t1, green t2 and blue t3 = 0x70. This was the last change.
2. Extra ugen temporaries that as1 deletes (cheatsheet section 2, applied here):
   - `u8 alpha`: ugen emits `sra; and 255; and 15`. as1 deletes the `and 255` and renames the shift
     to the deleted op's register, which gives `sra t6; andi t7`. That puts the entry ring and the
     two alpha reloads at t8/t9.
   - red `(u16)(arg6 >> 16) & 0xFF`, the same expression in both loops (the variable `red` is
     assigned again inside the tile loop). The chain is `sra; and 0xFFFF; and 255`, so red is two
     slots with the shift visible first. Sharing with the tile red keeps it in fp. 587 to 184 sbs.
   - tile flag `(u8)(arg6 >> 24) & 0xF`: one slot, like alpha. 184 to 41 sbs.
   - tile green `(u16)(arg6 >> 8) & 0xFF` (2 slots) plus blue `(u16)(arg6 & 0xFF)` (one deleted slot)
     move blue to t3 and fix the t2/t4 free-list swap. 41 to 12 sbs, 12 rows.
3. The quad loop as `for (quadrant = 0; quadrant < 4; quadrant++)` instead of `quadrant = 0; do`
   moves `move s6,zero` into place. 12 to 7 rows. Putting the head on one line (autotweak
   `group head`) has the same effect.

## Negatives (measured, do not repeat)

- Alpha casts `(u32)(...)`, `(s32)(u32)`, `(s8)`, `(s16)`, and an unsigned arg6 change nothing (no
  extra slot). Only u8 or `(u8)` gives the slot; `(u16)` would too.
- Quad red forms with `u8` alpha: `(arg6>>16)&0xFF`, `(u32)(..)&0xFF`, `(s32)(u8)`, `(s16)(..)&0xFF`,
  `(u8)(..)`, `(s8)((..)&0xFF)`, `(u8)(s8)(..)`, `(u8)(..)&0xFF` all lose the fp sharing or the slot
  count (535-761 sbs). `(u16)` alone also loses fp unless the tile reassigns `red` in the loop
  (`d4a`); assigning it before the tile loop (`d4b`) does not work.
- Tile green `(u8)((u16)..)`, `(s8)(u16)..`, `(u16)(s8)..`, `(u8)(s8)..` as an expression or as a
  reassigned variable: 2 slots with blue at t2, or 3 slots with the visible register second.
- Blue `(u16)arg6 & 0xFF` and `(u16)(s32)arg6 & 0xFF` load with `lhu`; `(u8)(arg6 & 0xFF)` is
  folded. `(u16)(arg6 & 0xFF)` and `(arg6 & 0xFFFF) & 0xFF` work.
- Row4 feeding v0/v1 through `row` (instead of the endpoint) moves row4 to s3.
- `order = arg7; order &= 0xF;` folds to the same code. Quad case 3 through `radius` changes nothing.
- Tile colour statement order (b,r,g / r,b,g) does not change the homes.
- autotweak (300 s on the 67-sbs state) found only `group head` (exact 112 to 110).

## Scripts (scratch, not committed)

`put.py`/`put2.py` (insert a body into the guard), `sbs.py` (side-by-side diff of target `.s` and
regdiff's `draft.o`), `homes.py` (R9TEMP temporaries next to the built stack slots), `lst2.py`
(`cc -S` ugen listing of the unit with `INCLUDE_ASM` lines stripped), spill-trace uopt built with
`drafts/T-25110/TACO/func_8013EAA4/spillpatch.py`.

## FAKEs left in the source (all marked T-25124)

`if (order) {}` at the end; the `row` local feeding only the endpoint; the (u16)/(u8) conversions;
the inherited `goto block_16` UV layout and one-line RGB/UV comma groups. The next hypothesis for a
cleaner source: the original likely used narrow locals (sibling `func_80046754` uses `u16 load_x/y`
and `u8 color`). Replacing the casts with u8/u16 locals while keeping the spilltemps order could
remove most of the casts.
