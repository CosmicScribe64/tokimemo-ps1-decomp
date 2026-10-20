---
id: T-24522
title: Match remaining RPG_BAT functions
status: In Progress
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[families/rpg-bat-family-results]]"]
---

## Goal

Investigate the six RPG_BAT assembly functions from baseline dba33c4f5 in an isolated worktree. Count only literal default C with linked proof.

## Acceptance criteria

- New hypotheses informed by previous negative results.
- Any matches pass literal linked proof, 27 binary SHA1 checks, header/global/prototype guards and standards review.
- Preserve useful unresolved C separately without claiming coverage.

## Notes

Assigned lane: func_801447E0, func_80144C84, func_8014BB80, func_8014BFE4, func_80154DD8, func_8015528C. Docker only, existing arm64 image, compiler jobs 1. Prior T-17113 and T-24027 negatives read before probes.

## Comments

### 2026-10-20 func_8014BB80 matched

Typed `GameState *gs` reads for both menu-choice switches split their promoted range from the plain `D_800E6280.unk_1093` condition read. Removing the historic unused `pad` lets the real pointer occupy its frame home. The volatile read and both fake comments are removed. Existing file-wide implicit-int callee view is retained; no new override or FAKE is needed.

Current full-unit scorer: inherited volatile baseline 43 literal rows/43 aligned (historic T-24301 22 is an older comparator result); pointer switches 22/22 consisting solely of frame offsets; pointer switches without historic pad 0/0. Whole-aggregate volatile initial switch reached 21/21 but retained an added address instruction; byte bit-fields did not help. No broad compiler grid.

Verification: Docker `ninja -j1 build/ovl/RPG_BAT.ok` passes. Docker `funcdiff.py --resolve --version default --no-build RPG_BAT:func_8014BB80` reports MATCH, linked function bytes equal immutable target, 1124 bytes. Actual default C has no NON_MATCHING guard or INCLUDE_ASM fallback. RPG_BAT SHA1 142d13e317aa7ae6def6a300085c38f645c19db6. 27/27 SHA1 passed using private RPG_BAT plus 26 unchanged read-only root build outputs; this is not a claim of rebuilding all 27 binaries. `check_headers` passes. Whole-repo migration check requires unrelated overlay reference asm: rerun using root generated asm read-only.

Standards self-review: C89 declarations at block head, fixed-width project types and existing GameState aggregate; no asm/header/global edits or added fake idioms, no game data staged. Coordinator independently reviewed source against CODING_STANDARDS.md and requested only indentation repair, applied before proof. Ticket remains In Progress for the remaining five functions.

Final declaration guards passed: headers OK; migrate_globals --check globals OK with unchanged generated reference asm read-only; sync_protos --check-branch 0 new disagreements, 15 known, branch check OK.

### 2026-10-20 func_80144C84 source reconstruction

Current scorer: archived T-17113 baseline252/224 aligned. Write the damage result directly through D_8015EBB0:138/134. Read/write unk_00 through a typed RpgStat pointer:92/88; replacing missing frame home:82/78. Staged D_8015EBB0 multiplication and divisions recover original v1 intermediate ranges:44/44. Multiplication operand spellings close4 rows; multiply-by-one wrappers on the five sum operands close10:30/30. These wrappers and the extra frame word would require FAKE documentation if promoted. No production edit yet.

Remaining30 rows come from third-arm unk_08/EBE0 store/reload scheduling plus branch distances and one earlier store order. Additional signed casts do not change code. Reusing the roll local, pointer accesses for all stats, explicit field writebacks, alternate clamp-local form and threshold/clamp pointer views did not close this; no coverage claimed.

### 2026-10-20 func_801447E0 matched

The same staged-result reconstruction solved the other battle calculator: put the random result directly in D_8015EBB0, then explicitly multiply/divide that global in sequence. This lets uopt promote the original shared v1 range and places the two flag webs in a0/a1. The typed source scores154/148 baseline,146/146 staged local,8/8 staged global. Declare actual locals `k` before `lim` to place the limit spill at0x38:0/0. No fake home, override, or compiler modification.

Docker `ninja -j1 build/ovl/RPG_BAT.ok` passes and default `funcdiff.py --resolve --version default --no-build RPG_BAT:func_801447E0` reports MATCH,924 linked bytes equal immutable target. The literal production C replaces INCLUDE_ASM; it has no fallback/define. Parent source review against CODING_STANDARDS.md found no blockers: C89, real locals, canonical GameState aggregate, no new FAKE/header/asm/game-data changes. Worker self-review agrees. Guard and27-hash evidence follows.

