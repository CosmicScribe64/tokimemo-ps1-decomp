# T-25305: SHOUGATU func_8013EA98 (second pass, wave 10)

## Start state

- First pass T-25143: no match. Best body is the T-25046 36-cost body (42 distinct `funcdiff --resolve`
  rows, 84 printed, 230/230 words, frame 0x58), kept here as `func_8013EA98.c` (unchanged).
  `src/ovl/SHOUGATU/8013E330.c` holds plain `INCLUDE_ASM` (42 rows is over the guard limit).
- First divergence (unchanged, same as T-25143): the girl-selection loop. The original has no
  selector copy in the header, a `>= 9` block that reads the stored byte through ugen forwarding
  (`andi t9,t8,0xff`), a latch `.L8013EC44` (`lui t4; lbu t4,F5F; b header`) that all five
  rejections reach, no reload in the count block, and `lui v1; lbu a0,F5F(v1)` after the loop.

## What was measured (whole-TU, `build/ea98/m.py`; trace with `units.sh` and `pri.sh`)

### 1. The natural spelling fixes three of the four places (model B)

`diag/n2-natural.c`: the base body with every loop read of the selector written as the plain
member `D_800E6280.unk_F5F` (no `*(u8 *)&` views; the `D_80146054` view kept). F5F is first
touched after the count loop's branch, so `tools/cvt_pass.py` keeps the `CVT` on these compares.
uopt then puts the selector into the CSE temporary of `cvt(lod F5F)` (unit bb7 def, bb10, bb13)
and leaves the variable F5F uncoloured (priority -1.8). Result:

- the `>= 9` block is exactly the original's (`andi t9,t8,0xff; sltiu at,t9,9; beqz`), no
  `move t4,t8` in the header;
- no reload in the count block;
- after the loop, `lui tX; lbu a0,F5F(tX)` with a separate hi register (t6 here, v1 in the
  original), the original's shape. The base gives `lui a0; lbu a0` (rlod into the a0 piece).

What stays wrong: no latch (227 words) and the temporary has priority 13.3, so it takes v0
and every loop register shifts (203 rows). The original's selector is t4, the eleventh register,
which means its range is cheaper: it pays a reload at the latch. The register order needed is
status v0, b1 v1, count a0, 10 a1, 5 a2, 8 a3, 9 t0, &D_800E6280 t1, D_80146054 t2, 6 t3,
selector t4, 0x38 t5 (`pri.sh n2` lists the ranges and priorities).

A CSE temporary can never be live at the latch (bb7 defines it before any use on every path), so
the latch reload in the original is the variable F5F's register range. That requires plain
variable uses in bb7/bb10/bb13 (the byte views of the base, model A).

### 2. The latch only survives with code in it at CFG time

uopt threads an empty continue/latch block before register allocation: `while (1)` / `do ...
while (1)` / `for (;;)` with `continue` and a final `if (b1 || count < 3) break;` (`c3`, `c4`,
`f1`, `l2`, model A `h1`) all lose the latch and invert the final test (`beqz at,header`). A dead
counter in the latch (`j++`, `d1`, `d2`) or a self-assignment (`status = status`,
`count = count`, `status += 0`, `F5F = F5F`: `pa`..`pd`, all 110 rows) is removed first. With
live code in the latch the block survives (`(i++, 1)`: r1, l3; `&& one` with a local `one = 1`:
`diag/k2-latch-test.c`, which keeps a `bnez a1` test there). No spelling found leaves the latch
with no code of its own.

Closest natural form: `} while (D_800E6280.unk_F5F);` after the continue/break body (`q2`, a
truth test, so no `CVT`; F5F is 1..10 there, so the test is always true). It keeps a latch that all
rejections reach, and the final test has the original's layout (`bnez v1,exit; slti; bnez at,exit`,
then the latch). The latch is `lui t6; lbu t6,F5F; nop; bnez t6,header` (230 words, 248 rows) where
the original has `lui t4; lbu t4,F5F; b header`. With byte views in the compares (model A, `wrBG`,
104 rows) the latch read becomes a load unit of the F5F range, and the range is coloured whole
(priority 3.5, a1). But the `>= 9` block is in the range again, and the uopt output puts the
latch's reload in a separate block that jumps to the test. Folded or no latch: `F5F == F5F`
(`q1`), `1 || F5F` (`o3`). Still a test: `F5F || 1` (`o1`, `o2`), `F5F >= 0` (`o4`). The increment in the latch never moves out completely: PRE lifts the
`addiu` into the header, the `(s16)` truncation (`sll/sra`) stays (`(s32)(x + 1) << 16 >> 16`
too, `e5`); an s32 counter is strength-reduced with the random word (`e6`).

### 3. Reload placement (corrects T-25143)

`docreatebb` is off by default (uoptinit.c), so `allpreppout` is false whenever a non-member
predecessor ends in `fjp`/`tjp`. A reload at a join therefore stays at the block start as soon as
one non-member predecessor ends in a conditional jump. That is the case for the latch (the
`>= 9` block jumps to it) and for L_EBE8 (the header jumps to it). The T-25143 concern that the
latch reload would move to an edge does not apply. as1 then hoists the `lui` of L_EBE8's reload
into the header (seen in `s2`: `lui s0` before `mfhi`), so the original's header `lui t4` is a
consequence of the allocation, not a source feature.

