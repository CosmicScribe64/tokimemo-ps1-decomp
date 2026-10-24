---
type: log
---

# Log


Append-only. Entry format: `## [YYYY-MM-DD] <op> | <title>` where op is ingest, query, lint, ticket, build, decision, or setup. Use [[wikilinks]] for wiki files and plain paths (e.g. `src/main.c`) for everything else. Recent: `grep "^## \[" wiki/log.md | tail -5`.

## [2026-10-22] ticket | T-25036 MAIN counter late-fold investigation

Created [[tickets/T-25036-main-counter-late-fold]] in an isolated worktree at 242c0c3a3. Reused [[tickets/T-25026-main-d7ec-near-candidate]] phase evidence and prior negatives; next test is a late-folded zero producer that preserves the original increment dependency without extra instructions.

## [2026-10-22] build | T-25036 identifies second constant-fold barrier

Whole-TU Docker donor transfer retains the counter addition through uopt and ugen but stock as1's initial-node expansion folds it to one. Final output remains 1,173 words with one differing instruction. [[tickets/T-25036-main-counter-late-fold]] records the phase evidence. Read-only donor and tracer reused; production sources and compiler unchanged.

## [2026-10-22] build | T-25036 closes MAIN 4692-byte counter dependency

Promoted `src/main/8006CB30.c` function `func_8006D7EC` using the T-25033 late-zero producer and a late-eliminated identity arm. Whole-TU guarded and default C each pass literal 4,692-byte linked proof and complete MAIN SHA1; header/global/prototype checks pass. Both source controls are FAKE-annotated and add no emitted operations. Root standards/spec review has no findings. [[tickets/T-25036-main-counter-late-fold]] moves to In Review for full integration verification; `drafts/T-25036/` preserves reproduction and phase evidence.

## [2026-10-22] build | MAIN 4692-byte match accepted

Root integrated abc02da59 and independently proved default-C func_8006D7EC literal4692bytes. Fresh27/27binary checksums, headers/globals and prototypes0new/15known pass. Standards/spec review has no findings; marked late-zero and unreachable identity controls introduce no observable effects or compiler exceptions. T-25036 moves Done. Progress6836/6952functions,2110228/2276920bytes (92.7%);116functions/166692bytes remain. Updated [[status]]. Worker continues T-25040 on same-file func_8006D138,872bytes, selected against larger7132C's broad unresolved structure. BUNKA candidate not yet counted.

## [2026-10-22] ticket | T-25037 BUNKA_SD upstream emission

Claimed [[tickets/T-25037-bunka-upstream-emission]] in an isolated worktree from 242c0c3a3. Read the T-25020/T-25023 evidence and retained the existing four-position guarded source. New work focuses on uopt/ugen emission before further source experiments; no coverage added.

## [2026-10-22] build | T-25037 optimized-ucode evidence

[[tickets/T-25037-bunka-upstream-emission]] reproduced the four-position guarded C difference and captured the actual uopt-output/ugen-input boundary. The first three targeted comparison probes do not separate constant and memory preloads: early year still precedes constant, and the 0x60 comparison leaves two extra instructions. Best source remains unchanged; no match claimed.

## [2026-10-22] build | T-25037 closes BUNKA_SD case0x49 ordering

[[tickets/T-25037-bunka-upstream-emission]] found a source form that independently stages the constant and year preloads. An empty frame==0x60 test followed by an unsigned-false year test removes the case0x49 residue without changing any other instruction:379 words, remaining cost2 at positions171/172 in case0x66. Source stays guarded; no coverage credit until the remaining pair matches.

## [2026-10-22] build | T-25037 literal BUNKA_SD match

[[tickets/T-25037-bunka-upstream-emission]] promotes func_80139290 after separate guarded and default-C private links prove all1516 bytes. Rebuilt all11 BUNKA_SD C units and normal link/hash commands pass BUNKA_SD.bin SHA1; declaration/global/prototype guards pass with0 new disagreements. Root standards/spec review has no findings. Ticket moves to In Review pending root full27 integration. Negative source ledger is preserved in drafts/T-25037/README.md; no compiler or header changes.

## [2026-10-22] lint | T-25037 integration checkpoint

Wiki lint reports0 broken links and0 status mismatches; two large pages are data/t0018-cases and the append-only log. The draft README prose check has no strong hits; its one weak colon introduces the explicit hash result and is retained. git diff --check passes. [[tickets/T-25037-bunka-upstream-emission]] remains In Review for integration.

## [2026-10-22] build | BUNKA_SD scheduling match accepted

Integrated5e132362 and independently verified default-C1516literal bytes plus fresh27/27binary checksums. Headers/globals and prototypes0new/15known pass. Root standards/spec review no findings; empty tests are marked FAKE and compile away without observable effects. T-25037 moves Done. Docker progress6837/6952functions,2111744/2276920bytes (92.7%);115functions/165176bytes remain. Four resumed matches add7068bytes. Worker continues T-25041 KANGEI8013852C in disjoint80137B90.c using current ledger and new preload mechanism; other four lanes continue. Updated [[status]] and catalog.

## [2026-10-22] decision | Resume quadrant matching after newer instruction

Root relayed newer direct human instruction to keep workers going until problems are resolved and matches made, superseding prior wrap-up. Reactivated existing Astra worker for [[tickets/T-25034-main-quadrant-extent]] from d8e482c4a, integrated by root as ff8a1c912. Same exclusive function, sole source writer, Docker only, no duplicate coordinator experiments or arbitrary stopping caps. Existing conservative361/381 and forward-join360/380 checkpoints retain full-word extent repair; no match yet.

## [2026-10-22] build | Quadrant color result placement control

Resumed Astra controls for [[tickets/T-25034-main-quadrant-extent]] show mask-before-shift moves the extra copy with expression order; explicit unsigned-byte consumers leave the best object unchanged. This narrows the remaining color issue to expression/result placement, not extraction arithmetic. Worker continues required primitive-field assignment propagation; no duplicate coordinator experiments, promotion or match.

## [2026-10-22] build | Quadrant allocated mask still copies

Astra trace for [[tickets/T-25034-main-quadrant-extent]] assigns the unsigned AND node itself to fp (priority7.625, uses5/defs2), but normal emission still routes the outer result through a temporary and copies back. Field assignment/grouping/remainder controls did not remove it. This rejects the simple outer-conversion allocation hypothesis and motivates output-ucode/lowering inspection. Production unchanged; source and negative evidence remain private pending a meaningful checkpoint.

## [2026-10-22] build | Quadrant copy precedes assembly scheduling

For [[tickets/T-25034-main-quadrant-extent]], Astra captured normal ugen inputs with and without CVT: both raw binasm streams already contain the extra masked-color copy, excluding as1 as its origin. Capture and normal object .text/all other sections match except .mdebug path metadata; resolved function identical. Destination forwarding into the shift child is next. This is diagnostic evidence, not justification for a compiler pass or a literal match.

## [2026-10-22] build | Quadrant destination-forwarding mechanism confirmed

For [[tickets/T-25034-main-quadrant-extent]], normal backend forwards fp into a single-use shift child, then rejects in-place AND and allocates/copies a temporary. Sharing the raw shift through one required color store suppresses forwarding and removes the copy in real C, confirming the mechanism. The control worsens overall output (668words/850aligned/862printed) through a0 allocation and remains diagnostic only. Astra continues source mechanisms; no backend changes or promotion.

## [2026-10-22] ingest | Preserve active quadrant lowering evidence

Root preserved587f8d5c1, documentation only: faithful capture excludes as1, required-store source control removes the extra color copy but worsens global allocation. No compiler patch or coverage claim. T-25034 continues. T-25038 SHOUGATU E6E4 reports zero resolved differences under literal verification; root authorized a narrow existing-pattern declaration override with all default declarations unchanged for other translation units. No experimental result counted.

## [2026-10-22] ticket | T-25041 KANGEI scheduling follow-up

Claimed [[tickets/T-25041-kangei-preload-schedule]] after pinning BUNKA_SD. Read T-24202/T-24901/T-25018 history; KANGEI's local readiness gap requires distinct dependency evidence. Sole source ownership is src/ovl/KANGEI/80137B90.c; no source experiments yet.

## [2026-10-22] build | T-25041 KANGEI source scheduling breakthrough

[[tickets/T-25041-kangei-preload-schedule]] captures a new empty stored-field test that changes uopt from direct str to promoted value plus later rstr. The final sum now precedes store-address lui despite unchanged besttime values. Removing the redundant signed conversion of the byte year restores t6 and gives45-word cost0. Literal proof follows; earlier readiness-only reasoning is corrected in the new ticket, with old logs preserved.

## [2026-10-22] build | T-25041 KANGEI literal C match

[[tickets/T-25041-kangei-preload-schedule]] promotes func_8013852C with separate guarded/default-C private links proving180 bytes. All8 current KANGEI C units rebuilt; normal overlay link/hash commands pass SHA1. Header/global/prototype guards pass with0 new disagreements. Root final-source review has no findings. Ticket and board move to In Review for root full27 integration; no compiler or header changes.

## [2026-10-22] lint | T-25041 reviewed integration pin

Wiki lint reports0 broken links and0 status mismatches; existing large data/log pages remain. The draft README prose checker reports no hits; git diff --check passes. [[tickets/T-25041-kangei-preload-schedule]] is ready for root integration with its source and complete experiment ledger.

## [2026-10-22] ticket | T-25039 MAIN club carrier

Created [[tickets/T-25039-main-club-carrier]] after GYOZI integration. Reused isolated checkout, fast-forwarded to 77eae26cf with prior private evidence retained; claimed only src/main/8005A0B0.c. Read T-25004/T-25008 controls and allocation history. Reproduce the baseline before testing distinct lifetime/phase mechanisms.

## [2026-10-22] build | T-25039 club precolor boundary

Docker full-TU baseline for [[tickets/T-25039-main-club-carrier]] is 215/215 words, three changed positions. Captured allocation confirms distinct shift-expression v1 and local a0 ranges; later coalescing produces the wrong a0. Repeated shift expression with a late-folded zero preserves the target v1 and argument move, leaving the halfword a0-versus-a1 carrier. Corrected actual function size to 860 bytes; no match or production edit yet.

## [2026-10-22] build | T-25039 MAIN club carrier matched

Moved the local club definition into its consuming else branch, before the existing empty halfword test, while comparing the raw shift beforehand. This closes both carriers without new late-zero arithmetic or ignored arguments. Default C matches all 860 linked bytes, MAIN SHA1 passes, and headers/global/prototype guards pass (zero new, 15 known). Root independently reviewed standards and scope with no findings. Recorded controls in drafts/T-25039/README.md and [[matching-notes]]; moved [[tickets/T-25039-main-club-carrier]] to In Review pending root integration.

## [2026-10-22] build | MAIN club and KANGEI matches accepted

Integrated cb6cc430b and c0249b80c. Root default-C literal860/180byte proofs, fresh27/27binary checksums, headers/globals and prototypes0new/15known pass. Independent standards/spec reviews no findings. T-25039/T-25041 move Done. Progress6839/6952functions,2112784/2276920bytes (92.8%);113functions/164136bytes remain. Six matches add8108bytes. User requested more agents: MAIN chat tasked to add two isolated Astra lanes T-25043 TACO8014AB50 and T-25044 parameter_up_down, keeping quadrant worker. T-25039 file now complete; its worker continues T-25045 ETC80146D4C. KANGEI worker continues T-25042. Root serializes integration.

## [2026-10-22] ticket | T-25038 selects SHOUGATU E6E4

Created [[tickets/T-25038-shougatu-selection-loop]] and its In Progress board card before experiments. The clean isolated tree fast-forwarded from d091557e4 to root integration77eae26cf, preserving prior work. Root reports E50C and GYOZI E0C Done with all27 hashes and860 literal bytes. Selected E6E4's preserved corrected187-edit body over EA98's284-edit corrected port; existing table-home fix and negative ledger are in drafts/T-24523/SHOUGATU/func_8013E6E4 and [[tickets/T-24523-seasonal-source-hypotheses]]. Same Astra worker continues sole ownership of src/ovl/SHOUGATU/8013E330.c through resolution; root retains integration ownership.

## [2026-10-22] build | T-25038 selector access classes close resolved E6E4

[[tickets/T-25038-shougatu-selection-loop]]: fresh canonical full-TU baseline187/187 has exact frame/tables and no saved56 residue. Integer debug return gives177; independent club6 byte read restores its reload; a byte-view status argument then removes the unwanted selector promotion and gives0/0,237/237 words. Literal/default-C proof and minimization remain pending. Private evidence in `build/t25038/`, concise ledger in `drafts/T-25038/README.md`. Coordinator's declaration audit found existing s32 D_80122D48 uses in main/DATE and an overlapping EVENT table view; shared-header ownership will be reconciled before promotion.

## [2026-10-22] build | T-25038 default C verified and ready for integration

Promoted SHOUGATU func_8013E6E4 with no fallback: private and production full-TU links match all 948 bytes, and an independent link of the actual Ninja object matches E6E4 plus preserved E50C472bytes. SHOUGATU SHA1 and headers/globals/prototype guards pass (0 new / 15 known). Removal controls document the integer debug return, two byte views, result narrowing and multiplication by one. Root approved the narrow debug guard, overlay forward prototype and existing T-11590 scalar-local exception; coordinator source review has no findings. Moved [[tickets/T-25038-shougatu-selection-loop]] and card to In Review pending root combined gate. Updated [[playbook]] and drafts/T-25038/README.md; raw evidence remains private.

## [2026-10-22] build | SHOUGATU selection loop accepted

Root reviewed ca9569534 source/header against standards/spec, no findings. Narrow debug-return guard leaves default declaration unchanged; existing scalar-local exception documented, overlay forward prototype consistent. Full520-step rebuild passes fresh27/27SHA. Default-C E6E4 literal948bytes and preserved E50C472bytes pass. Headers/globals/prototypes0new/15known pass. T-25038 moves Done. Docker progress6840/6952functions,2113732/2276920bytes (92.8%);112functions/163188bytes remain. Seven matches add9056bytes. Worker continues T-25046 EA98; TACO zero-diff candidate not yet counted. All seven lanes remain assigned.

## [2026-10-22] ticket | T-25043 TACO constant tail claimed

Created [[tickets/T-25043-taco-constant-tail]] In Progress in isolated managed worktree. Read [[index]], [[status]], [[playbook]], coding standards and prior [[tickets/T-24502-taco-completion]] evidence before matching.

## [2026-10-22] query | T-25043 baseline and late-zero interference controls

[[tickets/T-25043-taco-constant-tail]] reproduces the seven-word TACO constant-one allocation tail in the full translation unit. New controls based on late-zero comparisons do not close it: argument lifetime and loop-index priority change instead. Source remains unchanged; private replay is `build/t25043/` in the isolated tree. Prior T-25016 constant spellings and302 engine trials are excluded.

## [2026-10-22] build | T-25043 literal TACO stage handler match

`src/ovl/TACO/80149F90.c` now provides default C for func_8014AB50:1876 literal linked bytes and complete TACO SHA1 pass in Docker. A marked late-eliminated unsigned test keeps an existing helper result live across flag stores; allocator trace confirms constant-one moves to original v1. [[matching-notes]] and [[tickets/T-25043-taco-constant-tail]] preserve causal controls, semantic initialization check and failed candidates. Header/global checks pass; independent review pending.

## [2026-10-22] ticket | T-25043 enters In Review

Moved [[tickets/T-25043-taco-constant-tail]] and its [[kanban]] card to In Review after literal default-C and affected-unit proof. Coordinator owns independent standards/spec review and full27 integration.

## [2026-10-22] query | T-25043 independent review passes

Root reviewed [[tickets/T-25043-taco-constant-tail]] against coding standards and scope with no findings. Exact final source repeats literal default-C MATCH1876 bytes; prototype check reports0 new/15 known disagreements. Ticket stays In Review for root's full27 integration gate.

## [2026-10-22] build | TACO stage handler accepted

Integrated dfb51c5cb. Root independent default-C literal1876byte proof, fresh27/27 binary checksums, headers/globals and prototypes0new/15known pass. Standards/spec review has no findings; [[tickets/T-25043-taco-constant-tail]] moves Done. [[status]] now6841/6952functions,2115608/2276920bytes (92.9%),111functions/161312bytes remain. Eight matches add10932bytes. TACO worker continues under reserved T-25047; six other Astra workers remain active. GitHub check found no open PRs and unchanged contributor claim issue65; GEKO/OPTION remain reserved.

## [2026-10-22] build | Quadrant shared flag/color value hoists

For [[tickets/T-25034-main-quadrant-extent]], the required TILE-flag/red-mask shared high-word control hoists before the quad loop and adds a stack home (672words/810aligned/816printed), so it is rejected. Best source remains unchanged; Astra continues UV/control lifetimes. At newer root-relayed human request, coordinator also spawned independent Astra/high T-25043 TACO func_8014AB50 and T-25044 MAIN parameter_up_down workers in separate managed worktrees; no source ownership overlaps or root edits.

## [2026-10-22] build | Quadrant UV saved-register rotation explained

Astra traced separate-case-break controls for [[tickets/T-25034-main-quadrant-extent]]: scaled-row priority rises8.2->9.111 and overtakes constant127/quadrant, rotating saved registers. If-chain reproduces rotation; standalone shared-u0/default bypass returns to existing360/380 best. UV byte-result cast leaves end_v in v1. Matching continues with required masking/store-chain mechanisms; production unchanged.

## [2026-10-22] build | Quadrant signed-byte mask closes red placement

For [[tickets/T-25034-main-quadrant-extent]], signed-byte cast before mask with s32 red reproduces temporary SRA followed by direct ANDI into fp/s7 without sign-extension/copy. Whole-TU candidate674/667words,367aligned/415printed is not overall best; blue narrowing introduces a stack LB. Astra continues that local correction and trace. No literal match or promotion yet.

## [2026-10-22] build | Quadrant signed-mask source improves to212

[[tickets/T-25034-main-quadrant-extent]] improves from360/380 to212aligned/212printed at674target/666built padded words after removing only blue narrowing. Red/green direct masks, blue word extraction, full-word extents and0xA8 frame preserved. Archived immutable source as drafts/T-25034/main/func_80046AC8/signed-mask.c; inner signed CVTL is the measured color expression boundary. Astra continues remaining UV/register/CFG work; no promotion.

## [2026-10-22] build | Quadrant archived212 candidate replayed

Independent Docker replay of drafts/T-25034/main/func_80046AC8/signed-mask.c reproduces674target/666built,212aligned/212printed for [[tickets/T-25034-main-quadrant-extent]]. This validates the archived source/replay path without changing production. Worker continues remaining matching work.

## [2026-10-22] query | Transfer TACO allocation evidence

Added the verified T-25043 call-result liveness recipe to [[playbook]] and relayed it to the MAIN counter worker. The extra dead-arm constant use changes priority as well as interference; avoid treating this as a generic zero-expression recipe. All existing workers remain active with separate source ownership.

## [2026-10-22] build | Root preserves quadrant research checkpoint

Root independently replayed b25963c45 signed-mask candidate in Docker:674 target words,666 candidate words,212 aligned/printed differences, reproducing the worker result. Preserved source and research on main without promoting production or changing coverage. Worker has reported a newer198-difference candidate and continues; it is not yet independently verified.

## [2026-10-22] ticket | T-25044 parameter_up_down claimed

Created [[tickets/T-25044-parameter-up-down]] In Progress in an isolated worktree. Read the earlier eleven-row residual and failed source-control ledger before any matching experiments. No root source or shared declarations changed.

## [2026-10-22] build | T-25044 baseline lifetime trace

Docker full-translation-unit trace confirms the two existing [[tickets/T-25044-parameter-up-down]] residuals. Signed-byte expression and unsigned-carrier controls do not remove the head copies; unsigned carrier additionally reverses products. Private trace and exact source controls remain under build/T-25044; production guard retained.

## [2026-10-22] build | T-25044 mode head resolved diagnostically

A raw-byte lifetime use eliminates the extra nibble copies in [[tickets/T-25044-parameter-up-down]]. The first mismatch now starts at the club row-address result; removing that promoted expression would also restore the downstream temporary numbering. This is a diagnostic improvement, not a function match.

## [2026-10-22] build | T-25044 four-access stack residue

[[tickets/T-25044-parameter-up-down]] now has a complete 307-word source candidate differing only at four saved-row offsets. Reserved padding fixes those offsets but changes the frame, so no candidate is promoted. Stack/object controls are recorded privately and rejected with reasons.

## [2026-10-22] build | T-25044 default C matches parameter_up_down

[[tickets/T-25044-parameter-up-down]] matches all 1228 linked bytes with default C. Direct signed element expressions remove the extra scalar while preserving the separate index lifetime. Full MAIN SHA1 e823bd844a8f8fa4d05483b59c66bc54b8393b26 and header/global/prototype guards pass. Updated [[matching-notes]]; source-only controls retained in drafts/T-25044.

## [2026-10-22] ticket | T-25044 ready for integration review

Moved [[tickets/T-25044-parameter-up-down]] and its [[kanban]] card to In Review. Coordinator semantic review found no issue; its stale comment finding is resolved. Independent full27 integration remains with root.

## [2026-10-22] lint | T-25044 evidence replay validated

Docker source-only replay confirms zero diagnostic lines for current C, eight for four home substitutions, four for the padded frame pair, and 245 after removing the raw-byte lifetime use. Wiki lint reports zero broken links and zero status mismatches, with existing size warnings for [[data/t0018-cases]] and [[log]]. Root independent standards/spec review passed without findings. Source pin: 62429d6d9.

## [2026-10-22] build | MAIN parameter match accepted

Integrated e6da7a621. Root independently reviewed final source against standards/spec, no findings; default-C parameter_up_down matches1228 literal bytes and fresh27/27 binary checksums pass. Headers/globals and prototypes0new/15known pass. [[tickets/T-25044-parameter-up-down]] moves Done. [[status]] now6842/6952functions,2116836/2276920bytes (93.0%);110functions/160084bytes remain. Nine matches add12160bytes. Worker continues under T-25048, reserved with coordinator; TACO T-25047 now owns801511C0. Other five Astra lanes remain active.

## [2026-10-22] ticket | T-25040 MAIN flag dispatcher

Continued authorized Astra work with [[tickets/T-25040-main-flag-dispatch]], choosing `func_8006D138` from the two assigned same-file targets. Latest ledger is 219 candidate / 218 target instructions with a known flag snapshot/base lifetime issue; the alternate dispatcher remains outside scope. T-25036's committed match is preserved.

## [2026-10-22] build | T-25040 reproduces flag-dispatch structure

Whole-TU replay recovers a 219/218-word shape; bit-field snapshot restores target temporary numbering. A new late-zero pointer identity preserves relative flag loads but splits pointer ranges and adds instructions. [[tickets/T-25040-main-flag-dispatch]] records the distinction; production stays assembly while allocator evidence guides the next change.

## [2026-10-22] build | T-25040 exact dispatcher shape and allocator cause

[[tickets/T-25040-main-flag-dispatch]] recovered 218/218 words in the isolated whole-TU harness. The empty unsigned entry switch activates the existing copy boundary; allocator tracing identifies constrained year/base ranges preceding the unconstrained flags snapshot. Late-zero store indices fix the register cycle but introduce alias reloads; absolute spellings and volatile-store controls do not close it. Production source remains assembly. Replays are in `drafts/T-25040/probe.py`.

## [2026-10-22] build | T-25040 three-instruction checkpoint

[[tickets/T-25040-main-flag-dispatch]] isolates the register cycle to a single unprofitable constant range at the leaf constraint threshold. A defined late-zero identity in the empty year98 check restores all target registers; the default whole-TU candidate matches215 of218 words, with only the first b7 load address and adjacent scheduling residue remaining. `drafts/T-25040/` preserves the source and causal controls; no production promotion or coverage claim.

## [2026-10-22] build | Root verifies MAIN dispatcher checkpoint

Independently replayed [[tickets/T-25040-main-flag-dispatch]] candidate on current main:218/218 words, only rows72–74 differ. Saved checkpoint1da87a36b plus causal evidence; no coverage increase. All target register allocation outside the byte-load address sequence now agrees. Worker continues while root preserves the result.

## [2026-10-22] ticket | Root takes remaining MAIN event selector

Created [[tickets/T-25049-main-event-table-recovery]] before reconstruction. Root owns func_80073CB0 in src/main/800737A0.c after parameter match integration; T-25048 owns separate80075320.c. Recover existing draft and table evidence while seven Astra workers continue. Contributor check: no open PRs; issue65 unchanged.

## [2026-10-22] build | Event selector typed baseline recovered

[[tickets/T-25049-main-event-table-recovery]] now has a Docker compiling322-word whole-TU candidate with148 aligned differences. Corrected C0/D0 byte table views and second fallback's wrong GameState field from assembly evidence. Saved source and replay under drafts/T-25049; no promotion or header changes. Diagnostic cost did not change for the field correction, underscoring need for semantic review and literal proof. Remaining differences begin with allocation and selector promotion.

## [2026-10-22] build | Event selector zero-difference candidate

[[tickets/T-25049-main-event-table-recovery]] now322/322words,cost0 after removing artificial shared-store branches, inlining selector tests and retaining byte liveness in a marked empty test. Source/header promotion is local pending full binary gate and independent coordinator review. Cleanup controls retain only the necessary saved selection local, annotated FAKE. Replays updated to replace the function after production promotion.

