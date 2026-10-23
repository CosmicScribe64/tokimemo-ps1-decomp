# T-25120: GYOZI func_80138C9C (matched)

Result: default C matches, 992 bytes (249 words). The body is in `src/ovl/GYOZI/80138B50.c`.

## Start state

T-24707 guard, 75 resolved rows (`funcdiff --version c`), unchanged by the volatile `D_80147240`
that the func_8013907C match added earlier in this package. Two problems:

1. The two `D_80147240 == 1` tests in the gather loop: `bne s4,t6` built, `bne t6,s4` original
   (4 rows).
2. The rotation offset `r = D_800F6468 % count`: the original computes it right after the "\n"
   print, before the rotation loop guard, in `$v1`, and the guard `blez` reuses the divisor
   register. The draft computed it after the guard, in `$v0` (71 rows, mostly the ugen
   temporaries shifted by the extra `lui`/`lh` of the guard).

## What closed it

1. **Rotation offset before the guard.** The ugen listing (`cc -S`) shows uopt sinking `remu`
   into the preheader. The uopt trace has `ucvtl(r's expression)` inside the loop: global copy
   propagation replaces `r` in `i + r` by its expression and the loop-invariant expression is then
   hoisted into the preheader, after the guard. uoptcopy.c `find_replacements` propagates only an
   assignment that stays available on the back edge, so an operand of the expression must be
   altered in the loop. The count is altered when the values store and the count share one record:
   `#define CL (*(CandList *)D_80147224)` with `values[12]` and `count` at +0x18 (FAKE). With the
   view everywhere: 68 rows; the division moves before the guard and the guard reuses `t7`. The
   reorder loop then reloads the count after its first store (`lh t4,24(t4)`), where the original
   keeps the address `values + count*2` across the store (`lh t2,-2(v0)`, `sh zero,-2(v0)`), so the
   reorder loop keeps the separate symbols: 12 rows. A view based at `D_80147220` (count at +0x1C)
   moves the `%lo` offsets into the immediates (80 rows), and a view in the rotation stage only
   promotes the count into `$v1` (148 rows).
2. **`$v1` for the offset.** `s32 func_800BCDF0();` (per-file `MAIN_API_OVERRIDE_func_800BCDF0`, as in
   80140C90.c; this file has no other caller): 12 rows to 4. The draft's remainder and offset then
   avoid `$v0` after the print call, as in the original.
3. **Load first in `D_80147240 == 1`.** Cause: tools/cvt_pass.py. Every ordering of a narrow unsigned
   global against a register constant ends constant first in the pass's model: with the widening
   `CVT` kept, uopt's input rule (uoptinput.c "swap == and != if the left side isn't a var") swaps
   `cvt(D) == 1` to `1 == cvt(D)`; with the widening stripped (an entry-stored global compared),
   `_constant_first` swaps `LOD D; LOD Rconst; EQU` after uopt. cfe always puts a literal on the
   right. The way out is a widened load (kept `CVT`) on the right of a non-variable left operand:
   uopt then swaps it to the left, and `_constant_first` does not fire because the record before the
   constant is the `CVT`, not a `LOD`. Two FAKEs:
   - `CL.special = 0;` at the entry (byte at +0x1C of the view): the entry store by name put
     `D_80147240` in the pass's entry set, which strips the widening. Through the view it is an
     `ISTR`, so the location is not an entry location and the `CVT` stays.
   - `r = 1;` before the gather loop and `r * 1 == D_80147240` in both tests: `r * 1` is an
     operation at uopt's input (cfe keeps `* 1`), uopt swaps the operands, and copy propagation turns
     `r * 1` into the hoisted constant `li s4,1`. `r` has its own slot already, so the frame stays
     0x68.
   Result: MATCH (compared NON_MATCHING C, then default build).

## Negatives measured (this session)

- `1 == D_80147240`, `(int)`, `(unsigned)`, `(u16)`, `(short)`, `(u8)`, `(long)` casts, double casts
  `(u32)(s32)`, `(s32)(u32)`, `G - 1 == 0` (constant not hoisted: `li at,1`), `G + 0 == 1`,
  `G == 1 + 0`, `(G | 0) == 1`, a local `one` (constant-propagated; frame +4), a CL view member read
  (an `ILOD` at uopt input, swapped): all constant first. An `s32` local copy `(t = G) == 1` gives
  the load first but in a uopt register (`lbu v0`), not a ugen temporary.
- `if (r) {}` after the offset: division before the guard, but the guard reloads the count in a new
  block and the offset stays in `$v0` (132 rows).
- A work pointer `w->unk_10C8` for `D_800F6468`: propagated anyway (the address is constant), frame
  +8 (85 rows).
- Removing the T-24707 `if (i) {}` FAKEs on the final body: 25 rows; they stay.

Tests of the pass behaviour used mini files compiled through the real pipeline with dumps of the
cfe output, the uopt input after cvt_pass and the uopt output (shim scripts in the worktree's
`build/scratch/t1/`, not committed).
