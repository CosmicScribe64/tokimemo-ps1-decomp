# T-25128: ETC func_8013FFAC (scene initializer, 529 words)

Not matched. The best body is `body.c` next to this file: 197 resolved diff rows with the
`s32` view, 206 rows with the shared `void` declaration. The previous best (T-24518
`indexed-both-tables`, the body in the T-24518 replay) measured 346 rows at the start of
this pass. `src/ovl/ETC/8013FF80.c` stays plain `INCLUDE_ASM` (too many rows for a guard).

Measure: put `body.c` under `#ifdef NON_MATCHING`, set `s32 func_8013FFAC(void);` in
`include/ovl/ETC.h`, run
`tools/docker.sh python3 tools/funcdiff.py func_8013FFAC --unit ETC --resolve --version c`.

## What was measured and why it changed

Rows are funcdiff `--resolve` +/- lines. "Score" is `funcdiff.diff_score`.

1. Second table loop (the largest single cause). The original keeps the peeled stores'
   constants 0x80180600/0x80184600 in ugen temporaries (`t9`, `t1`) that the unrolled body
   reuses. The natural `for (i = 0; i < 6; i++)` loop makes uopt CSE them into registers
   (`rldc` a1/t0 in uopt's output), which removes two ugen slots and shifts every later
   `t` register. The matched sibling `func_8013CCE4` (T-24706) writes
   `buf[0] = K0; buf[1] = K1; for (i = 2; i < 6; i++) buf[i] = K0 + i * 0x4000;` with a
   `u32 i`: the unsigned body sums (ucode type J) do not CSE with the signed peel literals,
   and ugen reuses the literal registers. That recipe only works when cvt_pass.py turns
   uopt's global copy propagation off (`OPTN 405 0`). `func_8013CCE4` gets it from its
   compare-chain switch (cfe writes the selector into a VREG). A 15-case jump-table switch has
   no VREG, so `func_8013FFAC` runs with zcopy on and the 2..6 loop folds to constants
   (isolated tests: chain switch keeps `li a0,2`, jump-table switch folds). A dead two-case
   switch on `unk_F5F` before the real switch (FAKE) creates the VREG, turns zcopy off, and
   the original peel/body shape comes out exactly.
2. With a `u32` first-loop counter the strength-reduced compare becomes `sltu`. Separate
   counters (`s32 i` for the first loop, `u32 j` for the second) keep `slt`, but the first
   loop then unrolls; a dead `i = 0;` after it (FAKE, the playbook's "counter assigned
   again" rule) keeps it rolled.
3. Frame 0x70 and table at sp+0x54: every declared scalar costs frame space. Dropped the
   T-24518 FAKE `var_v1`, the `unused` local and `temp_t3` (case 9 is now
   `if (++unk_03A >= 0x80U)`), and declared `j` after `sp54`.
4. Case 5 tail: `((s32 *)D_8014DB90)[unk_F5F * 2]`, the spelling of the matched sibling
   `func_8013CF2C` (FAKE flat view of the `[13][2]` table), spends the ugen slot the
   original spends. All later ugen temporaries then line up (score 283 to 137 at that step).
5. Case 8: `*(s8 *)&D_8012210x = unk_03A` store views (FAKE) move `unk_03A` to `$v1` and the
   0x80 shade constant to `$v0`, as in the original.
6. `s32` return (no return statement, like `func_8013CF2C`): fixes the case 13 delay slot
   (original `nop`, draft hoisted `lui v0`), 206 to 197 rows, but adds `move v0,t0` at the
   entry and `move v0,v1` in case 8.

## First divergence left

- Entry: `lui t0; lbu t0,unk_F5F` from the dead switch. `unk_F5F`'s promoted range now starts
  at the entry, which is also what gives it `$t0` (the original's register). The same dead
  switch on an unused byte (`unk_110E`, `unk_110F`) has no load but puts `unk_F5F` back in
  `$v1` (score 437).
- Selector in `$a0`, original `$a1` (about 50 rows). In the original the 0x01000000 constant
  (cases 5 and 11) holds `$a0` and interferes with the selector in case 11; the draft gives
  the constant `$v0`.
- Case 5: D_8014DB90 element address `$v0` (original `$v1`).
- Case 8: the original's `andi v0,v1,0xff` (a CSE of the widened `unk_03A` used by the
  three shade stores) is missing. Case 9: the original loads the old value into `$v0` and
  puts the new value in `$v1` (`addiu v1,v0,1`); the draft uses one register.
- Second loop: shift `$v1`/pointer `$v0`, original reverse (a priority tie). A temporary
  `k = j * 0x4000; sp54[j] = 0x80180600 + k;` fixes it but adds 8 frame bytes (no way found
  to drop another stack slot; `register` does not help).

## Negatives added (do not repeat)

- Case 8 store spellings without effect on the missing `andi`: chains in all orders,
  post-increment chains, `(s32)`/`(u8)`/`| 0`/`& 0xFF` casts, `s32`/`u32`/`u8` local copies
  (copy-propagated), forwarding re-reads (`D_80122105 = D_80122106`), s8 views (move
  registers, no `andi`), `(s8)` value casts, the s32 return (gives `move v0,v1`).
- Entry read `i = D_800E6280.unk_03A;` before the dead switch makes `unk_03A` an entry
  location, so cvt_pass.py drops the CVT of the case 7/8 compares (checked in the ucode),
  and `unk_03A + 0` keeps a CVT on the shade stores; uopt still folds
  `cvtl 8(add(cvt(x), 0))` back to `x`: byte-identical to the best body (206 rows). With
  the compare CVT kept (no entry read), uopt does build the CVT range in `$v0` with `$v1` for
  the variable, as in the original, but computes it before the compare, where as1 folds it.
- Byte views of `unk_03A` in any of cases 5/7/8/9 are the same uopt variable as the field
  (32-combination grid, no change).
- Second loop on zcopy-on: `<< 14`, operand order, do/while, while, `!=`, `6U`, explicit
  peel with `s32`/`u32` index (folds), unsigned constant or cast on the sum with `s32` index
  (different unroll).
- Dead switch on the selector itself (plain field or view): worse (412/387).
- `s32` return before the fade fix: extra moves in cases 8/9 (prior T-24518 note confirmed).

## Next hypothesis

The original's case 8/9 widened-`unk_03A` range (in `$v0`, used by the three shade stores
and the `+1`) is what pushes `unk_03A` to `$v1`, 0x01000000 to `$a0` and the selector to
`$a1`. Look for a spelling whose ucode keeps `LOD unk_03A; CVT` on the shade stores of
cases 8 and 9 but not case 7 (cvt_pass.py's widen rule drops the CVT when the load goes
straight into a store), then retest the dead-switch target: once `$v0`/`$v1` are taken in
case 8, `unk_F5F` may get `$t0` without the entry load, as in the original. Scratch tools
used: a register-masked side-by-side aligner and a uopt input/output ucode dump shim (an
`extra_shims` wrapper around cvt_pass.run_uopt); both are easy to rebuild from
`tools/funcdiff.py` and `tools/cvt_pass.py`.
