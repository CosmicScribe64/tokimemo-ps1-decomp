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
| shared-x0.c |628|856|592|0x98|
| base-height.c |628|856|592|0x98|
| right-scratch.c |624|852|592|0x90|
| if-chain.c |632|862|590|0x90|
| direct-work.c |624|852|592|0x90|
| uv-first-word.c |671|873|583|0x90|
| uv-captures.c |624|852|592|0x90|
| uv-capture-store.c |624|852|592|0x90|
| indexed-uv.c |622|850|592|0x90|
| indexed-texture.c |624|852|592|0x90|
| indexed-uv-right.c |618|846|592|0x90|
| height-unsigned.c |606|836|592|0x90|
| height-x0.c |618|846|592|0x90|
| height-capture.c |618|846|592|0x90|
| height-first-narrow.c |650|804|594|0x90|
| height-staged.c |618|846|592|0x90|
| height-negated.c |616|846|588|0x90|
| height-negated-boundary.c |635|769|599|0x90|

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


One disjoint common-X0 reuse on counter-first628 removes only temp_a1's s32
declaration and uses temp_v1 for its assignments/reads in cases1/3. Cases0/2
already use temp_v1 for the same X0 role. No value crosses arms/calls/iterations;
all formulas, types, stores and relative declarations remain fixed. Before
compile the predicted home count was76 to72 bytes and frameA0 to98, poly down8.
Measured628cost/856rows/592words/frame98, exactly that frame prediction.
Poly8C/90, UV16084/UV2488, page3205C; saves18..3C unchanged. Page home moves4,
not8, because removing the earlier declaration changes its negative offset.
No arithmetic or register allocation gain; one real source home removal yields
8bytes through alignment. Remaining8B frame excess and UV placement remain
separate unresolved questions. No further merge or layout control follows.


Two further disjoint lifetime controls were executed strictly sequentially.
First, base-height reuses existing temp_a0 for the horizontal E base in cases1/3,
then H after x2. E's last use computes rightX before x1/y1/x2; H starts afterx2,
so no overlap or cross-call/arm/iteration value. Only temp_v1_2 declaration and
uses change. Predicted72-to68 homes, unchanged frame98, poly8C/90, UV84/88 and
page60. All observed offsets match;628cost/856rows/592words, no arithmetic gain.

Second, right-scratch reuses temp_lo_4 for Q68 in cases0/2 and rightX in cases1/3,
removing only temp_a2 declaration and replacing its uses. These are mutually
exclusive arm-local s32 lifetimes, never crossing calls or iterations. Formulas,
store order and every other declaration remain fixed. Original local identity
is uncertain; the merge is source-level lifetime reuse, not recovered naming.
Predicted68-to64 homes/frame90, poly84/88, UV1607C/UV2480, page5C. All observed
slots and frame match predictions. Result624cost/852 diagnostic rows/592words.
The four-cost gain matches original frame allocation/deallocation; no added
operations or arithmetic/register change. This is a new frame-correct structural
frontier, distinct from the earlier X1-cast624/594words/frameA0 negative.
Keep604 numeric best and all628 corrected-order checkpoints separately.
No frontend capture was made for these controls; home byte totals are the
source-derived model validated by final offsets/frame, not measured DEF records.
UV50/54 and page-register allocation remain unresolved. No padding or further
layout permutation accompanied these controls; production remains ASM.


Original dispatch graph established before control: D620/D62C/D638/D644 collect
four mode0/1/2/3 comparisons, then default D64C branches toDDA4 with UV load in
its delay slot. Arms0/1/2 jumpDDA0, arm3 fallsDDA0; DDA0 loads UV, then common
DDA4 tail. Current right-scratch switch has this same graph; its load is LB,
not original LW. No branch-graph correction was needed.

One source-equivalent if/else chain tested the single copied-switch-selector
versus direct predicate reads. All declarations, formulas, stores and signature
fixed. Prediction frame90 and all observed slots unchanged confirmed. Result
632cost/862rows/590words, worse624 despite two fewer words. Final code interleaves
conditions with arms: first BNE mode!=0 goes to the next condition, rather than
original four contiguous BEQs before arm0. Mode7/160s6/136fp, page5C, UV7C/80
and poly84/88 unchanged. This is faithful behavior but not original final CFG;
reject as frontier, no alternate if/goto/condition spelling sweep. No frontend
capture exists, so no claim that a hidden home was removed or relocated. The
original/current switch graph remains the supported structural frontier.


