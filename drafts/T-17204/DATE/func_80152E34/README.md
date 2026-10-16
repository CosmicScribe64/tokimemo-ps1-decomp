# DATE func_80152E34 (T-17204)

`body.c` is the POLY_G3 builder written from the asm (needs `#include "psx_types.h"`; try it with
`--proto 's32 SetSemiTrans();'`, which blocks `$v0`). Frame, store order and the ugen temporaries of `c0` match;
about 74 rows of colours of the six uopt ranges (`poly`, `c0`, `c1`, `c1 >> 8`, `c1 >> 16`, the `D_800E6280` base) are left.
The original colours them `a2 / t1 / a0 / t3 / t2 / a1`, the draft `a2 / a1 / v0 / v1 / a0 / a3`: in the original
`$v0`, `$v1`, `$a3` and `$t0` are never used by those ranges.
Tried: `& 0xFF` masks on c0 and c1, g/r/b store order, `s32` and `void` results, an int-returning `SetSemiTrans`
(blocks only `$v0`), u8 locals for the components, `order_search`, `autotweak` (231 tries). See wiki/matching-notes.md, section T-17204.
