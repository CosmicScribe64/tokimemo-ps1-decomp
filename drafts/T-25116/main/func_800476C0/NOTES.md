# main:func_800476C0 (T-25116)

The per-frame draw loop over the 160 display records of `D_8011ECD0` (3,132 bytes, 783 instructions). No match. The function stays `INCLUDE_ASM`; the best body is `body.c` in this folder.

## Start and result

- Start: `INCLUDE_ASM`; best known body `drafts/T-23006/main/func_800476C0/body.c`. Measured here at 1,191 diff lines (`score.py`), 816 built words against 783.
- Now: `body.c` has 649 diff lines (score 625) and 780 words against 783. The control flow, frame, stack homes and every block shape are the original's, except three places listed under "What is left". The other rows are register numbers.
- `body.c` needs `u8 func_8004901C(void);` before it. `score.py` adds that line. It also needs `DisplayFixActor44.unk_20` as one `s32` (the rotate word, `lw 0x20(s1)`). That header edit is not committed (the review kept `include/game.h` to what the committed C needs): in `include/game.h` replace the two lines `/* 0x20 */ s16 unk_20;` and `/* 0x22 */ s16 unk_22;` of `DisplayFixActor44` with `/* 0x20 */ s32 unk_20;` before running `score.py`. Nothing else uses `unk_20` or `unk_22` of that type (checked at the T-25116 build).

## Tools (Docker, repo root)

- `python3 drafts/T-25116/main/func_800476C0/score.py BODY.c OUTPREFIX [BODY2 OUT2 ...]`: compiles the unit with the other bodies stripped (`permute.build_base`), takes about 17 s against about 100 s for `funcdiff`, and prints the same diff-line count as `funcdiff --resolve --version c`. Write OUTPREFIX under `build/`, because the `.want` file is original disassembly.
- `sh drafts/T-25116/main/func_800476C0/rd.sh BODY.c` (host, `TOKIMEMO_SCRATCH` set): regdiff with the traced uopt, which prints every live range with its priority. The traced uopt is not in the repo. Build it once in a scratch directory with the commands in `wiki/uopt-register-allocation.md` (clone ido-static-recomp at 9c242adc, apply `tools/ido-ecvt.patch`, `make setup`, build uopt, `tools/uopt_patch.py trace`, gcc). The build takes about 7 minutes. Raw `R9SAVE`/`R9UNIT` lines are in `build/scratch/regdiff/func_800476C0/draft.log`.
- `drafts/T-23006/shape.py OUTPREFIX` masks registers and shows only the structural hunks.

## What changed, in order (diff lines after each step)

1. `tpx`/`tpy` are computed once after each `switch (attr & 0x03000000)` (two cases, no default), not in every arm. uopt copies the join code into the arms and reads `tpage` (s2) itself, as the original does: 1,137.
2. The record is the global array `((DisplayFixActor44 *)D_8011ECD0)[i]`, not a `rec` pointer. Stores to `spr` (an address-taken local) kill CSEs of pointer loads but not of loads from a named global. With this change `unk_03`, `unk_0C`, `unk_1A`, `unk_1C` and `unk_38` stay in registers across the sprite stores, as in the original. uopt strength-reduces `i` to s1: 1,117. Snippets: `snippets/c1.c` (the pointer form reloads `lbu 3(s0)`) and `snippets/c2.c` (the array form keeps `v0`), compiled with `snippets/snip.sh`.
3. Rotate is a word field: `spr.unk_20 = rec.unk_20` with `s32 unk_20` in the header. `*(s32 *)&rec.unk_20` forms a second strength-reduced address and spills two stack temporaries (frame 0x108): 929.
4. `parts = anim->part;` before the count test, with `part = &parts[j]` in the loop. The original keeps `anim + 8` in a uopt spill home at 0x7C and adds `j * 14`. `&anim->part[j]` computes `(anim + j*14) + 8` instead. The original has no `rot` or `attr` locals: `rec.unk_20` and `rec.unk_38` are CSEs. With `parts` inside the 11-word block, every stack offset matches (gs 0xEC, spr 0xC8, step 0xC4, slot 0xC0, i 0x90, tpage 0x8E, col 0x8D, spill 0x7C, frame 0xF0): 933.
5. `case 0:` labels in both `unk_05 & 0x7F` switches. Both tables start at 0, and without the labels IDO emits an `addiu -1`: 917.
6. `uv = anim->count; if (uv != 0) for (j = uv - 1; j >= 0; j--)` gives `addiu v1,a1,-1; bltz v1; move s3,v1`. A direct `for (j = anim->count - 1; ...)` or `do`/`while` folds the copy: 899.
7. One index variable for both animation arms (`tpy = rec.unk_18;` / `tpy = seq[slot][step*2+1];`, then `anim = unk_0C[tpy]` in each arm) gives the original's `a1` in both arms and `lui t1,0x8000` for the second bit test. `anim` computed once at the join merges the two arms: 887.
8. Mirror arms in the order x, mx, scale (`spr.unk_18 = ...; spr.unk_1C = 0x1000;`). The original's `lh; sh s4; subu` interleave needs the scale store last: 857.
9. Tail: `seq = ((s16 **)rec.unk_10)[slot];` at the join before `if (rec.unk_08 >= seq[step * 2 + 2])`, plus an empty `else {}` on the `unk_06 == 0` arm (marked FAKE). IDO's PRE moves the `seq`, `seq + step*4` and `unk_18` loads into the predecessors only when every predecessor is a real block with one successor. No critical edges are split, as `snippets/p3.c` (no PRE) and `snippets/p4.c` (PRE) show. The original has three copies (after `+=`, at the inner join, on the `unk_06 == 0` arm) and `move a1,v1` at the join, and this body has the same: 649 (via 795 with `seq` assigned in all three arms).
10. `for (i = 0; i <= gs->count; i++)` with `continue;` after the `printf` (instead of `do`/`while` with the if/else). PRE then puts the latch's `gs->count` load into the `printf` block, as in the original (`lw t6,0xEC; lw v1,0x1098(t6); b; lw t9,0x90`). `do`/`while` with `continue` does not do this.

