---
type: coordination
updated: 2026-10-17
---

# Current 100% matching run

User resumed work toward 100% verified game-code matching. Current published code checkpoint verified DATE T24051 integration (worker2da9bb5f8) has **6,510/6,958 functions and 1,861,692/2,279,368 bytes (81.7%)**, measured in Docker. Since baseline `283b86fca`: 44 functions / 48,224 bytes added. The active goal remains 100%, with 417,676 game-code bytes remaining. SDK assembly is separately reported and excluded.

## Verified gains

| Ticket | Function | Literal bytes |
|---|---|---:|
| T24003 | BUNKAKEN func_80140F60 |296|
| T24003 | BUNKAKEN func_8013BD84 |540|
| T24002 | OPTION func_80139198 |860|
| T24012 | ETC func_80136448 |1280|
| T24004 | SHOUGATU func_801430A0 |352|
| T24007 | GEKO func_80133598 |452|
| T24005 | BUNKAKEN func_80143DD0 |3232|
| T24008 | TAIIKU func_80137EA8 |692|
| T24013 | RPG_BAT func_80139510 |2424|
| T24016 | BUNKASAI func_80135C4C |1508|
| T24016 | BUNKASAI func_80158764 |1652|
| T24017 | GEKO func_8013FD88 |476|
| T24017 | SHUGAKU func_80133038 |176|
| T24017 | EVENT func_80108B24 |476|
| T24019 | GYOZI func_80142A8C |392|
| T24030 | BUNKASAI func_80142700 |1176|
| T24030 | BUNKASAI func_80151364 |844|
| T24027 | RPG_BAT func_80154760 |1656|
| T24026 | SHOUGATU func_8013F224 |316|
| T24026 | SHOUGATU func_80140510 |392|
| T24035 | GEKO func_80134228 |244|
| T24035 | BUNKA_SD func_801344A8 |152|
| T24034 | EVENT func_800FEF74 |940|
| T24034 | EVENT func_800FFA4C |404|
| T24034 | EVENT func_800F9730 |464|
| T24036 | SHUGAKU func_80134C98 |244|
| T24037 | EVENT func_80103774 |628|
| T24021 | MAIN dialogue_speak_part |8672|
| T24041 | MAIN func_80049FF0 |348|
| T24040 | OPTION func_801342A8 |468|
| T24040 | OPTION func_8013493C |560|
| T24038 | ETC func_8013FC64 |796|
| T24038 | MASTER func_8013B448 |956|
| T24045 | TEL func_8013D03C |2128|
| T24052 | SHOUGATU func_80133208 |488|
| T24052 | SHOUGATU func_801333F0 |544|
| T24053 | EVENT func_8010B614 |100|
| T24053 | BUNKA_SD func_801355AC |132|
| T24059 | GYOZI func_80137F48 |164|
| T24059 | SHOUGATU func_80136948 |164|
| T24058 | GYOZI func_80137188 |176|
| T24023 | EVENT func_801103A0 |6848|
| T24062 | MAIN func_8007BCFC |236|
| T24051 | DATE func_80144E20 |3200|

Each was independently checked against default linked function bytes, all27 binary hashes, headers, global views and prototype guards before main publication. OPTION argument evaluation and ETC return liveness have explicit FAKE explanations. A default ASM fallback MATCH is not new C progress.

## Active ownership

User clarified on2026-10-18: keep the100% goal active, limit matching to1–2Sol6.1medium workers, each persistent on one function; avoid batch churn and conserve usage. No routine Astra. User subsequently authorized two additional Antigravity Gemini4Argon Medium workers using separate account usage (see below). Root handles independent integration and quiet30-minute PR/issue monitoring.

- Sol6.1 chat01a14d91-82a4-7eb1-89d3-8c3dc2d084f8: one existing worker moves to T24067 ETC80140F50 func80141ABC only after completed DATE44E20; other T24064 functions deferred. Coordinator does no duplicate probe work.
- Sol6.1 chat01a1410a-cead-7cf3-9bfa-aa44f25f055b: ENDING80134D20 func80135F4C only, no extra agents.
- Astra stopped after EVENT T24023 match. TEL/MAIN7132C/GEKO42C9C reservations are inactive until explicit reassignment.
- All three root subagents stopped after saving T24061/T24063 archives and T24062 mixer match. Do not restart automatically.

