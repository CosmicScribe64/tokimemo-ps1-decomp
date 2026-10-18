# T-24078 stack map and limits

Read-only comparison of original assembly and current628 final instructions.
Existing frontend/raw optimizer captures belong to604, not628. No new capture,
compiler invocation or layout control accompanies this map.

| Region or object | Original frame0x90 | Current628 frame0xA0 | Classification |
| --- | --- | --- | --- |
| Outgoing register-argument build area | 00..0F | 00..0F | ABI call area; calls use at most four register args |
| 10..17 | no observed accesses | no observed accesses | unclassified reserved/alignment space; not invented locals |
| s0..s7,fp,ra saves | 18..3C | 18..3C | ten callee saves, identical offsets |
| 40..4F | no observed accesses | no observed accesses | standard project frame-pass extra16 region |
| UV word160 | 50 | 90 | genuine persistent local, initialized then updated after AddPrim |
| UV word24 | 54 | 94 | genuine persistent local, same lifetime |
| page320 induction | savedfp | 64 | current cross-call spill/home; original register resident |
| two polygon pointers | 84/88 | 98/9C | address-taken genuine local array, cursor advances4 twice |
| word above polygon array | 8C | none | reserved original slot, identity unknown |
| Remaining local-area words | no direct accesses | no direct accesses | cannot identify declarations from absent loads/stores |

Original local-area span50..8F is64bytes; current span50..9F is80bytes.
This does NOT mean all bytes are live locals. No extra outgoing stack arguments
or transient call spills are observed beyond current page home64. The original
word8C above poly is evidence against assuming poly was the first declaration.

Existing604 frontend maps declared homes from the top downward:
poly[-8..-1], sp54[-12], sp50[-16], temp_a1[-20], temp_a2[-24],
temp_v1[-28], temp_a0[-32], temp_lo[-36], temp_lo_2[-40], temp_lo_3[-44],
temp_lo_4[-48], temp_lo_8[-52], temp_v1_2[-56], var_fp[-60], var_s4[-64],
var_s0[-68], temp_v0[-72]. At LOC448 cfe additionally copies arg1 into
M-76 for switch selection. This is a compiler-created selector home, not a
source declaration or a guessed user pad. Raw optimized DEF M length remains76;
only UV(-12/-16) and page(-60) still have actual M loads/stores. Four-byte
rounding explains80 reserved bytes. The same declarations and final offsets
in628 agree with this model, but no628 frontend snapshot proves identical IR.

The documented IDO rule in wiki/matching-notes.md T3330 allocates even promoted
or unused declared locals when any address-taken array remains. First declared
is highest, with rounding below the last home. This explains why eliminating
runtime accesses does not itself shrink the frame. Scoped case-lifetimes control
already demonstrated increased homes/frameD0; do not repeat lexical scoping.

A genuine counter declared before poly could explain the original word8C;
later UV declarations could explain low50/54. These anchors do not determine
all intervening original declarations or whether its selector used a hidden
home. Therefore no arbitrary permutation or padding experiment is justified.
Source inspection does identify two redundant disjoint coordinate roles:
temp_v1 (cases0/2) and temp_a1 (cases1/3) both hold X0; temp_lo_4 (Q68 in0/2)
and temp_a2 (X1 in1/3) never overlap across arms. Merging either is a real
same-type lifetime reuse, but alone does not predict both original frame and
UV offsets. Existing shared-temps merged only arithmetic aliases; no such
coordinate-role merge has been measured. This is a bounded structural lead,
not proof of original spelling or authorization for a declaration sweep.


One constrained counter-first control on628 moves only existing var_s4 before
poly[2], with explicit FAKE unknown-original-order annotation. Prediction before
compile: unchanged76B homes/frameA0; poly94/98, UV1608C/UV2490, page60, counter
unused home9C above poly. All observed offsets match the prediction exactly;
628cost/856rows/592words/frameA0 unchanged. Arithmetic/register allocation is
unchanged. This confirms the declaration anchor mechanism, not identity of
original8C or a16B frame fix. No UV move, removed local, padding or sweep.
Original poly84/88 is still16bytes below the new array, so the unknown local
count/lifetime gap remains separate from this one-word anchor.
