---
type: log
---

# Log


Append-only. Entry format: `## [YYYY-MM-DD] <op> | <title>` where op is ingest, query, lint, ticket, build, decision, or setup. Use [[wikilinks]] for wiki files and plain paths (e.g. `src/main.c`) for everything else. Recent: `grep "^## \[" wiki/log.md | tail -5`.

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

## [2026-10-24] build | T-27004 shared core names applied
[[tickets/T-27004-shared-core-names]] (In Review, branch `p2-core-apply`, base `w2-good`): `config/renames/proposals/core.tsv` through `tools/rename.py` in five batches, each with a clean build (27 of 27 sha1 OK, 6,952 of 6,952 functions): 239 field rows (24,587 uses), 75 types, 18 const groups in the new `include/game_enums.h` (226 rows, 2,437 literals named), 92 func and 39 data rows (8 hypotheses in `config/symbol_addrs_hypotheses.txt`) and 61 English comments on O.BIN names; ledger `config/renames/core.txt`. 733 of 751 rows applied. Left out: the three `u54C` macro-alias rows and `DisplayActor44.unk_18` (a macro parameter named `frame`; both need tool work), the 12 Month rows (excluded by [[phase2]] step 2) and the two file rows (own commit later). Three `tools/rename.py` fixes with tests (`#line` mapping, `did you mean` ignored, HYPOTHESIS type rows in the round trip) and a `*.member` const site, documented in [[rename-tool]]. Prefix rows for the ADR main map are listed in the ticket for the review.

