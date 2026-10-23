# T-25302: main func_8004A1A0, second pass (628 B, 157 words)

Result: matched, default-build C in `src/main/80049FF0.c` (`funcdiff --resolve --version default`: MATCH, 628 bytes).
One FAKE: the empty `if (slot) {}` before `bg->unk_20 = 0`. Time: about 50 minutes (13:23-14:15 UTC).

## Start state

First pass T-25122 (`drafts/T-25122/main/func_8004A1A0/`): clean draft 64 rows, 157/157 words; boundary draft
(`do {} while ((u32)slot < 0);` before `bg->unk_20`) with every uopt colour of the original but 159 words
(82 +/- lines). Its diagnosis: after the split uopt emits no `unal` for the map pointer, so ugen writes no
`.noalias a0,v0` / `.noalias a0,sp`, and as1 keeps two `lw tX,0x74(sp)` slot reloads and the header order.

## What closed it (each measured on its own, whole-unit `funcdiff --resolve --version c`)

1. **The cause of the missing `unal` (read in the uopt reconstruction, `uoptemit.c` `base_in_reg`, and
   `uoptkill.c` `findbaseaddr`).** uopt writes `unal` pairs when it puts a base address in a register and the
   store's `baseaddr` is known. `baseaddr` is fixed when the ucode is read: `findbaseaddr` of the store's address
   expression. In the block that holds the pointer local's definition the load of `map` is replaced by its local
   copy (`ixa(lda D_80122120, slot*16)`), so the base is the `lda` and noalias holds against `bg`'s base and
   against `sp`. In a later block the address is the variable `map` itself: base unknown, so no `unal` at all,
   not even for `sp`. The first pass's boundary drafts all kept `map = &D_80122120[slot]` at the top, before
   the split. Moving the definition after the split (`map = ...` right after the boundary): 82 -/+ lines down to 4 rows.
2. **Spill homes (4 rows left: cells/index at sp+0x54/0x50 against 0x58/0x54).** With the split, uopt gives the
   `bg` address (one CSE across the two blocks) a stack temporary even though it lives in `$v0`. Declared
   locals take frame slots first, in declaration order (bg 0x6C, map 0x68, row 0x64, ...), so the temporaries
   start one word lower. The original has four locals with row third. Dropping the `map` local (stores written
   `D_80122120[slot].cellw = ...`) with locals `bg, cellh, row, col`: MATCH. Locals `map, cellh, row, col` with
   `bg` indexed also match; the `bg` pointer form is kept because it reads better.
3. Clean-ups that still match on top: natural GsBG field order (00..1E, then 20 after the split), natural GsMAP
   order (cellw, cellh, ncellw, ncellh, base, index), `bg->unk_14 = (u32)&D_80122120[slot]` (the T-25010
   `(u32)(D_80122120 + slot)` is no longer needed: the two computations sit in different uopt blocks), and loop 2
   as plain `if (row * w + col > 0x40) printf(...)` (the T-25122 `continue` is no longer needed).

## Constraints measured (negatives)

- `bg->unk_20 = 0` must be after the split (before it: 91 rows); `bg` must be live in the second block.
- `do { scalex; scaley } while (0)` instead of the empty test: 3 rows (the 0x1000 `li` moves; its range must
  start in the entry block). `do { rotate; map stores } while (0)`: 159 words (62/64 -/+ lines).
- `if (count) {}` / `if (count > 0x40) {}`: 56/55 -/+ lines (count stays live and 0x80 loses `$a2`).
  `if (slot >= 2) {}` matches like `if (slot) {}`.
- `h * 0x78` without the `cellh` local: shifts instead of `multu` (54/55 -/+ lines).
- Loop 1 `tp + row * 32 + col`: 10 rows (keep `tp + col + row * 32`, T-25122).

## Left

Nothing for this function. The FAKE is the empty test; a natural construct that ends a uopt block between
`scaley` and `rotate` without code was not found (`do {} while (0)` forms move the constant load).

## Lesson (for the playbook)

A late-folded block split loses uopt's `.noalias` for a pointer local defined before the split: define the
pointer (or index the array) after the split. A split also adds a stack temporary for an address CSE that
crosses it, so later spill homes drop by one word; the original then has one declared local fewer.
