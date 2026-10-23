# main:func_800476C0, finish gate round 1, lens "structure" (T-25606)

**Matched.** `src/main/80047550.c` now holds the function as plain C (no `INCLUDE_ASM`), `funcdiff --resolve`
prints `MATCH (linked function bytes equal target (3132 bytes))`, the clean build is 27/27, and
`check_headers.py`, `migrate_globals.py --check` and `sync_protos.py --check-branch` pass. Progress: main
827/830, grand total 6945/6952. `body.c` in this folder is the same body.

Start: `drafts/T-25317/main/func_800476C0/body.c`, 552 rows (`funcdiff --resolve --version c`), 555 diff lines in
`score.py`. Steps: 552 rows -> 522 rows (523 lines) -> 347 lines -> 7 lines (3 words) -> match. About 95
minutes.

## Header changes that ship with the match

- `include/game.h`: `DisplayFixActor44.unk_20` is one `s32` (was `s16 unk_20; s16 unk_22`). The function copies
  it with `lw 0x20` / `sw`. The clean build shows that no other user changes.
- `include/main_api.h`: `u8 func_8004901C(void);` (it is defined later in the same file and returns `u8`; the
  other caller, `src/main/8004C3F0.c`, still matches with this prototype).

## What changed (with evidence)

1. **Part address as an `s16` index** (552 -> 522 rows):
   `part = (DisplayImagePart0E *)((s16 *)parts + j * 7);` plus FAKE `if (part) {}` after
   `spr.unk_00 = rec.unk_38;`. The original computes `sll t2,s3,3; subu t2,t2,s3` (j * 7), loads parts
   (`lw t3,124(sp)`), then `sll t4,t2,1; addu s0,t3,t4`: an array step of 2 on index j * 7, base loaded between
   index and scale. `&parts[j]` gives one `mul 14` and the sum the other way round. Without a use of `part` in
   the join block, uopt moves the sum into both mirror arms and keeps j * 7 as a `$v0` CSE (735 lines).
   Measured on T-25317's gs-dead-branch variant, whose ugen ring before this block is the original's.
2. **`s16 uv`** (523 -> 347 lines). The original spends two hidden ugen temporaries at the join after each tpage
   switch (found with `tfifo.py --offsets`). As an `s16` local, `uv` is sign-extended through two temporaries
   (`sll 16; sra 16`) that as1 deletes. The same change stops as1 folding the arm-2 shifts (the original's
   `sll t4,a3,1; sll t5,t4,1`, T-25317's open item 3).
3. **Rotate tested in `spr`** (347 -> 7 lines): `if (spr.unk_20 != 0 || spr.unk_1C != 0x1000 ...)`. The
   original loads the rotate into a temporary (`lw t2,32(s1)`), stores it, and tests the same register in the
   next block, with one hidden temporary. That is a stack reload of `spr.unk_20` that as1 forwards from the
   store (replayed on our binasm with `replay.py`, then written in C).
4. **gs and the count** (7 -> 0): T-25317's dead branch after the printf is gone. Instead the printf path ends
   at a label, `print_done: continue;`, and the main path ends with FAKE `if ((u32)i < 0) goto print_done;`.
   The condition is always false. uopt keeps the edge while it allocates registers, and ugen folds it away, so
   no instruction is added. The extra edge into the block after the printf call means gs's loop piece must
   reload there. The piece then has no profit, so gs stays in memory (`lw t6,236(sp)` in the printf block, as
   in the original). The PRE copy of `gs->count` stays in the block right after the call, and uopt keeps a range
   with a unit there off `$v0`, so the count gets `$v1` as in the original.

All five FAKE markers in the function are still needed. Removing any of them (`if (part) {}`, T-25317's second
scale test, T-25116's empty arm, T-25317's anim reuse, or the goto) costs 69 to 885 lines.

## Tools in this folder (Docker unless noted)

- `score.py BODY OUT [...]`: the T-25116 scorer, about 17 s. It needs `src/main/80047550.c` with the plain
  `INCLUDE_ASM` line, because `permute.build_base` preprocesses with `-DNON_MATCHING`: with a guarded draft in
  `src/`, the draft is compiled and every variant scores the same (this cost 15 minutes here).
  `run2.sh NAME...` (host) scores `build/s/v/NAME.c` in two jobs. `mk.py BASE OUTDIR SPEC.py` writes variants.
- `ledger.py OUT [-v N]` (host): region ledger. Regions are split at every `jal` and at the loop and arm heads.
  For each region it counts rows, exact, register-only and structural; `-v N` shows a region side by side.
- `gdump.py BODY OUT.txt`: ugen's binasm (the as1 input) as text, plus the `.G`/`.T` files.
  `replay.py PREFIX EDITS OUT`: runs frame_pass and the real as1 on an edited copy of that binasm (`del`, `mv`,
  `set`, `dup`), so you can see what as1 makes of a given ugen output.
- `udump.py BODY OUT.txt`: ugen's input ucode. `lst.py`: `cc -S` listing (ugen text before as1's peepholes).
- `fifo.py DUMP.txt [-v]` (host): replays ugen's temporary choices against a first-in first-out free list:
  t6 t7 t8 t9 t2 t3 t4 t5, with t0 and t1 excluded because they are uopt colours in this function. A def takes
  the head; a source temporary that is not read again before the next label goes to the tail, reg2 before reg3.
  It had 0 mismatches on every body tried (about 230 temporaries each).
  `tfifo.py DUMP.txt OUT [--offsets]`: maps each def to the original's register and prints the ring offset
  wherever it changes. That located the three "+2" places of steps 2 and 3 to a single statement each.
