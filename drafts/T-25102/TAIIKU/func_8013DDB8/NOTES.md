# T-25102: TAIIKU func_8013DDB8 (281 words, 1092 bytes)

Result: matched, default C, commit `T-25102: match TAIIKU:func_8013DDB8`. Clean rebuild 27/27 sha1 OK;
check_headers, migrate_globals --check and sync_protos --check-branch pass.

## Start state

Plain `INCLUDE_ASM`. The only earlier bodies were automated m2c drafts (T-14500 score 412, T-16550
score 459) with word stores where the asm has `sh`, so no measured body to build on. The case 2/5
grid loop is the loop of `func_80139AB0` (matched earlier in this package), which served as donor.

## What closed it, in order (rows from funcdiff --resolve, NON_MATCHING C)

1. Donor loop for case 2/5, and the case 0/3/6 loop as the same 4-trip inner loop with
   `-160 + j * w` / `-160 + (j + 1) * w`. uopt unrolls it completely: the stores, the shared
   values and the `li s1,3; multu s1,a3` all come out as in the original. First draft: 71 rows.
2. `(h << 4) * 3` instead of `h * 16 * 3` (uopt folds the latter to `* 48` and strength-reduces it);
   the constant 3 then sits in `s1` like the original. `j * w` instead of `w * j` fixes the
   multu operand order.
3. A pointer local `p = &D_801499C0` for the half reads (the original rematerializes the address
   in `s2` after each call), with the word read and stored directly (`lui v1; lw s0` at the top
   then matches). A local `h` for the high half; `D_80122DD0` y store as `h - 0x100U`: with a
   signed subtraction uopt reuses the y value for `h >= 0x100` (`bltz a1`), which moves every
   register of the 6-step block. 17 rows.
4. The last 16 rows were order only, in three places: the step load (`D_80122EC4`) scheduled after
   the camera stores in two blocks, and the `lbu` of `D_801207EB` hoisted above the store to
   `D_8012080E`. Captured as1 input (`drafts/T-24900-as1` capture shim, stock `-xbbdbg 8` log):
   ugen emits the step load first; as1 moves the camera stores above it because they have no
   dependency on it. as1 keeps the order of a load and a store only when both use one symbol.
   Probe: reading the step as `*(s32 *)(D_80122DD0 + 0xF4)` removed 12 of the 16 rows at once.
   0x80122EC4 is offset 0xF4 of the 0x100-byte block main bzeroes at `D_80122DD0`, so the original
   addressed it through that block. Likewise 0x801207EB..0x80120812 are fields 0x03, 0x1A, 0x1C,
   0x26 and 0x2A of actor record 102 of `D_8011ECD0` (0x44-byte records, the file's
   `TaiikuActor44` view): with record fields, the sh/lbu order matches.
5. With the record view, the 6-step block's `D_80120804 = D_80120802 = x` chain no longer keeps the
   value in a promoted register. Writing `[102].unk_1A = x; [102].unk_1C = [102].unk_1A;` promotes
   `unk_1A` (its store sinks to the end of the block, as in the original) and stores `unk_1C`
   from the same register. MATCH.

## Declarations added

- `include/ovl/TAIIKU.h`: `TaiikuWork` (camera at 0, step at 0xF4) as a view of `D_80122DD0`;
  prototypes of `func_8013E1FC`, `func_8013E894`, `func_8013EC60` (called before their definitions).
- The other functions of the file still read the step as `D_80122EC4`; they were not touched.

## Negatives measured (do not repeat)

- `h * 16 * 3`: uopt folds it to `* 48` and ugen emits `sll; subu; sll`, no `multu` with `s1` = 3.
- `h` as a local with `y = h - 0x100`, `h + -0x100`, `-0x100 + h`, `h > 0xFF`, `s16 h`: all keep the
  `bltz` reuse. `y = h; y -= 0x100;` and `y = -(0x100 - h)`: worse. Only an unsigned subtraction
  (`0x100U` or `(u32)0x100`) avoids it.
- Reading the half through `p` at every use (no `h`): reloaded after each s16 store (alias).
- `p->word` for the word accesses: 27 rows.
- Separate `extern s32 D_80122DD4/D_80122DD8` for camera y/angle: no change in the order rows.
- Moving the step increment in the source: no change (uopt places the promoted load and store).
- `*(s16 *)(&D_801207EB + 0x23)` for the record store: worse than the record view.

## FAKE marks in the source

- `p`: halves read through the pointer, the word directly.
- `h - 0x100U`: unsigned subtraction, same bits.