Final second-match checks: check_headers OK; migrate_globals --check globals OK; sync_protos --check-branch0 new/15 known, branch check OK.27/27 SHA1 verified with private RPG_BAT142d13e317aa7ae6def6a300085c38f645c19db6 and26 unchanged root outputs mounted read-only. Source standards self-review and independent coordinator review pass.

### 2026-10-20 bounded remaining-lane outcome

func_80144C84 reaches28/28 after matched sibling's comma-return spelling; typed threshold/clamp/trailing-record pointers and switch/while/else forms did not solve the remaining third-arm reload/store gap. Annotated source-only lead saved at drafts/T-24522/RPG_BAT/func_80144C84/body.c; no production edit. Its frame home, operand wrappers and comma return are explicitly FAKE.

func_8014BFE4 current baseline128/128; meaningful actor-base pointer207/201, addressed-argument form185/185, combined247/247. These are negative results; no production change. Recent T-24027 DD8/5528C drafts and negatives were read; no new hypothesis justified rerunning their broad gaps. Source archive README documents the lane and retains every loose probe under ignored build/T-24522. Two exact matches total2,048 bytes; four remain assembly.

Final archive compilation confirmed28 literal rows /28 aligned after documentation and formatting. Work is In Review for the complete lane archive; both production functions already have independent coordinator source review and complete verification.

### 2026-10-20 archive review and read-only residual analysis

Coordinator reviewed source archive1564d1bf1 against CODING_STANDARDS.md: no blockers. The source-only28-row body explicitly denies production coverage, fake home/wrappers/comma-return are documented, and no instruction listings/game assets or production modifications are included.

Read-only C84 dependency map from saved28-row diagnostics and immutable target assembly:

- First divergence: the EBE0 load at target80144D9C precedes the initial counter writebacks; the source draft moves that identical load later. The typed first-stat pointer plausibly creates a different alias dependency for this load, but that interpretation is unproven.
- Target third threshold80144E34 loads extra weight(a3) and field08(a1), writes fields04/00 back, and tests their sum against roll.
- Target enemy-kind branch80144E80 fills its delay slot with field08's address preparation. On the noncall successor80144EA8 it reloads field08 and EBE0, decrements/stores field08, and schedules extra-minus20 in the field08-sign-branch delay slot.
- Current draft carries a1/a3 forward from the threshold. It speculates extra-minus20 into the enemy-kind branch delay slot and stores field08 on the call path to free a1 for the argument. Thus two target reloads disappear, an extra call-path store appears, and downstream branch distances change.

One precise untested source hypothesis: represent only the third threshold's extra/field08 values as an unescaped two-word local aggregate snapshot, then keep actual clamp mutations through plain globals. This could separate the threshold ranges from mutation ranges while replacing existing scalar homes; it might still be folded away by GVN. Ordinary field-pointer and branch-spelling variants were already tested and are not this hypothesis. No compiler was run for this read-only analysis, and no compiler limitation is claimed.

### 2026-10-20 bounded aggregate-snapshot result

Coordinator explicitly authorized the new two-word snapshot hypothesis and at most a few meaningful variants. Compiled three at j1: threshold-only capture; phase-boundary refreshes for bound and threshold; reversed member/assignment order. Every result34 literal/34 aligned. Diff equals the prior28 rows plus6 limit-spill offsets(0x2C->0x28); no separate threshold snapshot instruction survives. Optimizer folding therefore defeats this particular attempt to separate threshold and mutation ranges. Best source-only body remains28/28, production unchanged. Parent archive review and residual dependency graph remain the authoritative checkpoint; no more compiler probes planned without a new structural idea.

### 2026-10-20 fresh BB80/447E0 donor pass

Reopened In Progress for coordinator-authorized comparison of only the two newly integrated originals against the four remaining originals. All8 ranked opcode-normalized and register-retained ratios, method and source-template inspection are saved in drafts/T-24522/README.md. Best pairs:447E0/C84 .280000; BB80/BFE4 .222222;447E0/5528C .207921; BB80/DD8 .206186. The useful447E0 arithmetic transfer was already applied to C84. Other matched blocks are generic switch dispatch, call setup or HI/LO division spacing and do not carry the donor's successful selector lifetime or result-global semantics. No fresh structural transplant justified compilation; zero compiler runs in this pass. Returned ticket and board to In Review for final coordinator handoff.

