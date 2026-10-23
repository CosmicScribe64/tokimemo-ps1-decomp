# T-25309: TAIIKU func_80137784, second pass (1216 bytes, 304 words)

Result: no match committed. The guard in `src/ovl/TAIIKU/80133C80.c` stays at 4 rows (copy:
`guard-4rows.c`); its NON_MATCHING comment now names T-25309. A match exists and is tested
(`alias-match.patch`, below), but it adds two second symbol names in a config file, which T-25201
left for the user's approval. This pass does not make that decision.

Measured with `tools/funcdiff.py func_80137784 --unit TAIIKU --resolve --version c` (whole unit) and
`tools/regdiff.py -v` with the traced uopt (`$TOKIMEMO_SCRATCH/r9-uopt/variants/trace`).

## Start state

T-25106 / T-25201: guard at 4 rows, insns 119 and 263, original `bne a0,t8` / `bne a1,t9` (pointer
first), the guard gives `bne t8,a0` / `bne t9,a1`. Positive control `alias-8rows.c` with undefined
zero-offset aliases; next step was second names in `config/symbols_TAIIKU.txt`.

## First divergence (unchanged)

Same as the first pass: insn 119 and insn 263, the compare operand order. Everything else matches.

## New evidence

1. **No ordering lever can fix the natural compare.** `natural-226rows.c` (`D_8014984C == &D_8014984C[arg0]`,
   `D_8014981C == &D_8014981C[arg0]`) has the right operand order. 226 rows today, not the 249 in the
   T-25106 notes. regdiff -v: 984C base bit 238 at 0.5, 981C base bit 239 at 0.5, 0x44 bit 230 at 0.4,
   D_80122E94 bit 13 at 0.333. Probe: an extra use of D_80122E94 (code changed, measurement only) puts
   D_80122E94 first (0.667, a2). The 981C base then still takes a3 at cost 0. So in the original that
   base had priority <= 0, not just a later turn. The only way to get that is a block-3 compare constant
   that is not the same uopt candidate as the `++` address base. Block counts and priority ties cannot
   do it. The same holds for the 984C compare.
2. **The flow graph of this function** (trace, 32 blocks): the for loop leaves an empty block and the
   `k_reset` edge has a uopt-inserted block (bb31; probably where the reloads after the call go). A do/while loop removes one
   block (31 blocks), but 0x44 stays at 0.4 (N 15 -> 14, D stays 5). The spellings of the entry condition
   (`!(arg0 == 0 && ...)`, `arg0 ||`) leave the blocks and priorities as they are. Unreferenced labels
   do not split uopt blocks.
3. **The positive control resolves to 0 rows.** With alias names that end in the address
   (`slot_x_8014984C`, `slot_cnt_8014981C`), funcdiff resolves them: 0 rows. Only the literal link fails
   on the undefined names.