## What is left (first divergence first)

1. **0x1000 constant in fp, original s4** (row 28; every s4..fp row after it). Traced priorities: constant (bit 515) 600/41 = 14.63 (6 compare blocks at frequency 100, range of 41 blocks); ox and oy 520/27 = 19.26; `rec.x + ox` and `rec.y + oy` 410/26 = 15.77; j 610/26 = 23.46. The original colours the constant between j and ox, so its priority lies between 19.26 and 23.46, or ox, oy, bx and by all fall below 14.63. The five `sh` stores of 0x1000 in the mirror arms are not uses of the range: they are halfword constants (`LDC H`), and ugen only reuses the register. Holding 0x1000 in a variable makes all eleven sites uses: priority 28.97, it lands in s3, and 12 instructions are added. The arithmetic says the constant needs 8 or 9 use blocks, or a range of 31 blocks or fewer.
2. **gs gets a split piece in s0** (`lw s0,0xEC(sp)` in the preheader and before the latch; the original never holds gs in a register). The trace shows gs's range split into {bb0} (the def before the calls, spilled with `sw t6,0xEC` in the `jal` delay slot, as in the original) and a 5-block piece {loop top, printf block, latch} with priority (20 - 10)/5 = 2, which gets s0. When gs is assigned after the two calls (`variant-gs-after-calls.c`), the def stays with the loop piece, six save charges make the piece negative, gs is never in a register, and the body measures 629 rows. The prologue is then wrong (lui/addiu/sw after the calls), so this is not a fix. `volatile` gs breaks the absolute entry test. A gs initializer, gs between the calls, and gs for the printf arguments or `unk_10F8` change nothing.
3. Ring temporaries one slot off in several blocks, and `li at,-33; and` against `andi 0xffdf` in two tail arms. This as1 delay-slot choice follows the register assignment: it disappeared in one autotweak variant when allocation shifted.

## Negatives (measured, do not repeat)

- In the j loop, `for`, `do`/`while` and `j = count; while (--j >= 0)` give identical code. A separate count variable is what gives the copy.
- For the latch, `while (gs->count >= ++i)`, `++i <= count` and `for` without `continue` are all identical to `i++; while (count >= i)`.
- `s32 ox, oy` changes the frame (973). `seq` as a variable inside the animation arm folds `step*2*2` into one shift (891).
- `anim->part + j` gives the same code as `&anim->part[j]`. Inlining `part` into all uses gives 797; removing its declaration gives 825.
- Mask spellings `~0x20U` and `-0x21` give the same code. `((Rec24 *)&spr)->f` and `spr.f` give the same code (body.c uses `spr.`).
- autotweak (300 s, two runs) found `ox - (&parts[j])->unk_00` in one arm (791 to 675 on the step-10 body, but 629 to 813 on the gs-after-calls variant, so it is incidental) and `oy` before `ox` (649 to 643, but it puts oy in the lower register, while the original has ox s5 and oy s6). Neither is in body.c.

## Next hypothesis

Raise the constant's priority to between 19.26 and 23.46 without new instructions. The concrete test is to find two more J-typed uses of 4096 at frequency 100 in the j loop. The likely candidates are two of the halfword scale stores written through an int-valued expression that uopt does not fold to `LDC H`, or the `/ 0x1000` divisions, which are not counted now. Read bit 515's `R9UNIT` lines after each try (`rd.sh`). Once the constant sits in s4, rerun regdiff. The ring rows and possibly the gs piece (whose register is chosen last) may move. For gs, the piece needs one more reload charge or one fewer use. Compare the `R9SAVE bit=0` lines of `variant-gs-after-calls.c` and body.c.
