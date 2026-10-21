# T-25012 notes (class C/D run, partial)

Tools (`drafts/T-25012/tools/`, copy to `build/scratch/t25012/`; run through docker):
- `score.py UNIT:func real_src X.unit ...` : compile whole-unit copies (`.unit` = full C file text, or a body that replaces the INCLUDE_ASM line) and print a diff-line count against the original asm; `variant.py` writes `.unit` files by replacing a base text. `dis.py` prints the aligned want/got diff. Gives a 3 s/variant sweep with no ninja.
- `funcdiff --resolve` prints `@row` lines only for aligned diffs, otherwise plain `-`/`+`; count with `grep -E "MATCH|^  [-+]"`.

State found:
- VALEN func_80132D0C is already matched (MATCH, 892 bytes); `wiki/data/remaining-2026-10-20.md` is stale for it, sndisp, func_8006BA40 and others.
- main func_8005B43C: 3 rows kept. A u16 local copy of the halfword (`hv`) fixes the club-number colouring but moves the later temporaries (rows 54-77) one slot, so worse; arg/cmp expression forms, int types, early/second empty tests, `do {} while (0)` all 6 diff lines or worse.
- GEKO func_80133024: 64-variant sweep of day/month types (u8/s16/u16/s32) and empty tests on/off: nothing under 4 diff lines (2 rows).
- GEKO func_801367EC (44 rows, T-24922 draft): same cause as func_80136C20, the selector `lui` should fill the D_80145088 branch slot (selector block weight 7, scan stops at +6); no lever.
- OPTION func_801326BC (T-24503 draft 136 / T-24301 draft 106 diff lines): the original is not unrolled, does not hoist `% 4`, and does not CSE the three `(pal + 1) % 4` in the second loop. Loop forms (ptr, do/while, while, u32/neq/le), a local `pal`, `& 3`, u32 `%` all stay at 106+ lines. Open question: what blocks CSE and LICM of the signed `%` (probably an aliasing store or a volatile-like global read).

## GYOZI func_801412A4 (684 B, unguarded; seed drafts/T-24301/GYOZI/func_801412A4.c)
- Seed 103 diff lines. `count += (b1 == 0) ^ 1;` reproduces the target's `sltiu; xori` (a `!(x == 0)` or `!= 0` gives `sltiu;sltiu` / `sltu`). `if (b1 != 0) count++` scores better by line count (91) but not the same instructions.
- D_800F6468 is read as `D_800F53A0.unk_10C8` (reloaded every iteration after the F2F store); `D_800F6478/9` as `unk_10D8/9` (GYOZI.h), no new declarations needed.
- autotweak 300 s: exact 199 -> 96 (best text in best-autotweak.c; it adds `u16 count` which costs `andi 0xFFFF` and a 0x30 frame, so not kept); permuter 600 s -j4: best score 1580 (best-permuter-1580.c), no 0.
- Remaining structure rows: i/count temporaries are two ring slots high (t7 vs t5) in loop 1; second loop wants `lw D_800F6468; addu t7,t8` operand order; `D_801486E4` is read once into a reg in the target.
- `tools/twin_sweep.py`: 0 planned copies, 0 groups, so none of the 119 unmatched functions has a matched exact or near twin.
