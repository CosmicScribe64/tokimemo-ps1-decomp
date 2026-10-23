# T-24925 ETC:func_8013CCE4

Result: **not matched**. Guard left at **3 rows** (unchanged from T-24921; the
source file was not modified this run). Owned file only:
`src/ovl/ETC/8013BEF0.c`.

## What this run proved (ucode-level diagnosis)

New tooling (this directory, all scratch, no build change): `ucdump.py`
(compiles the guarded TU through cc.py with a uopt/ugen shim that dumps the
ucode at every cvt_pass stage), `uopt_shim.py` (cfe -> widen -> cpo -> pre ->
uopt -> post dumps), `uopt_flip.py` (same, but flips one procedure's OPTN 405
value before uopt runs), `ugen_shim.py`, `ucpretty.py` (ucode pretty-printer).
Dumps land in `drafts/T-24925/ucdump/` (not committed; reproducible with the
tools).

1. **The pass gives the plain selector OPTN 405 val=0.** Stage 3 (after
   `copy_propagation_options`) has `optn=405 val=0` before func_8013CCE4's
   ENT: the plain `switch (D_800E6280.unk_110D)` is a direct `LOD L
   (Ssym13+0x110d)` stored into the switch VREG temp at the entry, so
   `_assigns_switch_temp_from_unsigned_global` returns True. With zcopy off,
   uopt keeps the temp in `$v0`: its output has compare 0 on the global
   (`lbu v1; beqz v1`) but compares 1-3 on the temp (`beq v0,at`) - exactly
   the 3-row miss. `pre` also strips the widening CVT (unk_110D is accessed
   only as L in the whole TU), so uopt sees `lod L; vreg; str temp; lod temp`
   x4.
2. **The byte view gets OPTN 405 val=1 and fixes all three rows.**
   `switch (*(u8 *)&D_800E6280.unk_110D)` compiles to `lda(sym13, ...); ldc
   0x110d; add; ilod` - an *indirect* load, not a direct LOD, so the rule
   returns False and the procedure gets val=1 (zcopy on, IDO default). uopt
   then propagates the global into the compare chain: `lbu v1; beqz v1;
   or v0,v1,zero (delay slot); li at,k; beq v1,at` x3 - **the target's
   selector chain exactly** (target .s rows ACE4-AD18). The temp's def
   survives val=1 only in the ilod form (its def is an expression, not a
   plain copy), which is what keeps the dead `or v0,v1,zero` as the beqz
   delay filler.
3. **But val=1 also folds the buf[] loop - and the fold tracks the OPTN
   value, not the selector shape.** The decisive experiment: `uopt_flip.py`
   compiled the *plain* selector with only func_8013CCE4's OPTN record
   flipped to val=1. Result: all four compares on `$v1` (rows 7/10/13 fixed)
   AND the loop still folded (frame 0x68 vs 0x70, per-trip constants
   `lui/ori` standalone, `li a0,2` and the two `sll` gone). So:
   - val=0: temp kept (compares 1-3 on `$v0`, the 3-row miss) + induction
     kept (per-trip `lod R3; ldc base_k; add; cvt; istr` in uopt's output =
     the target's `addu t7,v1,t3` shape; the case-2 rows all match today).
   - val=1: compares propagated to `$v1` (target) + the induction folded to
     bare `ldc 0x801EA000..0x801F6000; str buf[k]` (~86 rows, frame 8 bytes
     small, `i` loses its stack slot).
   The per-procedure binary OPTN record cannot express "propagate the
   compares, keep the loop".
4. **Counter spellings do not rescue val=1.** Measured this run: `u16 i`
   under val=1 stops the *unroll* entirely (a real loop with `andi/sltiu`
   per trip - wrong shape). T-24921 had already measured `u32 i` (folds),
   `s32 i` (folds even under val=0), `i != 6`, do/while, one-line body/head,
   address-taken `i` - all fold under val=1. A hand-written compare chain
   (the target is a compare chain, not a jump table) would also be val=1
   (no switch temp) so it folds too, and it loses the `or v0,v1,zero` row.

## Conclusion

The three rows are a **build-model gap**, not a source spelling problem:
`tools/cvt_pass.py`'s per-procedure `OPTN 405` record couples the compare
propagation (val=1) to the unrolled-loop constant fold inside IDO 5.3's
uopt. The original's compiler propagated the switch temp's compares into the
global *without* folding the unrolled induction.

## Next hypothesis (pass-level, needs a build-step ticket, not a source fix)

The original's uopt likely propagated copies of **globals** (the switch temp
is a copy of the global unk_110D) but not of **locals** (the loop induction
`i`), while IDO 5.3's zcopy=1 propagates both. Options to investigate at the
cvt_pass/uopt level:

- find whether another uopt option (OPTN record or `-Wb,-` flag) separately
  controls the constant folding of unrolled-loop inductions under zcopy=1
  (dump uopt's option handling; `tools/uopt_patch.py` / uopt_eval.py exist
  for replacement-uopt experiments);
- or model it in the shim: give the procedure val=1 (to fix the compares)
  and block the induction's copy propagation in the ucode (a sentinel trick
  like ugen_pass's, inserted before the loop, removed from the output) - the
  induction is local, the selector copy is global, so a barrier keyed on
  local-variable copies could be uniform;
- re-check the flip experiment's family on the 260 already-matched `$v1`
  selector functions: none of them has a constant-trip unrolled loop, which
  is why the val=0 rule never met this coupling before.

The best source form remains the byte view (val=1 through the pass's own
rule, target-exact selector chain); it is 3 compare rows + ~86 fold rows.
The plain form stays at 3 rows with the case-2 loop matching.

## Reproducing

```
UCD_TAG=plain UCD_OUT=drafts/T-24925/ucdump/plain tools/docker.sh python3 drafts/T-24925/ucdump.py
UCD_TAG=byteview UCD_OUT=drafts/T-24925/ucdump/byteview \
  UCD_REPLACE='switch (*(u8 *)&D_800E6280.unk_110D)' \
  tools/docker.sh python3 drafts/T-24925/ucdump.py        # 89-row fold variant
UCD_TAG=flipplain UCD_OUT=drafts/T-24925/ucdump/flipplain UCD_UOPT_SHIM=uopt_flip.py \
  tools/docker.sh python3 drafts/T-24925/ucdump.py        # plain selector, OPTN val=1
tools/docker.sh python3 drafts/T-24925/ucpretty.py <file.uc>
```
