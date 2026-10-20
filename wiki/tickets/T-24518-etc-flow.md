---
id: T-24518
title: Reconstruct ETC initializer flow and live values
status: In Progress
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[tickets/T-24513-etc-matching]]", "[[playbook]]"]
---

## Goal
Recover literal default C for ETC func_8013FFAC (2116 bytes) through original-flow reconstruction, continuing until a match or a concrete isolated backend blocker.

2026-10-20 coordinator scope extension: retain ETC/BUNKA_SD/EN_NICHI/KANGEI ownership, prioritize closest owned guards and new matched sibling recipes when deep source progress stalls. Exclude exhausted KANGEI two-row scheduling tails. Current3FFAC best remains preserved; evaluate ETC4790C's archived44-row source against the separate stack-slot recipe from MAIN767A4 (read-only uncommitted donor in T-24501, literal bytes reported by coordinator, final checks pending). Target frame188/buffer7C imply12 upper bytes beyond a256-byte buffer; separate words are a FAKE layout hypothesis, not a recovered source fact. No generic padding search.

## Acceptance criteria
- Prove each region's source semantics, loop shape and live values against original instructions.
- No small type grids, production compiler patches, inline assembly, old block-copy direction probes or unsupported backend claims.
- Any promotion requires literal linked-byte proof, all27 hashes, guards and standards review.
- If isolated backend failure remains, retain minimal source/command/reproducer and exhausted source hypotheses for coordinator review.

## Notes
Fresh worktree /private/tmp/tokimemo-t24518, branch codex/t24518-etc-flow, exact base c768eda884eb30dd0067509f90d8892dc2563ef1. Own only ETC func_8013FFAC. T-24513 corrected six-entry extents/stride and current fields/tables; original first address loop remains rolled while prior source expands, second table unroll and buffer0x54 versus0x58 remain. These are questions, not backend proofs.

## Comments
- 2026-10-20: Read fresh wiki index/status/playbook/standards and ETC history including T-24513. Region-first analysis begins; no full27 baseline until a candidate.

### Region 5: address-table flow
- Fresh exact-base baseline reproduces529 target /538 candidate words,453 aligned cost. Pointer-address cursor and genuine pointer-bound comparison are neutral; first constant loop remains expanded.
- Reusing one source induction counter in the first address loop and second index loop retains the original first loop, yielding530 words/449 cost. Independent unsigned address word still expands, proving unsigned type alone is not the cause. Signed shared counter plus real scalar declaration above array gives530/447 and correct buffer0x54. First loop has original ten-instruction shape but address/bound are a0/a1 rather than v0/a0. This is structural source progress, not a backend blocker.
- Reusing an address/shift temporary only in the second loop body does not prevent first-loop expansion(538/447). The distinction is shared induction across loops, not an arbitrary copied expression. No extern-symbol workaround added: linker address materialization would introduce ADDIU where target uses ORI.
- Existing read-only diagnostic /private/tmp/t24047-uopt/variants/trace exists; coordinator authorized reuse with fidelity check. Next diagnostic asks why original-like field and counter ranges occupy different registers, after source loop structure is recovered.

