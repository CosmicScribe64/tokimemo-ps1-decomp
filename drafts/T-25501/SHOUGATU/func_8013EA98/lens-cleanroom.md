# T-25501 cleanroom lens: SHOUGATU func_8013EA98

Worktree `w12-shougatu-cleanroom` (base w2-good). Start 16:20 UTC, about 150 minutes.
Not matched. New best body: `W1a.c`, 60 printed `funcdiff --resolve` lines against 84 for the
T-25046/T-25305 best (`prior/best42.c`, "42 rows"), same pipeline, 230/229 words.

## Method

For the first hour only the target asm, the callees, the data and the matched neighbours
(E6E4, F070, EE30) were read; the earlier bodies and notes were opened after that.
All rows below are the printed `-`/`+` lines of `funcdiff --resolve` on a whole-TU compile
(`q.sh`; `best42.c` gives 84 here, which the earlier notes call 42 distinct rows).

Scripts (run from the worktree root, `TOKIMEMO_SCRATCH` exported):

- `q.sh BODY...`: builds the unit with each body in place of the `INCLUDE_ASM` line under
  `$TOKIMEMO_SCRATCH/cr/`, two at a time, prints the row count. `CR_EXTRA='-Wo,-flag'` adds IDO
  driver flags (experiments only), `CR_TAG` a name suffix.
- `dis.sh NAME`: the built function without raw bytes.
- `udump.py DIR`: the ucode that enters uopt (`draft.B` of a regdiff work dir).
- `genw.py FORM OUT [BASE]`: replaces the selection loop of a base body with a loop form
  (`forms/*.loop`, placeholders SEL, REC, B2, PREV).
- `latch_census.py asm/`: census of latch blocks in the original asm (below).
- `apply.py BODY`: puts a body into `src/` under `#ifdef NON_MATCHING`.

## Clean-room reading (before the earlier work)

`v1.c` (do-while(1) with `continue`s, plain member reads) 203, `v2.c` (the rejections as the
do-while condition) 191, `v3.c` (else-if chain) 203. They reproduce the original's `>= 9` block
(`andi t9,t8,0xff; sltiu at,t9,9`), the count block without a reload and the post-loop
`lui; lbu a0` shape, which is model B of T-25305 (`n2-natural`, 203) found independently.
What they lack is the latch `.L8013EC44` and the selector in `t4`.

Other clean-room measurements:

- `v4` store through `*(u8 *)&` in the header: identical to v1 (cfe folds the view).
- `s1` store through `*(s8 *)&`: 208, breaks the store forwarding (reload in the header).
- `v5` header through `GameState *gs`: 295, but `gs->unk_10F8 + i` gives the original
  `addu t9,t7,v0` operand order (10F8 first) without the `* 1` FAKE of E6E4.
- `h1`/`h2`/`W1h`: an empty `if (D_800E6280.unk_F5F) {}` at the entry makes `tools/cvt_pass.py`
  drop the CVTs of F5F (entry rule). The global then gets a register over the whole loop,
  header included (`move t5,t8`), and the compares turn variable-first (`bne t5,a2`) where the
  original is constant-first (`bne a2,t4`). So the original's compares keep the CVT: the entry
  rule is right for this function, and no cvt_pass change is indicated.
- Frame: locals are placed top-down from the frame top in declaration order (s16 at 2-byte,
  `ShoShorts7` at 4-byte alignment). The frame size is not set by the declared scalar count:
  one, two and three scalars all gave 0x58..0x60 depending on other ranges (`abl/A1`..`A5`,
  `fa`, `fb`, `o1`, `o2`, `r1`, `r2`). The earlier bodies already have the right frame.

## New findings

1. **cfe writes the latch as a constant conditional jump.** For `do {} while (1)` and
   `while (1)`, the continue label is `lab Lcont; ldc 1; tjp Ltop` in the ucode that enters
   uopt (`udump.py`, records 183-185 of v1). So the latch is removed inside uopt, not by cfe,
   and no ucode pass between cfe and uopt sees a different shape for the original's loop.
