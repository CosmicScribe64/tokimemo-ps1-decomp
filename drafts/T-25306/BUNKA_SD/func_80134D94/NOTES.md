# T-25306: BUNKA_SD func_80134D94 (second pass)

Result: matched (972 bytes, default C, `funcdiff --resolve --version default` MATCH). The body is in
`src/ovl/BUNKA_SD/80134540.c`; the header declares `s32 func_80134D94(void);` (implicit-int function, as
T-25142 found). About 80 minutes.

Start state: the T-25142 body (`drafts/T-25142/BUNKA_SD/func_80134D94/func_80134D94.c`), 45 positional rows,
aligned cost 12, plain `INCLUDE_ASM` in the source.

## What closed it (in order)

1. **First divergence, reread.** T-25142 put it at the first block's counter load (`lui v0` against the
   original's `lui a1`). A binasm dump of ugen's output (scripts below) shows what that `lui` is: a load
   expression range X in v0 (`lbu v0; move a1,v0; divu v1,v0,14`), and as1 renames only the load's
   destination, never its `lui`. So the original loads the counter straight into a1, and some other
   range holds v0 and v1 in that block. The register map of the T-25142 body is otherwise right
   (D v1, q a0, b a1, t v0, selector a2), so X stands in for a range the original has but does not show.
   A colouring argument (Chow ranges are per block, priority order) shows that with only D, q, b, t and
   the selector there, q can never get a0: one more invisible range in that block is needed.
2. **The selector read moved to the last block** (`if (D_80122EAC) {}` after `D_80120778 = w + 0xA2`).
   The T-25053 empty test in the first block gave the selector a2 by making it live there, but also
   loaded it one slot early. In the last block the selector is already in a register (its test is
   just before), so the read costs nothing, and the selector range now overlaps w (v1): it still skips
   v1 and takes a2, and its first-block load stays at the switch (`lui a2` hoisted, `lw a2` at the join,
   `addiu t3,a1,1` in the `beqz` slot). 45 -> 6 rows.
3. **The invisible range: a copy of the counter.** `t = (u32)b * 1; if (t < 0) {} q = (u32)t / 14;` with
   b read through a pointer (`g->unk_110D`, or `((u8 *)fc)[9]` in the first try). Read through a
   pointer, b is not copy propagated, so ugen loads it straight into a1; the copy is a uopt range in v1
   (`move v1,a1; divu v0,v1,14`) that as1 folds back into the divu. It blocks v1 for q, b and the
   frame counter in that block, and the division range D takes v0 (coalesced into `mflo a0`). Reusing
   `t` for the copy keeps the col spill at 0x3C; a new local `z` moved it to 0x38. 6 -> 2 rows.
4. **Last block, w and n.** The tail order (`move v0,v1` before `sb t8`) is an as1 tie: both nodes are
   ready at the same time with the same line, and the store, released last by the `addiu`, goes to the
   front of the ready list (traced with `/private/tmp/t24047-uopt/variants/as1-trace`, `-xbbdbg 8`). With
   the promoted-global form the store-back is emitted at the return and takes its line, so no `#line`
   can separate them (measured). With a local counter n the store is an ordinary store on its own line,
   and a `#line` pair giving `return w;` an earlier line puts the move first. The local counter then
   took v1 and w a0; `if (w) {}` (one more use of w) colours w first, and loading n before the
   `D_80120762 = 0x40` store (as the first block loads its counter first) gives the original's first
   lines. 2 -> 0.
5. All GameState reads go through one `GameState *g = &D_800E6280;` (instead of the T-25142
   `GsWord *fc` pointer plus direct field reads): same bytes, fewer views.

## Negatives (this pass)

- The `if (D_80122EAC) {}` read placed after the increment in the last block: 60 rows (reallocates).
- Separate scalar symbol `extern u8 D_800E738D` instead of the field: identical to the field.
- Copy propagation off for the whole procedure (scratch `cvt_pass` copy, experiment only): 264 words.
- Merging block-1 and last-block variables (q with the last counter, the raw quotient with w): uopt
  propagates the block-1 copy away or colours the merged range v1; 16-40.
- `(u8)b / 14`, `(b & 0xFF) / 14`: an extra ugen temporary shifts the ring, 128+.
- `y = b * 1` with matching types folds to b; it needs the `(u32)` cast and an `s32` target.
- Recompute form (`if (b / 14 >= 2) b = 0; q = b / 14;`): uopt's PRE gives the reset a `move v0,zero`,
  the frame counter v1.
- Last block: `#line` with the promoted counter, comma return, `++`, `= x + 1`: still store first.
- Parameters reused as q and b (K&R): homed to the stack, frame 0x40.

## Scripts (copy to build/T-25306/)

- `as1save.py`, `gc.py`, `g.sh`, `gdump.py`: compile a body (via `mk.py`, which needs a NON_MATCHING guard in the source and writes the unit with the
  body as default C) with an as1 shim that saves ugen's binasm (`.G`, `.T`), then print
  func_80134D94's binasm records (`g.sh BODY.c FROM TO`). This shows ugen's registers and order before
  as1 coalesces copies and schedules; it is what separated the load-expression range from the lui.
- The traced as1 run: `cp g_X.T x.T; /scratch/variants/as1-trace -elf -G 0 -p0 -EL -g0 -O2 -R -xbbdbg 8
  g_X.G -o x.o -t x.T` with `TOKIMEMO_SCRATCH=/private/tmp/t24047-uopt`.
- `step1-selector-in-last-block.c` (6 rows) and `step2-counter-copy.c` (2 rows) are the intermediate
  bodies.
