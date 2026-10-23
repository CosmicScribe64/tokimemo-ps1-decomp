# T-25314: ETC func_8013FFAC (second pass, 529 words)

Not matched. The best body is the `NON_MATCHING` guard in `src/ovl/ETC/8013FF80.c`:
17 resolved funcdiff rows (`tools/docker.sh python3 tools/funcdiff.py func_8013FFAC --unit ETC
--resolve --version c`). `include/ovl/ETC.h` declares `s32 func_8013FFAC(void);`.

## Start state

The T-25128 body (`drafts/T-25128/ETC/func_8013FFAC/body.c`): 197 rows with the s32 view.
Its first divergence: the dead `switch (unk_F5F)` entry load (`lui t0; lbu t0`, plus
`move v0,t0`), then the selector in `$a0` (original `$a1`, about 50 rows), then the case 5 and
11 constant, the case 8 `andi`, the case 9 old/new split and the second-loop shift/pointer tie.

## What changed and why (each measured alone, in this order)

Rows are my register-masked aligner's structural + register rows unless noted; official
funcdiff rows at the end. A side-by-side aligner (resolved words, register fields masked), a
uopt input/output ucode dumper (an `extra_shims`-style wrapper around `cvt_pass.run_uopt`) and
an as1-input dumper (a wrapper around `frame_pass.run_as1` that renders the binasm records)
were the scratch tools; all three are small scripts over `tools/funcdiff.py`, `tools/cvt_pass.py`
and `tools/frame_pass.py`.

1. Case 8 shade value in a `u32` local copy: `j = unk_03A; if (j < 0x81) { stores of j;
   unk_03A = j + 1; }`. With zcopy off the cross-block copy stays a range, and the selector
   moves to `$a1` as in the original (reg rows 99 to 50; official 197 to 108).
2. Case 9 `j = D_800E6280.unk_03A & 0xFF;` before the stores: ugen emits `lbu v1; and v0,v1`,
   as1 folds it into `lui v1; lbu v0,(v1)` and the increment is `addiu v1,v0,1`, the original.
   A plain copy is coalesced inside one block. Case 9 matches (98 rows).
3. `s32` return kept (the T-25128 reason): `$v0` live at the exit stops as1 hoisting `lui v0`
   into case 13's load delay slot, and keeps case 8's `andi v0` (in void mode as1 renames it away).
4. Second table loop with a `u32 k` temporary: `{ k = j * 0x4000; sp54[j] = 0x80180600 + k; }`
   gives the shift `$v0` and the pointer `$v1` (8 rows). It costs one 4-byte local: the frame is
   0x78 instead of 0x70 (see Left). `s32 i` as the temporary breaks the loop (s0, 50 rows).
   Declaring `s32 i; u32 j; u32 k; s32 sp54[6];` (scalars first) puts sp54 back at 0x54, so only
   the two `addiu sp` rows remain.
5. `func_800AE0A0` called undeclared (implicit int), as `8013BEF0.c` does (T-24706,
   `MAIN_API_OVERRIDE_func_800AE0A0`, inside `#ifdef NON_MATCHING`): the playbook's "int view
   takes $v0 away after the call" rule. The case 5 table address moves to `$v1` (3 rows). The
   same view of `func_8004A1A0` is worse (124 aligned rows).
6. Case 8 `k = j & 0xFF;` in the arm (k or i, both u32/s32 work): the original `andi v0,v1,0xff`
   comes out and the three stores use `$v0`. A mask on the global or on a copy made before the
   branch is removed by as1 (it knows the copy comes from `lbu`).
7. Entry: `i = D_800E6280.unk_F5F;` (dead) plus a dead `switch (D_800E6280.unk_110D) { case 0:
   case 9: break; }` replaces the dead switch on unk_F5F. The dead read makes unk_F5F an entry
   location; its promoted range now starts at the entry without a load, gets `$t0` and is
   reloaded at the case 8 join (`lui t0` hoisted, `lbu t0` at the label), exactly the original.
   The switch on the selector shares the selector's load. 72 to 23 aligned rows.

