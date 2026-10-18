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
