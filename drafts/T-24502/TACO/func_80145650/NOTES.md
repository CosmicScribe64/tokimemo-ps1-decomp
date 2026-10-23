# T-24502 typed quad transform draft

Not matched; production remains INCLUDE_ASM. Uses SDK POLY_FT4 pointers, SVECTORs and MATRIX rather than stale scalar stack aliases. Scratch IDO compile has the original 0x88 frame, correct matrix/input/output homes (0x54/0x7C/0x74), and all instructions through the second matrix copy.

First divergence: second 32-byte matrix copy alternates `$at` with `$t8`, target `$at` with `$t5`. The copy source still uses `$t3` in both. The following input.vx reload uses `$t7` instead of `$t8`; later expressions propagate further ugen ring differences. Target has 477 words; scratch has 480 including three trailing alignment nops. Resolved search cost 401.

Bounded tests: angle declaration before aggregates fixes all homes; natural XYZ input assignment improves ordering. Translation field order has no effect. Explicit s16/u16/s32 conversions of input x/y differences (9 forms) have no effect. All 8 combinations of own return void/s32 and callee func_8015CF30/func_800A0ECC void/s32 views have no effect. No production compiler edits or inline asm attempted. No claim that the remaining gap is proven unreachable from C.
