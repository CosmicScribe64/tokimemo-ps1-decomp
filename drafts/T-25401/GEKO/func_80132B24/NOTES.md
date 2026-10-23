# GEKO func_80132B24 (T-25401)

## Pass 1 (wave 11, 2026-10-23): matched

Start: the T-24302/T-24709 guard in `src/ovl/GEKO/801323E0.c` (frame 0x50, both
table copies right), 57 `funcdiff --resolve` rows, 124 built words against 120.
Result: MATCH, 480 bytes, default C, two marked FAKEs. About 60 minutes.

Measuring: whole-unit `funcdiff` costs about 90 s on the loaded host, so bodies
were scored in a one-function translation unit (`cc.py` on `common.h` +
`ovl/GEKO.h` + body, `objdump` against `expected/`, branch targets relative to
the function). That count tracked the whole-unit rows to within 3 lines. uopt
input/output ucode was dumped with shims around `cvt_pass.py` and
`ugen_pass.py` (copy the input file, then run the real pass), and
`regdiff.py -v --src <tu>` gave the ranges and the expression table
(`draft.o.trace`).

### First divergence and the two causes

1. **Girl in `$v1`, every exit with `move v0,v1`.** regdiff: girl is the
   promoted global `unk_F5F` (bit 11), coloured before the record pointer, but
   it takes `$v1` at cost 0, so `$v0` was barred for it. Any `return girl`
   (also with `return girl` in the early exit) does this: a range with a unit
   in a block that sets the return register never gets `$v0`. With no return
   at the end (s32, falls off), girl gets `$v0` but its range ends early, so
   day and the constant 12 take `$v0`/`$v1` (139-152 lines). The original keeps
   girl live to the exit without a return: an empty `if (girl) {}` at the end
   (FAKE) gives girl `$v0`, the record pointer and day `$v1`, 12 in `$t0`;
   57 to 18 rows. The b6 test also comes out as `sll; srl; bnez` again.
2. **Else-arm ugen temporaries (9 words).** With ugen's FIFO free list, the
   original emits the birthday term `(date << 23) >> 27` before the table
   entry `days.day[(month + 10) % 12]`; the draft the other way. The ucode
   entering uopt shows cfe already reorders the sum: an operand with an
   indirect load (ILOD) goes first, whatever the source order; with loads on
   both sides the right operand goes first (micro tests in a scratch TU). A
   register local (`date`) is a plain LOD, so the table entry always went first.
   `days.day[...] + ((*&date << 23) >> 27)` (FAKE): `*&date` is an ILOD for cfe
   and a plain read of `date` after uopt. 18 rows to 0. `*(u32 *)&date` and
   `(&date)[0]` match too; the same term on the left (`*&date` first) does not.

### Negatives (this pass)

- Return type `u8`/`s8`, girl `s32`/`u32`/`s16`/`u16`: no change or worse.
- `girl = 0` in the early exit with `return girl`; `if (b6) ... else`: worse.
- Bare `return;` in either exit: same as falling off.
- Reordering or casting the else-arm sum (`(u32)`, `(s32)`, `(u16)` on the
  entry or the shift, reassociated forms): uopt or cfe canonicalise to the same
  order, or to `(days - day) + bday`.
- Bit-field reads of the record (`.date.day`, `.date.month`) or a `GsWord`
  local: union locals are promoted like `u32`; record reads give the
  record-field family below.
- Record-field family (no `date` local, all reads `D_800E6280.unk_1BC[girl].unk_10.u`,
  `u32 day; u8 girl; u8 month;`, the T-24503 layout): the else-arm order is
  right, but the record word and month swap `$a0`/`$a1` (24 lines). The record
  word ILOD has the same hash as the b6-test ILOD (same base, offset not in the
  key), so after copy propagation it is version 1 with a late bit (79) and is
  coloured after month (bit 39). Month read through `*(u8 *)&`, a `month` local
  of other widths, compare operand order and statement order did not flip it.
- A `date` local plus a record-field read in the else arm: one load, but the
  load becomes its own CSE range (`$a3`) and `date` a copy (`move a0,a3`), frame
  0x58.
- A record pointer local (`Rec38 *rec`): 48-144 lines.
- A local for the table entry: it becomes a uopt range (`$a2`), frame 0x58.

Bodies: `body-z1.c` (18 rows, before cause 2), `body-g3.c` (record-field
family, 24 lines). The matched body is in `src/ovl/GEKO/801323E0.c`.
