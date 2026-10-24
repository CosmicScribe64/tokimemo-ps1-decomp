---
type: log
---

# Log


Append-only. Entry format: `## [YYYY-MM-DD] <op> | <title>` where op is ingest, query, lint, ticket, build, decision, or setup. Use [[wikilinks]] for wiki files and plain paths (e.g. `src/main.c`) for everything else. Recent: `grep "^## \[" wiki/log.md | tail -5`.

## [2026-10-22] ingest | Recover Octa scratch archive

User supplied /Users/lila/Downloads/t25016-scratch-2026-10-22.tar.gz (SHA256 fd41defd56adcbd8ff2ed3dfa7969f31ad3e329d772ed9ccff5e2aab6c048d68). Extracted privately to /private/tmp/t25016-recovered-20261022 after listing members. Includes corpus/QUEUE.md, census/ledger/precedent records and compiler traces/objects; raw outputs remain uncommitted. Queue confirms already-matched entries and packet-score caveats, so membership and score require whole-TU verification. Sol T-25019 will resolve candidate provenance and TACO func_80138B2C baseline; KANGEI worker reproduced its exhausted two-row scheduler tie without repeating sweeps. See [[tickets/T-25017-completion-plan]].

## [2026-10-22] ticket | T-25018 bounded KANGEI near closure

Created [[tickets/T-25018-kangei-near-closure]] In Progress in an isolated worktree. Read [[playbook]], [[as1-scheduler-rule]], T-24901 source notes and [[tickets/T-25016-candidate-reconstruction]] before attempting new source work. Reproducing the full-unit baseline; no repeated sweeps authorized.

## [2026-10-22] query | T-25018 pins KANGEI local scheduler blocker

Reproduced two full-unit NON_MATCHING C rows and the stock as1 DAG for KANGEI func_8013852C. The existing arithmetic/$at chains give equal aftercycles 4, same source line, but besttime 16 versus 15. No new dependency-backed source lever was established; zero duplicate source variants run. Pinned the source note in src/ovl/KANGEI/80137B90.c and filed the exact revisit condition in [[matching-notes]] and [[tickets/T-25018-kangei-near-closure]]. Moved the ticket and board to In Review; no promotion.

## [2026-10-22] build | T-25018 retained fallback verification

Default funcdiff verified 180 linked bytes equal the target (assembly fallback only). After the note-only source edit, the configured KANGEI.ok Ninja target passed SHA1. Header, GameState migration, and branch prototype guards passed; no new prototype disagreement. [[tickets/T-25018-kangei-near-closure]] remains In Review.

## [2026-10-22] ticket | Recovered TACO candidate investigation

Created [[tickets/T-25019-recovered-taco-38b2c]] in isolated worktree /private/tmp/t25019-taco at 7bc6d8d99; recovered archive remains private evidence. Read prior T-16564/T-19015/T-20600 negatives before any experiments.

## [2026-10-22] query | Recovered queue provenance and TACO baseline

[[tickets/T-25019-recovered-taco-38b2c]]: 410 recovered files; 151 C body artifacts, 122 m2c bodies. TACO func_80138B2C current guarded whole-TU Docker baseline is 30 positional rows/60 printed diff lines; mechanically adapted older archive gives 103 unequal-span printed diff lines, REAL DIFFERENCE. Restored source and registered existing colour tail without speculative variants. [[matching-notes]] records result and concrete next reconstruction target main func_8005215C. Ticket and board moved to In Review for independent standards review.

## [2026-10-22] decision | Evidence-based multi-chat completion plan

Updated [[plan-remaining]] under [[tickets/T-25017-completion-plan]] after fresh per-file Docker census and completed worker pilots. Remaining119 functions/172244 bytes; origin/main remains811c35028, no open PRs, issue65 still reserves GEKO/OPTION. BUNKA pilot de816368f reduces7 to4 instruction positions; MAIN pilot4cf12bdd1 reproduces frame/homes and exposes a concrete global reload defect. Neither is a match. Plan assigns three file-disjoint Sol6.1medium lanes (MAIN, BUNKA scheduler, TEL qualification), bounded Astra escalation and serialized verification; no new chats launched during preflight. Corrected index's stale127 count. Raw archive stays private.

## [2026-10-22] ticket | T-25020 BUNKA_SD trace mapping

Created [[tickets/T-25020-bunka-sd-trace-closure]] In Progress to verify the recovered volatile trace before drawing conclusions from block 60. Reuse the isolated T-25018 worktree; own only src/ovl/BUNKA_SD/801388D0.c for new source work.

## [2026-10-22] build | T-25020 improves BUNKA_SD guarded draft

Mapped the recovered trace: 779 normalized DAG records and all 74 mover events equal a fresh full-unit trace. Block 60 supplies eight of 37 moved instructions from the later modulo switch, unrelated to the earlier year-load residue. Two directed empty-field-test experiments established a case-0x3B-only improvement from seven changed positions to four; retained in src/ovl/BUNKA_SD/801388D0.c under NON_MATCHING with FAKE/T-25020 notes. All-arm tests regress global-mover slots. See [[tickets/T-25020-bunka-sd-trace-closure]] and [[matching-notes]]. No promotion.

## [2026-10-22] build | T-25020 verified four-row guard handed to review

Updated full-unit NON_MATCHING C verifies four changed positions (eight +/- lines), only case-0x49/0x66 load/lui swaps. Default linked-byte MATCH verifies the assembly fallback only (1516 bytes); affected BUNKA_SD SHA1 and header/global/prototype guards pass. Lint has zero broken links/status mismatches and one pre-existing large page. [[tickets/T-25020-bunka-sd-trace-closure]] and board moved to In Review, with bb 41/43/45 and head bb 42/44 mapped for the next bounded consultation. No C match counted.

## [2026-10-22] ticket | Typed reconstruction main func_8005215C

Created [[tickets/T-25021-main-5215c-reconstruction]] in the isolated T-25019 worktree. Recovered candidate uses invalid void-pointer selectors; current xa_wait supplies shared-prefix buffer widths. Read T-12050 wide-compare negatives and inspected original k_disp_inc call sites before changing call views.

## [2026-10-22] query | Typed state-machine reconstruction checkpoint

[[tickets/T-25021-main-5215c-reconstruction]]: recovered func_8005215C compiles after evidence-based buffer and callee repairs. Docker full-TU final source315target/313built words, aligned138 and printed138, frame64 and observed homes exact. Preserved wide arithmetic. Earlier baseline frame96/printed325. Source-only checkpoints saved under drafts/T-25021/main/func_8005215C/; production restored. Earliest structural read-boundary gap documented in [[matching-notes]] for the requested multi-chat attack plan. Ticket/board moved to In Review; no match or coverage claim.

