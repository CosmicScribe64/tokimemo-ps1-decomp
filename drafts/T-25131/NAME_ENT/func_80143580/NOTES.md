# T-25131: NAME_ENT func_80143580 (introduction scene, 1077 words)

Not matched. `src/` keeps `INCLUDE_ASM`; this folder holds the best body (`body.c`), the
prototypes it needs (`declarations.h`) and `replay.py` (run in Docker from the repo root:
`python3 drafts/T-25131/NAME_ENT/func_80143580/replay.py`).

## Measures

Rows are `funcdiff --resolve` rows. With equal lengths funcdiff compares by position, so a row
count of an equal-length body cannot be compared with one of another length. The Levenshtein
score of `funcdiff.diff_score` (insert/delete 1, substitute 2) is comparable across lengths and
is the measure used below.

| body | words | rows | score |
| --- | --- | --- | --- |
| T-20008 archive (start state) | 1078 | 835 | 811 |
| T-24526 checkpoint, s16 old_x restored | 1077 | 1398 | 896 |
| step 7 below (equal length) | 1077 | 1244 | 712 |
| + case-9 chain `c10 = c11 = c12 = 0x80` (step 8) | 1075 | 416 | 416 |
| + case-16 `girl = (girl & 0xF) + 0x10` (step 9) | 1076 | 477 | 471 |
| + record-loop set-up on the `do` line (step 10), this draft | 1076 | 471 | 465 |

Target: 1077 words, frame 0x90, `tiles[8]` at sp+0x6C, loop counter spilled at sp+0x64.
The draft has all three. Structure (opcode sequence) differs in 35 aligned hunks, most of them
`lui at` placement that follows the register choice.

## Changes and why (each one measured on its own)

1. Selector as the global, no local copies. `switch (step)` on `step = D_800E6280.unk_110D`
   (as in the matched sibling func_80142E24), every advance `D_800E6280.unk_110D++`. uopt then
   promotes unk_110D itself, and its saves are the three `sb t0,D_800E738D` of the target in
   case 5 (case head, delay slot of the x test). The T-20008 FAKE stores
   (`D_800E6280.unk_110D = index`) and the `index = D_800E6280.unk_110D` reloads are gone, and
   so is the extra `move t0,v0` at the entry. 1078 -> 1077 words. `= D_800E6280.unk_110D + 1`
   gives the same code as `++`.
2. Case 5: `if (spr0.unk_04-- < -0x140)` and `func_8004B19C(spr0.unk_04, 0, 0)`. The target's
   `slti v1,v0; addiu v0,v0,-1; sll/sra v0` is the promoted s16 field (v0), not an old_x local;
   the store is the save before the call.
3. Case 7: a separate u8 local (`level`) for `D_801217D0[2].unk_16`, stores from it, then
   `unk_16 = level + 1; if (level >= 0x81)`. Removes the extra `sltiu/xori` of the post-increment
   test; case 7 is then the target's shape.
4. Fade steps as chained assignments: `S.unk_10 = S.unk_11 = S.unk_12++` (or `--`) inside the
   test, `if ((... = S.unk_12++) >= 0x81U)`. The fade/fade2 locals are gone. This puts the fade
   value in a1 (target) in cases 0/2/4/6, and the case-9/12 promoted fields in a1/t2 (target).
   Score 916 -> 832.
5. Case 6 reset `spr1.unk_10 = spr1.unk_11 = spr1.unk_12 = 0;` reproduces the target's
   `move t2,zero; sb t2,c11; sb zero,c12; sb t2,c10` exactly (three plain stores give three
   `sb zero`). Score 955 -> 916 (on the earlier base).
6. Case 6 record loop without the T-24526 `depth` local: `sprite[-1].unk_12 = 0x1FF - index++;`.
   The local took a uopt register; without it the expression is the target's ugen `subu t6`
   and 0x01000000 gets the target's `ra` range in every case. Every register of the loop now
   matches. Score 832 -> 736.
7. One loop counter `i` for the case-6 record loop and the case-11 palette loop, declared
   second after `tiles` (`index` stays for the entry fill and the nested fills): frame
   0x88 -> 0x90, tiles sp+0x64 -> sp+0x6C, counter spill at sp+0x64 as in the target. With `i`
   only in case 11 the counter took v1 and the case-6 counter a1 (score 746); with one variable
   in both loops it crosses the case-11 call and spans the case-6 loop, where v0..a3 are all
   taken, so it gets the target's t0 in both. Score 736 -> 712.
8. Case 9 `spr0.unk_10 = spr0.unk_11 = spr0.unk_12 = 0x80;` (found by autotweak's chain kind):
   one `li 0x80` for the three stores, as in the target (`addiu a0,zero,0x80`; draft a1).
   The two words it removes were hiding two real deficits (case-5 `move a0,v0`, case-11
   `slti`), so the length is now 1075. Score 712 -> 416. Chaining the three 0x80000000 stores
   of case 9 the same way gives a chain temporary instead of the shared constant (418).
9. Case 16 `girl = (girl & 0xF) + 0x10;` without the explicit `& 0xFF`: the arm becomes the
   target's `andi; addiu v0; andi tN,v0,0xff; move v0,tN` (four words). The draft still has an
   extra `move v0,v1` before the arm (girl in v0, the promoted F5F in v1), so every later ugen
   temporary of case 16 is one ring slot off: score 416 -> 471, kept because the arm itself is
   the target's. Reading girl through `*(u8 *)&D_800E6280.unk_F5F` removes the copy (case 16
   then matches except `li v0,16`), but F5F is no longer promoted and case 17 loads it into a
   temporary (score 450).
