# T-25111: TACO func_8013C6D4 (pickup handler), matched

Result: literal default C, `funcdiff --resolve --version default` MATCH, 1388 bytes (347 words).

Start: plain INCLUDE_ASM; best body `drafts/T-24502/TACO/func_8013C6D4/body.c` (frame 0x50 against
0x88, 161 differing rows by a local aligned diff, first divergence the frame).

What closed it, in order (each measured on the whole unit, rows from a local SequenceMatcher):

1. First loop as a pointer `for (p = D_8015EC70; p < D_8015EC70 + 16; p++)`: gives the original's
   `sltu` exit, no top test and `addiu s3` before `addiu s0`. A signed or unsigned index loop gives
   `bne`; the old `do { } while ((u32)p < (u32)&D_8015EDB0)` gives the wrong setup order.
2. Second loop as `for (i = 0; i < 16; i++)` over `D_8015EC70[i]`: `bne` exit with the end address
   rebuilt in the latch (s3 holds the constant 1), and `move v0,zero` (the dead `i = 0`) in the first
   loop's branch slot because the function is `s32` with no return (void drops it, measured).
3. Frame: `u32 i` declared between the flags takes the 0x50 home; the register locals declared before
   the flags take six home words above them; six more words have no known source (FAKE `pad[6]`).
   The lowest word (0x40) is rounding, no pad needed.
4. Mode switch after the loops: each case clears the sound flag itself (no shared goto block).
5. The sound-flag clears in the two later blocks come after their calls (as1 then puts the store in
   the jal slot, as in the original).
6. The bonus: `D_8015EDD4 = 1;` then `switch (func_800AE0D0() & 7)` assigning D_8015EDD4 in every case,
   with `case 7: default:`; then `D_8015EDF0 += D_8015EDD4;`. This is what puts the 1 in `v0`
   (`li v0,1; sw v0`) and the case values in v0 with one store at the join. Negatives: a local amount
   (v1 or folded `li t9,1`), `x = 1; G = x;` (folded), chained `sp4C = G = 1` either way, the global
   form without a default (one store per case, 364 words), a local initialised to 1 without default
   (spill, frame +16).
7. Second loop's saturating add needs the u8 `value` local (`sltiu`); an s32 gives `slti`.

FAKE: the s32 return type (as in func_8013C1F4/func_8013C440) and `pad[6]`, both commented with T-25111.
Header: added `void func_80133410(s32 arg0);` (matches its definition in 80132220.c) and
`s32 func_8013C6D4(void);` to TACO.h.

Lessons for func_8013BAC4 (same file): pointer-vs-index loop form decides `sltu` vs `bne`; every
declared scalar before the stack flags owns a home word; a global assigned on every switch path is
promoted into v0 with one store at the join.