### 2026-10-20 continued DD8 structural reconstruction

Read T-24027, T-17106 and T-17114 before testing. Ownership evidence: EC68 is D_EC58.unk10, already used by matched801321D0.c; x/y endpoints are unk04/unk0C of the same RpgRec18. Baseline326/288, canonical duration326/288, typed path pointer replacing one fake home326/288. Replacing another fake home with a shared quotient before x/y negation gives331/293, frame8 bytes larger and quotient separation still unavailable. No artificial reference padding used.

Next approved causal test: explicit shared sound-call label and common switch join, reconstructed from original DD8 first basic blocks. This differs from prior predicate-order reversal: state1/counter gate and state>=2 branch converge on one call block. Target places slti(state,2) in counter-branch delay slot while current compound predicate lifts it earlier. One form only, no predicate grid.

### 2026-10-20 5528C start accumulator and clamp graph

Current baseline302/246; share the quotient as the mutable start accumulator301/245; remove the now-unnecessary precomputed (i-1)*stride local298/242. This is not the earlier broad natural-bounds rewrite: sentinel100, branch order and callback order remain unchanged.

The saved diff exposes a causal structural mismatch: original duplicates signed modulo3 within both upper-bound arms before independent negative-start clamps. Current m2c cross-arm goto block_9 shares the else zero block, and the compiler factors modulo after both bounds/clamps; the hoisted constant3 then receives fp rather than original s5. A new one-form source test removes that cross-arm goto and uses two independent zero assignments, as the target successors show. No new global ownership claimed; work-array view remains explicitly FAKE.

The explicit shared DD8 sound label compiles326/288, identical to canonical baseline. Its separate source branches do not change the optimizer's compound-predicate graph.

5528C independent clamps compile368/268 and still merge modulo after both clamps: the emitted single div/mfhi survives, target has two branch-local divisions. Thus the m2c cross-arm goto alone is not the cause. No compiler-limit claim.

A precise uncompiled narrowing hypothesis was sent to the coordinator: signed remainder modulo3 is always[-2,2], so an s8 conversion of just one arm's remainder is semantically redundant. If this preserves distinct uopt expressions and as1 removes the conversion, it may recover target branch-local divisions; it would be FAKE because the differing arm views lack source evidence. This is separate from prior no-op reference and predicate grids.

### 2026-10-20 modulo expression identity isolated

Single first-arm s8 conversion of the signed remainder restores both branch-local division/remainder blocks and the original saved-register ownership: modulo divisor3 in s5, stride s6, phase pointer s7, progress pointer fp. It is still nonmatching408/246 because real sll24/sra24/move instructions remain after mfhi. The domain[-2,2] proves semantics but the backend does not eliminate conversion. The conditionally approved s16 counterpart also408/246 and retains16-bit extension. This establishes that expression equality permits the factoring; no unreachable-CFG/compiler-limit conclusion.

Coordinator authorized exactly one follow-up: first-arm remainder multiplied by1 versus second-arm plain remainder, using the already demonstrated C84 operand-wrapper idiom. No identity-operation grid. Pair-aggregate alias reconstruction remains a future ownership-backed idea if needed.

### 2026-10-20 5528C branch-local modulo without conversion

Exactly one first-arm `(phaseSum % 3) * 1` wrapper (second arm plain) preserves both target modulo blocks without real conversion instructions. Score274 literal/206 aligned, versus baseline302/246 and shared-start298/242. Compiled code and target are both375 words: callback case-block lengths and post-modulo dispatch addresses agree. Divisor3/stride/phase-pointer/progress-pointer saved-register ownership now matches. The expression-identity hypothesis is confirmed; still no literal match and no production edit. Wrapper is marked FAKE. Remaining initial work-base/progress-value registers, operand choices and color/call scheduling need work.

New concrete sharing hypothesis sent before compilation: reuse the start accumulator for both initial raw progress value and loop quotient, removing temp_v0. Target's initial raw progress is v0 and base v1; current has v1 and t0. Real scalar sharing may free v1 for the common work base. This does not rerun the old direct phase decrement or type grids.

### 2026-10-20 structural evidence checkpoint

