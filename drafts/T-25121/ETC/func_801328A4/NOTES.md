# T-25121: ETC func_801328A4 (920 bytes): matched

Start: plain `INCLUDE_ASM`. Best earlier body: `drafts/T-24012/func_801328A4.c` (pointer and byte-offset
form, cost 217; 169 differing words in a one-function scratch compile).

Result: literal match in the default build (`funcdiff --version default`: MATCH, 920 bytes), with C in
`src/ovl/ETC/80132880.c`. Rgb555Split now has `u32` channels in `include/ovl/ETC.h`, and
func_80132880 still matches.

## What closed it, in order (diff words in a one-function scratch compile)

1. `next_row` was a declared local. In the original, `row + 1` is a uopt temporary spilled at sp+0x6C,
   below every declared home (`sw t5,0x6C(sp)`, then `lw fp,0x6C(sp)` at the latch). Removing the
   local and writing `(y + 1) % 3` with `y++` in the `for` head moved the spill to 0x6C (169 to 143).
2. Frame: the buffer sits at sp+0x90 with 6 words between it and the RECT at sp+0x3408, and 7 declared
   words below it. Six scalars declared before the buffer put it at 0x90 (143 to 140).
3. Index form over a real `Rgb555Split buf[3][274]` (`buf[(y + 1) % 3][x]`, three loops on `x`), not
   row pointers or byte offsets. This fixed the input and divisor saved registers (s2/s3) and loop 1.
   Row-pointer locals (`cur = buf[y % 3]`) cost 15 or more extra words.
4. Two semantic holes in the pixel tail. The original keeps both stores: `*dst = ...; *dst |= 0x8000;`
   (the store is reloaded through the u16, which gives `andi 0xFFFF`), and there is no `s16 pixel`.
   The three combined channels are locals (`cr`, `cg`, `cb` in v1/a0/a1), with
   `*dst = (cr & 0x1F) | (cg & 0x3E0) | (cb & 0x7C00)`. Together: 167 to 16.
5. Neighbour sum operand order: a sweep of all 24 orders (the same order for all three channels).
   `up + down + left + right` (`buf[(y + 2) % 3][x]`, `buf[(y + 1) % 3][x]`, `buf[y % 3][x - 1]`,
   `buf[y % 3][x + 1]`) gives the original's `((right + up) + down) + left` after uopt's
   reassociation: 16 to 2 (only the spill slot was left).
6. Slot count: 13 declared words in total (6 above the buffer, 7 below). With 10 real locals that
   leaves 3 unknown words, which are the FAKE `s32 pad[3]` (0 rows).
7. Cleanup: `u32` channel fields replace the `(u32)` casts and `100U`, with the same bytes.

## Negatives

- Chained `src = dst = ...` / `dst = src = ...` init: no change, or 3 rows worse.
- Row-pointer locals for loops 2 and 3: 245 words (+15).
- `pad` as 4 words, or 4 separate unused scalars below the buffer: spill at 0x68 (2 rows).

## Tools

`build/s328/fast.py` (not committed; under build/) compiles a minimal translation unit (includes plus
func_80132880 plus the body) with tools/cc.py and diffs the words against the .s (jal targets masked).
It takes about 4 s per candidate, against 70 s for funcdiff. The full-unit funcdiff agreed with it
on the final body.