Two sequential ordinary controls, not a width/layout sweep. First direct-work
uses matched D32C organization: assign GetWorkBase directly into *var_s0 and
pass *var_s0 to F040, remove only immediate temp_v0 alias declaration. No
intervening side effect. Predicted model64-to60 homes/frame90 unchanged through
rounding, visible slots unchanged; final target is resolved IDENTICAL to624
right-scratch (592words/frame90). This cleaner source remains a separate replay.

Second uv-first-word removes only (s8) from first sp54 use at u0, holding second
u2 cast and all sp50 uses fixed. Original DDA0 LW v0 remains live across SBs
to u0/u2; current scalar form uses LB. Predicted a word view might change
load/reuse, but actual result promotes UV24 to saved s7 across calls, mode tofp,
and136 no longer saved. No original tail LW appears;671cost/873rows/583words/
frame90, worse despite fewer words. The one-use matching-only control has an
explicit FAKE comment; original spelling is unproven. No mirrored other-UV,
height-width or cast sweep followed.

Original UV lifetime distinction is now precise: persistent two words are
memory-resident across calls/iterations; loaded copies v0/v1 each feed two
SDK SBs before AddPrim. Word-pair controls kept memory but reloaded before
second uses; scalar full-word controls promoted persistent values acrosscalls.
A possible faithful recipe is capturing the persistent pair into already-dead
coordinate scratch locals at shared tail, preserving exact original load/store
positions. That would add no homes and explicitly retain per-iteration snapshots
across potentially aliasing polygon stores. No such control has run here, and
unknown original persistent aggregate remains FAKE if used. No H width guess.


Two UV lifetime controls executed sequentially from direct-work624. First,
uv-captures represents persistent UVs as the previously audited FAKE s32pair,
but captures pair[1] into dead X0 scratch before u0 and pair[0] into dead H
scratch before u1. Captures feed both corresponding vertex stores. No added
home: the arm values are dead at the shared tail. SDK stores and geometry
remain ordered identically. Prediction frame90/no new homes confirmed.
Result624cost/852rows/592words/frame90; UVstill7C/80, poly84/88, page5C,
mode s7/160s6/136fp unchanged. Two LW copies v0/v1 each now feed
u0/u2 and u1/u3 without reloads or saved-register UV promotion. This restores
original memory-across-calls versus register-copy-across-stores lifetimes.
Unknown original persistent aggregate remains explicitly FAKE, not a new type
claim. No additional snapshots escape the tail or cross AddPrim.

Second uv-capture-store changes only second capture placement: u1 receives
(temp_a0 = texture_u[0]) rather than a separate immediately preceding capture.
The first capture remains separate, matching original UV24-before-poly order.
Prediction no count/frame/home change confirmed:624/852rows/592words/frame90.
Final second sequence now matches original ordering: poly-address LW, UV160 LW,
li a2,128, then SB. Previously separate capture emitted UV160 LW before poly.
This is a restored original-backed dependency/order despite unchanged global
score. Retain uv-capture-store as the latest causal structural frontier, with
604 numeric best and earlier624/628 forms kept independently. Main remaining
observations are saved160/page displacement and early H, not UV load/reuse.
No H type guess, logger, source permutation or production edit accompanied it.


Three sequential original/matched-source controls recover UV spill placement.
First indexed-uv removes persistent UV pair declaration/init/+8 updates and
uses captures i*8+24 beforeu0 and i*8+160 insideu1. Original i0/1 loop proves
identical values; matched D32C similarly expresses UVs through its loop index.
Earlier indexed-page control already showed source index expressions generate
bottom-of-frame induction spills. Predicted possible generated spills, not a
promised match. Result622cost/850rows/592words/frame90, poly84/88 unchanged.
Despite NO declared UV object or source updates, output initializes two words
at54/58 and increments8 at latch; these are proven candidate-derived induction
spills. Original50/54 still differs4bytes. LW/reuse/store order remains correct.
No unknown UV aggregate FAKE remains in this variant. Page explicit home64;
mode7/160s6/136fp persists.

