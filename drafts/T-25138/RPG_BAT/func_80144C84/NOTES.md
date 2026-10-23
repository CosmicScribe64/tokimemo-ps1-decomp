# T-25138: RPG_BAT func_80144C84 (1,276 bytes): matched

Result: default C, `funcdiff --resolve --version default` MATCH (1,276 bytes), 27/27 sha1 after a clean rebuild.
Commit `T-25138: match RPG_BAT:func_80144C84`. About 25 minutes.

## Start state

`INCLUDE_ASM`. Best body: `drafts/T-24522/RPG_BAT/func_80144C84/body.c`, 28 rows, with three FAKEs
(an unused frame home, `* 1` wrappers on the sum operands, a typed `RpgStat *stat` for `unk_00`) and the
comma return. Measured here: 20 rows (side by side, 317 vs 319 words).

## First divergence and the cause

1. At the join after the two threshold resets the target loads `D_8015EBE0` (a3) before the four counter
   writebacks; the draft stores first. Playbook section 5: as1 keeps a load and a store in source order
   only when both use one symbol.
2. In the third arm the target reloads `unk_08` and `EBE0` after the `D_8015EBCC[D_8015EBAC] == 1` test
   (a1/a3 again, `lui a1` in the `bne` slot); the draft keeps them in registers from the threshold sum and
   stores `unk_08` on the call path. A uopt promoted range does not cross a block with a reference that may
   alias the variable: in the original the indexed flag read aliases `EBE0` and the counters, so they belong
   to one object with the flag words.

Measured steps (rows of the side-by-side diff):

| change | rows |
|---|---|
| T-24522 body | 20 |
| `EBE0` read as `((s32 *)&D_8015EBFC)[-7]` (one symbol with the counters) | 18 |
| plus the flag read as `((s32 *)&D_8015EBFC)[D_8015EBAC - 12]` | 10 (join order and `unk_08` reload fixed; `EBE0` still promoted: an offset below the object is not aliased) |
| everything through `*(V *)D_8015EB70` (pointer view of an incomplete array) | 45-50: `EBE0` reload right, but the counter colours and sum operand order swap (an `ilod` view, not a direct member) |
| direct members of `extern V D_8015EBCC` (override of the header's `s32[3]`) | 4, no wrappers, no `stat` pointer |
| `EBE0 -= 0x14` written before the `unk_08` clamp | 0 (frame 0x30 vs 0x38) |
| locals `sel` (switch value) and `total` (roll argument) declared before `lim` | 0, frame 0x38 |

The `* 1` wrappers and the `stat` pointer of T-24522 are not needed once the counters are direct members of
the view. The comma return is still needed: the plain two-line form puts `addiu v1,v1,-5` one slot early
(2 rows); `unk_04 -= 5; return 0;` on one line also matches (same as1 line rule).

Other checks: the same view based at `D_8015EBAC` (with `idx`, `res` members) also matches, but
`func_801447E0` in this file uses `D_8015EBB0` as a scalar, so the view starts at `D_8015EBCC` (the
existing `RPG_BAT_OVERRIDE_D_8015EBCC` hook, no header change). Reading `D_8015EBD8` and `unk_38` through
the view or directly makes no difference; the final body uses the view for all of them.

## FAKEs left

- The `RpgBattleView` override of `D_8015EBCC` (file top): a partial view; the real aggregate's base and
  extent are not recovered (it holds at least the flag words, `EBD8`, `EBE0` and the counters at `EBFC`).
- The comma return (T-24522, same idiom as `func_801453B4`).

## Next hypothesis (for the real aggregate)

Recover the battle block as one type (from `D_8015EBAC` or `D_8015EBC0` up to `D_8015EC38`), declare it once
in `include/ovl/RPG_BAT.h` and migrate the other RPG_BAT users; this function and `func_80147930`
(801477E0.c, scalar override of `D_8015EBCC`) show the accesses that must stay in one symbol.
