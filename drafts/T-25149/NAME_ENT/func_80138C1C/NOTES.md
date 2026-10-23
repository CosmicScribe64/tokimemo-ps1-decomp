# T-25149: NAME_ENT func_80138C1C (740 bytes): matched

Start: plain INCLUDE_ASM in the tree plus a stale guard; best known body was
`drafts/T-24515/func_80138C1C.c` (pointer walks `p`, volatile space reads,
FAKE pad), measured here at 181/185 words, 104 funcdiff rows.

Result: literal match, default C, 740 bytes (funcdiff `--version default`
MATCH, NAME_ENT.bin sha1 OK). The body is in `src/ovl/NAME_ENT/80132000.c`.

## What closed it, in order

1. First divergence: the store block. The original loads the second byte
   (`lbu t9,1(v1)`) above `sb a0,0xD4(v0)` and leaves the `beqz` slot empty;
   the draft stored first and as1 put the `addu` in the slot. A user pointer
   `p` gets no `.noalias` mark, so as1 cannot hoist the load. Index loops over
   the arrays (`D_8014CC74[i]`, `i += 2`, `i < 6` / `i < 10`) give
   strength-reduced pointers that as1 may move past the stores, and also give
   the `D_8014CC7A`/`D_8014CC82`/`D_8014CC8E` end constants and `bne` on the
   third loop by themselves.
2. Index loops alone let uopt merge the second-byte loads (one `lbu a1`). The
   stores must go through a `GameState *gs = &D_800E6280` pointer: they may
   alias the arrays for uopt (three separate loads, as in the original) but
   not as1. 176 -> 185 words.
3. Space bytes: with stores through `gs`, plain reads of `D_800B3644[0]` and
   `[1]` written straight into the tests are promoted by uopt into `a2`/`t0`
   with separate `lui`/`lbu` pairs, the original's `lui t0; lui a2 ... lbu a2;
   lbu t0` order and the `beq t6,t0` operand order. Local copies (any type)
   are compared register-first (`beq t0,t6`): ugen puts a register variable
   before a loaded expression, two loaded expressions stay in source order.
   Locals also cost 8 frame bytes.
4. `for (i = 0, n = 0; ...)` puts `move a1,zero` after the `la`s.
5. Frame: `u8 buf[0x20]` gives 96 bytes with buf at sp+0x34 (FAKE: size
   unknown; 0x18 gives 4 rows).
6. Date tail: the third field of each date spends one more ugen temporary in
   the original. `(x + 1) & 3` on the two-bit `unk_09` fields does it (as1
   deletes the AND); 62 rows -> 0. Casts `(u8)`/`(s8)` change nothing.
7. Return type: void matches this function, but the caller `func_80138F60`
   needs the `s32` declaration in NAME_ENT.h, so the definition stays s32
   with no return (FAKE).

## Negatives measured

- Swapping operands, `| 0`, `* 1`, `^ 0`, `<< 0`, `!(a == b)` forms: no
  change to the compare order (canonicalised).
- Space locals as u8/u16/s16/s8/u32/s32: never folded back, always
  register-first compares.
- `c1 = p[1]` local: one load, 164 words.
- Reversed store order in the store block: uopt merges the loads.

Helper scripts used (not committed): a body-substitution compile and diff
(`build/t25149/try.py`) and an IDO `cc -S` listing of a minimal TU
(`build/t25149/ccs.sh`).