Contributor exclusions NAME_ENT/TT and DATE2 continue. Existing30-minute read-only contributor heartbeat stays ACTIVE; unchanged checks remain quiet. No contributor messages authorized. Old integration automation remains paused.

## Completed bounded passes

- T24001 MAIN counter: Sol and Astra negative variants archived; one increment-instruction mismatch remains.
- T24002 OPTION: one match; residual drafts archived. T24003 BUNKAKEN: two matches, loader draft remains. T24004 SHOUGATU: one match, residual drafts archived.
- T24005 BUNKAKEN state machine: complete 3232-byte match, independent review recorded, ticket Done.
- T24006 MAIN render family: no matches, source hypotheses and blockers archived. T24007 GEKO: one match, residual guard/drafts and diagnostic error-handling fix published.
- T24009 ENDING: no match; pointer-lifetime scheduling improvement and reproducer published at 4375a2858.
- T24010 DATE/ETC coordinator chat `01a14d91-82a4-7eb1-89d3-8c3dc2d084f8`: finished; all worker/coordinator handoffs on main. T24011 DATE no matches; T24012 ETC one match and three residual drafts.
- T24013 RPG_BAT: one 2424-byte match; two residual source drafts published at ae111b687.
- T24008 TAIIKU closure and guarded updater archived on main; T24015 MAIN closure archive integrated with336627a64.
- T24016 BUNKASAI: both targets verified,3160 bytes, independent root review complete.
- T24014 TACO: no match after specific PRE hypotheses; three-case reproducer and negative evidence published at 76dd4762f. Root Astra closer stopped; its slot is reused for Sol medium family work.

TT/DATE2 reservations and contributor claims #50/#56/#57 remain excluded. Contributor heartbeat runs every 30 minutes; no contributor messages are authorized. The old integration automation remains paused; this explicit 100% goal run is active.

## Integration and resource rules

Use `/Users/lila/Documents/Projects/tokimemo-wt/w2-int`, branchw2-int, and shared `.claude/orchestrator/state/int-merge.lock`. `w2-good` tracks verified integration. Main user checkout is fast-forwarded after publication; preserve untracked results.txt and all unrelated work.

Local merge-codex scripts preserve conflicts for manual resolution and retain worker checkouts; no automatic resets or forced worktree deletion. Wiki unions preserve append-only evidence, then kanban_sync runs in Docker. Source/header conflicts require deliberate resolution. First resumed integration received a clean668-step build; subsequent source changes use dependency-aware rebuild plus explicit hashes of all27 outputs, function proofs and guards. No per-function compiler exceptions or inline assembly substitutes.

Docker only for Python, toolchains and builds. Reuse built worker trees; immutable disc inputs use APFS clones or hardlinks. One compiler per worker, generally ninja-j1; root integration at-j2. Source-family recovery uses Sol6.1 medium; Astra receives bounded concrete blockers and negative-probe evidence. Preserve useful drafts without claiming compilation/audit clarity proves matching. See [[tickets/T-24000-complete-matching]], [[build-step-gaps]], [[matching-notes]] and per-family tickets before repeating work.

## Latest contributor and archive check

2026-10-18 07:11 UTC (Oct18 local): GitHub had no open PRs. Issues #50, #56, #57 remain open with no new comments; reservations unchanged. Next periodic check around 07:41 UTC. No messages posted.

DATE and ETC final coordinator/worker handoffs are now on main, including source-only residual drafts and fixed DATE diagnostic failure handling. No additional C bytes are claimed for these archives. The GEKO probe failure accounting was also repaired after review: candidate errors retain diagnostics and exit nonzero. All current source wins retain independent literal proofs.

Local wiki-only and source-archive merge helpers reject staged production paths and retain the integration lock/ancestry checks. Reviewed draft/prose-only changes do not require repeated binary builds; production source/header changes retain all 27 hashes, guards and literal proofs.

