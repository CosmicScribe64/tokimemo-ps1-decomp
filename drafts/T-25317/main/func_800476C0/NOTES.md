# main:func_800476C0, second pass (T-25317)

The per-frame draw loop over the display records of `D_8011ECD0` (783 instructions). Not matched; the function
stays `INCLUDE_ASM`. Best body: `body.c` here, 552 rows in `funcdiff --resolve --version c` (555 diff lines in the
T-25116 `score.py`), 781 words against 783, 8 structural hunks. The constant 0x1000 is now in `s4` and the
whole saved-register order (s0 part, s1 rec, s2 tpage, s3 j, s4 0x1000, s5 ox, s6 oy, s7 x+ox, fp y+oy) is the
original's.

## Start state (T-25116)

`drafts/T-25116/main/func_800476C0/body.c`: 649 diff lines, 780 words against 783, 14 structural hunks
(`drafts/T-23006/shape.py`). First divergence: the constant 0x1000 in `fp` instead of `s4` (row 28); then gs
gets a split piece in `s0`.

## Setup

- Both bodies need `DisplayFixActor44.unk_20` as one `s32` in `include/game.h` (replace the two `s16 unk_20` /
  `s16 unk_22` lines of that struct with `/* 0x20 */ s32 unk_20;`), as in T-25116. Not committed.
- `score.py` from `drafts/T-25116/main/func_800476C0/` scores a body in 10 s.
- `tr.sh BODY.c` (host, `TOKIMEMO_SCRATCH` set to a scratch directory that holds `r9-uopt/variants/trace`):
  traced-uopt compile of the unit with the body, prints every s-register range of the function with
  priority, compressed block count D and its units (`bb:freq/refs`, `s` = store). It takes about 2 s; use it
  instead of `rd.sh` (regdiff, 22 s) when only the colouring order matters.
- `var.py BASE.c OUTDIR 'name::old=>new;;old2=>new2'` writes text-replacement variants.

## What changed (each with the instruction evidence)

1. `s16 bx, by` with `bx + ox`, `by + oy` (was `s32 bx, by`, `ox + bx`). The original has `addu s7,a3,s5`
   (x first). IDO puts a converted `s16` operand first when the other one is `s32`, whatever the source order;
   with equal types the source order is kept (snippets `e1`..`e5`). The two words saved go to `s32 cnt`.
2. `cnt` for the part count, separate from `uv`. In the original the count is `a1` and the tpage UV value is
   `v0`, so they are different variables (with `uv` reused, both share one range).
3. `uv = part->unk_04 & 0xFF; spr.unk_0E = (uv & 0xF) * 8; spr.unk_0F = ((uv >> 4) * 8) + tpy;` (and the 0x3F
   arm). This gives the original's `lh v0,4(s0); andi t4,v0,0xff; sra; andi` (load in a colour, mask in a ugen
   temporary); `((uv & 0xFF) & 0xF)` gives a uopt CSE in `v0` instead.
4. `step = ...unk_18;` before `slot = ...unk_16;` in the second animation arm: slot gets `t0` as in the
   original (equal-priority tie, first-numbered wins).
5. `ox - part->unk_00` in the first mirror arm (was `ox - (&parts[j])->unk_00`, an autotweak find of T-25116
   that re-reads `parts` from 0x7C). The original reads `lh t5,0(s0)`. With 1..5 the tail is structurally
   identical (the `li at,-129; and` hunk is gone): `body-no-anim-reuse.c`, 845 lines, 8 hunks.
6. FAKE: `anim = (DisplayImage78 *)((s16 **)rec.unk_10)[slot]; tpy = ((s16 *)anim)[step * 2 + 1];` puts the
   sequence row in `v0` as in the original: 845 -> 595 lines.
7. FAKE: a second `if (rec.unk_1A != 0x1000 || rec.unk_1C != 0x1000) { tpx = 0; }` (dead store) in the j loop.
   The store is removed before colouring but the two compare blocks stay, so the constant has 8 use units
   (19.05, D 42) and outranks ox (18.57); the empty branch is dropped after colouring, so no instruction is
   added. 595 -> 555 lines; every position in the j loop tried gives the same 555. This is the mechanism the
   arithmetic below asked for. The original may have had a real statement here whose result is never used.
8. Lever with side effects (`body-gs-dead-branch.c`, 789 lines, 5 structural hunks): a dead branch between the
   printf and `continue` (`if (i) { tpx = 0; }`; `if (col)`, a dead switch, two arms or `if (printf(...))`
   give the same) makes the printf-path gs use need a reload, the gs piece drops to priority 0 and gs stays in
   memory as in the original (the five gs hunks go). But the count CSE `gs->count` then takes `v0` (original
   `v1`; with the PRE copy in the block right after the call block it gets `v1`), and the ugen ring moves by one
   from the tail on. A dead branch before the printf, in the loop head, at the loop end, a label, or an
   `if/else` before the printf keep the piece.