Actual p532 first-arm remainder-times-one body preserved at drafts/T-24522/RPG_BAT/func_8015528C/body.c, with meaningful local names and both FAKE sites documented. Exact archived text recompiled274/206. DD8 canonical duration ownership body preserved326/288. Reusing start accumulator for initial raw progress worsens412/392 and is rejected. Full experiment table and causal observations saved in drafts/T-24522/README.md; no production/header edits, no new coverage, no compiler-limit claim. Ticket remains In Progress for continued structural work.

### 2026-10-20 structural archive review and first dependency

Coordinator reviewed d2611ea6e source archive and README: no blockers. C89, source-only metrics/provenance and both new FAKE sites are explicit; no asm/assets or production edits. Do not infer helper-wide adjacency from the two known caller index pairs.

Next uncompiled causal hypothesis: target initial EBAC address in v1 feeds progress and initial correction's phase pointer, then dies; post-join phase initialization reconstructs EBAC in t1. Current common named work base occupies t0 while initial raw progress occupies v1, and a new post-join phase base(t8) hoists into the initial branch delay slot, shortening the pre-loop block by one instruction. Restrict named work base to initial progress/correction ownership and write the post-join phase initializer as a direct global-array view. This tests the observed two base lifetimes without adding a type/adjacency assumption or changing declaration order.

Post-join direct global phase initializer274/206 with a byte-for-byte identical diagnostic to p532: base reconstruction scope alone does not alter initial register owners or pre-loop one-instruction difference. No declaration grid followed.

A new composition was proposed before compilation: transplant the now-validated first-arm remainder identity into the original separate quotient/start scalar source. Target explicitly holds quotient in v0 before deriving start in a0, while merged-start p532 assigns quotient to a0 and shifts end candidate/phase sum. This tests actual distinct value ownership under the new correct modulo graph; the original302 baseline had never included this recipe.

### 2026-10-20 exact stock/tracer first-conflict diagnosis

Transferred the new remainder identity to the original separate quotient/start baseline:447/231, rejected. Used coordinator-provided existing read-only /private/tmp/t24047-uopt/variants/trace, with no compiler rebuild/patch. Before analysis, stock and traced candidate function bytes matched exactly:1500 bytes, SHA2565fc82f9a125b0855aea6790e2c29e1e6d3925e55c34c0c680e202e41c2118a29; resolved instruction/relocation keys also identical. Saved first-divergence report in ignored build/T-24522/trace/first-report.txt.

Initial work base: bit134 priority0.5 (2 uses/0 defs over2units), allocation23 ->t0. Initial-only progress_raw: bit10 priority1.5 (2uses+1def), allocation21 ->v1. It is not shared with the loop quotient; proposed signed/unsigned initial-read split therefore lacks its required causal precondition and was not run.

The related initial v0 owner is local phase_next, bit30 priority3 (2uses+1def), allocated before progress_raw; target decrement instead uses an expression temporary. Proposed one direct-counter source test on the newly corrected modulo graph, deleting named phase_next and updating/read-testing actual *phase. The old T-24027 direct-phase negative used the wrong merged-modulo graph; any new result must be measured without assuming transfer.

### 2026-10-20 named phase temporary identified and removed

Deleting named phase_next and writing/read-testing actual *phase on the corrected modulo graph gives53/53. Initial work-base v1 and raw progress v0 now match; phase decrement is the target's ugen t4/t6 expression. Remaining major loop allocation differs because the merged start variable holds the quotient in a0 whereas target quotient is v0 before start a0. Target callback bodies largely agree apart offsets.

Two authorized orthogonal follow-ups: restoring original separate quotient and lower-product locals359/227, rejected; explicitly spelling initial work+index pointers53/53 with unchanged operand order. New one-form hypothesis sent: reuse existing initial progress_raw(v0) for loop quotient, keep start distinct, leave lower product inline, add no home. This is distinct from rejected reuse of start itself during initialization.

### 2026-10-20 best53 source and fresh MAIN donors

Preserved best5528C53/53 source at body.c, former274/206 checkpoint at modulo-only.c. Fresh stock/tracer exact candidate agreement:1504 bytes SHA256ffb6038d8aa8ab017d1681a1a9960a13bed8ae79a51dee653f146ff7736acf0b, resolved keys identical. First loop mismatch start range priority45 precolored a0 vs target quotient v0; extra move at74 and initial pointer operand reversals remain. Restoring quotient/product locals359/227; pointer spelling53/53 unchanged; initial raw/loop quotient reuse369/225; sole quotient local359/215. No production changes.

