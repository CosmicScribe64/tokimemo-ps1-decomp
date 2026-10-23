# T-25109: EN_NICHI func_80136DEC

Result: no match. The guard in `src/ovl/EN_NICHI/80136B10.c` holds the best body (copy in `body.c`). It has
348 of 348 words, frame 0xF8, and every register and table offset agrees with the original. Only two
delay-slot fills in the outer dispatch differ: 4 aligned rows. `funcdiff --resolve --version c` counts 258
rows, because it compares by position and each fill shifts one word through rows 76-119 and 78-203.

## Start state

INCLUDE_ASM. Best known body: `drafts/T-24516/en-36dec.c` (= `drafts/T-24518/en-baseline.c`), 347 words
against 348, frame 0xF0, 191 aligned rows (319 funcdiff rows).

## What changed, in order, and why

Each step was aimed at the first divergence. Aligned rows after the step are in brackets.

1. **Case 3 / inner case 4 clears are one loop on the shared counter `i`.** The original has
   `li s1,1; addu v0,s1,base` after the three absolute index-0 stores. That is IDO's peel plus 4x unroll of
   `for (i = 0; i < 5; i++) { C34[i] = 0; C3C[i] = 0; C44[i] = 0; }`, and the remaining `i` sits in `s1`, the
   register of the case 1 counter. Sharing `i` lengthens its live range, which lowers its priority below
   the kinds cursor. The kinds cursor then takes `s0`, `i` takes `s1` and 0x1000 takes `s2`, as in the
   original. The frame grew to 0xF8. [164]
2. **Two scalars are declared above the stack tables.** The original tables sit at C4/98/6C/54, with 8
   bytes of scalars above them. [156]
3. **No `DisplayActor44 *p` local.** Its slot pushed the spill slots 4 bytes down (0x48 instead of 0x4C).
   [132]
4. **The selector is a separate value, not a direct `switch (D_800E6280.unk_110A)`.** With the global
   switch, cfe's switch temporary keeps IDO from folding the peeled first iterations of the 21-iteration
   flag loops (`sll tN,zero,6` and an IV start at record 0 instead of an absolute `D_80120653` store and an
   IV at record 1). It also makes uopt CSE the case 2 alpha load, which the original reloads. A lab
   (`switch` with two or more cases against an if-chain or a one-case switch) confirmed the switch is the
   trigger. A `u8` local or a `GameState *gs` selector fixes both. Case 0's loops became the plain
   `((s32 *)D_801391E4)[i]` form, which frees `offset`. [37 with `u8 state`]
5. **The case 1 init loop is written in ugen evaluation order.** The t-registers of the first stores were
   rotated by one: the original evaluates `unk_01 = kinds[i]` first and `unk_14 = D_801391F0` before
   `unk_05 = 0x80`. as1 reorders the stores afterwards. [12]
6. **The selector goes through `GameState *gs = &D_800E6280`.** It puts the selector in `v1`, as in the
   original. With `u8 state` it was in `v0`, because the copy-propagated global range found `v0` free.
   Every other GameState access through `gs` is neutral, so the body uses `gs` throughout. [6, of which 2
   rows are the jump-table relocation name in my aligner; funcdiff --resolve handles those.]

## The first divergence left

as1's global delay-slot filler takes, for D1 `beq v1,at,case1`, the case-1 head's `move s1,zero`, and for
D2 `beq v1,s0,case2`, the case-2 head's `lui t4,%hi(D_80120657)`. The original fills them from the next
compare: `li s0,2` and `li at,3`.

From the traced as1 (`drafts/T-24900-as1` scripts, the `as1-x2` binary of the T-24927 worktree):

- Numbering is RPO with fall-through-first DFS. Each case head (bb20, bb28) comes right after its compare
  (bb19, bb27), and the next compare comes after the whole case (bb27, bb35). Weights are D1 4, case-1
  head 7, D2 7, case-2 head 10, D3 10, case-3 head 13. The candidate list is sorted by (weight, number), so
  each case head is tried before the next compare.
