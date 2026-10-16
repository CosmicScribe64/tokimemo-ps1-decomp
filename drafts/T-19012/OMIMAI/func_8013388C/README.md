# OMIMAI func_8013388C typed draft (T-19012)

Girl-indexed animation selector (11-way `switch` on `unk_F5F`, 2,000 bytes). Restored to `INCLUDE_ASM` in
`src/ovl/OMIMAI/801331B0.c`: the draft has the original's instruction count within 2 (486 of 488) and
the same control flow, but about 590 funcdiff lines differ, all from register allocation.

Needs `void func_801340E4(void);` in `include/ovl/OMIMAI.h` (the function is defined later in the file).
Fields: `D_800E71DF` = `unk_F5F`, `D_800E71E8` = `unk_F68.w`, `D_800E6374` = `unk_0F4.h`, `D_800E62BE/BF` =
`unk_03E/03F`, `D_800E7378` = `unk_10F8`.

Findings:
- `if (func_800AE0D0() % 2)` keeps the signed remainder (`bgez; andi 1; beqz; addiu -2`); `(... % 2) != 0` is folded to `& 1`.
- Open: the original reads each of `D_800E71E8`, `D_800E6374` as its own scalar. It keeps the F68 word in `$v1`
  (a promoted range, one per case), the masked selector in `$v0` and `D_800CA148` in `$a0`. With GameState members
  the word is not promoted ("net zero" in `tools/regdiff.py`), the selector lands in `$v0` through a `move`, and
  `D_800CA148` takes `$v1`; the base `lui` of `unk_0F4` is hoisted above the case-1 and case-3 branches.
  Casts (`*(s32 *)&...`) and `volatile` views do not change this. See wiki/data/t0018-cases.md.
- Frame: a 4-byte local declared before `u8 sp2B` (`s32 pad;`, a FAKE if kept) moves the byte from `0x2F(sp)` to the original's `0x2B(sp)`.
  The draft in `body.c` does not have it.
- Other structure differences left (28 places of the opcode stream): the second `(h >> 12)` is not shared in the original
  (`srl t1,v1,12` again before the compare with 10), `unk_03E` is loaded twice in the case 4 chain, and the delay slot
  of the case 1 and case 3 branch holds the `lui` of `D_800E6374` instead of a `nop` before the `andi`.
