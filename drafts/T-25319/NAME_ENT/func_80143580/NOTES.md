# T-25319: NAME_ENT func_80143580 (second pass, introduction scene, 1077 words)

Not matched. `src/` keeps `INCLUDE_ASM` (the best body is far over the guard limit). This folder
holds the best body (`body.c`), the prototypes it needs (`declarations.h`) and `replay.py`. The
replay also applies the one change the body needs outside itself: `s32 func_80143498(void)` (step 3).
Run in Docker from the repo root: `tools/docker.sh python3 drafts/T-25319/NAME_ENT/func_80143580/replay.py`.

## Measures

`funcdiff --resolve` rows of the whole-unit NON_MATCHING compile, and the Levenshtein score
(`funcdiff.diff_score`, insert/delete 1, substitute 2). With unequal lengths the two agree.

| body | words | rows | score |
| --- | --- | --- | --- |
| T-25131 draft (start state, measured again here) | 1076 | 471 | 465 |
| 1. case 7 as one chained assignment, `level` local gone | 1076 | 467 | 461 |
| 2a. case 16 `girl` read through `*(u8 *)&unk_F5F` | 1075 | 440 | 440 |
| 2b. case 16 on `unk_F5F` itself + entry self-store of `unk_F5F` (replaces 2a) | 1075 | 382 | 382 |
| 3. `s32 func_80143498(void)` (viewsweep) | 1075 | 370 | 370 |
| 4. case 5 call argument `unk_04 ^ 0` | 1076 | 355 | 355 |
| 5. entry fill as `for (index = 0, address = ...; ...)` | 1076 | 353 | 353 |
| 6. case 1 `unk_110D++` before the `0x51000000` store (autotweak order) | 1076 | 347 | 347 |
| 7. case 6 record loop indexed `D_801217D0[i]` in a `for`, no `sprite`/`x`/`y` locals | 1076 | 331 | 331 |
| 8. `if (address) {}` before the case 6 fill, case 0 and case 9 store order (autotweak); this draft | 1076 | 293 | 293 |

Target: 1077 words, frame 0x90, `tiles[8]` at sp+0x6C, loop counter spill at sp+0x64 (all kept).

## Changes and why

1. **Case 7 is one chain**: `if ((D0.unk_14 = D0.unk_15 = D0.unk_16 = D1.unk_14 = ... = D2.unk_15 =
   D2.unk_16++) >= 0x81U)`. The target keeps the high part of `D_8012182E` in `v1` and the value in
   `v0`, and stores the increment last. The `level` local gave `lui v0; lbu v0` and the increment in
   the middle. Case 7 now matches. The unused `level` declaration must go (frame 0x90 -> 0x98 otherwise).
2. **Case 16/17 read `unk_F5F` directly, plus an entry self-store** `D_800E6280.unk_F5F =
   D_800E6280.unk_F5F;` (FAKE). In the target F5F is one promoted range in `v0` for cases 16 and 17
   (`lbu v0`, arm `andi t9,v0,0xf; addiu v0,t9,16; andi t6,v0,0xff; move v0,t6`, `li v0,16; sb v0`,
   case 17 `lbu v0`). With plain reads, tools/cvt_pass.py keeps the CVTs of F5F's compares, because F5F
   is first touched after a branch (its entry rule). uopt then keeps `cvt(F5F)` as a separate
   expression range (v0) next to F5F (v1) and splits F5F around the arm. Any access of F5F before the
   entry loop satisfies the pass's entry rule. A self-store is removed by uopt (no code, no range from
   the entry; `if (F5F) {}` at the entry keeps a load and makes F5F live from the entry, `a3`).
   The `girl` local and the comma placement of the store are gone.
3. **`s32 func_80143498(void)`** (viewsweep, the only callee view that helps): the same-file helper
   called at the top of case 4. It still matches as `s32` with no return value (checked with funcdiff).
4. **`func_8004B19C(spr0.unk_04 ^ 0, 0, 0)`** (FAKE): drops the a0 argument preference of the promoted
   D4 (`spr0.unk_04`). The target passes it with `move a0,v0`; this gives the move (the missing word)
   and F8 (`spr1.unk_04`) in `a0` as in the target. `<< 0` and `* 1` give the same code, `-(-x)` two
   `negu`. `x + 0`, `x | 0`, `(s16)x` and an unprototyped callee keep the preference (T-25131).
5. **Entry fill `for (index = 0, address = 0x80180000; address < 0x801A0000; address += 0x4000)
   tiles[index++] = address;`**: the comma init puts `addiu v0,sp,0x6C` before `lui v1` as in the
   target (the `do` form and the statement order swap do not).
6. **Case 1 `D_800E6280.unk_110D++;` before `spr0.unk_00 = 0x51000000;`** (autotweak order kind):
   the target's `addiu t9,t0,1; lui at` order.
7. **Case 6 record loop in natural indexed form**, with no `sprite`, `x`, `y` locals:
   `for (i = 0; i < 3; i++) { *(u32 *)&D_801217D0[i].unk_00 = 0x01000000; D_801217D0[i].unk_04 =
   i * 0x50 - 0xD0; ... D_801217D0[i].unk_0C = i * 2 + 9; D_801217D0[i].unk_12 = 0x1FF - i; ...;
   D_800E6280.unk_10A5[i] = 7; }`. uopt's strength reduction places every pointer and coordinate
   increment at the `i++` and rewrites the later stores as negative offsets; this is the target's
   loop instruction for instruction (`subu t6,t3,t0; addiu t0,t0,1; sh a0,4(v1); sh a1,12(v1); 4 x
   addiu; sw ra,-36(v1) ...; bne t0,t5; sb t4,0x10A4(a2)`). The pointer walk of T-25131 (with the FAKE
   one-line set-up) let uopt sink the increments to the end. The three locals must be removed too
   (unused declarations move the frame).