- The path test uses the successor's live-in set. `move s1,zero` and `lui t4` pass because `s1` and `t4`
  are dead at D2 and D3.
- For the original's fills, both case heads must be skipped while the case-3 head is not skipped: D3's slot
  holds the case-3 head's `lui t4`. Cases 1 and 2 contain loops and case 3 does not. The working theory is
  that in the original the case-1 and case-2 heads sat in a different as1 region (the loop regions) or
  were not dominated by their compare. No source change tried so far moves them.

## Negatives (measured, do not repeat)

- `s32` return type: neutral with the `u8` selector. With `gs` it adds `move v0,v1` (+1 word).
- Narrow loop counters (`u8`/`s16`/`u16`/`s8 j`) for the flag loops: these kill the unroll (328 words).
- A separate `s32 j` for the flag loops: frame 0xF0, worse.
- Byte-array forms `D_8011ECD0[i * 0x44 + 0x1983]` for the flag loops: neutral (the fold depends on the
  switch, not on the address spelling).
- Selector spellings: `(s32)`, `(s16)` and `(int)` casts add 2 words; `& 0xFF` gives 127 rows;
  `(u32)`/`(u16)`/`*(u8 *)&` are neutral; `switch (state = global)` is neutral; `s32`/`s16 state` gives 51
  rows; `u8`/`u16`/`u32 state` gives 37 rows (`v0` selector).
- The selector assignment placed before the table copies: neutral.
- `break` instead of `return` in cases 0-2: neutral. Braces around the case bodies: neutral.
- A `DisplayActor44 *p` local before the tables: tables shift 4 bytes, and the dispatch is unchanged.

## Next hypothesis

Find out where as1 gets its regions (the list at 0x100235d0; `func_428a2c`/`func_428fc4` in
`/private/tmp/t24047-uopt/variants/as1-trace.c`), and whether a loop region can begin at the case head.
Then test the source shapes that would put the case-1 and case-2 heads in their loop region or out of D's
dominance: the case 1 init loop as a `do`/`while` with the init on the case line; case 2's copy loop with
the if/else inside a loop shape; or an outer `for`/`while` wrapper that uopt removes. Check each one with
`XBBSCAN dst=19`/`dst=27` in the as1 trace, before counting rows.

## T-25201 (closer): matched

The body in the guard was right. The 4 rows came from the file's data: `D_80139C5C` (the scene table
that `func_80136B10` calls through) names `func_80136B74`, `func_80136DEC` and `func_8013735C`, and it
was still an `INCLUDE_RODATA` piece. Defined in C (`FnTbl3 D_80139C5C = { { ... } };`), it makes
`func_80136DEC` an address-taken label in the file. as1 then gives the jump-table `jr` of the inner
`switch (gs->unk_110D)` in case 3 an edge back to the function entry ([[as1-scheduler-rule]] rule 8).
That back edge forms a natural loop: entry, table copies, the compare chain D0-D3, the case-3 head
and the `jr` block. The case-1 and case-2 heads cannot reach the `jr`, so they stay in the outer
region. as1 handles the loop region first, and its candidate list for D1 and D2 holds only the loop's
blocks: D1 takes D2's `li s0,2`, D2 takes D3's `li at,3`, and D3 still takes the case-3 head's
`lui t4`. `funcdiff --resolve` gives MATCH in both the NON_MATCHING and the default build; 27/27 sha1.

How it was found: the traced as1 showed the case heads in the outer list in (weight, number) order,
with `move s1,zero` passing the path test. In the original the case-1 and case-2 heads had to be left
out of D1's and D2's lists while the case-3 head stayed in D3's list. The only thing the three heads
differ in is whether they reach the case-3 `jr`, and the playbook's KANGEI recipe (T-25150) gives the
edge.