## [2026-10-22] decision | Dispatch three file-disjoint completion chats

Preserved reviewed T-25020 guarded source and T-25021 draft checkpoints on main883760224. Reused three idle chats for T-25022 MAIN read-boundary repair, T-25023 BUNKA four-row scheduler closure and T-25024 TEL largest-function qualification. Exact source ownership and chat IDs recorded in [[tickets/T-25017-completion-plan]]. Sol6.1medium, one worker per chat, isolated trees, no worker main pushes; root verifies integration. No new match claimed.

## [2026-10-22] build | Preserved checkpoint and dispatch verification

Root Docker affected BUNKA_SD build passes header check and executable SHA1 after checkpoint integration. Wiki lint reports zero broken links, zero status mismatches and the pre-existing large data/t0018-cases page. All three assigned chats confirmed active with current in-progress turns; MAIN has isolated883760224 worktree and starts the reload test. No full-scope C-match claim.

## [2026-10-22] decision | Bounded Astra escalation for BUNKA scheduler blocker

Live T-25023 Sol worker reports no untested causal source lever after checking the four-position residue and negative ledger. Authorized one read-only Astra consultation within that chat, limited to bb43/45 local load order versus bb42/44 filler selection, at most three trace-backed hypotheses; Sol idles during consultation and tests only justified new ideas afterward. T-25022 worker reports first real global reload repair reduces138 to127 printed diff lines and recovers one instruction; second read boundary under test, no match claim. TEL chat remains active. Overall scope unchanged.

## [2026-10-22] decision | Rotate exhausted scheduler lane to a concrete trace question

Live T-25023 consultation found no supported source lever to separate local year-load ordering from global filler selection. Preserve packet and four-row guard; assigned same Sol chat T-25025 GYOZI func_80140E0C reaching-zero-definition trace investigation, explicitly excluding prior zero-local/promotion grids. Claim transfer recorded in [[tickets/T-25017-completion-plan]]. TEL whole-unit target1645/candidate1643 baseline aligned costs1038/970 confirm substantial reconstruction remains; MAIN preserves semantic reload repairs alongside lower-score partial candidates. No match claimed.

## [2026-10-22] ticket | MAIN reload-boundary investigation

Created [[tickets/T-25022-main-reload-boundary]] In Progress in an isolated worktree pinned to883760224, following [[plan-remaining]]. Exclusive source src/main/8004F870.c; first hypothesis restores mode reload across k_disp_inc using the T-25021 full-TU checkpoint. No shared-header edits or root-checkout edits.

## [2026-10-22] build | MAIN call-boundary checkpoint

[[tickets/T-25022-main-reload-boundary]] reproduces T-25021 and repairs the first mode reload:315target/314built words,127aligned/127printed. Preserved the semantically corrected later reload body despite worse310words,179/179; partial lower-score source is diagnostic only. Exact64-byte frame and homes remain. Bounded negative tests and callee effects are in drafts/T-25022/main/func_8005215C/README.md; normal/traced full-TU objects resolve identically. No C match or production change. Prepared a bounded consultation packet for read placement and conversion-web recovery.

## [2026-10-22] lint | Reload checkpoint documentation

Docker catalog reports793 tickets,444 sections and3,078 functions. Wiki lint reports zero broken links, zero status mismatches and one existing large-page warning ([[data/t0018-cases]]). `git diff --check` passes; src/, include/ and tools/ have no changes. Research-only pin prepared for root review; ticket remains In Progress.

## [2026-10-22] ticket | T-25023 BUNKA scheduler closure

Created [[tickets/T-25023-bunka-scheduler-closure]] and its In Progress card before investigation. Isolated base 883760224; only src/ovl/BUNKA_SD/801388D0.c assigned. Read [[plan-remaining]], [[tickets/T-25020-bunka-sd-trace-closure]] and [[tickets/T-24927-func-80139290-801388D0]]. Root owns integration; no old workers resumed.

## [2026-10-22] query | T-25023 four-position baseline and bounded packet

Reproduced whole-TU BUNKA_SD guarded C: four positions / eight printed lines / aligned cost 4, 379 instructions. Fresh print-only trace matches all four relevant sections. No source variants; prior local-order levers conflict with global filler selection. Preserved compact packet at drafts/T-25023/README.md and private pre/post evidence under build/t25023-cap. [[tickets/T-25023-bunka-scheduler-closure]] remains In Progress for root-authorized read-only Astra consultation; Sol idles during it.

## [2026-10-22] decision | T-25023 consultation closes without another source sweep

Read-only Astra clarified original-INST global selection versus local scheduling, and corrected the prior li-aftercycles description. No causal source recipe beyond the rejected ledger; zero new variants. Preserved exact upstream-emission revisit condition in drafts/T-25023/README.md and [[matching-notes]]. [[tickets/T-25023-bunka-scheduler-closure]] moved to In Review with its board card. Production source unchanged; no C match or coverage, root owns integration checks.

## [2026-10-22] lint | T-25023 checkpoint

Wiki lint passes with zero broken links and status mismatches; only the existing data/t0018-cases size warning remains. `git diff --check` passes. [[tickets/T-25023-bunka-scheduler-closure]] stays In Review. No source/header/tool differences or generated/raw artifacts included in the checkpoint.

## [2026-10-22] ingest | Preserve MAIN reload source and BUNKA consultation

Root reviewed research scope and source-only checkpoints, merged T-25022 pin1b50b37d6 and T-25023 pina9608aeba. No production promotion or header/tool change. MAIN keeps both semantic reload repairs separately from lower-score partial source; direct callee mutation remains unestablished, target reload placement is established. Authorized one read-only Astra consultation on the precise eager-read/control-flow packet, with Sol idle during it and at most three subsequent justified tests. BUNKA correction: the all-arm regression hoists year-address lui t0, not store-address lui at; li t2 aftercycles is0, not2. See drafts/T-25023/README.md for upstream-emission revisit condition. Merged wiki lint: zero broken links/status mismatches, one pre-existing large page. GYOZI and TEL workers continue.

## [2026-10-22] ticket | T-25025 GYOZI reaching-definition evidence

Released the BUNKA source claim at separate T-25023 pin a9608aeba. Created [[tickets/T-25025-gyozi-zero-reaching-definition]] In Progress and board card before GYOZI work. Exclusive src/ovl/GYOZI/80140C90.c; inspect propagation before considering source. No old workers or automatic Astra.

## [2026-10-22] query | T-25025 zero carrier dies before allocation

