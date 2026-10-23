# VALEN func_80132834 (T-25117): matched

Valentine candidate setup, 1240 bytes (310 words). It clears the event state, flags girl 9,
ranks the top three affinity sums, gathers and rotates the candidate list, and moves girl 0 and
girl 9 into place. Default C in `src/ovl/VALEN/80132760.c` matches the linked bytes. No guard,
no FAKE.

## Start state

Production had INCLUDE_ASM. The best body was `drafts/T-24523/VALEN/func_80132834/body.c`
(typed, correct control flow). Measured in this tree: aligned cost 336, 342 funcdiff rows.
Main divergences, in order: (1) the first two score assignments reuse the comparison sum in
the draft, while the original reloads both fields for them; (2) score homes sp+0x54.. and
frame 0x80 instead of sp+0x50 and 0x78; (3) the draft computes `r` after the rotation loop's
guard (two `beqz t4` in a row), while the original computes it before the guard; (4) the count
increment goes through a temporary and `move s2`.

Costs below are funcdiff's aligned cost (insert/delete 1, substitute 2).

## What closed it, in order

1. `GameState *state = &D_800E6280;` and `state->unk_1BC[i]` for every read in the ranking
   loop: 336 to 120. uopt treats reads through the pointer as loads that any later store can
   change, so the score stores before `rank_scores[0] = ...` and `rank_scores[1] = ...` force
   the reloads. The third assignment comes first in its arm and keeps the comparison sum, as in
   the original.
2. Sum operand order: the comparisons `unk_06 + unk_02`, the first two assignments
   `unk_02 + unk_06`, the third the same as its comparison: 120 to 108. IDO loads the fields
   in the reverse of the written order here.
3. Declaration order `r, candidates[13], state, i, rank_scores[3], count`: 108 to 74, with
   frame 0x78, candidates at sp+0x5C and scores at sp+0x50. Model, checked on six orders:
   locals take slots top-down in declaration order with their natural sizes (s16 is 2 bytes,
   arrays are 4-aligned). About 12 bytes of temporaries sit below them, and the total is
   rounded to 8 (`candidates, r, i, count, rank_scores, state` gives the same layout). With `r`
   first, the candidate capacity must be 12 or 13 to put the list at 0x5C.
4. `r = (u32)state->unk_10F8 % (u32)D_8013453C;`: 74 to 23. Through the pointer, the
   `D_80134524[i]` store in the loop can change the random word, so uopt does not copy `r` into
   the loop preheader, and the division stays ahead of the guard.
5. `count += 1;` before `D_8013453C += 1;` (as in the GYOZI sibling): 23 to 0 (51 alone on
   top of step 3).
6. `unk_0C.f.b1` / `.f.b8` (the Rec38Flags union member) instead of the CharFlags casts: still 0.

## Negatives (measured)

- D_80134540 shuffles moved ahead of the score stores, with direct D_800E6280 reads: 342 rows,
  no change. Stores to other globals do not kill direct indexed reads.
- Scores and candidates in one local struct (`struct { s16 sc[6]; s16 cand[13]; }`): 324. It
  fixes the score homes, but the sum is still reused.
- A `Rec38 *p = &D_800E6280.unk_1BC[i];` record pointer: 378. The reloads appear, but through
  a second base (`addiu v0,s1,444`), and the stores lose their sign extensions.

## Sibling hint

GYOZI `func_80138C9C` (75 rows, T-24707) is described as "uopt zcopy:1 copy-propagating r
into the D_80147224 loop preheader". That is symptom (3) here. Try reading the random word
through a `GameState *` pointer and use the declaration-order frame model. Not tried here: the
file belongs to another package.
