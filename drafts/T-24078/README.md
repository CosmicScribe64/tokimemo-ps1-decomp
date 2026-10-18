# T-24078 two-polygon renderer checkpoint

Active recovery of only ETC func_8013D570 (2352 bytes), src/ovl/ETC/8013BEF0.c.
Production remains ASM; no helper, header or config changed. These are private
full-TU default resolved diagnostics, not linked C matches.

Original frame0x90; VRAM8013D5BC sets cursor tosp84, 8013DE64 advances four
bytes and8013DE4C/DE68 enforce two iterations. This proves POLY_FT4*poly[2]
atsp84/88. Baseline fixes old singleton storage, all16 malformed aggregate
frame-index accesses to real GameState.unk_018 and two-argument AddPrim's byte
order-table address. Matched same-TU D32C supplies the SDK field/array idiom.
Historical T17300 plain792/typed957 are different contexts, not fresh results.

| Current full-TU source | Aligned cost | Printed +/- rows | Words | Frame |
| --- | ---: | ---: | ---: | ---: |
| baseline.c |638|800|594|0xC0|
| shared-temps.c |638|800|594|0x98|
| sdk-uv.c |678|790|582|0x98|
| uv-pair.c |638|800|596|0x98|
| case-height.c |638|800|596|0xA8|
| inline-height.c |678|790|582|0x98|
| tile-width.c |648|806|594|0xA0|
| indexed-page.c |638|800|594|0x98|
| body.c / word-coordinates.c |604|670|592|0xA0|
| word-uv-pair.c |606|682|594|0xA0|
| case-lifetimes.c |604|670|592|0xD0|
| case0-compound.c |635|703|593|0xA0|
| x1-narrow.c |638|800|594|0xA0|
| origin-first.c |628|856|592|0xA0|
| origin-x1.c |624|792|594|0xA0|
| origin-void.c |628|856|592|0xA0|
| origin-uv-pair.c |634|862|594|0xA0|
| counter-first.c |628|856|592|0xA0|

Original588 words/frame0x90. Rank with tools/funcdiff.py diff_score on resolved
instruction keys (insertion/deletion1, substitution2). Printed rows use different
alignment rules for equal/unequal lengths and are diagnostic ONLY. The earlier
790-row wording did not establish improvement: sdk-uv has WORSE aligned cost.
The retained body is now the604-cost word-coordinate form; it has fewer
words but a larger frame than the638-cost joined form. None is a match.

The shared-temp control joins12 same-type aliases whose lifetimes occupy
mutually exclusive switch arms, preserving expressions and store order. It
removes40 frame bytes, not the hypothesized48. SDK UV assignments remove
inherited signed-byte casts but promote scalar coordinates to saved registers,
spill mode/tpage and replace original MULTU136 with shifts. The FAKE-tagged UV
word-pair view restores LW and saved136/MULTU, but adds two words and retains
early160-arg0 plus spilled320. Splitting height temporaries by case only enlarges
the frame; inlining height is no gain. A named136 tile-width local costs648 and
adds a home, so is rejected. No source form tested simultaneously restores the
original stackUV50/54, saved136/320 and DIV-before-MULTU schedule. This is a
concrete unresolved source/allocation gap, not an impossibility claim.

Initial malformed parentheses during field substitution failed compilation and
were repaired before any score; that failure is not counted as a baseline.

Replay in existing Docker, one compiler at a time:

```sh
python3 drafts/T-24078/replay.py --variant shared-temps
python3 drafts/T-24078/replay.py --variant baseline
python3 drafts/T-24078/replay.py --variant uv-pair
```

The helper compiles only an ignored full-TU copy, invokes explicit default
resolved funcdiff and validates its diagnostic status. It accepts exactly the
ASM fallback or exact pinned body after any promotion, and checks production
text remains unchanged. Output: build/etc24078/replay/<variant>/diagnostic.txt.
The retained s32 fallthrough is conservatively FAKE-tagged: current implicit-int
callers ignore its result, but original return type is unproven.

