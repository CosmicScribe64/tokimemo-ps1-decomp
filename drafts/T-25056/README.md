# T-25056 ENDING scheduling investigation

No production C match. `func_8013A2E8` remains guarded; the unchanged full-TU C
scores12 and the earlier T-24009 pointer-lifetime source scores10. A scratch
assembler counterfactual matches4648 literal linked bytes, but it uses an
experimental assembler and is not a production result.

## Causal finding

The T-24047 reconstructed as1 source is pinned by SHA256 in `budget_probe.py`.
`func_42aa0c`, labels `L42acdc` through `L42ad20`, sets a candidate threshold of
10 for the outer region and100 for a natural-loop region. `region+4` is its
header-block pointer (created by `func_428fc4` at `L42912c`), not instruction
count. The old count is compared with `>` before increment; threshold10 admits
11 dominated candidate blocks. Failed instruction searches still consume a
candidate. This is separate from the weight cutoff.

For A2E8's first flag block10, the admitted equal-weight candidates are
60,70,71,72,73,74,81,118,119,120,319. Candidate320 arrives with old_count11 and
terminates the search. The first flag-arm candidates11 and58 have weight4 and
are never reached. Many earlier candidate instructions fail the control-transfer
path barrier at block26. Raising only this general threshold in the scratch
binary reaches block11 at count15 and block58 at count16. Both delay slots are
then filled correctly, yielding literal linked equality of all4648 bytes.

The scratch default is print-only and reproduces the stock assembler candidate
output exactly (baseline12). `T25056_BUDGET=100` is a diagnostic counterfactual, never used by the
production build. No compiler binary, reconstruction source, binasm or target
binary is committed.

## Reproduction

Use the existing `tokimemo-decomp-arm64` Docker image with this checkout at
`/work`, original disc at `/work/disc:ro`, and external
`/private/tmp/t24047-uopt` at `/trace:ro`. All commands below run inside Docker.

1. `python3 drafts/T-24009/probe.py` reproduces baseline12/pointer10.
2. Copy `drafts/T-24900-as1/{cap.py,cc_cap.py,shim.py}` to ignored `build/as1tr/`.
3. `python3 build/as1tr/cap.py src/ovl/ENDING/801396E0.c /work/build/t25056/base`
4. `python3 drafts/T-25056/budget_probe.py /trace`
5. `python3 drafts/T-25056/replay_budget.py`
6. `python3 drafts/T-25056/probe.py LABEL...` runs selected source controls.

The environment override affects only explicit invocations of the scratch
assembler. The exact patch is the small `replacement` string in
`budget_probe.py`. Capture files and compiler outputs stay ignored under
`build/t25056/`.

## Source controls

| Control | Aligned cost |
| --- | ---: |
| Direct string unsigned-false test |12|
| String high-word unsigned-false test |12|
| Flag unsigned-false test |12|
| String then flag / flag then string separate tests |12/12|
| Reused pad pointer, unsigned-false test |10|
| Pointer then second-mask unsigned-false / truthiness test |660/660|
| Single-iteration flag-chain loop, false unsigned flag test |508|
| Same loop, precomputed unsigned top test |564|
| Same loop, unsigned string-address test |1093|

These are causal probes, not recovered original source. No candidate improved
the existing pointer source. The loop probes preserve C behavior but materially
change allocation and scheduling, so none was promoted.

## Remaining hypotheses

The pointer source puts both halves of the first string address in the flag
block; local scheduling places the low half in its branch slot. The desired
output needs only the high half there and the low half in the call arm. A
source-only result needs upstream separation of those halves or a control-flow
representation that permits the existing global search to reach the arm.

A general budget change would require evidence beyond this one function and a
whole-corpus comparison. It would require recompiling every currently matched
C unit with stock and diagnostic backends, comparing code/data/relocation
sections and all27 final binaries, and rejecting any existing-match regression.
It also needs a compiler-history explanation for100 instead of10; a single
positive target is insufficient. No such production change is proposed here.

## Backedge phase boundary

The first-arm-only loop preserves the remaining flag chain outside the loop.
The first call exits directly to the original join. Conditions `flag * 0` and
`(u32)flag < 0` both score668; immediate `flag & 0` folds early and retains12.
Captured output has1164 instructions, the same as the stock candidate (the
original target is1162). The loop has vanished before as1 region analysis:
block10 still has threshold10 and the same candidate sequence. The first
mask changes from temporary t9 to promoted v1 and rotates later temporaries.