## 80% milestone verification

TAIIKU func_80137EA8 (692 bytes) and RPG_BAT func_80139510 (2,424 bytes) were independently reviewed against CODING_STANDARDS.md and verified from unguarded default linked function bytes before publication. Both integrations pass all 27 binary hashes and header/global/prototype guards. TAIIKU uses a matched sibling and a real four-byte result array; RPG_BAT uses SDK RECTs, shared aggregates, required callback declarations and explicitly marked two-word frame padding. No unresolved source findings. Family tickets remain open while their remaining functions are investigated.

## Refreshed sibling priority evidence

The Docker sibling census at main 1a8d22a06 indexed 6,475 matched functions and 460 remaining targets after excluding TT, DATE2 and NAME_ENT from targets. There are 22 targets at opcode similarity >=0.90 (20,456 instruction bytes), another 23 at >=0.80 (14,388 bytes), and three exact opcode/register clones. These are prioritization measurements, not verified coverage or the canonical progress denominator. Reports are ignored under build/complete-siblings.json and build/complete-siblings.log in the main checkout.

Published clone ports for T24017: EVENT func_80108B24 and GEKO func_8013FD88 (476 bytes each), both similar to matched ENDING func_80139B48; SHUGAKU func_80133038 (176 bytes), similar to matched DATE func_80149F48 / GEKO func_80137580. All three were independently verified and published at979a83671.

Large follow-up candidates: DATE func_801462B0 (3,804 bytes) and EVENT func_8011C750 (3,700) now have matched DATE2 func_80133AF0 as a strong reference; EVENT func_8011DDB8 (3,992) has matched DATE func_80147958. Read T12100/T23004 negative evidence before another attempt; DATE2 is reference-only, still excluded from edits.

New-C proof safeguard: T24016's first installation helper accidentally retained NON_MATCHING, so its first default proof was an assembly fallback and was rejected before integration. Corrected commit7baa1683c removes the guard, recompiles normal C and passes literal proof. The local verify-new-c.py integration helper now rejects any target retaining INCLUDE_ASM before running default linked-byte verification. Positive TAIIKU and negative guarded-TACO checks passed. No false C bytes were published or counted.

## Restart follow-through

Both user-owned chats were confirmed active after the restart. Astra completed the 8672-byte dialogue function and continues the large EVENT handler. Sol completed both T24030 BUNKASAI targets (2020 bytes), independently verified and published atd3965d02e, and moved to T24031. T24017 is Done after independent literal proofs and all27 hashes.

## Latest integrated follow-through

GYOZI T24019, BUNKASAI T24030, RPG_BAT T24027 and SHOUGATU T24026 code handoffs were independently reviewed against CODING_STANDARDS.md and passed all27 original hashes, header/global/prototype guards, target-INCLUDE_ASM rejection and default literal C proofs. Six functions/4776 bytes added after979a83671. Shared overlay headers preserve all existing code bytes. All FAKE views and scheduling forms remain documented. T24030 is Done; other partially matched family tickets remain In Review. T24015 and T24027 residual drafts are included with their code handoffs. Source-only T24018 and Astra T24020–23 archives preserve negative results without coverage claims.

## Refreshed census and near-miss evidence

At27d578ee0 the Docker sibling refresh indexed6486 matched functions and449 remaining targets after excluding TT/DATE2/NAME_ENT from targets:419900 instruction bytes,68 guards. There are16 targets at similarity>=.90 (18228 bytes) and20 at.80–.90 (10860 bytes); no exact opcode/register clones remain in this filtered target set. The canonical progress denominator remains2279368 game-code bytes. New GYOZI C provides a .724 reference for MAINfunc80049FF0, but no new high-similarity reference was unlocked. Reports remain ignored under build/complete-siblings-latest.json/log.