## [2026-10-22] build | MAIN event selector accepted

[[tickets/T-25049-main-event-table-recovery]] moves Done after independent coordinator standards/spec review and root literal1288byte proof. Full520-step Docker rebuild passes27/27 hashes, headers/globals and prototypes0new/15known. All saved controls reproduced after promotion. src/main/800737A0.c is fully matched. [[status]] now6843/6952functions,2118124/2276920bytes93.0%;109functions/158796bytes remain. Ten matches add13448bytes this effort; seven Astra lanes continue.

## [2026-10-22] ticket | T-25048 adjacent MAIN table selector

Claimed [[tickets/T-25048-main-80076fdc]] and src/main/80075320.c exclusively. The same-file 80073CB0 requires missing shared table declarations; chose 80076FDC with existing canonical tables under the frozen-header constraint. Read current and historical source/ledger before controls.

## [2026-10-22] build | T-25048 global background carrier

The two-local reconstruction in [[tickets/T-25048-main-80076fdc]] matches the complete instruction stream except the column save/reload stack home. Using the existing global as the carrier fixes the historical constant-store order and all switch-arm colors together. Local-background controls are preserved privately; final stack placement and necessity of the empty use are being checked.

## [2026-10-22] build | T-25048 plain C reaches zero diagnostic differences

[[tickets/T-25048-main-80076fdc]] declaration order restores the exact column home. Removing the artificial empty use keeps zero resolved differences. The final candidate is plain C with two real locals and direct global/table accesses. Literal linking is required to verify both jump tables before promotion.

## [2026-10-22] build | T-25048 default C matches852 bytes

[[tickets/T-25048-main-80076fdc]] passes default-C literal852-byte proof and MAIN SHA1 e823bd844a8f8fa4d05483b59c66bc54b8393b26, including both jump tables. Header/global guards pass; prototype check reports0 new/15 known. No artificial empty test remains. Exact field/table addresses and source-only controls are preserved in drafts/T-25048; updated [[matching-notes]].

## [2026-10-22] ticket | T-25048 In Review with validated evidence

Moved [[tickets/T-25048-main-80076fdc]] and [[kanban]] card to In Review after parent independent standards/spec review passed without findings. Source pin f3c5c1759. Docker replay confirms current0, reversed locals4, empty use0, local background252, reused background264 diagnostic lines. Wiki lint finds zero broken links/status mismatches and two existing size warnings; evidence prose checker has zero hits. Root owns full27 integration.

## [2026-10-22] ticket | T-25047 TACO home-store continuation

Created [[tickets/T-25047-taco-home-store]] In Progress for the last guarded function in `src/ovl/TACO/80149F90.c`, func_801511C0. Read historical function index/ledgers and latest T-25011/T-25016 evidence; prior ordering/loop/empty-test grids excluded. Root checkout remains untouched.

## [2026-10-22] query | T-25047 entry-boundary controls

[[tickets/T-25047-taco-home-store]] reproduces both historical41-word shapes. New late-eliminated tests/returns/wrappers preserve the late spill; an unreachable call/address escape selects the early home store but wrong ugen temporaries. Volatile self-store adds instructions and is rejected. Identity-definition controls follow from this negative evidence; no source promotion.

## [2026-10-22] build | T-25047 literal TACO pointer-walk match

`src/ovl/TACO/80149F90.c` now provides default C for func_801511C0:164 linked bytes and complete TACO SHA1 match. Removing the empty argument test restores the early home store; a marked late-zero identity chain wraps the temporary ring and disappears from emitted code. [[matching-notes]], [[playbook]] and [[tickets/T-25047-taco-home-store]] preserve the mechanism and rejected causal controls. Header/global/prototype guards pass, with0 new/15 known declaration disagreements.

## [2026-10-22] ticket | T-25047 review passes

Root independent standards/spec review reports no findings. Moved [[tickets/T-25047-taco-home-store]] and [[kanban]] card to In Review for root full27 integration; no compiler/header/build changes or copyrighted artifacts included.

## [2026-10-22] build | MAIN background and TACO pointer walk accepted

Integrated e6c71b6be and e95ec5c3b. Root independently reviewed both against standards/spec with no findings; literal default-C852/164byte proofs and fresh27/27 binary checksums pass. Headers/globals and prototypes0new/15known pass. [[tickets/T-25048-main-80076fdc]] and [[tickets/T-25047-taco-home-store]] move Done. [[status]] now6845/6952functions,2119140/2276920bytes93.1%;107functions/157780bytes remain. Twelve matches add14464bytes. TACO80149F90.c is complete. Workers continue under T-25050/T-25051, avoiding existing claims.

## [2026-10-22] ticket | T-25042 KANGEI dispatch follow-up

Claimed [[tickets/T-25042-kangei-dispatch-emission]] after pinning KANGEI3852C. Read prior switch-weight/global-mover negatives and current T-25011 cast-based draft; source ownership is only src/ovl/KANGEI/80134120.c. Baseline capture precedes new source tests.

## [2026-10-22] query | T-25042 default-arm emission baseline

Reproduced the current four-position cast-based KANGEI34724 miss and old uncast hoist (score26 under the current comparison). Stored-global empty tests promote all switch stores, while local empty tests change whole-function allocation; neither transfers the previous match directly. Preserved captures and negative ledger in drafts/T-25042/README.md and updated [[tickets/T-25042-kangei-dispatch-emission]]. Source is unchanged.

## [2026-10-22] query | T-25042 path rejection confirmed

Original binasm replay shows the cast's default shift rejected by the as1 path check, while the uncast shift is accepted; both default blocks have weight4. Distinct conversion, empty-control placement and adjacent-case exit tests have not restored the target temporary chain. Recorded phase evidence and negative classes in drafts/T-25042/README.md; [[tickets/T-25042-kangei-dispatch-emission]] remains active with no production source edit.

## [2026-10-22] query | T-25042 source-boundary negative ledger

Named conversion locals show that uopt can avoid the cast's extra scratch temporary, but their boundary disappears before the global move and the default t0 shift is hoisted again. Adjacent-case edges, nested switches and explicit cleanup forms likewise fail without producing a match. Preserved source classes and phase constraints in drafts/T-25042/README.md and [[tickets/T-25042-kangei-dispatch-emission]]. Production source remains unchanged; investigation continues.

## [2026-10-22] query | Preserve KANGEI phase constraints

Integrated research checkpoint872d89968 for [[tickets/T-25042-kangei-dispatch-emission]], preserving both histories. Production unchanged; worker continues investigating source boundaries surviving global scheduling. Relayed newly verified T-25047 alternating late-zero identity mechanism to ETC worker whose remaining issue needs one temporary slot. MAIN dispatcher worker reports216/218 exact in a newer unverified candidate; no coverage counted.

## [2026-10-22] build | MAIN dispatcher reduced to two words

Root independent Docker replay confirms [[tickets/T-25040-main-flag-dispatch]] snapshot candidate218/218words with only two differing words at116–117. Preserved source/replay; production unchanged and no coverage increase. Worker continues final b8 base-address residue. TACO continuation owns8013B460 in8013B170.c; quadrant worker reports182differences, not yet independently replayed.

## [2026-10-22] ticket | Root claims final BUNKA_SD function

Created [[tickets/T-25053-bunka-last-function]] before replay, owning80134540.c. T-24516 repairs establish semantic baseline; old guard remains unchanged. MAIN dispatcher worker reports zero differences and is proving literal/default output; T-25052 reserved for its next same-file function after acceptance.

## [2026-10-22] build | Final BUNKA_SD baseline reproduced

[[tickets/T-25053-bunka-last-function]] independently reproduces repaired T-24516 baseline243/243diagnosticwords,144aligned/368printed differences. No production edit or coverage claimed; investigate counter/switch allocation next. The diagnostic span is not a literal function-size proof.

## [2026-10-22] build | T-25040 literal flag dispatcher closure

Promoted `src/main/8006CB30.c` function `func_8006D138` to default C. An exact-width discarded byte view holds uopt's static alias while the real late-zero address stays base-relative; together with the measured constant-range boundary, all218 words match. Annotated guarded/default links each prove872 bytes and complete MAIN SHA1. Header/global checks pass; prototype branch0new/15known. Root's independent final standards/spec review has no findings. [[tickets/T-25040-main-flag-dispatch]] moves to In Review for full27 integration. Reproduction and negative controls are retained in `drafts/T-25040/`.

## [2026-10-22] query | BUNKA counter controls ruled out

[[tickets/T-25053-bunka-last-function]] counter narrowing and empty quotient use each preserve the baseline243words/cost144. First meaningful residue is quotient/counter allocation and missing switch copy. No production edit; continue causal lifetime analysis. ETC reports literal1056-byte default match under affected-unit verification, not yet integrated.

## [2026-10-22] build | MAIN flag dispatcher accepted

Integrated e98740640. Root independent default-C literal872byte proof, fresh27/27 binary hashes, headers/globals and prototypes0new/15known pass. Final source review has no findings; [[tickets/T-25040-main-flag-dispatch]] moves Done. [[status]] now6846/6952functions,2120012/2276920bytes93.1%;106functions/156908bytes remain. Thirteen matches add15336bytes. Worker continues T-25052; ETC candidate still awaits integration.

## [2026-10-22] ticket | T-25045 next target scope check

Created [[tickets/T-25045-etc-ring-slots]] under root's same-file assignment. src/main/8005A0B0.c has no remaining INCLUDE_ASM or NON_MATCHING after T-25039. Comparing at most three historical candidate names against their current default bodies before confirming that scope is complete. No source edits; previous scratch evidence retained.

## [2026-10-22] decision | T-25045 reassigned to ETC ring tail

Root confirmed src/main/8005A0B0.c is complete and reassigned [[tickets/T-25045-etc-ring-slots]] to ETC func_80146D4C in src/ovl/ETC/80146400.c. No stale-candidate rebuilds or MAIN changes. Earlier attempted fast-forward was rejected because integration used a different ancestry; previous pins and evidence remain intact, and relevant source equality will be checked before new work.

## [2026-10-22] build | T-25045 final-block temporary boundary

ETC full-TU baseline for [[tickets/T-25045-etc-ring-slots]] is 264/264 instructions, cost 12; actual size 1056 bytes. Target first skips a ring slot before the index shift and another before the counter result. New late-zero controls either preserve the baseline or alter allocation/add code; faithful bitfield controls also preserve baseline. Retained all production source unchanged and started unmodified ugen phase capture.

## [2026-10-22] build | T-25045 phantom slots localized

Unmodified phase capture for [[tickets/T-25045-etc-ring-slots]] shows the final binasm already has the wrong ring temporaries. Cast/empty-branch late-zero controls do not fix them. A promoted byte self-assignment reaches target t3/t4/t6, but emits unwanted byte stores and a reload copy; retained as diagnostic evidence only and proceeding to local capture. No production edit.

## [2026-10-22] build | T-25045 one missing allocation confirmed

Logging-only ugen replay is byte-identical to standard captured binasm. Allocation traces correct the prior two-slot inference for [[tickets/T-25045-etc-ring-slots]]: only the mask's t2 allocation is missing; t5 is already absent from the later free list. A narrowed late-negative expression reaches the target first load/shift but duplicates the shared address. Testing explicit sharing while keeping the original guard.

## [2026-10-22] ticket | T-25045 ETC literal match enters review

Transferred the newly verified T-25047 alternating identity to the total printf argument in func_80146D4C. The reused pad becomes a late-zero carrier; one intermediate spends the needed ring slot and vanishes in as1. Minimized to byte multiply-zero, marked FAKE, and removed the fallback. Default C matches1056 literal bytes; ETC checksum/header/global/prototype guards pass (0 new/15known). Root independent source review passes. Added drafts/T-25045/README.md, updated [[playbook]], [[matching-notes]] and [[families/etc-family-results]], and moved [[tickets/T-25045-etc-ring-slots]] to In Review pending root full27 integration.

## [2026-10-22] lint | T-25045 review checkpoint

Wiki lint reports zero broken links and zero ticket/board mismatches; only existing data/t0018-cases and growing log size warnings remain. Git diff check passes. ETC SHA1 is681c93b6e57c885f86e074c885a8bf6c3cca2319. [[tickets/T-25045-etc-ring-slots]] remains In Review for integration.

## [2026-10-22] query | BUNKA quotient-copy controls

[[tickets/T-25053-bunka-last-function]] separate signed/unsigned index and late-zero overlapping lifetime controls preserve baseline144cost; u16 narrowing worsens200. No retained source improvement; optimizer-range evidence is needed before further variants. ETC full integration gate is running.

## [2026-10-22] build | ETC grade function accepted

Integrated51a413820. Root default-C literal1056byte proof, fresh27/27 binary checksums, headers/globals and prototypes0new/15known pass; independent source review has no findings. [[tickets/T-25045-etc-ring-slots]] moves Done. [[status]] now6847/6952functions,2121068/2276920bytes93.2%;105functions/155852bytes remain. Fourteen matches add16392bytes. Worker continues T-25054 ETC8014790C; seven Astra lanes and root BUNKA research remain active.

## [2026-10-22] query | BUNKA allocation boundary measured

Instrumented compiler identifies counter local/global priority inversion for [[tickets/T-25053-bunka-last-function]]. Extra global use restores valuea1 but not its address or quotient/selector registers, reducing144→142alignedcost. Saved source/ledger under drafts/T-25053; no production change. Signed quotient controls do not improve. Worker chats remain active; no new completed function this turn.

## [2026-10-22] query | BUNKA quotient allocation constraints

[[tickets/T-25053-bunka-last-function]] full trace exposes high-priority address range before quotient and low-priority selector. Narrow index controls and definite later call-argument reuse do not recover target copies; recorded exact results in drafts/T-25053/README.md. No production change. Contributor check unchanged: no open PRs, issue65 claim unchanged. Requested stable quadrant improvement pin while worker continues.

## [2026-10-22] build | Quadrant signed-mask capture clarifies pipeline effect

Astra refined [[tickets/T-25034-main-quadrant-extent]] diagnosis: raw ugen still includes intermediate signed-mask chain/copies; the existing pipeline collapses them into the desired final instructions. Capture output exactly reproduces the canonical666word,212/212 candidate. Source improvement is verified, but earlier CVTL wording must not be read as proof of changed raw destination forwarding. No compiler/tool changes.

## [2026-10-22] build | Quadrant RGB grouping improves to198

[[tickets/T-25034-main-quadrant-extent]] per-vertex RGB comma grouping gives674/666words,198aligned/198printed. Colored-quad extraction/temporaries/twelve stores match; branch delay slot still differs. Current signed-mask.c updated from immutable worker candidate, previous212 source retained in b25963c45. No production promotion; Astra continues.

## [2026-10-22] build | Quadrant RGB-group checkpoint replayed

Docker replay of the updated signed-mask.c independently reproduces674target/666built words,198aligned/198printed for [[tickets/T-25034-main-quadrant-extent]]. This pins the newer grouping improvement while worker continues, without repeating the superseded212 proof. No production promotion.

## [2026-10-22] build | Quadrant UV load-delay slots recovered

[[tickets/T-25034-main-quadrant-extent]] moves four v3 stores after corresponding y3 coordinates and restores target load-delay-slot ordering through normal scheduling. Latest immutable source674target/670built words,186aligned/186printed, now four words short. Archived signed-mask.c updated; no production promotion, worker continues.

## [2026-10-22] build | Quadrant UV checkpoint replayed

Independent Docker replay confirms latest signed-mask.c at674target/670built words,186aligned/186printed for [[tickets/T-25034-main-quadrant-extent]]. Pinning source and scheduling notes while the worker continues. Production remains unchanged.

## [2026-10-22] build | Quadrant TILE load-delay scheduling improves

[[tickets/T-25034-main-quadrant-extent]] case0 width-after-y0 placement improves186 to182 differences at674target/670built words, retaining full-word extents. Immutable worker source preserved privately; worker continues case1 grouping. No production promotion.

## [2026-10-22] build | Quadrant UV exits improve to159

[[tickets/T-25034-main-quadrant-extent]] natural TILE width/height ordering reaches178/178, then explicit UV u0 stores before shared exit goto reach674target/673built words,159aligned/159printed. Saved-register ranks retained and all UV exit branches/delay stores restored. One missing TILE radius MOVE and UV endpoint allocation remain. Archived signed-mask.c updated; full-word TILE semantics preserved, no promotion.

## [2026-10-22] build | Quadrant159 checkpoint replayed

Independent Docker replay confirms current signed-mask.c at674target/673built words,159aligned/159printed for [[tickets/T-25034-main-quadrant-extent]]. Pin source plus causal notes; worker remains active and production unchanged.

## [2026-10-22] build | Quadrant radius move exists before final pipeline

[[tickets/T-25034-main-quadrant-extent]]159-candidate capture reproduces canonical673words exactly. Raw ugen emits the missing TILE case2 radius MOVE; final pipeline removes it, whereas case3 counterpart survives. Allocation-only premise rejected; worker continues coordinate/extent dependency and grouping. No promotion or compiler changes.

## [2026-10-22] build | Quadrant integer return restores exact word count

[[tickets/T-25034-main-quadrant-extent]] s32-return control restores missing TILE radius MOVE:674/674words,144aligned cost. Archived integer-return.c alongside conservative void159 source. Parent audited both known callers: discarded C result and no target v0 use after calls, with no existing header declaration. Supports integer/implicit-int contract without uniquely proving signature. No shared-header or production change; Astra continues.

## [2026-10-22] build | Quadrant integer-return checkpoint replayed

Independent Docker replay confirms integer-return.c at674/674words,144aligned/902positional-printed for [[tickets/T-25034-main-quadrant-extent]]. Astra also verified actual implicit-int spelling is byte-identical to explicit s32. Remaining groups: entry temporaries/homes, UV endpoint v1 vs a1 and related zero-argument hoist, TILE t2/t4 choices, green-shift temporary. No operation-count gap remains; worker continues.

## [2026-10-22] build | Root preserves exact-length quadrant checkpoint

Integrated7a4f9d6c2 research only. Root Docker replay of integer-return.c reproduces674/674words,144aligned/902positional lines; verified existing caller expressions discard results and header search has no declaration. [[tickets/T-25034-main-quadrant-extent]] remains active. No production/header change or coverage increase. Conservative void159 candidate and caller audit retained alongside improved integer-return branch.

## [2026-10-22] decision | Expand Astra matching to three additional units

User requested more agents. Root reactivated the existing TEL/ENDING/ETC coordinator chat with Astra high and authorized three Astra high workers: T-25055 TEL func_80136724 (`src/ovl/TEL/80132000.c`), T-25056 ENDING func_8013A2E8 (`src/ovl/ENDING/801396E0.c`), and T-25057 NAME_ENT func_801398EC (`src/ovl/NAME_ENT/80139740.c`). Coordinator creates tickets in isolated trees and confirms exact claims before experiments. Existing seven worker assignments and root T-25053 remain owned; GEKO/OPTION contributor reservation preserved. Docker-only, causal experiments, source checkpoints and literal default-C gates remain required. No coverage increase from dispatch. See [[status]] and [[tickets/T-25017-completion-plan]].

## [2026-10-22] query | BUNKA quotient storage controls negative

[[tickets/T-25053-bunka-last-function]]: Docker quotient-storage controls returned u16 244 words/cost171, s16 247/cost204, reversed zero assignments243/cost144 against243 target words. Existing counter-use cost142 remains best; no production edit or coverage change.

## [2026-10-22] query | BUNKA counter typing improves register allocation

[[tickets/T-25053-bunka-last-function]] new source checkpoint `drafts/T-25053/signed-selector.c` scores104 aligned edits at243/243 words, improved from142. Signed counter with unsigned division recovers a1/a0; marked empty selector use recovers a2. Missing quotient copy and early selector load remain. No match or coverage credit. GitHub check: no open PRs; issue65 unchanged, GEKO/OPTION reserved.

## [2026-10-22] decision | Three additional Astra workers confirmed running

Coordinator confirmed `/root/astra_tel_25055`, `/root/astra_ending_25056`, `/root/astra_name_25057` on TEL80132000.c, ENDING801396E0.c and NAME_ENT80139740.c respectively. Three targets remain guarded. Isolated worktrees and per-function tickets; two compiler slots in that chat while NAME_ENT begins qualification. Existing seven assignments continue; root retains BUNKA_SD. See [[status]].

## [2026-10-22] query | BUNKA reset ordering and index-conversion controls

[[tickets/T-25053-bunka-last-function]] archived signed-selector candidate now102 aligned cost after matching b-before-q zero reset order. Separate signed index and signed quotient do not restore target copy; narrow indices worsen132/166/177. No production change; full match remains unproven. MAIN T-25050 worker reports796-byte literal match, pending final pin and root integration gates.

## [2026-10-22] ticket | T-25052 remaining MAIN schedule dispatcher

Root accepted and pushed T-25040 at4933de332 after full27 hashes and guards passed. Created [[tickets/T-25052-main-schedule-dispatch]] In Progress for the exclusively assigned `func_8007132C`, preserving both accepted same-file matches. Read the newest T-24024 snapshot before new source experiments; production remains assembly.

## [2026-10-22] build | T-25052 dispatcher baseline reproduced

Whole-TU replay reproduces T-24024 exactly: 998 target words, 975 baseline (aligned cost 1013), 984 byte-view calendar candidate (892). A targeted volatile-calendar control is worse at 961 words / 967 and is retained as a negative. The first remaining structural issue is the schedule pointer's origin: target keeps the GameState origin plus index and accesses +0x69C, while the old draft points directly into the record array. Investigate that representation before register tuning; production remains assembly. See [[tickets/T-25052-main-schedule-dispatch]].

## [2026-10-22] build | T-25052 first dispatcher decisions recovered

Calendar byte views and independent first-count lifetime (`op`, with later count
in `t`) recover frame72/spill0x30 and the first101 instructions except exit-branch
distances. The984/998-word draft remains unpromoted. Explicit GameState pointer
rewrites, volatile calendars, empty-switch/test controls, loop-bound identities,
and index-view induction negatives are retained in drafts/T-25052. The direct
record type already produces the correct displaced access. Remaining work is
address/loop-bound promotion after the first helper calls. See
[[tickets/T-25052-main-schedule-dispatch]].

## [2026-10-22] build | MAIN dispatcher research checkpoint replayed

Integrated research pin656e2aa7e for [[tickets/T-25052-main-schedule-dispatch]]. Independent Docker replay confirms field-first-count-op984/998words,860aligned cost. Source-only archive preserves causal controls and first101instruction evidence; production unchanged. Wiki lint0broken links/0status mismatches, only existing large-page warnings.

## [2026-10-22] ticket | T-25050 ring input ownership

Created [[tickets/T-25050-main-ring-input]] In Progress for func_80075C84 in src/main/80075320.c after the accepted T-25048 match. Historical T-24303/T-24501 aliases and T-25016 diagnostics reviewed; canonical declarations and compiler pipeline remain fixed. Work stays in the isolated /private/tmp/tokimemo-t25044 tree.

## [2026-10-22] build | T-25050 baseline and reload control

[[tickets/T-25050-main-ring-input]] reproduces the canonical two-symbol baseline at338 diagnostic lines. A narrowly volatile position read after each key store restores the read but adds explicit address formation (202 diagnostic lines); it is a negative control, not a promoted candidate. Next separate the store-address alias question from selector allocation using existing globals.

## [2026-10-22] build | T-25050 switch reload mechanism isolated

[[tickets/T-25050-main-ring-input]] complete key-store integer address retains the key base and restores position rereads. The raw input-word empty use fixes selector allocation, reducing338 to31 diagnostic lines. Remaining evidence isolates outer-loop position-load hoisting and increment scheduling. No production/header/compiler edits.

## [2026-10-22] decision | T-25050 direct loop-load dependency

[[tickets/T-25050-main-ring-input]] records the alias, volatile, natural traversal and initialization negatives. Best31 remains canonical direct position load hoisted outside the loop. Parent suggests an unsigned-impossible comparison as a zero address offset dependent on the outer index, to test whether late folding retains the target reload. No header/global override is needed for this control.

## [2026-10-22] build | T-25050 direct reload restored

[[tickets/T-25050-main-ring-input]] zero-offset address forms prevent hoisting but retain an unwanted address register. A late-folded unsigned-impossible control after the direct scalar read restores instruction count and direct symbol access (28 diagnostic lines). Instrumented and ordinary compilers agree; remaining register/spill priorities and cursor increment placement are documented. No literal-match claim or production change.

## [2026-10-22] build | T-25050 four-row candidate

[[tickets/T-25050-main-ring-input]] reaches8 diagnostic lines (four rows) by placing the unsigned-impossible position store at the outer-loop tail. The store and branch disappear; global alias facts keep the direct loop reload, while all inner-loop register priorities now match. Remaining differences are swapped cursor increments and the a1 call-spill home2C versus28. This would require an explicit FAKE comment if retained.

## [2026-10-22] build | T-25050 literal ring input match

[[tickets/T-25050-main-ring-input]] proves796 literal linked bytes. Captured unchanged compiler ucode identifies the last spill offset as a missing word of local extent; naming the real random result fixes it without padding. C promoted with explicit FAKE explanations and exact field/table address audit. Parent review requested and default MAIN/declaration gates started.

## [2026-10-22] ticket | T-25050 checked source ready for integration

