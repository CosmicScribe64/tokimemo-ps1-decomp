# T-25146: ETC func_80144224 (796 bytes): MATCH

The C is in `src/ovl/ETC/80143240.c` as the default body (no guard). funcdiff `--version default` prints MATCH, and the ETC sha1 is OK.

## Start state

- The function was INCLUDE_ASM.
- Best earlier bodies, measured in this tree with `funcdiff --resolve --version c` (rows shown as -/+):
  - T-25002 n1 variant: 93/87, frame 0x40.
  - T-25002 body: 106/97.
  - T-24074 `body.c`: 74/88. Its local `frame`, reloaded after every call group, already gives `$a3` and `$t0`, and its `(&D_8015014C)[i]` arm gives the write-backs of `D_80150148` in `$v1`.

## Levers, in the order they cut rows

1. **Arm table as in the sibling.** `D_80150150[D_80150148 - 1]` in the arm gives the original's `D_80150150` base with the -4 offset. It also gives the dead `addiu v0,v0,4` after the second read.
2. **Phase local and spilled product.** A local `x` takes the phase value (`x = rem` in the fade-in arm, `x = 701 - rem` in the fade-out arm). The product `x * 3` is the value that is spilled to 0x28.
3. **Frame counter through a pointer local** (`GsWord *fc = &D_800E6280.unk_1104;`, all reads as `fc->u`). This change made the large cut (64/78 -> 47/46). The frame counter then has one register range: `$a3`, reloaded by uopt after each call group, with 701 in `$t0`. `D_80150148` is promoted only in the remainder-1 arm (`$v1`, with the original's write-backs). `x` stays a real variable in `$v0`, so the original's dead `or v0,v1,zero` appears. The local `frame` copy of T-24074 (`move a2,a3`) disappears.
4. **quot in its slot.** A word view in the test, `(s32)*(u32 *)&quot < 0xF`, keeps quot in 0x30. The test reloads it (`lw t3`), the `func_8014407C` argument reuses `t3`, and the ugen temporaries shift by two to the original numbering (47/46 -> 12 aligned rows, frame 0x40).
5. **v in its slot.** Word stores `*(s32 *)&v = x * 3` put v in 0x28 without an extra spill slot, so the frame is 0x38: MATCH.

The FAKEs in steps 4 and 5 are the same two tricks the matched sibling `func_80143A88` uses for `sp30`/`sp28`.

The final print as `D_80150150[D_80150148 - 1]` also matches. It replaces the `(&D_8015014C)[i]` first-symbol trick.

## Negatives measured here (do not repeat)

- **Direct global reads of the frame counter.** These include the `D_800E6280.unk_1104` field, the `D_800E7384` scalar and the sibling's repeated `% 701U` form. In all of them `D_80150148` wins `$a3` over the whole function (priority 1 against 0.33) and the frame counter is not promoted (144-156 aligned rows).
- **The pointer written as a cast instead of a local:**
  - `((GsWord *)((u8 *)&D_800E6280 + 0x1104))->u` and `(*(GsWord *)&D_800E7384).u`: 90 rows.
  - `(&D_800E6280)->unk_1104`: 154 rows.
- **Local `frame` with reloads (T-24074):** a second range in `$a2` and a recomputed divide in the fade-in arm. Uses of the local inside an assignment's right-hand side are not propagated to the global.
- **Other forms of quot:**
  - `s32` type, `<= 14`: no change; the test still takes the propagated `mflo`.
  - `volatile`, or an address-taken store with plain reads: one extra slot, frame 0x40, and the call reloads quot.
  - `*(s32 *)&quot < 0xF`: -4/+5.
- **Other forms of v:**
  - v as a plain declared local: it gets a spill slot of its own, frame 0x40.
  - No v, with `x * 3` expressions: with quot in memory, one extra slot.
- **Other forms of x, all before the pointer local:** `u32`/`s16` type, `v = (x = ...) * 3`, `x + x * 2`, a self-assignment. None gives the dead copy.
