# T-25101: main func_8005215C

Result: default-C match, 1260 bytes (316 words). `funcdiff --resolve --version default` prints MATCH; the function has no INCLUDE_ASM line. The body is in `src/main/8004F870.c`.

## Start state

The function was plain INCLUDE_ASM. Earlier checkpoints were T-25021 `body.c` (138 aligned) and T-25022 `partial-reload.c` (127/127, diagnostic only). Both cached the mode byte (`GameState.unk_03C`) in locals and used m2c's int arithmetic for the table lookup. I did not take either. I started over from the matched sibling `xa_wait`, which has the same prefix.

## What closed it, in order

1. **Mode byte as a promoted global (183 rows to 49).** Every mode read is `*(u8 *)&D_800E6280.unk_03C`, written in place with no local. uopt then keeps the byte as one promoted global range in v0. It loads the byte at the start of each region and reloads it after every call (`k_disp_inc`, `func_80045414`, `k_disp_goto_line_end`), which is the target's topology. `(u8)D_800E6280.unk_03C` on the s8 field adds `cvt` expression ranges, and those cache the value across calls. That cache was the T-25021/T-25022 blocker; T-25022 noted that its signed-read diagnostic already had the target's read/reload topology.
2. **Frame (49 to 45).** `s32 pad[2]` (FAKE, as in xa_wait) gives the 64-byte frame. The homes r +0x34, a +0x30 and k +0x28 already came out right.
3. **Colours of F61 and F62 (45 to 27).** In the target, F62 is in a0 and F61 in v1; xa_wait and the draft had v1 and a2. The cause is the plain argument in `k_disp_switch(D_800E6280.unk_F61, 0)`. It gives the `cvt(F61)` range an a0 preference in that block, and the cheapest colour then is a2 (cost 0.1). `F61 + 0`, `* 1`, `| 0`, `^ 0`, `<< 0` and `& -1` all give v1 for F61 and a0 for F62, and still compile to `move a0,v1`. `(u8)`, `(u16)`, `(u32)`, `(s32)` and the `*(u8 *)&` view change nothing, and `& 0xFF` keeps an `andi`. Kept: `+ 0`, FAKE. Diagnostics that ruled out the alternatives: removing everything after the first if/else leaves the colours unchanged; removing `t`, splitting `a`/`b`, the unsigned compare spellings, and `u8`/`u32` types for `now`/`ph` change nothing.
4. **Table lookup (27 to 0).** The target computes `D_80125BFC << 3` once, in a ugen temporary (t4), and shares it between `BF8 + off` (a3) and `BF4 + off`. Spellings that failed:
   - int arithmetic `D_80125BF4 + D_80125BFC * 8`, `CallPair8` casts or pointer views in one condition, an `off` local: the shift becomes a uopt CSE range in v0, and every later ugen temporary is one register low.
   - `CallPair8` indexing in separate statements, an `e` local, or an embedded `(e = ...)`: two `sll`.
   - `[D_80125BFC * 1]` on the BF4 side only: one shared temporary, but in the wrong order.

   Word indexing on both tables, `((s32 *)D_80125BF4)[D_80125BFC * 2]` and `((s32 *)D_80125BF8)[D_80125BFC * 2]` / `[D_80125BFC * 2 + 1]`, matches. It is the same spelling as the matched `func_80052060`, which stores these tables. Not a FAKE.

## Remaining FAKEs (marked in the source)

- `s32 pad[2]`: frame size, as in xa_wait.
- `else if (ph != 0)`: redundant test, as in xa_wait. The target does contain the `beqz`.
- `k_disp_switch(D_800E6280.unk_F61 + 0, 0)`: argument colour preference, see step 3.
- The u8 address view of the s8 mode field: the original most likely had an unsigned mode byte. The other functions in the file use `(u8)` casts, so the header was not changed.

## Negatives (measured)

- Mode cached in locals, or reloaded into locals: the earlier T-25021/22 drafts.
- `pad` removed: frame 56.
- In the lookup, the int, struct-cast and pointer-view spellings above.
- Implicit-int or `s32` return type for the function: no change.

## Method notes

- `tools/regdiff.py -v` and the raw `R9SAVE`/`R9UNIT`/`R9COST` lines in `build/scratch/regdiff/func_8005215C/draft.log` give the allocation order and the colour costs. The uopt listing `draft.o.trace` has the expression table (bit to op) and the "assigned" lines.
- A ring-temporary census of the target (t6..t9, t0..t5 in emission order) showed that the shift was a ugen temporary, not a uopt colour. That ruled out allocator-only fixes.