## [2026-10-24] ticket | T-27004 review fix: names brought to the ADR and glossary
[[tickets/T-27004-shared-core-names]] (In Review, branch `p2-core-apply`): the review found two blocking items, about 25 prefixes outside the ADR main map and glossary, and renames that contradict glossary stems. Fixed with 133 more `tools/rename.py` rows (`drafts/T-27004/fix_symbols.tsv`, `fix_types.tsv`, `fix_fields.tsv`; ledger `config/renames/core.txt`, 724 rows): the game mode machine is `game_mode_*`, `mode_stage_*`, `mode_step_*`; `bustup_*` is `portrait_*`, `card_*` is `memory_card_*`, `snd_*` is `audio_*`, `dialogue_*` is `talk_*`, `overlay_*` is `ovl_*`, the other unlisted prefixes took listed ones. The ADR main map, the glossary Module words and the glossary stems gained rows for `actor_`, `audio_`, `name_code_`, `scene_`, `game_state`, `system_data` and `place_` in the same commit. One clean build: 27 of 27 sha1 OK, 6,952 of 6,952 functions, no `INCLUDE_ASM`; `rename.py check`, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` OK, tool suite 1,646 tests OK, `funcdiff --resolve` MATCH. The `MainMode` enum and its `MODE_` constants stay: [[rename-tool]] has no row kind for them. `wiki/phase2.md`, `wiki/spec/core.md`, `wiki/rename-tool.md`, `GLOSSARY.md`, `docs/adr/0001-phase-2-naming.md` and `CODING_STANDARDS.md` refreshed.

## [2026-10-24] ticket | T-27004 Done after review
[[tickets/T-27004-shared-core-names]] passed the round 2 standards and spec review at the `p2-core-apply` tip 44aaa8114 (approved, no blocking findings) and moves to Done. Two non-blocking follow-ups are in the ticket comments: the `MainMode` enum tag and `MODE_*` enumerators still conflict with the glossary stem `game_mode` and need a tool row kind for enum tags and enumerators plus one rename batch, and six non-O.BIN names carry the O.BIN stem `dec_bg_` where the ADR map has `bg_`. No follow-up ticket exists yet, and [[phase2]] "Names to re-check" does not mention either item.

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

## [2026-10-24] ingest | Phase 2 analysis of the main flow unit (T-27019)
Read `src/main/80041000.c`, `800420D0.c`, `80042540.c`, `80042A00.c`, `800789E0.c`, `80061710.c` and `80053650.c` (115 functions: boot and `game_state_init`, the frame loop, the mode dispatcher and overlay loader with its sector and entry tables, the TT loader, pad and mouse polling, the title screen, the memory-card layer) with their callers, and probed the emulator from the title menu (title timings, the pad code, the soft reset, the loader read, the card check, initial game data). Wrote [[spec/m-flow]], `config/renames/proposals/m-flow.tsv` (312 rows, 3 HYPOTHESIS) and [[tickets/T-27019-p2-m-flow]] (In Review). Found: core's `MainMode` entries for modes 0x74-0x76 are in a different order than the loader code; `unk_1117` is a card bank index; the title pad code starts the opening movie; the loader skips the read for a resident file.

## [2026-10-24] ingest | m-flow table and spec completed (T-27019)
Finished [[spec/m-flow]] and `config/renames/proposals/m-flow.tsv` (324 rows: 87 func, 48 data, 44 field, 1 type, 10 comment, 14 file, 120 const; 3 HYPOTHESIS rows; every one of the 115 functions has a name, 28 from core, the patch or O.BIN). Added the eight card event handles, the ordering-table alias labels and the mode request table (section 3.9) after the first commit; the title pictures are `photo`, not `card`, so `card` stays the memory card. `tools/rename.py plan` on the func, data, field and type rows reported no problem. Open points for the reviewer are in section 10 of the spec (mode 0x74-0x76 entry order in core, `unk_1117` as a card bank, the shared `CardResult` names with m-sysmenu, prefix rows of the ADR and glossary).

## [2026-10-24] ticket | m-flow review round 1 fixes (T-27019)
[[tickets/T-27019-p2-m-flow]] review blocked on missing HYPOTHESIS marks. Added them to seven rows of `config/renames/proposals/m-flow.tsv` (the five overlay-folder `OvlLba` constants that the ADR map keeps as `confirm`, `CARD_RESULT_IO_ERROR`, `extra_mode_flags`) and to the matching places in [[spec/m-flow]] (3.4 table and note, 3.7, 6.1, 7); the table now has 10 HYPOTHESIS rows of 324. Applied the review's wording findings (`card_file_exists` open counts, the pad code window of 15 frames between steps and 241 frames set, the title idle clock restarting on every up or down press, the callers of `pad_update`, the unnamed display call in TT stage 2, the `pad_raw_read` no-device path) and added merge notes as spec open questions 11 and 12 (`D_80122D44` name versus the o-date table, shared-name disagreements, alias rows). No `src/`, `include/` or symbol-file change; the ticket stays In Review.

## [2026-10-24] ticket | T-27019 Done
Review round 2 of [[tickets/T-27019-p2-m-flow]] approved with no findings: the seven HYPOTHESIS marks added in `0cdb9358c` (five overlay-folder `OvlLba` constants, `CARD_RESULT_IO_ERROR`, `extra_mode_flags`) are in `config/renames/proposals/m-flow.tsv` (324 rows of 5 columns, 10 marked HYPOTHESIS) and in [[spec/m-flow]] (3.4, TT stage 0, 6.1, 7), and the reworded `card_file_exists` open counts, `pad_code_update` window and title idle clock match the C. The branch changes only the table and wiki files (nothing under `src/`, `include/` or `config/symbol*`). Ticket set to Done; branch p2a-m-flow goes to the merge into w2-good. Merging the table into the rename tool input stays open on the ticket.

## [2026-10-24] ingest | T-27022 phase 2 naming proposals and port spec for overlay DATE (dates)
Unit o-date (`src/ovl/DATE`, 668 functions, 19 files): read the matched C, the 72 step tables of the overlay data and the callers in main, grouped the 300 mechanical step functions by pattern and checked behaviour in about twenty short emulator runs from a boot-to-title state (a scratch copy of the emulator wrapper with a memory-poke option, kept in `build/`, wrote mode, girl, place, calendar and parameter values). Wrote `config/renames/proposals/o-date.tsv` (1,120 rows, 18 with a HYPOTHESIS mark) and [[spec/o-date]] under [[tickets/T-27022-p2-o-date]] (In Review). No `src/`, `include/` or symbol-file edits.
Findings: mode 0x50 has four stages (arrival, place, girl birthday gifts, player birthday); the 21 places share one step template and the answer is scored `n x trivia tier x place affinity / 16` per counter (checked in the emulator); a meeting-place question arises when the chosen place differs from the true appointment place; `date_end` counts the visit, stores the last date spot (looks >= 35, or >= 30 except Mira), applies the Golden Week and festival bonuses and the -40 syoushin; a third date in a row at one place from 1996 starts the street-gang ambush; mode 0x5E hosts 48 special date events (11 girls x 4-5 seasons, plus two Miharu intrusions) with a trigger table and fixed rewards.

## [2026-10-24] ticket | T-27022 review round 1 fixes
Fix pass on [[tickets/T-27022-p2-o-date]] after the Opus review (not approved, three blocking findings). `config/renames/proposals/o-date.tsv` (now 1,103 rows): the main-symbol rows left the unit table (`D_80122D44` is `scene_effect_flags` from m-flow; the other 17 are 18 request rows in `config/renames/proposals/o-date-main-requests.tsv`, including a `DialogueWork` field row for the launch code); every `event` name became `scene` (about 450 tokens, the kept patch names `date_event_trigger` and `date_event_choose` are rename rows now); the five `narration` names carry HYPOTHESIS and the other notes say what the code shows (id 14, core `GIRL_SOTOI`); content errors in three notes fixed (special reaction lines, sea not covered, looks thresholds); neutral names for the festival portrait loader, the park talk cursors and the literal-bearing step names; alias row `func_80132004`. `rename.py plan` over the func, data, type and field rows: 0 problems. [[spec/o-date]] updated to match (section 3 requests, 5.6, 6, 13 run table with every memory write, 14 notes on mode 0x5C and id 14, 15 open questions 8-10); [[spec/index]] says 48 special date scenes. No source, header or symbol file touched. The ticket stays In Review for the second review.

## [2026-10-24] ticket | T-27022 Done after review
[[tickets/T-27022-p2-o-date]] passed review round 2 (approved, no findings): the `scene_effect_flags` writers in [[spec/o-date]] match the C, no new name contains the word `event`, and the five `narration` names carry HYPOTHESIS. Tables and wiki only, nothing under `src/`, `include/` or `config/symbol*` (checked against the merge base). Merged as p2a-o-date.

## [2026-10-24] ingest | Phase 2 o-bunkaken: names and port spec (T-27026)

Unit BUNKAKEN (`src/ovl/BUNKAKEN/`, 18 files, 201 functions, main mode 0x71, the culture festival for players outside the six culture clubs): read the matched C, the overlay data tables, the disc script, background, portrait and sound files by sector, the core and scheduler spec tables, and probed behaviour in the emulator (title-menu save state, loader forced to mode 0x71, a local copy of the probe runner with `--poke` and `--sample`, kept in `build/scratch`, not committed). Wrote `config/renames/proposals/o-bunkaken.tsv` (457 rows: 201 func, 172 const, 40 data, 19 file, 15 field, 10 type; 2 start with HYPOTHESIS) and [[spec/o-bunkaken]], linked from [[spec/index]]; ticket [[tickets/T-27026-p2-o-bunkaken]] (In Review). Prefix `festival_visit_`.
Findings: the overlay is a table of 142 stage handlers (13 of them routers) run under an 11-phase entry; the visit is invitation (best-liked girl above 50 tokimeki and 80 affection), companion picker, six-club menu and one of 122 exhibit scenes chosen by club, year and girl; the only state it changes is the companion's affection and tokimeki (year tables, 9 + 11 + 11 rows), the refused girl's affection and syoushin, two birthday scenes, the `acquainted` bit and the event log; the festival words live inside the 0x100-byte `overlay_scratch` that the entry zeroes at the end; BUNKASAI differs by scoring from the player's own club and by setting the follow-up call flags.

## [2026-10-24] ticket | T-27026 fix pass after review round 1
Fixed the three blocking findings of [[tickets/T-27026-p2-o-bunkaken]] and the non-blocking ones. (1) Prefix: the table now uses the ADR row for BUNKAKEN, folder `culture_festival_tour`, prefix `tour_`, types `Tour*`, constants `TOUR_*`, header `include/ovl/culture_festival_tour.h` (the six-club menu and the visit structure confirm the "confirm" row; ADR and GLOSSARY are not edited). The nine shared festival words of main scope now carry the `culture` stem and agree with the BUNKASAI table where it names the address (`culture_phase`, `culture_exhibit_club`, `culture_visitor_girl`, `culture_show_variant`, `culture_return_stage`, `culture_return_step`, `culture_fortune_result`); D_80122E98 is `culture_display_skip` (BUNKASAI: `culture_default_disp_off`) and D_80122EA4 is `culture_refused_girl`. The TT flag bits use the o-tt names `direct_mode` and `stage_select`. (2) Abbreviations: `lit`, `comp`, `y95`-`y97`, `min`, `wrapup`, `fn` and `bustup` are spelled out (`literature`, `computer`, `1995`-`1997`, `minimum`, `wrap_up`, `handler`, `portrait`); `FV_` became `TOUR_`. (3) Spec 5.2 and the note of `tour_invitation_pick_girl` rewritten: girl 0 is gated by weighted affection only and seeds the running maximum, so a Shiori with affection of 80 or more and tokimeki under 50 blocks every girl with a smaller or equal sum; added to the quirks and confirmed in the emulator (title-menu save state, loader forced to mode 0x71; Shiori raw affection 100 and tokimeki 45 with girl 4 at sum 115: stage 1 and no invitation; girl 4 alone: companion 4). Also fixed: group wording of 5.3 steps 31-41 (second club), the per-route list of the solo fallback in 5.5, `acquainted` and `festival_call_95/96` cross-unit notes, setter note names (`tour_invite_script_setter[n]`), `tour_fortune_wait_counter`. Correction to the entry above: the table has 1 row that starts with HYPOTHESIS, not 2. `tools/rename.py plan` on the func, data, field and type rows: 266 rows, no problem (const and file rows are applied by other subcommands). The earlier entry keeps the name `festival_visit_` as history.

## [2026-10-24] ticket | T-27026 Done after review
[[tickets/T-27026-p2-o-bunkaken]] passed review round 2 (approved, no findings): all 457 rows follow the ADR row for BUNKAKEN (`culture_festival_tour`, `tour_`, `Tour*`, `TOUR_*`), the nine main-scope festival words carry the `culture` stem, and the abbreviations `lit`, `comp`, `y95` to `y97`, `min`, `wrapup`, `fn` and `bustup` are spelled out. The one open point is `tt` in four names, which is the TT overlay's own stem and not yet a glossary row (not blocking). Tables and wiki only, nothing under `src/`, `include/` or `config/symbol*` (checked against the merge base). Merged as p2a-o-bunkaken.

## [2026-10-24] ingest | T-27023 phase 2 analysis of overlay EVENT
Analysis of `src/ovl/EVENT` for [[tickets/T-27023-p2-o-event]] (phase [[tickets/T-27000-phase-2]]). The overlay is a dead development copy of the girl-scene, place-date and ride-movie code, linked against another build of the main program: none of its 135 main-side call targets is a function start of the shipped exe, 53 lie below the shipped text, its script loaders match the shipped `SERIFU/EVENT_xx.BIN` and `DATE_xx.BIN` sector counts for 10 of 12 and 10 of 11 files. Written: `config/renames/proposals/o-event.tsv` (1126 rows, passes `rename.py plan`), `o-event-devmain.tsv` (121 aliases of stale main addresses, which the tool refuses as overlay rows) and `o-event-deferred.tsv` (6 data names declared in `include/main_api.h`); the port spec [[spec/o-event]] and its step-table appendix [[spec/o-event-steps]]; the line in [[spec/index]]. Nothing under `src/`, `include/` or `config/symbol*`. Answers the EVENT half of [[tickets/T-0200-event-gyozi-loader-and-address]] (no loader exists). Ticket In Review; the board card is moved by the kanban sync.

## [2026-10-24] ticket | T-27023 review round 1 fixes
Fixed the three blocking findings of the review of [[tickets/T-27023-p2-o-event]] (still In Review) and the non-blocking ones; only tables and wiki pages changed (`config/renames/proposals/o-event.tsv`, `config/renames/proposals/o-event-devmain.tsv`, [[spec/o-event]], the ticket). Rule status: EVENT's outcome score and close step differ from the shipped DATE, so [[spec/o-event]] has a new section 7.0 (12 differences, DATE authoritative: factor b of the outcome score, the destination of the third term, the tokimeki bonuses and the 0x28 reduction in the close step, the repeat-reaction conditions, one athletics threshold, outcome ids one lower, the ride latch, a wrong girl index in the Yuina park bonus), and sections 7.5, 7.6, 10 and the function notes say so. The ride variant is bit 0 of the girl's event counter of the ride (the "ride-seen bit" reading is gone, game meaning HYPOTHESIS), and the other place variants use the same counter parity. EVENT's private views use core's names (`syoushin`, `event_counters`, `girl_last_date_spot`, `saved_mode/stage/step`, `appt_place_0`, `appointment_flag`, `pad_pressed`, `screen_center_x/y`), with a dev-to-shipped offset table in spec section 9; two names stay open between units (spec section 11). The table has 1136 rows (49 field rows, 17 whose note starts with HYPOTHESIS); `tools/rename.py plan` over its 1099 non-file/dir/const rows exits 0 with no PROBLEM line (`build/` output, not committed). Counts reworded and the 675 recount (closest sibling in DATE 559, GEKO 87) are in the spec; 25 position-only step names got behaviour notes.

## [2026-10-24] ticket | T-27023 Done after review
[[tickets/T-27023-p2-o-event]] passed the round 2 review (approved, no findings): the three blocking findings of round 1 are resolved, [[spec/o-event]] section 7.0 states that the shipped DATE is authoritative where EVENT's outcome score and close step differ, and the rows and spec lines of the fix pass match the C. Tables and wiki only; nothing under `src/`, `include/` or `config/symbol*`. Moved to Done; the board card is moved by the kanban sync in the merge step.

## [2026-10-24] ingest | T-27029 phase 2 o-etc: names and port spec for the ETC overlay
[[tickets/T-27029-p2-o-etc]] (In Review): read the matched C of all 13 files of `src/ovl/ETC` (394 functions), the callers in main, ENDING and OPTION, the overlay data tables and the core, scheduler and sysmenu tables. Wrote `config/renames/proposals/o-etc.tsv` (779 rows: 394 func, 233 data, 114 const, 18 field, 14 file, 6 type; 5 HYPOTHESIS) and [[spec/o-etc]]; one line added to [[spec/index]].
Findings: ETC holds seven entries, final exams (0x35), career guidance (0x36), career choice with a 10-question entrance exam (0x37), staff roll (0xC2), last photo (0xC3), clear save (0xC4) and the photo album (0x91). The exam day score is main + sub + biorhythm - 8 (halved when neurotic or sick) plus a correction bonus that shrinks per round, scaled by 100/128; three red marks start the remedial week (player flag bit 16, calendar marker 0x17). The results ranking draws noisy values per girl and a cosine rank number. The career result byte read by ENDING has bits 0-5 (accepted tier), 0x40 (club dream) and 0x80 (failed). `girl_cleared` is a per-girl ending counter incremented by the clear save. About 30 functions are dead code (three developer stub scenes, two skip-week helpers, an album fixture, three animation table setters). Emulator checks used a local poke wrapper (not committed): printed score values and screens matched the spec.

## [2026-10-24] ticket | T-27029 Done
[[tickets/T-27029-p2-o-etc]] passed review (approved, no blocking findings); the branch changes only `config/renames/proposals/o-etc.tsv`, [[spec/o-etc]], [[spec/index]], the ticket and the log. Fixed at merge: `album_event_log_pack` copies every `event_log` record whose girl equals `current_girl & 0xF` (no per-record date test); `album_anim_0d_show` and `album_anim_2d_show` call the 0C and 2C `tables_set` functions and `album_anim_4b_show` sets no tables. `core.tsv` now names the event log record bytes `scene` (+1) and `category` (+2) (`event_log_add(category, scene)` stores arg 0 at +2, arg 1 at +1); [[spec/core]] and [[spec/m-dates-b]] wording synced. Follow-up: `album_anim_unused_a_ptrs` is read by scene 0x4B. Merged as p2a-o-etc.

## [2026-10-24] ingest | NAME_ENT phase 2 analysis (T-27030)
[[tickets/T-27030-p2-o-nameent]]: naming table and port spec for the overlay NAME_ENT (file screens, name and birthday entry, voiced prologue, attract opening). `config/renames/proposals/o-nameent.tsv` has 639 rows (255 function, 228 data, 144 const, 5 file, 4 field, 3 type; 20 marked HYPOTHESIS); [[spec/o-nameent]] gives the four modes (0x11 opening, 0x13 file screens, 0x14 name entry, 0x15 prologue), the stage and step tables, the new-game formulas and the prologue script structure. Checked in the emulator (mode sequence, setup values, SELECT chord preset). Prefix decision: `name_entry_`, `prologue_` from the ADR plus `file_select_` and `opening_`. Nothing under `src/`, `include/` or `config/symbol*` changed.

## [2026-10-24] ticket | o-nameent review round 1 fixes (T-27030)
[[tickets/T-27030-p2-o-nameent]]: fixed the `file_select_scan` slot range (`slot_status[1..14]`, album 14 counts), the icon layout reset condition, the file counts (62 and 82), the GLOSSARY and ADR rows (`uv`, `file_select`, `opening`, `prologue`), `ui`/`dx`/`dy` names, MainMode merge notes, the cheat string addresses, and marked 15 scene-content names HYPOTHESIS (35 rows start with HYPOTHESIS in `config/renames/proposals/o-nameent.tsv`, 639 rows). `tools/rename.py plan` over func, data, type and field rows: 0 problems. See [[spec/o-nameent]].

## [2026-10-24] ticket | T-27030 Done
Review round 2 of [[tickets/T-27030-p2-o-nameent]] approved with no findings: the `file_select_scan` slot range (`slot_status[1..14]`, album 14 counts) and the spec 3.9 icon layout reset condition now match `src/ovl/NAME_ENT/80139740.c`; `config/renames/proposals/o-nameent.tsv` has 639 rows of 5 columns, file counts 62/104/9/80 by address (257 functions with the two kept `prologue_line_*`). The branch changes nothing under `src/`, `include/` or `config/symbol*`. Ticket moved to Done and merged by the phase 2 merge step.

## [2026-10-24] ingest | Phase 2 o-bunkasai: names and port spec (T-27025)
Unit `o-bunkasai` ([[tickets/T-27025-p2-o-bunkasai]], In Review): read the matched C of the BUNKASAI overlay (207 functions, 20 files in `src/ovl/BUNKASAI/`), its callers in the main exe and in BUNKA_SD, the TT spec and the core, scheduler and scene-library specs. Wrote [[spec/o-bunkasai]] and `config/renames/proposals/o-bunkasai.tsv` (prefix `culture_`). Findings: the overlay is the club-member festival (mode 0x70 for player club ids below 6; BUNKAKEN is the rest); it is one phase machine (10 phases) plus a 165-entry stage table of 15 blocks (visit blocks per heroine, four club-mate blocks, a shared fortune stage, a TT game launcher and its re-initialisation stage); the festival result adds a club-derived bonus plus half a 13x6 gain-table value to every girl's tokimeki and 2 affection to the visitor; the computer-club 1996 exhibit launches the TT game in demo mode and unlocks it; girl 0's birthday scene hangs on the end of the festival. The overlay is reloaded after every sub-scene, which is why its scene variables sit in main bss. Emulator probes (memory pokes into the title-menu state) confirmed the phase and stage order, the result formula, the fortune table and the birthday path. The table has 360 rows (208 func, 67 data, 8 field, 10 type, 21 file, 46 const; the `rename.py plan` dry run passes for func, data, field and type rows); 78 emulator runs reproduced the stage tables. Names go through `tools/rename.py` later; this pass wrote tables and wiki pages only.

## [2026-10-24] ticket | T-27025 review round 1 fixes
Fixed the five blocking findings of the review of [[tickets/T-27025-p2-o-bunkasai]] (still In Review) after merging w2-good (`2c825bfa1`) into the branch. `config/renames/proposals/o-bunkasai.tsv` lost its 16 main-scope rows (11 data and function rows, 5 fields): `func_80041168` is `vram_clear_blocks` (m-flow), `D_800E7D4F` is `extra_mode_flags` (m-flow), `D_80122E98` is `culture_display_skip`, the three bits are `stage_select` (BUNKAKEN), and the other rows were identical copies of BUNKAKEN's, so the merge step has one row per main address. Abbreviations written out: `CultureFortuneBloodTable` and `culture_fortune_blood_table` (was compat), field `stage_function` (was fn). `CULTURE_PHASE_GATE_*` became `CULTURE_PHASE_BACKGROUND_*` (no run showed the picture). `culture_unused_girl_bytes` lost its `girl`. Notes now mark `acquainted` and `festival_call_95/96` as m-scheduler HYPOTHESIS names; [[spec/o-bunkasai]] uses the merged names, counts 133 pattern scene functions (plus the stage-78 stub and the Ayako follow-up) and marks the `tt_unlocked` meaning as HYPOTHESIS. Six rows were added to `GLOSSARY.md` (exhibit, club mate, visitor, fortune telling, backstage, tokimeki gain). `rename.py plan` with the gcc proof over the 277 func, data, field and type rows reports no problem. The table has 344 rows; nothing under `src/`, `include/` or `config/symbol*` changed.

## [2026-10-24] ticket | T-27025 Done after review
[[tickets/T-27025-p2-o-bunkasai]] passed the round 2 review (approved, no findings): the five blocking findings of round 1 are resolved (merged m-flow and BUNKAKEN names for `func_80041168`, `D_800E7D4F`, `D_80122E98` and `stage_select`; abbreviations written out), and no new error was found in the rows or lines the fix touched. The branch changes only `config/renames/proposals/o-bunkasai.tsv`, [[spec/o-bunkasai]], `GLOSSARY.md` and wiki pages; nothing under `src/`, `include/` or `config/symbol*`. Ticket Done; the board card moves in the merge step.

## [2026-10-24] build | T-27004 replay fix for member chains across a type rename
Merging `p2-core-apply` into the integration tree failed in `tools/rename.py replay` (11 member rows such as `GirlRecord unk_00 -> tokimeki_frac`): `chains()` keys member rows by the type name of the row, so a chain that crosses the type rename `Rec38` to `GirlRecord` was cut in two. `tools/rename.py` now puts the field rows under the type's current name before replay builds its chains (`link_member_rows`), with the test `test_replay_field_chain_across_a_type_rename`; `w2-good` merged into the branch (conflicts only in `wiki/log.md` and `wiki/spec/core.md`), rebuilt 27 of 27, 6,952 of 6,952, checks and the 1,647 tool tests OK. Details in [[tickets/T-27004-shared-core-names]] and [[rename-tool]]. The tool change came after the round 2 review.

## [2026-10-24] decision | Phase 2 handoff after the core apply
T-27003 and T-27004 are merged (27/27). [[status]] handoff now lists them as applied, the leftover enum-tag rename, and that the reconcile pass T-27009 must map the older proposal tables with `rename.py lookup`. Phase 2 stays paused for weekly usage (95%).

## [2026-10-24] ticket | Resume phase 2 with Luna and Sol 6.1

Created [[tickets/T-27031-p2-o-taiiku]] through [[tickets/T-27037-p2-o-small]] for the seven missing analysis units. First batch claims sports festival, girl encounters and New Year/Christmas; remaining units are Ready. User model preference supersedes the historical Sonnet/Opus assignments. Shared wiki edits and commits are serialized by the orchestrator.

## [2026-10-24] query | Phase 2 continuation baseline

[[phase2]] now records the user's Luna/Sol 6.1 assignment, superseding historical model roles. [[status]] records the first three active units and proposal audit: 24 TSVs, 10,085 rows, no malformed rows, 135 conflicting keys and four same-kind/scope target collisions. Docker checks: headers OK; wiki lint zero broken links and zero status mismatches (one pre-existing oversized page). Initial progress failed on stale generated pre-rename asm names; configure and ninja refresh are underway. No source changes or new match claims.

## [2026-10-24] ticket | Record reconciliation preflight

Created [[tickets/T-27009-p2-reconcile]] as Ready with measured proposal conflicts, applied-core precedence and the required separation of executable batches from comments, enum suggestions, deferred rows and file renames. Source census: TAIIKU 213, GEKO 391, SHOUGATU 400 and GYOZI 417 C definitions. First Luna drafts are partial; agents are correcting unsupported names and documenting coverage, without Done claims.

## [2026-10-24] lint | Consolidate the port-spec index

[[spec/index]] now has one introduction and preserves all 21 existing unit links; removed repeated headings and boilerplate introduced by historical parallel merges. No spec content or source names changed.

## [2026-10-24] decision | Prioritize a playable port reference

User clarified that phase 2 should make code decently readable enough to start the Godot port; emulator, save files and debugger use are authorized. Created [[tickets/T-27040-school-day-readable-batch]] for nine school-day helpers plus seven formula parameter/local roles, requiring independent review and clean match proof before acceptance. Claimed [[tickets/T-27034-p2-o-tel-option]] for Luna while the first overlay drafts await review. Refreshed build succeeds; independent current binary hashes are 27/27. The progress scan still sees stale historical asm files and needs a clean regeneration.

## [2026-10-24] decision | Separate port-start readiness from the naming backlog

[[phase2]] now records the clarified immediate outcome: prioritize readable state, day progression, command effects and the first gameplay slice's scene rules rather than treating every remaining helper name as a prerequisite to starting Godot. Emulator/save-file/debugger evidence is authorized and must remain reproducible and excluded from committed game data. T-27040's sixteen-row school-day batch is under independent Sol 6.1 review and rename-tool preflight.

## [2026-10-24] build | Apply the reviewed school-day readability batch

Independent Sol 6.1 table review approved all 16 rows of [[tickets/T-27040-school-day-readable-batch]]. rename.py plan/apply passed, ledger config/renames/school-day.txt appended by the tool, and rename check reports 740 ledger rows. Nine helper functions and seven parameter/local roles are readable; arithmetic is unchanged. [[spec/m-weekday]] uses applied names. Clean Docker build and census are running; final verification and two-axis code review remain open.

## [2026-10-24] lint | Review the applied weekday batch

Independent Luna standards review found no hard violations or new smells in c3c830094...6036b5a86. Independent Sol 6.1 spec review found one P3 stale-argument-name passage in [[spec/m-weekday]]; corrected it to command, holiday_factor and result_divisor, including flag-to-factor normalization and stress usage. Removed copied Japanese title terms from the uncommitted seasonal draft, retaining English behavior descriptions. Clean build continues.

## [2026-10-24] ticket | School-day batch to In Review with full proof

[[tickets/T-27040-school-day-readable-batch]] to In Review. Clean Docker build exited 0 with all 27 hashes, independently rehashed 27/27; census 6,952/6,952 functions and 2,276,920/2,276,920 bytes. Luna standards review passed; Sol 6.1 spec finding fixed at a7a3dd221 and verified resolved. Five errors in uncommitted seasonal and telephone/options rule claims were sent back to their author for source-checked correction; those tickets remain In Progress.

## [2026-10-24] ticket | Finish the reviewed school-day readability batch

[[tickets/T-27040-school-day-readable-batch]] In Review -> Done after independent Standards and Spec reviews, the verified P3 documentation correction, clean 27/27 SHA1 and unchanged 6,952-function census. Applied code remains at the reviewed sixteen-row batch; pending overlay analyses and reconciliation are not credited as applied names.

## [2026-10-24] ingest | Preserve o-taiiku analysis checkpoint

[[spec/o-taiiku]] and config/renames/proposals/o-taiiku.tsv saved for T-27031: sports festival: partial body-backed transitions, input/movement and girl-stat rewards. Linked from [[spec/index]] and [[index]]. Ticket remains In Progress, with coverage and confirmation limits explicit. No overlay source names applied.

## [2026-10-24] ingest | Preserve o-geko analysis checkpoint

[[spec/o-geko]] and config/renames/proposals/o-geko.tsv saved for T-27032: geko encounters: five mode entries, walk-home/birthday rules, invitation selection and appointments. Linked from [[spec/index]] and [[index]]. Ticket remains In Progress, with coverage and confirmation limits explicit. No overlay source names applied.

## [2026-10-24] ingest | Preserve o-shougatu analysis checkpoint

[[spec/o-shougatu]] and config/renames/proposals/o-shougatu.tsv saved for T-27033: new year/christmas and gyozi: partial candidate, shrine, fortune and birthday rules. Linked from [[spec/index]] and [[index]]. Ticket remains In Progress, with coverage and confirmation limits explicit. No overlay source names applied.

## [2026-10-24] ingest | Preserve o-tel-option analysis checkpoint

[[spec/o-tel-option]] and config/renames/proposals/o-tel-option.tsv saved for T-27034: telephone/options: partial invitation eligibility, relationship effects and sound-test flow. Linked from [[spec/index]] and [[index]]. Ticket remains In Progress, with coverage and confirmation limits explicit. No overlay source names applied.

## [2026-10-24] query | Continuation handoff and review corrections

[[status]] now distinguishes completed [[tickets/T-27040-school-day-readable-batch]] from four unfinished analysis units and three Ready units. Luna authored bounded drafts; Sol 6.1 authored GEKO and independently audited seasonal/telephone rule claims. Six factual findings were source-corrected; the last contradictory stress sentence was replaced as requested. GEKO independent semantic review remains pending. Full phase 2 is not complete; next work is the port slice needs and [[tickets/T-27009-p2-reconcile]], respecting applied ledgers.

## [2026-10-24] lint | Final continuation catalogs and wiki checks

Regenerated [[tickets/catalog]]: 978 tickets, 598 sections, 3,106 functions. Fixed the new sports spec's broken GLOSSARY wikilink to plain GLOSSARY.md. All new proposal rows have five TSV columns; seasonal copied title terms were removed. The pre-existing data/t0018-cases size warning remains outside this task. No source changes followed the successful clean build and reviews.

## [2026-10-24] lint | Finish the local continuation checkpoint

Final wiki lint: zero broken links and zero ticket/kanban mismatches; only the pre-existing large-page warning. git diff --check is clean. Saved separate analysis commits f1da18ddd (sports), 8752deefc (GEKO), 028fa0a84 (seasonal) and f780ba05c (telephone/options), all explicitly In Progress. Applied weekday code is 6036b5a86 with documentation correction a7a3dd221; no source changes after clean proof. [[tickets/T-27040-school-day-readable-batch]] is Done; the broader [[phase2]] remains open. All subagents have handed off their outputs.

## [2026-10-24] ticket | Continue phase 2 toward completion

User requested continued work and notification on actual completion. Claimed [[tickets/T-27009-p2-reconcile]] for Sol 6.1 and [[tickets/T-27035-p2-o-master-ending]], [[tickets/T-27036-p2-o-bunkasd-shugaku]] for bounded Luna analysis. An hourly thread follow-up complete-tokimemo-phase-2 resumes verified work between sessions; no routine notifications, completion or required-action notifications only. Current completion criteria and clean 27/27 proof remain binding.

## [2026-10-24] ticket | Claim scheduler application and enum tooling

Created [[tickets/T-27041-scheduler-readable-batch]] for the reconciled scheduler batch and [[tickets/T-27042-enum-rename-support]] for the remaining enum rename blocker. Claimed [[tickets/T-27037-p2-o-small]] for Luna entry-flow analysis. Scheduler preflight currently resolves 40 body-backed rows; independent table review precedes application.

## [2026-10-24] build | Add checked enum-identifier rename support

[[tickets/T-27042-enum-rename-support]] implements enum tags with type rows and existing member names with enumerator rows. All 73 synthetic rename tests pass in Docker after correcting a new enumerator-vs-guard-suffix false positive. [[rename-tool]] documents the supported path; no game enum names applied. Independent review remains pending. [[tickets/T-27041-scheduler-readable-batch]] received independent Sol 6.1 table approval; evidence-note corrections are resolved before application.

## [2026-10-24] build | Correct enum tooling review findings and build scheduler

[[tickets/T-27042-enum-rename-support]] independent review found three P2 defects, now covered by synthetic regressions: declaration-scoped enum checks/replay, cross-header aliases and case-insensitive target collisions. All 76 rename tests pass in Docker. [[tickets/T-27041-scheduler-readable-batch]] apply passed with 780 ledger rows; clean Docker build and census are running. No enum game identifiers have been changed.

## [2026-10-24] build | Apply and document the scheduler names

[[tickets/T-27041-scheduler-readable-batch]] applies the approved forty-row table: all33 remaining scheduler placeholder functions, one pre-existing non-O.BIN name and six owned globals. rename.py plan/apply passed; [[spec/m-scheduler]] refreshed against ledgers with corrected last-picked-girl semantics. Clean build and final two-axis review remain pending. Enum tool review fixes were independently verified: all three P2 findings resolved and their three regressions pass.

## [2026-10-24] ticket | Expand the fan-out across chats

User explicitly requested many agents across multiple chats if necessary. Created [[tickets/T-27050-parallel-main-reconciliation]], [[tickets/T-27051-parallel-overlay-reconciliation]] and [[tickets/T-27052-phase2-completion-audit]] as disjoint coordination lanes. Worker chats may manage subagents, but only prepare/review assigned tables and specs; this primary chat remains the sole source applicator, build coordinator, committer and shared-wiki owner.

## [2026-10-24] ticket | Dispatch parallel chats and prepare game-mode enums

Three user-authorized chats now own disjoint main reconciliation, overlay reconciliation and completion/header planning lanes: [[tickets/T-27050-parallel-main-reconciliation]], [[tickets/T-27051-parallel-overlay-reconciliation]], [[tickets/T-27052-phase2-completion-audit]]. IDs and non-overlap rules are in their tickets and ignored orchestrator state. Created [[tickets/T-27043-game-mode-enum-names]] with 69 mechanical enum naming rows. Scheduler clean build passed 27/27 and 6,952/6,952; final spec corrections await narrow verification.

## [2026-10-24] ticket | Enum tooling In Progress -> In Review

[[tickets/T-27042-enum-rename-support]] In Review after 76 passing Docker synthetic tests, independent Standards review with zero findings and independent verification that all three Spec findings are resolved. The scheduler clean build also passed all27 hashes with the current tool checks.

## [2026-10-24] ticket | Enum tooling In Review -> Done

[[tickets/T-27042-enum-rename-support]] Done after 76 passing Docker synthetic tests, independent Standards review with zero findings and independent verification that all three Spec findings are resolved. The scheduler clean build also passed all27 hashes with the current tool checks.

## [2026-10-24] decision | Correct stale NAME_ENT mode meanings before acceptance

The completion-audit chat surfaced known NAME_ENT mode-label mismatches in the original core enum. [[tickets/T-27043-game-mode-enum-names]] prefix-only application was interrupted during check after its write, then its exact67owned files and new69row ledger were rolled back to the committed state. No other source edits were present. Revised table maps0x11toopening,0x13tofile selection,0x14toname entry and0x15toprologue using loader/spec evidence; fresh review precedes application. The verified scheduler batch remains committed and intact.

## [2026-10-24] ticket | Scheduler readability In Progress -> In Review

[[tickets/T-27041-scheduler-readable-batch]] In Review after clean Docker 27/27 SHA1, independent rehash27/27 and 6,952/6,952 functions. Standards found zero issues; Spec found two documentation issues fixed at c4151d9f6 and independently verified resolved. Normalized C/header diff matched the forty approved identifier substitutions exactly.

## [2026-10-24] ticket | Scheduler readability In Review -> Done

[[tickets/T-27041-scheduler-readable-batch]] Done after clean Docker 27/27 SHA1, independent rehash27/27 and 6,952/6,952 functions. Standards found zero issues; Spec found two documentation issues fixed at c4151d9f6 and independently verified resolved. Normalized C/header diff matched the forty approved identifier substitutions exactly.

## [2026-10-24] ingest | Checkpoint the last three analysis-unit drafts

[[spec/o-master-ending]], [[spec/o-bunkasd-shugaku]] and [[spec/o-small]] now exist with 10,7and13proposed function rows. Independent Sol6.1 audit corrected MASTER attribution/companion identity, ENDING audio-level purpose, DATE2/VALEN Shiori targets, VALEN candidate callback and KANGEI/OMIMAI entry dispatch. Verification resolved every finding; root clarified remaining flag-byte wording. Coverage remains partial and tickets In Progress. These drafts make all28unit spec pages present, not all28units complete.

## [2026-10-24] lint | Normalize new draft table syntax

Two Luna outputs used a bare header and function instead of the tool's func kind. Root normalized headers/kinds and leading hypothesis markers, then parsed all three new tables with tools/rename.py:10MASTER/ENDING,7stage/trip and13small-overlay rows. These remain documentary partial proposals, not application approval.

## [2026-10-24] decision | Prioritize logical files, topic headers and proven constants

User clarified that logical C filenames and header breakdown deserve explicit implementation time, and named constants should replace proven magic IDs/flags/thresholds throughout phase2. Shared priorities were sent to all three parallel chats: prepare separate supported file-move tables, exact header ownership/include plans and context-specific constants inventories, reusing core enums and leaving unclear literals alone. No blanket replacement or source restructuring is authorized in worker lanes; primary integrates with view and matching proofs.

## [2026-10-24] build | Apply corrected game-mode names

Revised [[tickets/T-27043-game-mode-enum-names]] collision-only dry plan passed69rows/67files; full apply then passed builtin gcc/prototype proofs and rename check (849ledger rows). Four NAME_ENT roles now read opening, file selection, name entry and prologue at0x11/13/14/15, and two stale source comments were corrected per independent review. Numeric values and ten existing hypothesis comments are unchanged. Clean Docker build/census are running; final proof and code review remain pending.

## [2026-10-24] ticket | Start engine-independent asset export lanes

User explicitly requested asset readers/exporters for the decomp, especially translation-ready text and standard image/audio outputs, while deferring direct Godot work. Existing T-0002 covers disc/build extraction only; created [[tickets/T-28000-asset-export-pipeline]] through [[tickets/T-28003-original-input-assets]]. [[asset-export]] records scope, primary SM64/OoT references and lossless/provenance requirements. Added .asset-cache/ to gitignore before extraction; build cleans must not delete export work.

## [2026-10-24] ticket | Dispatch text and general asset exporter teams

[[tickets/T-28001-text-script-export]] now has its own Sol6.1-led chat, while [[tickets/T-28002-media-asset-export]] and [[tickets/T-28003-original-input-assets]] share a separate media/input coordinator with disjoint module ownership. Both can delegate bounded Luna work and hard Sol review. No Godot project/integration is authorized. Hourly follow-up now preserves these user scope changes and tracks all five chat IDs without duplicate work; it reports actual phase2 completion separately from asset progress.

## [2026-10-24] build | Verify enum names and correct legacy callback annotations

[[tickets/T-27043-game-mode-enum-names]] clean Docker build passed all27hashes and6952functions. Corrected the three old MASTER callback-address comments against src/main/800789E0.c in addition to the reviewed NAME_ENT comments; an incremental rebuild follows those comment-only fixes. Refreshed primary-owned [[spec/core]], [[spec/m-flow]], [[spec/m-weekday]], [[spec/m-scheduler]], [[game-state]] and [[data-types]] to applied enum identifiers without editing other chats' active specs. Final code review is next.

## [2026-10-24] ticket | Queue logical files, header tooling and final Astra gate

Created [[tickets/T-27044-main-file-name-pilot]] and [[tickets/T-27054-topic-header-tooling]] for concrete structural implementation. User explicitly requested a phase-wide Astra reviewer near wrap-up followed by a separate Astra fix agent: [[tickets/T-27055-final-astra-review]] and [[tickets/T-27056-final-astra-fixes]] are Backlog final gates, not immediate expensive routine workers. [[phase2]] and parent criteria record the gate. Also resolved the final enum-spec finding by removing settled NAME_ENT aliases and updating MASTER prefixes in [[spec/core]].

## [2026-10-24] decision | Require comprehensive text and visual localization coverage

User clarified that all text must be handled masterfully for a comprehensive translation and every Japanese-text image asset must be listed, especially main-screen icons. The 44 unsupported SERIFU files are blocking backlog, not completion. Sent strengthened requirements to text/media chats and created [[tickets/T-28004-japanese-image-catalog]] for image-by-image source/frame/palette/context/review coverage. No translation or replacement art/Godot integration is started. Post-enum comment rebuild and independent hashes pass27/27 with6952functions.

## [2026-10-24] ticket | Game-mode enums In Progress -> In Review

[[tickets/T-27043-game-mode-enum-names]] In Review after clean27/27, post-commentrebuild27/27, independenthash27/27 and6952functions. Standards0findings; Spec1docfindingfixed2f0a3cef5 andverifiedresolved. All68valuesand10hypothesiscommentsunchanged; sourceis69approvedsubstitutionsplus5reviewedcommentfixes.

## [2026-10-24] ticket | Game-mode enums In Review -> Done

[[tickets/T-27043-game-mode-enum-names]] Done after clean27/27, post-commentrebuild27/27, independenthash27/27 and6952functions. Standards0findings; Spec1docfindingfixed2f0a3cef5 andverifiedresolved. All68valuesand10hypothesiscommentsunchanged; sourceis69approvedsubstitutionsplus5reviewedcommentfixes.

## [2026-10-24] ticket | Remove repeated cached-closure fallback work

[[tickets/T-27057-cache-header-closure-lookup]] addresses dict.get(f, tree.closure(f)) eagerly computing an ignored fallback for every already-indexed source. Shared enum-ledger checks amplify it across all437sourcefiles. Add behavioral tests for cached and unindexed paths; scope/proof results must stay identical.

## [2026-10-24] decision | Keep source-paletted color previews for browsing

User requested colored art exports for browsing. Root inspected local main-UI and calendar PNGs (already colored) and distinguished raw-index/STP diagnostic images from display previews. Media coordinator is adding clearly labelled source-CLUT color galleries/variants, preserving raw data and unresolved-palette status rather than inventing colors. [[asset-export]] records the requirement; actualimages remain ignored.

## [2026-10-24] build | Apply the first logical C-file names

[[tickets/T-27044-main-file-name-pilot]] moves src/main/8006CB30.c to src/main/scheduler.c and800737A0.c toschool_day.c throughrename.py. All437prototype-view payloads unchanged after onlythe2path-key mappings;0benign/0check/0risky. Clean27/27SHA1 and6952functions; originalobjectboundaries preserved. Regeneratedobjdiff paths follow names. Finalreview remains pending. User asked whether handoffs are integrated; root is prioritizing finished header-tool and symbol/file packets over new starts.

## [2026-10-24] ticket | Begin live topic-header extraction

[[tickets/T-27058-gameplay-topic-header-pilot]] is active in the primary chat, using the reviewed63declaration plan and integrated topic-aware tooling. User asked whether structural work is actively integrating; primary now performs real extraction, not further planning. Pre/post strict snapshots, guards and clean matching proof are required.

## [2026-10-24] build | Extract the first three real topic headers

[[tickets/T-27058-gameplay-topic-header-pilot]] now creates include/main/schedule.h, day_event.h andschool_day.h with32/5/26existing declarations. main_api.h forwards these leaves; no declarations are added or widened. All437Cfiles byte-unchanged,34intentional omissions preserved, headerguard0problems andstrictviewpayloads0/0/0. Clean matching build isrunning. This is live structural integration of the completed header-team handoff.

## [2026-10-24] ticket | Start exhaustive logical C-file naming

User explicitly requires all game-code C files to have understandable names and asked to start. Created [[tickets/T-27059-all-main-c-file-names]] and [[tickets/T-27060-all-overlay-c-file-names]]; [[phase2]] now makes zero address/number-only C basenames a completion gate. Existing symbol packets remain frozen while parallel coordinators prepare all remaining file-only mappings for root application.

## [2026-10-24] build | Integrate topic-aware header tooling handoff

[[tickets/T-27054-topic-header-tooling]] four-file unified patch is applied byte-for-byte; allbefore/afterhashes verified. Integrated23header and95prototype tests pass, supplementing independentworker355test/probe evidence. Live topicextraction T27058 passes63owners,34omissions,437unchanged Cfiles and0/0/0views. Parent is reviewing/committing completed packets; no new preparatory lane is being started in place of integration.

## [2026-10-24] lint | Resolve live header-pilot review findings

Independent integration review passed filepilotT27044andtoolingT27054. T27058had2findings: orphan evidence comments and missingplanned explicitleafincludes. Moved comments to the proper topicheaders, added5includes across the2logicalCfiles, retainedstateclosure andexactbodies. All437views still0/0/0, headercheck0issues;435Cfilesexactand2include-only changes. Post-fix matching rebuild is underway/completing before acceptance.

## [2026-10-24] ticket | Close verified filename and header pilots

[[tickets/T-27044-main-file-name-pilot]], [[tickets/T-27054-topic-header-tooling]], [[tickets/T-27057-cache-header-closure-lookup]] and [[tickets/T-27058-gameplay-topic-header-pilot]] moved to Done after independent Standards/Spec review and resolved findings. Header review fixes committed0be7fbcb7; exact437view payloads unchanged, clean and post-fix27/27hashes,6952functions.32remaining main file moves are now applying under [[tickets/T-27059-all-main-c-file-names]]; full phase2 is still open.

## [2026-10-24] build | Apply all remaining logical main C filenames

[[tickets/T-27059-all-main-c-file-names]] applied32file-only rows through tools/rename.py. All34main C basenames are now logical. Separate path-mapped snapshot comparison preserves all437declaration payloads exactly0/0/0; original snapshot retained. Standards review passed code/layout, with a stale living-page status finding corrected in [[spec/file-naming-main]]. Clean27hash proof remains pending. Registered completed worker spec/asset pages in [[index]].

## [2026-10-24] ticket | Continue header extraction and essential game constants

Registered [[tickets/T-27061-next-main-topic-extraction]] for the next exact reviewed extraction packet and [[tickets/T-27062-main-essential-locals-constants]] for the four main units' game-meaningful locals and literal sites. Existing247symbol rows remain frozen for root integration. Media lane continues remaining container/CLUT coverage and image-text review; no direct Godot work.

## [2026-10-24] ticket | Cover remaining phase2 reconciliation units

Created [[tickets/T-27063-hardware-main-reconciliation]], [[tickets/T-27064-complex-overlay-reconciliation]], [[tickets/T-27065-festival-overlay-reconciliation]] and [[tickets/T-27066-remaining-overlay-completion]] for disjoint remaining units. Source integration stays serialized in the primary chat while clean filename proof runs. Luna handles bounded reconciliation, Sol6.1 handles complex analysis/review. These are ongoing implementation tickets, not completed coverage.

## [2026-10-24] ticket | Audit remaining core and daily-rule constants

Created [[tickets/T-27067-core-scheduler-essential-completion]] for remaining shared core/scheduler/school-day semantics and exact proven literal sites; existing applied names remain authoritative. This fills the remaining unit ownership gap without changing production source.

## [2026-10-24] decision | Freeze exhaustive overlay filename packet

Committed c3053b47c with all403reviewed file-only mappings across26tables, [[spec/file-naming-overlays]] and its active ticket. All26frozen table hashes verified; full live aggregate plan is running. TT technical basenames preserve TT and assign no new module term/prefix; the folder-specific contributor note is not an extra approval gate for user-authorized basenames. Main32cleanbuild remains in progress with no failures so far. Persistent followup and ignored lane map now include all9chats and continued header/core/constant lanes.

## [2026-10-24] decision | Install the living main object and prefix map

Added src/main/README.md after independent narrow Sol review resolved the shared-data/grouped-ownership omissions. It maps all34logical objects and retains exact existing prefix ownership; corrected source attribution for sound driver/scene library and executable reload/STR playback adds no new prefixes. ADR0001 now points to this living map. [[spec/phase2-completion-audit]] has an explicit post-snapshot integration update without rewriting historical inventories.

## [2026-10-24] ticket | Support precise reuse of existing constants

[[tickets/T-27067-core-scheduler-essential-completion]] confirms club6 as CLUB_BASEBALL and its record index; girl7still has no proven consumer and is not renamed speculatively. Existing const-tool reuse/site limitations are tracked in [[tickets/T-27068-existing-constant-sites]]. Root caught and restored an overly broad ADR-tail edit before commit; Out-of-scope and Consequences sections remain exact.

## [2026-10-24] build | All main C filenames verified and complete

[[tickets/T-27059-all-main-c-file-names]] Done:32file-only moves applied,34logical names,437path-mapped declaration views0/0/0. CleanDocker27/27and6952functions/2276920bytes; independent rehash27/27. Both post-application reviews resolved; no body/prototype/boundary changes. Git staging briefly conflicted with live read-refresh operations; lock cleared normally after identifying active Git, without deleting a live lock. The403overlay file batch remains pending its full live plan.

## [2026-10-24] lint | Review main README prose and final filename proof

Committed5902c2c38 for all34logical main C filenames. Required deslop prose check found two curly-quote leads, removed with the living-map introduction clarification; two primitive/Primitives leads are accurate GPU terminology and retained. The grouped map and object evidence passed independent Sol review.

## [2026-10-24] ticket | Remove repeated irrelevant path tokenization

[[tickets/T-27069-file-path-scan-filter]] tracks a measured integration bottleneck: plan_file invokes path_rewrite over all code twice per move, lexing even files lacking its required literal old path. Full403liveplan remains read-only. A narrow literal-candidate filter and regressions are being prepared separately from game-code changes; no proof is waived.

## [2026-10-24] build | Integrate independently reviewed file-path filter

[[tickets/T-27069-file-path-scan-filter]] isolated approvedperformancehunks from the separateconstantfeature; unchanged79-test suitepasses in4.346s. Bothpathregexes requireliteraloldpath, so skippingfileswithoutthatliteral preservesallactualmatches; comments/asmstrings/boundaries covered. [[tickets/T-27068-existing-constant-sites]] remains open after independentSpec review caught disabled-preprocessor/raw-vs-AST provenance mismatch; it is not integrated. Fulloptimized403filenameplan is rerunning; no proof waived.

## [2026-10-24] build | Complete logical filenames for all game C objects

[[tickets/T-27060-all-overlay-c-file-names]] and [[tickets/T-27069-file-path-scan-filter]] Done:403overlaymoves,34mainlogicalnames,437total withzeroaddress/numericbasenames. Fulloptimizedplan/apply/ledgercheckpassed, all437path-mappedviews0/0/0; clean27/27and6952functions/2276920bytes, independent27/27rehash. IndependentLunaStandards/SolSpec applicationreviewsfound noissues. Fileonlysourcecommit isnext. Assetfrozen52-filemergedcheckpointpasses139tests; firstreadonlytestrunnerblockedtestcachewrites, correctedbywritableephemeralcopywhileinputsremainreadonly. Noassetcodechangeforthatsetupissue.

## [2026-10-24] build | Commit verified filename and asset milestones; integrate exact constant reuse

All437logicalCnames committed169145eff. Exactreviewed45Pythonassettool/testfiles committed98f3d22f5 from52-filefrozenmedia/textsource snapshots; combined139testspass, activeworkingfilespreserved. [[tickets/T-27068-existing-constant-sites]] exactfinalpatch integratedwith98testsandresolvedindependentreviews; actualbaseballliteralchangesremainnext. Source-onlymilestonesinclude nogameassets; broaderphase2andsemanticassetcoverage remainopen. Preservedstalezero-byteindexlockunderignoredT27060stateonlyaftercheckingnoactivehost/DockerGit; filenamecommitthensucceeded.

## [2026-10-24] build | Verify 2636 source symbol names in one matching wave

[[tickets/T-27050-parallel-main-reconciliation]] boundedpreparation/applicationmilestoneDone; [[tickets/T-27051-parallel-overlay-reconciliation]] remainsInProgressforits supplement. Applied247maintext/menu/HUD/holiday and2389DATE/NAME_ENT/GEKOfunc/datarows. Bothactual-sourcereviews pass,437ABIviews preservedunder scopedmapping; clean27/27and6952functions/2276920bytes, independentrehash27/27. User requestedfasterexecutionwith SAMEfullscope: asset/translation-preparationworkcontinues inparallel; avoidredundantchecks/chatter andbatchmatchingbuilds, no scope reduction.

## [2026-10-24] decision | Resolve reviewed namespace gates centrally

Adopted independent source-backed prefix closure: ETC keeps one expanded owner row;14unapplied etc_ targets willbecome misc_. Main boot/debug/audio groups include bounded mixed-object helpers. TT uses technicaltt_ only, preserving reservedEnglishgame/folder agreementandclaimingnoagreement. GLOSSARY/ADR/livingmainmap nowagree; no sourceidentifierschangedbythisdocumentationdecision. This removes avoidable coordinatorwaits while retaining evidence and one-owner rules.

## [2026-10-24] query | Revisit fan-patch tooling as a coverage cross-check

User pointed to [[ref-english-patch]]. GitHubprimaryfetch confirms repositorydefaultmain, no declaredlicense, and extraction/script-survey/renderer/font/repacking plusimage-redrawtools andper-filetranslationmetadata. [[tickets/T-28005-fan-patch-crosscheck]] comparesfactualoriginaloffset/layout/coverageevidence withour90scriptcontainers andremainingruntime/source/imagegaps. No code, prose, translatedtext orgameassets copiedintoGit. ExistingT11090notes remaincreditedfacts-only.

## [2026-10-24] build | Preserve matching after proven constants and corrected views

[[tickets/T-27068-existing-constant-sites]]Done:2baseball6literalsnamedthroughverifiedtool; girl7unexplainedandunchanged. [[tickets/T-27067-core-scheduler-essential-completion]]27quartercastcorrections/15O.BINcommentsappliedwithreviewedexactrebase. [[tickets/T-27064-complex-overlay-reconciliation]]RPGinvalidmemberreferencecorrectedtooffset03Ascreen_brightnesswithsigned-bytestorepreserved. Actual-sourcereviewpasses; currentmain/TEL/RPGrebuiltchecksumsandindependentall27hashesmatch,6952functionsunchanged.

## [2026-10-24] ticket | Fix the known signed-literal blocker

[[tickets/T-27070-signed-constant-replacement]] addresses the reproducedT27062negative-literal sign-reversalbug; unsafe negative sites remain unapplied until reviewedfix/proof. Existingpositiveconstantandidentifierworkcontinues.

## [2026-10-24] build | Apply reviewed main local and parameter names

Applied 305 main local/parameter rows plus six core follow-up rows through the rename tool.
Its final guard caught a new parameter reusing a retired global alias; a scoped rename chain
to mascot_selection is being proved. Actual-source review and matching verification remain
pending in [[tickets/T-27062-main-essential-locals-constants]]. The next overlay packets are
being frozen for integration while text and media work continues independently.

## [2026-10-24] query | Compare fan-patch dialogue locations with the source export

[[tickets/T-28005-fan-patch-crosscheck]] found identical 90-container sets and load bases;
all 29403 patch dialogue offsets occur among the 30036 local offsets. The 633 local-only
offsets are reconciliation leads, not a completeness claim. Routed those and image-region
metadata to the asset owners. User clarified that the future port will not inherit the PS1
textbox limit; patch wording and replacement images remain outside our translation work.

## [2026-10-24] ticket | Complete the bounded fan-patch reference audit

[[tickets/T-28005-fan-patch-crosscheck]] Done after independent per-file metadata review.
Recorded the exact commit/tree pin, repeated 90-file/load-base/offset comparisons, and explicit
limits in [[ref-english-patch]]. Full-tree image/direct-string counts remain reference leads;
runtime and translation completeness remain open with the text/media owners. No translated
wording, implementation or image data was imported into tracked files.

## [2026-10-24] decision | Authorize the final verified push

User requested a push to the repository after the final Astra review, separate Astra fixes,
reviewer verification and final matching check. Recorded the push gate in [[phase2]],
[[tickets/T-27000-phase-2]] and [[tickets/T-27056-final-astra-fixes]], and updated the existing
continuation automation. Current weekly usage is 49%; reduce repeated coordination/proofs
and reserve remaining usage for real implementation and the final reviews.

## [2026-10-24] build | Integrate the reviewed signed-constant correction

[[tickets/T-27070-signed-constant-replacement]] exact frozen two-file patch passed Luna
Standards and independent Sol Spec review, then all 111 integrated Docker tests. Three real
HUD negative-scale sites have read-only inverse/compiler/view proof; their source application
and matching check follow the large identifier wave. User requested cheaper agents and more
automation: bounded work uses Luna, proofs run in scripted batches, and Astra remains reserved
for the final review/fix pair.

## [2026-10-24] build | Validate the frozen text semantics checkpoint with media tooling

The reviewed T-28001 semantic packet contributes 32 Python source/test files. Tested its exact
hashes against committed media tooling in an ephemeral Docker copy; the combined asset suite
passes. Live asset work and ignored original/export data were preserved. Staging the reviewed
source-only checkpoint remains separate from comprehensive text-coverage completion.

## [2026-10-24] build | Qualify overlay field ownership before integration

The 4,738-row aggregate dry run caught GEKO DateTxt binding through DATE's separate alias.
No source was written. Root preserved the reviewed inputs and explicitly qualified overlay
field scopes in the integration aggregate; full proof is rerunning. The signed-constant fix
is committed0b9ddbf33, and the reviewed text-evidence checkpoint is committedc56f9dcd4.
[[status]] records which source changes still await the next matching build.

## [2026-10-24] build | Apply the qualified identifier wave and close festival spec findings

The full 4,738-row/232-file qualified-scope plan passes; root application is running.
Independent review approved BUNKAKEN's 250 symbol/type/field rows and 259 locals. Corrected
two living-spec findings: first-year companion reachability stays unproved, and the normal
visit log's direct scene assignment is separate from birthday call sites. T-27066's bounded
post-fix review also passes for small/BUNKA_SD/SHUGAKU packets. Created
[[tickets/T-27071-anonymous-fields-kr-scope]] for the six reproduced TACO scope blockers.

## [2026-10-24] lint | Reject an unsafe anonymous-member helper before integration

Independent review of the first T-27071 scratch patch found an unrequested-owner rewrite
hidden by normalization of a preexisting compiler error. The helper remains unapplied and
is being corrected with the reproduced regression. K&R masking has no new finding. The
BUNKAKEN reviewer separately confirmed both requested living-spec corrections are resolved.

## [2026-10-24] build | Resolve scope review and preserve EVENT prototype visibility

Corrected T-27071 patch passes independent counterexample verification and all 116 integrated
tests. Moved EVENT's func_801040F0 prototype from the main API to its overlay owner after the
rename application guard caught lost visibility; all 437 strict views remain unchanged and
header checks pass. Independent review approves the move. The 4,738-row apply is retrying;
no earlier failed attempt wrote that source wave. Created
[[tickets/T-27072-caller-scoped-constant-sites]] to bind reviewed constants to their caller
without merging unrelated state-machine values.

## [2026-10-24] build | Verify binary output and isolate the remaining replay guard

The clean source wave compiled all 27 binaries and passed every SHA1 target. Removed three
unused EVENT override guards after their aliases became private names; all 437 views remain
unchanged and headers pass. The final rename guard exposes T-27073: another reviewed macro
family in the same function trips strict scale-request provenance. No binary mismatch found;
the full build remains incomplete until that guard is fixed. T-27072's corrected CLI route
passes independent verification and all 122 integrated tests.

## [2026-10-24] build | Close the verified readability wave

Applied 311 main rows, the mascot correction, 4,738 qualified identifier rows and 112 proven
constant substitutions. Independent actual-source review passes. All 437 ABI views are
unchanged; the clean build and every guard pass, with 27/27 independently rehashed binaries
and 6,952 functions / 2,276,920 bytes. T-27070, T-27072 and T-27073 are Done after resolved
reviews and 126 current tool tests. The next 3,942-row read-only plan caught two parameter
collisions; role-preserving targets requested_end_frame and input_text are being re-proved.


## [2026-10-24] decision | Defer overlapping scene-effect data label

Removed `D_80122D44` from the m-flow standalone data proposals because it is `TalkWork + 0x74`; updated [[spec/m-flow]] and [[spec/m-dates-b]] to retain the unknown field and describe main-loader/DATE/GEKO evidence. Expanded cross-file prefix ownership in `src/main/README.md` and reconciled stale naming references in [[spec/h-cdio]] and [[spec/m-dates-a]]. Fresh independent review and stable Docker plans remain pending.


## [2026-10-24] decision | Close safe-subset Standards and Spec review

Final independent Sol 6.1 review passes the five primary apply packets, current `src/main/README.md` prefix ownership map, and synchronized [[spec/m-flow]], [[spec/h-cdio]], [[spec/m-dates-a]], and [[spec/m-dates-b]]. `TalkWork.unk_74` remains deferred as a field label; no independent D_80122D44 row remains. Stable Docker dry-run validation awaits root's source batch completion.


## [2026-10-24] build | Run five-unit safe apply plans

One two-CPU Docker batch ran the five apply tables against the stable snapshot. m-flow, h-graphics, m-dates-a and m-dates-b plans exited 0; h-cdio found two already-named `dec_bg_*` labels. Updated the h-cdio source labels to `dec_bg_lba_cache` and `dec_bg_brightness`; targeted re-plan remains outstanding. Independent Standards/Spec review passes; root source application and matching proof remain open.

## [2026-10-24] query | Explain the final-review dependency

The current committed source checkpoint is a90eac1e6: independent source review, all 437 ABI views, clean guards/build, 27/27 binary hashes and 6,952 functions pass. Final Astra review has not started because remaining source naming and logical header migration are unfinished, not because a reviewer is unavailable. The corrected next packet has 3,940 rows, including 2,807 functions; two proposals for nonexistent locals are excluded and target spelling/collision corrections are recorded in its ignored manifest. Its read-only Docker plan remains active. The header lane has prepared 467 further main declaration moves plus shared-type, SDK and EVENT packets, with final regeneration and complete umbrella disposition still open. [[status]], [[spec/header-split-plan]], [[spec/phase2-completion-audit]].

## [2026-10-24] decision | Defer one overlapping hardware alias to header ownership

The 3,940-row dry plan found only two new compiler diagnostics, both EVENT uses of D_80122740 after main renamed its camera reference. The address belongs to distinct EVENT tile-pointer storage during that overlay. A scratch identical-type private-header split preserved all 437 views but was correctly rejected by current canonical-declaration ownership checks; it is not applied. Root retained the original packet, deferred only this main hardware-data row to T-27061, and started the remaining 3,939-row guarded apply. The executable packet still includes all 2,807 function names. Its manifest records the deferral and SHA-256 9a60e8297a868f578227b3d60ac38069a0b062415dad2e151995e8e8ed9b9af0. [[spec/header-split-plan]], [[status]].

## [2026-10-24] build | Commit the reviewed media-reader checkpoint

Committed 47e315ce4 from the exact reviewed v3 frozen Python blobs: 25 changed source/test files covering five UI formats, original background context, MDEC export context, media coverage and Japanese-text image inventory. The manifest and all 48 frozen per-file hashes/sizes validate; combined committed text tooling plus the 42 owned frozen Python inputs passes 249 Docker tests with one optional integration skip. The shared CLI equals HEAD and is retained. Root staged frozen blobs without overwriting newer working files. Independent format/coverage/publication reviews and their limitations remain in the frozen manifest; full media/text coverage and hardware MDEC pixel parity remain open. An independently guessed aggregate source-hash formula differs, so only the pinned manifest and verified per-file identities are claimed pending the author's formula. [[assets-media]], [[assets-input]], [[asset-export]].

## [2026-10-24] query | Confirm the media snapshot aggregate identity

The author's aggregate format is sorted SHA-256, two ASCII spaces, relative POSIX path and newline for all 48 files. Root independently recomputed it and matched 2b32408b2dca8fffd3d6fed19be5adcf1743124c02b759af86a3ceae01a31c59. The earlier guessed-format mismatch is resolved without changing source or repeating tests. The ignored integration proof retains both calculations and their explicit formats. [[assets-media]].

## [2026-10-24] ticket | Prepare the remaining module path names

Created [[tickets/T-27074-overlay-module-paths]] In Progress for bounded directory/header-path preparation from the accepted ADR map, after all 437 logical C basenames are complete. The packet must retain disc identities and body-backed ownership, record unproved names/TT's contributor condition explicitly, and wait for stable naming/header providers before root's tool-only application. This closes an existing phase2 layout criterion rather than adding engine work. [[spec/file-naming-overlays]], [[spec/header-split-plan]].

## [2026-10-24] build | Apply remaining identifiers and preserve the callback alias

The 3,939-row batch is applied. Independent actual-source review finds exact body/operator/literal/string/type preservation across 377 changed C/header files, correct symbol ownership/HYP placement and the six intended TACO requests. Its independent scoped snapshot comparison passes all 437 units with zero changes. The post-apply check found the documented T-18000 D_80085368 callback-address alias after its main function became scene_still_fade_in. Root planned and applied the required one-row alias correction through rename.py; its old D_ spelling requires a data-kind request, while all three actual SHUGAKU callback declarations remain void(void), not data objects. The alias post-check and final proof are pending. The old alias is present in exactly those three snapshot views; TAIIKU's existing s32() override is unaffected.

Root grants the local Luna caller_constants worker a temporary exclusive source/build lease for only the reviewed comment patches, 274 proven festival-stage literal sites, alias proof and the prepared clean matching run; all other lanes remain preparation-only. Root retains commits/shared tracking. The worker must stop on a failed gate, preserve asset WIP, and publish ignored SOURCE-STABLE.json before header regeneration. Full phase2 review remains gated on header completion and the final Astra pair. [[status]], [[spec/header-split-plan]], [[tickets/T-27063-hardware-main-reconciliation]], [[tickets/T-27065-festival-overlay-reconciliation]].

## [2026-10-24] build | Commit the remaining matching readability wave

Committed 4dab92732: 3,939 identifier requests including 2,807 function names, the separately reviewed callback-address alias, 274 festival-stage literals and 22 definition comments; 427 source/config/value/ledger files, with no assets or future proposals staged. Independent final source review and both 437-unit scoped ABI proofs pass with zero changes. Clean configure/Ninja/progress and every guard pass; two independent binary checks confirm 27/27, with 6,952 functions and 2,276,920 bytes. The rename guard has 12,922 rows. Exact stable source digest is e771b82f3662d7018d737b3aeac7dd648ec579eb1f134432a5f2fa16375b64fe. The source/build worker's bounded lease is released. T-27061 may now regenerate its complete ordered header packet against stable source and ASM; the camera-reference ownership deferral and 141 unproved BUNKASAI literal sites remain explicit. [[status]], [[spec/header-split-plan]], [[tickets/T-27063-hardware-main-reconciliation]], [[tickets/T-27065-festival-overlay-reconciliation]].

## [2026-10-24] ticket | Close the anonymous-member and K&R tool gate

Moved [[tickets/T-27071-anonymous-fields-kr-scope]] to Done and moved its board card. All six intended TACO requests are now applied through the reviewed tool, independently checked in the actual source, and included in the clean 27/27 matching checkpoint. Earlier scope-proof findings are resolved; the broader unit and phase2 tickets remain open. [[kanban]].

## [2026-10-24] build | Commit reviewed sports, SD and battle exporters

Committed a1a0625aa from immutable v4 snapshot 7dd2b954f70f0ad78c7cf0cab5ff1c02296538c65fc41a992244478fa4f0d94c: 15 changed Python source/test files, with 51 owned frozen Python inputs tested against committed text tooling and an identical retained CLI. All 57 per-file identities and canonical aggregate aa8eca2e15b3434c97792e65929d7e138dd31f18be79830b2c5d641d9999c6d8 match. Combined Docker tests pass 282 with one optional skip. A summary-printer key mismatch occurred after successful tests/report persistence; only the ignored printer key was corrected, without retesting unchanged sources. Active worktree assets/text code was not overwritten. The catalog has 3,339 palette-backed variants and 1,831 unreviewed states; corpus completeness and hardware pixel parity remain open. [[assets-media]], [[assets-input]], [[asset-export]].

## [2026-10-24] decision | Approve registry path migration after counterexample fixes

The initial T-27074 ignored rename-tool proposal was rejected for rewriting nested metadata and accepting malformed JSON. Its structural replacement then exposed an empty-profile bypass. All counterexamples are resolved in exact patch 57df727aa38bb491f37372669b403e1d2acf96a9282e36b561d592173b328baa: parse full JSON with duplicate-key refusal, update only top-level owner/type paths, preserve other fields/order, and reject empty existing profiles before writes. Four focused regressions and the 130-test candidate suite pass; independent review approves the hash. The patch remains unapplied while T-27061 prepares its header stack. [[tickets/T-27074-overlay-module-paths]].

## [2026-10-24] decision | Refresh the continuation instructions and essential-constant lanes

Refreshed the existing complete-tokimemo-phase-2 heartbeat's checkpoint hints without changing its schedule or final Astra/push gates. Removed superseded filename and unsupported-SERIFU counts in favor of current coverage reports. Reconciled the ignored integration state to committed 4dab92732/a1a0625aa, released the completed worker lease, and recorded the approved but unapplied registry migration fix. Existing T-27051, T-27064, T-27065 and T-27066 chats now prepare disjoint executable essential-constant slices from actual applied names; they remain forbidden live source/tool/ledger/Git/shared-wiki writes. Root retains integration; T-27061 full header closure is the critical path. [[status]], [[tickets/T-27074-overlay-module-paths]], [[asset-export]].

## [2026-10-24] lint | Check the current checkpoint tracking

Targeted checks confirm the T-27071 Done and T-27074 In Progress frontmatter each has exactly one card in the matching board column. Current status links resolve, including the existing unique archive basenames. The initial root-only path check did not implement basename resolution; rerunning with the wiki's convention resolves those two archive links without changing history. Source/header trees remain clean after 4dab92732, and future proposals/active asset work remain outside the staged code checkpoint. [[status]], [[kanban]].

## [2026-10-24] build | Commit the reviewed translation-unit checkpoint

Committed b4977bd16: 35 changed text-tool/test files from 48 exact frozen Python blobs. All 50 manifest files and three numeric reports validate against pinned manifest 926ad42f2e9da6d04b4f6430d2e9a240e4bd68eeb6f88b43fe42270e4ab26f4d and canonical source set b67ddd6c594125dfc9dc8108886df02f80a7c3eb517be79cb18da2a2f4457124. The coherent committed-media-plus-frozen-text run passes 367 Docker tests with zero skipped on Python 3.10.12, pycparser 3.11 and gcc 11.4.0. Its first helper invocation preserved the successful test log but missed proof serialization; metadata was revalidated/recovered without rerunning unchanged tests. Root staged exact frozen blobs without overwriting active work. The checkpoint adds 286 stage-show bodies and renderer proofs; production validation covers 38,696 units with null translation targets. Comprehensive coverage remains false and 1,889 extents need classification. Source witness f9c679d5 remains explicitly historical; rebasing full source enrichment waits for final header/path stability. [[assets-text]], [[asset-export]].

## [2026-10-24] decision | Synchronize the module navigation map

Updated src/main/README.md to the committed boot_paused_step name and the two explicitly hypothetical audio helper names. The ledger confirms all three address/name mappings. The public-prose check has zero strong hits; its two weak primitive/Primitives hits are established GPU terminology and remain. No C or ABI changes. [[tickets/T-27063-hardware-main-reconciliation]].

## [2026-10-24] ingest | Publish the matching reviewed asset documentation

Staged the seven exact frozen wiki/ticket documents from the reviewed v4 media and b67 text checkpoints, alongside current coordinator tracking. Their hashes match the immutable manifests, all ticket statuses match their board columns, and newer working files remain untouched. These pages describe the committed readers and explicit coverage limits; ongoing v5 work remains a separate unstaged layer. No extracted text, images, audio or original game inputs are staged. [[assets-input]], [[assets-media]], [[assets-text]].

## [2026-10-24] decision | Begin the closing constant integration wave

Root grants the local Luna caller_constants worker one exclusive bounded lease for the independently reviewed T-27051 (204), T-27064 (324) and T-27066 (25) site packets, with baseline/source pin checks and unchanged rename.py APIs. The 133-value/141-site BUNKASAI extension is being converted to a guarded runner separately; it adds declarations to the existing enum and leaves all literal renames to the tool. The combined target is 694 sites, followed by one source/437-view review and matching build. The current SOURCE-STABLE record becomes historical baseline evidence once writes begin; the header lane is told to continue ownership/driver preparation and wait for the replacement ready marker before final proof. No tool, game asset, Godot, Git or unrelated source changes are authorized to the worker. [[tickets/T-27051-parallel-overlay-reconciliation]], [[tickets/T-27064-complex-overlay-reconciliation]], [[tickets/T-27065-festival-overlay-reconciliation]], [[tickets/T-27066-remaining-overlay-completion]], [[spec/header-split-plan]].

## [2026-10-24] ticket | Bound the final essential literal gaps

Created [[tickets/T-27075-essential-constant-selectors]] In Progress from independent bounded triage: ending byte masks/known heroine indices, stage-show club IDs, two school-trip rule groups and RPG guard timing. Existing documented arithmetic/layout indices and inspected hardware values are not blanket blockers. Existing tool routes are preferred; only exact indexed-member and compound-add support is authorized for the demonstrated missing contexts, with existing selector semantics unchanged. The new tool patch is preparation-only until the currently leased constant packets finish. [[rename-tool]], [[tickets/T-27066-remaining-overlay-completion]], [[kanban]].

## [2026-10-24] query | Refresh the current source-file navigation facts

The living [[source-files]] page still described 28 address-named main files and 834 functions. A bounded Luna documentation task now owns that page only, deriving the current 34 main/403 overlay logical-basename navigation from configuration and the verified 4dab92732 census (830 main, 6,122 overlay functions). Historical boundary evidence is retained as history, and proposed folder/header moves remain explicitly unapplied. The active constant wave changes no file/function boundaries. [[phase2]], [[spec/file-naming-main]], [[spec/file-naming-overlays]].

## [2026-10-24] ticket | Reduce repeated whole-ledger validation safely

Created [[tickets/T-27076-constant-batch-transaction]] In Progress after the guarded base apply spent about half an hour repeating full naming checks. A preparation-only coordinator must retain every request's existing count/inverse/provenance/GCC/view proofs, defer only intermediate post-write global validation, run the real complete guard once at the batch boundary, and roll back all authorized source/header/ledger changes on any failure. Original tools and producer packets stay unchanged; independent review and disposable failure probes precede live use. The active base/supplement run is not interrupted. [[rename-tool]], [[kanban]].

## [2026-10-24] query | Profile the slow global validation

Read-only py-spy samples in an ephemeral Docker profiler show progress through signed-request parsing and the old-name guard, not a non-progressing process. The repeated owner/address lookups rebuild SymbolTable.owned_lines for many ledger entries. Its documented immutable-snapshot contract permits per-instance indexed results with unchanged ordering/deduplication and fresh returned lists. T-27076's preparation-only wrapper will compare such scoped caches against original results, restore hooks on every exit, and keep the real complete guard. No running validator was interrupted or patched, no repository/tool file was mounted in the profiler, and no host dependency was installed. Ignored stack samples remain in constant-wave/. [[tickets/T-27076-constant-batch-transaction]], [[rename-tool]].

## [2026-10-24] ticket | Index the measured immutable-symbol lookup hotspot

Created [[tickets/T-27077-symbol-snapshot-index]] In Progress for a minimal preparation-only performance patch derived from the scoped transaction cache. It must preserve every owner/address result, alias order, deduplication, EVENT/main overlap and fresh-list behavior, and pass synthetic plus real-table equivalence against original results. The documented snapshot contract requires rebuilding after symbol-file changes. No live tool changes occur until frozen constant producer runs finish and independent review approves the exact patch. This speeds the mandatory guard without removing it. [[tickets/T-27076-constant-batch-transaction]], [[rename-tool]].

## [2026-10-24] decision | Preserve the applied complex-overlay constant checkpoint

The bounded operator completed T-27064 base297 and supplement27 literal sites with zero producer proof failures. It holds before the remaining370 sites while the transactional runner receives independent review and a fresh read-only preflight. Final matching remains the committed4dab checkpoint until the complete constant batch is proved. Usage77percent; reserve distinct Astra review/fix agents. [[tickets/T-27076-constant-batch-transaction]], [[tickets/T-27075-essential-constant-selectors]], [[status]].

## [2026-10-24] lint | Review constant selectors and rehearse full header closure

Independent Sol review approves T-27075 selector patch87c271b47ead7939ca7a95419eb6ec8193f6969710baed72788c8ae489e4dac4 after shared-main field ownership fixes;137 candidate tests pass. The complete archived T-27061 rehearsal removes both umbrella headers with zero ownership gaps, unchanged GameState layout, passing header/API checks and299 focused tooling tests. It still needs independent packet review, constant-replay compatibility and fresh generation/proofs against the final source. T-27076 corrected preflight5709cbfa passes370 sites, retaining44 explicitly reviewed unresolved sites; independent final hash approval precedes actual apply. No new binary matching claim. [[tickets/T-27075-essential-constant-selectors]], [[tickets/T-27076-constant-batch-transaction]], [[tickets/T-27061-next-main-topic-extraction]], [[status]].

## [2026-10-24] build | Apply the 694-site constant checkpoint and integrate reviewed tools

T-27064 base297/supplement27 and T-27076's remaining370 sites are applied with all request proofs and final renames guard12977 passing. Added only the eight approved domain declaration comments. No new binary matching claim:31 essential sites remain before the complete725-site source proof and clean check. After frozen producers finished, root integrated approved T-27075 selector87c and T-27077 symbol-index0b8e patches; the combined tool suite passes144 tests. Final31 has a separate source-only BASELINE marker; SOURCE-STABLE remains integrating until full proof. [[tickets/T-27075-essential-constant-selectors]], [[tickets/T-27076-constant-batch-transaction]], [[tickets/T-27077-symbol-snapshot-index]], [[status]].

## [2026-10-24] ingest | Commit reviewed native media readers and offline integration proof

Committed141fcb8d8 contains20 exact reviewed media-v5 source/test/Dockerfile blobs; no live files were overwritten by staging and no original/exported game data was staged. Frozen67-file manifest7bd5f51ff3150665b8c7546166ef1b1d66928d87ba6e0bce3ac6d9528950bf31 and canonical source-set1b93e1d90f4c1ff13f9a94a801a37c2f3b2bb513df00bcc0234265d08a610f2a validate. Combined55-module offline Docker suite passes466tests/0skips after supplying the read-only historical f9c source witness, whose14513 hashes validate before/after. V5 adds100photo pages, CDDA track2, original Form2 preservation,14910WAVs and997movie frames. Supported static catalog has3462 variants,425confirmedJapanese and70suspected; unsupported corpus/model/geometry/movie classification and one quarantined sector remain explicit open work. Five frozen asset documentation/ticket pages match current bytes. [[assets-media]], [[assets-input]], [[tickets/T-28002-media-asset-export]], [[tickets/T-28003-original-input-assets]], [[tickets/T-28004-japanese-image-catalog]].

## [2026-10-24] build | Complete the reviewed725-site gameplay-constant wave

All725 substitutions/350definitions/eight domain comments pass independent actual-source Standards/Spec review. Exact437 ABI views match baseline with0changes; all changed paths are approved. Clean Docker configure/build/progress and every guard pass, independent27/27 binary hashes,6952/6952 functions,2276920/2276920 bytes and12984 ledger rows. Source digest d6a53cd6486d6b390218d0d3d9f69c8b2fc80550b5996a0b6b73ced3921d5386; cleanproof SHA26ece335d6e8bbd6bba1f9a45a37323e01b1b5a953db83ddb85f3293650429e7. [[tickets/T-27075-essential-constant-selectors]], [[tickets/T-27076-constant-batch-transaction]] and [[tickets/T-27077-symbol-snapshot-index]] move to Done with matching board cards. Final Astra remains pending actual full-header/path integration. [[status]], [[rename-tool]].

## [2026-10-24] ticket | Start the phase-wide Astra review during final integration

[[tickets/T-27055-final-astra-review]] moves to In Progress. With all437Cfilenames and the reviewed725-site constants wave verified/committed atdb1d04f3b, the work is wrapping up. The full reviewer can assess gameplay readability, layer residue, tooling and documentation while the header lane finishes its current-source integration proofs. Actual integrated header/path changes remain mandatory review inputs before final verdict. The separate Astra fixer and original-reviewer verification remain required; no completion or push is authorized early. [[phase2]], [[status]].

## [2026-10-24] ticket | Route the full Astra findings to a distinct Astra fixer

The full reviewer delivered T-27055/findings.md revision1: six source-backed P2 readability/specification findings, with no binary regression. [[tickets/T-27055-final-astra-review]] moves to In Review pending verification; [[tickets/T-27056-final-astra-fixes]] moves to In Progress. Its delivered-report input replaces the closed-ticket dependency to avoid a review/fix cycle. A different Astra resolves the concrete state/field/local/constant/comment/documentation findings while the known final header/path work continues. No phase2 completion or push claim. [[phase2]], [[status]].

## [2026-10-24] decision | Apply reviewed strict headers and serialize the Astra correction batch

Original Astra approved exact header67355f07d3a7b7d8cdaede13c4a85ec017919d8f814783bd278475d6901e5130. Root applied197files; every changed byte matches the fresh r16 prepared tree, both umbrellas are absent, and actual437raw declaration views remain byte-identical. Preapplyproof9baeb4c6 passes504tests/allguards/73preservedtypeblocks. Root composed the separately reviewed Astra selector patch with the header tools;169tests pass. Bounded Luna operator applied47+1identifier operations through rename.py with guards13031/13032, then freshly prepared149constant sites/48requests/fivedefinitiongroups against actual new headers; transaction validation is in progress. Five exact reviewed living-spec patches are applied. No new binary matching or phase2 completion claim; working source remains uncommitted pending source proofs and one clean combined build. Rink/path and broader semantic acceptance remain open. [[tickets/T-27061-next-main-topic-extraction]], [[tickets/T-27055-final-astra-review]], [[tickets/T-27056-final-astra-fixes]], [[status]].

## [2026-10-24] build | Verify actual topic headers and the concrete Astra correction batch

Strict197-file topic-header migration and the distinct Astra repair batch are applied/proved:48identifierops,149constant sites,175definition comments/13domaincomments andfive living-spec patches. Actual437Ctoken streams match the approvedcandidate excludingincludes;437mappeddeclarationviews0changes;169tooltests pass. Cleanconfigure/ninja/progress/allguards pass, independent27/27hashes,6952/6952functions2276920/2276920bytes,13085ledgerrows; fullbuildfingerprint2db37ce14c0def248224a28400c0a0abcb17d40ae33e8726f699ab5f606f0c3b. OriginalAstra actual acceptance bba9a47b09b4fd33b83b64b9cf9f3908346ddeb5f0fe4a4921ce90c6ed71b64b permitslocalcommitonly. [[tickets/T-27061-next-main-topic-extraction]] movesDone foritsstrictpacket; [[tickets/T-27056-final-astra-fixes]] movesInReview. BroaderR1-R3classification/rink/logicalpaths/finalphaseapproval/push remainopen. [[tickets/T-27055-final-astra-review]], [[status]].

## [2026-10-24] decision | Finish the bounded review gates within the authorized remaining usage

Verified header and concrete Astra repairs are committed at 302996710. The user authorizes the full remaining seven percent to finish, superseding the earlier 95-percent usage stop; the subsequent account counter reports a new weekly window. Root applied the separately reviewed skating-rink table type correction: the actual 437-view comparison changes only its expected EVENT declaration from 22 to 31 entries. A serialized operator now applies eight further proven identifiers and four existing VALEN threshold uses. Logical paths are being proved in isolation; owner reports must distinguish supported gameplay fixes, source-backed exceptions and shared-owner referrals. No phase completion or push claim precedes original-reviewer verification and final matching. [[tickets/T-27055-final-astra-review]], [[tickets/T-27056-final-astra-fixes]], [[tickets/T-27074-overlay-module-paths]], [[status]].

## [2026-10-24] query | Tighten finite owner evidence and local path guards

The original reviewer approves DATEs 90 mutable text/command/voice slot names and the current five-file path-tool patch, but rejects signature-only parameter waivers and four reversed EVENT group/line names. Owner lanes repair those exact evidence patterns before consolidation. The eight-identifier follow-up passes the actual rename guard at 13,093 rows; four existing VALEN threshold substitutions are still being checked. Isolated path application finds a case-insensitive old-spelling guard error, so root requires a local fix rather than substituting a Linux-only proof. The human confirms the usage window reset. No new live path application, matching claim or push. [[tickets/T-27055-final-astra-review]], [[tickets/T-27056-final-astra-fixes]], [[tickets/T-27074-overlay-module-paths]], [[status]].

## [2026-10-24] build | Verify reviewed path tooling on the live checkout

Root applied exact independently approved five-file tool patch33a688df3fe19b55afa8007b788270eff6c7b9415646b84a305eb033ce6394ec. All337tests pass across rename, header, prototype, source-scan and symbol modules in Docker. This includes exact-case stale-path checks needed by the macOS checkout; the repaired isolated postflush guard and path/profile/YAML proofs still precede any live folder migration. The eight-identifier/four-threshold follow-up also finishes with437mappedviews0changes andguard13094; its approved comment is token-only. No final matching or push claim. [[tickets/T-27074-overlay-module-paths]], [[tickets/T-27056-final-astra-fixes]], [[status]].

## [2026-10-25] decision | Apply the independently approved logical module paths

The original reviewer approves exact56-row table047c7a5335e6ee5daa05cdb9e05efb30a59c1ddda9b8185df7dd136388f6498b with committedtool33a688df. Isolatedproofs pass:437mappedCfiles,0nonpathbody/viewdeltas,404changedCfileswithallstringchangesvalidated,965unchangedobject/subsegmentrecords,validownershiproots andmacOSexact-caseguard13141. Root freshplan passesGCC/profilechecks at actuala5be61444 withRink/eightID/fourthresholdfollowups; source/keyconfigdigest8c73b137c88afb3b7cd25fa6e0e9c9c9dd2b31352e3c3415835b107a4f598f9e. Serializedsingleapplicationbegins withfreshbaseline andproposaltable, ledger t27074-paths. Actual437/body/YAML/profileproofs andfinalmatchingremainmandatory. [[tickets/T-27074-overlay-module-paths]], [[tickets/T-27055-final-astra-review]], [[status]].

## [2026-10-25] build | Verify actual logical paths against the complete current source

The single approved56-row rename application completes withguard13150. ActualbaselineincludesRink31/eightIDs/fourthresholds. Fresh437pathmappedviews andnonpathCbodydifferencesarezero;404changedCfileshaveeverystringdifferencevalidated. Fullmetadataofall27YAMLfilesmatchesafter728approvedpathnormalizations, including965object/subsegmentrecords. Topicownership70/requiredtypes21/linkunits26 andheadercasefolduniqueness28pass. Sourceleaseisreleased; originalrevieweracceptance, remainingsemanticcorrectionsandfinalmatchingstillprecedeDone/push. [[tickets/T-27074-overlay-module-paths]], [[tickets/T-27055-final-astra-review]], [[status]].

## [2026-10-25] build | Verify final Phase 2 source and reconcile reviewed specs

The final clean configure, build and progress checks pass at source digest `687b5ce789d59f154b527264d23227b741a08bc619e4a1f8c67d617257f4c4f3`: all 27 binaries independently match, with 6,952 functions and 2,276,920 bytes. The original Astra reviewer independently verified the digest and hashes. The separate Astra fixer records resolution of R1–R6; the original reviewer approved the final sixteen-page prose patch, which root applied with exact before/after hashes. The accepted source includes 437 logical C basenames, topic headers, logical overlay paths with the TT exception, 1,298 semantic operations plus one local-name repair, and fourteen original-data fragment includes. All 43 C-owned data islands / 2,083 pieces have no unexplained gaps; declaration and inverse-source proofs pass. The earlier failed guard/build attempts remain historical evidence. Final tracker review and publication are still pending. Wiki lint reports zero broken links and zero ticket/board mismatches; its two page-size warnings are pre-existing. [[tickets/T-27055-final-astra-review]], [[tickets/T-27056-final-astra-fixes]], [[tickets/T-27000-phase-2]], [[spec/phase2-completion-audit]].

## [2026-10-25] ticket | Complete Phase 2 and publish reviewed source

Original Astra verification accepted all six source/spec findings after the separate Astra fixes. Root pushed reviewed source commit `182af9d13d418b159973e59fbeaedf5e39f87902` to `origin/main`; an independent `git ls-remote` confirmed the exact remote commit. The unchanged source digest is `687b5ce789d59f154b527264d23227b741a08bc619e4a1f8c67d617257f4c4f3`, with final clean 27/27 matching, 6,952 functions and 2,276,920 bytes. The reviewer approved both exact tracker transitions and the actual push witness. T-27000 and T-27056 now move Done with their publication criteria checked and board cards consistent; all Phase 2 children are Done. Asset tickets remain separate open work: comprehensive text extraction and image/audio coverage are not claimed complete. [[tickets/T-27000-phase-2]], [[tickets/T-27055-final-astra-review]], [[tickets/T-27056-final-astra-fixes]], [[status]], [[asset-export]].

## [2026-10-25] ingest | Capture stable Phase 2 source witness and resume asset lanes

Captured pushed source plus final clean generated assembly in ignored `.asset-cache/shared/source-snapshot-phase2-687b5ce7`: 14,697 files, including 13,872 assembly files and 437 C files. The capture verifies live source digest and assembly hashes before/after copying; source-tree digest `75eafb5958b44eef4c1ead490eea7bc06372d094e2cf939854a3ab7b67b638cd`. Historical f9c evidence remains unchanged. Resumed existing text chat with Sol 6.1 and media chat with Luna for isolated current-source pin/enrichment candidates; root remains the sole live integration/wiki/Git writer. No parser or comprehensive asset-coverage completion is claimed. [[asset-export]], [[tickets/T-28000-asset-export-pipeline]], [[tickets/T-28001-text-script-export]], [[tickets/T-28002-media-asset-export]].

## [2026-10-25] build | Integrate reviewed text readers against the finished source layout

Applied the exact T-28001 sealed packet after independent Standards and Sol 6.1 correctness reviews. All 50 source/test/doc hashes match; 37 files changed, preserving prior dirty documentation. Root's isolated integration passes 437 canonical asset tests with no skips, and the live replay passes 233 text tests plus current source/table/renderer/substitution and JSONL/TSV checks. Independent review verified 612,332 original instruction words across 27 images and 38,696 units against 125 original inputs. Original payloads/control/voice semantics and verified identities are preserved; one unresolved global locator migrates explicitly. All 1,072 protected C/header/config/build files are unchanged, so the matching game was not rebuilt. The finished-source witness remains immutable; no copyrighted output is staged. Comprehensive text coverage is still false with 1,889 unknown extents. Media rebase/export candidates and image-text coverage remain open. [[tickets/T-28001-text-script-export]], [[tickets/T-28000-asset-export-pipeline]], [[assets-text]], [[asset-export]], [[status]].

## [2026-10-25] build | Integrate reviewed media source bindings and ETC SD color exports

Applied the exact 22-file media packet after independent Standards and Sol 6.1 correctness reviews. Finite tree/build-digest, stale HUD citation and test-count metadata findings are resolved; earlier packets remain immutable. Original ETC SD tables bind SD_P30/SD_P30W to eighteen native-color frames with raw/indexed/STP/CLUT/sequence and EXE/ETC dependency provenance. Independent raw-disc, native, regenerated PNG and manual-review checks pass; 1,370 nonselected records remain unchanged. The focused suite passes 91 tests and the coherent current-text/media suite passes 441 tests, zero skips. The static catalog has 359 image sources / 3,480 regions (425 Japanese, 350 other text, 2,635 no text, 70 suspected); all completeness flags remain false. All 1,072 protected game-source/header/config/build files are unchanged. No copyrighted exports are staged; 73 NBN sources, 235 background geometries and movie/whole-corpus localization work remain open. [[tickets/T-28002-media-asset-export]], [[tickets/T-28004-japanese-image-catalog]], [[assets-media]], [[asset-export]], [[status]].

## [2026-10-25] ingest | Verify a bounded movie Japanese-text inventory

Independent visual review confirms Japanese text in each of sixteen original-color OPENXA output frames 392–407. Individual PNG hashes/dimensions, source frame-header fields, sector-index lists and the original raw-track extent all match the ignored review packet `790bdd9742e2c6c1534ae46bf521544e9289e5b390329cb217cb2b20bc4a56bd`. No dialogue, transcriptions or images are committed. The window is separate from the static corpus; remaining movie frames, timing/runtime presentation, hardware pixel parity and the replacement method remain unproved. Later localization needs edited frames or synchronized replacement compositing. No tooling/game/build source changed. [[tickets/T-28004-japanese-image-catalog]], [[assets-media]].

## [2026-10-25] decision | Publish verified asset checkpoints and hold the next BS candidate for repair

Pushed reviewed text/media/OPENXA commits through `0d8f98a3244ba452473629263daa49de077344b8` to `origin/main`, independently confirmed by remote query. Coherent 441-test suite passes without skips; all 27 unchanged binary hashes still match, and committed changes are confined to reviewed asset tools/tests/wiki. No game-source or build change required a rebuild. The next isolated four-BS candidate is not applied: independent negatives expose self-rehashed witness acceptance and missing exact asset/index/LBA binding, despite correct original four-file pixels. Its owner is repairing trusted proof closure, table binding, citation and full visual-catalog integration. RPG initial-buffer and other semantic work remain isolated; comprehensive asset coverage remains false. [[tickets/T-28000-asset-export-pipeline]], [[tickets/T-28002-media-asset-export]], [[asset-export]], [[assets-media]].

## [2026-10-25] ingest | Integrate five flagged original RPG buffer views

Applied the exact eight-file RPG initial-buffer packet after independent Standards and Sol 6.1 correctness reviews. All 52 after-images match; the two new Python files were absent before application. Root's coherent suite passes 453 canonical asset tests without skips. The live replay passes 245 text tests and original/source/delta/JSONL/TSV proofs: 56 pins, 604 instruction words, 220 selected initialized bytes and 38,701 null-target rows. All 38,696 old rows are preserved except five additive provenance links; five new initial-only views remain flagged, ready rows stay 31,346 and unknown extents stay 1,889. The 1,072 protected game/header/config/build files are unchanged. No runtime contents, reads, display lifetime or full text-coverage claim follows; SPEAKTST remains explicitly unresolved. No game data is staged. [[tickets/T-28001-text-script-export]], [[tickets/T-28000-asset-export-pipeline]], [[assets-text]], [[asset-export]], [[status]].

## [2026-10-25] ticket | Start a disjoint sequence-music export lane

Created T-28006 In Progress for the user's existing general/music asset scope and added its board card. A bounded Luna assignment will prepare original SEQ inventory, primary/source-backed event semantics and an actual lossless reader/JSON/MIDI candidate with dependency metadata. WAV rendering remains a later acceptance requirement; samples/MIDI alone are not completion. All real outputs remain ignored, and root retains shared tool/wiki/Git ownership. [[tickets/T-28006-sequence-music-export]], [[tickets/T-28002-media-asset-export]], [[asset-export]], [[assets-media]].

## [2026-10-25] ingest | Integrate four strictly bound school-day backgrounds

Applied the repaired six-file BS v4 packet after independent correctness and standards reviews. Trusted native/source/EXE/table/file pins resolve the earlier fabricated-witness and wrong-row substitution findings; placement metadata now retains conditional VRAM rectangles and runtime-unknown draw origin. The coherent suite passes 466 tests without skips. Live replay reproduces all sixteen PNG hashes, preserves 1,368 unselected records including both ETC SD sources, and verifies all 6,963 standard-catalog artifacts. The static catalog has 363 image sources / 3,484 active regions (427 Japanese, 350 other text, 2,635 no text, 72 suspected, zero unreviewed); twelve diagnostics are excluded from art totals. Background geometry backlog is 231, NBN remains 73, and whole-corpus/hardware parity claims stay false. All 1,072 protected game/header/config/build files are unchanged. Earlier reviewed RPG work was pushed through `64fa0606d6ab6789c75d8ae5d424bb2782b214e1`, independently confirmed on origin/main. Exact replay/profile/catalog hashes and limitations are in [[assets-media]], [[tickets/T-28002-media-asset-export]], [[tickets/T-28004-japanese-image-catalog]], [[asset-export]] and [[status]].

## [2026-10-25] ingest | Prove twelve native shooter display-input contexts

Applied the exact eight-file shooter packet after independent Standards and Sol 6.1 correctness reviews. Root verifies 54 after-images and unchanged hashes for all 34,945 tracked files outside the application. The coherent suite passes 478 tests without skips. Live replay passes 257 text tests, 53 source pins, 161 renderer words, six byte-reader facts and twelve original argument/call/delay-slot paths; complete catalog/delta and JSONL/TSV replays preserve all 38,701 units, readiness, payloads, IDs and null targets. Unknown extents decrease from 1,889 to 1,877. Glyph modeling, runtime values/execution and complete text coverage remain unproved. The first isolated test run lacked the original-input-root environment for its `/repo` mount; that harness path was corrected and the successful run retains the earlier failure evidence. No game source/build files changed. [[tickets/T-28001-text-script-export]], [[tickets/T-28000-asset-export-pipeline]], [[assets-text]], [[asset-export]], [[status]].

## [2026-10-25] ingest | Export four original seasonal school-day backgrounds

Applied the independently reviewed three-file seasonal extension. Original selector/table/native/file proofs verify 753 instructions and sixteen rejecting negative cases. The coherent current-source suite passes 483 tests without skips; the application preserves all 34,950 other tracked files. Actual replay reproduces sixteen PNG hashes, retains all 1,368 nonselected records and verifies 6,971 catalog PNG/mask references. The merged static census is 367 image sources / 3,488 active regions (427 Japanese, 350 other text, 2,635 no text, 76 suspected, zero unreviewed); the four new regions remain suspected. Twelve diagnostics are excluded from art counts; native read overreads never enter decoder input. BS geometry backlog falls to 227; NBN remains 73, while hardware parity/movie/whole-corpus claims stay open. The legacy ignored-packet tool-hash field is distinguished from the unchanged game-source witness. Prior shooter work was pushed through `fb25ca394c37ec4c4ad37bb299534a00fba65bbc`, independently remote-verified. Exact artifacts and provenance are in [[assets-media]], [[asset-export]], [[tickets/T-28002-media-asset-export]], [[tickets/T-28004-japanese-image-catalog]] and [[status]].

## [2026-10-25] ingest | Export lossless sequence packages and provisional MIDI

Applied four new music source/test files from reviewed v5 after all native/parser/validator findings were resolved; all 34,953 existing tracked files are preserved by the application. The coherent suite passes 493 tests without skips. Both installed CLI modes validate structured reconstruction, source/event/ID fields and identical repeated writes across 63 original files / 177 entries / 1,008 aliases; all original bytes remain unchanged. The supported subset has 260,108 events and 138 provisional MIDI exports, while 39 streams remain raw-only. All MIDI bytes match reviewed outputs. Preview JSON contains output paths, so a separate audit verifies only path relocation and dependent JSON checksums/sizes after the first cross-directory byte assertion proved too strict. No native music WAV, bank/timing parity or whole-corpus completion is claimed. Exact hashes/commands/limits are in [[assets-media]], [[tickets/T-28006-sequence-music-export]], [[tickets/T-28002-media-asset-export]], [[tickets/T-28000-asset-export-pipeline]], [[asset-export]] and [[status]].

## [2026-10-25] lint | Refresh the generated ticket catalog

Regenerated [[tickets/catalog]] from current ticket frontmatter: 1,016 tickets, including T-28006 and the already completed phase 2 statuses. Ticket files and board statuses are unchanged; the generated catalog had retained older columns. Unrelated generated note catalogs retain their exact before-images. No source acceptance or asset-completeness status changed.

## [2026-10-25] decision | Hold isolated NBN and music extensions for native repairs

Published the reviewed seasonal/music v5 commits through `ee27829ae9620b9388e96f80861497b8ee58accc`, independently confirmed on origin/main after 493 coherent tests and all 27 unchanged matching hashes passed. The next NBN candidate stays unapplied despite 498 passing composed tests: independent review reproduces escaped writes from unchecked baseline IDs and incorrect native bottom-row sampling, with missing per-cell catalog fields. Music v6 stays isolated because native cached-FF meta events are incorrectly rejected, despite otherwise proved tempo/control support. Authors prepare separate immutable revisions with actual fixes, fresh output review and independent replay. Current source, exported counts, raw originals and asset-ticket statuses remain unchanged. [[tickets/T-28002-media-asset-export]], [[tickets/T-28006-sequence-music-export]], [[assets-media]], [[asset-export]].

## [2026-10-25] ingest | Decode every current-game SEQ event stream

Applied the exact reviewed v9 after native cached-meta, bounded-EOT, framing-label and output-identity findings were resolved. All four source/test after-images match and all 34,953 other tracked files are preserved by the application. The coherent suite passes 498 tests without skips; installed default/audition CLIs reconstruct and validate all 63 originals / 177 entries / 1,008 aliases, decode 337,797 events and export 177 MIDI files. Original input hashes and the earlier 138 supported channel streams remain unchanged. Native tempo units are proved through 1,212 original words and the full timing chain; header rounding, meta truncation and scheduler quantization prevent playback-parity claims. Output relocation changes only embedded cache paths and dependent JSON checksums/sizes. All original SEQ event streams are covered; bank/WAV/loop rendering, text/image/movie gaps and the held NBN candidate remain open. [[assets-media]], [[asset-export]], [[tickets/T-28006-sequence-music-export]], [[tickets/T-28002-media-asset-export]], [[tickets/T-28000-asset-export-pipeline]], [[status]].

## [2026-10-25] ingest | Prove sixteen native label and ranking contexts

[[tickets/T-28001-text-script-export]] and [[assets-text]]: source/code contract verifies 58 pins, 569 original words in four native functions, 39 required facts and three guarded byte paths. All 273 text tests pass without skips; sixteen disjoint existing extents reduce unknowns from 1,877 to 1,861. All 8,837 physical payloads/IDs, 38,701 unit IDs/source/control data, null targets and readiness persist. Glyph model, runtime values/execution and complete coverage remain unproved.

Root application verifies all 56 captured before/after images and preserves 34,953 unrelated tracked files. The coherent applied suite passes 514 tests without skips; game source, headers, configuration and build remain unchanged. Independent native/Standards reviews and exact packet pins are recorded under `.claude/orchestrator/state/T-28001/`. All asset tickets remain In Progress.

## [2026-10-25] ingest | Preserve conditional sequence bank provenance

[[tickets/T-28006-sequence-music-export]] and [[assets-media]]: independently reviewed v3 verifies source/native/ISO facts and all 63 original SEQ identities, producing 53 conditional load requests across 45 paths. Playback assignments stay null; 18 paths, runtime epochs, sample/tone rendering and native WAV remain open. Source enumeration order and fresh modified-source negatives pass. Five new tooling/profile/test files preserve 34,959 existing tracked files; all 528 installed asset tests pass without skips. Initial isolated harness runs lacked logical original/package fixtures and are retained alongside the corrected complete-fixture pass; no production validation was bypassed. [[asset-export]] and parent ticket updated, all asset tickets remain In Progress.

Installed bank CLIs reproduce the reviewed graph and a current-catalog graph differing only in its catalog hash; export/validation repeats are deterministic and all 63 originals remain unchanged. Early audit-only stdout parsing and field-name mistakes are preserved separately; the corrected live CLI proof is PASS. No production code changed to satisfy these root audits.

## [2026-10-25] query | Hold NBN integration for test isolation

[[tickets/T-28002-media-asset-export]]: the bounded v2 native/pixel/output repairs pass independent review, but root composition catches import-time test mutation of shared repository/cache globals, breaking an existing CDDA test. The 536-test failure is retained; no live NBN code was applied. Owner prepares separate v3 with scoped cleanup and regression coverage. Current published bank/text tooling remains at 528 passing tests; original data and game source are unchanged.

## [2026-10-25] query | Hold instrument metadata for declared size validation

[[tickets/T-28006-sequence-music-export]]: independent VAB reader review checks all 63 original bank pairs and identifies an omitted/unchecked declared total size. Sixty bank mappings pass independent byte/field/extent checks and three remain explicitly unsupported. Candidate v1 stays isolated; owner prepares v2 size validation and complete status inventory. Published sequence and conditional bank tooling retain 528 passing tests; no new asset/tool completion or native WAV claim.

## [2026-10-25] ingest | Preserve SPEAKTST layout candidates without runtime claims

Added the pinned twelve-bank conditional layout inventory described in
[[assets-text]] and [[asset-export]]. The reader retains 16,702 flagged candidate
texts, twelve FF controls, four unknown tokens, and all 1,253,376 original bytes;
fixed-stride records and metadata remain opaque. Projection appends only flagged,
null-target text rows while preserving the existing 38,701 rows. File installation,
runtime execution, glyph bindings, voice and character semantics remain unproved;
the physical unknown count remains 1,861. [[tickets/T-28001-text-script-export]]
remains In Progress, and full text coverage remains incomplete.

Root verifies nine tooling changes against 59 captured after-images and preserves 34,960 unrelated tracked files. Independent production/native and Standards reviews pass; the coherent isolated suite passes 550 tests without skips. Original game source, headers, configuration and matching build remain unchanged.

Installed normal/optimized inventory CLIs, all 55,403 serialized unit fields and 295 text tests per mode pass; actual coherent asset suite: 550 tests, zero skips. Existing rows/readiness and the original byte evidence remain unchanged.

## [2026-10-25] query | Require canonical NBN cell identities

[[tickets/T-28002-media-asset-export]]: v3 resolves test isolation and passes 559 installed asset tests after the SPEAKTST integration. Actual export succeeds, but generic visual catalog construction rejects cells without distinct frame/variant identities. The three applied tooling files remain uncommitted; owner prepares v4 metadata and direct-catalog regressions. All native pixel/source evidence and previous outputs remain preserved. Published text tooling is at 55,403 export rows with 16,702 new flagged candidates; no full text/media or native rendering completion claim.

## [2026-10-25] ticket | Plan the Godot port with Astra

[[tickets/T-29000-plan-godot-port]] created In Progress for the user-requested roadmap completion update and an Astra-authored Phase 3 plan. Finished matching/readability phases are separate from ongoing [[asset-export]] work. Root retains source/wiki/Git integration ownership; original game inputs and pending asset changes remain untouched.

## [2026-10-25] ingest | Integrate the reviewed Astra Godot port plan

[[phase3]] records the source-backed engine/language/content/state/timing/text/audio/save decisions, first playable school-day route and later milestones. [[tickets/T-29000-plan-godot-port]] moves to In Review after independent Standards/spec PASS and the applied-name correction; six child implementation tickets are created, with foundation/oracle/content work Ready and dependent controllers/presentation/services Backlog. ROADMAP.md and [[status]] distinguish completed decompilation phases, the planned port and continuing [[asset-export]] work. No engine project, game data or runtime parity is claimed.

## [2026-10-25] ticket | Complete the Godot port plan

[[tickets/T-29000-plan-godot-port]] moves from In Review to Done after independent integrated Standards and spec PASS with no remaining findings. [[phase3]] is the canonical Astra-authored plan; six implementation tickets retain three Ready and three Backlog statuses. ROADMAP.md marks Phases 1 and 2 complete and Phase 3 planned. This closes planning, not an implemented or validated port. Original inputs, matching source/build and pending asset work remain unchanged.

## [2026-10-25] ingest | Export reviewed prologue NBN native cells

[[tickets/T-28002-media-asset-export]] and [[tickets/T-28004-japanese-image-catalog]]: completed reviewed v4 source/output safety, native crop, test isolation and canonical identity repairs. Installed CLI/source-to-pixel/manual review/full catalog/gallery proof passes; all 1,368 nonselected assets and 3,488 prior region records persist. Static coverage becomes 371 sources / 3,512 active regions, including 24 new bounded no-text cells and separate raw bit-15 diagnostics. All 560 coherent asset tests pass without skips. Failed audit artifacts remain; JSON object-key representation was diagnosed with every inherited field/value preserved. [[assets-media]], [[asset-export]] and parent ticket updated. All asset tickets remain In Progress; matching source/build and original data unchanged.

## [2026-10-25] ticket | Begin the Godot implementation tranche

User authorized starting the port alongside continuing asset work. Claimed [[tickets/T-29001-godot-engine-foundation]], [[tickets/T-29002-native-runtime-and-oracle]] and [[tickets/T-29003-port-content-adapter]]; added scoped standards at port/godot/PORTING.md before implementation. [[phase3]] remains the approved plan. Matching source is protected; no playable or parity claim.

## [2026-10-25] build | Verify bank-local VAB metadata while port work starts

Installed V2 source and seven synthetic tests pass within 567 coherent asset tests, zero skips. Actual read-only exports cover all 63 original pairs, preserving explicit unsupported shapes and null rates; 60 banks yield 709 programs, 1,721 tones and 1,158 sample slots. [[assets-media]] records ignored outputs and rendering limits. [[tickets/T-28006-sequence-music-export]] stays In Progress. The earlier five-title text packet is held for corrected native club predicates; [[tickets/T-28001-text-script-export]] records the finding without changing coverage counts.

## [2026-10-25] decision | Preserve the concurrent roadmap edit

Remote main advanced with a user-authored ROADMAP.md simplification while the reviewed instrument commit was ready. Inspected and merged only that remote change without overwriting local port work. Published asset commit ffeaf8b7f with merge ec78bd292; [[tickets/T-28006-sequence-music-export]] remains open for synthesis and runtime binding. Marked Phase 3 In Progress in the working roadmap for the user-authorized implementation.

## [2026-10-25] ticket | Add a visual-only local port progress page

User requested an updating web page with screenshots and audio/video clips, without written progress updates, and explicitly chose a local browser page. Created [[tickets/T-29007-visual-port-progress-viewer]]; source/tooling stays separate from ignored captures. Port/runtime/content and asset work continue in parallel.

## [2026-10-25] ticket | Verify port foundation and review bounded runtime/content packets

[[tickets/T-29001-godot-engine-foundation]] moves to In Review after real headless and Mesa rendering proofs; its V3 addresses both independent false-pass findings and awaits original-reviewer verification. [[tickets/T-29002-native-runtime-and-oracle]] has executable OpenBIOS/instruction witnesses with explicit route gaps; failed-session fixes are under recheck. [[tickets/T-29003-port-content-adapter]] remains held for four contract fixes. No port candidate has been applied to live source yet. The separate visual viewer [[tickets/T-29007-visual-port-progress-viewer]] is active.

## [2026-10-25] ticket | Advance from foundation to the first playable slice

[[tickets/T-29001-godot-engine-foundation]] is Done after both review axes and actual installed headless/rendered tests. [[tickets/T-29002-native-runtime-and-oracle]] now has a genuine ordinary command boundary with two deterministic replays, without RAM edits. User requested faster port progress; started bounded parallel implementation under [[tickets/T-29004-school-day-playable-rules]] and [[tickets/T-29005-native-scene-presentation]], keeping their full route acceptance gates. [[tickets/T-29007-visual-port-progress-viewer]] now watches only the approved port capture folders automatically; actual Godot art preview is visible and no synthetic QA footage is on the real page.

## [2026-10-25] decision | Compare the playable slice against the emulator and fix gallery order

User explicitly requested emulator comparisons for the port. Existing genuine command-boundary and command0/7 queue captures provide the comparison baseline under [[tickets/T-29002-native-runtime-and-oracle]] and [[tickets/T-29004-school-day-playable-rules]]; rendered checkpoint comparisons remain required by [[tickets/T-29005-native-scene-presentation]]. Fixed the left/right gallery controls to follow visible thumbnail order under [[tickets/T-29007-visual-port-progress-viewer]], including keyboard controls.

## [2026-10-25] build | Fix the real progress clip layout

Browser inspection found the actual Godot activity clip decoded but overflowed its player frame vertically, putting controls out of reach. Fixed contain-fit image/video layout under [[tickets/T-29007-visual-port-progress-viewer]] and rechecked the real clip rather than only synthetic media.

## [2026-10-25] ticket | Add a disjoint save and audio service lane

User requested more parallel port work. Started bounded save/schema and ordered audio-service implementation under [[tickets/T-29006-port-save-audio-services]], using the frozen session contracts and explicit native-audio limits. The existing text worker has a sealed revision awaiting root review and can prepare this separate packet without overlapping rules, presentation, content adapters or shared source.

## [2026-10-25] build | Integrate the validated static content adapter

Applied final V4 under [[tickets/T-29003-port-content-adapter]] after independent Standards and spec checks resolved the shared schema and runtime failures. Installed tests pass three Godot suites and ten Python/Draft 2020-12 cases; the test-only JSON Schema dependency is pinned and remains Docker-only. Actual external preview assets remain ignored. The first-activity table/media adapters remain separate bounded packets; full route and parent-ticket acceptance stay open.

## [2026-10-25] ticket | Assemble the working slice while module reviews run

Started [[tickets/T-29008-assemble-playable-development-app]] to join the already runnable rules/presentation/content packets through their fixed contracts. The user requested faster parallel port work; this disjoint lane provides a visible application without waiting for unrelated corpus coverage. Only verified route commands may be enabled, and full original HUD/audio/New Game acceptance remains open.

## [2026-10-25] ticket | Fan out the next independent port modules

User explicitly authorized more agents and chats. Created [[tickets/T-29009-ordinary-week-continuation]], [[tickets/T-29010-native-hud-font-rendering]] and [[tickets/T-29011-native-dialogue-player]] with disjoint source ownership and ignored-packet handoffs. Ordinary-week, glyph/HUD and dialogue work can proceed alongside the first activity app, asset adapters and save/audio lane. Root keeps canonical integration and review authority.

## [2026-10-25] decision | Report full-port estimates and keep the gallery usable

User requested overall percentage updates. [[phase3]] records the complete Japanese-port scope and coarse weighting; current estimates are about 5% overall and 60% first slice. [[tickets/T-29007-visual-port-progress-viewer]] shows the numeric estimate with actual captures, serves a still immediately, and excludes intermediate raw video frames from the timeline. All source media/evidence remains ignored and retained. Three additional chats now prepare ordinary-week, HUD/glyph and dialogue work in parallel.

## [2026-10-25] ticket | Assign Astra text completion and translator handoff

User explicitly requested an Astra agent for remaining text work and notification as soon as their translation can start. Created [[tickets/T-28007-complete-text-and-translator-handoff]]: prioritize a usable checked ready-text queue with stable IDs, protected tokens and return validation; keep unresolved evidence separate and continue source-backed coverage work. Existing ready rows can be translated while runtime context remains under investigation. No package is accepted yet. [[assets-text]].

## [2026-10-25] build | Integrate the reviewed first-activity rules

Applied the seven-file [[tickets/T-29004-school-day-playable-rules]] packet and two registration patches after exact before/after and independent-review checks. Four canonical Docker suites pass. Fresh canonical replay against genuine command-0/7 native checkpoints reports zero failures for queue, girl records, calendar, parameters, RNG, mode/timers and isolated first-result behavior. All 564 existing game C/header files remain unchanged. First harness invocation omitted the writable proof mount and failed; the corrected isolated proof run passes and retains the failed diagnostic. Presentation/content/app integration remains open; no full route-timing claim.

## [2026-10-25] build | Integrate bounded ordinary-week continuation

Applied final reviewed [[tickets/T-29009-ordinary-week-continuation]] packet with exact controller and mode32/week-test composition. Five canonical suites and all 28 fresh original-emulator checkpoint comparisons pass. Native snapshot writes update only the proven parameter slice; source-profile edits and unsupported holiday progression fail transactionally. Presentation cache/HUD integration remains open. Then applied the reviewed presentation counter correction and strict table/media adapters after resolving profile and sequence-length validation findings. All existing game C/header files remain unchanged. Actual canonical checks pass 21 route Python tests, standalone Godot presentation tests and the real-pack Godot getter/negative suite; no whole-game parity claim.

## [2026-10-25] ingest | Accept the first translator handoff

Both independent review axes accept [[tickets/T-28007-complete-text-and-translator-handoff]]'s initial editable queue and return validator. Root installed both exact tooling after-images, independently verified all archive IDs/counts and source evidence hashes, and passed 19 actual tests plus all 332 live return batches. All 55,403 rows are preserved: 31,346 ready for review and 24,057 unresolved. User notified immediately, then requested repository/worktree operation instead of ZIP delivery. [[assets-text]] now documents generating a separate editable ignored queue in that worktree. No translation, original-text Git payload or comprehensive coverage claim. Astra continues remaining source/native gaps.

## [2026-10-25] decision | Image translation ownership and deferred artwork

The user confirmed Gemini's translation pipeline also covers image text and same-size wording/layout suggestions for icons and other graphics. Recorded in [[phase3]] and [[tickets/T-28004-japanese-image-catalog]]: preserve asset identity, dimensions and rendering context for later translated-image creation; no replacement art now, and no changes to Gemini's pipeline or editable translations.

## [2026-10-25] build | Verify canonical development app and visual viewer

The six-file [[tickets/T-29008-assemble-playable-development-app]] focus-v2 integration passes 26 focus checks, ten original-state/effect comparisons and render schedules 0/1/3. Actual rendered command 0 reaches the declared first result; native HUD, New Game, audio and week presentation remain open. Both independent reviews accept the ten-file [[tickets/T-29007-visual-port-progress-viewer]] implementation and root reran its twelve Docker tests. Explicit publication of canonical captures deduplicated all four existing images. Existing workers resumed ordinary-week presentation and native HUD work; dialogue has parallel independent reviews. Rough first-slice/full-port estimates remain 60%/5%, not measured coverage.

## [2026-10-25] build | Integrate reviewed DATE context without changing translation

Applied the three exact reviewed files for [[tickets/T-28007-complete-text-and-translator-handoff]]. Fourteen installed tests and actual original-input replay pass: 3,527 native instructions, 293 sections and 5,237 existing unit IDs. [[assets-text]] records the optional context lookup and its runtime limits. Gemini's pipeline and editable translations remain untouched. Port work continues in parallel on actual week presentation, native HUD, strict command-7 content and real-session save/reload; dialogue finite findings are assigned for repair.

## [2026-10-25] build | Advance full-port integration and repair font output boundary

[[tickets/T-29010-native-hud-font-rendering]] review found an exporter accepting cache-root lookalikes outside the repository's ignored cache. Root prepared a separate two-file fix and five passing regression tests; independent re-review is pending. The HUD owner continues actual source/runtime reconstruction rather than substituting a captured original screen. [[tickets/T-29009-ordinary-week-continuation]] found a real mirrored progress-tail mismatch in original comparisons; a narrow source-proved correction is authorized in isolation, without dropping comparison fields. Save/reload, command-7 production media and dialogue repairs remain active parallel work toward the full [[phase3]] scope.

## [2026-10-25] ticket | Start independent New Game initialization lane

Created [[tickets/T-29012-native-new-game-initialization]] for additive native initialization/profile rules and original-state/RNG proof. This moves toward [[phase3]] P3.3 without replacing the first-slice integration work or treating an initializer as a complete opening. Root keeps shared app/controller/schema ownership; original tables remain external and translation work stays with Gemini.

## [2026-10-25] build | Correct native font fade quantization in isolation

Independent review of [[tickets/T-29010-native-hud-font-rendering]] verified all 3,418 original glyph bitmaps but found a fade rounding mismatch. Root prepared cumulative four-file `font-fix-v3` with native five-bit modulation before display expansion, retaining the accepted cache-containment correction. Godot regression tests pass; fresh independent resolution checks remain required before application. [[tickets/T-29012-native-new-game-initialization]] has begun source-backed rules and external-table integration; its first incomplete scratch is explicitly not accepted. Ticket catalog/lint reports zero broken links and status mismatches.

## [2026-10-25] build | Integrate verified native font rendering and HUD records

Applied six reviewed files for [[tickets/T-29010-native-hud-font-rendering]], including the resolved cache-root and native fade-quantization findings. Root's canonical tests pass five Python cases, the Godot HUD suite, all 3,418 original glyph bitmaps and 2,064 native palette/brightness images. No original font payload or captured native screen was added to source. The dynamic HUD owner now builds on these canonical files; whole-screen composition and gameplay integration remain open.

## [2026-10-25] decision | Serialize native proof and preserve port acceptance scope

The HUD worker released its private emulator session; root verified no live tokimemo-emu containers before granting exclusive use to [[tickets/T-29012-native-new-game-initialization]]. New Game's private function-level differential probe is now active, while HUD composition proceeds from recorded native command-selection evidence. Updated [[status]] with accepted font integration and explicitly separate pending week, HUD, media, save and dialogue work. No isolated candidate or passing synthetic suite is counted as full-playable acceptance.

## [2026-10-25] build | Integrate actual ordinary-week application flow

Applied reviewed [[tickets/T-29009-ordinary-week-continuation]] app-v1 after exact before/source/input/proof pins and successful launcher exit verification. Canonical five suites, 26 focus checks, 243-step headless schedules and actual rendered playback pass. All 15 original checkpoints include full progress and three timers; ten first-result checks also pass. The source-proved mirror-tail correction resolves the earlier byte mismatch without dropping comparisons. Captured 250 nested raw frames and published named actual port media; unsupported April 9 remains an unchanged-state stop. No full-week holiday or full-port completion claim.

## [2026-10-25] decision | Keep original character initialization data external

Independent review of [[tickets/T-29012-native-new-game-initialization]] found original girl birthday/blood arrays embedded in the candidate. Confirmed the [[phase3]] external-content requirement applies; author must repair in a separate version and retain original native endpoint comparisons. The reviewed original-table loader does not itself waive other source-derived character data. No New Game code applied yet.

## [2026-10-25] build | Hold rest-command content integration on clean CLI failure

Applied the nine exact reviewed [[tickets/T-29003-port-content-adapter]] command-7 files, then tested the documented production CLI without special PYTHONPATH. It fails before export on package import (`assets`); author is repairing the finite import boundary and adding a clean CLI regression. Keep the application uncommitted until actual replay and review pass. Created [[tickets/T-29013-native-holiday-command-loop]] for the next original holiday-to-command route and granted the week owner the released exclusive private emulator lease.

## [2026-10-25] ticket | Coordinate rest-command app and holiday continuation

The frozen dialogue v2 is in fresh independent review. Its owner now prepares a disjoint isolated command-7 app candidate under [[tickets/T-29008-assemble-playable-development-app]], preserving command-0 week behavior and proving the original rest endpoint. [[tickets/T-29013-native-holiday-command-loop]] reached the genuine holiday menu and continues native input toward the next command boundary with private cards. Protected game source and headers remain unchanged from Phase 2; only the pre-existing m-flow proposal differs under config.

## [2026-10-25] build | Accept clean production rest-command content import

Resolved the [[tickets/T-29003-port-content-adapter]] CLI import hold using reviewed package-relative imports and direct-script bootstrap. Root's actual clean CLI succeeds; all 46 canonical tests and both Godot pack versions pass. Final pack manifest, 40 runtime PNGs and 40 retained reader PNGs match reviewed outputs. The expanded loader/exporter is accepted as content infrastructure; the rest-command app and native rendering proof remain pending. No matching game source, original input or Gemini translation changed.

## [2026-10-25] build | Canonical stable-boundary save continuation

Integrated the independently reviewed v2 save codec and playable-session adapter for [[tickets/T-29006-port-save-audio-services]]. Actual canonical replay passed 37 filesystem/codec and 50 app checks, including all 80+40+243+163 continuation hash pairs at command/result boundaries. Evidence: `.claude/orchestrator/state/T-29006/live-save-proof/`. User-facing controls and audio remain open. Updated [[status]]; game source and Gemini translation files were untouched.

## [2026-10-25] decision | Production HUD boundary and dialogue fix review

Reviewed the dynamic HUD candidate for [[tickets/T-29010-native-hud-font-rendering]] and returned the private-oracle-dependent exporter for production adaptation; native actor/rendering evidence remains useful. Independently checked the finite dialogue v3 path correction for [[tickets/T-29011-native-dialogue-player]], with nine normal and nine optimized Docker tests passing. Original Spec review remains pending. Granted the holiday owner a bounded second emulator capture lease after checking no emulator was running. New Game owner continues the actual profile-entry route while its initializer is independently reviewed. Gemini translation inputs remain untouched.

## [2026-10-25] build | Publish actual dynamic HUD progress captures

For [[tickets/T-29010-native-hud-font-rendering]], root inspected the actual Godot/Mesa capture path and image, published two source-composed HUD captures, and verified exact hashes through the live viewer API. Oracle screenshots and synthetic state-change proof remain excluded. The production importer and app integration are still open; no acceptance or percentage increase is implied.

## [2026-10-25] build | Integrate reviewed dialogue and New Game rules

Integrated eleven exact reviewed source/test files for [[tickets/T-29011-native-dialogue-player]] and [[tickets/T-29012-native-new-game-initialization]]. Canonical tests pass: dialogue nine Python checks, fresh original 12-line reparse and both Godot suites; New Game 109 Godot checks, 13 Python checks and 19 preserved native state/profile/RNG comparisons; five existing shared suites. Runtime original data remain external. Full conversation and New Game UI/prologue acceptance remain open. Archived logs and stopped four obsolete failed three-hour-old Godot test containers; active workers and the visual viewer were preserved.

## [2026-10-25] decision | Hold rest app until save composition is fixed

Current-composition review for [[tickets/T-29008-assemble-playable-development-app]] reproduced two save integration defects despite passing standalone rest/native tests: unresolved rest draw IDs and lost command capabilities after load. Root preserved the working canonical app and dispatched the finite correction to the save owner. The rest owner continues [[tickets/T-29009-ordinary-week-continuation]] toward the full ordinary week under a bounded serialized emulator lease. Native HUD, New Game UI and holiday content work continue independently.

## [2026-10-25] ticket | Expand ordinary command content coverage

Opened [[tickets/T-29014-ordinary-command-content-coverage]] for commands 1 through 6 so full-game progress continues alongside rest/save, Sunday, HUD and New Game work. The owner prepares isolated original-input exporter/runtime capability changes; inventory alone is not completion, and root retains shared integration and emulator scheduling.

## [2026-10-25] build | Install reviewed New Game entry flow

Integrated seven reviewed source/test files for [[tickets/T-29012-native-new-game-initialization]]. Actual canonical replay passes 63 controller checks, five Python cases and eight rendered UI checks; the opening name-entry image is byte-identical to root's reviewed composition. Original kana/preset/font packs remain external, and no captured gameplay state is used. The scene stops before unimplemented prologue/service handling. Both review reports and exact installed source hashes are bound in the ignored live application receipt; ticket remains In Progress.

## [2026-10-25] build | Publish installed name-entry scene capture

Published only the actual installed Godot name-entry scene capture from [[tickets/T-29012-native-new-game-initialization]] to the local visual page and verified its served hash `2f752e66dd4e99f6c1ba24105029626b93fbbd2ff776bf411a53927d77931b12`. It is the reviewed development entry scene, not a native screenshot or native UI parity claim. Percentage estimates remain unchanged.

## [2026-10-25] build | Export reviewed code-only desktop development player

For [[tickets/T-29015-desktop-development-player]], added Docker staging/export tooling and six invented-source tests. Independent review resolved a source-root symlink traversal and the pinned engine's universal texture-import requirement. Actual v3 export verifies both macOS architectures, 41 source hashes and 81 packaged resource mappings without original game payloads. Host launch and input launcher remain open; this is not desktop release acceptance.

## [2026-10-25] ingest | Validate Gemini translation return

For [[assets-text]] and [[tickets/T-28007-complete-text-and-translator-handoff]], read the user-reported completed translation from its separate worktree and reran the canonical importer read-only against all 332 TSV batches. All 55,403 rows pass, with 54,786 targets; returned overlay matches the independently reconstructed result byte-for-byte. Supplemental text and image coverage are being audited separately. Gemini files remain untouched; no English content/art was applied to the port.

## [2026-10-25] build | Correct native DONE and START entry behavior

For [[tickets/T-29012-native-new-game-initialization]], integrated four reviewed corrections distinguishing native DONE's tab reset from keyboard START's retained tab. Real focused-keyboard dispatch is tested, with canonical 85 controller and ten rendered UI checks passing. No original content, translation pipeline or matching source changed. The previous v1 desktop artifact remains an explicitly historical build.

## [2026-10-25] ingest | Reconcile supplemental translation and image suggestions

Independent read-only audit for [[assets-text]] confirms 508 unique supplemental rows: 412 populated text records with targets and 96 empty karaoke layout slots, with matching TSV/JSONL and protected markers. Source locators are in range and disjoint from main units; per-row decoded-byte provenance and complete corpus coverage remain unverified. Image suggestions match inspected source/frame entries but lack a canonical region crosswalk; they do not imply replacement images or whole-image-text completion. Twelve main-return source labels mapped to whitespace are recorded for contextual formatting review. No Gemini files changed.

## [2026-10-25] build | Integrate reviewed rest command and save composition

Root applied the exact five-file command7 app packet and two-file save-v4 correction after independent Standards and original Spec re-verification. Actual canonical execution passes 111 route checks, 26 focus checks, 15 native week checkpoints, 28 rest save checks, 50 existing study save checks, 37 codec checks and five shared suites. Fresh Mesa rest playback reaches iteration 163; its full trace, final state, RNG and stop boundary equal the reviewed original-backed route. Both save findings are resolved: rest draw identities restore correctly and authenticated command capabilities survive restore. Evidence: `.claude/orchestrator/state/T-29008/command7-save-live/acceptance.json`; final review `.claude/orchestrator/state/T-29006/v4/final-spec-resolution.json`.

[[tickets/T-29008-assemble-playable-development-app]] and [[tickets/T-29006-port-save-audio-services]] remain In Progress. This remains a captured-boundary development app: study supports the bounded ordinary week, rest stops at its first result. New Game linkage, full rest-week progression, native HUD/audio and held save UI shortcuts remain separate open work. No game C/header/build changes or translation changes.

## [2026-10-25] build | Integrate original bank0 loading control

Applied the four-file prologue bank provider after independent Standards/source review PASS. The loader preserves original cache decisions, request ordering, byte-width counters, readiness/timeout predicates and ordered audio operations. Its content adapter verifies original source calls and ISO identities and exports authenticated metadata outside the project. Root installed tests pass: 24 Godot control checks and six Python checks in both normal and optimized execution. Review independently reproduced ten ISO bindings, two sequence headers and the external pack. Evidence: `.claude/orchestrator/state/T-29012/prologue-bank-provider-v1/{spec-standards-review,canonical-application}.json`.

Actual CD transport, driver readiness delivery, playback, native function timing and the connected normal prologue remain open. This component does not make New Game playable; T-29012 remains In Progress.

See [[tickets/T-29012-native-new-game-initialization]].

## [2026-10-25] build | Integrate original school command 1–3 media adapters

The four-file exporter/loader/test tranche is applied after independent source review and re-verification of both portability and external-pack findings. Installed execution passes five invented Python tests and 25 Godot assertions. A fresh canonical original-disc export reproduces all 103 reviewed files exactly, manifest SHA256 `b26ef7e5c0301633c45d9a6a898ca4bdb7be68ee30f163825eed827e4f5c0c28`. Proof: `.claude/orchestrator/state/T-29014/canonical-acceptance.json`; resolved review `spec-resolution-v2.json` in the same directory.

Only opening-week non-summer no-companion content for commands 1–3 is accepted. App/gameplay composition, commands 4–6 and other event/season variants remain open; the ticket stays In Progress. Original pixels and tables remain external under ignored `.asset-cache/port/spring-command-current-236ab3/`.

See [[tickets/T-29014-ordinary-command-content-coverage]].

## [2026-10-25] ticket | Track modernization and localization settings requests

Added Backlog tickets [[tickets/T-30001-modern-widescreen]], [[tickets/T-30002-modern-settings]], [[tickets/T-30003-player-customization]] and [[tickets/T-30004-modern-achievements]] and linked ROADMAP.md. Includes gender-aware player references and independent translation/voice/image-text options. RetroAchievements PS1 game 19805 currently lists no base achievements; https://retroachievements.org/game/19805 checked 2026-10-25. No modernization or translation runtime changes; full Phase 3 work continues.

## [2026-10-25] decision | Note optional English dub exploration

Recorded the user's possible English voice-pack idea in [[tickets/T-30002-modern-settings]], including performance evaluation, original audio selection and performer permissions. This is exploration only; no audio generation or upload.

## [2026-10-25] ingest | Add SNES achievement design reference

Updated [[tickets/T-30004-modern-achievements]] with the user's cross-version clarification. RetroAchievements SNES game3913/series hub lists97 achievements; single-session confession subset is a further optional reference. Our port may define its own verified gameplay goals. No achievement implementation yet.

## [2026-10-25] build | Integrate original Sunday command rules

Root committed the seven pinned GDScript files and router hook as `cf6767298` from reviewed packet `9b174d7cdf48427b618632a07d37cca394bcc65c2bcba5dc2cedb793ba5d2c0e`. Independent Standards/source reviews resolved all rule findings. Actual canonical execution passes all 185 original per-handler semantic transitions under render schedules 0/1/3, invented holiday rollback/order cases and five shared suites. Existing app checks still pass: 111 command0/7 checks, 26 focus checks and 15 native week checkpoints. Evidence: `.claude/orchestrator/state/T-29013/canonical-proof/acceptance.json`.

The controller is not yet attached to the visible app. Holiday media remains held for a separate shared-loader project-path exclusion finding; HUD and continuous holiday actor/render/save/focus integration remain open. Existing routes stop transactionally without holiday capabilities. One-step native cases do not prove continuous playback or audio parity. T-29013 remains In Progress.

See [[tickets/T-29013-native-holiday-command-loop]].

## [2026-10-25] decision | Require first-launch import and track all PS1 releases

User wants the shipped game to request their original copy once and retain extracted assets thereafter. Added Ready [[tickets/T-29016-first-launch-original-import]] and Backlog [[tickets/T-29017-support-ps1-releases]], updated [[phase3]] packaging acceptance. User clarified PS1 releases only. [[versions]] provides existing five-disc evidence; new release/runtime support remains unproved. End-user tooling requirements are removed from the intended shipping flow; development tooling stays Docker-only.

## [2026-10-25] ticket | Start first-launch importer implementation

Moved [[tickets/T-29016-first-launch-original-import]] Ready → In Progress. Assigned additive setup/controller and installed-content receipt work with real importer boundaries; a partial pack must not be advertised as a complete game installation. Root retains canonical integration.

## [2026-10-25] ticket | Research widescreen quality and modern settings

User requested agent research into modern resolutions/widescreen without stretched or poor-looking art, and reiterated conventional modern settings. Opened [[tickets/T-30005-widescreen-settings-feasibility]] as In Progress, linked [[tickets/T-30002-modern-settings]]. Research only; port implementation continues.

## [2026-10-25] decision | Keep generated widescreen scenery a last resort

Recorded the user's preference in [[tickets/T-30001-modern-widescreen]] and [[tickets/T-30005-widescreen-settings-feasibility]]. Prefer original art and framing/layout solutions; any later generated extension is optional, explicit and reviewed. No image generation performed.

## [2026-10-25] decision | Keep developer prose out of game presentation

Applied the user-requested deslop prose review to current authored app labels and recorded the product direction in port/godot/PORTING.md, [[tickets/T-29008-assemble-playable-development-app]] and [[tickets/T-29012-native-new-game-initialization]]. The skill scanner misclassifies GDScript syntax, so its code hits are not findings. Manual review found visible implementation disclaimers and raw state paragraphs; assigned cleanup to current HUD/New Game integration owners. Final presentation is the game, with opt-in debugging. No translated dialogue changed.

## [2026-10-25] build | Integrate four-day rest route

Root applied the five-file reviewed packet `7a7b095dc7d28744443ea41c0d06a1e6a71cde1794df524077af52c37b801418`. Canonical rest now continues through April 8 in 335 steps. Installed checks pass: 109 ordinary-week, 111 first-result, 26 focus, 16 continuing-save, 37 codec, 50 existing-save and 28 rest-save checks, plus bank/New Game and shared suites. Fresh Mesa playback matches the headless full trace, final state, RNG and checkpoints exactly. Native comparison reproduces all 29 control segments and 150 actor rows; three original 30-poll SD waits remain one preloaded-cache poll, with clock differences explicit. Proof: `.claude/orchestrator/state/T-29009/rest-week-app-v1/canonical-proof/acceptance.json`.

Holiday scene integration and native HUD/audio remain open. Original first-result behavior is retained when requested explicitly. User-requested removal of default developer text is assigned to the HUD successor; this packet preserves the old development layout.

See [[tickets/T-29009-ordinary-week-continuation]].

## [2026-10-25] ingest | Widescreen and settings research

Integrated the independently reviewed T-30005 recommendations into [[tickets/T-30001-modern-widescreen]] and [[tickets/T-30002-modern-settings]]. Preserve original art proportions and scene apertures, separate modern UI layout, and verify wider cameras per scene. Settings need accessible navigation and safe display rollback; renderer/platform behavior must be tested. Source audit and primary references remain in the ignored research handoff. No generated art or modernization runtime changes.

## [2026-10-25] query | Refresh current port integration status

Updated [[status]] with the accepted rest-week implementation and current scene/setup limits, superseding older paragraphs that still described rest integration as held. T-29012 UI cleanup is in independent two-axis review; T-29013 content path correction is being reverified before live application. Full Phase 3 remains open.

## [2026-10-25] build | Install reviewed holiday content

Applied the original holiday media packet and resolved shared project-root admission correction after independent review. All thirteen installed afterimages match the reviewed composition. Fresh original-input export reproduces all 129 files; 23 Python checks, actual Godot loader negatives, five shared suites and the current rest/study/save driver pass. See [[tickets/T-29013-native-holiday-command-loop]]. Connected Sunday gameplay is still being implemented; no full route or audio claim.

## [2026-10-25] ticket | Complete widescreen and settings research

Published the reviewed source-backed report in [[widescreen-settings]], linked the index and closed [[tickets/T-30005-widescreen-settings-feasibility]] with its board card. This completes research only; no modernization runtime, generated scenery or quality claim.

## [2026-10-25] build | Publish live HUD candidate captures

Inspected and published two actual Godot-rendered command/activity images from the T-29010 app successor to the existing local visual page. Default developer text is absent; original UI and scene content remain visible. These are candidate port captures, not original-emulator screenshots or a full-frame parity claim. Independent app/source review continues before canonical integration; the full-port percentage is unchanged.

## [2026-10-25] setup | Prepare repeatable native launch

Prepared an isolated direct-executable wrapper for [[tickets/T-29015-desktop-development-player]] to remove Launch Services process ambiguity during native export checks. It is a development aid, not the final install flow or an accepted native launch. Canonical runtime and original assets remain unchanged.

## [2026-10-25] build | Integrate minimal New Game presentation

Applied reviewed T-29012 cleanup v2 after both review axes resolved the permanent control-banner finding. All beforeimages and proof pins match. Installed eight UI/diagnostic, 85 controller, five shared and 13 actual rendered checks pass. Runtime validation and source initialization are unchanged. See [[tickets/T-29012-native-new-game-initialization]].

## [2026-10-25] build | Repair candidate setup control layout

Actual T-29016 rendering at 640x480 exposed offscreen import/cancel controls. Root's separate candidate v3 wraps the buttons; the actual scene now keeps all five within the window and passes the 55 flow assertions with required original fixture mounted read-only. Original setup fixes and this finite layout change await independent verification. Nothing is applied to the canonical setup flow; no packaged importer exists yet. See [[tickets/T-29016-first-launch-original-import]].

## [2026-10-25] decision | Standing cross-chat coordination authorization

The user explicitly reaffirmed that port and asset chats may coordinate and asked them to stop requesting permission for routine coordination. Relayed the direct human authorization to all five active external owners/review coordinators. Handoffs, review requests and findings may proceed directly; root retains canonical source/wiki/Git integration ownership.

## [2026-10-25] build | Integrate reviewed first-launch infrastructure

Applied seven T-29016 setup/store/test files after independent resolution of reentry, source-root overlap, preference recovery and visual layout findings. Canonical 55 workflow assertions and five shared suites pass; actual default-size rendering fits every control. Synthetic project configuration and existing content dependencies remain untouched. The packaged importer and boot connection remain open. See [[tickets/T-29016-first-launch-original-import]].

## [2026-10-25] build | Establish native startup path failure

Installed the reviewed direct development launcher and built a code-only macOS export from accepted New Game sources. A uniquely identified native headless process confirms an empty physical resource root, which turns the existing project-containment prefix into a rejection of all absolute paths. Shared exported-path handling is now a concrete implementation task; content checks were not bypassed. See [[tickets/T-29015-desktop-development-player]].

## [2026-10-25] build | Read sparse original VAB banks

Applied the reviewed two-file sparse VAB reader/test packet after root replay matched the original SDK/corpus proof. Installed tests pass normally and optimized. PRO_VOL and UNDOKAI inventory now succeeds; prior 118 outputs are unchanged and YUUEN remains explicitly rejected. Recorded the exact 121 paired-entry audit separately from earlier selected exports. See [[assets-media]] and [[tickets/T-28006-sequence-music-export]]. Playback/backend readiness remains open.

## [2026-10-25] build | Integrate commands 5 and 6 content

Applied the independently reviewed five-file capability packet and verified the installed clean exporter reproduces all 125 files. Fourteen Python, 90 Godot and five shared suites pass. Gameplay and other variants remain separate acceptance work. See [[tickets/T-29014-ordinary-command-content-coverage]].

## [2026-10-25] decision | Freeze reviewed low-usage port checkpoint

The user requested a good stopping point and push with 10% usage remaining. Directed workers to complete only current bounded operations, preserve exact checkpoints and idle. Added [[port-checkpoint]] with accepted behavior, unaccepted candidates, failed-probe limitation and next dependencies. Fresh original binary hashing passes 27/27; no full-port claim.

## [2026-10-25] build | Reviewed usage checkpoint ready for publication

[[tickets/T-28009-version-translation-overlays]] moved to Done after independent exact-payload/projection review; `translations/en/` preserves the user’s English returns without extracted Japanese fields. [[tickets/T-29015-desktop-development-player]] installs the reviewed shared path fix; actual macOS packed-export startup now accepts authenticated external rules and entry content. [[port-checkpoint]] records accepted work and remaining isolated drafts. Fresh matching proof remains 27/27 binaries. User authorized pushing this checkpoint; workers are idle and the emulator lease is released.

## [2026-10-25] build | Format English translation tree for GitHub contributors

Integrated `tools/translation_tree.py` and `tools/test_translation_tree.py` under [[tickets/T-28007-complete-text-and-translator-handoff]] and [[tickets/T-28009-version-translation-overlays]] to split `translations/en/returned-v3.jsonl` and `translations/en/supplemental.jsonl` into 184 per-archive Markdown table files grouped under `translations/en/dialogue/`, `translations/en/test_dialogue/`, `translations/en/system/` and `translations/en/supplemental/`, plus `translations/en/CONTRIBUTING.md`, `translations/en/INDEX.md` and target-only `translations/en/graphics/image_text_catalog.md` (`image_text_catalog.jsonl`). Character archive prefixes match [[spec/o-event]]. Docker verification passes 8/8 unit tests, confirms zero Japanese characters across all 192 files and reconstructs the pinned `returned-v3.jsonl` and `supplemental.jsonl` SHA-256 digests byte-for-byte without duplicate metadata blobs.

## [2026-10-25] build | Repair T-29012 native bank witness harness overlap

Under [[tickets/T-29012-native-new-game-initialization]], relocated the isolated PCSX-Redux bank witness trampoline from `0x801D0000` (which overlapped the `0x801C8000..0x801F0000` music body buffer) to `0x80150000..0x80151000`, added an explicit eight-region pairwise disjointness assertion and initialized `CdInit`, `sys_init` and `SpuInit(0)` before `audio_init()`. Exclusive Docker execution of `.claude/orchestrator/state/T-29012/connected-control-v2/run_witness.sh` hits all 21 marker breakpoints and records 120 binary dumps in `native/receipt.json`, proving `prologue_arrival` (`SYS_MIX` + `PRO_VOL`) and `normal_gameplay` (`COM_MIX` + `SPRING`) bank readiness transitions.

## [2026-10-25] build | Integrate T-29012 connected opening route and VAB memory installer

Applied nine reviewed files from `.claude/orchestrator/state/T-29012/connected-control-v2/` into `port/godot/` under [[tickets/T-29012-native-new-game-initialization]]: `port/godot/flow/new_game_command_reload.gd`, `port/godot/flow/new_game_state_adapter.gd`, `port/godot/flow/prologue_arrival.gd`, `port/godot/rules/new_game_route_state.gd`, `port/godot/services/native_vab_memory.gd` and four test suites (`run_new_game_command_reload.gd`, `run_new_game_state_adapter.gd`, `run_native_vab_memory.gd`, `run_connected_opening_route.gd`). Canonical Docker execution against `.asset-cache/port/connected-opening-v2` passes 110/110 assertions across committed name entry, fresh route state materialization, prologue arrival/finish with software sound RAM bank installation and `0x34` stage `0xFF -> 1` command reload. Normal prologue stages 1..20 remain open.

## [2026-10-25] build | Integrate T-29010 live HUD adapter, shared path helper adoption and T-29006 F5/F9 save controls

Applied the 20-file composed packet from `.claude/orchestrator/state/T-29010/composed-app-v2/` under [[tickets/T-29010-native-hud-font-rendering]], [[tickets/T-29006-port-save-audio-services]] and [[tickets/T-29015-desktop-development-player]]: adopted `ExternalContentPath.is_external_path` across `port/godot/services/school_day_media_store.gd`, `port/godot/services/weekday_56_content_store.gd` and `port/godot/presentation/command_hud_presenter.gd` (closing the POSIX `//` alias finding); integrated `port/godot/presentation/hud_compositor.gd`, `port/godot/presentation/live_hud_adapter.gd`, `port/godot/app/boundary_save_controls.gd`, `port/godot/app/playable_boot.gd`, `tools/port/hud_content.py` and `tools/port/run_playable.sh`. Canonical Docker verification passes 8/8 Python HUD exporter tests, 7/7 user-data containment cases, 34/34 external path checks, 9/9 slash alias checks, 50/50 live HUD app checks and 30 write + 38 read F5/F9 save control checks. Published four new live HUD captures to the local progress viewer.

