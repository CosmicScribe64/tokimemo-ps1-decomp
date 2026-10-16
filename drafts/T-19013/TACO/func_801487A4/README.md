# TACO func_801487A4 typed draft (T-19013)

Typed redraft of the boss actor transform (948 bytes), restored to `INCLUDE_ASM` in `src/ovl/TACO/801480B0.c`.
Structure and frame match (237 of 237 instructions, locals at the original offsets); 140 compare rows differ in
registers only. Needs `void func_801487A4(s32, s32, void *, void *, void *)` in `TACO.h` and these declarations:
`u8 st;` before `MATRIX m; SVECTOR vec; SVECTOR rot;` and then `s32 sel; s32 px; s32 py;` (the order gives the 0x7C
hole above the matrix and the 0x48/0x44/0x40 scalar slots), `TcPos` for the 8-byte arg2 records, `TcPair2` for arg3.
Open: the ugen temporaries run one step off from the first `sll` (draft `$t8`, original `$t9`), so every later
`$t2..$t9` is rotated. Not fixed by: statement orders of the three leading statements of each arm (order_search
changes the structure), `sel = 0x10 + arg1`, index-first `arg3` access, a separate selector local, an inline `o`
pointer, autotweak. See wiki/matching-notes.md, "T-19013".
