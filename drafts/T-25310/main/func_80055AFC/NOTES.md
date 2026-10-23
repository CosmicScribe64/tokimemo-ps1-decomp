# main:func_80055AFC (T-25310, second pass)

Result: matched, 1396 bytes (348 words), literal default C. The body is in `src/main/80053650.c`.

## Start state

T-25105 left `best.c` at 113 rows: a FAKE `extern volatile u8 D_800E7397` view of the slot byte, frame 0x38
against the original's 0x48, the bank bases in t4/t5 against t5/ra, and the unrolled 256-record loop with
its end pointer in t2 against v0. Its notes named three gaps: the copy propagation of `arg0`, an
unidentified range at colour 11, and the colour order of the unrolled loop.

## What closed it (three causal changes)

1. **Frame (113 to 110 rows).** Every declared scalar owns a slot and spill homes go top-down below them.
   The original's homes (E1 at 0x30, E2 at 0x2C, raw 0x20/0x1C) and frame 0x48 (raw 0x38) mean five words
   of declared locals; the draft had two. Three more words fix the frame and both home offsets.
2. **Slot read (110 to 102 rows, volatile gone).** `g = &D_800E6280;` before the `func_80055A38(0)` call and
   `arg0 = g->unk_1117 & 1;` (the register-cheatsheet recipe for the selector-copy family). The indirect load
   keeps arg0 a copy, so the head is the original's `lbu a0; andi t6,a0,1; ...; move a0,t6` and the bank
   base is one range. With `g` assigned after the call, the multiply is `multu` and the size changes.
3. **The missing range (102 to 0 rows).** Read from the trace: the colour order of the whole function
   follows if something holds v0 in the 256-loop preheader (bb 36, the block after bzero) but not in the loop
   body. Then the start values skip v0 (dest4 v1, sources a0-a3, dests t0-t2), the end constant (loop body
   only) takes v0, the bank+i*8 temporary takes t3, the counter's defs-only range t4, E1 t5 and E2 ra. A
   toy (`x = bzero(...)` with x never read) gave exactly the original preheader. In the unit this needs a
   real prototype, not a cast (a function-pointer cast gives `jalr t9`): `void *bzero(void *p, s32 n);` in
   main_api.h (BIOS A(0x28); every other caller discards the result, so no other code changes; clean
   rebuild 27/27) and `p = bzero(g->unk_75E, 0x800);` with `u8 *p` declared.

Final locals: `GameState *g; u8 *p; u32 i; s32 pad[2];` (pad is FAKE: two unknown frame words).

## Measured negatives (this pass)

- A discarded int or pointer result of bzero (cast, no assignment): no range, no change.
- `i = bzero(...)`: no change (the next `for` kills it).
- Byte-offset forms of the 256-record loop (`i += 8`, `< 0x800`): the loop is no longer unrolled, or 221 rows.
- `D_800E6280.unk_75E[i]` as the destination, pointer walks `*d++ = *s++`, `d[i] = s[i]` with pointer
  locals (toy): the end pointer stays last.
- `g = &D_800E6280;` after the call with `g->unk_1117`: other size (`multu`).
- Unroll rule (toy, confirmed): a counter loop stays rolled only when the block after it assigns the same
  counter again; alternating counters (i, j, i, j) unroll every loop.

## FAKEs left

- `s32 pad[2]`: two unknown frame words.
- `p = bzero(...)` with p never read: the original reads bzero's result, its use is unknown.
- The two bank spellings (as in func_8005557C).