Second indexed-texture derives page as320+i*128, removing only page local/init/
update on indexed-uv. This matched D32C recipe was previously tested only with
explicit UV locals, not derivedUVs.624/852rows/592words/frame90 is worse622;
UV54/58 unchanged, generated page spill5C, latch ordering changes. It does not
repair original pagefp or saved160. Preserve negative, do not transfer blindly.

Third indexed-uv-right restores a genuine separate rightX local temp_a2 in
cases1/3, leaving Q68 temp_lo_4 in cases0/2. The prior disjoint reuse was legal,
but their semantic roles differ and original local identity remains unproven.
No padding or arbitrary declaration permutation: restore the actual coordinate
local at its pre-merge relative position, belowpoly. Prediction: fill four-byte
rounding gap without frame growth; generated UV spills50/54, page64-to60,
poly84/88 unchanged. All observed offsets match.618cost/846rows/592words/frame90.
Source-derived home model: indexed-uv48declared bytes +4hidden switch +8derived
spill bytes =60, rounded64; restored coordinate adds4 and removes the rounding
gap. These byte totals are inferred from source and existing604 home rule, NOT
a new frontend capture. Actual final spills/frame are measured directly.

Latest causal structural frontier is indexed-uv-right618, distinct from604
numeric best. Frame90, poly84/88, UV50/54 and two LW-copy lifetimes now agree
with original in this candidate. This does NOT prove original declaration or
source spelling and is NOT a linked byte match. Remaining measured differences
include savedmode7 versus6, constant160 in s6 instead of per-arm immediates,
136fp versus s7, page spill60 versusfp, earlyH and temporary register choices.
No H width guess, new capture/logger, toolchain change or production edit.

See inventory.md for current618 opcode counts and the precise remaining
constant160/page-spill question; counts are not a match or scheduling proof.

allocation.md records the current618 priority/interference observation and
mandatory whole-object fidelity. It supplies no supported new source edit.

constant-use-map.md follows the six saved160 geometry reads to original LI
sites; it finds no allocated-register UV sharing or supported source edit.


One parent-authorized H-only unsigned-literal diagnostic follows the allocation
observation, not a suffix/type sweep. Exactly four temp_a0=0xA0-arg0 statements
become0xA0U-arg0. Other lower-right160 literals, s32 shared H/E local, every
signed DIV/product cast, geometry formula/store order and declaration remain
fixed. Inline same-line FAKE comments state priority-partition purpose and
unproven original signedness; no newline/statement-group change. For actual
normal callers A0..80, both signed and unsigned expressions yield80..160,
exactly representable in existing s32, so no overflow or implementation-defined
assignment in this domain. Subsequent word additions/low16 SDK stores agree.

Prediction was typed constant web might partition six geometry uses, or normalize
neutral; no page/frame/register fix was promised. Measured606cost/836rows/
592words/frame90. Poly84/88, UV50/54 and Q11 DIV-before-P MULTU remain. Mode now
s6 and136s7, as original; constant160 instead occupiesfp, still displacing page
which remains memory home60. Final allocated160 register now feeds ONLY four
height SUBUs. Two signed lower-right formulas fold to P-Q11-R-Q68+ADDIU160,
losing original origin160-before-Q11 association. Counts versus618 change only
ADDU41-to39 and ADDIU35-to37, all others unchanged. Thus source typing affects
allocation but does not free page, prove original signedness, or produce match.
No new priority trace was run on606; do not transfer618 priority measurements.

Keep606 as a separate FAKE diagnostic frontier,618 original-order/six-use view,
and604 numeric best independently. No further suffix, width or expression split
was authorized or tested by this packet. Production remains ASM, same function
active. The observed result is not permission for a signedness permutation search.

height-lifetimes.md preserves the three independent lateH predictions and
negative outcomes; no form moved the current earlyH definition.

The second height-lifetimes.md set distinguishes exact-count/saved-pool616
from a635 narrow-boundary negative; neither is a byte match.

The explicit parent-authorized u16-boundary followup is archived in
height-lifetimes.md:615/595words/frame90, four masks survive. Not a match.

counted-loop.md preserves the matched-sibling loop-form test:612/592words/frame90,
geometry arms unchanged from618, no saved160/page repair. Replay counted-for.
