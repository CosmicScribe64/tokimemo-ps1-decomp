# T-25103: main:func_8006C988 (src/main/800674B0.c): matched

Result: **matched**, 69 words / 276 bytes. The default build compiles the C body (no guard,
no `INCLUDE_ASM`); `funcdiff --resolve --version default` prints MATCH and the full build is
27/27 sha1 OK.

## Start state

The T-24928 guard, 5 rows (`drafts/T-24928/NOTES.md`): rows 5/6 (the record-base init
`addiu t1` scheduled after `move a3,zero` instead of before it) and rows 26/30/32 (the
`cur = val` copy emitted per arm as `andi v1,t0,0xff` where the original advances the base per
arm and keeps one `move v1,t0` in the latch delay slot).

## Reading the target

The per-arm `addiu t1,t1,8` of the original is one instruction of the join block: the then-arm's
`b` steals the join's first instruction (`addiu t1,8`) into its delay slot and branches to join+4;
the else arm falls into it. So the original join block is {base advance; `cur = val` as a plain
move; loop test}. The draft's join had the advance only, with the copy pushed into the arms.

## Steps (funcdiff --resolve --version c, whole unit)

1. `n++` in each arm: base advance moves into the arms, the copy stays at the join (as `andi`),
   but `n++` is no longer hoisted to the top. A separate arm-incremented index `j` (eliminated
   after strength reduction) with `n++` at the top gave the right loop shape but swapped
   `i`/`val` registers. Diagnostic only.
2. `s32 cur`: the join copy becomes `move` (no narrowing; `*last = cur` stays `sb`). Alone, on
   the T-24928 body, the copies stay per arm (`move v1,t0` in both arms), 5 rows.
3. **`n++;` before `cur = val;`** at the end of the loop body, with `s32 cur`: the copy stays at
   the join, the base advance shares the join block, every loop row matches. 5 -> 2 rows (only
   rows 5/6). With `u8 cur` the join copy is `andi`, 3 rows.
4. **First loop as `for (i = 0; i < 0x100; i++)`** instead of `i = 0; do { i++; ... } while`:
   uopt still hoists `i++` and the `i < 0x100` compare to the top, and the induction init now
   comes before `i = 0` in the preheader. MATCH.
5. A separate `u8 type` for `unk_02` (instead of reusing `cur`) also matches; kept for
   readability.

## Negatives added

- `i = 0; n = 0;` order: swaps the a2/a3 colours of n and i (23 rows).
- All four inits on one line: reorders the init moves (5 rows).
- `g = &D_800E6280;` as a statement between `n = 0;` and `i = 0;`, or before `sel = 0xFF;`:
  no change to rows 5/6.
- Removing the dead `first`/`last` tests still fails with the final body (`val` takes `$a0`,
  `first` is spilled), so they stay as a FAKE.
- Positive test form (`type == 0 || ...` with the `sel` arm first): differs.
- Empty-then PRE form (`if (...) {} else { sel = load; } cur = load;`) with the dead tests:
  loads PRE'd but base/value registers swapped and copies per arm (11 rows).

## Lever summary

The placement of the strength-reduced base advance follows the statement that increments its
induction variable, and uopt pushes a join copy into the predecessor arms when it is the first
statement of the join; putting the increment first keeps the copy at the join.