## [2026-10-25] build | Wire T-29014 ordinary commands 1–3 and 5–6 into gameplay

Integrated `port/godot/services/ordinary_command_media_adapter.gd`, `port/godot/tests/app/run_ordinary_commands_1_6_tests.gd`, `ExternalContentPath.is_external_path` adoption in `port/godot/services/spring_school_day_content_store.gd` and ordinary commands 1–3 and 5–6 rules/controller/boot wiring across `port/godot/flow/command_screen.gd`, `port/godot/rules/scheduler.gd`, `port/godot/rules/ordinary_week.gd`, `port/godot/rules/school_day.gd`, `port/godot/scenes/school_day_controller.gd` and `port/godot/app/playable_boot.gd` under [[tickets/T-29014-ordinary-command-content-coverage]]. Root review retained canonical `_weighted_tokimeki`, `_closeness` and `resolve_initial_encounters` in `scheduler.gd`. Canonical Docker verification passes 238/238 checks across commands 0..3 and 5..7 under render schedules 0/1/3 while preserving exact command 0/7 state/event/RNG traces and rejecting command 4 and unsupported variants before mutation.

## [2026-10-25] build | T-29016 first-launch original-game import backend and boot router integrated

Integrated `port/godot/services/original_game_import_backend.gd`, `port/godot/app/boot_router.gd`, `port/godot/shell.gd`, `port/godot/app/first_launch_setup.gd`, `port/godot/services/content_store.gd`, and `port/godot/tests/run_import_backend_tests.gd` for [[tickets/T-29016-first-launch-original-import]] from `.claude/orchestrator/state/T-29016/import-backend-v1/`. `OriginalGameImportBackend` decodes prologue NBN cells dynamically at import time from the user's original PS1 media (directory, CUE/BIN, MODE2/2352 track, ISO, or ZIP) with zero copyrighted image payloads in `res://`, diagnoses unsupported PS1 releases per [[versions]], and stages/validates packs through `ContentStore`. `BootRouter` and `shell.gd` route directly to playable gameplay when a validated pack is installed (offline/restart safe) or to `first_launch_setup.tscn` on first launch/repair. Canonical Docker verification passed 55/55 backend/router assertions (`run_import_backend_tests.gd`), 55/55 install flow assertions (`run_install_flow_tests.gd`), and 5/5 foundation suites (`run_tests.gd`).

