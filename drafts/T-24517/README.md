# MASTER363F4 consultation packet

Source checkpoint: e49b55556, base c768eda884. Production remains unchanged;
zero new C bytes. T-24517 stays In Progress. Current source has two real scalar
locals, i and the RNG result value, with indexed clone and point/speed loops.

## First unresolved step

All 652 diagnostic instruction positions match opcode/immediate/relocation
annotations after register fields are masked. Frame 64, clone sltu/bnez,
clone/speed spill 34 and point spill 30 agree; first 213 diagnostic words agree.
There are 167 register-only substitutions, aligned cost 330. funcdiff keeps strings/tables
normalized, so this is not literal linked-byte proof.

At zero-based row 213 (offset 0x354), target uses `addiu t2,t0,60`; candidate uses
`addiu t3,t0,60`. Both have copy source t0 and destination t9. Target end is
source+2 in ring order t6,t7,t8,t9,t0..t5; candidate end is source+3.

Exact stock/traced sections (.text/.rel.text/.rodata/.rel.rodata) and all 652
checkpoint target keys agree. Final uopt ranges use only v0/v1/a0/a1; no
range is assigned t0..t9. The first copy-end t3 has no trace live range.

T24092's enabled ugen shim inserts gadgets before every Umov. Its prior
6337-function preservation audit was read, not repeated. One exact-candidate
control omitting only that shim gives identical complete text/data/relocation
sections and unchanged 330 target score. The current sentinel is not causal here.
This aligned-copy register question is distinct from the fixed F/B direction bug.

## Small source evidence

`copy68-repro.c` contains two source-equivalent ways to clear byte 4 then copy
an aligned 68-byte union. Current pipeline results:

| Store spelling | Source/end temporaries | Relative end |
| --- | --- | --- |
| Direct byte field |t7/t0|+3|
| Integer-address view |t8/t0|+2|

The full candidate's integer-store control also gets source+2, but changes its
preceding address setup, so it is not a solution. Separate record97 naming did
not help. Relative ring choice alone does not prove a compiler defect or C
impossibility; source lowering and dead temporary consumption can differ.

## Exact commands

Use tokimemo-decomp-arm64 with this checkout at /work, original disc at
/work/disc:ro, and existing /private/tmp/t24047-uopt at /scratch:ro. After
configure and `ninja -j2 build/ovl/MASTER.ok`, run sequentially:

```
python3 drafts/T-24517/checkpoint.py
python3 drafts/T-24517/trace_fidelity.py
python3 drafts/T-24517/shim_control.py
python3 tools/cc.py drafts/T-24517/copy68-repro.c build/t24517/copy68-repro.o ido 5.3
```

Current generated evidence is in build/t24517/{checkpoint,tracer,sentinel-control}
and copy68-repro.asm. All are ignored; no binary, ucode or assembly dump is
archived. checkpoint.py retains REGIMM selector/syscall/break bits and asserts
COP formats absent; existing-object validation remains 167/0.

## Consultation question

Can ordinary C89 preserve the target's preceding address setup and copy bases
while giving the source+2 copy-end relation, or what concrete ugen input/state
would distinguish it? No production compiler patch, source register forcing,
version sweep or impossibility claim is proposed.