T24029 seasonal drafts and T24028 DATE/EVENT near-miss checkpoints are on main. T24028 DATE guard improves26 to11 differing instructions; EVENT source-only draft has7 remaining, both exact instruction counts and frame sizes. Both still use ASM in default builds and add zero verified bytes. Explicit FAKE fade/frame forms and timer-allocation negatives are preserved; no EVENT header mutation. Main publication checked all27 unchanged binary hashes and guards.

## Latest verification

T24035 first pair and allthree T24034 targets passed independent root review, all27 default binary hashes, header/global/prototype guards and explicit target-INCLUDE_ASM rejection plus linked C proofs. EVENT940-byte recovery removes duplicate argument-site narrowing from the legacy K&R calls while preserving the sibling’s marked dead-copy liveness. SDK primitives and existing EventStateView are used. T24034 Done; T24035 final guarded-draft checkpoint8e742c12d is archived on main. T24033 VALEN32D0C guarded draft was also published at e5bddd3cf with no added verified bytes. Local integration helper now checks staged whitespace before expensive builds as well as afterward.

Coverage uses canonical splat sizes, not the sum of linked proof extents: T24035 BUNKA_SD344A8 is148 canonical bytes versus152 linked proof bytes; T24034 EVENT800FFA4C is392 canonical versus404 linked. This batch adds2188 canonical bytes,2204 linked proof bytes. The Docker progress total above is authoritative.

## Latest source and archive review

T24036 SHUGAKU244-byte match and T24037 EVENT628-byte match passed independent review, explicit no-fallback C gates, normal linked proofs, all27 binary hashes and header/global/prototype guards before main publication. T24037 keeps the canonical main scalar declarations and uses documented EVENT load-time byte/halfword arrays of eight entries; all header closures compile. Neither match adds a new FAKE. Their unresolved GEKO/EVENT sibling drafts remain guarded, not counted. T24031 four-body source-only archive is preserved on main with its reproduction script and accurate negative results.

## Largest-function closure and combined verification

MAIN dialogue_speak_part now compiles as actual matching C:2168 instructions/8672 literal text bytes, original0x290-byte frame. Replacing an unused padding word with an argument local and explicitly marked `(render_x & 0) - 40` expressions gives the early constant lifetime required at two calls. Baseline replay remained4 aligned edits/16 printed rows; the accepted form is0/0. Root verified target fallback absence and normal linked bytes independently. T24021 Done; remaining hard targets stay open.

The line renderer, two OPTION menus, ETC number renderer and MASTER event-record handler were reviewed and staged locally, then verified together with dialogue using one build, all27 original hashes, all six explicit C proofs and all header/global/prototype guards. Main and w2-good advanced only after success. This reduces repeated shared-header builds without weakening verification. T24038 Done; T24040/T24041 remain In Review for their unresolved siblings.

## Worker continuation, 2026-10-18

User requested restarting stopped chats, especially large functions. Astra and the MASTER T24043 chat were confirmed active; no duplicate assignment was sent. Restarted chat `01a1410a-cead-7cf3-9bfa-aa44f25f055b` with Sol6.1 medium on T24047, ENDING func_80135F4C (~3220 instruction bytes), revoking its obsolete MAIN/EVENT/TEL ownership. Confirmed the new turn is active. Reassigned the idle OPTION worker to T24048, MASTER func_801344E0 (~2912 instruction bytes), excluding T24043's file. Workers create their own tickets before code work.

T24044 closed with source-only pin `2c7dbab72`, pending archive integration. T24045 delivered actual-C TEL func_8013D03C pin `c6847bd31` (2128 literal bytes); worker proof passes, root independent review/integration pending. Neither handoff changes the published 81.1% coverage yet.

## Latest verified checkpoint

TEL T24045 published f828daec1 after independent all27 hashes, interface guards and actual-C literal proof2128 bytes. T24044 corrected TAIIKU/TACO source archives published73e1ab681, no new C claim. Superseding earlier ownership: T24049 worker owns ETC80146400 func801480D0; T24048 worker owns MASTER80133680 func801344E0; T24047 chat owns ENDING80134D20 func80135F4C in /private/tmp/tokimemo-ending-large. MAIN T24046, MASTER T24043 and Astra EVENT remain active.