## [2026-10-25] build | T-29016 exported desktop .pck proof and translation README deslop

Applied `port/godot/services/installed_pack_store.gd` fix from `.claude/orchestrator/state/T-29016/exported-desktop-proof-v1/` for [[tickets/T-29016-first-launch-original-import]] so virtual `res://` in exported `.pck` builds does not falsely match `/` during storage-root overlap validation (24/24 exported `.pck` assertions + 55/55 install flow + 55/55 import backend assertions in Docker). Also deslopped `translations/en/README.md`, `translations/en/CONTRIBUTING.md`, `translations/en/INDEX.md`, and `tools/translation_tree.py` for [[tickets/T-28007-complete-text-and-translator-handoff]] (`find_tells.py` 0 hits) and separated the 617 empty binary source slots (`Empty source slot (no Japanese text).`) from the 54,786 translatable lines (100% translated).

## [2026-10-25] decision | Consolidate docs/adr/0001-phase-2-naming.md into wiki/phase2-naming.md

Moved `docs/adr/0001-phase-2-naming.md` to [[phase2-naming]] (`wiki/phase2-naming.md`), removed the redundant top-level `docs/` directory so all project knowledge base pages live under `wiki/`, and updated references in `CODING_STANDARDS.md`, `GLOSSARY.md`, `src/main/README.md`, [[index]], [[phase2]], [[phase3]], and [[status]].