Official funcdiff rows: 197 (start), 108 (1), 98 (2), 17 (all).

## Left (17 rows)

- `move v0,a1` at the entry (and `sw ra` one slot later): the dead switch's VREG is coloured
  `$v0` and the s32 view keeps `$v0` live at the exit, so as1 cannot delete the dead copy. In
  void mode it goes, but case 8's `andi` and case 13's `nop` are lost (82 aligned rows).
- Frame 0x78 instead of 0x70: locals are i, j, k (12) + sp54 (24) + the dead switch VREG (4) +
  the element chain's hidden 1-byte VREG = 41 bytes; 40 fit. Without k the frame is right and
  the shift/pointer tie comes back (ib3, 10 rows). Plain or mixed shade chains drop the 1-byte
  VREG (frame right) but put 0x80 in `$a3` and reorder the three stores (dca: 21 rows).
- 0x01000000 in `$v0`, original `$a0`, in case 5 (after the first func_8004A1A0) and case 11
  (5 rows). The two uses form one range (a single-use typed copy in case 5 becomes a ugen temp,
  cx1). Statement order and chains in cases 5 and 11, unsigned views and constant types do not
  move it.

## Negatives added (do not repeat)

- Dead zcopy switch on an unused byte (unk_110E/110F) or on the selector without the unk_F5F
  entry read: unk_F5F leaves `$t0` (a2/v1), 170+ rows. The entry read alone (z1) gives `$v1`
  until steps 4-5 put `$v0`/`$v1` elsewhere.
- An outlier `case 0xFF:` in the real switch (its VREG instead of a dead switch): the extra
  compare stays (`li at,255; beq`), 32 aligned rows.
- `D_800E71DF` / `D_800E7384` symbols instead of the GameState fields: byte-identical.
- Case 8: chains in any order, `(u8)`/`& 0xFF`/`(s8)` on the stores, s8 store views, u8 locals
  (frame +8), re-read forwarding, view self-stores: no `andi` or worse.
- Second loop without k: operand order, `<< 14`, `*(sp54 + j)`, `(u32 *)` view, do/while,
  `0x...U` constants: tie unchanged. u8/u16 j, s8/s16 i: loop shape breaks. s32 j for both loops: breaks.
- `D_8014DB90[F5F][0]` / pointer locals in case 5: worse than the flat view.
- Narrow counters or casts on the first loop (`(s32)i < 6`): LFTR stops.
- Dead switch with only `default:` or a single case: no VREG, zcopy stays on (340 rows). A selector
  assigned through a local (`switch (j = G)`) also loses the trigger. A dead `k = unk_03A;` entry read
  or other case values change nothing.
- Int views of func_8004A1A0 (header-free cast `((s32 (*)())f)(...)` gives `jalr`; a renamed int
  prototype moves the constant to `$v1` and unk_03A to `$a3`).
- Without k (frame right): `<= 5`, while, peel after or swapped: tie unchanged or worse.
- autotweak setup fails on this file (cc.py stage error). A 600 s permuter run (-j 2) found only
  `i = 0xA0; ... -i` for the second func_8004A1A0 argument, which is worse by the aligned count.

## Next hypothesis

The three left items share one cause: the zcopy trigger costs a VREG (4 frame bytes, the dead
`move v0,a1`). In the original, zcopy-off probably came with the real jump-table switch itself
(no dead copy, no extra slot). Under the current cvt_pass rule only a VREG stored from a direct
unsigned-global load before the first branch counts. Look for a selector form whose VREG is
read by the dispatch (so the copy is live, not dead) and gets `$a1` with the global's load
retargeted by as1, or a VREG that uopt colours past `$v0` because a `$v0` range is live at the
entry. If the frame then fits with k, the remaining 0x01000000 `$a0` may follow from the new
colouring order (the original colours shift/pointer before the constant).