10. Case 6 record loop set-up on the `do` line, `sprite = D_801217D0; i = 0; x = -0xD0; y = 9;
    do {` (autotweak group kind, FAKE layout): 477 -> 471 rows.

## First divergence left

Word 10: the selector load `lbu t0,D_800E738D` is `lbu a2` in the draft. The promoted global
(uopt range, prio 0.5, coloured last) gets the lowest colour free of conflicts: v0, v1, a0 and a1
are taken, a2 is free. In the target it is t0 in every piece (head, reloads after calls, the
`t0+1` advances). In the target a2/a3 are only used in the case-6 loop, where the selector is
dead (reloaded after the calls before the loop), so a plain lowest-free rule cannot give t0.
The case-6 loop counter and the case-11 counter are also t0 in the target and in the draft.

Other colour regions (draft -> target): high part 0x8012 of the spr0 fade loads v1 -> a0
(cases 0/2/4/6); 0x80000000 v0 -> t1 (cases 2/4/6/9/13/16); D4 (spr0.unk_04) a0 -> v0 with the
compare v0 -> v1 and F8 (spr1.unk_04) v1 -> a0 (cases 1/3/5/6), rec0.unk_04 and its compare
swapped in case 8; F5F v1 -> v0 in cases 16/17. D4 takes a0 from the argument preference of
`func_8004B19C(D4, 0, 0)`; a FAKE `D4 ^ 0` argument removes the preference (F8 then is a0 and
the target's `move a0,v0` appears) but D4/compare stay swapped and one word is added, so it is
not in the body.

Structure left: (a) case-6 record loop: the target stores x/y, then increments y, x, sprite
and the unk_10A5 pointer, then the other fields at -0x24(v1)...; the draft stores every field
at positive offsets and increments at the end. (b) Nested fill loops of cases 6/13/16: the
target keeps the end constant in two ranges (`addu a0,a1,zero` or two `lui`) and rematerializes
the start each outer pass; the draft keeps the start in a register (`move v1,aN`) and one end
range. (c) Case 11: target loop test `slti at,t0,3; bnez`, draft `li at,3; bne`
(`while (i < 3)`, `i <= 2` and `for` all give `bne`). (d) Case 16: the extra `move v0,v1`
(step 9). (e) Case 5: the target's `move a0,v0` before the call is missing (D4 sits in a0).

## Negatives (measured, do not repeat)

- Unprototyped `func_8004B19C` (`funcdiff --proto 'void func_8004B19C();'`): identical output.
- D4 argument as `(s16)x`, `*(s16 *)((u8 *)&D_801220D0 + 4)`, `x + 0`, `x | 0`: identical.
  `x << 0` / `x ^ 0`: preference gone, +1 word, D4/compare swapped (see above).
- `s32 step` for `u8 step`; direct `switch (D_800E6280.unk_110D)`: step local is 20 score better.
- `D_800E6280.unk_110D = step + 1` in the cases without calls: 1072 words, saves lost.
- Case 16 with direct F5F reads/stores instead of the girl local: F5F splits into v1/v0 and the
  save moves into the arm (worse).
- Case 6 `fade2 = 0` then stores from it: uopt propagates the zero (three `sb zero`).
  Two-level chain `c10 = c11 = 0; c12 = 0`: `move t1,zero` on c10 only.
- Case 7 load through `((Rec24 *)&D_801217D0[2])->unk_16` or `((u8 *)D_801217D0)[0x5E]`:
  identical.
- Case 6 loop with `} while (++index != 3)` and `unk_10A5[index]`; case 13 outer loop as
  do-while with an explicit `address += 0x4000`; case 11 as `for`: identical output.
- Empty test before every advance (`if ((s32)D_800E6280.unk_110D) {}`, the cheatsheet recipe
  for a promoted byte global): selector a2 -> a0, +1 word.
- Declaration orders with `index` as the only counter (`tiles; address; index`,
  `address; tiles; index`, `x; tiles; address; index`): frame stays 0x88, spill 0x60/0x5C.
- Diagnostic only (scratch copy of tools/cvt_pass.py forcing `OPTN 405 0`, never committed):
  copy propagation off does not fix the loop store order or the selector colour (score 753).

## Next hypothesis

The selector colour t0 (about 70 of the differing words) is the lever, and one experiment moves
it. autotweak (emptytest kind) put `if (sprite) {}` right after the case-6
`load_palette(0x801C8000, 0xE, ...)` call, before the spr1 reset chain. `sprite` is not yet
assigned there, so this reads an unset local and cannot stay in C, but it measures the
mechanism: the selector becomes t0 in every case (head and all `t0+1` rows right) and the body
goes 471 -> 445 rows (score 439; on the step-8 body 416 -> 380); frame stays 0x90; fade2 and
the spr1 high part then move t2 -> t1, t1 -> a3. The same test at the start of case 0 or
case 19 changes nothing, so what matters is a range that reaches case 6 from the entry across
the case-6 fill loop and calls (`sprite` takes v1). Next: find the original variable with that
range, a value set before the switch (not a constant, which uopt propagates: `sprite =
D_801217D0;` at the top was tested and changes nothing) and used in case 6 after the calls,
for example the record pointer or a coordinate read from memory, then re-measure fade2, D4 and
0x80000000 on top of it. The traced uopt (regdiff `draft.log`, R9SAVE/R9UNIT and R9COST for
the unk_110D bit) shows which registers that range blocks.
