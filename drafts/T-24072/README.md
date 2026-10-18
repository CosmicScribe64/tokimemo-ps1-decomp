# T-24072 model initializer checkpoint

Only ETC func_8014354C (248 original bytes) in src/ovl/ETC/80143240.c
is assigned. Production remains INCLUDE_ASM. The new source reaches5 resolved
+/- rows, not a C match or verified-byte gain. No neighbor, header, configuration
or compiler was changed. Earlier44DF0/44F74 archives stay untouched.

Fresh baseline.c reproduces13 rows from primary main's T-24055/model.c.
That pass already tested split model pointers, literal map arguments, byte
pointer, all-three-vector chained zero and array view. New source adapts the
subsequently verified func_8013F980 integer model cursor and real TaiikuBig/u32
pointer do-loop. It preserves4354C's mapbase0x801F9004, no NOBJ printf, helper
calls, record stride0x50/object stride0x10 and SDK SVECTOR vz/vx/vy zero order.
The entire prologue, model cursor and loop now agree.

| Control | Resolved +/- rows | Result |
| --- | ---: | --- |
| current full-TU T-24055 baseline |13| baseline.c |
| verified integer-cursor/do-loop recipe |5| body.c, retained |
| vz multiplied by zero |5| neutral/rejected |
| separate vz, then vy = vx = 0 |5| neutral/rejected |

The multiply-by-zero control transfers a recent BUNKA_SD zero-store idiom;
it preserves the s16 value's final zero but does not improve this tail. Its
FAKE diagnostic comment states the purpose. The two-field chain follows the
original shared pair and stores vx before vy through right-associative
assignment; prior T-24055 only chained all three. It also changes nothing.
No line-grouping or cast sweep followed.

The remaining tail has three AT/LUI address groups for the typed vz/vx/vy
stores, while the original uses two and shares vx/vy. Original ra reload
scheduling also differs around that grouping. The original is248 bytes;
the draft adds one address-setup instruction. The existing recovered SVECTOR
is retained: no D_80150140 scalar alias was reintroduced. CODING_STANDARDS8
requires fields through their known aggregate; migrate-global keep exceptions
need hoisted read/modify/write evidence not present in this zero-store tail.
No faithful supported next source form is established, and no compiler
impossibility is claimed.

The transferred initializer/do source grouping is explicitly FAKE. Its
necessity was measured for the matched sibling, not isolated for this still
nonmatching target; the source retains the tested layout for reproducibility.

Replay retained forms serially inside the existing tokimemo-decomp-arm64 image:

```
python3 drafts/T-24072/replay.py
python3 drafts/T-24072/replay.py --variant baseline
```

Other variants are listed in replay.py. The helper substitutes only the assigned
fallback in a private full TU, removes the archive-status comment to restore
measured layout, uses explicit-object --resolve --version default --no-build
and checks production unchanged. It validates process results before reporting
scores. Outputs remain ignored in build/etc24072; only source/prose is retained.
Actual default C proof/checksum/interface review has not been reached.


The parent obtained a bounded read-only review, then approved exactly one
additional control: declare SVECTOR *rotation, keep vz=0, take the real
aggregate address after that store, and write vx/vy through the pointer.
rotation-pointer.c is neutral5 with the same residual address/store rows.
The complete full-TU object is identical to the integer-recipe object
(SHA256 `7e87912a5ce7fa6919399ff3ddeca70c005881820d756d4b9fae0cd2b54b903c`).
No aliases, headers or other source choices changed. No pointer sweep or
instrumentation followed; the5-row source remains the checkpoint.

```
python3 drafts/T-24072/replay.py --variant rotation-pointer
```

Final status: unresolved/In Review, not Done. Production remains ASM.
Stop4354C diagnostics at this checkpoint; no compiler-impossibility claim.
