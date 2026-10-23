# T-25145: ENDING:func_801330E4 (796 B, 198 words): matched

Result: default C in `src/ovl/ENDING/80133030.c` matches (`funcdiff --resolve --version default`
MATCH, no INCLUDE_ASM left). Commit `T-25145: match ENDING:func_801330E4`.

## Start state

Best body: `drafts/T-24924/ENDING/func_801330E4/body.c`, 92 diff lines (frame already fixed by
T-24924). First divergence at row 84, the level block: the original loads the f9 flag word into
`a0`, shifts it into `v0` (`sll v0,a0,20`), extracts into a temporary (`srl t1,v0,29`), uses the
temporary in the sum and copies it to `v0` in the `bnez` delay slot (`or v0,t1,zero`). The draft
put the word, the shift result and f9 in `v0` directly.

## Tools used

`regdiff` works with the traced uopt of another lane mounted read-only as scratch
(`TOKIMEMO_SCRATCH=.../opencode/scratch-T-24705`, `--scratch /scratch/<own name>`). It compiles a
full-TU copy (`--src`) in about 15 s; `funcdiff --no-build --built /scratch/<name>/func_801330E4/draft.o`
gives the exact row count of that object. `cc -S` listing plus a FIFO simulation of ugen's free list
(see "ugen temporaries") explained the remaining registers.

## Causes and fixes, in order

1. **level saved at 0x54, not 0x56** (rows 113-114). An s16 declared right below `points` lands at
   0x56. Declaring `i` (s16, register-allocated, slot unused) after `points` and before `level`
   puts `level` at 0x54 with the frame unchanged (0xB0) and three leading pad slots instead of
   one more. 92 -> 88.
2. **word in `a0`, copy-back of f9** (rows 84-104). regdiff showed the draft's f9 expression
   `shr(shl(S8+1072,20),29)` in `v0` (2 uses), and the global word S8+1072 uncoloured ("net zero:
   1 use"). The target shape is ugen's destination forwarding (T-25034): the f9 CSE requests `v0`,
   forwards it to the shift child; when the load child is in its own register (`a0`), the shift
   goes into `v0` and the srl result coincides, so ugen takes a temporary and copies back. The word
   must be a coloured global with one use, which the original compiler allowed and IDO 5.3 does not
   (wiki/original-compiler.md rule 3, the T-0018 gap). A second read in an empty test,
   `if (D_800E6280.unk_1BC[0xB].unk_0C.u) {}` (FAKE), gives priority 0.5 and `a0`. Removal control:
   90 rows.
3. **sltiu/xori in place** (rows 90-96). `(x == 0 ^ 1) * 2` emits `sltiu t2; xori t3,t2`; the
   target has `xori t3,t3`. `(x > 0) * 2` (same code as `(x >= 1) * 2`) gives the in-place pair,
   as `(t >= 2)` does in DATE `func_801324DC`. It also removed the extra 199th instruction.
   Control with the matched body and `== 0 ^ 1`: 199 words, 85 rows.
4. **sum association** (rows 97-99). One-statement sums are reassociated by uopt; only
   `B2 + (A2 + F)` gave the target's `(F + A2) + B2` with F first; 82 rows, 40 register rows left.
5. **ugen temporaries of the block and of the tail** (rows 84-127 and 156-194). A FIFO model of
   ugen's free list (allocate from the head, append on last use) reproduces the draft listing
   exactly. Working back from the target registers after the clamp (`t2 t3 t4 t5 t9` for the
   0x4D byte, rem, index, address, constant 11) requires the free order t2 t3 t4 t5 t9 t8 t0 t7 t6
   t1, i.e. the f9 + A2 sum freed before B's byte. That is the shape of separate statements:
   `level = f9; level += (A > 0) * 2; level += (B > 0) * 2;`. This matched the whole function,
   tail included.

Last FAKEs: `s32 pad[4]` (four unused word slots above `rank`; frame 0xB0, `chance` at 0x30;
`s32 pad[4]` first or after `p` both match) and the empty flag-word test.

## Negatives measured (do not repeat)

- A local `u32 n` / `s16` / `u16` / `s8` / `u8` / `s32` holding f9 (`n = f9; level = n + ...`):
  every type adds a range or a conversion; 100-108 rows, often 199-202 words.
- A local copy of the word, `w = ...unk_0C.u; ((w << 20) >> 29)` twice or through `n`: the word
  goes to `a0` (copy propagation gives the global two reads), but the shift pair is recomputed
  for the test instead of kept in `v0` (199 words).
- `(x != 0) * 2` and `!!x * 2`: `sltu` instead of `sltiu/xori`. `!(x == 0) * 2`: two `sltiu`.
- All six operand orders of the one-statement sum and the six `X + (Y + Z)` forms: 82-plus rows;
  only `B2 + (A2 + F)` has the target association.
- `(D_800E6280.unk_F5F & 0xFF) == 0xD` / `(u8)` cast on the F5F test: shifts the F5F byte itself
  (68 / 82 rows); the missing temporaries are inside the sum, not before it.