Current and archived-v6 whole-TU measurements agree: 97:96 instructions, five printed lines, aligned cost 5. Input contains entry z and merge use; stale expression-table entries coexist with a propagated constant store, but no live z range reaches allocation. Optimized ucode recreates zero at merge; initial as1 nodes already use hardware zero. Stock/traced v6 four-section fidelity passes. No new variants, source edits or Astra. Preserved phase-boundary evidence and exact remaining internal trace question in drafts/T-25025/README.md and [[matching-notes]]. [[tickets/T-25025-gyozi-zero-reaching-definition]] moves to In Review, BUNKA claim remains released.

## [2026-10-22] lint | T-25025 checkpoint checks

Wiki lint has zero broken links and status mismatches, one existing data/t0018-cases size warning. `git diff --check` passes; no production/header/tool/config differences. [[tickets/T-25025-gyozi-zero-reaching-definition]] remains In Review for root review.

## [2026-10-22] decision | Preserve zero-propagation evidence and qualify large near candidate

Root reviewed T-25025 documentation-only scope; current and v6 both97:96 instructions, five printed lines, as1 input already hardware-zero uses. This closes allocation/scheduler tuning on that body without a new upstream mechanism. Preserving pin5c2954688. Same Sol chat now releases GYOZI and claims src/main/8006CB30.c under T-25026 solely to reconcile current4692-byte func_8006D7EC against historical one-row guard and later negative ledger. TEL remains TEL-only; no duplicate large-file ownership. MAIN reload consultation continues.

## [2026-10-22] build | MAIN bounded Astra consultation

[[tickets/T-25022-main-reload-boundary]] received one read-only Astra consultation while Sol idled. Signed diagnostic supplies target-like final reload topology but wrong load sign; conversion-web trace motivates two unsigned controls. Explicit signed-word zero tests give315target/309built words,198aligned/198printed. Word-mask mode reads give315/317,262/262 and retain lb+andi. Both rejected, source and findings preserved in drafts/T-25022/main/func_8005215C/. No third justified hypothesis, no sweeps, no production/shared-header edits or match claim.

## [2026-10-22] ticket | T-25024 isolated TEL qualification

Created [[tickets/T-25024-tel-qualification]] In Progress from exact pinned base 883760224a9ff9712c4850ca7662c5ce3df2625d after confirming ancestry under advanced main. Exclusive function is TEL func_80136724 in src/ovl/TEL/80132000.c; [[plan-remaining]] multi-chat qualification rules apply. Fresh worktree created successfully before local guidance/skills copy.

## [2026-10-22] ingest | T-25024 recovered TEL qualification baseline

[[tickets/T-25024-tel-qualification]] verified GameState offset 0x110D as u8 against target LBU/SB. Recovered m2c body still has unknown stack aggregates, invalid stack-relative rank access, scalar packed-word aliases and case-0x40 label/index artifacts; older tracked drafts already address these. Fresh Docker TEL-only object build succeeded. Complete-TU resolved T-23001 baseline reproduces 1645 target / 1643 candidate words, 1048 printed lines, 1372 changed ordinal positions, 1038 aligned cost; adapted T-20830 pointer body is 1643/984/1403/970. Exact sources/results remain in private build/T-25024 pending archive. Matched rank-helper callers justify one void ABI control, rejected at aligned cost974 versus970. No C match or source promotion.

## [2026-10-22] decision | T-25024 preserve measured TEL sources