Contributor check 2026-10-18 07:21 UTC: no open PRs; issues50/56/57 unchanged. Keep NAME_ENT and TT claims excluded. Next periodic check around07:51 UTC. DATE T24051 confirmed active in same built /private/tmp/tokimemo-complete-etc tree, branch codex/complete-date-large-followup; target callback-pointer return-view hypothesis differs from earlier whole-function return tests.

Latest ownership override: ETC T24049 source-only archive published b57b6e50b (corrected eleven-record bonus bound, no match). Its worker now owns T24052 SHOUGATU801330C0 funcs80133208/801333F0, using matched GYOZI siblings. ENDING T24047 remains active: full-TU aligned cost114,803/805 instructions, frame176 versus160 target; no verified C claim.

## Function-count pass requested by user

Filtered the prior sibling census against current production INCLUDE_ASM references and sorted targets up to300 instruction bytes. Existing contributor and live-TU exclusions still apply. T24053 Sol medium worker owns EVENT8010B360 func8010B614(100), BUNKA_SD80135160 func801355AC(132), SHUGAKU80139D00 func8013A1D4(160). First lead: T20620 already reported a scratch EVENT match withheld for conveyor ownership; matching-notes retains byte-store/later-read recipe. Must reproduce against current types and prove actual default C before counting.

After preserving T24050, its Sol medium worker continues T24054: main80042A00 func80043010(176), OPTION80132000 func80132000(176), KANGEI80137B90 func8013852C(180). Short size alone is not evidence of easy matching: historical failed experiments must be checked first. Large EVENT, ENDING and DATE chats stay active. T24052 SHOUGATU sibling pair stays active. T24048 MASTER source-only archive published444be6846; no new matching bytes.

Latest count-pass publication: T24052 first sibling f90483388 and T24053 two short wins d40a1ece7 are independently verified on main. Both tickets remain active for unfinished targets. T24050 source-only archive9bf7361e4 adds no bytes; worker proceeds with T24054. DATE T24051 and EVENT Astra now have operand-order-only near misses; exchanged comparison-source evidence, no additional match claimed.

Latest active ownership: T24052 pair complete and verified; worker continues T24056 SHOUGATU8013D660 funcs8013D820/8013D92C and SHOUGATU8013DD90 func8013DF14. T24053 final eight-row SHUGAKU source archive published1f42091ee; its worker continues T24055 ETC80143240 funcs8014354C/80143758/80143968. T24054 short main/OPTION/KANGEI lane and large EVENT/DATE/ENDING chats remain active.

## 2026-10-18 source checkpoints and recipe transfer

Published T24057 ENDING fade-pair source archive2a97faf17 and T24060 SHUGAKU source archive89ba6cc66; both change only drafts/wiki and add no matched functions. Current verified totals remain6507/6958 and1851416/2279368 bytes. Astra reports a6848-byte literal scratch EVENT match; do not count until independent default production proof. Its neighbor-base selector access recipe has been transferred to the DATE coordinator. T24064 corrects a provisional coordinator ticket-ID collision with existing SHUGAKU T24060.

Contributor check07:47UTC: no open PRs; claims#50/#56/#57 unchanged. NAME_ENT and TT exclusions continue.

## 2026-10-18 focused-worker checkpoints

DATE negative-transfer checkpoint33ce56acce published96301444d. ENDING checkpointb0111332c reviewed and integrated with guarded C, explicit FAKE width/alias views, original ASM fallback, all27 hashes and interface guards passing; one trailing blank line removed from archived body. No new matching functions or bytes. Both Sol workers remain on their assigned functions. DATE subsequently reports scratch4-row result with exact frame/tables and only two reversed selector comparisons; not a literal production match and not counted.

## 2026-10-18 Antigravity CLI workers

User authorized two Antigravity workers using Gemini4Argon Medium; explicitly requested CLI. Installed CLI is `~/.local/bin/agy` (not `antigravity`); `agy models` confirms `gemini-4-argon-medium`. Existing two Sol workers remain DATE/ENDING. No additional subagents authorized inside these workers.