### Scalar state lifetime and remaining regions
- Exact stock/traced fidelity passed twice: shared-signed and field-counter candidates have identical code/data/relocation sections and resolved target words under stock uopt and the existing read-only trace binary. Production compiler remains stock.
- Trace identifies direct GameState member selector as unallocated: 15 uses+17 definitions pay 32 reload/save charges. A coherent `(*(u8 *)&D_800E6280.unk_110D)` view promotes state across unrelated member stores, reducing447 to351 cost,530 candidate versus529 target words. Raw-offset and named-field-address forms are byte-equivalent. This is a measured diagnostic view, not evidence for separate globals and not a match. Any eventual use needs an explicit FAKE explanation under this ticket.
- Masking only registers/branch displacements for localization aligns512/529 target positions. Remaining instruction/order regions: first-loop address materialization; second-loop constant/store scheduling; case5 shared128 constant and zero-store schedule; case8 missing ANDI; case11 duplicate01000000 materialization; final default delay-slot LUI.
- Independent bounded controls: chained flag/shade assignments, signed-byte shade stores and girl-field byte view are neutral351. Coherent fade-byte view worsens to427 despite513 normalized positions. An s32/no-return definition (private copied header only) worsens to355/532 words. Production source/header unchanged; these are discarded controls, not coverage.
- No isolated backend blocker established. Continue original-flow reconstruction and range attribution; no further full27 baseline until a matching candidate.
- Signed-vs-unsigned flag identity is causal: F4 is s32, D0 is u32 in main_api.h. A private signed D0 store in case11 removes duplicateLUI01000000 and reaches529 target/529 candidate words,190 cost (`drafts/T-24518/signed-flag.c`). No shared-header edit. A complete element-store shade chain shares128 but schedules wrong RGB order; individual element stores and real SpriteSlot24 member/pointer views do not recover the original order. Combining s32/no-return liveness fixes final NOP/LUI but adds case9 ANDI; not selected as best.
- Checkpoint self-review uses local code-review skill standards/spec axes without extra agents per coordinator. All private C controls carry FAKE/ticket provenance and are excluded from production. Source/include/tools/config diff against base is empty; no match, linked-byte or all27 claim is made. Scope, existing read-only trace fidelity and Docker-only execution reviewed. Work remains In Progress.
- Coordinator archive review requested less duplication. Keep only baseline and best signed-flag in full; controls.json/replay.py reconstruct every other exact source with SHA256 checks. All24 replay hashes verified; original working sources retained under ignored build/T-24518/working-evidence. README contains compact results/causal questions. Shade copies through first global also neutral529/190, not recompiled further.
- Later bounded3FFAC source work: widening fade snapshot from matched main sibling gives302; direct short-circuit final flow, one-line second table and standalone byte-shade local are byte-neutral190. Reusing table counter for shade or a third RGB loop incorrectly retains the second table loop(516 words), rejecting those source models. Stock/traced fidelity also passes exact marked best signed-flag.
- Closest owned guard review: exhausted KANGEI tails excluded. Current BUNKA39290 history already exhausts return/copy/signed selector hypotheses, so no repeat. Targeted sibling reports over generated ETC/BUNKA_SD assembly find no>=0.70 donor for39290/4790C; partial generated corpus is not a global census.
- ETC4790C current baseline282/279 words85cost, frame188/buffer7C. MAIN767A4 measured separate-slot transfer and matched ETC43758 typed record walk are both byte-neutral. EN36DEC original table offsetsC4/98/6C/54 with top endF0/frameF8 support one two-upper-slot check: frame becomesF8,313 cost versus317. Saved kind cursor and broad loop/store gaps remain. No match or backend claim; sources retained as compact replays.
- New causal shade-order prediction succeeds: normal RGB element chainE0=E1=E2 produces originalE1/E0/GSzero/E2 call-slot order with oneLI128. Replay shade-rgb-chain527words352cost recovers515/529 masked positions, retained beside raw190cost best. Remaining opcode-count gaps are case8 ANDI and case13 flag-load delay NOP/D801500D8 LUI hoist (earlier loose final-default label corrected). Int-no-return fixes hoist but duplicates case9 alpha mask; postfix and word-local controls do not remove duplication, no match.
- EN36DEC exact stock/trace fidelity passes. Kind induction expression bit46 getsv1 priority15.5; indexi getss0 16.33, constant1000 getss1. OriginalkindS0/indexS1/constantS2; this is a source/range question, not an isolated backend proof. Historical80048F64 int view worsens313 to443, postincrementcursor453 confirms old negative. Current34→38 compact replay controls preserve precise source; source/include/tools/config remain unchanged.
- New first-table reconstruction succeeds: both tables as natural shared-index loops let strength reduction create first addressV0/boundA0 independently of second indexA0. Exact resolved words162..171 (function+288..2AC) all identical, verified after FAKE scalar-home annotation. Candidate527/336,516 normalized positions. Current helper names from obin_renames restore actual prototypes; name repair alone is not the loop cause. Independent address scratch reused for shade/alpha still expands firstloop(537/398), rejected.
- Second-table explicit peel from T22019 scene folds away target runtime induction(522/373); unsigned index changes original firstSLT toSLTU(527/400); unsigned second-stride expression is neutral336. No index-width grid follows. True remaining count gaps are case8ANDI and case13flag-load NOP/LUI; second table has scheduling/register differences, not missing opcodes.
- Specific MASTER callee-view transfer: call-site cast emitsJALR, rejected. Direct s32func40AE4 view preserves its matched56words/0cost but not callerhoist (528/341, extra case8NOP). T24207 call-block boundary is neutral352. Global-alpha preincrement under own int/no-return trades duplicatedANDI for unwantedV0copy(529/392). No production prototype edit. Checkpoint standards/spec review confirms all views/home tricks marked and no match claim;48 replay sources remain exact and production src/include/tools/config unchanged.
