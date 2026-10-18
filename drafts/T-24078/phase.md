# T-24078 stock phase boundary evidence

One unchanged stock IDO5.3 capture of saved604 source. Existing T24071 wrapper
adapted only paths/hash and enabled only ugen copying; default uopt conversion
and frame/as1 pipeline stayed active. No instrumented/new compiler, altered
options or production source. Unused inherited as1 wrapper branch was NOT enabled.
Mandatory same-source whole-object equality passed BEFORE any interpretation.

- Source SHA256609467db3b985d285c786a9db47057fbb59b4276268d12d39ed6aab98d30713d.
- Saved/captured whole object BOTH67fbefa6e2af48b3ac7a8e508b774cea8c8efd2c4f824bce6269755d29163a00.
- Equality includes.text15008,.rel.text10696,.rodata224,.rel.rodata448 bytes.
- Binary/options/stage hashes are capture-fidelity.json. Raw stage files/objects
  remain ignored build/etc24078/backend604; no original/built dumps archived.

Target ENT56 is the unique input procedure with14 signed DIV records, consistent
with the target's3+3+4+4 divisions. Relative input-record indices below are from
its ENT; output indices are binasm records relative to the matching IENT56.

| Boundary | Measured fact | Supported conclusion |
| --- | --- | --- |
| ugen input128..137 | P=i*136 MPY/STR5 precedes A*11/80 DIV/STR4 | P-before-Q11 already exists BEFORE ugen; do not blame ugen/as1 for creating this order |
| input146..155 | base B=P-Q11-R in reg7, X0=B+24 in reg8 | full-word coordinate optimization retains a separate unoffset base |
| input183..188 | X1 reads reg7, subtracts Q68, adds160 | bypasses X0; folds its24 with136, unlike original D768/D76C X0-Q68+136 |
| input204..211 | x2 stores reg8; THEN reg22(160)-reg17(A) produces H in reg2 | H is still near its first use in optimized input |
| ugen output124,127,137 | x2 SH, then SUBU reg2=reg22-reg17, then y2 SH | H still near Y2 AFTER ugen |
| final object first-arm diagnostic | H subtraction moved before first Q11 quotient completion | movement occurs POST-ugen in ordinary assembly pipeline, not a measured uopt hoist |

This locates boundaries, not every internal pass. No pre-uopt capture exists,
so 'uopt vs conversion pass' attribution of P order remains inference. No
separate as1 input capture was made; do not generalize scheduler behavior beyond
this observed post-ugen movement. Frame pass is unchanged. Earlier combined
'uopt hoist of P/H' wording is superseded by these distinct measured boundaries.

One source implication: original X1 consumes X0+24 rather than the unoffset B.
The earlier joined s16 source's existing final diagnostic retains +136. Parent
approved one matching-only use-site narrowing of temp_v1 in X1, cases0/2,
keeping word storage/all other uses fixed. Low16 equivalence does not prove
original C types; if useful this requires FAKE annotation. No height barrier,
width sweep, new logger or compiler change follows from the evidence.

Reproduce only in existing Docker, one compiler at a time, at the pinned context:

```sh
python3 drafts/T-24078/replay.py --variant word-coordinates
python3 drafts/T-24078/capture.py
```

Capture source SHA guard refuses changed context. Full-object fidelity must pass
before reading copied input/output. tools/cvt_pass.py.parse and
 tools/frame_pass.py.parse/procedures decoded the existing stage files; no new
binary decoder or instrumentation is required.

## Approved use-site control result

Exact X1-only cast in cases0/2 measured638 alignedcost/800rows/594words/frameA0
against604/670/592/A0; rejected as best. Final machine dataflow stores X0 from a0, copies a0 to t5, subtracts Q68 from
that copy, adds136 and stores X1, replacing folded base+160.
Case2 also adds136. This confirms a changed final value web, not a new uopt-stage
capture. It adds two copy words and worsens allocation; no explicit halfword
extension appears. FAKE explains matching-only narrowing and original-type
uncertainty. No further casts or phase instrumentation followed.

## Stock optimizer boundary capture

One additional parent-authorized SAME604 stock compile, unchanged compiler,
options and default conversion. Existing T24071 input/raw/post copy wrapper,
with a frontend copy before default conversion. Whole-object equality again
passed67fbefa6e2af48b3ac7a8e508b774cea8c8efd2c4f824bce6269755d29163a00
BEFORE interpretation; sections/lengths exactly as above. Source same609467db.
Exact wrapper is uopt_capture.py; hashes/options are uopt-fidelity.json.

| Stage, relative to ENT56 | Measured order | Equality/limit |
| --- | --- | --- |
| frontend.B | A*11 MPY104, Q11 DIV106/STR107; P MPY111/STR112 | source statement order survives frontend |
| actual converted pre.B | identical arithmetic and all818 target records | target ENT..END BYTEIDENTICAL to frontend; standard OPTN405=1 is added before ENT |
| raw stock uopt raw.O | P MPY130/STR131, Q11 DIV136/STR137 | reversal occurs inside stock optimization under current default input/options |
| default post-conversion post.O | same762 target records/order | WHOLE FILE byte-identical to raw.O; no post-conversion reordering |
| previous ugen.input | same target ENT..END bytes | independently captured optimizer output agrees with earlier ugen boundary |

The two capture runs' whole-stream hashes differ, but target ENT..END bytes
are identical. Do not claim whole-stream identity across separate stage runs.
Raw/post identity within this capture is exact. The normal default405=1 reset
is recorded; no option was disabled, changed or used as a matching exception.

P reversal is now PROVEN optimizer-origin, not frontend/pre/post-conversion
instruction rewriting. This does not identify the internal optimization pass
or reconstruct original compiler/source settings. H movement remains separately
post-ugen. Source implication: ordering independent source assignments is not
a barrier; arbitrary labels, casts or dummy dependencies do not follow from this
measurement. No faithful new dependence between P and Q11 has been identified.
The remaining narrow question is which existing optimizer transform chooses P
first, and whether a plausible source organization changes that choice without
new operations. No new logger/binary, optimizer edit, no-opt build or further
source control was performed.

Reproduce with existing Docker, same source hash/context, serially:

```sh
python3 drafts/T-24078/replay.py --variant word-coordinates
python3 drafts/T-24078/uopt_capture.py
```

Use only after full-object fidelity passes. Raw stage files remain ignored in
build/etc24078/uopt604; no binary, original assembly or ucode dump is committed.
