# T-25110: TACO func_8013DEE4 (matched)

Result: matched, 1344 bytes (334 instructions). Default build compiles the C in
`src/ovl/TACO/8013DB80.c`; `funcdiff --resolve --version default` prints MATCH.

## Start state

T-24502 body (`drafts/T-24502/TACO/func_8013DEE4/body.c`): 69 rows in the
full TU, 335 against 334 words. The outer loop kept rot, count and the point
offset as declared locals (`rot`, `o`, `q`) and a `pad[2]` FAKE.

## What closed it, in order

1. **Spill homes are uopt's, not declared locals (69 to 21 rows).** A uopt
   dump of the draft (`rstr`/`rlod` records) showed that a split live range
   that is not a declared variable gets a new home below all declared
   locals. The target's rot (0x60) and offset (0x54) sit there, so they are
   strength-reduction temporaries of a plain index loop:
   `for (o = 0; o < 2; o++) { w.vy = v.vy + o * step - step / 2; for (j ...)
   RotTransPers(&w, &pt[j * 2 + o], ...); }`. `&pt[o + j * 2]` gives 93 rows
   (uopt then strength-reduces the full address, base included);
   `j * 2 + o` keeps only `o * 4` as the outer temporary and adds the base in
   the loop head (`addu s2,v1,t5`). Count `o` is declared at the slot of the
   target's 0x90 (decl order poly, o, depth, flag, dist, kind, ...).
2. **if tests read the copy, switches read kind (21 to 3 rows).** The target
   reads kind's home 0x80 in the four if tests and a spill copy (0x64) in the
   two jump-table switches. With `mode = kind;` after the first switch, the
   if tests on `mode` and the switches on `kind` give that split; the
   reverse puts every use on the spill copy. It also lets uopt keep the
   constants 5 and 4 in s3 and s1 (dist moves to s2).
3. **Colour value is a small switch (3 to 0 rows).** `switch (mode) { case 5:
   c = 0xFF; break; case 6: c = 0xFF; break; default: c = 0x80; break; }`
   gives the target layout (`beq 5 -> A; li at,6`, the case-6 block absorbed
   into the second branch's delay slot, A = `b J; li v0,0xFF`). Every if/else
   spelling leaves 1 to 3 rows. cfe makes a temporary for this compare-chain
   switch (sp+0x68), which pushes the spill homes down by one slot; one
   declared scalar has to give that slot back, so `pad[2]` became one
   `s32 pad` above `poly` (FAKE, T-25110), and `step` moved above `v`.

## Negatives (measured on the final body)

- no `mode` (all uses on kind): 35 rows; `pad` removed: 47 rows; `pad`
  replaced by a real `half = step / 2` local: 6 rows (rot's spill home moves
  to 0x5C); `case 5: case 6:` shared label: 1 row; default-first case order:
  0 rows (equivalent).
- `u32 kind`: 126 rows (one ring slot early from the CVT); `u32`/`s16`/`u8`
  mode: no gain.

## Lesson for siblings

When target homes of loop state sit below every slot your declared locals can
reach, check whether they are uopt spill homes (strength-reduced `o * k`
products, copy-propagated values) before adding pads. Dumping uopt's output
ucode (`rstr`/`rlod` offsets) shows which homes are new. The sibling
func_8013EAA4 in the same file uses the same projection grid.
