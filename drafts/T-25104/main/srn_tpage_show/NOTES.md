# T-25104: main srn_tpage_show (806 words, 3224 B)

Matched. `src/main/8004C3F0.c` compiles `srn_tpage_show` as plain C in the default build
(`funcdiff --resolve --version default`: MATCH, 3224 bytes; all 27 SHA1 checks pass).

## Start state

`INCLUDE_ASM`. Best known body: `drafts/T-24046/srn_tpage_show.c` (aligned cost 504, 812 against
806 words, frame 120 right). Measured as a guard: 592 funcdiff rows.

## What closed, one cause each

1. **Switch case layout** (first divergence, word 20). The target stores `pageWidth` (0x68) before
   `divisor` (0x6C) in each case, with the divisor constant numbered first. Swapping the two
   assignments renumbers the constants (score 518, worse); putting both on one line,
   `divisor = 4; pageWidth = 0x40;`, gives the target order (score 494). FAKE layout.
2. **No `(s8)` casts on the V coordinates.** The T-24046 body casts `poly->v0/v1 = (s8)(...)` in
   the middle and right pages. In the middle page that built a real `sll/sra 0x18` sign extension and
   a separate common subexpression with its own spill slot (0x38) and reload (6 extra words). Without
   the casts: 807 against 806 words, score 57, funcdiff 57 rows.
3. **libgpu macro shape.** The 57 rows left were store order inside each quad (u3 store, `li a2,0x3C0`
   and `move a3,zero` placement) and the v0/v1 tie of the seam-column U values (+3/+4 and +7/+8
   swapped). Writing each quad's colour, UV and XY stores as one comma statement in the
   setRGB0/setUV4/setXY4 field order (`u0, v0, u1, v1, ...`, `x0, y0, x1, y1, ...`) closes all of
   it at once: as1 schedules by source line, and the u0-first ucode order makes the +3 value the
   first mention (v0). This is the register-cheatsheet "UV/XY macro order" row (TACO func_80140ADC,
   T-19014) and the TACO 8013B170 setUV4 precedent (T-24200). Score 0, MATCH.

The comma statements may span several lines (one vertex pair per line): IDO's line number is the
statement's first line, so the multi-line form matches too and is what `src/` uses.

## Cleanup controls (all still MATCH)

Removing the m2c casts `(u8)`, `(s16)`, `(s32)` and redundant parentheses, and casting the
`GetWorkBase` result to `POLY_FT4 *`, changes nothing.

## Negatives

- Assignment order `pageWidth` before `divisor` in the cases: worse (constants renumber).
- `divisor = 4;` and `pageWidth = 0x40; break;` grouping: no change from the separate-line form.

## Lesson

For a repeated POLY_FT4 builder the per-field stores of a draft follow m2c's scheduled order; try the
libgpu macro shape (one comma statement per macro, field order) before any register work. Narrowing
casts on byte fields that m2c adds can create real `sll/sra` sign extensions when they wrap a common
subexpression.