2. **Census of the original's reload latches** (`latch_census.py asm/`): a block that holds only
   global loads and a back branch exists in two functions of the whole game. One is this function
   (`.L8013EC44`, five conditional-jump predecessors). The other is main `func_8004F6B0`, which is
   matched with IDO 5.3 (`.L8004F834: lh t0,D_80125BF2; lh v1,D_80125BF0; b .L8004F700`). Its
   ucode latch has the same shape (`lab; lab; ldc 1; tjp top`) but one predecessor, an `fjp`
   whose other arm returns. So IDO 5.3 keeps a reload latch through allocation when it has one
   conditional predecessor. The open question narrows to latches with several predecessors.
   With only one such latch in the game, a census cannot back a uniform toolchain rule.
3. **`-Wo,-createbb`** (uopt option, `docreatebb`, off by default in uoptinit.c) was untested on
   this function. Measured: best42 84 -> 45 printed lines, but the cause is a different
   alignment. createbb adds edge blocks for the b1/count reloads (`beqz v1; lui t4; b ...`), the
   rejections still go to the header and no latch appears. v1/v2/n2 are unchanged
   (203/191/203), s2-split is worse (234), q2 stays at 248. This is a negative for the loop
   shape. It would be a global flag change anyway, which needs a full 27/27 census.
4. **Nested accept form keeps the original's final-test layout and fixes the t0/t2 tie.**
   `forms/W1a.loop` on the best42 base (`W1a.c`, 60):

   ```c
   do {
       sel = (10F8 + status) % 10U + 1; status = (s16)(status + 1);
       if (rec[sel].b1 || sel < 9) {
           if (*(u8 *)&sel != 5 || (b2 >> 4) != 8) {
               if (*(u8 *)&sel != 6 || (b2 >> 4) != 9) {
                   if (prev != *(u8 *)&sel) {
                       if (rec[sel].b1) break;
                       if (count < 3) break;
                   }
               }
           }
       }
   } while (1);
   ```

   Compared with best42, `D_80146054` is now `t2` and 9 is `t0` (the tie T-25143 reported is
   gone), and the post-loop `beq t2,v0` is right. Left (regdiff `-v` on W1a):
   - status `v1` / b1 `v0` (target reversed): b1 expression priority 7.5 against status 6.2;
   - selector piece `t3` (bit 24/lr 151, 1.8) against constant 6 `t4` (1.571), target reversed;
   - the `>= 9` block is in the selector range (`move t3,t8`, `move t3,t9`);
   - no latch: the count block holds the reload (`beqz v0; lui t3; b; lbu t3`);
   - post-loop `lui a0` (target `lui v1`) and `bne t0,v0` (target `bne v0,t0`).
   The same form with plain member reads (`W1.c`..`W4.c`, n2 base) is 187.
   Negatives on the W1a base: `||` final test `W1c` 98, `>= 9` hole `9 + b1 * 0` `W1d` 82, latch
   increment `W1e` 174, alias forced store `W1f` 162 (with the hole, `W1g` 82). The never-true
   latch break `(u32)F5F < 0` (`L1`/`L2`) gives 102: it leaves a `b header` latch with no load,
   because uopt removed the test.
   Never-true tests at the latch position fold before allocation and leave no latch:
   `(u32)count < 0` break `W1k` 74, store arm `W1m` 74; `status < 0` (u32) break `W1s` 58, store
   arm `W1n` 58. W1s/W1n reach 58 only by moving priorities: status/b1 come out right, but the
   base address and the 5/8/9/D_80146054 constants shift by one register. They are FAKEs with no
   code of their own, so W1a stays the base.
   `tools/autotweak.py` on W1a (420 s, 424 edits, all cheat-sheet kinds) found nothing (60 -> 60).