8. **`if (address) {}` right after `spr0.unk_00 = 0x80000000;` in case 6** (autotweak emptytest,
   FAKE): `address` holds 0x801A0000 from the entry fill, so the read makes it live from the entry
   through the switch head into case 6. This is the range the first pass looked for: the selector
   gets `t0` in every piece (73 words). The same test at the start of case 6, after the D4 step,
   at the start of case 13 or 16, or on `index` is worse (329-377 rows). Two order edits come with
   it: case 0 `unk_110D++` before `D_8014D170 = 0`, case 9 `unk_110D++` last.

## First divergence left

Word 22 (case 0): the spr0 high part `lui a1` where the target has `lui a0`, and the fade value
(`spr0.unk_10`, promoted) in `a2` where the target has `a1`. The chain temporary of the fades
(cfe VREG `M108-47` in this draft, the old value of `unk_12`, priority 8, no instruction of its own)
is coloured early and takes `a0`, because `v1` is held by `address` in the case 6 fade block (the
step 8 range) and `v0` is not available to it. `unk_12` then takes `a1` and `unk_10` `a2`. That
`a2` is what keeps the selector off `a2` now, so step 8 reaches the target's selector colour by a
route the original probably did not take: in the target `unk_10` is `a1`.

Other colour regions (draft -> target): D4 (`spr0.unk_04`) `a3` -> `v0`, the shared compare
temporary of the four `x-- < K` / `x++ >= K` tests (cfe VREG at -64, cases 1/3/5/8) `v0` -> `v1`,
F8 `v1` -> `a0`, rec0.x (case 8) `v1` -> `v0`, the 0x80000000 constant `v0` -> `t1`. The compare
temporary has priority 4 (two refs per unit, four units, denominator 2) and is coloured before D4
(1.33), F8 and rec0.x (1.0), so it gets `v0` whenever `v0` is free in cases 1/3/5/8. In the target
it is `v1` while D4 and rec0.x are `v0`; under the reconstructed rules that needs a range of
priority > 4 holding `v0` in one of those four blocks, and none exists in this C.

Structure left (16 structural rows): the nested fill loops of cases 6/13/16 (the draft hoists the
start constant into a register, `move v1,aN`, and keeps one end-constant range; the target
re-materializes the start at the outer head and keeps two end ranges, `addu a0,a1,zero` in 13/16);
case 9 (the `D_801217D0[2]` store moves up between the `0x80` chain stores; the 0x80 value is
`unk_10`'s register in the draft, a separate `a0` value in the target); case 11 `slti` (draft `bne`);
one `lui at` order at each of the case 0 and case 18 selector advances.

## Negatives (measured on this pass, do not repeat)

- Case 9 chain orders (`c12 = c11 = c10`, `c11 = c12 = c10`, ...), `(u8)` cast dropped, separate
  last store, the `[2]` store after the advance: no change or worse (the order fix of step 8 is
  `unk_110D++` last).
- Case 16 with `girl` kept: `girl = F5F` in case 17 as well, `F5F = girl = 0x10`: identical to 2a.
  Direct F5F without the entry reference: F5F splits (`cvt(F5F)` range in v0). `if (F5F) {}` or
  `(void)F5F` at the entry: F5F live from the entry (`a3`) or no effect.
- Entry self-store of `unk_110D`, of `spr0.unk_10` or `spr1.unk_10`: no change / much worse (the
  struct-cast store is not removed).
- `s32 func_8004B19C()` views: no change. viewsweep after step 8: no gain.
- Compare forms of the `x--` tests: `<= -0xF1`, `(s16)` cast, operands swapped: identical; `x--;
  if (x < K-1)` and `(x -= 1) < K-1`: no compare temporary, +2 words; `(u8)`/`(s16)`/`(u16)` casts
  of the comparison: +4..9 words. Folding the 10F8 test into `&&` (case 1, case 8): identical.
- `register u8 step`: an extra `move` at the dispatch; `s32`/`u32` step: worse; `switch (step =
  ...)`, `switch (D_800E6280.unk_110D)`: identical.
- Fill loops: outer `do`/`while` without the outer init, `goto` outer loop, comma init in the inner
  loop, `for (; address < END; ...)` (visible entry test), index-terminated inner loop (uopt unrolls
  it by 4): none removes the start-constant hoist or splits the end constant.
- Record loop: pointer walk orders with the `i++` first or after the x/y stores, `do` on its own
  line: worse than the indexed `for`.
- Case 0 / case 18 statement orders other than step 8: no gain.
- `if (address) {}` at the start of case 6, after the D4 step, at the start of case 13 or 16, or on
  `index`: 329-377 rows; a second read in case 13 or 16 on top of step 8: no change.

## Next hypothesis

Step 8 shows that one range from the entry into case 6 is enough to move the selector, but it does
it through `unk_10` in `a2`. The target needs `v0`/`v1` taken in the fade blocks by something other
than the chain temporary, so that the temporary falls to a register the fades do not show, `unk_10`
gets `a1` and the high part `a0`. Next: find the real range that is live from the entry (the value
of `address` or `index` after the entry fill is the only candidate the code offers) and read it
where the original does, then re-measure the D4/compare pair; the compare needs `v0` blocked in one
of cases 1/3/5/8 by a range of priority above 4, which the same entry range could provide if it
reaches those blocks (try the read at the end of the switch head's reach, e.g. inside the case 1/5
tests). Tools: `regdiff -v` on the draft, then the R9SAVE/R9UNIT lines of `draft.log` (this pass
used small scripts that list the ranges with units in given blocks and the colour from R9COST).