## [2026-10-25] ticket | Integrate Sunday holiday HUD, continuous scene and boundary save/load app v2 (T-29013, T-29010, T-29006)

Integrated the 10-file Sunday holiday app integration v2 packet from `.claude/orchestrator/state/T-29013/app-integration-v2/` (`port/godot/app/boundary_save_controls.gd`, `port/godot/app/playable_boot.gd`, `port/godot/presentation/holiday_hud_adapter.gd`, `port/godot/services/holiday_scene_adapter.gd`, `port/godot/services/playable_save_adapter.gd`, `port/godot/tests/holiday/run_continuous_scene_tests.gd`, `port/godot/tests/holiday/run_scene_adapter_tests.gd`, `port/godot/tests/holiday/run_sunday_app_integration_tests.gd`, `port/godot/tests/hud/checkpoint_smoke.gd`, `tools/port/holiday_hud_content.py`), updating [[tickets/T-29013-native-holiday-command-loop]], [[tickets/T-29010-native-hud-font-rendering]], and [[tickets/T-29006-port-save-audio-services]]. Canonical Docker verification passes 1,135/1,135 checks in `run_sunday_app_integration_tests.gd` and 15/15 continuous scene checks across 355 steps in `run_continuous_scene_tests.gd`. Published 3 live Sunday HUD captures (`T-29013-sunday-holiday-*`) and 7 live ordinary command 1–3/5–6 HUD captures (`T-29014-command*`) to the local progress gallery.

