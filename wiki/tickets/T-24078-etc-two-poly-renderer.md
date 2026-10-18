---
id: T-24078
title: Recover ETC renderer with actual two-slot polygon buffer
status: In Progress
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[etc-family-results]]", "[[tickets/T-17300-types-first-etc]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Recover only ETC func_8013D570 (2352 bytes), src/ovl/ETC/8013BEF0.c. Preserve matched D32C/BFFC/C8EC and every other body. Current primary ASM/no active claim or later T-240xx target packet found.

## Acceptance criteria
- Read latest primary wiki/history and original assembly; old plain792/typed957 rows are historical, not a near-zero promise. Fresh valid full-TU SDK baseline first.
- Replace old scalar sp84 buffer with actual POLY_FT4*poly[2]: original frame0x90/s0=sp84, four-byte step and two iterations prove slots84/88. Verify full target addresses before source changes. Use real GameState.unk_018 frame-buffer indexing, not aggregate+frame*2 scaling.
- Matched same-TU D32C/T-22019 supplies real two-polygon SDK initialization/AddPrim idiom. Transfer target semantics one change at a time, not old scalar/register casts.
- Same sole Sol medium Docker compiler/ninja -j1, built checkout fromeff5d1b96 on codex/complete-etc-two-poly-renderer; no extra agents/instrumentation or broad sweeps.
- No assumed shared headers/config changes; coordinate exact evidence first. Actual C needs default literal2352-byte proof, ETC hash/interfaces and unchanged-neighbor audit; parent reviews/all27. Archive replay must work after any promotion.

## Notes
Both retained drafts use a singleton pointer despite two-slot accesses; typed fixed pointer step to one but not buffer extent, plain step4 scales sixteen bytes. Correcting actual layout is new faithful reconstruction, not a compiler workaround. Preserve exact SDK field widths/order and original two-iteration/draw-buffer semantics. T-24077 remains unresolved In Review with probes stopped.

Original full VRAM8013D5BC sets cursor tosp84, 8013DE64 steps4bytes and
8013DE4C/DE68 enforce two iterations, proving slots84/88 in frame90. Fresh
baseline repairs singleton to real POLY_FT4*poly[2], typed pointer step1, all16
GameState.unk_018 frame-index reads and exact two-argument AddPrim byte-ordering
table address. Old plain792/typed957 remain history. No production/header change.

Fresh valid current baseline800/frameC0; joining12 disjoint same-type aliases
preserves expressions/store order and reduces frame to98 with800 rows. Direct
SDK UV assignments remove inherited signed-byte casts, restore word loads and
reach790/frame98. Source-only controls/replay are drafts/T-24078; production,
headers/config and helpers remain unchanged. Original frame90; recovery active,
no match or linked C proof claimed. Initial field-substitution syntax error was
repaired before scoring and is not a baseline result.

Metric correction: printed +/- rows are diagnostic only, with different equal-
length alignment rules. Existing tools/funcdiff.py diff_score measures aligned
resolved-key cost. Original588words/frame90: baseline638cost/800rows/594words/C0;
joined638/800/594/98 is retained; scalar SDK UV678/790/582/98 is WORSE despite
fewer rows. UVpair638/800/596/98 restores LW/saved136 but adds2words; per-case
height638/800/596/A8, inlineheight678/790/582/98, namedwidth648/806/594/A0 are
rejected. drafts/T-24078 replay now prints alignedcost/words/frame evidence.
No supported form simultaneously restores original stackUV50/54, saved136/320
and DIV-before-MULTU scheduling. Active In Progress pending focused direction;
no further unsupported probes, no match/impossibility claim. Production ASM.

Parent-authorized bounded Astra read-only review returned one source-backed
D32C transfer: derive pageX as0x140 + var_s4*0x80 and remove only independent
var_fp declaration/init/increment. Single indexed-page control neutral638cost/
800rows/594words/frame98. Compiler reconstructs320 induction but spills atsp50
rather thanbaseline60; no targetfp320/frame90 gain. drafts/T-24078/indexed-page.c
and replay preserve exact source; measured ignored build/etc24078/indexed-page.
Astra idle, no further unsupported control/instrumentation; still active pending
direction, production unchanged.

Read-only original/source relationship audit: drafts/T-24078/relationships.md
maps all4 arms/shared tail to full VRAM formulas. Signed Q11/Q141/Q68/Q112 and
R=(A*i*68)/80, distinct upper/lower right expressions, repeated signed center
reloads, UV words and all persistent values agree; no missingop/address/formula
correction found. One structural question is full-word coordinate intermediates
until SDK SH versus3 inherited s16 locals; their later low16-only uses make early
narrowing observationally equivalent, NOT a proven semantic bug/original C type.
Proposed single3-local s32 control is source-backed but UNTESTED; no compiler,
new instrumentation, source sweep or production change in this read-only pass.

Exact parent-approved word-until-SDK-store control changes ONLY temp_a1,
temp_a2,temp_v1 froms16 tos32 againstjoined638. New best604 alignedcost/670
printedrows/592words/frameA0 (original588/90), frame8B larger thanjoined98.
First divergent arithmetic still MULTU i*136 before Q11 DIV; original DIV then
MULTU. UV LB and early160-A remain. drafts/T-24078/body.c/replay and full-VA
relationships.md preserve result plus all9 controls. No other width/control or
production edit; active ownership continues. This is structural evidence, not a
proven behavior bug or original C type.

With full-word coordinates fixed, UV-word-pair control606cost/682rows/594words/
A0 restores LW but is worse than604. Case-local arithmetic declarations keep
604/670/592 yet enlarge frame toD0: lexical scopes do not reuse these homes.
Both rejected as best; no declaration-order sweep. Next source-only check is
case0 Q11/P/R alias inlining into exact unchanged X0/X3 associations; all other
arms fixed. Inputs A/i do not mutate within the arm and no calls occur there.

Further one-at-a-time controls against604: original-backed UVwordpair606/682rows/
594words/A0 restores LW but is worse; true case-local arithmetic lifetimes604/
670/592/D0 reject enlarged lexical homes; case0-only compound Q11/P/R aliases
635/703/593/A0 reject, first arithmetic remains MULTU-before-DIV. All12 variants
and exact sources/replay retained drafts/T-24078. Best604 unchanged; source
math agrees, no wrongop or compiler-limitation proof. Active same-function
read-only investigation continues before any supported new compile; no sweep,
new instrumentation, production/helper/header/config change or target churn.

Broader read-only PRIMARY matched semantic search at9d0d6a8e0 found only scalar
TACO39BCC signed/80; matched TACO3D528/40ADC use projected word points and/127
depth, TAIIKU3D9B4 tiledUV shifts, RPG_BAT54220 incoming word arrays. No genuine
68/136 slope11/141/112 repeated-division quad analogue or new supported source
form. relationships.md records search/limits. Existing target evidence has only
final objects, NO compiler-stage inputs: cannot attribute earlyP/H to uopt vs
ugen/as1; previous 'uopt hoist' causal wording is unproven. Existing stock-capture
pattern read only, no new diagnostic scope executed. Same target active.

Stock ugen capture of saved604 source passed SAME-SOURCE WHOLEOBJECT identity
67fbefa6... before interpretation, all code/data/relocation sections equal.
phase.md/capture.py/capture-fidelity.json record exact provenance. P-before-Q11
alreadyugenINPUT128..137. H remainsnearY2 input208..211 andugenOUTPUT127after
x2SH124, so earlyHfinal movement is POST-ugen; prior joint'uopt hoist' claim is
superseded, no broaderphase attribution. Input183..188 also folds X0=B+24 and
X1=X0-Q68+136 intoB-Q68+160. One approved matching-only use-site narrow in X1
cases0/2 follows; no other type/barrier/instrumentation change.

Approved X1-only s16 use-site cast in cases0/2 changes final offset web back to
X0-Q68+136 but costs638/800rows/594words/A0, worse than604. X0storedfroma0 then
MOVEto t5 beforeX1 creates extra copies (2words); no explicit halfword extension.
Matching-only FAKE, notsemanticrepair; source/replay preserved x1-narrow.c.
No additional cast orstagecapture. Next narrow unresolved phase question is
frontend vsoptimizer origin ofP-before-Q11: current capture begins afterthat
boundary. Samefunctionactive, production unchanged.

Second exact stock capture locates P reversal: frontend target818records has
Q11DIV106 thenPMPY111; actualconvertedpre.B target BYTEIDENTICAL. Stockraw.O
762records hasPMPY130 thenQ11DIV136. Postconversion WHOLEFILEidentical toraw.O;
previousugeninput targetbytes identical. Wholeobject67fb... and sections pass
FIRST, unchanged options/default405=1. Thus P-order change is inside stock
optimizer, not frontend or conversion rewrite; internaltransform stillunknown,
no faithful new source dependency identified. phase.md/uopt_capture.py/manifest
preserve exactprovenance. H movement remains separately post-ugen, not this
phase. No further compile/probe/instrumentation or production edit; active.

Read-only existing stockuopt generatedreference: oneproc statically calls copy
propagation, findinduct/codemotion, coloring thenreemit. Firstarmactualgraph has
independentQ11 andP feedingSUB; no valueedgeQ11→P, so statementorder isn'tbarrier.
Earliergraphrewrite vsfinalDAGre-emission remain unproven internalcandidates.
phase.md records referencecalls/dependencytable andsmallestmissingobservation:
firstarmLOC450/label159 statement/DAG order atENTRYreemit. No suchsnapshot,
no supportednewsourceform, no logger/build/options/probe executed. Active.

One approved bounded native reemit observer PASS same604 WHOLEOBJECT67fb...
FIRST, allsections/options unchanged. ENTRY BB11 LOC450/451/452/453 arithmetic
assignments are alreadyNOP96; firstX0store DAG ADD(SUB(SUB(P,Q11),R),24).
Correspondingoutput emitsleftP beforeQ11. Thus priorassignmentelimination plus
operandfirstreemission, notswapofsurvivingassignments; exactearlierpassunknown.
reemit.md/patch/fidelity preserveminimalrecipe/hash/observations; rawbinary/logs
ignored. One faithfulsourceassociation lead isoriginfirst24-Q11+P-R mirroring
lowerX3origin160; no newsourceprobe yet. Samefunctionactive, noextra logger.


Origin-first cases0/2 control restores original Q11-DIV/P-MULTU/R-DIV order,
628cost/856 diagnostic rows/592words/frameA0. Keep604 numeric best and628 causal
frontier separately; neither matches. Actual caller A0..80/i0..1 supports exact
reassociation, no invented dependency. Source/replay and regional comparison
are in drafts/T-24078; cases1/3 unchanged. Remaining X1+160 versus original
X0+136 dependency needs read-only analysis before any further control. Active.


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


Read-only stack-map.md distinguishes original64B local region from current80B:
poly84/88 has original unknown word8C above it;628poly98/9C has none. UV50/54
versus90/94 and currentpage64 are actual accesses. Existing604 frontend declares
72B plus hidden4B switch home; optimizedDEF76 survives, four-byte rounding.
No628 frontend capture exists. First-declared-highest IDO rule explains current
layout, but original unused homes are unidentified; no arbitrary permutation
or padding proposed. Disjoint X0/right-coordinate roles are a possible real
same-type reuse lead, not measured or a complete original-layout explanation.


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
