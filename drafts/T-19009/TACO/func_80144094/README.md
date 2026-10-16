# TACO func_80144094 typed draft (T-19009)

Guarded draft that was 111 compare rows (about 45 instructions) from the original; restored to `INCLUDE_ASM` in
`src/ovl/TACO/80143E80.c`. Needs `TcTex4` (4-byte u8 table, `D_8015F5F8` and `D_8015F5FC`) and a `void` prototype
for `func_800A1070` (in `include/main_api.h`). The frame matches (656 bytes, slots 0x64..0x8C, with a 8-byte `pad[2]`).
Open: `D_8011ECA0` is kept in `$a1` by the original (loaded before the loops, reloaded after `AddPrim`) and the induction
spill is at 0x54 (draft: 0x58). See wiki/matching-notes.md, "T-19009".