Preserved two complete-TU measured sources, hashes and Docker replay in drafts/T-25024; [[matching-notes#T-25024 TEL largest-function qualification]] records primary changed-position metrics and structural earliest divergence. Matched rank/packed-word donors support current types, but no new semantic source repair was identified. Default TEL unit checksum passes only with unchanged fallback. No production/header/build changes, no all27 run, no coverage credit. [[tickets/T-25024-tel-qualification]] moves to In Review after standards/spec self-review.

## [2026-10-22] build | T-25024 guard prerequisites

Both archived complete-TU source replays reproduced their hashes/metrics and check_headers passed. Initial global migration guard lacked the five baseline DATE/TACO keep-proof disassemblies in this TEL-only fresh tree; generated those two immutable split inputs without compiling C, then reran the guards. This prerequisite failure is not a changed-source regression. No full27 build performed.

## [2026-10-22] lint | T-25024 final self-review and checks

[[tickets/T-25024-tel-qualification]] is In Review with matching board card; function-note backlink updated. Header/global/prototype guards pass (0 new prototype disagreements,12 known in partial-split tree; three known void-result rows absent, explicitly recorded). Standards/spec self-review has no unresolved findings; remaining semantic proof and large matching residue are limitations, not completion. Exact sources replay at hand1372/pointer1403 changed positions, both1643 vs1645 words. Production source unchanged; git diff --check passes.

## [2026-10-22] query | Review T-25024 TEL qualification handoff

[[tickets/T-25024-tel-qualification]] checkpoint e43de7fb9 reviewed against883760224: narrow source/replay/results/wiki only, no production/header/build-tool/raw-data changes. Both whole-TU bodies remain far from matching; changed-position metric and semantic/ABI limits explicit. Earliest pointer opcode divergence is table-load NOP versus moved total initialization at word326. No new archive findings or compiler rerun; In Review for root acceptance, no C coverage credit. Single worker is idle pending ownership decision.

## [2026-10-22] ticket | T-25026 high-byte MAIN near-candidate decision

Released GYOZI at pin 5c2954688 and created [[tickets/T-25026-main-d7ec-near-candidate]] In Progress with board card. Exclusive src/main/8006CB30.c, only func_8006D7EC initially; current in-tree body and historical ledger first, no source grids or automatic Astra.

## [2026-10-22] query | T-25026 confirms high-byte one-position uopt fold

Current MAIN func_8006D7EC guard remains one differing position,1173/1173 instructions: row94 addiu versus li, two printed lines/aligned cost2. Fresh uopt input/output and stock as1 initial nodes locate the fold before ugen/as1 scheduling; traced/stock four-section fidelity passes. Historical direct-counter/backedge grids and rejected nonuniform zcopy rule provide no novel repair. Preserved precise near-candidate/revisit decision in drafts/T-25026/README.md and [[matching-notes]]. No new variants/source changes or coverage. [[tickets/T-25026-main-d7ec-near-candidate]] moves to In Review.

## [2026-10-22] decision | T-25026 reuses exact prior Astra consultation

Checked root's conditional consultation request: T-24001 Astra already addresses identical idx-zero/addiu-versus-li blocker and full-TU row94. Source diff from d57c78028 contains only explanatory guard-comment changes. Fresh phase/metric evidence preserves that consult's premise, so reused its bounded negative rather than duplicate it. No new consultant or variants. [[tickets/T-25026-main-d7ec-near-candidate]] remains In Review. Wiki lint zero broken links/status mismatches, one existing large-page warning; diff check passes and source/header/tools/config unchanged.

## [2026-10-22] decision | Preserve large near blocker and select new reconstruction evidence

Reviewed T-25026 research-only pin1cce82f2e:4692-byte function has exactly one changed position; prior identical Astra consultation reused, no duplicated tests. Current body unchanged. Sol lane releases8006CB30 and takes T-25028 bounded read-only triage of at most12 unclaimed remaining candidates for still-unrepaired semantic/control/ABI defects with donors, excluding active8004F870 and contributor GEKO/OPTION. MAIN worker continues T-25027 sndi; TEL worker idle. Keep two Sol workers and no active Astra until a specific new question warrants it.

## [2026-10-22] ticket | MAIN sndi reconstruction

Created [[tickets/T-25027-main-sndi]] In Progress in the existing isolated worktree. Exclusive file src/main/8004F870.c remains unchanged. Read prior sndi negative ledger before experiments; compare current tracked and recovered source with whole-TU evidence rather than archived canonical scores. No extra workers or Astra consultation.

## [2026-10-22] build | sndi target entry-control recovery

[[tickets/T-25027-main-sndi]] reproduces tracked full-TU baseline263target/265built words,150aligned/150printed. Audited recovered buffer widths, call ABI and found sequencing. Explicit target stage[0] guard restores G-loop indexed cursor structure and improves to264words,123/123; target-timed join-reset alternate264/125/127 preserved. Word parity controls regress. Fresh traced object equals normal object exactly. Source/audit at drafts/T-25027/main/sndi/; private target traces stay ignored. No match or production/shared-header/tool change. Remaining base/constant and parity/cursor priority conflict is the known frozen blocker.

## [2026-10-22] lint | sndi checkpoint validation

Docker wiki catalog reports794 tickets,444 sections and3,078 functions. Lint reports zero broken links, zero status mismatches and one existing large-page warning ([[data/t0018-cases]]). `git diff --check` passes; src/, include/ and tools/ have no changes. T-25027 remains In Progress; source-only checkpoint is prepared for root review.

## [2026-10-22] ingest | Preserve reviewed sndi entry-control repair

Root reviewed T-25027 source-only checkpoint624541ad3 against prior source/audit: second scan now tests stage[0] at entry and indexed byte at latch; target-timed reset alternate also preserved. Default production unchanged. Measured264built versus263target words/123printedlines is an improvement, not a match. Remaining known allocation/spill conflicts are explicit. MAIN worker idle pending T-25028 candidate screening; no automatic consultation or repeated sweep. Full matching goal stays active.

## [2026-10-22] ticket | T-25028 read-only candidate prioritization

Released src/main/8006CB30.c at T-25026 pin 1cce82f2e; created [[tickets/T-25028-read-only-candidate-triage]] and In Progress card. At most12 promising remaining functions, excluding reserved GEKO/OPTION and active src/main/8004F870.c; no builds/grids/source changes or agents. Root selects next implementation.

## [2026-10-22] query | T-25028 twelve-function triage without another sweep

Read-only Docker progress confirms119 remaining at main f688ec594; checked membership of12 reviewed functions. No new evidenced semantic/control/ABI/width defect qualified after newest source/ledger reconciliation. Historical matched entries and already repaired raw-draft errors excluded. Preserved concise table and a bounded strCallback upstream-expression question in drafts/T-25028/README.md and [[matching-notes]]. No compiles, source grids/root edits or agents. [[tickets/T-25028-read-only-candidate-triage]] moves to In Review; this is not an all119-blocked claim.

## [2026-10-22] lint | T-25028 triage checkpoint

Wiki lint zero broken links/status mismatches, only existing data/t0018-cases size warning. Diff check clean, production/header/tools/config unchanged; body pointers exist. [[tickets/T-25028-read-only-candidate-triage]] remains In Review.

## [2026-10-22] decision | Trace callback temporary and broaden scoped triage

Root reviewed and preserved T-25028 twelve-function audit; no new repair found in those12, no inference about all119. Assigned former MAIN worker T-25029, exclusive src/main/800563F0.c strCallback, to locate extra RECT-copy temporary with faithful phase evidence before source tests. Released8004F870. Same triage worker takes T-25030, up to12 different older high-byte overlay candidates, stop early on concrete unrepaired defect with donor/target evidence. Exclude contributorGEKO/OPTION, qualifiedTEL36724 and prior12. TwoSolmedium workers, no activeAstra; root integration remains serialized.

## [2026-10-22] ticket | T-25030 overlay reconstruction qualification

Created [[tickets/T-25030-overlay-candidate-triage]] and In Progress card before second read-only triage. At most12 different actual remaining overlay functions; stop on a concrete unrepaired semantic/control/ABI defect. No builds/source grids/root writes/agents; strCallback remains MAIN T-25029, TEL36724 and contributor GEKO/OPTION excluded.

## [2026-10-22] query | T-25030 corrected overlay bodies supersede raw defects

Reviewed12 different actual remaining overlay functions; current119 scope confirmed read-only. No strong new semantic/control/ABI repair qualified. Old raw field/dispatcher/callee/array/actor errors already repaired; preserved corrected-body pointers and ledgers in drafts/T-25030/README.md. Eight bounded target/source direct-call multisets agree after renames, without full semantics claim. No compiles/source variants/root changes/agents. [[tickets/T-25030-overlay-candidate-triage]] moves to In Review; other107 not individually audited.

## [2026-10-22] lint | T-25030 checkpoint checks

Wiki lint zero broken links/status mismatches with only existing data/t0018-cases size warning; diff check clean and production/header/tools/config unchanged. [[tickets/T-25030-overlay-candidate-triage]] remains In Review.

## [2026-10-22] ingest | Preserve second scoped triage

Root reviewed documentation-only T-25030 pin708f1c4f6 and preserved corrected starting-body pointers. Combined T-25028/T-25030 cover24 distinct remaining functions, not all119; no qualified new semantic repair. T-25029 callback trace remains live and is the current implementation lead; triage worker idle instead of repeating near-tail sweeps. Latest contributor check: no open PRs, issue65 unchanged. No coverage increase.

## [2026-10-22] decision | Consult newly measured callback backend state

Live T-25029 reproduces160-word strCallback with75 register-only positions. Existing logging-only diagnostic identifies eval_mov reused stack slot0x10000 and two-dummy path, candidate last writer f_loadstore; normal/logged whole-TU bytes agree. Bypassing existing copy-state shim is neutral, as are address-offset/alignment controls. Exact earlier strCallback consult covered sentinel, not this measured last-writer path. Authorized one read-only Astra consultation on required predecessor/control-flow ordering while preserving reloads/stores/ABI/160words; Sol idles and tests only up to3 new trace-backed hypotheses. No backend-slot patch, input-specific rule or broad source grid authorized. This is a new phase diagnosis, not a match.

## [2026-10-22] ticket | strCallback upstream temporary diagnosis

Created [[tickets/T-25029-main-strcallback-ring]] In Progress in isolated worktree; release src/main/8004F870.c and exclusively own src/main/800563F0.c. Read T-25001/10 negatives before compiling. Trace actual RECT copy inputs/outputs before at most three new causal source tests. No generic grids, shared rules, additional worker or Astra.

## [2026-10-22] build | strCallback small-copy reservation identified

[[tickets/T-25029-main-strcallback-ring]] reproduces160target/160built words,148aligned/150printed,75 register-only instructions. Captured actual8-byte Umov and normal ugen output; shim/raw callback binasm agree. Private offset/alignment/length controls isolate fixed reservation, not source address or second-word count. Existing T-24067 logging-only tool, audited whole-object identical, confirms reused slot+232=10000 selects double dummy t0/t1 before actual second copy wordt2; candidate writer f_loadstore saved gp. Target is consistent with one dummy. No faithful new source mechanism established, so no source controls or build changes. Evidence/limitations in drafts/T-25029/README.md; raw reference/traces stay private. No match or coverage claim.

## [2026-10-22] decision | strCallback bounded branch-route consultation

For [[tickets/T-25029-main-strcallback-ring]], checked prior exact callback Astra history: sentinel bypass only, not the new slot path. One authorized read-only Astra follow-up, Sol idle, found required predecessor branches occur before later scalar operations and next required branch after Umov; f_jump's predicate/emitter order offers no justified source-required pre-copy route. Zero source hypotheses recommended or tested. Preserved exact record ordering and limitations in drafts/T-25029/README.md; no universal source-impossibility or compiler-exception claim.

## [2026-10-22] lint | strCallback mechanism checkpoint

Docker catalog795 tickets,444 sections,3,078 functions; wiki lint zero broken links/zero status mismatches and one existing large-page warning ([[data/t0018-cases]]). `git diff --check` passes. src/, include/ and tools/ unchanged. T-25029 research/consultation preserved for root review; no match or coverage change.

## [2026-10-22] ingest | Preserve callback mechanism and continue unaudited groups

Root reviewed T-25029 pin37bf807db: faithful phase observations and limited required-branch consultation, no production code or promotion. Preserve candidate last-writer caveat and no-universal-impossibility limit. Assigned twoSolmedium read-only passes: T-25031 small overlays and T-25032 otherMAIN, at most12 new candidates each, disjoint from prior24 and contributor claims; stop early on actionable defect. Source800563F0 released after checkpoint, no active source-edit claims. No activeAstra. Objective remains all119 remaining game functions, not merely draft coverage.

## [2026-10-22] decision | Two concrete source experiments from new screening

Live T-25031 found SHOUGATU8013E50C archived FAKE table-address arguments to renamed matched zero-argument check_para_limit; incoming argument registers unused. Assigned same Sol T-25033 canonical no-argument whole-TU control in src/ovl/SHOUGATU/8013E330.c after ledger check. Ignored arguments may still be original caller behavior, so no match/defect certainty inferred. T-25032 found newest MAIN80046AC8 draft still narrows tile extent to s16 before positivity test whereas target tests full width; assigned same Sol T-25034 correction and whole-TU comparison in src/main/80046500.c. Workers first pin read-only evidence then create implementation tickets. Two disjoint source owners, no activeAstra, no further screening until these controls resolve.

## [2026-10-22] decision | Hands-on escalation for near SHOUGATU closure

Live T-25033 corrected guard reaches five printed lines: missing entry zero carrier and its two uses, other instructions aligned. Three-argument status-call control restores count but emits zero too late and disturbs reload base; rejected. Authorized one hands-on Astra worker on this exact claimed source after Sol pins trace/bestbody, with Sol idle and exclusive editing transferred. Must consult T-24907/T-25025 and current negatives, avoid repeated grids and semantic/byte shortcuts; initial30min sustained attempt with literal proof for any match. This supersedes read-only consultation restriction for this specific blocker. T-25034 MAIN continues source lifetimes/frame/control work. No coverage claim yet.

## [2026-10-22] decision | User switches active implementation to Astra

Human explicitly states Sol is insufficient and requests Astra. Sent both active matching chats model override gpt-6-astra/high: T-25033 SHOUGATU five-line closure and T-25034 MAIN full-width extent reconstruction. This supersedes earlier Sol-default/read-only-Astra policy for active implementation. Continue exact best sources and negative ledgers; a running Astra child remains sole editor if already dispatched, avoiding duplicate work. Root integration/proof requirements unchanged, no new fan-out or screening. No coverage increase claimed.

## [2026-10-22] decision | User requires continued work through resolution

Human directs active workers not to stop until their assigned problems are resolved. Relayed to both Astra matching chats: supersede prior time boxes/probe-count stopping limits; checkpoints and negative experiments do not terminate work. Continue evidence-informed implementation and verification on current files, preserving interim sources. Required literal match/semantics/proof standards remain; ordinary difficulty is not an external blocker. Root integration remains serialized.

## [2026-10-22] ticket | T-25031 small-overlay candidate qualification

Created [[tickets/T-25031-small-overlay-candidate-triage]] and In Progress card before work. Read-only NAME_ENT/SHOUGATU/VALEN/KANGEI/GYOZI/BUNKA_SD selection excludes prior24, reserved GEKO/OPTION/TEL and exhausted tails; MAIN separately owned. At most12 functions, no builds/source variants/root edits/agents.

## [2026-10-22] query | T-25031 nominates proven zero-argument helper model

Stopped after four reviewed actual remaining functions, current119. Matched check_para_limit at80084D3C overwrites incoming a1/a0 before reads; corrected SHOUGATU E50C archive still passes &sp40 and a record address as FAKE arguments. Verified callee resolves earlier open escape question. Preserved exact target/canonical source evidence and one minimal canonical no-arg proving variant in drafts/T-25031/README.md and [[matching-notes]]. No build/source trials or runtime-bug claim; root assigns implementation. [[tickets/T-25031-small-overlay-candidate-triage]] moves to In Review.

## [2026-10-22] lint | T-25031 actionable checkpoint

Wiki lint zero broken links/status mismatches, one existing data/t0018-cases size warning. Diff check clean and production/header/tools/config unchanged. [[tickets/T-25031-small-overlay-candidate-triage]] remains In Review; root authorized separate T-25033 proving experiment after pin.

## [2026-10-22] ticket | T-25033 canonical helper call control

After separate T-25031 pin e211b9bb5, created [[tickets/T-25033-shougatu-canonical-call]] and In Progress card. Root authorized one current whole-TU no-argument check_para_limit control for corrected SHOUGATU E50C. Exclusive src/ovl/SHOUGATU/8013E330.c, no shared changes or grids; check prior exact variant first.

## [2026-10-22] build | T-25033 canonical call and source-layout reconciliation

No exact no-arg call among37 tracked prior narrow bodies. Canonical control improves archived152to99 aligned cost. Newer in-tree i/girl declarations omitted by the archived body restore exact40/30 table homes and frame58; corrected random field plus canonical call gives93cost,117/118words, first28 exact. [[tickets/T-25033-shougatu-canonical-call]] continues sustained matching in private full-TU candidates; production unchanged.

## [2026-10-22] build | T-25033 matched-family caller transfer closes broad drift

Retaining corrected in-tree local layout, the GYOZI relative's integer-return view for func_800847B8 reduces93to87 aligned cost; explicit u8 narrowing of func_80051A68 result reduces87to9,117words versus118target with exact frame58/table40/30. All remaining differences are entry zero-carrier/store/call and one reload-base register. Continuing the concrete byte-argument transfer in [[tickets/T-25033-shougatu-canonical-call]], no source grid/shared header changes.

## [2026-10-22] build | T-25033 five-line guard and faithful trace pinned for Astra

Updated only owned SHOUGATU E50C source: correct random field, canonical final helper, known relative's marked return/narrowing/byte-view controls. Actual guarded C5cost,117/118words, exact frame/homes; one three-arg status control rejected10cost. Default SHOUGATU checksum and declaration guards pass; no C match counted. Stock/trace four-section fidelity passes. [[tickets/T-25033-shougatu-canonical-call]] remains In Progress for explicitly authorized sole hands-on Astra implementation; coordinator idles after pin. Compact ledger at drafts/T-25033/README.md.

## [2026-10-22] ingest | Preserve corrected SHOUGATU guard while Astra continues

Root reviewed pin7a9ed5a81 and ancestor T-25031 evidence against CODING_STANDARDS: marked caller views, corrected random field, canonical helper, intact NON_MATCHING fallback. Merged append-only board/log conflicts preserving both histories. Docker rebuilt SHOUGATU and verified its checksum plus headers; wiki lint reports zero broken links/status mismatches, with size warnings for log and data/t0018-cases. This verifies fallback safety, not a C match. Both Astra chats confirmed active; updated [[plan-remaining]] to supersede historical time limits and ownership. Contributor watcher shows no PR changes and issue65 remains the existing GEKO/OPTION claim. Coverage unchanged; T-25033 remains In Progress.

## [2026-10-22] ticket | Read-only MAIN qualification

Created [[tickets/T-25032-main-candidate-audit]] In Progress; release800563F0.c editing. Read latest main b6d7754bd and prior T-25028/30 tables before selecting from current membership. Bound12MAIN functions/15minutes, no builds/source sweeps/subagents/Astra. Preserve37bf807db separately; root handles integration.

## [2026-10-22] query | MAIN remaining extent-width candidate

[[tickets/T-25032-main-candidate-audit]] stops early after five focused remaining candidates and two raw-prefix provenance checks. Latest main44a495b88 confirms membership. Newest tracked func_80046AC8 still narrows tile extent to s16 before positive test; target branches on full word and narrows only at TILE store. Known T-24501 suspect now confirmed as still-unrepaired source/control gap, not a new archive discovery. Current callers and exact one-type baseline experiment recorded in drafts/T-25032/README.md; reuse any existing private corrected variant first. No builds, source experiments, workers/Astra, production/root edits or coverage change.

## [2026-10-22] lint | MAIN read-only audit checkpoint

Metadata-only Docker catalog/lint pass:796 tickets,444 sections,3,078 functions; zero broken links, zero status mismatches, one existing large-page warning ([[data/t0018-cases]]). `git diff --check` passes and src/, include/, tools/ unchanged. No compiler/build/source trial occurred. Candidate qualification is ready for root review; no match or coverage credit.

## [2026-10-22] ingest | Preserve MAIN extent-width evidence

Root reviewed and integrated T-25032 pin62948769f, documenting the full-word branch versus s16 draft narrowing in func_80046AC8. Documentation only; implementation continues independently under T-25034 with Astra. No match credited. Preserved both log histories and regenerated the wiki catalog to include the integrated tickets.

## [2026-10-22] ticket | Full-width quadrant extent implementation

Created [[tickets/T-25034-main-quadrant-extent]] In Progress after pinning [[tickets/T-25032-main-candidate-audit]] at62948769f. Exclusive source src/main/80046500.c; first repair retains full-word tile branch values and narrows only at TILE store. Preserve baseline and semantic checkpoint even if score regresses. One worker, bounded investigation, root owns integration; no shared-header/build changes.

## [2026-10-22] build | Quadrant full-word extent correction

[[tickets/T-25034-main-quadrant-extent]] reproduces674target/680built words,738aligned/796printed. Single extent-width repair improves to671words,613/649 and matches all four signed full-word branch widths, narrowing only to TILE fields. Continued source/lifetime reconstruction: shared primitive role reduces frameB8->B0; mask/callback lifetimes and target UV-tail sharing remain under investigation. No promotion or coverage claim; sustained matching continues.

## [2026-10-22] decision | Astra owns sustained quadrant matching

Human requests Astra hands-on and no stopping before resolution. [[tickets/T-25034-main-quadrant-extent]] transfers sole source writer to Astra; Sol coordinates without duplicate experiments. Prior time/probe caps superseded. Corrected source improves to674target/671built words,367aligned/389printed with frameB0 and full-word extent tests. Saved source and ledger before handoff; matching continues through remaining CFG/lifetime/frame differences. No production match or coverage claim yet.

## [2026-10-22] build | Quadrant target frame recovered

Astra re-audited the newest367-cost source in [[tickets/T-25034-main-quadrant-extent]]: direct-low-channel already emits target0xA8 frame and incoming argument homesB8/BC/C0/C4. Earlier handoff incorrectly carried forward0xB0 from its predecessor. Corrected ticket without changing old log history. Remaining local homes/color-expression webs continue under sole Astra ownership.

## [2026-10-22] decision | User-directed quadrant wrap-up

User requested wrap-up, commit and push, superseding sustained matching for [[tickets/T-25034-main-quadrant-extent]]. Astra stopped all experiments. Archived conservative361/381 and forward-join360/380 candidates, full-word extent correction, target0xA8 frame, findings and Docker replay under drafts/T-25034. Target674words includes12bytes trailing padding after2684byte active body; candidates671/670paddedwords remain12/16bytes short. No adjacent function, production change, match or coverage credit. Root owns main integration.

## [2026-10-22] lint | Quadrant wrap-up replay verification

Docker replay independently reproduced conservative674/671words,361aligned/381printed and forward-join674/670words,360/380 for [[tickets/T-25034-main-quadrant-extent]]. Docker catalog797tickets; lint zero broken links and zero status mismatches, with one existing large-page warning for [[data/t0018-cases]]. git diff --check passes. src/, include/ and tools/ unchanged. Source-only checkpoint remains unfinished research; all worker experiments stopped at user request.

## [2026-10-22] ingest | Preserve MAIN wrap-up checkpoints

Integrated d8e482c4a after source/replay review. Root Docker replay confirms conservative674/671words with361aligned/381printed and forward-join674/670 with360/380. Source drafts only, production unchanged, no match credited. Wiki catalog/lint pass with zero broken links/status mismatches and two size warnings. MAIN chat reports newer direct user wrap-up/push instructions; root withdrew its resume request based on older context and preserves that stopped lane. Updated [[plan-remaining]]. SHOUGATU remains active with reported118/118-word zero-carrier experiment and four aligned differences; not yet a match.

## [2026-10-22] decision | User resumes sustained matching after MAIN wrap-up

New direct root instruction: keep workers going until problems are figured out and matches made. This supersedes the earlier MAIN-chat wrap-up. Resumed T-25034 Astra from preserved d8e482c4a candidates and reinforced T-25033 continuation from the118-word zero-carrier experiment. No time/probe limits; checkpoints and failed controls do not terminate assignments. Concrete external blockers and literal proof standards remain. Root integrates, coordinators avoid duplicate experiments.

## [2026-10-22] decision | Additional Astra matching lanes

User explicitly requests more agents across unmatched functions and independent solutions. Dispatched three file-disjoint Astra/high workers: T-25035 GYOZI func_80140E0C, T-25036 MAIN func_8006D7EC, T-25037 BUNKA_SD func_80139290. Each uses isolated checkout, Docker, existing negative ledger, and sustained implementation until literal match or concrete external blocker. GYOZI/counter workers will independently assess the new SHOUGATU late-folded producer, not duplicate its source editing. BUNKA worker pursues remaining source-to-scheduler ordering. Existing T-25033/T-25034 continue; GEKO/OPTION reserved. Root serializes proof and integration. No new coverage claimed.

## [2026-10-22] query | Share concrete late-folded producer across independent lanes

T-25033 supplied exact private donor single-identity-or/unit.c and unsigned-less-than-zero producer, preserving target entry zero carrier without an input load. Recorded provisional mechanism and FAKE/verification limits in [[matching-notes]], relayed to T-25035/T-25036. These workers test transfer independently in separate files; no match or coverage claim. T-25034 continues color-result placement, T-25037 scheduler ordering.

## [2026-10-22] decision | T-25033 continuous sole-worker attempt

Latest human direction removes historical time/probe stopping limits for [[tickets/T-25033-shougatu-canonical-call]]. Sole implementation ownership transferred to the requested Astra worker; coordinator idles. Preserve the five-line guarded candidate while testing evidence-informed zero-carrier mechanisms through literal resolution or a concrete external blocker. Updated drafts/T-25033/README.md; no source changes.

## [2026-10-22] build | T-25033 zero representation controls

Private full-TU controls in build/t25033/astra retain the five-line baseline for direct float-to-integer zero and null-pointer zero. A live float local adds conversion code; mixed-width conditional writes and register-volatile locals force unwanted stack traffic. Union punning encounters the existing frame-pass unsupported-stack-form guard. None is retained. Next hypothesis is a width-limited extraction with a disjoint mask, recognized after uopt so the target zero carrier can remain live. See [[tickets/T-25033-shougatu-canonical-call]].

## [2026-10-22] build | T-25033 late-folded zero producer found

Disjoint masks survive uopt but remain explicit ANDI-zero instructions; they do not give the target move. Unsigned comparison with zero, `(u32)flag_word < 0`, does produce an entry register-zero move and retains its uses, reaching the target 118-word length. It currently promotes the shared flag load into v1, shifts temporary registers and allocates the carrier in v0 (112 aligned cost), so it is diagnostic evidence, not a candidate improvement. Next test feeds an already-available table address into that comparison, avoiding a promoted flag range. Private data remains in build/t25033/astra. [[tickets/T-25033-shougatu-canonical-call]] remains In Progress.

## [2026-10-22] build | T-25033 single-use late-zero carrier

Single-use unsigned comparisons eliminate their input evaluation and preserve an entry zero move: 118 words, 12 printed / 10 aligned, post-call code exact. Direct call selects a0; byte-view plus third argument selects a2 but leaves byte masks and changes one later base. Pointer and signedness forms did not resolve both constraints. [[tickets/T-25033-shougatu-canonical-call]] records this concrete new mechanism; five-line source guard remains preserved.

## [2026-10-22] build | T-25033 target zero carrier reproduced

A single-use unsigned-negative comparison plus an identity first argument and explicit ignored table/third arguments reproduces the target entry a2 zero, sb a2, and move a0,a2. `single-identity-or` is 118/118 words, six printed / four aligned; only the later F5F reload base and its scheduling differ. An eliminated extra byte use repairs scheduling but gives v1 rather than a2 (four printed/four aligned). Trace confirms the late carrier R6 conflicts with a distinct narrowed global-byte range R3. Early promoted global initialization also works and defers the store to the target merge, but its entire byte range chooses v1. Continuing allocation and source minimization; no production change or C coverage claimed. [[tickets/T-25033-shougatu-canonical-call]].

## [2026-10-22] build | T-25033 literal SHOUGATU E50C match

Early selected-girl initialization plus the exact unsigned-byte status argument closes the function: full-TU 118/118, zero resolved differences. The diagnostic unsigned comparison minimized to `girl = D_800E6280.unk_F5F * 0; D_800E6280.unk_F5F = girl;` before the conditional. Private real linking proves all 472 function bytes equal the target. Literal zero regresses to 117 words/15 rows; removing result narrowing gives 78 aligned cost, and removing the integer847B8 view gives 117 words/25 rows. Retained meaningful girl local; deleting it shifts table homes. Promoted C in src/ovl/SHOUGATU/8013E330.c, removed the fallback, and marked the zero-carrier FAKE. Default-C checksum/declaration validation follows. [[tickets/T-25033-shougatu-canonical-call]].

## [2026-10-22] ticket | T-25033 verified C match enters review

Production default-C funcdiff confirms all 472 linked bytes; affected SHOUGATU checksum passes after compiling src/ovl/SHOUGATU/8013E330.c. Header/global guards pass; prototype branch check has 0 new / 15 known disagreements. Source review found no extra effects or unsafe arithmetic in the marked multiply-by-zero carrier. Updated [[playbook]], [[families/shougatu-family-results]] and drafts/T-25033/README.md; moved [[tickets/T-25033-shougatu-canonical-call]] and its board card to In Review. Coordinator owns formal review and full 27-binary integration.

## [2026-10-22] lint | T-25033 reviewed match checkpoint

Coordinator source review of [[tickets/T-25033-shougatu-canonical-call]] found no issues. Independent default-preprocessed full-TU proof explicitly excludes the E50C assembly fallback and confirms 118 words / 472 literal bytes. Wiki lint reports zero broken links/status mismatches and the existing data/t0018-cases size warning; git diff check passes. Ticket remains In Review for the coordinator's combined integration gate.

## [2026-10-22] ticket | T-25035 GYOZI late-folded zero carrier

Created [[tickets/T-25035-gyozi-late-folded-zero]] in an isolated checkout from 242c0c3a3. Read existing T-24907/T-25025 evidence and claimed only src/ovl/GYOZI/80140C90.c. New lead is the concurrent SHOUGATU late-folded carrier; old zero-local grids remain exhausted.

## [2026-10-22] build | T-25035 donor transfer preserves entry zero

Docker full-TU controls for [[tickets/T-25035-gyozi-late-folded-zero]] transfer the new SHOUGATU late-folded unsigned comparison: target and candidate now both have 97 words with exact zero initialization/store/first call, versus the old 97/96. Remaining aligned cost 4 is the second-call reload base and its schedule. Focused coalescing controls are negative; inspecting compiler allocation next. No production edits or coverage.

## [2026-10-22] build | T-25035 GYOZI literal C match

Transferred the improved T-25033 early-global-assignment form into GYOZI func_80140E0C. A marked late-folded multiply-by-zero expression preserves the entry a1 carrier; the early store lets allocation retain the later a1 reload base. Default C links to all 388 target bytes and GYOZI SHA1 passes. Headers/global/prototype guards pass after missing reference splits were generated. Removed the assembly fallback, documented controls in drafts/T-25035/README.md and [[matching-notes]], and moved [[tickets/T-25035-gyozi-late-folded-zero]] to In Review for root review/integration.

## [2026-10-22] query | T-25035 independent source review

Root reviewed the GYOZI change against standards and scope with no findings: defined zero expression, disjoint early-store move, marked FAKEs and no build exceptions. [[tickets/T-25035-gyozi-late-folded-zero]] stays In Review until the combined integration gate.

## [2026-10-22] build | Two late-zero family matches verified on main

Root combined T-25033/T-25035, reviewed source against standards/spec with no findings, and refreshed every binary checksum:27/27 pass. Default-C literal linked proofs pass SHOUGATU472bytes and GYOZI388bytes; neither function has an assembly fallback. Headers/globals pass, prototypes0new/15known. Docker progress6835/6952functions,2105536/2276920bytes (92.5%);117functions/171384bytes remain. Moved both reviewed tickets/cards to Done, updated [[status]] and regenerated catalog. Research workers continue MAIN counter/quadrant and BUNKA scheduling. No experimental follow-up counted.

## [2026-10-22] decision | Continue successful workers on new functions

After verified T-25033/T-25035 integration, reassigned their Astra capacity without duplicating active work. SHOUGATU chat reserves T-25038 for the next remaining function in8013E330.c, selecting E6E4 or EA98 by existing candidate evidence. Former GYOZI worker reserves T-25039 MAIN func_8005B43C in8005A0B0.c, releasing GYOZI. Both sustain implementation through literal proof; MAIN quadrant/counter and BUNKA scheduler workers continue. Root owns integration.

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

## [2026-10-24] build | T-20500 emulator probe merged

[[emulator]]: PCSX-Redux (pinned 8288e927 plus upstream fix 3a24bbcc) with OpenBIOS (MIT) in Docker; the game boots headless to the title menu. tools/emu/run.sh drives breakpoints on main and overlay code, memory watches, call stacks, traces, scripted input, screenshots and save states for phase 2 naming. Ticket stays In Progress for its GameState naming pilot.

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

## [2026-10-24] ingest | Phase 2 analysis of overlay TT (T-27024)
Analysed the TT overlay (TwinBee Time Attack, 303 functions) for the port: entry `tt_main`, nine core states, 22 work blocks at 0x80110000, rules for the clock and scroll speed, player, shots, bombs, bells, clouds, waves, ground targets, bosses, ranking and the two secret codes. Wrote `config/renames/proposals/o-tt.tsv` (949 rows, 21 hypotheses) and [[spec/o-tt]], linked from [[spec/index]]. Ticket [[tickets/T-27024-p2-o-tt]] is In Review. The emulator confirmed the title, stage 1, its boss, the ranking screen and the state sequence 1, 2, 5, 7; direct stage mode plays recorded input. No src/, include/ or symbol file changed.

## [2026-10-24] ticket | T-27024 review round 1 fixes
Fixed the blocking review findings of [[tickets/T-27024-p2-o-tt]]: bell kind 1 rule and the shot level 10 downgrade quirk in [[spec/o-tt]] (6.4), HYPOTHESIS marks on all bell names, the header file row, TtCore and warp field rows, shared record byte rows; also return fire by stage (6.9), `sys.use_pad2` bit 5, the boot sector list (7.2). `config/renames/proposals/o-tt.tsv` now has 994 rows (38 hypotheses). Ticket stays In Review for the next review pass. No src/, include/ or symbol file changes.

## [2026-10-24] ticket | T-27024 review round 2 fixes
Fixed the blocking finding of [[tickets/T-27024-p2-o-tt]]: a bell in state 0x70 is a falling hazard (it hurts a ship that is not invulnerable, a shot frees it), documented in [[spec/o-tt]] 6.4 and 6.9 and in the `TtBell.state`, `tt_bell_hit_by_shot`, `tt_bell_collect` and `tt_collisions_update` notes of `config/renames/proposals/o-tt.tsv`. Also: boss 4 beam offset tables (entry counts, new row for `D_80155F14`), bell kind table size, dangling type names, HYPOTHESIS marks on descriptive bell, option and arm swing wording, naming consistency (shot level consts, settings menu vs follower options, boss forms, hit box slots), and core table names in spec sections 2 and 8. The table has 995 rows (42 start with HYPOTHESIS). Ticket stays In Review. No src/, include/ or symbol file changes.

## [2026-10-24] ticket | T-27024 Done
[[tickets/T-27024-p2-o-tt]] passed the third review (approved, no findings): the bell hazard rule (state 0x70 falls, hurts a ship that is not invulnerable, a shot frees it) in [[spec/o-tt]] 6.4 and 6.9 and the boss 4 beam table rows in `config/renames/proposals/o-tt.tsv` match `src/ovl/TT/8013BD10.c`, `80142D40.c`, `80144D70.c` and the asm data. The branch changes no `src/`, `include/` or symbol files. Moves to Done.
