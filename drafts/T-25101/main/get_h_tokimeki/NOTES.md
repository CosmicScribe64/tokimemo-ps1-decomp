# main get_h_tokimeki (T-25101)

Result: MATCH as default C, 1600 bytes (400 words plus the 6-word jump table), commit
"T-25101: match main:get_h_tokimeki". Time: about 35 minutes.

## Start state

`INCLUDE_ASM`. The only earlier drafts were m2c output (T-14500, T-16100, T-17000, scores 742 to 876)
with wrong control flow (gotos between the threshold ladders, `*(s32 *)` reads of the bonus row).
T-24702 had noted that it needed a typed reconstruction first. No body was reused.

## Reading the asm

- `girl = arg0 % 13`; base `sum = D_800E6280.unk_1BC[girl].unk_02 * D_800B4100[girl] / 100`
  (`multu` is IDO's normal choice for s16 * u8, see `get_h_yuukou`).
- Loop 1, `i = 1..7`: `use[i] = (D_800B4110[girl] >> i) & 1` written as `if (mask & (1 << i))`,
  into an `s32` array whose element 1 is at sp+0x40.
- Loop 2, `i = 1..6`: `if (use[i]) switch (D_800B4120[girl])`, six cases. Each case compares the
  parameter `(&gs->unk_0FC)[i].unk_02` with a ladder of the seven bytes `D_800B4198[0..6]` plus a
  per-kind offset and adds one `s8` of the row `D_800B4130[girl]`:
  kind 1: +50 +35 +20 +20 +5 +5 +0; kind 2: all 0; kind 3: +50 +50 +50 +20 +20 +20 +20;
  kind 4: +10 x4, 0 x3; kind 5: +20 x4, 0, -5, 0; kind 6: 0 x4, -5 x3.
- The compares are `sltu`: in K&R mode a u8 plus a constant is unsigned.
- Clamp to 0..100, `return sum & 0xFF` (as `get_h_yuukou`).

## Steps (rows are funcdiff --resolve rows, compared NON_MATCHING C)

1. Straight typed body, thresholds read in the cases, `u32 val` and `s8 *tbl` locals: 191 rows,
   same 400 instructions, frame 104 instead of 136. The threshold loads were hoisted to the loop-2
   preheader, but they were coloured after the parameter value.
2. Parameter read in every compare (no `val` local): 340 rows (the value took t1).
3. Thresholds read into locals `l0..l6` after loop 1, mask in a local before loop 1: 240 rows,
   loop 1, the preheader and all threshold colours exact (t0..t5, s0). `u8` locals packed (frame
   112); `u32` levels give the 136 frame. The mask must be `u8` (`u32`/`s32` swap the `and` operands).
4. Stack: declaration order runs top-down, so `use[8]` declared after the 11 scalar slots and
   `tbl` declared after the array put `&use[1]` at sp+0x40: 230 rows. Everything up to word 75
   matches; all remaining rows are the bonus row in s2 and the parameter value in s1 (target s1/s2).
5. Diagnosis with the traced uopt (regdiff -v; the expression table in
   `build/scratch/regdiff/get_h_tokimeki/draft.o.trace` after "REMOVAL OF REDUNDANT STORES").
   Unconstrained ranges are coloured in bit order. The value range kept its main-pass bit (88),
   but the bonus row is a copy-propagated local: copy propagation re-enters every expression whose
   operands changed, after the main pass, so the row's address got bit 258. The fix is to make the
   parameter value a re-entered expression too: read the parameters through a
   `GameState *gs = &D_800E6280` local. gs is propagated as well, and the value expression is
   re-entered after the row (bits 261 and 270): 0 rows. `gs` for every GameState read, or only for
   the parameters, at the top or in the preheader: all 0 rows.

## Negatives (measured)

- tbl placement: per case, in the loop head, in the `if (use[i])` head, in the loop-2 preheader,
  inside loop 1: always bit 258 (late). Before loop 1 it keeps an early bit (22), but then it is
  coloured before the thresholds (a0/t5/s0): 318 to 744 rows.
- tbl spellings (`(s8 *)D_800B4130 + t * 8`, `(t << 3)`, int arithmetic, `&D_800B4130[0][t * 8]`,
  `*(D_800B4130 + t)`) and a direct `D_800B4130[t][k]`: no change (230).
- val as an `s32`/`u32`/`u16` local before or after tbl: 230. `s16 val`: the value takes s2, but a
  dead s16 range takes s1 and the row s3 (frame 144, 132 rows). Separate `val1..val6` locals: 310.
- Parameter spellings: byte-offset `*(s16 *)`, `(&unk_100)[i - 1]`: 230; `((s16 *)...)[i * 2 + 1]`,
  `(&unk_0FC + i)->` and a `GsRec0FC *` base pointer split the D_800E6280 base: 488 to 615.
- Moving type and the levels before loop 1: the offset induction (mpy(i, 4)) is then numbered after
  the levels and level 0 takes a0.

## Tool note

`funcdiff --version c` on a guarded body reported "linked function bytes differ" with zero rows:
the guarded compile keeps the unit's data in `.data` instead of the split `.data_800B40F0` section,
so every D_800B41xx reference links to another address. The default build (guard removed) is the
real proof and printed MATCH.