[[tickets/T-25050-main-ring-input]] moves to In Review after default-C literal796 proof, MAIN checksum and declaration guards pass. Parent standards/spec review has no findings. Necessity controls remove the redundant integer key-address trick; final source uses canonical array indexing and two marked FAKE mechanisms. Source-only baseline/replay and all meaningful negative findings are preserved under drafts/T-25050; root owns full27 integration.

## [2026-10-22] lint | T-25050 evidence checks

[[tickets/T-25050-main-ring-input]] source-only pin2ebd6fd3a is ready for root integration. Final simplified diff independently reviewed with no findings. Wiki lint reports0 broken links and0 status mismatches; two existing large-page warnings remain. Evidence prose findings were reviewed, with technical key terminology retained and historical counts formatted as a table.

## [2026-10-22] build | MAIN ring input accepted on main

[[tickets/T-25050-main-ring-input]] Done after independent default-C796-byte MATCH, fresh27/27 binary checksums, headers/globals and0new/15known prototypes. Final source standards/spec review has no findings. Source integrated345fea3b; evidence09b341db2 preserved. Entire80075320.c is C. [[status]] now6848/6952functions,2121864/2276920bytes;104functions/155056bytes remain. Fifteen matches add17188bytes this resumed effort. ETC match still pending its own integration.

## [2026-10-22] query | BUNKA quotient identity boundary controls

[[tickets/T-25053-bunka-last-function]] q*1 is neutral102 aligned cost; OR late-zero from b/selector worsens112/138, all243words. Keep archived best; no production change. Root independently accepted MAIN T-25050 in prior entry; ETC and TEL zero-difference candidates are undergoing source and integration validation.

## [2026-10-22] query | BUNKA counter alias controls

[[tickets/T-25053-bunka-last-function]] scalar-load and combined load/store views worsen102→142; store-only remains102, all243words. Preserved best candidate. TEL worker assigned next same-file target under reserved T-25060 after current match pin; MAIN nextT-25058 and ETC nextT-25059 remain reserved independently.

## [2026-10-22] ticket | T-25054 ETC results scene continuation

Created [[tickets/T-25054-etc-results-scene]] for func_8014790C, continuing exclusive ownership of src/ovl/ETC/80146400.c after accepted T-25045. Selected the latest roughly44-row T-24909 archive over the file's much larger ranking draft. Preserved prior pins/evidence on codex/t25054-etc-results; reproduce complete-TU baseline before causal source experiments.

## [2026-10-22] build | T-25054 late-folded index isolates remaining base and call scheduling

[[tickets/T-25054-etc-results-scene]] reproduced the current full-TU baseline (282 words, aligned cost66), then removed the stale separate n counter and used a late-folded zero OR to retain the original index arithmetic (281 words, cost21). Unmodified compiler captures locate the remaining base sharing in uopt and a call delay-slot reorder. Pointer bias and late branch controls do not solve the base sharing. Private evidence is in build/t25054/; production remains guarded.

## [2026-10-22] build | T-25054 one-iteration boundary restores independent bases

[[tickets/T-25054-etc-results-scene]] found a source control-flow boundary that survives uopt but folds before final assembly: a one-iteration initializer loop with an unsigned-less-than-zero condition. It restores both target address materializations and 282-word size; its condition operand currently extends the switch byte's range. Captures and negative controls remain under build/t25054/.

## [2026-10-22] ticket | T-25054 matched and moved to In Review

[[tickets/T-25054-etc-results-scene]] now has a default-C literal match for all1128 bytes, an unchanged ETC checksum and passing declaration guards. Canonical DisplayActor44 fields preserve the result. Recorded the separate-base boundary, array noalias mechanism and negative controls in [[matching-notes]], [[playbook]] and drafts/T-25054/README.md. Ticket and board moved together to In Review for root source review and full27 integration.

## [2026-10-22] decision | T-25054 source review passed

Root independently checked [[tickets/T-25054-etc-results-scene]] against CODING_STANDARDS.md and its scope. No findings remain. Final canonical source passes the default literal comparison, ETC checksum and declaration guards; root will run the full 27-binary integration gate.

## [2026-10-22] build | ETC results scene accepted on main

[[tickets/T-25054-etc-results-scene]] Done after independent default-C1128-byte MATCH, fresh27/27 binary hashes, headers/globals and0new/15known prototype checks. Canonical source direct standards/spec review has no findings. [[status]] now6849/6952functions,2122992/2276920bytes;103functions/153928bytes remain. Sixteen matches add18316bytes this resumed effort. Worker continues801480D0 underT-25059; TEL candidate still pending integration.

## [2026-10-22] decision | Correct switch-copy explanation from actual ucode

T-25052 option inspection finds `OPTN 405 1` before both the schedule dispatcher
and the accepted D138 sibling. The earlier claim that D138's empty default-only
switch enables zcopy off was incorrect: cfe drops that selector and retains entry
control flow. Its literal match is unaffected. READMEs and tickets are corrected.
A real two-case switch or a register-qualified existing local assigned a direct
unsigned global does invoke the uniform zcopy-off rule; the latter adds no
runtime operations but gives 985/998 words, cost975, so is only a diagnostic.
See [[tickets/T-25040-main-flag-dispatch]] and
[[tickets/T-25052-main-schedule-dispatch]].

## [2026-10-22] query | Independently verify compiler-option correction

Root Docker parse of captured input confirms field-entry-switch has OPTN405=1 for both acceptedD138 and unmatched7132C. register-entry-unrelated keepsD138=1 and changes7132C=0. Preserved correctiond259ee253 with no production/compiler changes. Historical logs remain intact; [[tickets/T-25040-main-flag-dispatch]] remains Done.

## [2026-10-22] decision | Respect MAIN coordinator chat wrap-up request

MAIN coordinator reports a direct human request in that chat to wrap up, commit and push. Its three lanes (T-25034 quadrant, T-25051 TACO, nextT-25058) are stopping experiments and preserving branches. Root will not restart those lanes contrary to that request. Other chats and root workers continue existing assignments; root alone integrates main. [[status]] reflects this scope distinction.

## [2026-10-22] query | BUNKA return lifetime ruled out

[[tickets/T-25053-bunka-last-function]] scratch-only s32 definition/declaration control worsens best to244words/cost105; baseline243/cost146. Canonical void header remains unchanged. Current archived best102 persists. TEL final ABI removal controls and worker gates pass; source pin is being prepared for root integration.

## [2026-10-22] ticket | Claim T-25055 TEL causal matching lane

[[tickets/T-25055-tel-pass-mechanisms]] starts In Progress from main633d95356 in the reused isolated /private/tmp/tokimemo-t25024 checkout on codex/t25055. Prior branch codex/t25024-tel-qualification preserves3b257980c and both donor candidates. Exclusive scope is src/ovl/TEL/80132000.c:func_80136724. No production/header/tool edits or C coverage claim. Next step reproduces whole-TU baseline and traces early pass-level divergence; broad shape grids remain excluded.

## [2026-10-22] query | Reproduce TEL pointer baseline and prepare pass captures

[[tickets/T-25055-tel-pass-mechanisms]] reproduces T-25024 pointer baseline at1643/1645 words,1403 changed positions,970 aligned cost in Docker on current full TU. First non-displacement mismatch remains word85. The unchanged early state increment precedes a one-slot temporary-ring deficit; capture harness drafts/T-25055/probe.py will forward identical stage arguments and retain uopt/ugen/as1 files privately under build. Ranked causal probes: (1) erased narrowing after state increment consumes the missing temporary; (2) eligibility zero and selected minimum share a range in the original; (3) mixed exact-width views explain redundant address setup. No source-shape grid or production change. Compiler slot yielded for sibling baseline after this reproduction.

## [2026-10-22] build | TEL selector and total ranges remove widespread temporary mismatch

[[tickets/T-25055-tel-pass-mechanisms]] Docker controls: case0x40 direct D_801405E4 stores/reads (no ent local) gives1644 words/cost127 versus baseline1643/cost970. Exact byte-view local load is neutral970. Sharing case6 maximum and total in total alone gives1644/cost969. Combining the two gives1645 target and candidate words,41 changed positions,cost78; preserved drafts/T-25055/combined-41.c. The early temporary mismatch was a missing reserved color, not a consumed slot; avoid increment-mask probes. Global store movement crosses no call or observable alias use before sndisp, and total is assigned in both max branches before its eligibility use. No production promotion; remaining target mismatch starts at word298, with global t0/t1 swap, local max input colors, one address schedule and one compare operand order.

## [2026-10-22] build | TEL default C reaches literal linked match

[[tickets/T-25055-tel-pass-mechanisms]] adds a defined always-false unsigned girl test before case6 rank calculation:41 changed positions becomes1 (the local-sharing and argument-view controls are neutral). The last second case0x40 branch operand order is corrected with hp*1; whole-TU1645words/0changed positions. Annotated source installed as default C. Docker funcdiff --resolve --version default reports MATCH: linked function bytes equal target6580bytes. Padding, inferred text extent, unsigned-false test and neutral compare expression are marked FAKE; the observed third strcat argument is explained. Full27/guard validation and direct coordinator standards/spec review remain pending.

## [2026-10-22] build | TEL final ABI cleanup and full validation

[[tickets/T-25055-tel-pass-mechanisms]] passes full27 binary checksums, then fresh27 hashes after neutral cleanup; default C is literal linked MATCH6580bytes. Headers/globals pass; prototype branch check reports0 new/15 known, OK. Removing extra strcat month arguments independently and together is neutral; typed set_kanji_string integer casts are neutral and removed. Actual func_80052060 definition takes4xs32, supporting its retained table-pointer casts. Final trace identifies girl field static6+0xF5F: uses7->9,priority0.25->0.4,t1->t0; selector t0->t1,lim a0->v0,hp a1->a0. Compact metadata in drafts/T-25055; no compiler/header/flag edits.

## [2026-10-22] ticket | TEL T-25055 ready for integration

[[tickets/T-25055-tel-pass-mechanisms]] moves to In Review with matching board. Coordinator direct standards/spec review has no unresolved finding after frame/buffer FAKE annotations and ABI cleanup. Source-only intermediate controls, final source and exact result/hash metadata are preserved; private full-TU/assembly/ucode outputs are excluded. Root owns integration and global counts. Compiler slot released after final checks.

## [2026-10-22] lint | T-25055 final archive checks

[[tickets/T-25055-tel-pass-mechanisms]] git diff whitespace check passes. Docker wiki lint finds0 broken links and0 status mismatches; existing large data/t0018-cases and log pages remain. The archive README writing check has0 strong hits and one weak passive construction retained for the exact allocator identity. Source/replay metadata contain no private instruction listings or binary files.

## [2026-10-22] build | Largest remaining TEL dialogue accepted on main

[[tickets/T-25055-tel-pass-mechanisms]] Done: root independent default-C6580-byte MATCH, fresh27/27 binary hashes, headers/globals and0new/15known prototypes pass. Direct final source standards/spec review has no findings; ABI cleanup and all appointment bits verified. [[status]] now6850/6952functions,2129572/2276920bytes93.5%;102functions/147348bytes remain. Seventeen matches add24896bytes this resumed effort. EntireTEL80132000.c is C; worker transferred to remainingTEL8013D88C in8013CCD0.c underT-25060.

## [2026-10-22] build | Quadrant entry scaling and row priority separated

[[tickets/T-25034-main-quadrant-extent]] early row scaling fixes entry temporary order but adds four endpoint uses, raising priority8.2->12.2 and shifting s7->s3. Repeated original extraction canonicalizes unchanged. This control is rejected as a candidate; integer144 remains best. The successful signed-conversion-tree boundary distinction was also shared with T25051's independent scheduling investigation, without parent source experiments.

## [2026-10-22] build | Quadrant hidden UV base-copy range identified

For [[tickets/T-25034-main-quadrant-extent]], raw ugen disproves a regdiff constant-label guess: hidden a1 range is a required base-UV copy later removed by as1. Priority30 overlaps endpoint40 and reverse-radius36.67. Real consumer lifetimes now guide the remaining endpoint-register work; integer144 remains best and production unchanged.

## [2026-10-22] decision | Preserve quadrant UV ordering checkpoint and wrap up

[[tickets/T-25034-main-quadrant-extent]] standard forward UV comma grouping exchanges real base-row/endpoint range priorities and improves aligned144 to116 with674/674 padded words. Archived source-only draft and replay option; native trace confirms endpoint a1 and removes wrong a1=0 hoist. User requested current work committed and pushed; all three workers instructed to freeze and preserve evidence. No production promotion or completion claim.

## [2026-10-22] build | Verify final quadrant research checkpoint

[[tickets/T-25034-main-quadrant-extent]] archived uv-standard.c replay independently returns target674/candidate674 words, aligned116/printed156. Docker wiki lint:0 broken links,0 status mismatches,1 existing large-page warning. README prose check:0 hits. Diff whitespace check passes. No generated binary or raw input is staged.

## [2026-10-22] build | Preserve stopped quadrant lane checkpoint

Root independently replayed [[tickets/T-25034-main-quadrant-extent]] uv-standard.c:674/674words,116aligned cost,156printed lines. Preserved131bc66b6 and the corrected allocator explanation. Production unchanged; lane remains stopped under its chat's human wrap-up request, no coverage credit.

## [2026-10-22] ticket | T-25051 TACO wipe quad store schedule

Created [[tickets/T-25051-taco-quad-store]] In Progress for func_8013B460 in `src/ovl/TACO/8013B170.c`, after completing 80149F90.c. Historical and current ledgers read; old grouping/order grids excluded. Source ownership is narrow; root checkout untouched.

## [2026-10-22] query | T-25051 store-promotion and expression staging controls

[[tickets/T-25051-taco-quad-store]] reproduces332-word baseline and two early halfword stores. New stored-field tests disrupt broader promotion; arithmetic across UV side effects moves coordinate loads too late. Explicit y3 local preserves the desired late store but interferes with constants and shifts downstream temporaries. Full-TU allocation tracing will distinguish those effects; no source promotion.

## [2026-10-22] build | T-25051 dependency controls

Recorded source-local store, expression, and temporary-lifetime controls in [[tickets/T-25051-taco-quad-store]]. The baseline remains two misplaced stores; local lifetime changes either displace the UV constant registers or add code. Explicit narrowing spends a temporary but leaves the store early. All probes use the unchanged Docker compiler and isolated scratch files; production source is unchanged.

## [2026-10-22] build | T-25051 local scheduling trace

The actual stock compiler trace ties both y3 and UV stores at aftercycles6 before the mode load. Recorded the trace and new conversion-boundary, scoped-local, and immediate-join negatives in [[tickets/T-25051-taco-quad-store]]. The production function remains guarded; no checksum claim is made for scratch probes.

## [2026-10-22] decision | T-25051 preserve failed temporary controls

Extended [[tickets/T-25051-taco-quad-store]] with expression/encoding and local-array negatives and the stock scheduler tie-break reading. The y3 temporary continues to occupy v0; all encoding variants either retain that allocation or add code. Baseline remains the best valid draft. Continue source reconstruction; no production promotion or blocker claim.

## [2026-10-22] ticket | T-25051 requested research checkpoint

The user requested current work be frozen, committed, and pushed. Moved [[tickets/T-25051-taco-quad-store]] to In Review and archived source-only probe generators in drafts/T-25051/. The production source remains unchanged with the two-store mismatch; this checkpoint is not a match or external blocker. Parent owns independent wrap-up review and integration.

## [2026-10-22] lint | T-25051 checkpoint review and validation

Parent independently reviewed the research packet for [[tickets/T-25051-taco-quad-store]] and reported no findings. Docker parsed all 27 archived Python scripts; wiki lint found zero broken links and zero ticket-status mismatches, with two existing oversized-page warnings. Applied the local deslop prose check and corrected its weak colon finding. Production source remains unchanged; the unmatched function stays In Review.

## [2026-10-22] build | Preserve stopped TACO scheduling lane

Root replay of [[tickets/T-25051-taco-quad-store]] baseline reproduces332words/alignedcost4. Preserved5a5733657 source-only generators, negative controls and scheduler evidence; production unchanged. Ticket In Review is a research handoff, not a match or external blocker. Lane remains stopped under its chat's human wrap-up request.

## [2026-10-22] ticket | T-25058 MAIN screen-offset qualification

Created [[tickets/T-25058-main-screen-offset]] In Progress before experiments and added its board card. Historical T-23006/T-24303/T-25016 and newer candidate ledgers identify two quotient narrowings and a temporary-order gap in `src/main/80047550.c` `func_80047560`. Read-only audit found the guarded signed-byte divisor view differs from the unsigned target load and canonical timer declaration; ownership confirmation requested from the parent before source edits.

## [2026-10-22] decision | Preserve T-25058 qualification without experiments

The parent relayed the user's request to commit and push all work and explicitly stop before T-25058 source experiments. [[tickets/T-25058-main-screen-offset]] returns to Ready with its card synchronized. The source-level audit records the timer-128 denominator discrepancy (128 in the signed-view draft versus 384 in the original unsigned load); no compiled behavior is claimed and no production source was edited. Future work must reconfirm ownership before testing this evidence.

## [2026-10-22] lint | T-25058 qualification records

Docker `tools/wiki.py lint` reports zero broken links, zero status mismatches and the same two existing large pages (`data/t0018-cases` and this log). `git diff --check` passes. The only pending files are [[tickets/T-25058-main-screen-offset]], [[kanban]] and [[log]]; no source or private input is included.

## [2026-10-22] query | Preserve MAIN screen-offset qualification

Preserved e3df7af22 and [[tickets/T-25058-main-screen-offset]] Ready status. Root source read confirms the guarded divisor uses a signed-byte view while the canonical timer advances through128; this is an unresolved semantic audit, not a matching promotion. No experiments or production edits added to the stopped lane. All three wrapped-up lanes now have their handoff checkpoints on main. Combined wiki lint0broken links/0status mismatches; existing large pages only.

## [2026-10-22] ticket | T-25056 ENDING scheduler lane

Created [[tickets/T-25056-ending-scheduler]] In Progress in isolated codex/t25056, preserving prior ENDING research branch. Ownership is src/ovl/ENDING/801396E0.c; first target func_8013A2E8. Read existing [[notes/matching-07]] evidence before qualification.

## [2026-10-22] build | T-25056 baseline qualification

[[tickets/T-25056-ending-scheduler]] reproduced full-TU baseline12 and prior pointer-lifetime10 using the existing Docker image. No production changes. Beginning print-only assembler trace to identify the failed motion gate.

## [2026-10-22] query | ENDING global motion candidate budget

[[tickets/T-25056-ending-scheduler]] traced A2E8's first flag block: equal-weight later join candidates consume the as110-attempt budget before either flag arm is examined. This gives a concrete cause for the prior whole-function deletion threshold. Source-only preload controls follow; production unchanged.

## [2026-10-22] query | T-25056 preload controls and exact cutoff

[[tickets/T-25056-ending-scheduler]] records six preload and three late-false loop controls, none improving the pointer baseline10. Corrected scanner identity and off-by-one: threshold10 admits11 candidate blocks, rejects the12th. Compiler slot yielded to another lane while preparing a scratch counterfactual; production remains unchanged.

## [2026-10-22] query | ENDING budget counterfactual closes diagnostic

[[tickets/T-25056-ending-scheduler]] proves the as1 candidate budget causes this residue: an unchanged-C scratch backend with threshold100 yields literal4648-byte equality; its default diagnostic equals stock. This is diagnostic evidence only, not a production match or authorized pipeline change. Exact source patch and replay recorded in drafts/T-25056; source-only work continues.

## [2026-10-22] query | T-25056 backedge phase boundary

[[tickets/T-25056-ending-scheduler]] records17 causal source controls. Early-folded backedges preserve the restrictive as1 budget; a late-zero carrier retains a natural-loop region with budget100 but also unwanted runtime branches. Added the candidate-count cutoff to [[as1-scheduler-rule]]. Direct standards/spec checkpoint review found no production changes; root's wording clarification resolved. Investigation continues.

## [2026-10-22] ingest | Preserve ENDING scheduler counterfactual evidence

Preserved91e1f4182 under [[tickets/T-25056-ending-scheduler]]. Review confirms all diagnostic assembler construction remains under ignored build/, guarded by an exact reconstruction source hash; no production toolchain/source/config changes. Captured candidate-budget evidence distinguishes region header and scan count from instruction count. Four archived scripts parse in Docker; wiki lint0broken/0status mismatches. No coverage claim and no independent root replay of the experimental backend in this ingest; worker literal counterfactual remains research only.

## [2026-10-22] query | BUNKA tail local reuse control

[[tickets/T-25053-bunka-last-function]] reusing b/q in the later animation block gives baseline138 and current-best104 (243words), so archived102 remains best. Source unchanged. T-25059 ETC worker has passed default2808-byte proof and affected checks; root awaits its pin for combined integration.

## [2026-10-22] ticket | T-25057 NAME_ENT claim

Created [[tickets/T-25057-name-ent-pass-investigation]] In Progress in isolated /private/tmp/tokimemo-t24515 from633d95356. Exclusive source src/ovl/NAME_ENT/80139740.c; first target func_801398EC. Prior30b4f5716 preserved on its original branch. Docker compilation waits for a coordinator slot; source/history analysis proceeds.

## [2026-10-22] build | T-25057 remainder lifetime restores exact shape

[[tickets/T-25057-name-ent-pass-investigation]]: Docker full-TU controls recover123 target words from126 using a named frame remainder and a late-erased unsigned comparison; moving the index initialization before the selector fixes v0/v1. Best source in drafts/T-25057/func_801398EC.c has72 diff rows, no match. Allocator trace identifies the remaining unconstrained pointer/remainder/frame/index ordering. Eight causal probes, no production/header/tool edits; compiler slot returned for TEL.

## [2026-10-22] build | T-25057 record induction ordered correctly

[[tickets/T-25057-name-ent-pass-investigation]]: an exact-width discarded record read moves the existing induction to a0 without emitted instructions. Best source123 words/36 diff rows, still not matching; index/remainder allocation and one load ordering remain. Follow-up local phi controls fail and are archived by serial replay scripts. No production changes; compiler slot released to ENDING.

## [2026-10-22] build | T-25057 isolated frame/remainder pair

[[tickets/T-25057-name-ent-pass-investigation]]: precomputing the frame remainder before the flag test lets uopt sink it lazily and order the index range correctly. Best123 words/30 diff rows: only frame and remainder registers are exchanged. Signed/in-place forms are neutral; narrow and phi forms regress. Best saved in drafts/T-25057/func_801398EC.c; no production promotion.

## [2026-10-22] build | T-25057 two precise source frontiers

[[tickets/T-25057-name-ent-pass-investigation]] archives scalar123/30 and array123/34 sources. The array's removed dead initialization fixes every uopt colour; captured ucode identifies the remaining row/tile expression evaluation order before ugen temporaries. No production changes or coverage claim. Source controls and measured counts preserved under drafts/T-25057/.

## [2026-10-22] lint | T-25057 checkpoint review

[[tickets/T-25057-name-ent-pass-investigation]]: annotated sources reproduce108/123 and106/123 identical resolved words. Docker wiki lint has0 broken links/0 status mismatches; existing large data/log pages remain. Direct standards/spec and staged prose review found no unresolved checkpoint issue. Ticket remains In Progress; production C unchanged.

## [2026-10-22] build | NAME_ENT saved frontiers independently replayed

Root replay of [[tickets/T-25057-name-ent-pass-investigation]] checkpoints confirms123words each: scalar108identical/15different positions, array106identical/17different;30/34printed rows respectively. Preserved1f0eab46b without production edits. Worker now reports a new unsigned noncommutative expression with0differences, pending literal proof and final validation; archived frontiers remain research, not coverage.

## [2026-10-22] ticket | T-25059 ETC ranking generator continuation

Created [[tickets/T-25059-etc-ranking-generator]] before experiments, preserving the T-25054 match pin. Read the corrected T-24049 reconstruction and T-24513 negative controls. Continue exclusive ownership of src/ovl/ETC/80146400.c with func_801480D0; root handles integration.

## [2026-10-22] build | T-25059 correct counter reuse and isolate first-noise lifetime

[[tickets/T-25059-etc-ranking-generator]] reproduced the corrected baseline and verified diagnostic tracer fidelity. Reusing the generation counter as the inner counter in the tie and selection phases removes an unwanted saved induction range. Separating the first noise result from later adjustment amounts restores the target frame and saved-register count. Best scalar reconstruction is705 words/cost595 versus702 target words; production remains INCLUDE_ASM. Private candidates and traces are in build/t25059/.

## [2026-10-22] build | T-25059 recovers generation order and target local homes

[[tickets/T-25059-etc-ranking-generator]] now has704 candidate words versus702 target, aligned cost526. Counter-role corrections, a separate noise lifetime, inverted selector layout, commuted product operands and reused random temporary restore the first generation block. Local ordering follows observed stack offsets and uses no padding. The second product's destination is the next causal difference; production remains guarded by its original INCLUDE_ASM.

## [2026-10-22] ticket | T-25059 ranking generator matches and enters review

[[tickets/T-25059-etc-ranking-generator]] matches all 702 instructions / 2,808 bytes in the default C build. Natural counter/scalar lifetimes, explicit lower-bound delta and direct player-rank accesses close the remaining differences. The unused frame word and scalar order are marked FAKE. Added drafts/T-25059/README.md and updated [[matching-notes]] and [[playbook]]. ETC checksum and declaration guards pass with zero new / 15 known prototype disagreements. Root independent standards/spec review reports no findings. Ticket and board moved together to In Review pending root's full 27-binary gate; src/ovl/ETC/80146400.c now has no remaining fallback guards.