4. **The config route works end to end.** splat rejects a second name at one address ("Duplicate
   symbol detected") unless both entries carry `allow_duplicated:True`. With the aliases listed first
   (`can_be_referenced:False`) and `D_8014981C`/`D_8014984C` listed after them, splat keeps the D_ names
   for every label and data piece. The aliases go only to `build/ovl/TAIIKU_undefined_syms_auto.txt`.
   With `alias-match.patch` applied (new `config/symbols_TAIIKU.txt`, one path line in
   `config/overlays/TAIIKU.yaml`, two FAKE declarations in `include/ovl/TAIIKU.h`, the C with no guard):
   `funcdiff --version default` gives MATCH (linked bytes, 1216), a clean rebuild gives 27/27 sha1 OK,
   and check_headers, migrate_globals --check and sync_protos --check-branch all pass. The tree was
   reverted afterwards.

## Negatives added

- do/while form of the block-1 loop with natural compares: 31 blocks, priorities unchanged.
- entry condition spellings `!(arg0 == 0 && D_80122E94 != 0xFF)` and `(arg0 || ...)`: no change.
- unused labels between the slot store and the compare: no block split, 226 rows.
- `(Fix32 *)&D_8014984C` / `(s32 *)&D_8014981C` as the left constant: the same candidate, 226 rows.
- a block-scope `extern s32 D_8014981C[];` redeclaration: the same candidate, 240 rows (block 3 only).
- the alias store pointer with the real base on the left (`D_8014984C == &((Fix32 *)&D_8014985C - 4)[arg0]`):
  265 rows. The trace shows why: uopt splits the alias offset off, so D_8014985C becomes the slot base
  (t4, both units), the store is `sw t8,-16(t9)` and the compare is `bne t9,t7` against `base + 16`.
  Any alias with a nonzero offset ends like this.

## Next hypothesis

The C alone cannot do it with IDO 5.3 and today's symbols: only a second zero-offset name for
0x8014981C and 0x8014984C gives the separate compare constant the original has (or the original
compiler, 5.2, treated a compare's address constant differently, which only a toolchain pass could
model). Apply `alias-match.patch` once the user approves second names for this function
(`git apply drafts/T-25309/TAIIKU/func_80137784/alias-match.patch`, then `python3 configure.py` and a
clean rebuild). The names in the patch (`slot_x_8014984C`, `slot_cnt_8014981C`) end in the address so
that funcdiff resolves them. The policy question is the same kind as wiki/keep-lines.md (a second name
for one object, with a FAKE comment at the use).

## Third pass (2026-10-23, fresh eyes)

Result: no match; the guard stays at 4 rows and its code is unchanged. Every lever below was measured
against the natural compares or the guard. Rows are `funcdiff --resolve --version c` diff lines unless
marked "mini". A mini count comes from a reduced unit: the two includes, the three macros and the body,
compiled with `tools/cc.py` and compared word for word with the relocated immediates masked. It takes
about 13 s under load, against minutes for funcdiff. Its counts are 18 higher, because the shared
`lui at` of the block-2 runner stores differs there. The guard gives 22 mini rows.

regdiff -v on the natural body (226 rows) gives the same picture as the second pass: bit 238
(0x8014984C) 0.5 in t5, bit 239 (0x8014981C) 0.5 in a2, bit 230 (0x44) 0.4 in ra, bit 13 (D_80122E94)
0.333 in a3.

Negatives added:
- A dead second parameter as the slot pointer (`s32 func_80137784(s32 arg0, Fix32 *pos)`, header
  `s32 func_80137784();`, so the callers are unchanged). The parameter's home is in the caller's frame,
  so the frame stays 56, and pos gets a0. But IDO stores the incoming a1 at entry (`sw a1,60(sp)`),
  and the alias offset still moves to the pointer side (`addiu t8,a0,16; bne t8,t9`): 230 rows. With
  `pos == D_8014984C` the base takes t5 again: 250 rows. A plain variable first does not stop the
  addend move, because cfe puts the complex operand first and uopt swaps it back.
- Casts on both sides of the natural block-3 compare: `(void *)`, `(u8 *)` and `(u16 *)` give 240 rows
  (the same as the natural block 3 alone, so the candidate is unchanged). `(s32)`, `(u32)` and
  `(s32)(D_8014981C + arg0)` give 388.
- `volatile` on D_8014981C and D_8014984C (in the header) with both compares natural: the base still
  takes t5, and the frame grows to 64: 292 rows.
- A zero index that uopt folds late (`&D_8014981C[arg0 * 0]` on either side): it is still the same lda
  candidate (a2 is held from the `++` into the compare), mini 250.
- A second name made in C: `#pragma weak alias = D_8014981C` is ignored by cfe 5.3 for an extern
  (Warning 780: the primary name must be defined with an initial value in this file). D_8014981C comes
  from INCLUDE_RODATA, so this would need the object defined in C, and the result would still be a
  second name, which is the open policy question.
- No other linker symbol lies at 0x8014981C or 0x8014984C (`build/ovl/TAIIKU.map`,
  `TAIIKU_data_syms.ld`), so no existing name can serve as the zero-offset compare constant.

Why C cannot close it (summary of three passes): the original materialises the slot base twice, once
for the CSE'd address (`addu a1,v0,t8`) and again in the next block for the compare (`bne a1,t9`). The
saving formula (`(L+S)*F` minus one reload per unit that needs one) gives such a base a positive saving
as soon as the compare's lda is the same expression as the address base, and a free register then
takes it. Any distinct lda with a nonzero offset gets its addend moved onto the pointer side, by uopt
or by ugen. So the compare constant must be a different symbol at offset 0. The alias patch is still
the only measured match.
