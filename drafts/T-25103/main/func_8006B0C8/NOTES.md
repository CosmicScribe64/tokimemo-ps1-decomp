# T-25103: main func_8006B0C8 (1272 B, 318 words)

Result: not matched. Best body `body.c` here: 130 rows (the structural lead `alt-split-counter.c`: 254) with
`tools/docker.sh python3 tools/funcdiff.py func_8006B0C8 --unit main --resolve --version c`
(guarded in the file, same length as the original: 318 words, words 0-70 equal).
`src/main/800674B0.c` keeps plain `INCLUDE_ASM` (over the 30-row guard limit).

## Start state

`INCLUDE_ASM`, no saved C body anywhere (T-11690 and T-24600 kept theirs in scratch only). Their notes: a
natural draft compiles to the same blocks but 476-556 rows; inner sort loop unrolled by 2 (original by 4),
the `GameState` base hoisted into a register, frame too small. A fresh natural draft measured 479 rows.

## What the function does

Compacts the eight `GameState.unk_54C` entries whose `ScheduleEntryFlags.active` is set, `bzero`s the rest,
turns January into month 13 when the current month (`unk_03F`) is 12, selection-sorts the entries by
`day + month * 100` (keys stored in `keys[j]`, never read back), swaps through a `GsWord` temporary, then
turns month 13 back into 1.

## Changes that held (479 to 130 rows)

| change | effect | why |
|---|---|---|
| compaction loop counter is `j`, the clear loop is `for (j = n; ...)` | first loop rolled with `sltu` on the walk pointer | counter assigned again right after the loop (T-7020 rule 3); with `i` (re-assigned only in the sort) IDO unrolls it by 2 |
| `if ((keys[j] = e) < min)` or a separate store, not `keys[j] = e; if (keys[j] < min)` in one block | inner loop unrolled by 4 | the reread spelling costs more uopt size; by 2 at 279 words |
| key as `day + month * 100` | `addu t9,t8(day),t7(prod)` | operand order of the original |
| declarations `i, j, n, k, tmp, keys[8], min`, then `s32 pad[10]` (FAKE) | frame 0xB0, `n` home 0xA4, `tmp` 0x9C, `keys` 0x7C, a0 spill 0x40 | locals are laid out top-down in declaration order; the original has ten more words below `keys`, unknown use |
| `for (n = 0, j = 0; ...)` | `move t3,zero` after the walk/end `addiu`s | line grouping of as1 |
| `do { if (keys[j] < min) {...} } while (0);` after the store (FAKE) | key is a ugen ring temporary (`t9,t6,t7,t8` per copy) | without a block boundary uopt forwards the store into a CSE register (`$v0`); across the boundary only ugen forwards, as in the original |
| `goto swap; swap:` before the swap (FAKE) | `&D_800E6280` never held in a register | see below; removes the whole one-register ring shift (329 to 228 word rows) |

## The base-constant range (main finding)

The original rebuilds `&D_800E6280` with `lui/addiu` at every use, even inside the call-free compaction loop
(`addiu t6,t9,0x54C; lui t7; addiu t7; addu t8,t6,t7; sw $at,0(t8)`). IDO puts it in a register (`rlda` in
uopt's output, `ra`/`t1`) from the function entry to the swap. A scan of all built objects found no matched C
function with a full address built inside an innermost call-free loop; only unmatched ones (this function,
`func_80065964`, NAME_ENT `func_80143580`). Probes (semantic changes, scratch only): removing the compaction
store, or making the clear loop's `bzero` argument loop-invariant, each drops the range, so it lives on the
sum of the in-loop store and the clear-loop start address. The empty block before the swap also drops it.
Labels at the top of the outer body, before the inner loop, before the clear loop or before the month test do
not. A `do { swap } while (0)` drops it too but puts the outer `i` in `$v0` (10 more rows).

## First divergence left (word 71)

Main-loop preheader of the unrolled month loops (words 71-74 and 265-268): the original computes the end
`base + n*4` before the start `base + j*4` (`sll t6,t3,2; sll t8,a2,2`), IDO the start first.

Then the inner sort loop (words 144-171, 213-223): the original creates the `keys` walk (`$v1`) before the entry
walk (`$a1`): keys init first in both preheaders, `addiu v1,v1,4` hoisted into the body with `sw t9,-4(v1)`,
entry increment in the `bne` delay slot. IDO does the reverse. That swaps two ring temporaries per preheader,
and the ring offset runs through the swap (228-235) and the undo loop (245-308). Word 136: the outer `t5`
increment lands three slots early in the loop head.

## Negatives (measured, do not repeat)

- Store spellings for the compaction copy (`[n] = ...; n++`, `((GsWord *)&D_800E6280)[n + 0x153]`,
  `(u8 *)` sums): uopt canonicalises them, no change. Absolute `((GameState *)0x800E6280)`: `lui/ori` code.
- Compaction loop as `do/while`, `goto` loop, `for (;;)` with break, `continue` form: same code or worse.
- Clear loop as `while`, `if (n < 8) do {} while`, `sizeof(GsWord)`: same.
- Inner loop as `while`; empty `if (k != i) {}` before the swap; keys walk through a user pointer `s32 *p`;
  `keys[j] = 0; keys[j] += e;` and `keys[j] = day; keys[j] += ...` (uopt folds both); `n > j` tests;
  declaration order variants (`i,k,n,j`, `k,i,n,j`); block-scope `extern` of the base (same symbol id).
- Month loops with `i` instead of `j`: 320/332 word rows (worse).
- Permuter, 2 runs (15 and 10 min, -j 2): first found the swap label (score 3382 to 872); second only noise.

## Exact next hypothesis

The keys-first SR order appears when no loop before the sort uses the inner counter (uopt seems to order SR
temporaries by the age of the address expression `&unk_54C[j]`). `alt-split-counter.c` (254 rows) does that:
a separate counter `m` for the compaction, clear and month loops, and the compaction test written as
`if (!active) continue;`. With the plain `if` the split counter brings the base `rlda` back (363 word rows);
with `continue` it stays unallocated and no swap label is needed (removing the label changes nothing there).
The inner loop then has the original's preheader order, `addiu v1,v1,4; sw t9,-4(v1)` and delay-slot entry
increment, and the same ugen temporaries. Every word has the original's opcode except two schedule spots
(word 4, word 136).

What is left in that shape is uopt colouring: the first-loop walk/end are `v0/v1` (original `a1/v0`), `m` is
`a1` (original `a2`), min and the entry walk are `a1/a0` (original `a0/a1`), the remainder counts `a3`
(original `v0`); and from the swap (word 228) the ring is one off, as are the month preheaders (71-74, 265-268).
Next: run `regdiff` on `alt-split-counter.c` with a traced uopt (none was available here; recipe in
`wiki/uopt-register-allocation.md`) and read why `a0`/`v1` are free for the first-loop walk in IDO but not in
the original. Declaration order of `m`, `i`, `j`, `k` does not change it (measured). The do-while(0) around the
min update is still needed there (without it: unrolled by 2 and the base range back).

Helper: `sbs.py` (run in Docker: `python3 drafts/T-25103/main/func_8006B0C8/sbs.py func_8006B0C8 main`) prints a
register-normalised side-by-side of the guarded C against the original.
