# T-24514 MASTER source reconstruction

Base dba33c4f5, branch codex/t24514-master, isolated tree
/private/tmp/tokimemo-t24514. Zero new verified C bytes. Production source,
headers, tools and config are unchanged. These are compiling research bodies.
No whole translation-unit context or original assembly dump is retained.

## Reproduction

In the existing Docker image, configure the tree and build MASTER with the
original disc mounted read-only. Then run:

```
python3 drafts/T-24514/replay.py
```

The replay replaces just each function's assembly anchor in memory, reads the
current source/header context, and writes ignored build/t24514/replay output.
Its aligned scores are diagnostic; neither candidate is a match. The 363F4
candidate needs the narrow overlay forward declaration of func_80137BF4,
which the replay adds to its private translation unit.

## 363F4 frame and copy setup

The retained source scores 342. It uses the target's own cases 0..3, late
check_end_k calls, and actions in cases 10/11. The shared case 4 typed actor-copy
and point-loop structure comes from T-24048's archived 344E0 selected draft.
That donor is nonmatching; no matching property is assumed from the splice.

Replacing the declared sprite pointer with an indexed clone loop and removing
the one-use raw RNG result local recovers the original 64-byte frame. The first
213 instructions align exactly. The first residual is the aligned struct-copy
end temporary: original t2, candidate t3, with the same copy address registers.
Later the clone loop compares with bne where the original uses pointer sltu
and a conditional branch. Live spill homes and later temporary numbering also
differ. These observations do not establish compiler impossibility.

The enabled T-24092 ugen_pass already fixes the historical F/B direction state
of unaligned block copies. This residual is the register setup of an aligned
68-byte copy. It is distinct from that fixed direction issue; no uniform
compiler defect has been established here. No old direction fix or backend
audit was repeated.

| Causal control | Aligned score |
| --- | ---: |
| Target-specific source plus archived typed shared loops |420|
| Indexed clone loop, remove declared clone pointer |363|
| Indexed point loop instead of point/speed pointers |424|
| Remove one-use raw RNG local with pointer clone |433|
| Remove that local with indexed clone (retained) |342|
| Pre-copy final byte store through byte lvalue view |342|
| Same store through integer address roundtrip |439|

Both address-view controls are experimental FAKE devices; neither was selected.
The initial splice script accidentally selected a nested donor case label;
that invalid scratch variant was fixed before measurement and is not retained.
An overly broad scratch local-removal transformation was likewise restricted
to the target before the stated results. Only compiling selected source is
archived.

## 35FB8 matched-twin source

The retained 58-score source is the T-18600 port of verified existing 33F80 C,
updated to DisplayFixActor44 and ScheduleClubFlags. Its scalar probability-byte
view is inherited from the twin and marked FAKE with its purpose and tickets.
The address of D_8013C57C is still kept in s2 across the initialization region;
the original materializes that saved base only for the later shuffle loop.
First-sum operand/register order and the tail fade colour also differ.

| Causal control | Aligned score |
| --- | ---: |
| Current typed twin (retained) |58|
| Separate extern end marker in the initialization for loop |148|
| Do loop with separate end marker |58|
| Absolute initialization pointer only |62|
| Absolute initialization pointer and endpoint |150|
| Reuse the advancing pointer as an explicit shuffle base |58|

The absolute-address controls are discarded experiments, not proposed source.
The separate end marker adds a precondition guard in the for-loop form; the
do-loop form removes that guard but does not repair the saved-base lifetime.

## Other remaining functions

The historical correctly-framed T-22015 326B4 source rescored at 538. Its known
raw-halfword versus extracted-selector promotion issue remains; no repeated
callee sweep or source grid was run. The 344E0 T-24048 history already covers
these shared animation-loop issues. No new standalone 344E0 experiment was run.
