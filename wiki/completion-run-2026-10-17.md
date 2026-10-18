---
type: coordination
updated: 2026-10-17
---

# Current 100% matching run

User resumed work toward 100% verified game-code matching. Current published code checkpoint `f55a2d7e9` has **6,499/6,958 functions and 1,847,520/2,279,368 bytes (81.1%)**, measured in Docker. Since baseline `283b86fca`: 33 functions / 34,052 bytes added. The active goal remains 100%, with 431,848 game-code bytes remaining. SDK assembly is separately reported and excluded.

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

Each was independently checked against default linked function bytes, all27 binary hashes, headers, global views and prototype guards before main publication. OPTION argument evaluation and ETC return liveness have explicit FAKE explanations. A default ASM fallback MATCH is not new C progress.

## Active ownership

- T24046 Sol medium root worker, codex/complete-main: MAIN8004C3F0 srn_tpage_show, roughly3.2KB. T24041 renderer verified; CD source-only checkpoint5ca2b84b4 queued for archival integration.
- T24045 Sol medium root worker, codex/complete-bunkaken: TEL8013CCD0 func8013D03C, roughly2.1KB, using existing49-row draft and matched local recipe. T24035 final guard is integrated; T24039 source-only checkpoint68126eccd queued.
- T24044 Sol medium root worker, codex/complete-option: TAIIKU80138950 func8013B3B0 and TACO80141DE0 func801424A8. T24040 two OPTION wins and final guarded selector are integrated.
- Astra chat01a14d9f-757c-7661-a87e-e183dc7e7687: dialogue_speak_part completed and independently integrated. Continues EVENT8010FE10 func801103A0; reserved MAIN8006CB30 ONLYfunc8007132C, GEKO80142760 ONLYfunc80142C9C, TEL80132000. T24042 is the renamed TEL helper follow-up; T24026 remains the earlier SHOUGATU ticket. One compiler.
- Sol medium chat01a14d91-82a4-7eb1-89d3-8c3dc2d084f8: T24043 MASTER80139000 func80139CC8, roughly2.7KB. T24038 both matches are independently integrated. One compiler.

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

2026-10-18 06:43 UTC (Oct17 local): GitHub had no open PRs. Issues #50, #56, #57 remain open with no new comments; reservations unchanged. Next periodic check around 07:13 UTC. No messages posted.

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

Both user-owned chats were confirmed active after the restart. Astra continues the roughly9KB dialogue function. Sol completed both T24030 BUNKASAI targets (2020 bytes), independently verified and published atd3965d02e, and moved to T24031. T24017 is Done after independent literal proofs and all27 hashes.

## Latest integrated follow-through

GYOZI T24019, BUNKASAI T24030, RPG_BAT T24027 and SHOUGATU T24026 code handoffs were independently reviewed against CODING_STANDARDS.md and passed all27 original hashes, header/global/prototype guards, target-INCLUDE_ASM rejection and default literal C proofs. Six functions/4776 bytes added after979a83671. Shared overlay headers preserve all existing code bytes. All FAKE views and scheduling forms remain documented. T24030 is Done; other partially matched family tickets remain In Review. T24015 and T24027 residual drafts are included with their code handoffs. Source-only T24018 and Astra T24020–23 archives preserve negative results without coverage claims.

## Refreshed census and near-miss evidence

At27d578ee0 the Docker sibling refresh indexed6486 matched functions and449 remaining targets after excluding TT/DATE2/NAME_ENT from targets:419900 instruction bytes,68 guards. There are16 targets at similarity>=.90 (18228 bytes) and20 at.80–.90 (10860 bytes); no exact opcode/register clones remain in this filtered target set. The canonical progress denominator remains2279368 game-code bytes. New GYOZI C provides a .724 reference for MAINfunc80049FF0, but no new high-similarity reference was unlocked. Reports remain ignored under build/complete-siblings-latest.json/log.

T24029 seasonal drafts and T24028 DATE/EVENT near-miss checkpoints are on main. T24028 DATE guard improves26 to11 differing instructions; EVENT source-only draft has7 remaining, both exact instruction counts and frame sizes. Both still use ASM in default builds and add zero verified bytes. Explicit FAKE fade/frame forms and timer-allocation negatives are preserved; no EVENT header mutation. Main publication checked all27 unchanged binary hashes and guards.

## Latest verification

T24035 first pair and allthree T24034 targets passed independent root review, all27 default binary hashes, header/global/prototype guards and explicit target-INCLUDE_ASM rejection plus linked C proofs. EVENT940-byte recovery removes duplicate argument-site narrowing from the legacy K&R calls while preserving the sibling’s marked dead-copy liveness. SDK primitives and existing EventStateView are used. T24034 Done; T24035 final guarded-draft checkpoint8e742c12d is queued. T24033 VALEN32D0C guarded draft was also published at e5bddd3cf with no added verified bytes. Local integration helper now checks staged whitespace before expensive builds as well as afterward.

Coverage uses canonical splat sizes, not the sum of linked proof extents: T24035 BUNKA_SD344A8 is148 canonical bytes versus152 linked proof bytes; T24034 EVENT800FFA4C is392 canonical versus404 linked. This batch adds2188 canonical bytes,2204 linked proof bytes. The Docker progress total above is authoritative.

## Latest source and archive review

T24036 SHUGAKU244-byte match and T24037 EVENT628-byte match passed independent review, explicit no-fallback C gates, normal linked proofs, all27 binary hashes and header/global/prototype guards before main publication. T24037 keeps the canonical main scalar declarations and uses documented EVENT load-time byte/halfword arrays of eight entries; all header closures compile. Neither match adds a new FAKE. Their unresolved GEKO/EVENT sibling drafts remain guarded, not counted. T24031 four-body source-only archive is preserved on main with its reproduction script and accurate negative results.

## Largest-function closure and combined verification

MAIN dialogue_speak_part now compiles as actual matching C:2168 instructions/8672 literal text bytes, original0x290-byte frame. Replacing an unused padding word with an argument local and explicitly marked `(render_x & 0) - 40` expressions gives the early constant lifetime required at two calls. Baseline replay remained4 aligned edits/16 printed rows; the accepted form is0/0. Root verified target fallback absence and normal linked bytes independently. T24021 Done; remaining hard targets stay open.

The line renderer, two OPTION menus, ETC number renderer and MASTER event-record handler were reviewed and staged locally, then verified together with dialogue using one build, all27 original hashes, all six explicit C proofs and all header/global/prototype guards. Main and w2-good advanced only after success. This reduces repeated shared-header builds without weakening verification. T24038 Done; T24040/T24041 remain In Review for their unresolved siblings.
