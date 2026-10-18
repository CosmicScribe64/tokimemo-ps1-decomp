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
against604/670/592/A0; rejected as best. Final machine dataflow shows X0/a0 ->
SH x0, MOVE t5=a0 -> subtract Q68 -> add136 -> SH x1, replacing folded base+160.
Case2 also adds136. This confirms a changed final value web, not a new uopt-stage
capture. It adds two copy words and worsens allocation; no explicit halfword
extension appears. FAKE explains matching-only narrowing and original-type
uncertainty. No further casts or phase instrumentation followed.
