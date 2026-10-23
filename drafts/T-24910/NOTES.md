# T-24910 RPG_BAT func_80154DD8: draft notes

Status: **not matched**. Best draft **102 funcdiff rows** (`--resolve`, NON_MATCHING C, full TU).
The tree is left on `INCLUDE_ASM` (row count is far above the ~30-row guard limit). The draft is in
`drafts/T-24910/RPG_BAT/func_80154DD8/body.c`; paste it into `src/ovl/RPG_BAT/80154430.c` under
`#ifdef NON_MATCHING ... #else INCLUDE_ASM ... #endif` and add the three `extern s32` declarations in
the body (they are not in `include/ovl/RPG_BAT.h`).

`drafts/T-24910/dump.py` (Docker: `python3 drafts/T-24910/dump.py`) writes the guarded C object's
disassembly to `build/T-24910/dd8.s`; that plus `funcdiff --resolve` was the loop.

## What the draft is

Scalar form of the function (not the `RpgRec18`/`D_8015EC58` aggregate form of T-24027):

- `D_8015EC5C`, `D_8015EC64`, `D_8015EC9C` as plain `s32` externs; each access keeps its own `lui`,
  as the original does.
- The selector is `switch (*(s32 *)&D_8015EBAC)` (FAKE: scalar-address view, T-17114 idiom). This is
  the one edit that gained rows (113 -> 126 -> the aggregate-free baseline).
- Two locals `x`, `y` reused by case 1 and case 3 (m2c's four `temp_a1*`/`temp_a2*` are the same two
  values; declaring them separately costs 0x20 frame and 11 rows).
- Counters are direct `D_8015EC9C++;` + a re-read in the test (m2c's `temp_a0*` locals give the same
  code).

Progression measured this run: 113 (T-17100 body + read the state global twice) -> 140 (aligned
rows) -> 126 (selector view) -> 102 (x/y reuse; direct counters).

## Remaining diff, classified (all 102 rows)

1. **Frame 0x48 vs 0x58, spills 0x40/0x44 vs 0x44/0x48** (2 rows). No source change tried moves it:
   unused locals anywhere except the top do not add frame, 4 unused locals at the top give 0x58 but
   leave the spills at 0x40/0x44, 1-4 declared parameters only add homes at 0x48+, a named selector
   local changes nothing. The original's extra 0x10 sits below the two spilled values, so it is
   probably one more spilled value plus a word of padding, or a different outgoing-argument area.
2. **Inner-switch selector load position** (rows 37-46). Original: `lui v1; lw v1, D_8015EBAC` first,
   before the five `D_8015EC74..80` stores. Ours: the pair lands between the third and fourth store.
   A named local (`s32 sel = D_8015EBAC;` before the stores, `switch (sel)`) does put the load first
   but recolours the constant 1 from `a3` to `v1` and costs 14 rows elsewhere (rows 8/9/24/27), for
   116 total. That is the concrete open lever: keep the load hoisted *and* the constant 1 in `a3`.
3. **Unary-minus coalescing** (case 1 rows 134-157, case 3 rows 246-249). Original keeps the
   producer in a ugen temp and negates into the local: `mflo v0; negu a1,v0`, `subu v0; addu v1;
   negu a2,v1; negu a1,v0`. Ours fuses: `mflo a1; negu a1,a1`, `addu a2; subu a1; negu a1,a1;
   negu a2,a2`. Tried and no better: separate `temp = expr; x = -temp;` (102), temps with y negated
   first (122), assignment `x = a - b; x = -x;` (122), y-expression first (122).
   Note the mirror order too: original computes x's sum first and negates y first; ours the reverse.
4. **Counters in `a0`, ours `v0`** (case 1 rows 171-178, case 2 rows 226-231, case 3 rows 281-291;
   12 rows). Tried: `++g`, `g = g + 1`, a local copy `c = g; c++; g = c;`, an empty `if (g) {}` after
   the increment, `s32` vs `void` return view of `func_8015528C`. All 102, none move the colour.
   `viewsweep` (12 callee/return views) reports no gain (70 vs 70).
5. Case 3 call block order (rows 256-268): original stores the two spilled locals *after* the five
   stack-argument constants and reloads `D_8015EC68` last; ours stores them first and reloads it
   first. Probably follows from 3 (whether x/y are spilled at definition).

Everything else (all branch targets, block layout, case order, the `default:`-first inner switch,
both 9-argument calls) is byte-identical in structure: the diff is aligned row by row, no shape rows.

## Tools that did not run here

`tools/autotweak.py RPG_BAT:func_80154DD8` SKIPs at setup (`cc.py` stage 55 in its scratch compile of
this file, same for order_search's setup path). Worth checking on a fresh tree before the next run:
the guard in this file sits between two `INCLUDE_ASM`s, and the tool's setup compile is the only one
that fails.