5. **The latch exists through uopt; only bb6 is in the way (corrects T-25305 section 2).**
   `latch_exp.py` (LATCH_MODE=none, LATCH_DUMP) copies the ucode that leaves uopt. For v1 it
   holds `lab 0x31; ujp 0x30` (the latch, reached by the five `ujp 0x31` continues), and for W1a
   it holds `lab 0x3b; ujp 0x30` (reached by five conditional jumps). So the empty latch is a
   block during allocation. ugen/as1 thread it later only because it is still empty. IDO 5.3
   puts no reload there because the latch is not a needs-reload unit of the selector range.
   Trace of the selector variable (bit 24) on the W1 form:
   - `W1a`: units {bb5 `>= 9` load, bb7 load, bb10, bb13, bb16 count block load, bb15 exit
     load}, split piece priority 1.8, coloured t3. The `>= 9` block is a member, so the latch's
     predecessors all hold the value and the latch needs no reload.
   - `W1d` (hole `9 + b1 * 0` in the `>= 9` block): {bb18 **latch load=1**, bb16 load, bb15 load,
     bb6 load, bb7 load, bb10, bb13}, -1.5. The latch reload and the bb7 reload are the
     original's two reloads.
   - `F3` (W1d plus a header `flag` local for the final b1 test): loop piece lr 160 = {bb18
     latch load, bb6 load, bb7 load, bb10, bb13}, priority exactly 0, uncoloured. This is T-25305's
     r3 shape, now in a form whose final test and latch layout are the original's. Without the
     bb6 unit the piece nets +10 and would be coloured with the original's reload placement.
   - `F4` (F3 with the first test inverted: an empty then-arm for the rejection, the rest in the
     else-arm, so bb5's edge to the `== 5` block is the taken `fjp` and the fall-through goes to
     the rejection): units {bb6 load, bb7 load, bb10, bb13, bb23, bb24}, priority 2, 210 rows. The
     latch has left the range, and the zero-reference unit bb6 takes the reload. So on either
     layout one zero-reference unit next to the `>= 9` block carries a reload that the original
     does not have. Whether bb6 is an empty graph node or the node numbering of a forward label
     (uoptinput.c numbers a label's node at its first reference) is not settled. The bb numbers
     7/10/13 of the three tests, with 9 and 12 as zero-reference numbers between them, fit
     either reading.
   - **`F5` reproduces the original's loop shape with stock IDO 5.3** (diagnostic, 278 rows
     because of the frame and register shifts). It is F3 plus one more selector use in the
     D_80146054 test (`&& *(u8 *)&SEL != 11`, always true, FAKE), which lifts the loop piece
     above 0 so it is coloured. Final code: the `>= 9` block forwarded
     (`andi t9,t8,0xff; sltiu at,t9,9; beqz at,latch`), `lui a2` hoisted into the header after
     `break 7`, `lbu a2,F5F` at the `== 5` block, the final test `bnez v1,exit; slti at,a1,3;
     bnez at,exit`, and the latch `lui a2; lbu a2,F5F; b header`. That is the target's
     `.L8013EC44` with another register. uopt's output (`F5u.out.u`) shows bb6 is a real node: it
     holds an `rlod` between the `fjp` of the `>= 9` block and the `lab` of the `== 5` block. ugen
     emits that rlod as a dead `andi t7,t8,0xff` (forwarded store value), one instruction the
     target lacks. So no compiler change is needed for the latch. The costs are the bb6 unit and
     the FAKEs that shape the range: hole, flag local (frame 0x68, saved s0) and the extra use.
   - Steps from F5 toward the frame (all on the best42 base): `F6` (flag in `i`, s16) 271:
     frame 0x60, and the s16 flag adds `sll/sra; move`. `F7` (F3 with the flag in `i`) 173.
     `F9` (counter `i` with `i++`, flag in the u32 `status`, hole) 205: the frame is 0x58 again
     and b1 is in v1 with the counter in v0, but the sum is `addu t9,v0,t7` (i first; E6E4 needs
     the `* 1` FAKE for this), the flag gets its own `a0` copy and the selector range is
     uncoloured. `F11` (F9 plus a truth-test use `&& *(u8 *)&SEL` instead of `!= 11`) 254, but
     with frame 0x58 and the target's whole loop shape: the hoisted `lui a2`, `lbu a2` at
     `== 5`, the final test, and the latch `lui a2; lbu a2; b header`. Its extra instructions:
     the FAKE truth test (`beqz a2,latch`), the dead bb6 `andi t7,t8,0xff`, and the flag copy
     `move a0,t6` (the original keeps b1 in v1 only, so its final test reads the header's b1
     expression and not a flag local). Its registers are shifted (0x38 in ra), and the sum is
     still i first. Spelling the sum `(i + 10F8)` (`F12`) leaves 205 as is; `10F8 * 1 + i`
     (`F13`) gives 263.
     `F10` (F9 plus `!= 11`) was generated but not measured. The extra constant 11 of F5/F6
     takes a register and pushes 0x38 into s0.
   - Ucode rewrites before uopt cannot help. Replacing the latch's `ldc 1; tjp` with `ujp`, or
     adding a `nop` or `loc` record (`latch_exp.py` modes ujp/nop/loc, `qx.sh`), changes nothing
     (v1 203, W1a 60). cfe's input has nothing between bb5's `fjp` and bb7's `lab`, so bb6 is
     created inside uopt.

