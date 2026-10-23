# T-25105: main func_8005557C (src/main/80053650.c)

Result: MATCH, 1212 bytes (303 words), literal default C. One FAKE (two spellings of the bank address).

## Start state

`INCLUDE_ASM`. The only earlier bodies were m2c drafts (T-14500/T-16100/T-17000, 523 rows, untyped
pointer walks). The function is the game-save writer: it copies GameState fields into bank `arg0`
of the card image (`D_80123120 + arg0 * 0xF00 + 0x200`), then stores the bank's byte sum (the same
tail as `func_80054FC0`).

## What closed it

1. **Bank layout.** `SaveGameView` in `include/game.h` (bank offset = asm offset - 0x200), plus two
   copy records: `GsBlockC4` (0x7C bytes, word-aligned: the asm copies GameState +0xC4 with lw/sw) and
   `GsBlock56C` (0x100 bytes, byte-aligned: lwl/lwr). The GsWord arrays (+0x54C, +0x66C, +0xF64,
   +0xF68) are copied with `lw at; sw at`, so they are 4-byte record copies, not s32 assignments.
   First typed body (one `bank` local, `u32 offset` checksum counter): funcdiff 425 rows.
2. **Unrolling: one counter for all loops.** IDO unrolled the 256-trip record loop (+0x75E) by four;
   the original keeps it rolled. A loop stays rolled when its counter is used again later. Truncation
   test: with a shared `i`, only the last loop of the function is unrolled, whatever it is. The
   original unrolls only the byte sum, so the byte sum uses the same counter as the copy loops.
3. **Counter type u32.** With `s32 i`, the bound `0xF00` of the byte sum is the same s32 constant as
   the multiply in `arg0 * 0xF00`. uopt keeps it in a register for the whole function and the bank
   address becomes `li t0,0xF00; multu`. With `u32 i`, the compare constant is unsigned and the
   multiply stays a shift sequence.
4. **Two bank bases.** The original computes `arg0 * 0xF00 + D_80123120` twice: a2 at the entry
   for the straight-line stores, the record copies and the byte sum, and a3 again before the first
   loop for all loops. Both are spilled around `get_k_speed`. Writing the straight-line accesses as
   `((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->f` and the loops as
   `((SaveGameView *)&D_80123120[0x200])[arg0].f[i]` gives the two values (FAKE: the real spelling is
   unknown).
5. **Frame slot.** The spills are at 0x2C/0x28: four declared scalars. `GameState *g = &D_800E6280;`
   (as in `func_80054E6C`) is the fourth. A `u8 *buf = &D_80123120[arg0 * 0xF00]` local used for the
   straight-line stores also matches.
6. **Signedness.** Six bytes were declared `s8` in GameState (`unk_03C`, `unk_F6C`, `unk_F6D`,
   `unk_F6E`, `unk_F70`, `unk_F73`) and loaded with `lb`. Every direct load of them in the game's asm
   (main and overlays) is `lbu`, and src has 63 `*(u8 *)&...` views of them. They are now `u8` in
   `main_api.h`; the full build keeps all 27 hashes.

## Measured negatives

- `bank` local for every access (byte offset, integer `(u32)D_80123120` arithmetic, pointer
  arithmetic, `&D_80123320[arg0]`): 425 to 538 rows. The local becomes a register holding
  base + 0x200.
- One spelling for both stores and loops: one base register (440 rows with the old counters).
- Pointer arithmetic `D_80123120 + arg0 * 0xF00 + 0x200` in the loops: `arg0 * 0xF00` is shared in a1,
  frame 0x48. `sizeof(SaveBank)`, `0xF00U` and `(u32)arg0` in the loop index: same as that.
- `bank` local for the straight-line stores with the array form in the loops: 379 rows.
- A `SaveGameView *banks = (SaveGameView *)&D_80123120[0x200]` local for the loops (with or
  without `g`): 32 rows.
- Separate `u32 offset` counter for the byte sum: the record loop is unrolled.

## Follow-up

Two FAKE comments say the `u8` view is needed because `unk_F6C` is "declared s8 in header"
(`src/ovl/NAME_ENT/80139740.c`, `src/main/8005A0B0.c`). That is no longer true. Those views and the
other `*(u8 *)&` views of the six fields can now be removed. Check each file's hash after removing
them.

Harness used (gitignored, not committed): `build/scratch/t25105c/` held `dis.sh` (preprocess the
TU with NON_MATCHING, cc.py, objdump of the function), `score.py` (difflib rows against the .s) and
`try.sh`. About 1 s per variant, against about 30 s for funcdiff.
