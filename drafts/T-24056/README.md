# T-24056 short SHOUGATU sibling source checkpoint

Start5d361e017. All three assigned targets remain INCLUDE_ASM. No production,
header or configuration changes; zero new verified C bytes. Prior T-24052
matches remain unchanged. Source-only files in this directory compile against
the current complete translation units, but none is byte-matched.

## Prior evidence and semantic transfer

Matched GYOZI implementations read first: func_80140244/func_80140330 in
src/ovl/GYOZI/801400D0.c and func_801408C4 in80140740.c. SHOUGATU stores the
corresponding level/counter in three-bit CharFlags.f9 fields rather than the
sibling's scalar year/counter variables. Its threshold halves are signed
GameState fields, and its failed gate increments two halves instead of one.
DF14 also increments/clamps the f9 level before the shared display setup.

T-9370/T-10030 already exhausted96 named-threshold variants and a bounded
permuter for D820; these were not repeated. T-9510/T-17500 old target results
are read as negative history, not fresh coverage. New sources use canonical
GameState and f9 fields, correct signed threshold casts and the existing
DisplayActor44-compatible scalar declarations. D92C's predicate must be >=1,
matching the sibling and target comparison, rather than !=0: this restores
one instruction missing from the first port.

DF14 needs scratch-only extern s16 D_80145FDC, supported by the target LH and
SH into display actor+0x14; sibling D_801486DC is also s16. Root approved that
minimal declaration only if a viable match required it. Since no match was
obtained, no production header addition was made.

## Results and bounded negatives

Counts are printable resolved diagnostic rows, not instruction-error counts.

| Source | Target/candidate words | Rows |
| --- | ---: | ---: |
| D820 current sibling/unsigned f9 level (archived) |67/67|62|
| D820 s32 fall-through return view |67/67|62|
| D92C first canonical port using !=0 |76/75|85|
| D92C sibling >=1 predicate |76/76|86|
| D92C named limit and >=1 (archived) |76/76|72|
| D92C explicit packed word reuse |76/76|72|
| D92C packed word plus manual byte counter update |76/76|72|
| DF14 canonical f9 update/clamp (archived) |109/107|12|
| DF14 GameState pointer, global/pointer clamp |109/107|12|
| DF14 explicit byte update/clamp |109/107|64|
| DF14 forced clamp-byte reload |109/109|36|
| DF14 postincrement plus forced reload |109/109|36|
| DF14 explicit assignment plus forced reload |109/109|100|
| DF14 register pointer plus forced reload |109/109|36|

The D92C named limit is marked FAKE because it was introduced to inspect the
arithmetic live range. No such shaping is installed in production. Volatile
and register probes remain ignored scratch only; none was accepted.

D820 retains the established arithmetic temporary-chain problem (target first
threshold in v0, candidate ugen temporaries). D92C remains broad allocation
rather than a single close site. DF14's12-row candidate differs in two connected
address/reload choices: the first byte load uses an extra absolute-base LUI,
while the clamp forwards the earlier stored byte and omits the target's fresh
byte load/store-address sequence. Forcing the clamp reload recovers code length
but still has the wrong initial byte-load address and clamp base choice.
The rest of the initializer already agrees. This concrete type/alias gap is
the useful lead; no blind flag, declaration-order or compiler sweep was run.

## Reproduction and unchanged default control

From the repository root, with one compiler:

```
tools/docker.sh python3 drafts/T-24028/probe.py SHOUGATU func_8013D820 drafts/T-24056/func_8013D820.c
tools/docker.sh python3 drafts/T-24028/probe.py SHOUGATU func_8013D92C drafts/T-24056/func_8013D92C.c
tools/docker.sh python3 drafts/T-24028/probe.py SHOUGATU func_8013DF14 drafts/T-24056/func_8013DF14.c
```

The final annotated files reproduce62/72/12 rows respectively. These are actual
scratch C compilations in the current full TU; the helper reports compiler
failures as errors and never interprets fallback ASM as a C match.

Docker ninja -j1 build/ovl/SHOUGATU.ok had no work. Independent SHA1 of the
unchanged default build passes config/overlays/SHOUGATU.sha1:
d956b1df6fbf9d1f8261d7e1f98ea301f7eb66a9. This proves the existing fallback
build only; no literal C match was obtained or claimed. Scratch variants,
objects and instruction comparisons remain ignored under build/T-24056 and
build/T-24028/reproduce/. Only source and prose are committed.

Ticket is In Review pending root archive review. No push/merge.