## [2026-10-22] lint | T-25059 checkpoint evidence checked

Wiki lint reports zero broken links and zero status mismatches; the two existing large-page warnings remain. Git diff check passes. Deslop review of drafts/T-25059/README.md and the ticket found no strong tells; retained the ticket heading and stack-offset list colon. [[tickets/T-25059-etc-ranking-generator]] remains In Review for integration.

## [2026-10-22] build | ETC ranking generator accepted on main

[[tickets/T-25059-etc-ranking-generator]] Done: root independently proves2808 default-C linked bytes, fresh27/27 hashes, headers/globals and0new/15known prototypes. Source standards/spec review has no findings. [[status]] now6851/6952functions,2132380/2276920bytes93.7%;101functions/144540bytes remain. Eighteen matches add27704bytes this resumed effort. EntireETC80146400.c is C; worker continues80144224 in80143240.c underT-25061. GitHub check: no openPRs, issue65 unchanged; GEKO/OPTION claim retained. Current plan refreshed to supersede obsolete near-set-only advice after large source reconstructions succeeded.

## [2026-10-22] build | T25052 live-index bounds and unsigned tail dispatch

[[tickets/T-25052-main-schedule-dispatch]] retains defined sequenced late-zero
bounds for only the three loops carrying a live index (984/998words,cost854).
Unsigned tail switch with explicitFF/default restores seven target branch-tree
words (991,cost847); selector identity recovers the missing copy shape but
still has wrong colors (992,cost852). Independent lexical counts enlarge the
frame and are rejected. Production remains assembly; evidence is in
`drafts/T-25052/README.md` and named whole-TU replay controls.

## [2026-10-22] build | MAIN loop and switch checkpoint replayed

Root independently replayed [[tickets/T-25052-main-schedule-dispatch]] late-bounds-unsigned-switch-ff:991/998words,cost847. Preserved d4c458069 and exact loop-bound/selector evidence without production changes. [[tickets/T-25053-bunka-last-function]] transfer test of NAME_ENT's memory-local ordering worsens q-array255/245 and b-array263; current best102 retained.

## [2026-10-22] build | T-25057 literal492-byte candidate

[[tickets/T-25057-name-ent-pass-investigation]]: unsigned subtraction of an unsigned negation preserves the required row-before-tile evaluation. Complete TU123/123 words and private-linked492 bytes MATCH. Removed unnecessary tail test; four necessary FAKE constraints have removal controls. Default C installed and full27 build started. Coordinator direct standards/spec review has no findings, pending final gates.

## [2026-10-22] build | T-25057 default C verified and ready for integration

NAME_ENT func_801398EC passes the default-build literal linked 492-byte proof, full ninja build, fresh 27/27 binary hashes and declaration guards (headers/globals OK; prototypes 0 new / 15 known). Four FAKE constraints and removal controls are preserved in drafts/T-25057; no shared header/compiler or other function changed. Root's direct standards/spec review has no findings. Updated [[playbook]] and [[matching-notes]] with the remainder-range and expression-order mechanism. Moved [[tickets/T-25057-name-ent-pass-investigation]] and [[kanban]] to In Review for root integration; the next candidate will use T-25062.

## [2026-10-22] ticket | T-25062 NAME_ENT byte-pack claim

Created [[tickets/T-25062-name-ent-byte-pack]] In Progress and added its [[kanban]] card before investigation. Scope is only func_8013AF78 in src/ovl/NAME_ENT/80139740.c, continuing after the clean T-25057 match pin. Read [[index]] and function catalog; historical pointer-zero loop and byte-register packing need current-pipeline qualification before any production edit.

## [2026-10-22] query | T-25056 cached assembler control

[[tickets/T-25056-ending-scheduler]] records exact original IDO5.2/stock5.3
equality across1164 candidate instructions on identical captured input, plus
the shared100/10 region gate. Two conditional pointer controls regress692/691.
The raised-budget scratch output preserves all15 sibling instruction bodies;
final sibling B640 independently matches96bytes despite changed section padding.
Source-only reproduction and hashes are in drafts/T-25056; no production change.

## [2026-10-22] ticket | Astra overlay coordinator resumed

[[tickets/T-24510-parallel-overlay-coordination]] resumes on633d953563b1038d65572295e6667d709e8ce1e1 in the clean reused coordinator tree, preserving the old branch. Started fresh agents astra_tel_25055, astra_ending_25056, astra_name_25057 with explicit high Astra authorization and file-disjoint TEL80132000, ENDING801396E0, NAME_ENT80139740 claims. Source confirms all initial functions remain unmatched. Workers create their tickets before research. Two initial compiler slots go to TEL/ENDING; NAME_ENT qualifies source and history pending rotation. Root received actual agent names and exact claims. No production changes, no new match claim, no fourth compiler workload.

## [2026-10-22] query | Astra lanes establish causal reductions

[[tickets/T-24510-parallel-overlay-coordination]] worker reports: TEL T-25055 direct case0x40 global access restores a temporary slot (aligned970 to127); sharing disjoint case6 max/total lifetimes yields1645 target/candidate words and41 changed positions, aligned78. NAME_ENT T-25057 named remainder with unsigned-false late use plus index-first initialization yields exact123words and36 differing words; coordinator read confirms remainder is assigned in the same arm before the discarded test. ENDING T-25056 traces candidate-count budget exhaustion before useful arms; corrected gate old_count>10 allows11 scans, stops on12th, with100 in natural-loop regions. These remain worker-reported nonmatches; source/archive and causal checks continue. Compiler slots rotate between read phases; no coordinator build workload.

## [2026-10-22] build | TEL candidate closes and full checksum gate passes

T-25055 worker reports whole-TU equality1645words after unsigned-false girl read41to1 and hp*1 comparison1to0. Annotated production C then passes default literal6580-byte MATCH and all27 checksums; declaration and ABI removal controls are still pending. Coordinator direct source review found no unresolved issue in initialized total/rank/packed fields or defined identity controls; requested buffer/padding FAKE notes (now present), extra strcat month-argument necessity controls, and removal of unnecessary set_kanji_string pointer-to-integer casts. func_80052060 canonical no-prototype declaration has matched four-s32 definition, supporting its pointer-sized argument casts. No integration claim. NAME_ENT T-25057 reaches123words with only15 positions differing from frame/remainder register swap. ENDING T-25056 scratch global budget100 yields4648-byte literal equality, but stock source remains unmatched; root requested source-only fixes and general-rule evidence before any pipeline proposal. See [[tickets/T-24510-parallel-overlay-coordination]].

## [2026-10-22] build | Verified TEL pin delivered for integration

T-25055 clean pin596c2c269, base633d95356, handed to root independently of other lanes. Final default C literal6580-byte proof, all27 binary hashes and declarations pass. Extra strcat month arguments and two set_kanji_string text casts were independently byte-neutral and removed; four-s32 func_80052060 casts follow its matched definition. Coordinator inspected source, archive scripts/metadata, scoped diff and whitespace; review concerns resolved. Ticket In Review pending root. Final body SHA256 af1569291448dc825dfdf9afd27f32dc299bf67d59782d01d4cc8833e83d5dd9. Root reserved T-25060 continuation, but owned TEL80132000.c has no remaining ASM; requested explicit transfer to TEL8013CCD0.c sole remaining func8013D88C before edits. ENDING source-only checkpoint91e1f4182 is durable, investigation remains active. See [[tickets/T-24510-parallel-overlay-coordination]].

## [2026-10-22] decision | TEL integration accepted and last-function claim transferred

Root independently verified and pushed T-25055 as mainc18c8dcb8; ticket Done. Reported project6850/6952functions,2129572/2276920bytes93.5%,102remaining. TEL64/65,47228/50200progressbytes; remaining2972 differs from func8013D88C symbolspan2980 and worker will verify padding. Root explicitly transferred T-25060 to src/ovl/TEL/8013CCD0.c, sole remaining TEL function. Worker created ticket on codex/t25060 from596c2c269 in reused isolated tree; no shared-root edits. ENDING and NAME_ENT retain their claims and causal investigations. See [[tickets/T-24510-parallel-overlay-coordination]].

## [2026-10-22] query | NAME_ENT closes through unsigned subtraction ordering

T-25057 source-only checkpoint1f0eab46b preserved scalar108/123words and array106/123words. Array restores all allocated registers but uopt emits tile multiply before row divide/multiply. The equivalent unsigned subtraction row-(0U-tile*16)+128 restores row-first emission, full-TU123/123words and literal linked492-byte proof. Unnecessary unsigned tail use removed with neutral control; removing record-use/dead-init/array/subtraction gives72/50/50/34printedrows. Final production body reviewed directly against standards/spec: defined unsigned wrap, bounded i0..12, tile assignment dominates use, four artificial levers marked FAKE, no header/compiler changes. Full27 build now running; no integrated coverage claimed. ENDING cached5.2 shares the same100/10budget, source investigation continues. TELT25060 frame traced native224/final240 vs target224, lifetime/base-view controls in progress. See [[tickets/T-24510-parallel-overlay-coordination]].

## [2026-10-22] build | NAME_ENT final validation passes

T-25057 worker reports final default-C492-byte literal MATCH, successful full ninja-j2, fresh27/27SHA1 and headers/globals/prototypes0new15known. Source unchanged from coordinator and root direct review; pin packaging in progress. Root reserved T-25062 for next NAME_ENT target, qualify func8013AF78 in same file before edits. ENDING cached5.2 dynamic replay equals5.3 at1164words; diagnostic raised budget preserves15 sibling bodies. Worker has no evidence-backed source transformation removing five required scan candidates without changing output, and prepares narrow general-research proposal rather than a blind grid. Root informed of potential ENDING ownership transfer after checkpoint. TELT25060 continues; no new match there. See [[tickets/T-24510-parallel-overlay-coordination]].

## [2026-10-22] ticket | T-25046 continues SHOUGATU EA98

Created [[tickets/T-25046-shougatu-ea98]] and In Progress board card before source edits, preserving clean E6E4 pin ca9569534. Root reviewed [[tickets/T-25038-shougatu-selection-loop]] with no findings and owns its combined integration. Same Astra worker continues EA98, using the corrected284-cost history in [[tickets/T-24523-seasonal-source-hypotheses]] and concrete target/callee evidence before transferring matching controls. The corrected284 body was not committed; older T-17500 archives are a distinct baseline. No duplicate coordinator compiler tests or uncoordinated shared-header changes.

## [2026-10-22] ingest | T-25046 recovers exact EA98 source lineage

Recovered prior private t24523-sho-ea.c plus generator/scorer, then verified target semantics. Corrected copied event byte0x11 to target0x18 and final helper to check_para_limit(). Fresh full-TU baseline is282 aligned/286 printed,232/230words, with frame/table offsets eight bytes too high. Target random word reloads inside retry and shares selector through club tests; draft hoists the random and promotes selector through status call. Two isolated access-class controls test these observed causes. [[tickets/T-25046-shougatu-ea98]] records the evidence; production remains assembly.

## [2026-10-22] build | T-25046 pins optimizer frame growth and direct-call control

Faithful EA98 baseline trace reproduces282 cost and shows local area0x28 before uopt,0x38 afterward, with no surviving accesses to added homes. Per-call void847 cast regresses because it introduces jalr/address setup; a private direct void declaration is identical to baseline. Exact random access views and do-while control are also identical. Volatile random restores ra copy allocation but leaves wrong addressing/ranges, so remains diagnostic. [[tickets/T-25046-shougatu-ea98]] records precise negative controls; next test follows the existing unsigned-load pass entry gate rather than varying declarations.

## [2026-10-22] build | T-25046 identifies hidden homes and shared selector range

A faithful private tracer pins EA98 hidden homes to random/address, flag, selector conversion and previous-selector conversion. Optimized code shares the selector through three loop checks, forced-selection refresh and status RPAR, unlike the target's fresh post-merge load. Removing inherited random multiply-by-one restores its per-retry load and target table-copy registers (215 cost,231 words); call-only byte views do not resolve the selector range. [[tickets/T-25046-shougatu-ea98]] records the focused negative controls and current causal split. No production/compiler changes.

## [2026-10-22] build | T-25046 restores frame and isolates post-call promotion

Combined evidence-led access classes restore EA98 frame0x58, both table homes and230 words. The do-while condition restores the target G-base allocation. A post-status volatile read proves the later comparison extends the loop selector promotion; an existing D_800E71DF scalar alias only there gives48 aligned,230 words, with downstream arithmetic exact. Private847-byte control gives44 but228 words. [[tickets/T-25046-shougatu-ea98]] records all controls. Coordinator requires literal match plus keep necessity proof before any alias config/source promotion; production remains unchanged.

## [2026-10-22] build | T-25046 count-loop sequencing and tied ranges

Postincrement indexing fixes the complete EA98 count-loop schedule and gives38 aligned differences at230words, exact frame/tables. Faithful44-cost trace identifies selector range21/150 and equal2.2 priorities for previous choice, G-base and constant9; the missing common latch remains the main causal question. Created drafts/T-25046/README.md with current candidate path, trace hash and negative-family ledger. [[tickets/T-25046-shougatu-ea98]] remains In Progress; no production changes or coverage.

## [2026-10-22] build | T-25046 confirms latch and allocation interaction

For-loop retry increment restores the target previous-choice/constant9/G-base registers, but only addiu hoists early; signed narrowing stays at the latch. A coordinator-suggested word counter with explicit s16 updates retains that problem. Faithful late-zero trace proves its producer survives uopt, yet tested latch forms change promotion or disappear without the required reload. [[tickets/T-25046-shougatu-ea98]] and drafts/T-25046/README.md record the exact negative controls. Preserved best remains38 differences/230words; production and neighboring matches unchanged.

## [2026-10-22] build | T-25046 unsigned recurrence and latch phase evidence

[[tickets/T-25046-shougatu-ea98]] preserves36 differences/230 words with exact frame and tables after u32 status reuse plus signed16 normalization restores sum operand order. Faithful for-latch trace shows optimized ADD/CVTL/CVT remain together at the latch; as1 alone moves ADD early. Union storage, redundant normalization and selector-zero arithmetic controls regress and are not promoted. Updated drafts/T-25046/README.md; production remains ASM and no external blocker exists.

## [2026-10-22] build | T-25046 first-rejection availability boundary

[[tickets/T-25046-shougatu-ea98]] records rejected CFG/late-identity/read controls and corrects an invalid old local-selector scratch negative (accidental forced-store replacement; production unaffected). Corrected controls remain negative. Target first rejection bypasses the t4 load, while current uopt RLOD makes t4 available there; tracing that availability boundary now. Best remains36/230, exact frame/tables.

## [2026-10-22] ticket | Claim final TEL menu builder

[[tickets/T-25060-tel-date-menu]] starts In Progress from clean596c2c269 on codex/t25060 in /private/tmp/tokimemo-t25024. Root explicitly transferred exclusive scope to src/ovl/TEL/8013CCD0.c:func_8013D88C after the preclaim audit showed func_80134FE4 already default C and former TU fully matched. Read T-24039/T-24511 semantic corrections and negative controls; baseline755 versus745 words,232 versus224-byte frame. No compiler run or source change yet; slot yielded while preparing the new replay.

## [2026-10-22] build | T-25060 baseline and packed-word lifetime control

[[tickets/T-25060-tel-date-menu]] replayed the preserved T-24511 full-TU source: 755/745 words, 689 changed positions, aligned cost 798, final frame 240/224. Native allocation shows flags already in v1 and a separate saved x/packed-word range in s2; old notes about a saved flag pointer and frame 232 describe prior controls. Reusing dead arg0 for packed-word capture gives 751 words and cost 786; narrowing coordinate x is neutral. Sources and generator are under drafts/T-25060; private object/trace evidence stays under build/T-25060. No production source changed.

## [2026-10-22] build | T-25060 independent allocation and multiply controls

[[tickets/T-25060-tel-date-menu]] isolated pointer/index false-reads and negative-literal multiplication after separating packed flags from coordinates. Index read reaches747 words (cost804); negative-scale arg0 control755 (cost770) restores a target multiply macro. None matches. The next concrete hypothesis is signed/unsigned packed-word alias identity: three signed loads alone fail to share the target GameState base. Preserved generators are in drafts/T-25060/lifetime_controls.py; no production edits. Compiler slot yielded after this bounded stage.

## [2026-10-22] build | T-25060 address-sharing hypothesis narrowed

[[tickets/T-25060-tel-date-menu]] found three unsigned-load controls byte-neutral. An explicit page-one GameState pointer restores common-base loads but expands the function to776 words. The combined prior allocation reads produce756 words; native trace capture is examining their interaction. Baseline frame terminology corrected: native224, final240 after the uniform pass, target final224. No production or shared tooling changed.

## [2026-10-22] ticket | T-25061 next ETC continuation

Created [[tickets/T-25061-etc-next-match]] before experiments, preserving the completed T-25059 pin. The current file is complete; qualify up to three remaining ETC functions against specific ledgers and announce one new file claim before source edits. Root handles integration.

## [2026-10-22] decision | T-25061 selects ETC record-scene controller

Compared three specific histories and claimed func_80144224 in src/ovl/ETC/80143240.c; parent confirmed the claim before source edits. T-25002 retains the closest concrete body with two global-promotion gaps. [[tickets/T-25061-etc-next-match]] records the selection and 780-byte body span; initializer and polygon-renderer controls will not be repeated.

## [2026-10-22] build | T-25061 current full-TU baseline reproduced

The T-25002 draft now measures 199 target / 190 candidate words, aligned cost 175. Resolved comparison includes four target alignment words after the 780-byte body. Historical counts are not directly interchangeable. [[tickets/T-25061-etc-next-match]] will trace frame-counter versus order-index lifetimes; no production source changed.

## [2026-10-22] query | T-25042 late-edge lifetime checks

Unsigned aliases do not isolate the shared store's promotion; a late-zero index retains an extra address instruction. Signed nonnegative tests survive into as1 but remain in final code, and nested case-label or neutral-assignment forms do not preserve the required harmless edge. Added the measured negative classes to drafts/T-25042/README.md for [[tickets/T-25042-kangei-dispatch-emission]]. No production source change or match claimed.

## [2026-10-22] query | T-25042 staged selector allocation

A staged byte conversion restores the172-word uncast stream with a full-width local, isolating the two lost instructions from earlier widening tests. Conditional neutral identities still disappear too early; impossible byte-selector cases retain extra dispatch code. Recorded the causal controls in drafts/T-25042/README.md for [[tickets/T-25042-kangei-dispatch-emission]]. Production source remains unchanged.

## [2026-10-22] query | T-25042 exact live-register conflict

A print-only as1 replay reproduces stock target instructions and identifies the cast default shift's t1 bit as the precise conflict with dispatch live-in. Whole-switch loop, high-bit and translated-selector controls retain changed code or allocation. Preserved measured outcomes in drafts/T-25042/README.md for [[tickets/T-25042-kangei-dispatch-emission]]; no production source change.

## [2026-10-23] ticket | T-25100 wave 8 conveyor opened