## [2026-10-25] ticket | Integrate Command 4 club variants and content store (T-29014)

Integrated the Command 4 club activity content exporter (`tools/port/club_command_content.py`, `tools/port/test_club_command_content.py`), external pack store (`port/godot/services/club_command_content_store.gd`, `port/godot/tests/run_club_content_store.gd`), and Command 4 gameplay/media/biorhythm/practice-point rules (`port/godot/services/ordinary_command_media_adapter.gd`, `port/godot/rules/school_day.gd`, `port/godot/rules/scheduler.gd`, `port/godot/rules/ordinary_week.gd`, `port/godot/flow/command_screen.gd`, `port/godot/scenes/school_day_controller.gd`, `port/godot/app/playable_boot.gd`, `port/godot/tests/app/run_club_command_tests.gd`, `port/godot/tests/app/run_ordinary_commands_1_6_tests.gd`) from `.claude/orchestrator/state/T-29014/command4-club-v1/`, updating [[tickets/T-29014-ordinary-command-content-coverage]]. Canonical Docker verification passes 6/6 Python exporter tests, 45/45 club content store assertions, 278/278 Command 4 club integration checks, and 238/238 ordinary commands 1–6 checks.

## [2026-10-25] ticket | Integrate 22-stage prologue progression, VAB audio service and playable opening join (T-29012, T-29006)