### 4. Why the loop range cannot be coloured as one piece: the edge block bb6

uopt puts an empty block (bb6) between the `>= 9` block's `tjp` and the `lab` of the `== 5`
test. bb6 is always in F5F's live blocks (live: bb7 reads first; reached: around the back edge).
With the `>= 9` block a hole (needed for its forwarded read) bb6 needs its own reload, and the loop
part of the range nets exactly 0: latch -10, bb6 -10, bb7 +10-10, bb10 +10, bb13 +10 (`r3`,
`r3n`: "not colored (-ve save)"). Post-call reads of F5F net 0 each (one load, one reload).

The only way to drop bb6 is a split that starts at bb7: `split()` takes the first unit whose
first access is a store, otherwise the first unit with references, and grows along successors
only, so bb6 (a predecessor) stays out. `diag/s2-split.c` shows it. FAKEs: hole in the `>= 9`
block (`9 + b1 * 0`), latch `(i++, 1)`, `flag` local for the final test, forced store through the
`D_800E71DF` alias (so the range has no first-store unit), natural post-call reads. The whole range
has priority 0.33. It crosses the func_80051A68 call, so it is split. The piece from bb7
{7, 10, 13, latch, 23} gets priority 2.4. The loop comes out with the original's allocation
pattern: `lui` hoisted into the header, `lbu` at L_EBE8, one register for the three compares,
latch `lui/lbu` + `b`. But the piece grows through the loop exit into the post-loop block bb23,
so it takes s0 and the frame grows by 16 (219 rows). With an extra hole in the exit block to stop
the growth (`s3`), the whole range drops to priority 0 and nothing is coloured.

## Negatives added (whole-TU rows; base 42 distinct / 84 printed)

- Call argument `func_80051A68(D_80146058)` or plain `D_800E6280.unk_F5F`: 80 printed (234 words).
- Byte view at the `>= 9` read (`a1`): identical to the base (byte views and plain reads form the
  same F5F unit).
- `D_80146054` spellings (`y1`..`y4`): no change, except a plain read in the loop compare (`y2`,
  168).
- Byte-view forced store (`s1`): identical to `r3n` (uopt folds `istr(lda)` to `str`).
- Mixed symbols in the loop (`m1`: store and `>= 9` through `D_800E71DF`; `m2`: compares through
  `D_800E71DF`): the struct read is hoisted as loop-invariant (241 / 194 rows).
- Natural-spelling loop forms (`l1` `(status, 1)` 197, `l2` continue 191, `l3` latch increment
  197, `l4` for-increment 191, `b1` `if (!(...)) break;` 187, `f1` 197, `c3` 191, `c4` 197): all
  227 words, no latch.

## Rows now

Unchanged: best is still `func_8013EA98.c` (42 distinct rows). Source stays `INCLUDE_ASM`.

## First divergence left

Same place as before: the latch block and the selector's range. The full mechanism is now known
(selector variable range = piece split from bb7, latch reload at a join with a conditional-jump
predecessor). What resists is the source. IDO 5.3 threads an empty latch before allocation, and
the edge block bb6 makes the loop part of the range worth exactly 0. With natural source, the
original compiler either had no empty edge blocks or threaded jumps after allocation.

## Next hypothesis

Build on `diag/n2-natural.c` (natural spelling, model B), not on the byte-view base. It already
has the original's `>= 9` block, count block and exit. Two pieces are missing, the latch and t4
for the selector. Look for one construct that does both: an F5F reference in the latch that
exists at CFG time and whose branch is gone after allocation. Start from `q2` (`while (F5F)`
gives the latch, the final-test layout and the F5F read there) and look for a condition on F5F
that uopt keeps through allocation but turns into an unconditional jump. Check each candidate
with `pri.sh`: the latch must be a unit with `load=1` in the selector's range, and the `>= 9` block
must not be. If nothing natural exists, the s2
mechanism is the fallback, provided the split piece is kept out of the post-loop code. That
fallback needs a keep line with a proof for `D_800E71DF` (config/migrate_globals.txt) and FAKEs
for the hole and the latch.

## Files

- `func_8013EA98.c`: best body (T-25046, 42 rows).
- `diag/n2-natural.c` (203 rows, model B), `diag/s2-split.c` (219 rows, split mechanism),
  `diag/c3-break.c` (191 rows, break form), `diag/k2-latch-test.c` (112 rows, latch kept by a
  live test), `diag/q2-truth-latch.c` (248 rows, `while (F5F)` latch; `diag/q2.loop` is its loop
  in `gen.py` placeholder form).
- `scripts/`: `gen.py` (n2 body with the loop replaced, placeholders G/REC/PREV/BASE),
  `gen2.py` (base body, plus BG for the byte view), `pri.sh LABEL` (every live range with
  priority and units: L loads, S stores, r reload, s save), `show.sh LABEL [from to]` (draft
  instructions). They use the T-25143 `m.py`, `tr.py`, `units.sh`, `run2.sh` and the
  `uopt-trace` binary from `drafts/T-25143/SHOUGATU/func_8013EA98/NOTES.md`, all under
  `build/ea98/`.
