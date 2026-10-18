# T-24048 MASTER animation source pass

Zero new C matches or bytes. func_801344E0 (2912 original instruction bytes) remains plain INCLUDE_ASM in src/ovl/MASTER/80133680.c. No production source/header/config change. One Docker compiler; bounded semantic/lifetime pass, no blind sweeps.

Read current main completion checkpoint f55a2d7e9, MASTER family, T-9960, T-10630, T-18600, available T-14500/T-16100/T-18600 bodies and the copy-state evidence in matching-notes. T-10630's historical 350-row source-only handoff was in scratchpad/a14 and is unavailable in this checkout; the current available typed foundation reports475 rows and freshly reproduces475/469 aligned.

The scene awards stats to thirteen girl records when its event counter advances to3,4 or5 after the failure threshold, otherwise loading its baseball technique scene. It progresses dialogue, initializes a primary sprite plus nineteen particles, animates their fade, and returns to the date scene. The 68-byte display copies use the existing DisplayFixActor44; the ten position pairs and speeds use MasterPoint2 and s16 arrays. The original sprite-clone loop starts at record2; record1 is the particle template. Its unk_18 random value is stored before advancing to the next record. Fade loops update primary record0 separately, then records1..19; the paired reset writes fade first, then clears bit7.

Selected source uses direct byte-counter increments, typed resource-pointer assignments and meaningful names. It compiles to434 diff rows/428 aligned cost. These are score metrics, not literal matching instructions recovered. Its required same-TU forward declaration is in prefix.h; no shared header was changed. selected.c is a body for probe insertion, not a standalone translation unit.

Measured bounded forms (diff rows / aligned cost):

| Form | Result |
|---|---:|
| Current shared types, original available draft |475/469|
| Natural clone store/increment, remove m2c spill-home assignments |471/465|
| Explicit leading fade updates |454/454|
| Replace particle offset casts with x/y members |454/454|
| Share meaningful arithmetic temporary |454/454|
| Scope pointers to phase4 |448/448|
| Assign resource pointers with their real type |454/454|
| Initialize clone pointer after first random call |442/442|
| Existing record96/97 label boundaries |442/442|
| Typed rather than unsigned clone bound |468/456|
| Scratch original end label D_80120BA0 |442/442|
| Counted clone loop plus end label |434/428|
| Direct byte-counter increments |434/428|
| Preincrement byte-counter tests |434/428|
| Redundant byte lvalue |434/428|
| Separate lexical pointer scopes per loop |434/428|
| Final names and typed resource pointers |434/428|

No new impossibility claim: old copy-state observations are a concrete lead, not proof that source recovery cannot work. The first divergence in copy setup remains its end-register choice: original source+2 versus current source+3, with a shifted copy-tail temporary sequence. The current compiler also keeps the clone end pointer live across the RNG call, where the original rematerializes it afterward. The resulting additional stack home contributes to a72-byte frame versus original64. Pointer spill homes differ (clone38 versus34, point3C versus30). Byte counter promotion still uses v0 where the original uses v1. Earlier reward-update regions otherwise follow the reconstructed thirteen-record loops; offsets after the initialization differ. No volatile, inline assembly, compiler exception or per-function patch was used.

The rejected split-label/end-label/byte-lvalue forms are marked FAKE in their preserved scratch bodies and were not selected. They isolate alias/optimizer boundaries and do not justify a header declaration. All loose input scripts, C variants and instruction diagnostics are preserved in ignored build/T-24048-probes; no instruction listing is committed. Older drafts are untouched.

Reproduce in Docker after baseline `tools/docker.sh ninja -j1 build/ovl/MASTER.ok`:

```
tools/docker.sh python3 drafts/T-24048/probe.py func_801344E0 build/T-24048-probes/reproduce.json drafts/T-24048/prefix.h MASTER
```

Create reproduce.json as a JSON array containing `["selected", contents_of_selected.c]` inside Docker. probe.py inserts the supplied body into a temporary full unit, uses the production compiler recipe with guarded C enabled, retains diagnostics, and exits nonzero if any candidate fails. Candidate scores are never a default linked C proof. The untouched baseline MASTER checksum and declaration guards pass; no ASM fallback comparison is counted as new coverage.