The remaining eight-byte frame excess and expression scheduling require a new
source mechanism before further probes. No allocation/type sweep or compiler
instrumentation; ownership remains active pending focused direction.

The parent-authorized bounded Astra read-only review supplied one untried form:
matched D32C derives pageX from its loop index. Against shared-temps only,
indexed-page removes var_fp declaration/init/increment and passes
0x140 + var_s4 * 0x80 to ECB0. Original two iterations make this equivalent.
Measured neutral638cost/800rows/594words/frame98; compiler recreates320 induction
but spills it atsp50 instead ofsp60. Mode remains s7, constant160 s6,136 fp.
This does not restore original fp320 or frame90. Source and diagnostic preserved;
no other review hypothesis or new control was tested. Astra idle; no further
unsupported probes or instrumentation. Ticket remains active pending direction.

Read-only arithmetic/lifetime audit: relationships.md maps all four original
arms and shared tail, with exact full VAs. No missing operation, wrong formula
or address was found. The three inherited s16 coordinate temporaries narrow
earlier than original register values; later low16-only uses mean this is NOT a
proven behavior bug, nor proof of original C declarations. Parent approved one
structural control changing ONLY temp_a1/temp_a2/temp_v1 to s32 against the
joined638 baseline. It reaches604cost/670rows/592words/frameA0 (original588/90).
Whole object SHA25667fbefa6e2af48b3ac7a8e508b774cea8c8efd2c4f824bce6269755d29163a00.
The first divergent arithmetic still places MULTU i*136 before the first signed
Q11 DIV, whereas original executes DIV before MULTU. UV signed-byte loads and
early160-A remain. No additional width, UV combination or ordering probe begun.

Further original-backed controls against604, one at a time:

- word-uv-pair preserves two persistent UV words until SDK SB stores; unknown
  original aggregate remains FAKE-tagged. It restores LW but606cost/+2words is
  worse, so retained best stays604.
- case-lifetimes scopes only arithmetic temporaries within each switch arm,
  preserving existing relative declaration order, types, formulas and stores.
  No such value crosses an arm or any call. It is neutral604 but grows frame toD0;
  IDO does not reuse these lexical homes. No declaration-order sweep followed.
- case0-compound inlines only Q11/P/R aliases into case0 X0/X3 expressions with
  exact original grouping; other arms fixed.635cost/593words/A0 is worse. First
  arithmetic still MULTU-before-DIV; no transfer to other arms attempted.

All twelve variants remain source-only; helper output prints aligned cost,
word count and first frame instruction. Retained604 is NOT a byte match.
The unresolved question is why invariant tile-spacing and height computations
move ahead of the original first Q11 DIV despite agreeing arithmetic. These
controls do not identify a wrong operation or prove a compiler limitation.
Recovery remains active on this one function; next read-only work must find a
supported source relationship before another compile, not a width/order sweep.

The one phase-backed X1-only s16 use-site cast (cases0/2, storage and other uses
remain word-sized) reaches638cost/800rows/594words/A0, worse than604. Final
code confirms the intended dependency change: X0 stored from a0, copied to t5,
then X1 subtracts Q68 from that copy and adds136. Case2 likewise restores136.
No explicit sign-extension instruction is emitted, but two copies add two words.
Thus original X1 association is restored without an overall gain; retain604.
The matching-only casts are explicitly FAKE, not a semantic repair. No other
width, ordering or new stage capture accompanied this test.

Current smallest phase question: source statements assign Q11 before P, yet
ugen input already orders P before Q11. Existing capture does not show whether
this originates in frontend output or optimization. No pre-uopt observation
exists for this input and no additional diagnostic compile has been authorized
or executed by this packet. Further source forms need a supported relationship.


Origin-first control (cases0/2 only) changes X0 from (P-Q11)-R+24 to
((24-Q11)+P)-R, matching the original-backed lower-X3 association. It produces
628 aligned cost /856 diagnostic rows /592 words /frame0xA0. Numeric best604 is
retained separately; origin-first is the causal frontier because final code now
executes Q11 DIV, then P MULTU, then R DIV as original. No invented dependency,
new operation or barrier. Actual callers pass0 or counter*2 with counter0..40;
within A0..80 and i0/1 the reassociation is equivalent without signed overflow.
Cases1/3 were unchanged: saved604 already emits their Q141 before P.