User request "Finish the game decomp" (ultracode). [[tickets/T-25100-wave-8-finish]] and package tickets T-25101..T-25153 created: 53 file-disjoint packages, 92 functions (GEKO and OPTION stay reserved for issue #65). One deep Opus pass per function from the best known body, Opus closer for guards of 25 rows or fewer, Sonnet review and merge3 with literal default-C checks.

## [2026-10-23] ticket | T-25106 Done

Reviewed [[tickets/T-25106-w8-TAIIKU-80133C80]] against CODING_STANDARDS.md; matched TAIIKU `func_80137C44` (612 bytes) and `func_80135F3C` (1,676 bytes) as default-build C, 27/27.
`func_80137784` stays guarded at 4 rows (notes in drafts/T-25106/TAIIKU/func_80137784/); the review added FAKE markers to the record-46 and runner-index views.

## [2026-10-23] ticket | T-25200 wave 9 neighbor conveyor opened

User asked for neighbor work next to wave 8. [[tickets/T-25200-wave-9-neighbor]] takes untouched wave 8 packages from the tail of the queue and runs near-miss closers for merged wave 8 packages under [[tickets/T-25201-wave-9-closers]]. Job picking and package blocking go through one lock shared with the wave 8 worktree script.

## [2026-10-23] ticket | T-25105 Done

Reviewed [[tickets/T-25105-w8-main-80053650]] against CODING_STANDARDS.md; matched main `func_80054E6C` (340 bytes), `func_80054FC0` (796) and `func_8005557C` (1,212) as default-build C, 27/27.
`func_80055AFC` stays INCLUDE_ASM at 113 rows (notes in drafts/T-25105/main/func_80055AFC/); the review marked the checksum address forms as FAKE.

## [2026-10-23] ticket | T-25107 Done

Reviewed [[tickets/T-25107-w8-MASTER-80133680]] against CODING_STANDARDS.md; matched MASTER `func_801363F4` (2,608 bytes) and `func_801344E0` (2,912 bytes) as default-build C, 27/27.
Both use the marked FAKE never-true arm before the first record copy; ENDING `func_80134270` has the same symptom (see wiki/playbook.md section 10).

## [2026-10-23] ticket | T-25103 Done

Reviewed [[tickets/T-25103-w8-main-800674B0]] against CODING_STANDARDS.md; matched main `func_8006C988` (276 bytes), `func_8006A044` (648 bytes) and `func_80067F04` (2,452 bytes) as default-build C, 27/27.
`func_80067870` (75 rows) and `func_8006B0C8` (130 rows) stay `INCLUDE_ASM` with notes in drafts/T-25103/main/; the review added a FAKE marker for the `gs` split in `func_80067F04`.

## [2026-10-23] ticket | T-25104 Done

Reviewed [[tickets/T-25104-w8-main-8004C3F0]] against CODING_STANDARDS.md; matched main `mod_set` (780 bytes) and `srn_tpage_show` (3224 bytes) as default-build C, 27/27; the review moved the `func_80098B80` prototype into `src/main/8004C3F0.c` so only one C file changes.
`mod_set_gpu` stays guarded at 4 rows and `mod_trans_vram` stays `INCLUDE_ASM` at 47 rows, with notes in drafts/T-25104/main/.

## [2026-10-23] build | T-25201 w9c-TAIIKU-80133C80
No match: TAIIKU `func_80137784` stays guarded at 4 rows (the two `bne` operand orders); the NON_MATCHING comment now names T-25201 and the uopt rule behind it.
Reviewed against `CODING_STANDARDS.md`, clean rebuild 27/27; next step (second symbol names in `config/symbols_TAIIKU.txt`) needs approval, see [[tickets/T-25201-wave-9-closers]] and [[matching-notes]].

## [2026-10-23] ticket | T-25153 Done
Reviewed [[tickets/T-25153-w8-NAME_ENT-80139740]] against CODING_STANDARDS.md; no matches, 27/27 clean rebuild. The T-24132 guard on NAME_ENT `func_8013AF78` (99 rows) is plain `INCLUDE_ASM` again.
Best bodies (57 and 61 rows, two FAKEs) are in drafts/T-25153/NAME_ENT/func_8013AF78/ with the `$v0` blocker and next hypothesis in NOTES.md.

## [2026-10-23] ticket | T-25101 Done

Reviewed and merged w8-main-8004F870: matched main `sndi`, `func_8005215C`, `get_h_tokimeki`, `get_near_menu`, `func_8004FC10` (7508 bytes, default C, 27/27).
`func_80052F68` stays INCLUDE_ASM (69 rows, notes in drafts/T-25101/main/func_80052F68/NOTES.md); see [[tickets/T-25101-w8-main-8004F870]].

## [2026-10-23] build | T-25201 w9c-main-8004C3F0
Matched: main `mod_set_gpu` (804 bytes) as default-build C, guard removed; for-loop bounds read from the record, a FAKE dead store `idx = 0;` (T-25201) and an `xpos` local.
Reviewed against `CODING_STANDARDS.md` (no findings), clean rebuild 27/27, `mod_set` still matches; see [[tickets/T-25201-wave-9-closers]] and [[matching-notes]].

## [2026-10-23] ticket | T-25152 Done

Reviewed [[tickets/T-25152-w8-TACO-80138890]] against CODING_STANDARDS.md; matched TACO `func_80138B2C` (476 bytes) as default-build C, 27/27.
The review marked the unused K&R parameter FAKE; the clamp-arm zero term and the two address views were already marked.

## [2026-10-23] ticket | T-25102 Done

Reviewed and merged w8-TAIIKU-80138950: matched TAIIKU `func_80139AB0`, `func_8013E894`, `func_8013DDB8`, `func_8013F3A4`, `func_8013E1FC`, `func_8013EC60` (7796 bytes, default C, 27/27).
All six functions of the package are matched, none left with rows; see [[tickets/T-25102-w8-TAIIKU-80138950]].

## [2026-10-23] ticket | T-25151 Done

Reviewed [[tickets/T-25151-w8-GYOZI-80140C90]] against CODING_STANDARDS.md; matched GYOZI `func_801412A4` (684 bytes) as default-build C, 27/27.
No review findings: the two fakematches (the `(s32)` view of the random word, the count-loop body on the `for` line) already carry `FAKE`, the reason and the ticket.

## [2026-10-23] ticket | T-25108 Done

Reviewed [[tickets/T-25108-w8-main-80062CD0]] against CODING_STANDARDS.md; matched main `func_80065964` (424 bytes) and `func_8006509C` (1,076 bytes) as default-build C, 27/27.
`func_80065B0C` stays INCLUDE_ASM behind a NON_MATCHING guard at 9 rows (notes in drafts/T-25108/main/func_80065B0C/).

## [2026-10-23] ticket | T-25109 Done

Reviewed [[tickets/T-25109-w8-EN_NICHI-80136B10]] against CODING_STANDARDS.md; matched EN_NICHI `func_8013735C` (2,300 bytes) as default-build C, 27/27.
`func_80136DEC` stays guarded NON_MATCHING at 4 aligned rows (two dispatch delay-slot fills; notes in drafts/T-25109/EN_NICHI/func_80136DEC/).

## [2026-10-23] ticket | T-25110 Done

Reviewed [[tickets/T-25110-w8-TACO-8013DB80]] against CODING_STANDARDS.md; matched TACO `func_8013DEE4` (1,344 bytes) as default-build C, 27/27.
`func_8013EAA4` stays `INCLUDE_ASM` behind a NON_MATCHING body at 4 rows (notes in drafts/T-25110/TACO/func_8013EAA4/); no review findings.

## [2026-10-23] ticket | T-25150 Done
Reviewed against `CODING_STANDARDS.md` (no findings), clean rebuild 27/27, funcdiff MATCH. Matched: KANGEI `func_80134724` (688 bytes) as default C, FAKE cast dropped, by defining the object's own scene table `D_80139B30` in C ([[tickets/T-25150-w8-KANGEI-80134120]], [[matching-notes]]).
No function left with rows in this package; the data-island lead for other switches is in `drafts/T-25150/KANGEI/func_80134724/NOTES.md`.

## [2026-10-23] ticket | T-25149 Done

Reviewed [[tickets/T-25149-w8-NAME_ENT-80132000]] against CODING_STANDARDS.md; matched NAME_ENT `func_80138C1C` (740 bytes) as default-build C, 27/27.
No findings; the three FAKEs (0x20 buffer, `& 3` on the two-bit date fields, s32 return for the caller) already carry reasons and the ticket.

## [2026-10-23] ticket | T-25148 Done

Reviewed [[tickets/T-25148-w8-ENDING-80137340]] against CODING_STANDARDS.md; matched ENDING `func_801387B0` (748 bytes) as default-build C, 27/27.
No review findings: the three fakematches (four test spellings, `* 1` on two reads, the D_8013CAA4 view of D_8013CAA0) already carry `FAKE`, the reason and the ticket; no rows left in the package.

## [2026-10-23] ticket | T-25111 Done

Reviewed [[tickets/T-25111-w8-TACO-8013B990]] against CODING_STANDARDS.md; matched TACO `func_8013C6D4` (1388 bytes) and `func_8013BAC4` (1840 bytes) as default-build C, 27/27; the review added two comments for non-obvious tricks in `func_8013BAC4`.
Both functions of `src/ovl/TACO/8013B990.c` are now C, notes in drafts/T-25111/TACO/ and [[matching-notes]].

## [2026-10-23] ticket | T-25113 Done

Reviewed and merged w8-ETC-8013BEF0: matched ETC `func_8013F054` (716 bytes) and `func_8013D570` (2352 bytes), default C, 27/27.
Both functions of the package are matched, none left with rows; see [[tickets/T-25113-w8-ETC-8013BEF0]].

## [2026-10-23] ticket | T-25112 Done
Reviewed [[tickets/T-25112-w8-TACO-80139170]] against CODING_STANDARDS.md; no findings, clean rebuild 27/27. Matched TACO `func_80139FB8` (1480 bytes, plain C, no FAKE) and `func_80139424` (1676 bytes, default C, FAKEs T-25112: dead `x3 = 0;`, `pad[2]`).
Lessons are in [[matching-notes#T-25112: TACO func_80139FB8]] and [[matching-notes#T-25112: TACO func_80139424]] (for loop instead of m2c do/while; vertex written as centre + (offset -/+ half)).

## [2026-10-23] build | T-25201 w9c-main-80062CD0
Reviewed against `CODING_STANDARDS.md` (one formatting finding in `wiki/playbook.md`, fixed), clean rebuild 27/27, funcdiff MATCH. Matched: main `func_80065B0C` (1064 bytes) as default C with one FAKE (never-true indirect-store arm that puts the else arm in the `$s5` range).
No function left with rows in this job; notes in `drafts/T-25108/main/func_80065B0C/NOTES.md`, [[matching-notes]], [[playbook]].

## [2026-10-23] build | T-25201 w9c-EN_NICHI-80136B10
Matched EN_NICHI `func_80136DEC` (1,392 bytes) as default-build C, guard removed, 27/27, review without findings ([[tickets/T-25201-wave-9-closers]]).
The scene table `D_80139C5C`, which names the function, is now defined in C, so as1 links the case-3 jump-table `jr` back to the entry and fills the D1/D2 delay slots as the original does ([[as1-scheduler-rule]], [[matching-notes]]).

## [2026-10-23] build | T-25201 w9c-TACO-8013DB80

Reviewed [[tickets/T-25201-wave-9-closers]] job w9c-TACO-8013DB80; matched TACO `func_8013EAA4` (1964 bytes) as default-build C, 27/27 (`src/ovl/TACO/8013DB80.c`).
One review finding fixed: the steering local `half` now carries a `FAKE` comment; `pad[8]` and `pad_a` stay FAKE.

## [2026-10-23] ticket | T-25147 Done

Reviewed and merged w9-main-80058D20: matched main `func_8005938C` (764 bytes, default C, one FAKE never-true arm from the T-25107 recipe), 27/27.
The only function of the package is matched, none left with rows; see [[tickets/T-25147-w8-main-80058D20]] and [[matching-notes]].

## [2026-10-23] ticket | T-25115 Done

Reviewed [[tickets/T-25115-w8-main-800490C0]] against CODING_STANDARDS.md; matched main `_sprite_set_light_effect1` (1,124 bytes) and `func_80049A40` (1,456 bytes) as default-build C, 27/27.
One finding fixed (FAKE comments on the `gs` alias and `channel * 0` black vertices in `func_80049A40`); notes in drafts/T-25115/main/.

## [2026-10-23] ticket | T-25114 Done

Reviewed [[tickets/T-25114-w8-RPG_BAT-80154430]] against CODING_STANDARDS.md; matched RPG_BAT `func_8015528C` (1,500 bytes) as default-build C, 27/27.
`func_80154DD8` stays INCLUDE_ASM behind a NON_MATCHING guard at 34 rows, down from 102 (notes in drafts/T-25114/RPG_BAT/func_80154DD8/).

## [2026-10-23] ticket | T-25117 Done

Reviewed [[tickets/T-25117-w8-VALEN-80132760]] against CODING_STANDARDS.md; matched VALEN `func_80133088` (1108 bytes, two FAKEs) and `func_80132834` (1240 bytes, no FAKE) as default-build C, 27/27.
Both functions of `src/ovl/VALEN/80132760.c` are now C, notes in drafts/T-25117/VALEN/ and [[matching-notes]].

## [2026-10-23] ticket | T-25116 Done

Reviewed [[tickets/T-25116-w8-main-80047550]] against CODING_STANDARDS.md; matched main `func_80047560` (352 bytes) as default-build C, 27/27.
`func_800476C0` stays `INCLUDE_ASM` at 649 diff lines (notes in drafts/T-25116/main/func_800476C0/); the unneeded `include/game.h` change was reverted in review.

## [2026-10-23] ticket | T-25146 Done
Matched ETC `func_80144224` (796 bytes) as default-build C, 27/27; review found one issue (needless `s32` return without a return statement, now `void` with an ETC.h prototype) and it is fixed ([[tickets/T-25146-w8-ETC-80143240]]).
The frame counter read through a `GsWord *` local is the lever that fixed the register ranges; two FAKE word views (quot, v) as in `func_80143A88` ([[matching-notes]]).

## [2026-10-23] ticket | T-25145 Done
Reviewed against `CODING_STANDARDS.md` (one formatting finding in `wiki/matching-notes.md`, fixed), clean rebuild 27/27, funcdiff MATCH. Matched ENDING `func_801330E4` (796 bytes) as default C in `src/ovl/ENDING/80133030.c` with two FAKEs (`pad[4]`, an empty test on the f9 word).
No function left with rows in this job; notes in `drafts/T-25145/ENDING/func_801330E4/NOTES.md`, [[matching-notes]], [[tickets/T-25145-w8-ENDING-80133030]].

## [2026-10-23] ticket | T-11900 In Progress (claim #65)

Outside-contributor claim CosmicScribe64/tokimemo-ps1-decomp#65 (posted 2026-10-20): the 11 GEKO and 3 OPTION functions left at `ff6c770b2`. Upstream has since matched GEKO `func_80134808`, `func_80133308`, `func_80144198`, `func_801420E4` and OPTION `func_80139A30`, `func_8013A7EC`; the branch keeps upstream's versions of those. OPTION `func_801326BC` matched on this branch. [[tickets/T-11900-geko-option-closers]].

## [2026-10-23] ticket | T-25119 Done
Reviewed [[tickets/T-25119-w8-main-80061710]] against CODING_STANDARDS.md; two comments were missing the `FAKE` marker (pointer cast, work-area view) and are fixed; clean rebuild 27/27. Matched main `func_80062948` (736 bytes) and `func_80061A3C` (1216 bytes) as default C.
Both functions of `src/main/80061710.c` are C now; notes in drafts/T-25119/main/ and [[matching-notes]].

## [2026-10-23] ticket | T-25144 Done

Reviewed and merged w9-RPG_BAT-8014ADD0: matched RPG_BAT `func_8014BFE4` (892 bytes, default C, 128 rows to 0; FAKEs: `t[1]`, one-line pre-loop statement, empty first arm now marked), 27/27.
The only function of the package is matched, none left with rows; see [[tickets/T-25144-w8-RPG_BAT-8014ADD0]] and [[matching-notes]].

## [2026-10-23] ticket | T-25118 Done

Reviewed [[tickets/T-25118-w8-main-800563F0]] against CODING_STANDARDS.md; matched main `func_80056BA8` (1296 bytes) as default-build C, 27/27; the review added a comment on its missing return.
`strCallback` stays INCLUDE_ASM (75 rows from the copy-state shim); candidate tool rule in drafts/T-25118/main/strCallback/ for a tooling ticket.

## [2026-10-23] ticket | T-25123 Done

Reviewed [[tickets/T-25123-w8-main-800451D0]] against CODING_STANDARDS.md; matched main `func_800455C4` (2,768 bytes) as default-build C, 27/27.
The guard in `src/main/800451D0.c` is gone; FAKEs (D_801255B5 via the B4 symbol, `* 1` loop test) are ticketed, notes in [[matching-notes]] and [[playbook]].

## [2026-10-23] ticket | T-25143 Done

Reviewed [[tickets/T-25143-w8-SHOUGATU-8013E330]] against CODING_STANDARDS.md: no source change, no findings; SHOUGATU `func_8013EA98` not matched (42 rows, over the guard limit), source stays `INCLUDE_ASM`.
Body, unit-level allocator trace findings and next hypothesis kept in drafts/T-25143/SHOUGATU/func_8013EA98/ and [[matching-notes]].

## [2026-10-23] ticket | T-25121 Done

Reviewed [[tickets/T-25121-w8-ETC-80132880]] against CODING_STANDARDS.md; matched ETC `func_801328A4` (920 bytes) as default-build C, 27/27.
`func_80132C3C` stays INCLUDE_ASM behind a NON_MATCHING guard at 15 rows, down from 105 (notes in drafts/T-25121/ETC/func_80132C3C/).

## [2026-10-23] ticket | T-25141 Done

Reviewed [[tickets/T-25141-w8-TACO-8013F250]] against CODING_STANDARDS.md (no findings), clean rebuild 27/27, funcdiff MATCH. Matched TACO `func_8013FBF0` (1040 bytes) as default C in `src/ovl/TACO/8013F250.c` with two FAKEs (`pad[6]`, one-line UV stores).
The only function of the package is matched, none left with rows; notes in `drafts/T-25141/TACO/func_8013FBF0/NOTES.md` and [[matching-notes]].

## [2026-10-23] ticket | T-25142 Done
Reviewed against `CODING_STANDARDS.md` (one formatting finding in `wiki/matching-notes.md`, fixed), clean rebuild 27/27. No function matched: BUNKA_SD `func_80134D94` stays `INCLUDE_ASM` (old T-24137 guard removed) with its draft at aligned cost 12, 45 positional rows, in `drafts/T-25142/BUNKA_SD/func_80134D94/`.
No function matched in this job; notes in `drafts/T-25142/BUNKA_SD/func_80134D94/NOTES.md`, [[matching-notes]], [[tickets/T-25142-w8-BUNKA_SD-80134540]].

## [2026-10-23] ticket | T-25140 Done

Reviewed [[tickets/T-25140-w8-main-8004AD60]] against CODING_STANDARDS.md (no findings), clean rebuild 27/27, funcdiff MATCH. Matched main `make_color_bar` (1072 bytes) as default C in `src/main/8004AD60.c`, no FAKE.
The only function of the package is matched, none left with rows; notes in `drafts/T-25140/main/make_color_bar/NOTES.md` and [[matching-notes]].

## [2026-10-23] ticket | T-25124 Done
Reviewed [[tickets/T-25124-w8-main-80046500]] against CODING_STANDARDS.md; the goto FAKE comment now also covers the comma UV groups and a duplicate draft copy of the body was removed; clean rebuild 27/27, funcdiff MATCH. Matched main `func_80046AC8` (2696 bytes) as default C in `src/main/80046500.c`.
The only function of the package is C now; notes in `drafts/T-25124/main/func_80046AC8/NOTES.md` and [[matching-notes]].

## [2026-10-23] ticket | T-25120 Done

Reviewed [[tickets/T-25120-w8-GYOZI-80138B50]] against CODING_STANDARDS.md; matched GYOZI `func_8013907C` (844 bytes) and `func_80138C9C` (992 bytes) as default-build C.
Findings fixed (CandList offset comments, catalog link, kanban column); FAKEs are marked; notes in drafts/T-25120/GYOZI/.

## [2026-10-23] ticket | T-25126 Done

Reviewed [[tickets/T-25126-w8-ENDING-80134D20]] against CODING_STANDARDS.md; matched ENDING `func_80135F4C` (3220 bytes) as default-build C, 27/27.
The guard is gone; the fix was defining the object's `.data` table `D_8013CA3C` in C (T-25150 as1 rule), lead for `func_80134270` and `func_8013A2E8` in [[matching-notes]].

## [2026-10-23] build | T-25201 w9c-ETC-80132880

Reviewed w9c-ETC-80132880 against CODING_STANDARDS.md (no findings), clean rebuild 27/27, funcdiff MATCH on both versions. Matched ETC `func_80132C3C` (884 bytes) as default C in `src/ovl/ETC/80132880.c` with a never-true `p1->code = 0x20` arm (FAKE) that raises the priority of 0x20.
The only job of the package is matched, none left with rows; notes in `drafts/T-25121/ETC/func_80132C3C/NOTES.md`, [[matching-notes]] and [[playbook]].

## [2026-10-23] ticket | T-25122 Done

Reviewed [[tickets/T-25122-w8-main-80049FF0]] against CODING_STANDARDS.md; matched main `func_8004A414` (1,024 bytes, one FAKE) as default-build C, 27/27.
`func_8004A1A0` stays INCLUDE_ASM at 64 rows, down from 89 (notes in drafts/T-25122/main/func_8004A1A0/).

## [2026-10-23] ticket | T-25127 Done

Reviewed [[tickets/T-25127-w8-ENDING-801396E0]] against CODING_STANDARDS.md; matched ENDING `func_8013A2E8` (4648 bytes) as default-build C, 27/27.
The guard in `src/ovl/ENDING/801396E0.c` is gone: defining the scene table D_8013CB74 in C (data island, `config/overlays/ENDING.yaml`) gave as1 the jump-table back edge; notes in [[matching-notes]] and [[as1-scheduler-rule]].

## [2026-10-23] ticket | T-25125 Done

Reviewed [[tickets/T-25125-w8-main-8006CB30]] against CODING_STANDARDS.md: no source change, nothing matched, 27/27 build; no findings.
`func_8007132C` stays INCLUDE_ASM (738 rows from 950); notes and next hypothesis in drafts/T-25125/main/func_8007132C/NOTES.md.

## [2026-10-23] ticket | T-25138 Done

Reviewed [[tickets/T-25138-w8-RPG_BAT-801447E0]] against CODING_STANDARDS.md (two small findings fixed), clean rebuild 27/27, funcdiff MATCH. Matched RPG_BAT `func_80144C84` (1276 bytes) as default C in `src/ovl/RPG_BAT/801447E0.c`; FAKEs: file-local `RpgBattleView` override of `D_8015EBCC` and the comma return.
The only function of the package is matched, none left with rows; the real battle-block aggregate is the next step, notes in `drafts/T-25138/RPG_BAT/func_80144C84/NOTES.md` and [[matching-notes]].

## [2026-10-23] ticket | T-25139 Done

Reviewed [[tickets/T-25139-w8-KANGEI-80132090]] against CODING_STANDARDS.md (no findings), clean rebuild 27/27, funcdiff MATCH. Matched KANGEI `func_801327DC` (1096 bytes) as default C in `src/ovl/KANGEI/80132090.c`; FAKEs: two empty tests of the club halfword and a `(u8)` narrowing; the scene table `D_801399B0` now defined in C (data island).
The only function of the package is matched, none left with rows; notes in `drafts/T-25139/KANGEI/func_801327DC/NOTES.md` and [[matching-notes]].

## [2026-10-23] ticket | T-25130 Done

Reviewed [[tickets/T-25130-w8-MASTER-80132140]] against CODING_STANDARDS.md; matched MASTER `func_801326B4` (3264 bytes) as default-build C, 27/27.
Findings fixed (u32 copy marked FAKE, blank line, kanban column); the six always-false mode tests are FAKE; notes in drafts/T-25130/MASTER/func_801326B4/.

## [2026-10-23] ticket | T-25137 Done

Reviewed w9-TACO-8013B170 against CODING_STANDARDS.md (no open findings; the `#line` pair per quad is an accepted marked FAKE), clean rebuild 27/27, funcdiff MATCH. Matched TACO `func_8013B460` (1328 bytes) as default C in `src/ovl/TACO/8013B170.c`.
The package's only function is matched; the as1 line tie-break rule is in [[as1-scheduler-rule]] and [[playbook]], notes in `drafts/T-25137/TACO/func_8013B460/NOTES.md`, see [[tickets/T-25137-w8-TACO-8013B170]].

## [2026-10-23] ticket | T-11900 Done

Four matches: OPTION `func_801326BC`, GEKO `func_80136C20`, `func_801367EC` (both closed by defining `D_80145094` in C, [[as1-scheduler-rule]]) and `func_80135DC0`. `func_80133024`, `func_80132B24` keep their guards; `func_80142C9C` checkpoint in `drafts/T-11900`. Inline review against CODING_STANDARDS.md (FAKE comments, no inline asm). Notes in [[matching-notes]]. [[tickets/T-11900-geko-option-closers]].

## [2026-10-23] ticket | T-25129 Done

Reviewed [[tickets/T-25129-w8-GYOZI-80136220]] against CODING_STANDARDS.md; matched GYOZI `func_801368B8` (2256 bytes) as default-build C, 27/27, funcdiff MATCH.
One FAKE (dead entry read, marked); finding fixed (comment for the always-true `| 8` in case 6); lead for SHOUGATU `func_801350F8` in [[matching-notes]].

## [2026-10-23] ticket | T-25128 Done

Reviewed [[tickets/T-25128-w8-ETC-8013FF80]] against CODING_STANDARDS.md; no function matched, `src/ovl/ETC/8013FF80.c` unchanged (`func_8013FFAC` stays `INCLUDE_ASM`, 0 bytes added).
Best draft is 197 funcdiff rows (346 before), notes and next hypothesis in `drafts/T-25128/ETC/func_8013FFAC/NOTES.md` and [[matching-notes]].

## [2026-10-23] ticket | T-25300 wave 10 second pass opened

The wave 9 neighbor run ([[tickets/T-25200-wave-9-neighbor]]) stopped when a job-picker agent returned no structured result; its last pick had found no untouched packages left. [[tickets/T-25300-wave-10-second-pass]] replaces it: a second deep pass, from the first pass's notes, on each of the 20 non-GEKO files still holding INCLUDE_ASM (tickets T-25301..T-25320), starting each file as soon as no wave 8 or wave 9 worktree holds it.

## [2026-10-23] ticket | T-25136 Done

Reviewed w9-TACO-801447D0 against CODING_STANDARDS.md (no open findings; the empty `if (x + y > 0) {}` is a marked FAKE), clean rebuild 27/27, funcdiff MATCH. Matched TACO `func_80145650` (1908 bytes) as default C in `src/ovl/TACO/801447D0.c`.
The package's only function is matched, none left with rows; the block-copy state method is in [[playbook]] and [[matching-notes]], notes in `drafts/T-25136/TACO/func_80145650/NOTES.md`, see [[tickets/T-25136-w8-TACO-801447D0]].

## [2026-10-23] ticket | T-25134 Done

Reviewed [[tickets/T-25134-w8-main-80043510]] against CODING_STANDARDS.md; matched main `palette_load_vram` (2024 bytes) as default-build C, 27/27.
One finding fixed (comment on the empty `case 1: default:`); the two stack-frame locals are FAKE and ticketed; notes in drafts/T-25134/main/palette_load_vram/.

## [2026-10-23] ticket | T-25131 Done

Reviewed [[tickets/T-25131-w8-NAME_ENT-80142A80]] against CODING_STANDARDS.md; no matches (`src/` unchanged, `func_80143580` stays INCLUDE_ASM).
`func_80143580` is down to 471 rows from 835 (draft and notes in drafts/T-25131/NAME_ENT/func_80143580/).

## [2026-10-23] ticket | T-25133 Done

Reviewed [[tickets/T-25133-w8-TEL-8013CCD0]] against CODING_STANDARDS.md; matched TEL `func_8013D88C` (2980 bytes) as default-build C, 27/27; the guard and `INCLUDE_ASM` are gone, no function left in `src/ovl/TEL/8013CCD0.c`.
Review finding fixed: the empty month-bypass arm and the g/D_800E6280 split are now marked FAKE; levers and negatives in `drafts/T-25133/TEL/func_8013D88C/NOTES.md` and [[matching-notes]].

## [2026-10-23] ticket | T-25303 Done

Reviewed [[tickets/T-25303-w10-main-800563F0]] against CODING_STANDARDS.md; matched main `strCallback` (640 bytes) as default-build C, 27/27, funcdiff MATCH; `func_80056BA8`, `strNext`, `strInit` still MATCH.
The match is the T-25118 body plus a marked never-true istr arm that sets the shim's sticky F state (no tooling change); notes in [[matching-notes]] and `drafts/T-25303/main/strCallback/NOTES.md`.

## [2026-10-23] ticket | T-25132 Done

Reviewed [[tickets/T-25132-w8-SHOUGATU-80134890]] against CODING_STANDARDS.md (two comment fixes); matched SHOUGATU `func_801350F8` (2308 bytes, two FAKEs) as default-build C, 27/27.
The INCLUDE_ASM in `src/ovl/SHOUGATU/80134890.c` is gone; levers and the GYOZI `func_801368B8` lead in [[matching-notes]] and [[playbook]].

## [2026-10-23] ticket | T-25304 Done

Reviewed [[tickets/T-25304-w10-main-8004C3F0]] against CODING_STANDARDS.md; matched main `mod_trans_vram` (752 bytes) as default-build C, 27/27, funcdiff MATCH, no findings.
The cause was a `u8 flags` local that makes uopt colour `t0` (recipe in [[playbook]]); notes in `drafts/T-25304/main/mod_trans_vram/NOTES.md` and [[matching-notes]].

## [2026-10-23] ticket | T-25100, T-25200, T-25201 Done; status 98.6%

Wave 8 finished (34 packages merged, 57 verified matches) and wave 9 stopped (20 verified matches from 17 packages and 6 closer jobs). With PR #68, main is 6,932/6,952 functions (98.6%). [[status]] lists the 20 functions left; 16 are in the wave 10 second pass ([[tickets/T-25300-wave-10-second-pass]]), 4 are GEKO (issue #65). T-25135 is superseded by T-25311.

## [2026-10-23] ticket | T-25302 Done

Reviewed [[tickets/T-25302-w10-main-80049FF0]] against CODING_STANDARDS.md; matched main `func_8004A1A0` (628 bytes) as default-build C, 27/27, funcdiff MATCH, no findings.
The cause was a pointer local defined before the late uopt block split, which lost `.noalias`; one FAKE (the empty `if (slot) {}` split). Notes in `drafts/T-25302/main/func_8004A1A0/NOTES.md` and [[matching-notes]].

## [2026-10-23] ticket | T-25301 Done

Reviewed [[tickets/T-25301-w10-NAME_ENT-80139740]] against CODING_STANDARDS.md; matched NAME_ENT `func_8013AF78` (296 bytes) as default-build C, 27/27, funcdiff MATCH for default and `--version c`.
Four marked FAKEs (a dead `register` initializer turns copy propagation off, see [[playbook]]); no blocking findings, one catalog link added.

## [2026-10-23] ticket | T-25305 Done

Reviewed [[tickets/T-25305-w10-SHOUGATU-8013E330]] against CODING_STANDARDS.md; no match: SHOUGATU `func_8013EA98` stays `INCLUDE_ASM` (best body 42 funcdiff rows, over the guard limit), 27/27, no findings.
Second-pass results (natural member spelling fixes the `>= 9` block, count block and exit; latch and t4 selector still diverge) and the next hypothesis are in `drafts/T-25305/SHOUGATU/func_8013EA98/NOTES.md` and [[matching-notes]].

## [2026-10-23] ticket | T-25308 Done

Reviewed [[tickets/T-25308-w10-RPG_BAT-80154430]] against CODING_STANDARDS.md; matched RPG_BAT `func_80154DD8` (1204 bytes) as default-build C, 27/27, funcdiff MATCH, no findings.
Frame fixed by declaring only x and y (`-x, -y` in both calls), call order by two marked same-symbol FAKE views; notes in `drafts/T-25308/RPG_BAT/func_80154DD8/NOTES.md` and [[matching-notes]].

## [2026-10-23] ticket | T-25307 Done

Reviewed [[tickets/T-25307-w10-main-8004F870]] against CODING_STANDARDS.md; matched main `func_80052F68` (1200 bytes) as default-build C, 27/27, funcdiff MATCH, no findings.
The cause was counter ownership (`m` spans loops 1, 2-inner, 3-outer and 4, `i` loops 2-outer and 3-inner); one FAKE (three unused locals for the frame). Notes in `drafts/T-25307/main/func_80052F68/NOTES.md` and [[matching-notes]].

## [2026-10-23] ticket | T-25306 Done

Reviewed [[tickets/T-25306-w10-BUNKA_SD-80134540]] against CODING_STANDARDS.md; matched BUNKA_SD `func_80134D94` (972 bytes) as default-build C, 27/27, funcdiff MATCH, no findings.
The header now declares `s32 func_80134D94(void);`; notes in `drafts/T-25306/BUNKA_SD/func_80134D94/NOTES.md` and [[matching-notes]].

## [2026-10-23] ticket | T-25400 wave 11 opened (repeat until matched, GEKO included)

The user does not want to wait for the issue #65 claim; diman8 was told on the issue that our agents now also work on the four GEKO functions and that their PRs stay welcome. [[tickets/T-25400-wave-11-until-matched]] and T-25401..T-25412 cover every file still holding INCLUDE_ASM: GEKO first, then each file wave 10 leaves unmatched, up to three passes per file.

## [2026-10-23] ticket | T-11910 In Progress

Second round of claim CosmicScribe64/tokimemo-ps1-decomp#65: the last four GEKO functions (`func_80133C50`, `func_80142C9C`, `func_80133024`, `func_80132B24`). One Opus agent at a time. [[tickets/T-11910-geko-remainder]].

## [2026-10-23] ticket | T-11910: GEKO func_80133C50 matched

Agent A matched `func_80133C50` (volatile club view, FAKE) and moved the `func_80142C9C` checkpoint from 162 to 119 lines. PR opened for the match because the maintainer's agents now also work on the remaining GEKO functions (#65). Notes in [[matching-notes]]. [[tickets/T-11910-geko-remainder]].

## [2026-10-23] ticket | T-25500 stubborn functions opened

User asked for extra effort, outside-the-box approaches and build tooling changes if needed for the functions that keep failing. [[tickets/T-25500-stubborn-functions]] starts with SHOUGATU func_8013EA98 ([[tickets/T-25501-stubborn-shougatu-8013ea98]]) and TAIIKU func_80137784 ([[tickets/T-25502-stubborn-taiiku-80137784]]); tooling or config changes must meet CODING_STANDARDS.md 7a and pass two adversarial reviews.

## [2026-10-23] decision | Playbook: compiler research summary for agents

Added "Before proposing a compiler or build-step change" to [[playbook]]: which compiler the game used, the IDO 5.2/5.3/4.1 comparisons, the rejected allocator rules and levers, and where a real difference has been found before (the front end's ucode, `cvt_pass.py`). Prompted by the stubborn-function attack ([[tickets/T-25500-stubborn-functions]]) so agents start from these results instead of repeating them.

## [2026-10-23] ticket | T-25309 Done

Reviewed [[tickets/T-25309-w10-TAIIKU-80133C80]] against CODING_STANDARDS.md; no match: TAIIKU `func_80137784` stays behind its `NON_MATCHING` guard (4 funcdiff rows, the operand order of two pointer compares), 27/27, no findings.
The only measured match needs second symbol names (`drafts/T-25309/TAIIKU/func_80137784/alias-match.patch`, waiting for the user's approval); notes in [[matching-notes]].

## [2026-10-23] ticket | T-25310 Done

Reviewed [[tickets/T-25310-w10-main-80053650]] against CODING_STANDARDS.md; matched main `func_80055AFC` (1396 bytes) as default-build C, 27/27, funcdiff MATCH; two small findings fixed (cheatsheet row placement, FAKE comment ticket).
A dead `p = bzero(...)` (pointer-returning prototype) fixes the colours of the unrolled 256-record loop; notes in `drafts/T-25310/main/func_80055AFC/NOTES.md`, [[matching-notes]] and [[register-cheatsheet]].

## [2026-10-23] ticket | T-25311 Done

Reviewed and merged: ENDING func_80134270 matched (1848 bytes, default C, three FAKE marks), src/ovl/ENDING/80133C10.c has no INCLUDE_ASM left. [[tickets/T-25311-w10-ENDING-80133C10]], notes in [[matching-notes]].

## [2026-10-23] ticket | T-11910 Done

Agent B matched GEKO `func_80132B24` (s32 with no return plus a kept-live empty test; a no-op assignment to order cfe's `+` operands) and kept `func_80133024` guarded at 2 rows with the ucode finding in its comment. With `func_80133C50` (#69) that is two matches in this round; `func_80142C9C` and `func_80133024` stay open (the maintainer's agents also work on them, #65). Inline review against CODING_STANDARDS.md. Notes in [[matching-notes]]. [[tickets/T-11910-geko-remainder]].

## [2026-10-23] build | T-25403 w11-GEKO-80142760-p1
Pass 1 on GEKO func_80142C9C did not match: `src/ovl/GEKO/80142760.c` unchanged (INCLUDE_ASM stays, best body 159 rows); reviewed, only drafts and notes merged.
Next pass starts from `drafts/T-25403/GEKO/func_80142C9C/NOTES.md` (global copy propagation off, [[tickets/T-25403-w11-GEKO-80142760]]).

## [2026-10-23] ticket | T-25501 Done: SHOUGATU func_8013EA98 matched

The stubborn-function attack ([[tickets/T-25500-stubborn-functions]]) matched SHOUGATU func_8013EA98 (920 bytes) under stock IDO 5.3: a nested-if loop shape, counted free references and a forced cost split; no tool change, one keep line with a keepproof proof. Both reviews pass after the FAKE marking was completed ([[tickets/T-25501-stubborn-shougatu-8013ea98]]).

## [2026-10-23] ticket | T-25502 Done: TAIIKU func_80137784 matched in plain C

The stubborn-function attack ([[tickets/T-25500-stubborn-functions]]) found that the original's two pointer compares are uopt's test replacement of plain `arg0 == 0` tests; no extra symbols or tool change. Both reviewers' wording findings on the mechanism were fixed before merge ([[tickets/T-25502-stubborn-taiiku-80137784]]).

## [2026-10-23] ticket | T-25600 finish gate opened

User approved one gate in place of the separate conveyors. [[tickets/T-25600-finish-gate]] (tickets T-25601..T-25609) loops the nine functions left until none is in assembly, using the lens, synthesis and review treatment that matched TAIIKU func_80137784. Waves 10 and 11 start no new work; their running passes finish first.

## [2026-10-23] build | T-25401 w11-GEKO-801323E0-p1
Matched GEKO func_80132B24 (480 bytes) and func_80133024 (280 bytes) in `src/ovl/GEKO/801323E0.c`; func_80132D04 got an old-style definition (same bytes). The file has no INCLUDE_ASM left, so [[tickets/T-25401-w11-GEKO-801323E0]] is Done after review; notes in [[matching-notes]].

## [2026-10-23] ticket | T-25317 Done

Reviewed [[tickets/T-25317-w10-main-80047550]] against CODING_STANDARDS.md; no match: main `func_800476C0` stays `INCLUDE_ASM` (552 funcdiff rows, 8 structural hunks, saved-register order now original), 27/27, two small findings fixed.
Draft body and notes in `drafts/T-25317/main/func_800476C0/`; section in [[matching-notes]].

## [2026-10-23] ticket | T-25314 Done

Reviewed [[tickets/T-25314-w10-ETC-8013FF80]] against CODING_STANDARDS.md; matched ETC `func_8013FFAC` (2116 bytes) as default-build C, 27/27, funcdiff MATCH, no findings.
`register s32 i` is the zcopy-off trigger, two empty labels split uopt blocks, `func_8004A1A0` is an implicit-int view; notes in `drafts/T-25314/ETC/func_8013FFAC/NOTES.md` and [[matching-notes]].

## [2026-10-23] ticket | T-25318 Done

Reviewed [[tickets/T-25318-w10-main-8006CB30]] against CODING_STANDARDS.md; matched main `func_8007132C` (3992 bytes) as default-build C, 27/27, funcdiff MATCH; one small finding fixed (FAKE comments at the two remaining late-bound loops).
Reading the schedule as `D_800E6280.unk_69C[D_800E6280.unk_71C]` with no record pointer leaves the base unallocated; notes in `drafts/T-25318/main/func_8007132C/NOTES.md` and [[matching-notes]].

## [2026-10-23] build | T-25403 w11-GEKO-80142760-p2
Pass 2 matched GEKO `func_80142C9C` (3,656 bytes, default C, 27/27); `src/ovl/GEKO/80142760.c` has no INCLUDE_ASM left. Reviewed, no findings; keep line for `D_800E71DF` added.
Lever in [[playbook]], notes in `drafts/T-25403/GEKO/func_80142C9C/NOTES.md`, [[tickets/T-25403-w11-GEKO-80142760]] Done.

## [2026-10-23] ticket | T-25319 Done

Reviewed [[tickets/T-25319-w10-NAME_ENT-80142A80]] against CODING_STANDARDS.md; no match: NAME_ENT `func_80143580` stays `INCLUDE_ASM` (draft at 293 rows, score 293, from 471), 27/27, no findings left.
The draft packet is `drafts/T-25319/NAME_ENT/func_80143580/` (next hypothesis in NOTES.md) and [[matching-notes]].

## [2026-10-23] ingest | T-25700 recent wins distilled into the playbook

New page [[notes/wins-2026-10]]: all 93 functions of waves 8-11, the wave 9 closers, the stubborn-function attack and PRs #68 to #70, grouped by recipe with a ranked summary. [[playbook]] gained 11 recipes (new sections 13 and 14) and 19 extended entries, [[register-cheatsheet]] 11 rows; [[tickets/T-25700-distill-wins]] is Done after review.

## [2026-10-23] ticket | T-25606 Done: main func_800476C0 matched

The finish gate's block-by-block lens matched main func_800476C0 (3,132 bytes) in round 1; merged directly after an orchestrator review so the match does not wait for the synthesis stage ([[tickets/T-25600-finish-gate]]).

## [2026-10-23] ticket | T-25604 Done: NAME_ENT func_80143580 matched

The fresh start's history digest found a 122-row body; its guided-search lens closed it (4,308 bytes), with the case 9 record chain confirmed against the Rev 1 copy of the function ([[tickets/T-25600-finish-gate]]). Merged directly after an orchestrator review.

## [2026-10-23] query | T-25608 IDO 4.1 ugen on the block-copy rule candidates

Ran the build's 5.3 front end and uopt with IDO 4.1's ugen (re-downloaded, sha256 verified, gitignored) on
the T-20710 reproducers (states as recorded) and on 24 new cases in `tools/ido_eval_cases/ugen_copy_state/`
(README there). Neither candidate rule for [[tickets/T-25608-gate-main-func_8006B0C8]] agrees with 4.1 on
every case: rule A misses 8, rule B 8, the build rule 7. For `func_8006B0C8`, 4.1 gives B for the three swap
copies (injection test), which both rules predict. 4.1's own rule (F after an indirect store, B only at a
register-register compare branch) fits all 30 cases. As a ucode pass it changes three procedures in the corpus:
`func_8006B0C8` matches, and main `strCallback` and `func_8005938C` fail because of their FAKE steering loops.
Section added to [[ido-52-evaluation]]; harness in `drafts/T-25608/ido41/`.

## [2026-10-23] ticket | T-25608 Done: main func_8006B0C8 matched; every game function is C

The last function matched with a narrowed block-copy state rule in tools/ugen_pass.py; an IDO 4.1 check (user-approved download) confirmed the three swap copies' state and showed a better general rule, now [[tickets/T-26000-ugen-block-copy-41-rule]]. With this merge all 6,952 game functions are matched ([[tickets/T-25600-finish-gate]]).

## [2026-10-24] ticket | T-26001 Done: stale tool tests fixed

[[tickets/T-26001-stale-tool-tests]]: test_build_deps now copies tools/ugen_pass.py into its fake project and the protoverride compile_unit test gives its fake context a name; the full tool suite passes (1572 OK, 3 skipped).

## [2026-10-24] decision | Phase 1 complete: 6,952/6,952 game functions

Every game function is C and every binary rebuilds byte for byte ([[status]]). Closed the wave 10/11, stubborn-function and finish-gate tickets (T-25300, T-25400, T-25500, T-25600 and their per-file tickets); T-26000 (IDO 4.1 block-copy rule) is in progress. ROADMAP.md marks phase 1 done and phase 2 (readable code) in progress; README.md says every game function is C.

## [2026-10-24] build | T-27002 rename tool for phase 2

[[tickets/T-27002-rename-tool]] (In Review): `tools/rename.py` renames functions, globals, members, types, locals, parameters, C files, overlay folders, headers and constants from a table of rows keyed by (unit, address); `plan` checks every collision and exits 1 on any problem, `apply` proves the edit (inverse round trip, gcc `-fsyntax-only` error set, `sync_protos.py` views) and appends to a ledger under `config/renames/`, `check` (ninja `build/renames.ok`) keeps old names out, `replay` brings branches and merges up to date, `lookup`/`export` serve the wiki. Members are found by a gcc oracle; `tools/symtab.py` resolves names over the symbol files of every yaml and places new names (per-overlay `config/symbols_<NAME>.txt`, `_hypotheses` files). Prerequisite fixes from the T-25900 audit: `check_symbols.py` over every yaml (`cd_read_status` moved out of EVENT's range), `main_names.ld` inputs, `split_objects.py`/`data_island.py`/`orphan_rodata.py` keep yaml names, `gen_overlay_configs.py` keeps symbol files, `migrate_globals.py` accepts named bases, `srcscan.py`/`sync_protos.py`/`object_layout.py` map readable folders and headers to the unit. Trial on RENSYU (scratch branch `w19-rename-trial`, not merged): symbols, a type, a member, locals, a file, an alias merge across three overlays, a const enum, a folder and a header renamed; both clean builds 27/27 sha1, 6,952/6,952 functions. Usage: [[rename-tool]]; [[tooling]], [[build-system]], [[source-files]] updated.

## [2026-10-24] build | T-27002 review fixes

[[tickets/T-27002-rename-tool]] stays In Review after the first review's four blocking findings were fixed: `tools/rename.py replay` no longer stops on a ledger chain of 3 or more names followed by a local or field row (merge mode builds chains from the ledger only); type and field rows carry their file scope for the name mapping of proofs 2 and 3; a field's HYPOTHESIS mark goes on the renamed member; the card is on [[kanban]], [[rename-tool]] has a line in [[index]], the phase 2 link points at ROADMAP.md. Also fixed: two members of one type in one table, m2c-only placeholder shapes, replay row selection by the scoped search of `check` and its order (symbols before locals), a message when check fails after apply, `#` in ledger notes. New tests in `tools/test_rename.py`; open follow-ups listed in the ticket.

## [2026-10-24] build | T-20500 emulator probe merged

[[emulator]]: PCSX-Redux (pinned 8288e927 plus upstream fix 3a24bbcc) with OpenBIOS (MIT) in Docker; the game boots headless to the title menu. tools/emu/run.sh drives breakpoints on main and overlay code, memory watches, call stacks, traces, scripted input, screenshots and save states for phase 2 naming. Ticket stays In Progress for its GameState naming pilot.

## [2026-10-24] decision | Phase 2 naming conventions (ADR 0001)

The user's four phase 2 decisions (names from code and emulator evidence, claims of game meaning marked as hypotheses; O.BIN names kept with one-line English comments; English file, folder and header names with a disc-name map; English game terms except iconic ones) are recorded in `docs/adr/0001-phase-2-naming.md` with the conventions: case, overlay prefixes from a disc-name map, main subsystem prefixes, enums and constants, hypothesis files, locals, offset comments, file and header naming. `CODING_STANDARDS.md` sections 4 and 5 rewritten (8 and 8a aligned, checklist updated). `GLOSSARY.md` created at the repo root from the local glossary seed (traps, people, parameters, calendar, places, clubs, events, systems, mini games). Plan: [[phase2]] ([[tickets/T-27001-phase-2-conventions]]).

## [2026-10-24] ticket | Phase 2 tickets created

[[tickets/T-27000-phase-2]] (umbrella, In Progress), [[tickets/T-27001-phase-2-conventions]] (In Review), [[tickets/T-27003-pilot-obin-names-overlays]] and [[tickets/T-27004-shared-core-names]] (Backlog, blocked by T-27002, the rename tool, which is written separately). Board cards are added by the kanban sync at merge.

## [2026-10-24] decision | Phase 2 conventions: review fixes

Fixes for the first review of [[tickets/T-27001-phase-2-conventions]] in `docs/adr/0001-phase-2-naming.md`, `CODING_STANDARDS.md`, `GLOSSARY.md` and [[phase2]]. Glossary: abbreviation table, `card` and `pad` traps, rows for the map's game-term prefixes; main prefixes `game_mode_` and `memory_card_`. Renames: none before the rename tool merges (ADR and section 4 now agree). Hypotheses: main names in EVENT's range go to the main-only files. File renames change the `.rodata`/`.data` subsegments and inner asm paths too. The ledger keeps the evidence notes. Section 10 states the no-game-text rule. Pending user decisions (audit questions 2 and 5) listed in the ADR. The GameState pilot of [[tickets/T-20500-emulator-assisted-naming]] moved to [[tickets/T-27004-shared-core-names]]; [[tickets/T-27000-phase-2]] gets the umbrella-header criterion and its first box unticked until T-27001 is Done. No status changes.

## [2026-10-24] query | T-25900 phase 2 naming audit

drafts/T-25900/audit.md: naming state per unit (6,649 of 6,952 functions and 16,629 data labels still placeholders, 437 address-named files), how renames flow today and their gaps (the merge-time rename table matches no files since the per-object split), the design of tools/rename.py, naming conventions and the phase 2 work order.

## [2026-10-24] build | T-26000 IDO 4.1 block-copy rule adopted
tools/ugen_pass.py now follows IDO 4.1's block-copy state rule (30 cases as unit tests); main strCallback and func_8005938C re-steered by respelling a test, two obsolete empty-test FAKEs removed, clean build 27/27 ([[tickets/T-26000-ugen-block-copy-41-rule]]).
4.1 also sets F at a branch on a value; not modelled because of main func_8006B0C8 ([[ido-52-evaluation]] "T-26000", [[toolchain]], [[matching-notes]]).

## [2026-10-23] build | T-26000 review fixes: compare rule corrected, FAKE arms cut down
Case `copy_store_then_ne0_left_if` (indirect store first) showed that IDO 4.1 keeps the state at a compare with the constant on the left; tools/ugen_pass.py now sets B only for a compare of two non-constants (0 of 566 procedures change state). 4.1 ignores a store in an arm ugen drops (`copy_dropped_arm_store`); the never-true FAKE arms rely on the pass counting it, and are cut down to the store alone in main strCallback, func_8005938C and MASTER func_801344E0, func_801363F4 ([[tickets/T-26000-ugen-block-copy-41-rule]], [[toolchain]], [[ido-52-evaluation]], [[matching-notes]], [[playbook]]). drafts/T-25608/ido41/utrace.py works again with the new pass.

## [2026-10-24] ticket | T-26000 Done
[[tickets/T-26000-ugen-block-copy-41-rule]] passed the standards and spec reviews (approved, four non-blocking follow-ups noted in the ticket) and moves to Done.
Merged as w17-ugen41; follow-up still to file: the 4.1 census of F at a value branch, `func_8006B0C8`'s 4-byte copy, stored compares, `Uxjp`, `Uijp`.

## [2026-10-24] ticket | T-26002 FAKE sweep under the 4.1 block-copy rule (In Progress)
[[tickets/T-26002-fake-sweep-41-rule]] created on branch w20-fake-sweep (base w2-good) to remove the FAKEs that only steered the old block-copy detector.

## [2026-10-24] build | T-26002: no FAKE removable, comments of the kept ones corrected
Census of the full build (566 procedures with a `Umov`, throwaway logging shim, scripts in `drafts/T-26002/`): the copy state depends on a FAKE in exactly four procedures, MASTER `func_801344E0`, `func_801363F4`, main `strCallback`, `func_8005938C`. Of the 58 candidate hits, 50 are other mechanisms (35 in functions without a block copy); the 8 related ones were removed and rebuilt one by one and all stay: the four never-true arms, the repeated copy line of GYOZI `func_8014494C` and SHOUGATU `func_8014265C` (the original has two identical loops, both B), and the frame pads of GEKO `func_801406CC` and EVENT `func_80109404`. Comments of the arms and the repeated lines rewritten (comment-only edits to `src/main/800563F0.c`, `src/main/80058D20.c`, `src/ovl/MASTER/80133680.c`, `src/ovl/GYOZI/80143AD0.c`, `src/ovl/SHOUGATU/80141780.c`). Notes in [[matching-notes]] "T-26002".

## [2026-10-24] ticket | T-26002 Done
[[tickets/T-26002-fake-sweep-41-rule]] passed the standards and spec review (approved, no blocking findings; comment-only change in src/, no FAKE removed). Kanban card added under Done, and the comments of `strCallback` and `func_8005938C` now cite the full path of `copy_dropped_arm_store.c`. Merging as w20-fake-sweep.

## [2026-10-24] ticket | T-26003 opened (Backlog)

[[tickets/T-26003-ugen-value-branch-dropped-arm]]: the two IDO 4.1 behaviours the T-26000 rule does not model yet (value-branch F, dropped-arm stores), which keep four never-true FAKE arms alive per the T-26002 sweep.

## [2026-10-24] ingest | T-27028 phase 2 naming proposals and port spec for overlay TACO
Read all 38 files of `src/ovl/TACO` (406 functions), the call graph per stage handler and the data tables, and wrote `config/renames/proposals/o-taco.tsv` (911 rows: 406 func, 179 data, 70 field, 24 type, 40 file, 176 const, 16 comment; 14 marked HYPOTHESIS) and [[spec/o-taco]] (stage machine of 19 stages, ship and wingman rules, weapons, bombs, collisions and scoring, three stage scripts, three bosses, result tally, name entry, settings in the work area at `D_800E7D10+0x40`, entry from MASTER and OPTION). Prefix `shooter_` after the glossary seed. Findings: x is mirrored (pad left raises x); the wingman cannot die (its HP is never read); `func_8015CF30/D0D0/D270` are libgte RotMatrixZ/X/Y copies (proposed as `shooter_rot_matrix_z/x/y`); enemy type 11 never spawns; boss damage percent starts above 0 by construction. Ticket [[tickets/T-27028-p2-o-taco]] In Review ([[tickets/T-27000-phase-2]]). No source, header or symbol file touched; emulator not used (see the ticket).

## [2026-10-24] ticket | T-27028 review round 1 fixes for the TACO table and spec
Fixed the two blocking findings. [[tickets/T-27028-p2-o-taco]]: `func_801335A0` ORs the low byte into 0x600 when its first argument is even, so scene scripts that write 0x5xx play 0x6xx; the table notes and [[spec/o-taco]] now cite the played ids everywhere (boss 1 defeat 0x608, boss 2 defeat 0x604, title 0x604 to 0x607, opening cues, boss 1 to 3 cues and cadences: boss 2 every 240 frames at 1 in 3, boss 3 every 180), with the cooldown rule corrected (only for first argument exactly 1). New constants 0x604 to 0x608 and 0x511, 0x512, 0x514, 0x516. The pad enum is now `PadBits`; six core.tsv values (SELECT, START, L2, R2, L1, R1) conflict with the pad word main builds and are listed in spec section 14 for the core owner. Also `TcActor.unk7A` notes (boss part HP bars), the `Tc14` alias, settings types overlapping core, seven comment rows dropped as duplicates of core (913 rows now). Tables and wiki only.

## [2026-10-24] ticket | T-27028 Done
[[tickets/T-27028-p2-o-taco]] passed the standards and spec review in round 2 (approved, no blocking findings; round-1 sound ids and pad enum verified against the C) and moves to Done. The table `config/renames/proposals/o-taco.tsv` (913 rows) and [[spec/o-taco]] are merged as p2a-o-taco; the six conflicting `core.tsv` pad values are for the core owner (spec section 14).

## [2026-10-24] ingest | h-graphics phase-2 table and port spec
Unit h-graphics (`src/main/80043510.c`, `80047550.c`, `800490C0.c`, `80049FF0.c`, `8004AD60.c`, `8004C3F0.c`, `80085E30.c`): read the matched C and its callers, checked behaviour with `tools/emu/run.sh trace` and `watch` from a NAME_ENT save state, wrote `config/renames/proposals/h-graphics.tsv` (274 rows, 25 HYPOTHESIS) and [[spec/h-graphics]], linked from [[spec/index]]; ticket [[tickets/T-27020-p2-h-graphics]] In Review. No src/, include/ or symbol-file changes.
Findings: `Rec24` has the libgs GsSPRITE layout; the work-area allocator spaces primitives at 4x their size; the texture-block cache never evicts; `func_80043510` is the per-frame renderer (always called with 1) from the main loop `func_80042134`; the second half of `80043510.c` and five helpers in `80085E30.c` are not graphics (sound-bank loader, girl pick, overlay check).

## [2026-10-24] ticket | T-27020 review fixes
Reviewer findings on [[tickets/T-27020-p2-h-graphics]] (five blocking, several non-blocking) fixed in `config/renames/proposals/h-graphics.tsv` (263 rows, 20 HYPOTHESIS) and [[spec/h-graphics]]: field scope uses old type names, `model_objects` is 16 records, `ACTOR_ANIM_END_BACK` also clears RUN, the TACO limiter includes stage 12, `pick_top_girl` rule corrected. `unk_10E8` renamed `display_pass_enabled`; 11 sound-loader data rows left to the sound unit. Each claim re-checked against `src/main/80043510.c`, `80047550.c`, `8004C3F0.c`, `80085E30.c`. No src/, include/ or symbol-file changes. Ticket stays In Review.

## [2026-10-24] ticket | T-27020 review fixes, round 2
Second review of [[tickets/T-27020-p2-h-graphics]] (three blocking findings, several non-blocking) fixed in `config/renames/proposals/h-graphics.tsv` (263 rows, 24 start with HYPOTHESIS) and [[spec/h-graphics]]. Correction to the `ingest` entry above: the texture-block cache does evict, once per frame. An actor's claim on a slot is written to the frame map only and `search_tpage(0x7FFF)` frees every slot whose resident word is -1 at the next frame start, so blocks are queued for upload again in every frame that uses them; only slots with a resident word other than -1 persist. Also fixed: `prim_box_gouraud` mode table (no diagonal case, mode 3 reads green for blue), `func_800866A0` compares eight words and is renamed `asset_package_verify_step` (HYPOTHESIS; core calls it `bustup_read_step`), `make_color_bar16` direction, cell size versus pixel depth, record 160 one past the actor pool (0x80121750), HYPOTHESIS marks, call-site counts recounted, and a core-versus-h-graphics name comparison (74 conflicting names) in the spec. Each claim re-checked against `src/main/80047550.c`, `800490C0.c`, `80085E30.c`, `8004AD60.c`. No src/, include/ or symbol-file changes. Ticket stays In Review.

## [2026-10-24] ticket | T-27020 Done
[[tickets/T-27020-p2-h-graphics]] passed the third review (approved, no findings): the texture-block cache section and the `prim_box_gouraud` mode table in [[spec/h-graphics]] and `config/renames/proposals/h-graphics.tsv` match `src/main/80047550.c` and `src/main/800490C0.c`. The branch changes no `src/`, `include/` or symbol files. Moves to Done.

## [2026-10-24] ticket | T-27028 review round 3 fixes for the TACO table and spec
Fixed both blocking findings of the round 3 review of [[tickets/T-27028-p2-o-taco]]: the boss start damage percents are 36, 51 and 63 (integer division of `shooter_boss_damage_percent`, thresholds tested on the integer) and the left/right names now use the sign of x (world +x is screen left). Non-blocking items done in `config/renames/proposals/o-taco.tsv` (912 rows) and [[spec/o-taco]]: stale pad conflict notes, comment row columns, `sound_bank` names that follow core, separate particle view type, `TcActor.unk7A` named `counter`, score digit claim, behaviour-label note, table header, no symbols file row. Not touched by design: `wiki/index.md` (needs a line for [[spec/index]]), `wiki/kanban.md`, and the umbrella ticket link that has no file yet.

## [2026-10-24] ticket | T-27028 Done after review round 4
[[tickets/T-27028-p2-o-taco]] passed the round 4 review at f73c1b522 (approved, no findings; boss start percents 36, 51 and 63 and the pos_x/neg_x names verified against the C and the `.data.s` bytes). Follow-up commit to the earlier merge; tables and wiki only, nothing under `src/`, `include/` or `config/symbol*`. Merged again as p2a-o-taco.

## [2026-10-24] ingest | Phase 2 core: names and port spec (T-27010)
Read the matched C of GameState, the mode machine, the day-event queue, the girl score code and the top 100 main functions; wrote the proposals table `config/renames/proposals/core.tsv` (738 rows: 90 func, 39 data, 234 field, 75 type, 238 const, 60 comment, 2 file; 74 carry HYPOTHESIS) and [[spec/core]], linked from [[spec/index]]. Ticket [[tickets/T-27010-p2-core]] (In Review). Nothing in `src/`, `include/` or the symbol files changed; no emulator run (image not built here).
Findings: `unk_1108/1109/110A/110D` are mode/stage/step/substep; `unk_0F4` is the player flag word (health state 0-3, club, flags); `unk_F68` the last chosen command; the calendar tables come from disc blobs via `D_800B3748`; `unk_71E` marks played birthday scenes; `unk_720..722` is the return triple. EVENT and GYOZI calls into main are stale (older main layout), so their uses are excluded from the top-100 census (see [[game-state]], [[data-types]]).

## [2026-10-24] ticket | T-27010 review fixes
Fixed the four blocking findings of the review of [[tickets/T-27010-p2-core]] in `config/renames/proposals/core.tsv` (now 742 rows, 85 with HYPOTHESIS) and [[spec/core]]: the `PadBits` constants follow the pad word built in `src/main/80042A00.c` (SELECT 0x100, START 0x800, shoulders 0x1-0x8), which makes `unk_F6E` a pad 2 START toggle (`sd_anim_fast`); `unk_72C`/`unk_73C` are `girl_last_date_spot` and its weekly timer with the repeat-spot penalty; the `parameter_up_down` text matches `src/main/800737A0.c`; `func_80071038` (`girl_counters_clamp`) and `func_80071280` are in the table and the spec, so the forced encounter and rumour re-arm below syoushin 0x32. Non-blocking points handled: HYPOTHESIS marks on the camp, meeting and seasonal-party names, English-form SHOUGATU names, `bustup_wink_update`, use-count caveat, month-blob load (open question 10), and [[data/p2-core-name-conflicts]] (96 names that differ from the hardware tables, rule in spec section 17). Nothing in `src/`, `include/` or the symbol files changed; no emulator run.

## [2026-10-24] ticket | T-27010 second review fixes
Fixed the three blocking findings and ten non-blocking ones of the second review of [[tickets/T-27010-p2-core]] in `config/renames/proposals/core.tsv` (now 751 rows, 97 with HYPOTHESIS) and [[spec/core]]. Blocking: `MenuMode` 2 is mouse-like + A and 3 is menu B (the stored values the settings screen writes, not the item numbers); `unk_71E` is `daily_event_bits` (bits 1, 2, 4 are the birthday skips of the queue reader, the other bits are HYPOTHESIS done marks); the event log rank is `get_g_zyotai_h`, not `girl_state_rank`. Non-blocking: `mode_timer` counted in `main_state_dispatch`, `year` is the calendar year modulo 100, `girl_met_today_clear` became `girl_flags_bit0_clear`, `rumor_done`, `saved_mode/stage/step`, HYPOTHESIS marks on three mode names, seven `GameState` members renamed from `unk_` with HYPOTHESIS names (23 members keep `unk_`), housekeeping notes and counts. No `src/`, `include/` or symbol-file edits; ticket stays In Review. See also [[data/p2-core-name-conflicts]] (no new conflicts).

## [2026-10-24] ticket | T-27010 Done
[[tickets/T-27010-p2-core]] passed the second review (approved, no findings; the `MenuMode` values, `daily_event_bits` and the event log rank were checked against the C) and moves to Done. The branch changes only `config/renames/proposals/core.tsv`, [[spec/core]], [[spec/index]], [[data/p2-core-name-conflicts]] and wiki pages; nothing under `src/`, `include/` or the symbol files. Merged as p2a-core.

## [2026-10-24] ingest | T-27021 phase 2 h-cdio: names and port spec for the CD, movie, MDEC, 3D and mouse layer
[[tickets/T-27021-p2-h-cdio]] (In Review): nine main files (80 functions) read against their callers and three cold-boot emulator runs. Wrote config/renames/proposals/h-cdio.tsv (200 rows: 53 func, 49 data, 37 const, 27 comment, 17 field, 9 file, 8 type; 8 hypotheses) and [[spec/h-cdio]] (linked from [[spec/index]]).
Findings: the CD layer is a 48-entry command ring over libcd with an end-of-file marker test on checked reads; both movies play in 24-bit mode (movie flag starts at 1, never written), so the caption branch is not reached; `initLight`, `initCoordinate` and the mouse read functions have no caller in src/. No change to src/, include/ or symbol files.

## [2026-10-24] ticket | T-27021 h-cdio fix pass after review
[[tickets/T-27021-p2-h-cdio]] (still In Review): resolved the three blocking review findings in `config/renames/proposals/h-cdio.tsv` and [[spec/h-cdio]]. Data `D_800B5944` renamed `dec_bg_wipe_count` (it clashed with the function `dec_bg_wipe_step`); the public command id enum is `CdCommandId` (struct `CdCommand` kept); the fade draws were rewritten from the code (`dec_bg_fade_draw` 4 sprites from the shown buffer, `dec_bg_crossfade_draw` 6 sprites over shown and hidden buffer; purposes marked HYPOTHESIS). Also adopted the graphics unit's `draw_origin_x/y` for `GameState.unk_014/018`, dropped the `MainMode` and `CdQueueCmd` const rows, corrected the retry, clamp, volume, debug-print and reset-call notes, and added spec section 11 listing 18 names that differ from the core proposal. Table: 186 rows, 10 hypotheses. No change under src/, include/ or config symbol files.

## [2026-10-24] ticket | T-27021 h-cdio fix pass 2 after review round 2
[[tickets/T-27021-p2-h-cdio]] (still In Review): resolved both blocking findings and the non-blocking ones in `config/renames/proposals/h-cdio.tsv` (187 rows) and [[spec/h-cdio]]. File row `src/main/80056300.c` now maps to `src/main/reload_exe.c`: `exit_function` reloads the executable named PSX.EXE, the boot file name on the Rev and Shokai discs ([[versions]]), so "exit to launcher" was wrong; the spec says so in the module map, 8.2 and section 9. The CD status protocol is now a table in spec 2.4 and includes `cd_last_command` (12 overlay sites test `== 9` for "stream stopped"); `models_update_draw` is described as gated by `GameState.unk_1113`. Corrected the `dec_bg` call-site counts (345 and 384 with `grep -w`), `DEC_BG_LAYER_*` constant names, field scope `TaiikuBig` plus a `ModelRec88.unk_14` flag row, comment provenance, `cd_queue_push` return, wipe line count, `GetlocL` re-queue, debug flag setter, `D_800B5938[2]`, display callers in main (a pad-triggered soft reset), speaker evidence. Section 11 lists the 22 name conflicts with core (adds `CdRingView`, `MouseOut`, `unk_014/018`). No src, include or config edit.

## [2026-10-23] ticket | T-27021 Done
[[tickets/T-27021-p2-h-cdio]] passed the third review (approved, no findings): the `reload_exe.c` file name and the `cd_last_command` status-protocol rows in `config/renames/proposals/h-cdio.tsv` and [[spec/h-cdio]] match `src/main/80056300.c` and the 12 overlay sites. The branch changes no `src/`, `include/` or symbol files. Moves to Done.

## [2026-10-24] ingest | T-27027 phase 2: overlay RPG_BAT naming table and port spec
Read the RPG_BAT overlay (61 files, 328 functions) with its three launchers and the main club and girl name tables; wrote [[spec/o-rpgbat]] (launch codes, 16 stages, five script channels, start values, damage and AI formulas, girl behaviors, end conditions, hidden code entry) and `config/renames/proposals/o-rpgbat.tsv` (706 rows: 328 func, 160 data, 104 const, 62 file, 27 field, 9 type, 16 comment; 9 marked HYPOTHESIS), prefix `rpg_battle_`. No change to `src/`, `include/` or symbol files; no emulator run (no save at a trigger point, no memory poke). Ticket [[tickets/T-27027-p2-o-rpgbat]] (In Review).

## [2026-10-24] ticket | T-27027 RPG_BAT review fixes
Fixed the review findings on [[spec/o-rpgbat]] and `config/renames/proposals/o-rpgbat.tsv`: launch codes 4 and 5 rewritten from `func_8013C7D0` (`D_800CA2F8` is the dating place; `RPG_LAUNCH_DELINQUENT_AMBUSH` became `RPG_LAUNCH_DELINQUENTS_REPEAT_PLACE`, code 5 clauses completed); the `ClubId` and `GirlId` blocks are gone (core owns `Club` and `GirlId`; `RpgPartner` 14 is the local, HYPOTHESIS-marked no-partner value); `D_8015EC34` is `rpg_battle_escalation_counter` because the no-club special writes its tier back and clears it at step 9; `RpgStat` field rows use the old type name; 'park' dropped (`rpg_battle_background_wave`); habu renamed snake; `stage_next` is `main_stage_next`; 15 `comment main` rows that core already has were dropped; stage 14, state 0x104, the knockdown gate, the aliens pool and the boss round weights corrected. Table now 667 rows (328 func, 160 data, 80 const, 62 file, 27 field, 9 type, 1 comment), 9 marked HYPOTHESIS. Open for the orchestrator: core `PadBits` vs the pad layout used here. Ticket [[tickets/T-27027-p2-o-rpgbat]] stays In Review. No change to `src/`, `include/` or symbol files.

## [2026-10-24] ticket | T-27027 RPG_BAT second review fixes
Resolved the second review of [[spec/o-rpgbat]] and `config/renames/proposals/o-rpgbat.tsv`. Names: family name Himoo replaced by `yuina` in four dialog functions, `RPG_ENC_ROBOT` and `RPG_LAUNCH_ROBOT`; `RpgRec18` field and type rows removed (the six words mean different things in the girl run, the basketball dunk and the art move), data row now `rpg_battle_move_params`; ten text-buffer rows without a code reference removed; result constants renamed `RPG_RESULT_GOOD` and `RPG_RESULT_BAD` (the aliens retreat writes 1 although nobody is knocked out); `D_8015EC38..EC4C` renamed `rpg_battle_bg_word_0..5`; stage constants 12 and 13 renamed `RPG_STAGE_RESET_AUDIO` and `RPG_STAGE_BANK_RELOAD` after reading `func_800674B0`. Aliasing documented in the table notes and in spec section 4 ("Shared words"): voice-started block ending in the dialog step and flag, background words, ripple second set inside the scratch array, move parameters. Spec: pad conflict removed (core's PadBits matches the code tables, `PAD_START` 0x0800), section 7 and the prose use core names with a placeholder map, loot text says draws, DATE reward uses `date_girl`. Row count is 650 (328 func, 150 data, 80 const, 62 file, 21 field, 8 type, 1 comment; 9 HYPOTHESIS); the 706-row figure in the earlier report was stale (667 at the previous commit). `tools/wiki.py lint` reports only the missing T-27000 page and kanban card, which live outside this worktree. `wiki/spec/index.md` is new here and other units add it too: expect a merge conflict.

## [2026-10-24] ticket | T-27027 Done
[[tickets/T-27027-p2-o-rpgbat]] passed the third review (approved, no findings; all nine earlier findings resolved) and moves to Done. Tables and wiki only: [[spec/o-rpgbat]], `config/renames/proposals/o-rpgbat.tsv` (650 rows) and the [[spec/index]] line; nothing under `src/`, `include/` or `config/symbol*`. Merged as p2a-o-rpgbat; the rename tool run stays a later step.

## [2026-10-24] ingest | Phase 2 analysis of overlay TT (T-27024)
Analysed the TT overlay (TwinBee Time Attack, 303 functions) for the port: entry `tt_main`, nine core states, 22 work blocks at 0x80110000, rules for the clock and scroll speed, player, shots, bombs, bells, clouds, waves, ground targets, bosses, ranking and the two secret codes. Wrote `config/renames/proposals/o-tt.tsv` (949 rows, 21 hypotheses) and [[spec/o-tt]], linked from [[spec/index]]. Ticket [[tickets/T-27024-p2-o-tt]] is In Review. The emulator confirmed the title, stage 1, its boss, the ranking screen and the state sequence 1, 2, 5, 7; direct stage mode plays recorded input. No src/, include/ or symbol file changed.

## [2026-10-24] ticket | T-27024 review round 1 fixes
Fixed the blocking review findings of [[tickets/T-27024-p2-o-tt]]: bell kind 1 rule and the shot level 10 downgrade quirk in [[spec/o-tt]] (6.4), HYPOTHESIS marks on all bell names, the header file row, TtCore and warp field rows, shared record byte rows; also return fire by stage (6.9), `sys.use_pad2` bit 5, the boot sector list (7.2). `config/renames/proposals/o-tt.tsv` now has 994 rows (38 hypotheses). Ticket stays In Review for the next review pass. No src/, include/ or symbol file changes.

## [2026-10-24] ticket | T-27024 review round 2 fixes
Fixed the blocking finding of [[tickets/T-27024-p2-o-tt]]: a bell in state 0x70 is a falling hazard (it hurts a ship that is not invulnerable, a shot frees it), documented in [[spec/o-tt]] 6.4 and 6.9 and in the `TtBell.state`, `tt_bell_hit_by_shot`, `tt_bell_collect` and `tt_collisions_update` notes of `config/renames/proposals/o-tt.tsv`. Also: boss 4 beam offset tables (entry counts, new row for `D_80155F14`), bell kind table size, dangling type names, HYPOTHESIS marks on descriptive bell, option and arm swing wording, naming consistency (shot level consts, settings menu vs follower options, boss forms, hit box slots), and core table names in spec sections 2 and 8. The table has 995 rows (42 start with HYPOTHESIS). Ticket stays In Review. No src/, include/ or symbol file changes.

## [2026-10-24] ticket | T-27024 Done
[[tickets/T-27024-p2-o-tt]] passed the third review (approved, no findings): the bell hazard rule (state 0x70 falls, hurts a ship that is not invulnerable, a shot frees it) in [[spec/o-tt]] 6.4 and 6.9 and the boss 4 beam table rows in `config/renames/proposals/o-tt.tsv` match `src/ovl/TT/8013BD10.c`, `80142D40.c`, `80144D70.c` and the asm data. The branch changes no `src/`, `include/` or symbol files. Moves to Done.

## [2026-10-24] decision | Phase 2 conventions: user decisions, port-first scope, review round 2 fixes

Third pass on [[tickets/T-27001-phase-2-conventions]] (still In Review): `docs/adr/0001-phase-2-naming.md`, `CODING_STANDARDS.md`, `GLOSSARY.md`, [[phase2]], [[tickets/T-27000-phase-2]], [[tickets/T-27003-pilot-obin-names-overlays]], [[tickets/T-27004-shared-core-names]]. User decisions of 2026-10-24: `tools/rename.py` is the only way to rename once it has merged (audit question 5, now the binding rule of section 4); old names stay in tickets, logs and old notes while current pages use the new names (audit question 2); the scope is port-first, "not 100%, just enough for the Godot port" (game logic in full, hardware layer at module level and entry points, locals and hex literals only where essential, SDK names optional, matching build exact). The Pending section is gone; the ADR has a Scope section; T-27000's criteria no longer ask for every symbol.

Review fixes: the prefix rule is "a glossary stem, a listed abbreviation, or a row of the Module words table" everywhere (32 rows added, all 78 map prefixes pass a script check); non-O.BIN names are re-checked per unit and renamed through rename.py when they fail (cases in [[phase2]]); `buf`, `lba`, `init`, `src`, `dst` are abbreviations; `main_state_set` is a patch name; `girl_` has one owner, `sound_` is no main prefix, `spot_` replaces `spots_`; file names get the HYPOTHESIS rule; O.BIN comments go at the C definition; the SDK exception is recorded as the user's. T-27000 moved to `T-27000-phase-2.md` so the links of the other phase 2 units and the TACO ticket resolve (two links in earlier entries of this branch were updated). deslop: 0 strong hits; `tools/wiki.py lint` in Docker: 0 broken links.

## [2026-10-24] ticket | T-27001 Done
[[tickets/T-27001-phase-2-conventions]] passed the standards review in round 3 (approved, no blocking findings; `tools/wiki.py lint` 0 broken links) and moves to Done: `docs/adr/0001-phase-2-naming.md`, `CODING_STANDARDS.md` sections 4 and 5, `GLOSSARY.md`, [[phase2]]. [[tickets/T-27000-phase-2]]'s first criterion is ticked. The board cards for T-27000, T-27001, T-27003 and T-27004 come from the kanban sync at merge. Non-blocking: a Module words row for `pad` would make the `pad_` prefix explicit.

## [2026-10-24] build | T-27002 review round 2 fixes

[[tickets/T-27002-rename-tool]] stays In Review after the second review's two blocking findings and two small ones were fixed in `tools/rename.py`: replay walks a member chain from the name the struct holds (B1), replay moves objects, island lines and C files that a stale branch left under an old folder or file name and fixes their yaml lines, asm paths and config keys (B2, also chains of three names for file, folder and header rows), the symbol-line dedupe keeps the attributes of the dropped lines (N4), and a table note keeps its `#` (N5). Tests in `tools/test_rename.py`; trial 4 on a scratch copy (two symbols, a member and a file of RENSYU) built 27 of 27, and the replay onto an old-name copy gave the same tree ([[rename-tool]]).

## [2026-10-24] ticket | T-27002 Done
[[tickets/T-27002-rename-tool]] passed the standards and spec review in round 3 (approved, no blocking findings; full tool suite 1,637 tests OK) and moves to Done. Three non-blocking follow-ups are in the ticket comments: member uses in uncalled header macros, a double space after a deduped symbol line, and the `T-27000-phase-2` links that come from w2-good. Merging as w19-rename through merge3.sh.

## [2026-10-24] ingest | T-27014 phase 2 m-dates-a: names and port spec for the sound driver (src/main/80079B10.c)
[[tickets/T-27014-p2-m-dates-a]] (In Review): the unit was labelled "date sequences, first half" but `src/main/80079B10.c` is the game's sound driver (54 functions); the date and girl code is in `src/main/8007C030.c` (unit m-dates-b). Wrote config/renames/proposals/m-dates-a.tsv (202 rows: 53 func, 46 data, 34 field, 66 const, 1 type, 1 file, 1 comment; 3 carry a HYPOTHESIS mark) and [[spec/m-dates-a]] (linked from [[spec/index]]). Evidence: the matched C, all literal `func_80044750` ids in src/, `asm/libsnd.s`, the 49 O.BIN `SD_*` names (all agree), five short emulator runs.
Findings: the 16-bit command word has classes (control 0, BGM 2, voice clip 3, effects 5 to 8), a stop/pause/resume modifier and fade families with exact per-frame rates; the voice clip request converts to sectors as `index * 8 or 0x20 + channel + base -/+ 0x10` (checked on a real clip); `func_80079E00` ignores its argument; ids 0xB2/0xC2 do nothing; the mono command has no sender. No change to src/, include/ or symbol files.

## [2026-10-24] decision | T-27014 review round 1: voice pause corrected, smaller claims softened
Review of [[tickets/T-27014-p2-m-dates-a]] found one factual error, fixed in `config/renames/proposals/m-dates-a.tsv` and [[spec/m-dates-a]] (sections 3.1, 3.3, 4, 8.4, 12, 15). The voice-clip pause the game sends is the class-0 command 0xE4, and `snd_control_dispatch` calls `snd_xa_promote_next` right after `snd_xa_control`. The promote clears the paused bit and the fade request in the same call, so a pause during playback starts no lasting fade and issues no CD stop: the driver forgets the clip (no end watch, level meter 0) while the CD stream keeps running; a pause during the CD start stops the CD at once and forgets the clip; 0xF4 never resumes (the paused bit is already clear) and has no sender. The earlier "fast fade that ends in a stop" holds only for the class-3 form 0x2300, which nothing sends. RPG_BAT sends 0xE4 in the step after 0x300, probably while the start is pending: marked HYPOTHESIS, no emulator run covered pause.
Also fixed: `snd_init` caller is `func_80042134` (core: main_loop), the `snd_xa_check_end` note (the level meter is read only before the clip end), command 5 and the bank-set loading claim (six of the ten command 5 senders sit next to a bank load; the rest do not; purpose marked HYPOTHESIS), the `SsSetTickMode` row (value 1 is 60 ticks per second in the asm), the `snd_xa_fade_finish` paused test (always true there). Table: 202 rows, 5 HYPOTHESIS marks (3 before). Alias rows kept: `D_80125D14` and `D_80125E62` are used in `src/main/8007C030.c`. No change to src/, include/ or symbol files.

## [2026-10-23] ticket | T-27014 Done
[[tickets/T-27014-p2-m-dates-a]] passed review (approved after the round 1 fix, no findings): the class-0 voice-clip pause and resume description in `config/renames/proposals/m-dates-a.tsv` and [[spec/m-dates-a]] matches `src/main/80079B10.c` and the RPG_BAT and BUNKA_SD senders. The branch changes no `src/`, `include/` or symbol files. Moves to Done.

## [2026-10-24] ingest | m-scheduler phase-2 table and port spec
Unit m-scheduler (`src/main/8006CB30.c`, 56 functions): read the matched C, its callers and callees and the `p2a-core` table, probed behaviour with a small Lua job on the emulator image (mode byte poked from the title-menu state: first-week queue layout, Sunday build with a holiday, weekly affection decay, meeting record, injury step, vacation transitions), wrote `config/renames/proposals/m-scheduler.tsv` (116 rows: 33 func, 11 comment, 8 data, 9 field, 54 const, 1 file; 15 start with HYPOTHESIS) and [[spec/m-scheduler]] ([[tickets/T-27011-p2-m-scheduler]], In Review).
Findings: the week queue is built on the Sunday that ends the previous queue; the command check is a 23/28/33-day biorhythm whose result is the half-effect flag; type 0x47 is a visit, queued after a poor Saturday, six good days or an exam week, not only for illness; the event_flags bytes are laid out in per-girl blocks; the meeting selectors (type 0x48) use parameter thresholds that rise once two girls are met.

## [2026-10-24] ticket | T-27011 review fixes
Reviewer findings on [[tickets/T-27011-p2-m-scheduler]] (five blocking, several non-blocking) fixed in `config/renames/proposals/m-scheduler.tsv` (115 rows: 33 func, 11 comment, 8 data, 8 field, 54 const, 1 file; 16 start with HYPOTHESIS) and [[spec/m-scheduler]]. Blocking: event flag 77 note (a set flag stops girl 11 scene 1, the girl 11 call and the ending roll, checked in `src/main/8006CB30.c` and `src/ovl/ENDING/80133030.c`); event flag 82 marked HYPOTHESIS (GEKO stores a girl id in it and adds 1, ENDING passes it to `func_800847B8`); 3 January 1998 branch is clubs 7-10, not 6-10; `progress_flags_update` rewritten from the code (eight windows from June 1995 to April 1997, entry 0xE, second switch sets 0x80, TEL date-spot page evidence, index not a girl id); `CharFlags` b2 and b6 notes corrected (`meeting_select_club` never tests b2; girl-record init sets b6 for girl 0 only). Non-blocking: `schedule_step_prepare` cursor wording, `ScheduleGirlFlags.b18` and core flag overlaps noted, girl 12 call count and `girl_scores` marked as suggested, `health_illness_*` renamed `health_sick_*`, F6Bits row dropped, `month_data_load_step` 1996-09-01 note, `overlay_scratch` BUNKAKEN user. No src/, include/ or symbol-file changes. Ticket stays In Review.

## [2026-10-24] ticket | T-27011 review round 2 fix
Reviewer round 2 on [[tickets/T-27011-p2-m-scheduler]] approved the five round-1 fixes and found one wrong word: TEL's pending-entry step (`src/ovl/TEL/80132000.c`) takes the last (highest-index) pending `progress_state` entry, not the first (it counts the pending entries, then picks the one where the count reaches 0; entry 0 only when it is the sole one). Fixed in `config/renames/proposals/m-scheduler.tsv` (row `progress_flags_update`) and [[spec/m-scheduler]] section 3.1. Table unchanged otherwise: 115 rows, 16 HYPOTHESIS. No src/, include/ or symbol-file changes. Ticket stays In Review.

## [2026-10-24] ticket | T-27011 Done
Review round 3 of [[tickets/T-27011-p2-m-scheduler]] approved with no findings: the TEL pending-entry fix (last pending entry) matches `src/ovl/TEL/80132000.c` lines 404-424, the table keeps 115 rows of 5 columns, and the branch changes only `config/renames/proposals/m-scheduler.tsv` and wiki files (nothing under `src/`, `include/` or `config/symbol*`). Ticket set to Done; branch p2a-m-scheduler merged into w2-good. Table: `config/renames/proposals/m-scheduler.tsv`, spec: [[spec/m-scheduler]].

## [2026-10-24] ingest | Phase 2 analysis of the main HUD unit (T-27016)
Read `src/main/80062CD0.c` and `src/main/800674B0.c` (81 functions: portrait face, parameter and date panels, message window and title banner, command icon bar, season and vacation helpers, day music, check-pattern background, calendar page with marks and biorhythm graph, information magazine, appointments, club list, save slot panel, mascot, cursor services) with their callers; wrote [[spec/m-hud]] and `config/renames/proposals/m-hud.tsv` (200 rows, 9 marked HYPOTHESIS) under [[tickets/T-27016-p2-m-hud]] (In Review). Main findings: the three 23, 28 and 33 day sine waves behind the command roll (`command_roll_poor_result`, `biorhythm_value`); the 21 flag bytes of `GameState.unk_120` are the date place flags (quarterly magazine, the friend's phone call, GEKO events); free icon layout rules; calendar mark rules and cell geometry; season, summer and vacation tables; lip-sync thresholds per girl. Emulator checks from a weekday-run save state (layout screenshot, 145-frame trace, `break` on the roll, write watch on the cursor guard byte, dumps); the calendar check page was seen on screen, the TEL date-day pick page was not. Names follow `config/renames/proposals/core.tsv` and the hardware tables; nothing under `src/`, `include/` or `config/symbol*` changed.

## [2026-10-24] ticket | T-27016 review round 1 fixes
Fixed the three blocking review findings of [[tickets/T-27016-p2-m-hud]]. `season_palette_fade` (`D_800B5BC8`): the writers are 0xEF at the first month of a season (`month_data_load_step`, `src/main/8006CB30.c`) and in OPTION, 0xFE in `src/main/8005A0B0.c`, BUNKASAI, BUNKAKEN and NAME_ENT; the crossfade runs from the previous to the current season palette ([[spec/m-hud]] section 11, question 1 answered). `D_800B374C` is a pointer of its own, not `month_data_table[1]`: new row `ui_vram_image`. The three `sd_*` functions are called only by the event still branch of `bustup_wink_update`, GEKO and SHUGAKU, so they are now `event_mouth_lipsync`, `event_mouth_anim_start`, `event_eyes_anim_start` (no longer HYPOTHESIS), and `ActorSlot` 96 and 97 take the m-dates-b names `ACTOR_EVENT_MOUTH` and `ACTOR_EVENT_EYES` (16 is `ACTOR_HIZUKE_FIRST` too). Smaller notes in `config/renames/proposals/m-hud.tsv` and the spec: icon 14, the mood rank callers, `bustup_idle_counter_wrap` (HYPOTHESIS, called but without effect), quad 3 of the calendar, `D_800B5A60` readers and writers, ETC as caller of `biorhythm_value`, section 19 callers, `(new)` replaced by `-`. Table now 201 rows, 8 with a HYPOTHESIS mark. Open for the merge: names of `ActorSlot` 30 and 31 and of `D_800B5A60` differ from m-dates-b.

## [2026-10-24] ticket | T-27016 Done
[[tickets/T-27016-p2-m-hud]] passed the second review (approved, no findings): the `season_palette_fade` writers (0xEF at a season change and in OPTION, 0xFE in `src/main/8005A0B0.c`, BUNKASAI, BUNKAKEN and NAME_ENT) and its consumer in `screen_frame_draw`, the separate `ui_vram_image` pointer (`D_800B374C`) and the `event_mouth_lipsync`, `event_mouth_anim_start` and `event_eyes_anim_start` names in `config/renames/proposals/m-hud.tsv` and [[spec/m-hud]] match the C and the exe. The branch changes no `src/`, `include/` or symbol files. Moves to Done; merging the table into the rename tool input stays open on the ticket.

## [2026-10-24] ingest | T-27012 phase 2 naming proposals and port spec for the school-day command loop (m-weekday)
Unit m-weekday (`src/main/800737A0.c`, 19 functions): read the matched C, its callers (`main_state_dispatch`, the queue reader, the holiday module, KANGEI) and the SD, background and parameter tables, and checked behaviour in the emulator (a new game from the title menu to 1995-04-05, then the 1995-07-24 vacation day; a scratch copy of the emulator wrapper with a memory-poke option, kept in `build/`, wrote queue records and player flags into loaded states). Wrote `config/renames/proposals/m-weekday.tsv` (71 rows: 10 func, 19 data, 32 const, 1 file, 9 comment; 1 starts with HYPOTHESIS) and [[spec/m-weekday]]; ticket [[tickets/T-27012-p2-m-weekday]] is In Review. No `src/`, `include/` or symbol-file edits.
Findings: `week_day` (mode 0x40) is a fixed pipeline per school day (screen setup and background read, SD pack read and checksum, SD actors, animation plays once, then the command is applied once and the result held 14 frames); `parameter_up_down` scales only beneficial changes (gains and stress relief) and a poor day adds one point of stress, checked against memory dumps; the poor flag and the companion girl are fixed when the week's queue is built, the companion is a figure inside the SD pack; vacation days use stage 10 and holiday packs; `D_800B6928` has 11 club rows, the twelfth is padding; `func_800737A0` is the last stage of mode 0x32.

## [2026-10-24] ticket | T-27012 Done after review
[[tickets/T-27012-p2-m-weekday]] passed the review at a5a3aad64 (approved; three non-blocking findings recorded in the ticket: the eight `WeekdayCommand` rows duplicate m-scheduler's and go at consolidation, spec open question 4 is stale against m-hud's actor names, and four addresses are named differently in p2a-m-dates-b). Tables and wiki only, nothing under `src/`, `include/` or `config/symbol*`. Merged as p2a-m-weekday.

## [2026-10-24] decision | Phase 2 handoff section and usage pause

[[status]] now opens with a phase 2 handoff: what is merged, what is in flight, how each run resumes, and the usage pause (new phase 2 work stops at 95% weekly plan usage; in-flight units finish or leave WIP snapshots).

## [2026-10-24] ingest | T-27015 m-dates-b: main scene library naming and port spec
Phase 2 unit `m-dates-b` ([[tickets/T-27015-p2-m-dates-b]], In Review): `src/main/8007C030.c` (111 symbols) is the shared scene step library of the event scenes, not only girls and day advance. Wrote [[spec/m-dates-b]] and `config/renames/proposals/m-dates-b.tsv` (258 rows, 25 HYPOTHESIS), plus a line in [[spec/index]]. Method: matched C, overlay step tables in the asm data, SERIFU script data parsed from the disc (numbers only), about twenty short emulator runs with memory writes (scratch copy of `tools/emu`, no repo change; save states stay in the gitignored `build/emu/`). Findings: scene = table of function pointers indexed by `step`; dialogue scripts are SERIFU disc files with three parallel tables and 20 speak codes; wipe kind = `month % 7` (16 and 18 frame wipes, 32 frame fades); portrait sector to BUSTUP file mapping; choice shuffle rule confirmed. Nothing under `src/`, `include/` or `config/symbol*` changed.

## [2026-10-24] ticket | T-27015 review round 1 fixes
Fixed the two blocking review findings of [[tickets/T-27015-p2-m-dates-b]] and the non-blocking ones; branch rebased on `w2-good` (log and spec index conflicts only). `config/renames/proposals/m-dates-b.tsv` is now 255 rows (2 file, 20 func, 75 comment, 67 data, 27 field, 64 const, 14 HYPOTHESIS). Name clashes: the CD level meter uses the m-dates-a names (`snd_cd_level`, `snd_cd_level_right`, `precision_bits`, `options`, `snd_cd_level_samples`); `D_800B5A60` follows m-hud (`mouth_frames_reversed`, partner `mouth_frames_reversed_saved`); `ActorSlot` 30 and 31 take the m-hud names; the three SD sector tables are dropped (m-weekday and m-holiday name them); the actor-96 and 97 alias labels use the m-weekday pattern. Spec fix: the event picture mouth flag at `D_80122D10` has three modes (0 voice-driven lip sync, 1 periodic animation, 2 neither) in [[spec/m-dates-b]] sections 3, 6 and 9 and in the table rows. Also: usage counts recomputed from the data asm and the C sources, `dec_init` header test and the `nn` skip rule corrected, speak-code names without game-meaning words, `include/main/scene.h` file row, four new open questions (unowned `D_80125C04`, m-menutext name code overlap, `dialogue_` versus `talk_` prefix, mood flag pair). Code reading only; no `src/`, `include/` or symbol file touched. The branch was rebased again on the new `w2-good` tip (it had moved to include m-weekday), and three statements of [[spec/m-hud]] and one of [[spec/m-dates-a]] that described the name conflicts as open now say they are resolved.

## [2026-10-24] ticket | T-27015 Done after review
[[tickets/T-27015-p2-m-dates-b]] passed the round 2 review at the `w2-good` tip 9404a703a (approved, no findings): no row clashes with a merged proposals table, the CD level meter, mouth flag, actor slot and actor-96 alias rows follow the m-dates-a, m-hud and m-weekday names, and the `sd_file_sector_*` rows are gone. Tables and wiki only; nothing under `src/`, `include/` or `config/symbol*`. Moved to Done; the board card is moved by the kanban sync in the merge step.

## [2026-10-24] build | T-27003 O.BIN and SDK names carried into the overlays
[[tickets/T-27003-pilot-obin-names-overlays]] (In Review, branch `p2-pilot`): the first real run of `tools/rename.py`. 104 O.BIN rows and 24 SDK rows (ledgers `config/renames/obin.txt` and `config/renames/sdk.txt`, tables in `drafts/T-27003/`) replaced 5,171 placeholder uses in 274 files; the overlay yamls and the `tools/gen_overlay_configs.py` template now load `config/symbol_addrs_obin.txt` and `config/symbol_addrs_sdk.txt` (the two files left `build/main_names.ld`); `config/proto_known.txt` lost 11 of 12 alias rows. The tool gained three rules: carried O.BIN/SDK names skip the style checks, an alias row merges the two `include/main_api.h` declarations, and `rename.py check` refuses a placeholder of a named main symbol. The build found 8 objects with a changed call view among about 64 dual declarations: `MAIN_API_OVERRIDE_` for `tpage_buf_clear`, `get_now_k`, `Vblnk_Timer_Init`, `Rot_2D`, `get_g_zyotai_s`, `search_tpage_multi`, one block-scope declaration (`set_dec_bri` in `func_80145AC4`, NAME_ENT), three types set to their main-exe definitions. The two-name address 0x80084C98 stays. G3 (`cd_read_status`) was already done by [[tickets/T-27002-rename-tool]]; `check_symbols.py` rejects EVENT-range names in the O.BIN and hypotheses files (new test). Every commit: clean build 27/27 sha1, 6,952/6,952 functions; tool suite OK; `funcdiff.py --resolve` MATCH on 13 sample functions. Procedure: [[rename-tool]] ("Alias rows"); effects on the build: [[build-system]], [[obin]]; plan: [[phase2]].

## [2026-10-24] ticket | T-27003 Done
[[tickets/T-27003-pilot-obin-names-overlays]] passed the standards and spec review (approved, no blocking findings) and moves to Done; five non-blocking follow-ups are in the ticket comments (stale names in `wiki/playbook.md` line 32 and `tools/redraft_extras.py`, one block-scope declaration in `src/ovl/NAME_ENT/80144AD0.c`, one placeholder-shaped file-local macro in `src/ovl/DATE2/80137D50.c`, the board card). Merging as p2-pilot through merge3.sh.

## [2026-10-24] ingest | Phase 2 m-holiday: names and port spec (T-27013)
Read the matched C of `src/main/80075320.c` (the mode 0x46 handler: holiday command screen, Sunday club flows, appointment place question, phone book, magazine), the data tables of the object and of its neighbours, and the proposal tables of the units merged meanwhile. Wrote `config/renames/proposals/m-holiday.tsv` (140 rows: 28 func, 22 data, 4 field, 73 const, 12 comment, 1 file; 2 HYPOTHESIS) and [[spec/m-holiday]]; ticket [[tickets/T-27013-p2-m-holiday]] is In Review.
Findings: the date place question is a memory test (`appt_place_0..3` = chosen, true, two decoys); appointment-word place values are 0-based; mode 0x60 starts at the first club activity; a holiday command pays four school-day gains for one school-day cost; phone slot 11 stands for the rival; the magazine hint is gated by the secret-code latch. Emulator probes (GDB on a title-menu state, forced mode 0x46) confirmed icon availability, command 0, the phone-book call, the dial shortcut, the place question, the first-club-activity scene, the Sunday join list and the practice-match path. Open questions: [[spec/m-holiday]] section 15.

## [2026-10-24] ticket | T-27013 Done
[[tickets/T-27013-p2-m-holiday]] passed review (approved; four non-blocking findings recorded in the ticket Comments: `get_g_zyotai_s` instead of the nonexistent `girl_state`, the duplicate `HOLIDAY_GAIN_MULTIPLIER` / `PARAM_HOLIDAY_FACTOR`, the header note on the `file` row, and open question 1 contradicting `core.tsv` on 0-based place values). The branch changes only `config/renames/proposals/m-holiday.tsv`, [[spec/m-holiday]], [[spec/index]] and wiki pages; nothing under `src/`, `include/` or the symbol files. Merged as p2a-m-holiday.

## [2026-10-24] ingest | m-menutext phase-2 table and port spec
Unit m-menutext (`src/main/8004E500.c`, `src/main/8004F6B0.c`, `src/main/8004F870.c`, 61 functions): read the matched C, its callers and callees, the core and scheduler tables, and probed behaviour on the emulator (title-menu save state, GameState bytes poked, a copy of the probe runner with a `--poke` option kept in `build/scratch`): text entries, a dialogue block and its six-line scroll, menu navigation on the settings screen in menu modes 1 and 3, voice gating, the name builders for all 16 girl ids through TEL lines loaded by the overlay loader, and the girl score pipeline checked against a Python model. Wrote `config/renames/proposals/m-menutext.tsv` (150 rows: 10 func, 53 data, 3 field, 5 file, 56 const, 23 comment) and [[spec/m-menutext]], linked from [[spec/index]]; ticket [[tickets/T-27018-p2-m-menutext]] In Review.
Findings: a text screen is a 52-entry list whose glyph slots are consecutive, so the mark/reset helpers rewind the glyph allocator; a dialogue block is a run of entries with a three-row window that scrolls from the fourth line; the player form in dialogue name codes always comes from the current girl; the two girl rank functions differ by design; girls 13 and 14 score as 0 and 1; the closeness grid and points formulas of `girl_scores` match the original for 11 girls in two poked scenarios; the spare name slot 15 is girl 10's given name. Disagreements with core are listed in the spec (section 16), not as second table rows.

## [2026-10-24] ticket | T-27018 review round 1 fixes
Fixed the three blocking review findings of [[tickets/T-27018-p2-m-menutext]] and the non-blocking ones in `config/renames/proposals/m-menutext.tsv` (153 rows: 10 func, 54 data, 4 field, 7 file, 54 const, 24 comment; 4 start with HYPOTHESIS), [[spec/m-menutext]], `GLOSSARY.md` (new rows `closeness`, `points`) and one line of [[spec/m-scheduler]] (`girl_points_others`). Blocking: the `NAME_CODE_PLAYER` and `NAME_CODE_GIRL` rows are dropped (m-dates-b owns `NameCode`), the `TextSpeed` rows stay here and m-sysmenu must drop its duplicates; the names with unlisted prefixes now use `girl_`, `speaker_` and `mode_` (`param_`, `voice_`, `debug_` are not used); `girl_points_heroines` is `girl_points_others`, `GIRL_HEROINE_COUNT` is `GIRL_SCORED_COUNT`, the honorific strings are `girl_name_suffix_familiar` and `girl_name_suffix_polite`. Non-blocking: the Shiori minimum table is labeled at 0x800B4340 with `D_800B4341` as alias, `girl_scores` lists its ten call sites, cd state masks are written as values, the reset keeps the speed, `TEXT_SPEED_CLAMP`, the page-wrap and break-capacity wording, a comment row for `text_set_advance`, headers split by topic. Core renames (`text_block_first`, `text_block_end`, `color`, `ot_slot`) and the `func_800460EC` double name (core versus h-cdio) are requests in spec section 16. `rename.py plan` on the 80 func, data and field rows: 0 problems.

## [2026-10-24] ticket | T-27018 Done
[[tickets/T-27018-p2-m-menutext]] passed review round 2 (approved, no findings): the NAME_CODE rows are gone and the spec cites m-dates-b's `NAME_CODE_PLAYER_FORM` and `NAME_CODE_GIRL`, and the renamed names use the ADR main-map prefixes. One follow-up for the m-sysmenu merge: its three `TEXT_SPEED_*` rows duplicate this table's `TextSpeed` rows and must be dropped there (spec section 16 of [[spec/m-menutext]]). The branch changes only `config/renames/proposals/m-menutext.tsv`, [[spec/m-menutext]], [[spec/index]], [[spec/m-scheduler]] (one line), `GLOSSARY.md` and wiki pages; nothing under `src/`, `include/` or the symbol files. Merged as p2a-m-menutext.
## [2026-10-24] ingest | T-27017 phase 2 m-sysmenu: names and port spec for the player's room screens (src/main/8005A0B0.c)
[[tickets/T-27017-p2-m-sysmenu]] (In Review): the file behind the unit label "system menus, clubs, rumors, holiday prep" holds eight screens over the player's room picture: the weekly command screen (mode 0x34, stages 0, 1, 0xFF), the calendar check, the memory-card data screen, the system settings with the mascot list, club joining, the rumor report (mode 0x44) and the Christmas and New Year preparation (modes 0x42, 0x41). `config/renames/proposals/m-sysmenu.tsv` has 264 rows (92 func, 27 data, 1 field, 126 const, 16 comment, 2 file; 7 HYPOTHESIS); the spec is [[spec/m-sysmenu]].
Findings: the build step of the command screen picks usable icons by tomorrow's calendar marker (exam, club season, culture festival, school trip), the remedial week flag and health; weekday club joining cannot be reached (icon 4 is locked without a club); the data screen keeps a slot directory in the system file and a background write job; loading a first-day or ending-day save starts a new run; the click speed change always yields the minimum; the mascot menu starts one item off. Emulator checks used the HUD unit's command screen state: icon to stage mapping, settings flow, calendar, the three click spots in mouse-like mode, and the data screen through a real save and load on the virtual card (the scan is slow there; a load at an ordinary date rebuilds the command screen twice). `tools/rename.py plan` on the func, data and field rows: no collision. Unapplied O.BIN names (`system_config_*`, `data_save_load*`, `game_mode*`) are quoted in notes as evidence only.

## [2026-10-24] ticket | T-27017 m-sysmenu review round 1 fixes
[[tickets/T-27017-p2-m-sysmenu]] (still In Review): the reviewer found two blocking problems and several smaller ones in `config/renames/proposals/m-sysmenu.tsv` and [[spec/m-sysmenu]]. Fixed: the field row for the club flag byte is dropped (the scheduler and HUD tables already name `ClubRecord.flags` and `ClubFlagBits.quit`), so the table has 263 rows (0 field); the club activity affection bonus cites `CharFlags.b2` by its merged name `acquainted` and leaves its meaning to that table's HYPOTHESIS; two sound rows moved into the holiday unit's `UiSe`; `CLUB_JOIN_MESSAGE_*`, `RumorStage`, a separate `MascotMenuItem`, `system_data_flags_view`, and the card poll results merged into `CardResult` with the flow table's names; `room_click_window` marked HYPOTHESIS. Spec corrections: stage 0xFF is entered by NAME_ENT (twice) and the data screen load, not by OPTION; ETC has its own clone of the save init; the room background load is not shared with the weekday screen; foreign callee names now follow the flow and menutext tables; dialogue paraphrases cut to topics; open questions 1a (romaji helper stem) and 1b (`room_` owner) added. `tools/rename.py plan` on the 119 func and data rows: exit 0, 44 files, no problem (with and without the gcc proof).

## [2026-10-24] ticket | T-27017 Done
[[tickets/T-27017-p2-m-sysmenu]] passed review round 2 (approved, no findings): the `quit_flags` row is gone (the club flag byte stays `ClubRecord.flags` / `ClubFlagBits.quit`), `CharFlags.b2` is cited as `acquainted` with no meaning of its own, and the stage 0xFF, ETC init clone and `UiSe` sound rows match `src/main/8005A0B0.c`, `80144AD0.c`, `80139740.c`, `8013A550.c` and `80143240.c`. The branch changes only `config/renames/proposals/m-sysmenu.tsv`, [[spec/m-sysmenu]], [[spec/index]], the ticket and the log; nothing under `src/`, `include/` or the symbol files. Open follow-up for the table merge: the three `TEXT_SPEED_*` const rows duplicate [[spec/m-menutext]]'s `TextSpeed` rows and should be dropped from `m-sysmenu.tsv` then. Merged as p2a-m-sysmenu.
