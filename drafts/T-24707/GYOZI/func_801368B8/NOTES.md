# func_801368B8 (GYOZI, 2256 bytes) — bounded negative, T-24707

Started from the T-24029 typed checkpoint. Three fixes measured this session:

- the three string buffers are initialised local char arrays; `u8 sp64[] = "..."` etc. copy from
  D_80145900/D_80145918/D_80145930 with the original's lwl/swl tail;
- one `s32 pad` (FAKE) is the missing frame home: frame 0x80 and buffer offsets 0x64/0x4C/0x3C then match;
- every branch to the epilogue returns 0 (`or v0,zero,zero`), so `return;` had to be `return 0;`.

Result: 765 funcdiff rows (census score was 701). The dominant remaining difference is that the
original keeps the display-record base `D_80129F40` in a register (`lui v0; addiu v0,v0,%lo;
lw t0,0x19AC(v0)`) while C field accesses through the `u8 D_80129F40[]` symbol fold each offset into
its own relocation. Case 3 (the three narrow angle tests) and the case 1/7/9 join also differ in
block shape. No guard kept; production is INCLUDE_ASM.