Read-only region comparison uses each object's own switch-arm boundaries,
not fixed offsets. Original boundaries0/57/167/274/402/524/588;604 boundaries
0/59/168/276/403/526/592;628 boundaries0/59/167/275/402/526/592. Regional aligned
costs (prologue, arms0..3, tail) are604:[40,121,51,165,153,74] and
628:[38,108,129,143,134,76]. These independent regional alignments do not sum
to the whole-function score and do not replace literal matching. An unchanged
arm1 can change allocation when the neighboring expression graph changes.
Original X1 uses X0-Q68+136; optimization folds the retained word expression to
B-Q68+160. The earlier X1-only narrow control changed that web but added two
copies against604. Its behavior with the corrected Q/P order is not measured.
This checkpoint includes no further source control or diagnostic scope.


Correction after inspecting final words: origin-first628 ALREADY uses actual
X0-Q68+136 in cases0/2 without a narrow cast. The prior pending-X1 description
was an unverified carryover from604's folded160 expression. The one subsequent
origin-x1 control adds only X1-use (s16) casts, otherwise identical628 source.
It measures624 aligned cost /792 diagnostic rows /594 words /frameA0, but adds
MOVE a0-to-t5 in each arm. It preserves Q11/P order and changes allocation;
it does NOT restore a previously missing dependency or add a sign extension.
Retain604 numeric best and628 no-extra-copy causal frontier. Preserve624 only
as a measured added-copy control, not a semantic/structural improvement. Its
FAKE comments describe intent; final code shows the dependency already existed.
No further control follows from this result. Active same target, production ASM.


One private void(s32,s32) return-view control against628 is target-neutral:
identical resolved target,628cost/856rows/592words/A0. Private full-TU forward
matches definition exactly; production/header unchanged. Audit finds six actual
calls (three each C330/C700), all ignored results; original DE44..DE9C epilogue
sets no explicit return. Matched D32C is s32 fallthrough, so this evidence does
not prove original void. All35 full-TU function bodies were compared: C330
alone changes9 words (alignedcost18), from v1 to v0 for post-call F5F tests and
1104 increment. Other34 including target identical. Thus even ignored-result
callers can depend on return liveness for matching. Reject promotion of void;
retain conservative FAKE s32 fallthrough, no invented return value. The private
control remains reproducible via --variant origin-void, with its exact forward.
No new structural source lead follows from this neutral result. Same target active.


One current628 word-UV-pair transfer follows explicit verification that628
still uses LB at UV accesses. Geometry, coordinate types, source ordering and
s32 signature remain fixed. The existing FAKE aggregate view is unproven as
original C but represents the two observed persistent stack words. Measured
634cost/862 diagnostic rows/594words/frameA0, worse628 and two extra words.
It restores LW atsp94/sp90 (originalsp54/sp50), with repeated reloads before
u2/u3 rather than original word reuse. Mode stays s7,136 staysfp,160 stayss6,
page320 remainssp64; no cross-call/register/frame repair. Reject as frontier,
preserve exact source/replay. Earlier638/604 pair results were not assumed to
predict this outcome. No new stage capture or geometry claim. Same target active.

See stack-map.md for original/current slot classifications and the limits of
the existing604 frontend observations. No new capture or layout control.


One constrained counter-first control on628 moves only existing var_s4 before
poly[2], with explicit FAKE unknown-original-order annotation. Prediction before
compile: unchanged76B homes/frameA0; poly94/98, UV1608C/UV2490, page60, counter
unused home9C above poly. All observed offsets match the prediction exactly;
628cost/856rows/592words/frameA0 unchanged. Arithmetic/register allocation is
unchanged. This confirms the declaration anchor mechanism, not identity of
original8C or a16B frame fix. No UV move, removed local, padding or sweep.
Original poly84/88 is still16bytes below the new array, so the unknown local
count/lifetime gap remains separate from this one-word anchor.