An existing `pad` scalar initialized with `(u32)flag < 0`, then used as the
loop condition, is a later carrier. Plain carrier scores682; adding an
unreachable `if (pad) { pad += (u32)flag < 0; }` scores710; making that identity
update the flag byte instead scores733. The identity-carrier capture proves
that a natural-loop region survives: destination11 scans candidate blocks
12,14,13 with threshold100. It also retains two zero moves, an identity add,
and real branches, and has1170 instructions. That code is not an admissible
match. The flag load has been hoisted out of the loop header by uopt, so the
larger budget alone also does not put the wanted filler into its load slot.

These two captures distinguish the phase boundary: early-folded backedges
cannot affect as1 regions; a retained carrier creates the desired region but
also unwanted runtime code. The next source hypothesis must address both.

## Conditional pointer and compiler-history controls

Two first-call argument controls initialize `top = (u32)flag < 0` and select
`(flag & 2) ? p : p + top`. The direct string form scores692 and the existing
pad-pointer form691. Both pointers are defined and equivalent, but these
controls do not provide the needed address-half separation.

The cached original IDO5.2 assembler has the same region-header selection of
100/10 at0x4284b0 (IDO5.3 equivalent0x42acdc). Its binary SHA256 is in
`evidence.json`. Replaying the exact captured input through original5.2 yields
the same1164 resolved instructions as stock5.3, with zero differing positions.
This rejects a simple5.2-to5.3 budget-rule change as the explanation.

Replay with the existing `tokimemo-decomp-ido52` image (`--platform linux/amd64`),
this checkout at `/work`, and the external cache at `/trace:ro`. Copy
`build/t25056/base/in.T` to `build/t25056/budget/ido52.T`, then run inside Docker:

```
/trace/ido5.2/usr/bin/qemu-irix -silent -L /trace/ido5.2 \
  /trace/ido5.2/usr/lib/as1 -elf -G 0 -p0 -EL -g0 -O2 \
  build/t25056/base/in.G -o build/t25056/budget/ido52.o \
  -t build/t25056/budget/ido52.T
```

Compare with `funcdiff.resolved_functions` against `build/t25056/base/unit.o`.
The cached4.1 assembler is unavailable at documented local locations; no
download or installation was attempted.

The scratch raised-budget output also preserves all actual instructions in
the15 sibling functions in this TU. The final function's trailing section
padding changes as A2E8 shrinks8bytes; `func_8013B640` itself independently
passes literal linked MATCH96bytes. This is only a local non-regression check,
not the whole-corpus evidence required for any general rule.

## Proposed next research decision

The open question is whether the original assembler used a different general
search rule, or whether the original source supplied different pre-scheduling
control flow. Neither is established. The known pointer source gets the first
string high half into the load-delay slot, but its low half then takes the
branch-delay slot needed by the second flag mask. Mask staging, late-false
preloads and retained-backedge controls have not separated those events while
preserving allocation and runtime instructions.

Budget consumers include later required calls and joins, not just empty labels.
There is no supported source edit yet that removes enough consumers while
preserving the target's instruction graph. Removing tail code is diagnostic
only. A shared string-copy call would change that graph and is not justified
as the next matching probe.

For coordinator review, the next general-rule investigation would be:

1. Replay this pinned input with an independently obtained original assembler
   version from the plausible production lineage, especially4.1. Inspect the
   region-header budget selection and actual admitted candidate sequence.
   The existing5.2 result is a negative control. A version with the same gate
   would reject that historical explanation; no per-function version choice
   is justified by this experiment.
2. If historical evidence supports a different general rule, instrument that
   rule in a scratch backend without changing production tools. Compare its
   decisions on ENDING and on small functions whose scanner never hits the
   existing gate. Keep failed attempts distinct from accepted motion, and
   count blocks separately from instruction candidates.
3. Pin a current integration commit and build all matched C translation units
   with the unchanged frontend and both backends. Use separate output roots.
   Compare each matched function's resolved instructions and each object's
   code/data/relocation sections. Record padding-only differences separately;
   do not normalize changed instructions or relocation targets away.
4. Link both complete builds and verify every one of the27 reference checksums,
   declaration guards and prototype checks. Any regression in an existing
   match rejects the proposed rule. Review source, standards and historical
   rationale before any production adoption; corpus success alone does not
   explain the rule.

The current scratch threshold100 simply reuses the natural-loop budget for
outer regions. It is a useful counterfactual, not evidence that the game's
assembler actually did that. No production implementation is requested.
