# T-25113: ETC func_8013D570 (src/ovl/ETC/8013BEF0.c), 2352 bytes

Result: matched as default C (funcdiff `--version default` MATCH; no INCLUDE_ASM line is left for it). The body is in `src/ovl/ETC/8013BEF0.c`. One FAKE, for layout only: one source line per vertex (see step 3).

## Start state

INCLUDE_ASM. The best earlier bodies were in `drafts/T-24078/`. Measured again here with a private full-unit compile: `body.c` aligned cost 604 / 670 rows / 592 words / frame 0xA0; `indexed-uv-right.c` 618 / 846 / 592 / 0x90; `height-negated.c` 616 / 846 / 588 / 0x90 (original 588 words, frame 0x90). All of them walk a `POLY_FT4 **` cursor and keep explicit page and UV counters.

## What the original shows

- Saved registers, in priority order: s0 `&poly[i]`, s1 arg0, s2 `&D_800E6280`, s3 80, s4 `i`, s5 68, s6 mode, s7 136, fp page. The two UV values sit in stack slots 0x50/0x54 and grow by 8 at the latch. 160 is loaded with `li` at each use.
- The preheader sets s0, s3, s4, s5, s7 and fp in register order, after the two UV stores. That is how uopt emits strength-reduction and range-entry setup, not source statements. So `&poly[i]`, the page (`0x140 + i * 0x80`) and the UVs (`i * 8 + 0x18`, `i * 8 + 0xA0`) are induction variables derived from `i`, as in the matched sibling func_8013D32C. The arm products (`i * 0x88`, `arg0 * i * 0x44`) stay multiplies, because they are not executed on every iteration.

## Steps (one change each, measured)

1. **Write it like func_8013D32C.** `for (i = 0; i < 2; i++)`, `poly[i]` everywhere, the page as `0x140 + i * 0x80`, the UVs from `i`, and the coordinates as full expressions that uopt CSEs (`0x18 - arg0 * 11 / 80 + i * 0x88 - arg0 * i * 0x44 / 80`, and so on). This took the cost from 604 to 20 (22 rows, 588 words, frame 0x90). The saved-register set, the 160 rematerialisation and the arm arithmetic all match. The T-24078 problem (constant 160 taking a saved register and pushing the page out) does not occur: with the cursor and explicit counters gone, 160 never wins a register.
2. **A height local `h = 0xA0 - arg0;`** for y2/y3 (22 to 4 rows). The UV induction spills were 4 bytes high (0x54/0x58 against 0x50/0x54). One more declared scalar after `poly` moves them down: an unused `pad` gives the same 4 rows, and `h` is the natural one. Declaring the extra scalar before `i` gives 6 rows. Swapping `i` and `poly` gives 24.
3. **One source line per vertex** (4 rows to 0). The last two rows were one swapped pair in case 0: `addiu t6,t5,-101` (y2) and `subu t1,t8,a1` (the `160 - Q11` head of x3). `cc -S` shows that uopt emits `160 - arg0` and `160 - Q11` right after the x2 store, attributed to the x2 line, so as1's per-line scheduling decides the order. Joining x2, h and y2 on one line in case 0 matches. Writing every vertex as one line (`x = ...; y = ...;`, with `h` between x2 and y2) matches in all four arms, and the same layout for the UV pairs and the colour line still matches, so the source uses that layout throughout. It is marked FAKE because the original layout cannot be proven.

## Negatives (measured, all 4 rows unless noted)

- Spelling inside case 0 changes nothing: `base - 0x65 + h`, `h + base - 0x65`, `base + (h - 0x65)`, `+ -0x65`, parentheses around `0xA0 - arg0 * 11 / 80`, x3 as `0x18 - Q11 + 0x88 + ...`.
- Line joins that leave x2 and y2 on separate lines: h with y2, y2 with x3, x2 with h, y1 with x2, h/y2/x3, y2/x3/y3, x3/y3.
- `h` on its own line before x2: 104 rows. `h` at the top of the arm: 188 rows. `h = ...;` first on the x2 line: cost 518.
- autotweak (300 s, 2 jobs) on the 4-row guard found nothing (2 to 2, regdiff: order-only).

## Levers worth reusing

- When the original's preheader sets pointers, page counters and constants in register order, they are strength-reduction temporaries: write the indexed form (`p[i]`, `base + i * k`) instead of cursors and counters. This changes which constants win saved registers.
- For an order-only pair inside one block, read `cc -S`: uopt may emit a subexpression early under the previous statement's line, and then the source line grouping decides the as1 order.