- `tr.sh BODY OUT.log` (host, `TOKIMEMO_SCRATCH` with `r9-uopt/variants/trace`, for example
  `.../opencode/scratch-T-25308`) and `trall.py LOG [BB...]`: traced uopt, every range with colour, priority and
  units.

## as1 and uopt facts measured here

- as1 folds `x << 1` then `<< 1` into `<< 2` when the middle register is dead after the second shift. If the
  first source is dead too, it retargets the load (`lh a3; sll t4,a3,1; sll t5,t4,1` became
  `lh t4; sll t5,t4,2`). A live middle register, or a block boundary between the shifts, keeps both shifts.
- as1 deletes `sll 16; sra 16` after an `lh` (and retargets the load when the loaded register is otherwise dead).
- as1 turns `lw $10,..; move $2,$10` into `lw t2` with every use of `$2` renamed, across a block boundary, and
  forwards a stack reload of a word just stored (`sw $10,216($sp) ... lw $11,216($sp)`) across a branch.
- `cc -S` listings show ugen's code before these peepholes, so they can mislead about the final shape.
- uopt ends a block at every call (the arguments and `jal` in one block, the code after it in the next). A range
  with a unit in the block after a call is kept off `$v0`. A loop piece whose unit has `load=1` (an incoming
  edge from outside the piece) gains nothing there. The formula in [[uopt-register-allocation]] reproduces
  priorities 2 and 0 for the two gs pieces exactly.
- An empty test `if (count) {}` with a loop-invariant pointer is constant-propagated to the absolute global, so
  it is not a use of the CSE.

## Negatives (measured, do not repeat)

- Narrow types: `s16` tpx, cnt; `u16`/`u8` tpx, tpy, uv; `s16` step, slot, j: worse or equal.
- Rotate as `*(s32 *)&rec.unk_20`: 865 lines. A local for the rotate: no change.
- Dead branches after the printf (any form: `if (i) {}`, `if (gs) {}`, `if (printf()) {}`, `if (count) {}`,
  dead switch...): gs piece gone but the count copy moves to the join block and takes `$v0` (7 lines).
- Empty tests elsewhere (loop head, before the unk_34 test, start of the else arm, after `parts`, loop end,
  before the loop): the gs piece stays (789 to 833).
- `&gs` taken: count `$v1` and gs in memory, but the entry test then reloads gs instead of the absolute load, and
  the parts spill home moves (23 lines). If/else instead of `continue`: no PRE copy. Nested head ifs: same as
  the && chain. Early `continue`s: 875. An unconditional `goto print_done` (o3): 791. `if (i < 0) goto`: 43.
- `autotweak` (600 s, 644 edits) on the 7-line body: nothing.

## Proposal

Done for this function: review the five FAKE markers (two from this round) and take the match. The method is
worth keeping for the last functions: region ledger, then `fifo.py`/`tfifo.py` to place every ugen ring offset
on one statement, then `replay.py` to check a guessed ugen shape against as1 before writing C.
