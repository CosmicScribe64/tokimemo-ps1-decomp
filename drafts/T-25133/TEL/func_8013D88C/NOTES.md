# T-25133: TEL func_8013D88C (date-spot menu builder)

Result: MATCH, default-build C, 2980 linked bytes (743 words plus two padding nops). The body is
in `src/ovl/TEL/8013CCD0.c`; no guard and no `INCLUDE_ASM` remain.

## Start state

INCLUDE_ASM. Best earlier body: `drafts/T-25060/word-arg0.c` (pointer walks from m2c), measured
here at 751 words against 745, aligned cost 786, 836 printed rows, frame 240 against 224.

## What closed it, in order (aligned cost from `funcdiff.diff_score`)

1. Frame evidence first. The target's frame holds only 8 bytes of declared scalars above the
   four 24-byte arrays; the ys/steps/flags homes at 0x58..0x64 lie below the arrays, so they are
   uopt spill homes of strength-reduced induction variables, not declared pointers (every declared
   scalar owns a slot; playbook section 7). Rewrite with `s16 x[11], y[11], avail[11], step[11]`
   indexed by `i` and only two scalars (`i` and later `g`). Frame 224 at once.
2. Call arguments as the repeated expressions `(1 - i / 5) * -0x88` and `(i % 5) * 0x10 - 0x38`,
   not `x[i]`/`y[i]`: uopt CSEs them into s0/s1 and the call gets `sll/sra` of those (746 words,
   cost 663). uopt distributes `(1 - q) * -0x88` into `negu; negu; mult; addiu -0x88` itself; only
   the loop-three arm whose 1 is already in v1 keeps `subu`.
3. Row-10 stores on one source line (as1 store order; FAKE): 659.
4. Page-one store order `x, step, y` (fp goes to the x pointer): 597.
5. First month read of each page-two ladder through another base than the second read (here
   `g->unk_03F`; `*((u8 *)&D_800E6280 + 0x3F)` is equivalent): uopt no longer promotes the month
   into v0 for the ladder, the F64 word PRE temporary takes v0 and flags v1 as in the original
   (161). `volatile` on either read confirmed the mechanism but added words.
6. The month bypass as `if (arg2 == 12 || arg2 == 2 || arg2 == 0 || arg2 == 14) { } else {`:
   the original's compare order (a `switch` is sorted 0, 2, 12, 14 by ugen) and, unlike the
   `!=`/`&&` form, the 0x74 constant stays below the loop-three `i * 2` range (s5 vs s4): 155.
7. Page-one stores on one line `x; y; step;` (79), then spot flags through `g->unk_120[i]` on
   page one (61): the flags induction variable is then created last and takes v1/home 0x58.
8. Page-one and ladder packed-flag bits through `FLAGS_F64(g)`, the else-switch and loop-three
   bits through `FLAGS_F64(&D_800E6280)`: page one keeps the GameState base in v0 for all five
   cases (the plain spelling turns cases 1/3/5 into an unpromoted global word and loads them
   absolute) while page two keeps the PRE temporary (cost 6). Other splits: all via g 584,
   page one only 342, else-switch too 598.
9. Page-one stores back on three plain lines `x; y; step;`: cost 0, MATCH.

## Negatives measured here (do not repeat)

- Pointer-walk bodies of T-24039/T-24511/T-25060: wrong frame and spill homes by construction.
- `switch (arg2)` with any case order: always sorted 0, 2, 12, 14.
- Month read spellings `(u8)`, `% 3`, `(u32) % 3`, `unk_03F == arg2`: identical (the CSE is by
  variable, not by expression shape).
- Removing the second scalar slot (frame changes, 629); `(&D_800E6280.unk_120[10])[i]` and
  `*(unk_120 + i)` flag spellings (684, 610); `((u8 *)&D_800E6280)[i + 0x120]` (no induction
  variable at all).
- Empty `if (FLAGS_F64->b0) {}` tests to renumber constants: no gain.
- `tools/autotweak.py` fails setup on this unit (asm-processor stage 55), so it was not used.

## Tools

`regdiff.py` with the traced uopt (`TOKIMEMO_SCRATCH` holding `r9-uopt/variants/trace`) and the raw
`R9SAVE`/`R9UNIT` lines of `build/scratch/regdiff/func_8013D88C/draft.log` gave the priorities and
block uses that identified steps 5 and 8.
