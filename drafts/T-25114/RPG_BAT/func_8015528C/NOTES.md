# T-25114: RPG_BAT func_8015528C

Result: matched. The function is default-build C in `src/ovl/RPG_BAT/80154430.c` (1500 bytes, 375 words).
`funcdiff --resolve --version default` prints MATCH, and the RPG_BAT sha1 passes.

## Start state

The file had `INCLUDE_ASM`. The best draft was T-24522's `drafts/T-24522/RPG_BAT/func_8015528C/body.c`
at 4 funcdiff lines. Measured again in the current tree (with the func_80154DD8 guard active), it had the
same 2 instructions, both commutative `addu` instructions with exchanged operands:

- word 6, initial progress address: target `addu s8,v1,t7` (base first), draft `addu s8,t7,v1`
- word 42, correction-arm phase address: target `addu s7,v1,t3`, draft `addu s7,t3,v1`

The post-join phase address (`lui t1; addiu t1; sll t9; addu s7,t9,t1`) is index first, and the draft already had it right.

## What decides the operand order (measured with small test files)

cfe's ucode order sets the `addu` operand order, and uopt keeps it:

- `&p[i]`, `p + i`, `i + p`, `p = p + i` and `&A[i]` (global array): index first (`ADD(SHL i, p)`).
- `p[i]` read or written (an `IXA`), `p += i`, and integer sums in source order: base first.

## What closed it (two causal changes)

1. **Progress as subscripts.** `(&D_8015EBAC)[arg6]` for the read, the store and the loop reads, with no
   `progress` pointer. This is an `IXA`, so the address is base first. uopt CSEs it into `$fp` for the whole
   function: 4 lines to 2.
2. **Phase as one range with two operand orders.** In the target, the arm and the join write `$s7` with
   opposite operand orders. A pointer form gives either one expression (`&work[arg7]` twice: one range, but
   both index first) or two ranges. Every base-first pointer form in the arm (`phase = work; phase += arg7`,
   `work[arg7]` subscripts) made the arm its own range. That range took `$v0` and pushed
   progress/progress_raw/base to s7/v1/t0: 230 lines. Integer address sums,
   `(s32 *)((s32)&D_8015EBAC + arg7 * 4)` in the arm and `(s32 *)(arg7 * 4 + (s32)&D_8015EBAC)` after the
   join, are one uopt expression (`cvt(add(lda, mpy(arg7, 4)))`, 2 defs, 5 uses, priority 2.667, `$s7`).
   Each def still emits its own source order: 2 lines to 0.

## Controls (regdiff draft object against the original, aligned word rows)

- arm `phase = (s32 *)((u32)work + ...)` with the join still `&work[arg7]`: 115 word rows (230 lines)
- arm `phase = work; phase += arg7;`: 115 rows; arm `work[arg7]` subscripts: 115 rows
- `phase = work` at the top, arm `phase += arg7`: 376 words, 343 rows (phase becomes a local in `$v0`)
- without the T-24522 `(phase_sum % 3) * 1`: 361 words, 294 rows (still needed)
- the two `start < 0` tests merged after the if/else instead of the shared `goto` label: 18 rows
- neutral, both kept: `u32` casts or `u8 *` byte offsets instead of `s32` integer sums; no `work` local
  (direct `(&D_8015EBAC)[i]`); no `progress_raw` local (`-=` on the subscript); `for` instead of `do`/`while`;
  no redundant `(s32)` casts on the call arguments

## FAKEs left in the source

- `(&D_8015EBAC)[n]` views and the integer address sums (T-25114): the header declares D_8015EBAC as a
  scalar, while it is an int array (indices 0x32..0x35 are D_8015EC74..D_8015EC80, which func_80154DD8 sets).
- `(phase_sum % 3) * 1` (T-24522, T-25114).

## Method notes

regdiff with `--src` on a scratch copy of the file and a separate `--scratch` directory gave a full measurement
in about 25 s (funcdiff takes 60 s). The traced uopt in the main tree's `build/scratch/t25012/r9-uopt` was copied
into the worktree's `build/`.
