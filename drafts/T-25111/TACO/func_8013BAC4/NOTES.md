# T-25111: TACO func_8013BAC4 (collision handler), matched

Result: literal default C, `funcdiff --resolve --version default` MATCH, 1840 bytes (460 words).
Clean rebuild 27/27 SHA1; headers, globals and branch proto checks pass.

Start: plain INCLUDE_ASM; best body `drafts/T-24502/TACO/func_8013BAC4/body.c` (456 against 460
words, Levenshtein cost 330, frame 0x4A8 against 0x4A0).

Each step was measured on the whole unit with a resolved-word Levenshtein score (funcdiff keys).
In order:

1. Second distance compared directly: `ydist = f(...); if (xdist < r + 0x18 && ydist < r + 0x18)`.
   The old `ytest = f(...) < r + 0x18` bool gives `slt a0` plus `beqz a0`. A separate y variable
   per loop (one def each) let the threshold `r + 0x18` take v1 (score 322).
2. Score index base first, `scores[(D_8015EDB4 + j)->unk84[2] % 17]`, as in func_8013CE78 and
   func_8013CFD8. The other accesses stay index first, `(j + D_8015EDB4)->`.
3. `if (unk84[1] != 0) continue;` before the second score add in loop 1. With a plain `if`,
   ugen reuses the stored D_8015EDF0 value across the fall-through (no reload, and every later
   temporary is one ring slot off). Score 322 to 90.
4. Loop 2 as `for (i = 0; i < 16; i++)` over `(i + arg0)`: uopt's strength-reduced pointer and
   offset get the 0x5C/0x60 homes, the dead `i = 0` store stays, and p is stored before the offset
   in the latch.
5. Frame: no `init` counter (the fill loops use j), and 11 scalars above the tables, with i third
   and damage tenth (x, y, i, z, j, radius, xdist, ydist, kind, damage, width). Score 5: only the
   first branch slot of loop 2 is left.
6. That slot needs the s32 return with no `return` (v0 live at the exit, so as1 cannot move
   `andi v0,v1,0xf` into it and copies `lw t3,0x60(sp)` from the latch instead). But under s32
   the y distance got `move a0,v0`: tools/cvt_pass.py turns global copy propagation off for a
   procedure whose switch selector is an unsigned global read at the entry. uopt then keeps
   ydist as a range, and v0 is never chosen for a call-result range defined in the call's block
   (cost 0 for a0 while v0 was free, checked on toys). In a void function as1 deletes the move;
   in an s32 function it does not, because v0 is live afterwards.
7. `switch (*(u8 *)&D_800E6280.unk_1109)`: the byte view is an indirect load, so the pass leaves
   copy propagation on. ydist is then folded into its compare, and the stage still compares in
   v1. Score 0.
8. Cleanup with no code change: `s32 func_8013D2A0(s16, s16, s16)` in TACO.h (its definition in
   8013D1E0.c; the old `(s32)` casts in func_8013CE78 and func_8013CFD8 are dropped, and both
   still match), `TcDamage9 D_8015F548` in TACO.h, `u32 scores[256]` (no `U` casts), and the
   kind variable in loop 2.

FAKE: the s32 return type and the byte-view switch selector, both commented with T-25111.

Negatives measured:
- A plain `if` instead of `continue` (score 268).
- The `&` of the two compares (449).
- A pointer blocker `p = scores; if (p) {}` before the switch (367).
- A local copy as selector (`j`, `i`, `arg1` or `u8 sel`): the selector goes to v0, and the
  pointer and end inits swap in the case delay slots (18 to 24).
- Without the s16 prototype, `(s16)` casts drop the double narrowing of x/y in loop 1 (340).
- Under zcopy off, a call-result variable used in another block never gets v0.

Tool lesson: in a procedure that switches on an unsigned global at the entry, every cross-block
copy stays unpropagated. If an s32 function then shows `move rX,v0` after a call where the
original compares v0 directly, read the selector through a byte view.