Integrated `port/godot/flow/prologue_arrival.gd`, `port/godot/flow/new_game_command_reload.gd`, `port/godot/core/mode_router.gd`, `port/godot/scenes/school_day_controller.gd`, `port/godot/app/playable_boot.gd`, `port/godot/services/audio_service.gd`, `port/godot/services/native_vab_memory.gd`, `port/godot/tests/run_prologue_stages.gd`, `port/godot/tests/run_audio_vab_lifecycle.gd`, and `port/godot/tests/run_connected_opening_route.gd` from `.claude/orchestrator/state/T-29012/prologue-stages-v1/`, updating [[tickets/T-29012-native-new-game-initialization]] and [[tickets/T-29006-port-save-audio-services]]. Canonical Docker verification passes 1,996/1,996 prologue stage assertions, 30/30 audio VAB lifecycle assertions, and 78/78 connected opening route assertions.

## [2026-10-25] lint | Deslop user-facing repository and wiki documentation

Applied the repository-wide documentation deslop pass from `.claude/orchestrator/state/deslop-repo-v1/candidate/` across `README.md`, `ROADMAP.md`, `GLOSSARY.md`, `src/main/README.md`, `versions/README.md`, `port/godot/PORTING.md`, [[index]], [[overview]], [[status]], [[phase2]], [[phase2-naming]], [[phase3]], [[versions]], [[build-system]], [[toolchain]], and [[widescreen-settings]], eliminating all strong `find_tells.py` hits across non-catalog prose while preserving all hashes, counts, commands, addresses, and links.