Latest coordinator priority is fresh donors. Compared exactly8 MAIN76000/767A4 original pairs against4 remaining RPG_BAT targets; ranked metrics and source-template findings in drafts/T-24522/README.md. Best767A4/DD8 .265918 is generic division-trap code; no fresh shared structure. MAIN76000's packed icon/selector source was inspected and does not fit the current target blockers.767A4 source still ASM in root when checked; source comparison waits for its pin. No donor-driven compiler variants.

### 2026-10-20 repeated quotient expression closes loop ownership

Exactly repeated *progress/10 expressions (no named quotient or assignment) gives64 literal/32 aligned with exact375-word length. Quotient now v0, selector v1, and every instruction after row106 agrees, including all callback bodies/loop latch/tail. This is a stronger source lead than raw-row53: it closes the major range split and restores callback addresses.

Remaining: two initial pointer-operand swaps; phase_sum a1/end_candidate a2 reversed from target a2/a1; associated ugen phase load/quotient/product temporaries and assignment schedule. Proposed one independent computation reorder: end_candidate formula before phase_sum load, matching target range order and ugen quotient/product/phase sequence. No declaration permutation or register grid.

### 2026-10-20 helper reaches two operand swaps

End-candidate computation before phase-sum load produces46 literal/10 aligned. Computing each arm's color remainder before end assignment and sentinel100 then produces4/4, exact375 words. Target and current differ only in the order of the two source operands of initial progress and correction-arm phase pointer additions: target base first, current scaled index first. All other373 instructions agree, including every callback and loop tail. Best source is preserved at drafts/T-24522/RPG_BAT/func_8015528C/body.c; prior53-row ownership checkpoint is ownership-53.c. These remain source-only, with no coverage claim.

The approved first-arm arg6-times-one/arg7-times-one index wrapper, leaving postjoin phase indexing plain, regresses230 literal/210 aligned. First divergence changes initial base v1 to t0, raw progress v0 to v1 and saved pointer ownership; desired operand order remains unresolved. No generic identity grid followed. Proposed a byte-addressed base-plus-byte-offset form as a distinct source-addressing hypothesis; not compiled pending coordinator assessment.

Coordinator approved the two bounded addressing tests. Byte-offset indexing through u8* gives230/210, changing initial base/raw/saved pointer owners as the index wrapper did. Scalar32-bit integer-address addition gives373/229. Neither preserves target owners or fixes the literal residual. No more identity/address forms were run; best4 is preserved for a targeted coordinator consult. This is a precise unresolved source normalization issue, not a demonstrated toolchain limit.

Preserved best4 text recompiled from its committed-destination path:4 literal/4 aligned. Documentation checker has0 strong findings; weak colon/long-sentence suggestions were inspected as technical metric descriptions. No production edits or unverified-match claims.

### 2026-10-20 final donor source peek and integer-address roles

Coordinator reviewed430699874 source archive: no blockers. Actual MAIN767A4 source is now present in integration. Its split stack padding separates an ID list and three text buffers; none of the four remaining RPG_BAT blockers has corresponding buffer/frame ownership. Its modulo3 and division traps are generic. No original-pair rescan or donor compiler variant followed.

Read-only role normalization of p545 reveals a narrower positive hidden by its poor score: both initial integer-address additions are base-first, as target. Initial progress uses base t0/index t7, correction phase uses base t0/index t4; postjoin plain pointer addition remains index-first. Initial phase is now v0, postjoin phase s8, while the best4 keeps phase s7 in both. This is emitted ownership evidence, not a traced web proof: existing trace covers best53, not p545. A uniform integer-address spelling at all three sites might restore shared ownership while preserving the desired initial operand order; uncompiled pending coordinator decision. Byte-offset/index wrappers did not show this operand-role success.

### 2026-10-20 uniform scalar address ownership

Coordinator authorized one uniform scalar-address expression class at all three sites, with initial base+scaled-index and postjoin scaled-index+base, matching target operand roles. Result41 literal/39 aligned,376 words versus375. All target register owners and all three pointer-add operand roles now agree. Remaining differences are prolog/epilog save/restore order, initial progress store before phase-address setup, and one extra epilog nop; loop/callback arithmetic has no residual. This supports the mixed-expression-class ownership hypothesis at emitted level, without a traced uopt claim. Preserved separately as scalar-address-41.c with FAKE explanation; best literal4/4 remains body.c. No further addressing forms were run; no production promotion.
