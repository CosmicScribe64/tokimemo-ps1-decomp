# VALEN func_80133088 (T-25117): matched

Location/background setup for the Valentine event, 1108 bytes (277 words). Default C in
`src/ovl/VALEN/80132760.c` matches the linked bytes; the guard is gone.

## Start state

Production had INCLUDE_ASM. The best body was `drafts/T-24523/VALEN/func_80133088/body.c`
(documented cost 14). In this tree it measured 26: the T-24903 `volatile` on `D_80134544`
(VALEN.h, for func_80132D0C) keeps the byte's `lui` in the 自宅前 block, where the original
hoists it above the affinity test (rows 108-118).

## What closed it, in order

1. Non-volatile view `*(u8 *)&D_80134544` in the 自宅前 condition only: 26 to 14.
2. Third dispatch written `if (mode != 2) goto other; goto two;`: the original `beq 2; b default`
   pair, 14 to 10 (only shows the dispatch was a switch compare chain).
3. The two "id before strcpy" blocks (second 校門前 call, 教室 call) written call first, id
   after: 10 to 0. The original's `lui a1; li a2; sw a2,0x28(sp); addiu a1` order comes from
   `strcpy(...); id = K;` there. uopt moves the constant above the call and spills it.
   The matched GYOZI twin `func_801393C8` has the same shape in its last 廊下 block and its source
   writes the call first. Ten toy spellings with the id assignment first (comma forms, one line,
   K&R/implicit prototypes, `register`, u16, array string) all keep `li a2` first.
4. Cleanup against the GYOZI twin: the global `D_80134548` is the selector (assigned in each
   arm, `switch (D_80134548)`), no gotos, no union, no `(u8)` cast. One dead entry store
   `D_80134548 = state->unk_10F8 & 7;` is needed (without it: cost 37; the promoted global takes
   v1 and the random word a0 instead of v0/v1). Either of GYOZI's two entry stores alone works.
5. One `GameState *state = &D_800E6280` for every field access (as func_80132D0C does). All
   direct accesses: 40; pointer only in the middle part: 0; pointer everywhere: 0 (kept).

## Remaining FAKEs (both marked T-25117)

- Dead entry store of `D_80134548` (register choice, see 4).
- `*(u8 *)&D_80134544` in the 自宅前 test (plain read: 12 rows), only needed because VALEN.h
  declares the byte `volatile` for func_80132D0C (T-24903).

## Negatives measured

- Real switch with the store before it, `switch (D_80134548 = mode)`, `switch (D_80134548)` after
  a store from a local, a store in every case: 29-30 (store placement only comes from the
  promoted global with the entry store).
- `if (mode == 2) goto two; else goto other;` and a nested two-label switch: 14 (inverted pair).
- No entry store: 37; plain `D_80134544` read: 12; all-direct `D_800E6280.` accesses: 40;
  selector written `D_80134548 = rnd & 7; if (D_80134548 == 0)`: 144.

## Lesson for the twins

VALEN func_80132834 and the SHOUGATU twin likely follow the same GYOZI shape (global selector,
entry store, call before id). An id that the original spills before a call
(`li; sw; jal`) can still be assigned after the call in the source.