## The constant (closed by change 7)

Traced (`tr.sh`) before change 7: j 23.46 (D 26), ox and oy 19.26 (520/27), x+ox and y+oy 15.77 (410/26),
constant 14.63 (600/41). The original order is j, constant, ox, oy, x+ox, y+oy. ox and x+ox are fixed by the
instructions, and the constant's range is the whole i loop (its reload sits in the preheader, so the latch is in
range: D 41). So the constant needed 8 or 9 frequency-100 use units. Read in the uopt reconstruction
(n64decomp/ido d068e43): `constinreg` makes a live unit only for a constant that does not fit the parent's
immediate; `Uistr` stores pass 1, `Uequ`/`Uneq` 3, `Udiv`/`Umpy`/`Umod` 5. Dead stores are removed before
colouring, empty branches after it, which is why change 7 adds units and no code.

## What is left (first divergence first)

1. **gs piece in `s0`** (5 hunks: `lw s0,236(sp)` in the preheader, the original's `lw t7,236(sp)` delay-slot
   reloads and the printf block's `lw t6,236(sp); nop; lw v1,4248(t6)`). Piece units: bb4 (loop-first reload,
   moved to the preheader, no charge), bb13 (printf, PRE copy of the latch load), bb160 (latch); priority 2,
   D 5. It takes `s0` at cost 0 because part's range is only the j loop. The original has no piece: either the
   piece pays one more reload charge, or every used saved register is live in the head blocks.
2. **ugen ring and caller-saved colours** in the j loop (about 250 register-only rows from row 125 on, for
   example `andi t5,a1,0x1` against `andi t2`, `sll t2,s3,3` against `sll t9`): with the s-registers right these
   are now the bulk of the rows. Not analysed yet.
3. **Second animation arm**: the original does not fold `step * 2 * 2` (`lh a3,24(s1)`; `sll t4,a3,1;
   sll t5,t4,1`): its `step` is a coloured value (`a3`), ours is the copy-propagated `lh t2,24(s1)` with one
   `sll 2`. A `seq` variable in the arm gives the unfolded shifts (`g1`) but merges with the tail's `seq` range
   and breaks the tail (28 hunks).

## Negatives (measured, do not repeat)

- Constant uses: stores of 0x1000 never count (local struct, global, global struct, through a pointer, or an
  assignment to a multi-def local: snippets `c2`..`c4`, `n1`, `o1`..`o4`), also when the stored value is a
  propagated `s32 one = 0x1000` (frame grows 8 bytes, uses stay 6). A value compare `s.c = x != 0x1000` counts
  but costs `xor/sltu`. `/`, `*`, `%`, `<` with 0x1000 do not count.
- Frequency: `do {} while (0)`, `for (;;) { ...; break; }`, `goto` loops and `goto`- or `switch`-written
  mirror arms keep every j-loop unit at 100 (`do {} while (z)` with a local raises it but keeps the branch).
- i loop as `do`/`while`, `while` with `goto next`, or `for` with `goto next`: identical ranges.
- `s32 ox, oy`: same priorities, frame +8. Inlining `rec.x` into the arms reloads it in every arm (calls kill
  the global load), 796 words. `bx`/`by` assigned before `ox`/`oy`: same code.
- Byte masks: `rec.unk_03 &= ~0x80` is narrowed to `andi 0xff7f` in every spelling tried (`0xFFFFFF7F`,
  `(s8)~0x80`, `~0x80U`, bit-field, pointer, reordered statements); it disappeared only with change 5.
- Arm 2 index spellings on `body.c`: `(step << 1) + 1`, `step + step + 1`, byte offsets, `s16 (*)[2]`: all
  keep one `sll 2` (or change the shape).
- Experiment `experiment-loop-carried-ox.c` (not a candidate: extra printf arguments): making ox and oy live
  around the i loop drops them to 13.0 (D 40), below the constant, but x+ox and y+oy stay at 15.77 and take
  s4/s5. Making only ox/oy loop-carried is not enough.

## Next hypothesis

Start from `body.c` (constant and saved registers right). The gs piece needs its printf-path use to pay a reload
while the PRE copy of `gs->count` stays in the block right after the printf call block (that keeps the count in
`v1`). `body-gs-dead-branch.c` gets the first half; look for a block boundary that splits the piece without
moving the count's def, then rerun regdiff (`rd.sh`) and fix the ugen ring block by block from the first
temporary that is off (playbook section 2). The `autotweak` run on the 595-line body (300 s, 337 edits) found
nothing.