## Mechanism (what still differs)

The original's selector is a split piece of the F5F variable range, {== 5 block, == 6 block,
D_80146054 block, latch}. It reloads at the start of the `== 5` block and of the latch (as1 hoists
the first `lui t4` into the header). The `>= 9` block and the header stay outside it, as forwarded
memory accesses. IDO 5.3 does keep the latch block through allocation (finding 5). The one
difference left is bb6, a zero-reference unit between the `>= 9` block and the `== 5` block.
T-25305 reads it as the empty block on that critical edge (identity not confirmed, finding 5). It
is a live, reached member of the range that needs its own reload, and that makes the loop piece
worth exactly 0. The original either had no such block or did not count it.
No source form removes the edge: the `== 5` block is reached from the header (b1 set) and from
the `>= 9` block, in the original as well. Two FAKEs set up the rest of the range: the hole in
the `>= 9` block and the header flag. The census shows that only this function exercises the
difference.

## Proposal

1. Take `W1a.c` (60) as the base for the synthesis: it is the first body with the original's
   final-test layout and the t0/t2 allocation together. `forms/F3.loop` is the diagnostic that
   isolates bb6, and `forms/F5.loop` shows the complete target loop shape under stock IDO. The
   next search is between F5 and W1a. Keep F5's loop range (latch and `== 5` reloads). Remove
   the flag local's frame word and saved register; T-25046 found that reusing an existing local
   changes the frame least. Stop the range from growing into the post-loop code and drop the
   dead `andi` from bb6. Check with the bit-24 units after each step. Work on the selector range with the T-25305 `pri.sh`/T-25143 `units.sh`
   scripts or this folder's trace greps (`R9SAVE bit=24`).
2. Find where uopt creates bb6. I read the n64decomp/ido reconstruction at d068e43 (WebFetch
   summaries, not a full read). No empty node on a fall-through edge is created by uoptinput.c:
   a `lab` after an `fjp` reuses or creates the label's node, and no empty block is left behind.
   The same holds for uoptcontrolflow.c (only unreachable-code removal and `Unop` nodes around
   interprocedural calls), uoptloop.c (intervals and loop marks only), uoptcm.c (insert/delete
   bit vectors only), uoptppss.c (consecutive labels are merged) and uoptreg1.c (live units
   only). uopttail.c makes nodes only for tail recursion. Still to read: uoptkill.c,
   uoptcopy.c, uoptreg2.c `split`/`createbb` paths and uoptemit.c. First check whether "bb6" is
   a real graph node or a unit on an edge (dump the graph with `-Wo,-zdbug` options; T-9800 has
   the trace build). `-createbb` is ruled out: it is
   off by default, and turning it on adds more edge blocks. If one uniform switch drops empty
   critical-edge blocks from register live ranges, measure it as an experiment: a patched recomp
   uopt (tools/uopt_patch.py style) over the 6336 matched functions as a regression census, plus
   F3 without its FAKEs. With one known occurrence in the game it cannot meet CODING_STANDARDS
   7a's "evidence-backed over a broad sample" alone. A zero-regression census is the evidence
   it would need, and the decision belongs to the user (compiler patch, licensing rule).
3. If neither closes it, keep `INCLUDE_ASM`. W1a is near the 30-row guard limit but uses the
   byte views and the `D_800E71DF` alias of best42, so it needs the keep-line proof before it can
   be a guarded draft.
