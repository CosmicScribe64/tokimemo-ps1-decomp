# T-24078 original value relationships (read-only)

Compare original func_8013D570 with pre-control638-cost shared-temporary body.
A=arg0, i=iteration0/1, P=low32(i*136), H=160-A. Qn denotes signed
DIV80 of low32(A*n); R denotes signed DIV80 of low32(low32(A*i)*68).
Y denotes a FRESH signed GameState.unk_018[D_8011ECA0] load, not a cached value.
Intermediate arithmetic is word-sized; SDK vertex stores retain low16 bits.

| Original full VRAM | Relationship | Retained source |
| --- | --- | --- |
| 8013D654..D700, D9B8..DA70 | Case0/2 X=(P-Q11)-R+24 | temp_v1 same formula |
| 8013D80C..D8C0, DBB8..DC70 | Case1/3 E=(Q141+P)-R; X=E+24 | temp_v1_2, temp_a1 |
| 8013D704..D770, DAC0..DB10 | Case0/2 upper-right=X-Q68+136 | x1 same association |
| 8013D7C4..D7E0, DB5C..DB88 | Case0/2 lower-right=((160-Q11)+P)-R-Q68 | x3 same association; do not collapse to x1 |
| 8013D8C4..D930, DCC4..DD10 | Case1/3 right=E-Q68+160 | temp_a2, x1/x3 |
| 8013D70C..D7FC, D8CC..D9A8 | Case0/1 top=Y-101; bottom=(Y+H)-101 | y0/1 and y2/3 agree |
| 8013DA74..DBAC, DC74..DD98 | Case2/3 top=(Y+Q112)-101; bottom=((Y+Q112)+H)-101 | temp_lo_8; both y associations agree |
| 8013DDA0..DDF8 | LW UV words sp54/sp50, then SB copies to u0/u2 and u1/u3 | inherited s8 casts truncate earlier; low byte agrees |
| 8013DE44..DE6C | After AddPrim, reload UV words, +8; i+1; cursor+4; page+128; stop at2 | independent induction variables agree |

Constants11,141,112 are confirmed by original shift/add/sub sequences; every
quotient uses signed DIV, not DIVU. The original does not replace R with i*Q68,
though that identity holds for i=0/1. Such simplification would erase original
repeated multiply/divide dependencies. No formula/address/missing operation
correction was found in this pass.

True cross-call/iteration values: s1=A, s6=mode, s2=GameState address (not a
cached field), s3=80, s5=68, s7=136, s4=i, s0=&poly[i], fp=320+128*i;
UV words start160/24 and advance8 after AddPrim. No calls occur inside the four
arithmetic arms. Quotients and computed vertex coordinates need survive only
within their arm; persistent constants/induction values survive initialization
calls and AddPrim. Frame index and signed center values are repeatedly reloaded
after polygon stores; do not cache these globals across stores or calls.

One supported structural question, not a proven semantic bug: original v1 is
word-sized atD6FC/D700, stored with SH atD708, then still consumed as a word by
SUB D768 before SH D770. Cases1/3 a1/a2 likewise remain full words from
D8C0/D928/D92C to SH D95C/D98C (and DC70/DD08/DD0C to DD40/DD74).
The draft declares temp_v1/temp_a1/temp_a2 as s16. All later uses are additions,
subtractions and low16 stores, so early narrowing is observationally equivalent
for these outputs; original assembly alone does not prove original C types.
A single word-until-SDK-store control changing ONLY those three locals to s32
would follow the observed register values. Parent subsequently approved exactly this control:604 alignedcost/670rows,
592words/frameA0 versus joined638/594/98. The first arithmetic divergence
remains MULTU-before-DIV, so the width correction does not close the dependency
schedule. No other source choice changed.
This is not authorization for other width/layout permutations or instrumentation.

## Broader matched semantic search and phase limit

Read-only current primary at9d0d6a8e097caa224f5a6b50fea26a7091ac7920, also
compared this worktree. Search literal signed /80 or /0x50, SDK polygon X/Y
assignments with division, widths68/136 and slopes11/141/112; do not use the
existing opcode-sibling0.70 cutoff as semantic evidence.

| Actual matched C read | Genuine relationship | Limit for D570 |
| --- | --- | --- |
| TACO80139170.c func80139BCC | signed scale*0x7000/80, s32 temp/scale | one scalar passed to helper; no quad interpolation/UV iteration |
| TACO8013D1E0.c func8013D528 | SVECTOR projection into s32 pt[4], then SDK halfword vertex stores | projection calls and /127 depth, no corresponding P/H/Q11 dependencies |
| TACO8013F250.c func80140ADC | same word projected-points/halfword output; depth quotient | no equivalent slope or signed/80 repeated-coordinate calculation |
| TAIIKU80138950.c func8013D9B4 | SDK tiled UV, explicit narrow-before-add FAKE | shifts/UV only; not a type or scheduling recipe for this geometry |
| RPG_BAT801525B0.c func80154220 | s32 coordinate/UV input arrays narrow at SDK fields | no interpolation/division and arrays are actual incoming objects |

No additional testable original-backed relationship found. TACO UV macro
expansion line grouping affects its eight byte stores; that does not explain
D570's first arithmetic dependency, so no speculative macro/line-group test.

Current build/etc24078/word-coordinates contains source.c, source.o, dependency,
measurement and final diagnostic only. No stock uopt/ugen input/output was
retained. The observed final sequence is MULTU P before Q11 DIV plus early H;
it does NOT identify which compiler phase moved these operations. Earlier
'uopt hoist' descriptions are hypotheses, not measured phase attribution.
Read-only tools/cc.py and drafts/T-24071/backend_capture.py show an existing
stock-stage copy wrapper and full-object identity check. A target/path adapter
could observe ugen input/output without a modified compiler, but no capture,
new instrumentation, toolchain change or diagnostic compile was performed.


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