- T24065 OPTION func8013394C, src/ovl/OPTION/80132000.c. Reused clean complete-option checkout, fast-forwarded to main b493b6d48. Conversation a95cd5c6-72fb-4270-804c-6e4404606bdd; exec session70265; logs /tmp/t24065-argon.jsonl and .stderr.
- T24066 BUNKA_SD func80134418, src/ovl/BUNKA_SD/80134260.c. Reused clean complete-bunkaken checkout, fast-forwarded to main b493b6d48. Conversation abd1a788-0787-4e42-a019-383201090726; exec session78281; logs /tmp/t24066-argon.jsonl and .stderr.

Launch flags `--model gemini-4-argon-medium --effort medium --output-format stream-json --print`. Auto mode is unavailable for this account; CLI retained default request-review permissions. No bypass flag used. Monitor logs for approval blockers and live process handles before restarting. Both own only one function, one Docker compiler, preserve evidence and local commits; root alone verifies/publishes. New tickets created by workers before edits.

Antigravity startup follow-up: both CLI sessions terminated after shell permission denials in default request-review mode (70265 and78281 are finished). No matching progress claimed. User approval for routine project commands was requested; do not restart with bypass flags or repeat denied actions before approval. Sol DATE/ENDING work continues independently.

## 2026-10-18 DATE dispatcher closure

T24051 independently verified as actual unguarded default C: literal3200bytes, all27 binary hashes and header/global/prototype guards pass. Exact21 local tables and0xE38 frame retained. Neighboring selector byte view, comparison captures and pointer home are explicitly FAKE. Five DATE declarations reflect observed callbacks/data; one absolute D800CA2E8 export adds no storage. Root standards review complete; ticket Done. Two Sol lanes now ETC41ABC and ENDING35F4C; Antigravity remains pending shell approval.

## Shared diagnostic allocator restored2026-10-18

ENDING worker rebuilt the documented pinned tracer in Docker in about3minutes/62MB. Generated uopt source hash matches the documented pin; patched and traced outputs reproduce stock text/rodata/relocations and section inventories for the preferred TU. Host binary `/private/tmp/t24047-uopt/variants/trace`; `TOKIMEMO_SCRATCH=/private/tmp/t24047-uopt` mounts it at `/scratch/variants/trace`. Use explicit `regdiff --uopt` path per tool help. Production compiler unchanged. ETC T24067 was given this read-only binary for its save/end pointer colour mismatch; workers write separate traces in their own ignored build directories, one compiler each. No extra agent or duplicated rebuild. Binary and raw traces remain uncommitted.

## Contributor check 2026-10-18 08:40 UTC

No open PRs. Open claims #50, #56 and #57 have unchanged update timestamps; NAME_ENT and TT reservations remain excluded. No contributor messages sent. Next check around09:10 UTC. ENDING and ETC workers remain active; their latest source-only archives are published at833561fb7 andebf0aa7b1 respectively. Verified coverage remains81.7%.

## Contributor integration 2026-10-18 09:10 UTC

PR #62 integrated at64ef469bf: NAME_ENT func_80132320 and func_80134248 independently verified as default C396/2256 bytes. All27 hashes and interface guards pass. T-20013 reviewed and Done. Authoritative Docker progress now6512/6958 functions,1864344/2279368 bytes (81.8%);415024 bytes remain. Gain2652 bytes. PR #61 contains TT compiler measurements and a read-only survey, no match claim; fetched for follow-up integration. Open claim timestamps #50/#57 unchanged; #56 updated with that evidence. Next contributor check around09:40 UTC. No contributor messages sent.

PR #61 follow-up: integrated at5397dc7f7. Root inspected the read-only survey and reproduced its counts in Docker; all27 hashes and interface guards pass. GitHub confirms PRs #61 and #62 MERGED. TT remains ASM with zero new matched bytes; contributor evidence narrows the open lead to compiler-emitted divide/address expansion. No assembler workaround was introduced. Two existing Sol matching workers continue ENDING35F4C and ETC41ABC. Next contributor check remains around09:40 UTC.
