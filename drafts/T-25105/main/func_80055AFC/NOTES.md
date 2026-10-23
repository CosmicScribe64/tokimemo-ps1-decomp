# main:func_80055AFC (T-25105)

The function is the load half of the game save: it copies bank `D_800E6280.unk_1117 & 1` of the card image
back into GameState. It mirrors `func_8005557C` (the save, matched in T-25105), field by field and in the same order.

## Start state

- `src/main/80053650.c` had a plain INCLUDE_ASM. The only earlier body was the m2c census draft
  (drafts/T-14500/main/func_80055AFC/body.c, score 555). No hand draft existed.

## Bodies kept here

- `best.c`: 113 rows (`funcdiff --resolve --version c`, same size as the original, 349 words). The slot
  byte is read through `extern volatile u8 D_800E7397;`, a FAKE (see below). It needs a `keep` line in
  config/migrate_globals.txt before it can be committed as a guard.
- `best-plain.c`: 114 rows, no volatile and no separate symbol.
- The function stays INCLUDE_ASM: both bodies are far above the 30-row limit for a guard.

## What the source looks like (measured)

- Two spellings of the bank base, as in `func_8005557C`. The straight-line code uses
  `((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->f` (the original's `t5`). The loops use
  `((SaveGameView *)&D_80123120[0x200])[arg0].f[i]` (the original's `ra`). One spelling gives one base register.
- The stores go through `GameState *g = &D_800E6280`. With direct `D_800E6280.f` stores, uopt reuses the
  two test loads (`bank+0x205`, `+0x206`) for the stores to `unk_03E` and `unk_03F`, and the original reloads them.
- One counter, `u32 i`, for all ten loops. The last loop of a counter that nothing reads later is unrolled
  and gets a `!=` exit. With a second counter that happens to a 16-byte loop (`j` only in loop 1: the
  2-record loop gets `bne`; `k` only in the 256-record loop: the `unk_74C` loop unrolls). The original
  unrolls only the 256-record loop.
- Slot: `arg0 = <unk_1117> & 1` (the original homes `a0` at entry, and so does every draft that writes arg0).

## The head: a copy-propagation gap

The original head is `lui a0; lbu a0,%lo(D_800E7397)(a0); andi t6,a0,1; <E1 from t6> -> t5; move a0,t6`.
ugen emits that sequence for `arg0 = X & 1` when uopt kept arg0 as a copy and computed E1
(`D_80123120 + arg0 * 0xF00`) from arg0. The `sll` reads `t6` only because as1 forwards the copy.
In the original, E1 is then one live range from the head to the last store (`t5`, spilled at 0x30 around the three calls).

IDO 5.3 propagates arg0's definition into E1. The head copy of E1 is then an expression of the global, a
separate high-priority range (`a0`, then a copy to `t4`). After the first loop, whose stores may alias
GameState, E1 is recomputed or held in a second range. Every plain spelling tried measures 114 to 291 rows:

- slot order and form: bank first or arg0 first, assignment inside the index, `% 2`, `1 &`; 114 to 115 rows;
- a `bank` pointer local (`u8 *bank = &D_80123120[slot * 0xF00]`) against spelling A: the local is what
  gives 114 (spelling A alone: 226);
- slot types: `u32` 114; `u8`, `s8`, `u16`, `s16` 209 to 216 (`andi 0xff` copies and narrowed E1);
- bit-field views of the slot bit (`u8 b0:1` and `u32` bit 24): 104/111 rows, different size (`multu`);
- empty tests (`if (arg0) {}` 187; `if (bank) {}` 114; `if (D_800E6280.unk_1117) {}` 227);
- slot assigned just before the first loop (E2 computed there from a reload): 204;
- the whole slot expression written inline at every use: 291.

This is the gap of the T-11570 selector-copy family (wiki/notes/matching-04.md): the original does not
propagate a local copy of a global, and IDO does.

A volatile read of the slot byte blocks the propagation. arg0 then goes to `a0` and E1 becomes one range
(19 uses, priority 2.2), with the original's instruction sequence. Two forms were measured:

- `*(volatile u8 *)&D_800E6280.unk_1117` (and three other cast spellings) materialize the address with an
  extra `addiu`: 208 to 216 rows;
- `extern volatile u8 D_800E7397;` (a splat label in asm/data/bss.bss.s, so it links) keeps `%lo` folding: 113 rows.
  Its cost: as1 never moves a volatile `lui`, so the `bne` delay slot after `func_80055A38` takes the
  `D_80123120` lui instead of the slot lui (2 to 3 rows).

## Rows left in best.c (113)

1. Colours: the original never uses `t3` or `t4`. The draft has `i` (a defs-only range of the ten `i = 0`
   stores, priority 2.5) in `t3`, E1 (2.2) in `t4` and E2 (1.22) in `t5`. The original has E1 in `t5` and
   E2 in `ra` (colour 13). One more range must take colour 11 before E1 and interfere with E1 and E2, or
   colour 11 must cost E1 more. This accounts for about 50 rows (every E1 and E2 use, plus the spill registers).
2. Frame: the original is 0x48 and the draft 0x40. The spill offsets already match (E2 0x2C, E1 0x30), so the
   original has two more words of locals above them. The two missing words and the missing colour-11 range
   probably come from the same source construct.
3. The unrolled 256-record loop (about 40 rows). The original colours the end constant `0x800E71F6` first (`v0`),
   then dest4 (`v1`, the compare IV), the four sources (`a0`..`a3`) and dests 1 to 3 (`t0`..`t2`). The draft
   colours dest4 (20.5) first. Its source and dest IVs (15.5 each) alternate by bit order: dest1 < src1 < dest2 ...
   The end constant (10) comes last in `t2`.
4. The `bne` delay slot (volatile `lui`, above).

## Negatives added (do not repeat)

- A second counter in any distribution (ten patterns): it changes LFTR and unrolling.
- Dead stores (`j = 0`): no live range. The frame changes only in some combinations (0x48 without `g`, 0x40 with `g`).
- A declared `SaveGameView *b` for the loop base: it is propagated away and leaves no defs-only range. It
  also keeps `+0x200` in the base register (`addiu t8,a0,512`).
- Declaration order of `g`, `i`, `j`, `slot`: no change.
- Direct `D_800E6280.f` stores instead of `g->`: the test loads get CSE'd (see above).
- autotweak 300 s (`--score-jobs 2`) on best.c: 1258 edits tried, no improvement.
- A second IV in the 256-record loop (`for (i = 0, j = 0; ...; i++, j++)`, j on either side): 193 to 211 rows, other size.
- A dead `arg0 = 0;` before the return: no change.

## Next hypothesis

Find the colour-11 range and the two frame words together. The best candidate is a second local whose
uses disappear under strength reduction while its definitions survive, as `i`'s do. It must not be a loop
counter whose last loop is followed by no further use: that loop would unroll. A local read after the
256-record loop (in the final stores or the `func_8006C988` call) could keep its earlier loops rolled.
The next step is to trace that with regdiff -v.

The unrolled loop needs the end constant at the top priority. Look for a form where the bound has more
uses or more blocks, for example a `do { } while (p != end)` with an explicit end pointer, and check that
the other loops stay rolled.

Drop the volatile symbol only if a plain C form keeps arg0 as a copy. Otherwise the symbol needs a
`keep ... proof=` line (wiki/keep-lines.md) once the function matches.
