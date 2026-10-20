---
id: T-24078
title: Recover ETC renderer with actual two-slot polygon buffer
status: Backlog
assignee: Codex
created: 2026-10-18
updated: 2026-10-20
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


Read-only current618 instruction inventory isolates all mnemonic differences:
LI-5,LW+2,SW+2,MOVE-1,NOP+6, totaling four words; every other count agrees,
including14DIV/16MULTU/33SH/11SB. Page spill explains memory/move overhead;
shared saved160 replaces six per-armLI with one, while extraNOPs offset savings.
Exact scheduling/IRcause remains unproven and current618 stagecapture absent.
inventory.md states one narrow constant-web/source-condition question; no H
width guess, literal/type sweep or new instrumentation. Same function active.


Parent bounded Astra CURRENT618 review returned no supported source edit.
One subsequently approved private read-only allocation observer passes exact
stock WHOLEOBJECT000d7483... and all code/data/relocation sections FIRST.
Actual input IR identifies page320 M/block56/-48, not an assumed offset.
Constant160 bit159 priority12 and page bit6 priority6.2 both initially eligible,
with bidirectional interference. Both have colors20..22 before160 chooses20;
page availability loses20 immediately, and all saved colors are forbidden by
its later consideration. Thus current competition, not intrinsic initial page
ineligibility. Other consumers of21/22 unlogged; no source edit or impossibility
claim follows. allocation.md/patch/recipe/fidelity preserve bounded evidence;
generated compiler/raw logs ignored. No production/options/source change, same
function active. Signed/unsigned160 or H width guesses remain untested.


Requested existing-data use map confirms current618 allocateds6 serves exactly
six geometry SUBUs: four H=160-A and two lower-right160-Q11. No UV/page use;
UV initializer separatelyLI t8,160, rightX uses immediateADDIU160. Case2 LIorder
was checked byconsumers (DB3Ct5 islower-right, DB48t4 isH), not assumedorder.
constant-use-map.md preserves fulloriginalVAs/currentrelativeoffsets. Parent
read-only followup again supplied no faithful edit. Ordinary geometry pooling,
no UVcross-purpose cause supported; finaluses are not completeIRprioritycounts.
No new build/instrument/source/signednesssplit. Same function active, unchanged.


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


Bounded lateH set of three independent signed618 controls: deadX0scratch reuse
is target-identical618; firstY2capture neutral618 but changes six ADDU words;
firstHconsumer SDKs16 view650/594words, adds4MOVE/removes2NOP. All retain frame90,
poly84/88, UV50/54, Q/P order, same earlyH v0 and saved160/page mismatch. Semantic
lifetime/domain proofs and before-predictions are in height-lifetimes.md; FAKE
choices explicit, no type recovery claim. No further width/mask/placement sweep;
618/606/604 frontiers separate, production unchanged and same target active.


A second bounded expression/lifetime set follows, no random search or type grid.
Staged-H sets the existing H local160 then compound-subtracts A at its original
post-x2 location. This plausible marked two-stage assignment tests whether a
local initial-value lifetime survives copy propagation. Actual A0..80 semantics
are identical; result TARGET IDENTICAL618/592words/frame90, no new evidence.

Negated-H changes only four H expressions to -(A-160). Actual A0..80 gives
intermediate-160..-80 then result80..160 without overflow. MIPS can encode
subtraction of160 as immediate arithmetic, so prediction was canonicalization
or a different immediate/negation web without saved160. Measured616cost/846rows/
588words/frame90. All saved roles now agree with original: mode6,136s7,pagefp,
no page memory spill; UV50/54 and poly84/88 retained. But final H is early NEGU
v0,A; optimizer distributes160 into Y's final offset, yielding Y-A+59 instead
of original standalone160-A followed byY+H-101. Two lower-right formulas still
fold to ADDIU160. Original word count is not a match: current counts differ
LI-6, ADDIU+2, ADDU-2, NEGU+4, SUBU-4, NOP+6, all other counts equal. Preserve
616 as a separate exact-count/saved-pool diagnostic, not a recovered spelling.

One further typed-boundary control against that NEW616 representation adds only
(s16) around the four negated H definitions, leaving the shared H/E local s32.
This is an exact value identity for H80..160; its explicit FAKE purpose tests
stopping the observed affine distribution, not recovering original type. The
prediction was a boundary might retain H while SDK low16 consumers drop narrow
operations, or leave extra operations. It DOES retain four local LI160/SUBH and
keeps original saved roles, but four SLL/four SRA/four MOVE survive; NOP80-to75.
Measured635cost/769 diagnostic rows/599words/frame90, reject extra instructions.
Rows769 lower than846 is not improvement. No alternate widths/masks/suffix
permutation followed. Original low16 field stores alone do not prove H width.

This isolates a remaining source question: retain a real word-valued H boundary
without either global160 pooling or distributing160 into Y+59, and without
non-original narrow/copy instructions. No faithful solution is established.
Current source forms all remain archived separately;618/606/604 frontiers are
not overwritten, production remains ASM and same function remains active.


The parent-authorized unsigned boundary followup changes ONLY the same four
negated H definitions to `(u16) -(arg0 - 0xA0)`; H/E storage remains s32.
For normal A0..80, the intermediate is -160..-80, negation is80..160 and
conversion to u16 then s32 preserves that exact value. All signed DIVs, other
literals/formulas, SDK widths and store order remain fixed. Inline FAKE notes
state the boundary purpose and unproven original type.

Prediction was an unsigned mask might disappear at the existing low16 stores
while retaining the H expression boundary, or survive/canonicalize. Measured
615 aligned cost /753 diagnostic rows /595 words /frame90. Four local LI160/SUB
pairs remain, with four ANDI0xffff and four extra MOVE instructions. Against
original, opcode counts differ LI-2, ADDIU+2, ADDU-2, ANDI+4, MOVE+4, NOP+1;
all other counts equal. Mode s6,136s7,pagefp, poly84/88 and UV50/54 remain;
Q11-before-P is retained. The mask does NOT vanish, so this fails the intended
no-extra-operation boundary. A one-point lower aligned cost than616 does not
make its seven extra words a source solution. Preserve as a separate negative;
618/606/604 and616 stay unchanged. No production edit or further width grid.


One matched-D32C canonical-for control against signed618 reaches612 aligned
cost/840 rows/592words/frame90. All geometry-arm words and opcode counts agree
with618; only15 prologue/tail initialization and reload scratch words differ.
No160/page allocation repair: mode7/160s6/136fp/page60 remain, UV50/54/poly84/88
and Q/P order preserved. Historical T14500/T17300 forms were do/while only.
Exact semantic proof, prediction and comparison are in drafts/T-24078/counted-loop.md;
replay counted-for, marked unproven loop spelling. No loop permutation grid,
production unchanged and same function remains active.


Signed negative-offset control (C=A-160, eight Y-C consumers) is equivalent on
normal domain but measures630/584words/frame90. It absorbs all H negations into
eight Y-A+59 forms; no original H/ADD boundary recovered. Original saved roles,
UV50/54/poly84/88 and Q/P order preserved. Source/replay height-offset and
height-lifetimes.md retain prediction/opcode deltas; no width/mask grid,
production unchanged, same function active.

## Comments

- closing (2026-10-20): no match; ETC func_8013D570 is still INCLUDE_ASM, evidence in drafts/T-24078/. Backlog.
