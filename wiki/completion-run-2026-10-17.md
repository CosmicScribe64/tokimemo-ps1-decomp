---
type: coordination
updated: 2026-10-17
---

# Current 100% matching run

User resumed work toward 100% verified game-code matching. Current published code checkpoint `4d66bf645` has **6,486/6,958 functions and 1,832,660/2,279,368 bytes (80.4%)**, measured in Docker. Since baseline `283b86fca`: 20 functions / 19,192 bytes added. The active goal remains 100%, with 446,708 game-code bytes remaining. SDK assembly is separately reported and excluded.

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

Each was independently checked against default linked function bytes, all27 binary hashes, headers, global views and prototype guards before main publication. OPTION argument evaluation and ETC return liveness have explicit FAKE explanations. A default ASM fallback MATCH is not new C progress.

## Active ownership

- T24029 Sol medium root worker, codex/complete-main: GYOZI80136220 func801368B8 and SHOUGATU80134890 func801350F8, a shared source family.
- T24028 Sol medium root worker, codex/complete-bunkaken: DATE801461B0 func8014718C and EVENT8011C650 func8011D5C4; matched DATE2func801348F0 is reference-only.
- T24032/T24033 Sol medium root worker, codex/complete-option: six DATE/TACO416-byte SDK-like matrix routines get one provenance/control pass, then VALEN80133C70 func80134030 and VALEN80132760 func80132D0C. No per-function compiler exceptions or coverage reclassification.
- Astra chat01a14d9f-757c-7661-a87e-e183dc7e7687 active on MAIN8007C030 dialogue_speak_part; owns TEL80132000, EVENT8010FE10, MAIN8006CB30 ONLYfunc8007132C and GEKO80142760 ONLYfunc80142C9C as reserved follow-ups. Do not touch parked MAIN8006D7EC. One compiler.
- Sol medium chat01a14d91-82a4-7eb1-89d3-8c3dc2d084f8 active on T24031, paired GYOZI80143AD0 funcs80144B70/80144018 and SHOUGATU80141780 funcs8014288C/80141D20. Reuses built private/tmp tree, one compiler.

## Completed bounded passes

- T24001 MAIN counter: Sol and Astra negative variants archived; one increment-instruction mismatch remains.
- T24002 OPTION: one match; residual drafts archived. T24003 BUNKAKEN: two matches, loader draft remains. T24004 SHOUGATU: one match, residual drafts archived.
- T24005 BUNKAKEN state machine: complete 3232-byte match, independent review recorded, ticket Done.
- T24006 MAIN render family: no matches, source hypotheses and blockers archived. T24007 GEKO: one match, residual guard/drafts and diagnostic error-handling fix published.
- T24009 ENDING: no match; pointer-lifetime scheduling improvement and reproducer published at 4375a2858.
- T24010 DATE/ETC coordinator chat `01a14d91-82a4-7eb1-89d3-8c3dc2d084f8`: finished; all worker/coordinator handoffs on main. T24011 DATE no matches; T24012 ETC one match and three residual drafts.
- T24013 RPG_BAT: one 2424-byte match; two residual source drafts published at ae111b687.
- T24008 TAIIKU closure and guarded updater archived on main; T24015 MAIN closure archive a260c269a pending integration.
- T24016 BUNKASAI: both targets verified,3160 bytes, independent root review complete.
- T24014 TACO: no match after specific PRE hypotheses; three-case reproducer and negative evidence published at 76dd4762f. Root Astra closer stopped; its slot now runs Sol medium T24016.

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

Queued clone ports for T24017: EVENT func_80108B24 and GEKO func_8013FD88 (476 bytes each), both similar to matched ENDING func_80139B48; SHUGAKU func_80133038 (176 bytes), similar to matched DATE func_80149F48 / GEKO func_80137580. Wait for T24008 closure before reassigning its worker; no overlap with active large-function EVENT ownership.

Large follow-up candidates: DATE func_801462B0 (3,804 bytes) and EVENT func_8011C750 (3,700) now have matched DATE2 func_80133AF0 as a strong reference; EVENT func_8011DDB8 (3,992) has matched DATE func_80147958. Read T12100/T23004 negative evidence before another attempt; DATE2 is reference-only, still excluded from edits.

New-C proof safeguard: T24016's first installation helper accidentally retained NON_MATCHING, so its first default proof was an assembly fallback and was rejected before integration. Corrected commit7baa1683c removes the guard, recompiles normal C and passes literal proof. The local verify-new-c.py integration helper now rejects any target retaining INCLUDE_ASM before running default linked-byte verification. Positive TAIIKU and negative guarded-TACO checks passed. No false C bytes were published or counted.

## Restart follow-through

Both user-owned chats were confirmed active after the restart. Astra is concentrating on the roughly9KB dialogue function after preserving EVENT and MAIN investigations. Sol returned BUNKASAI func80151364 (844 bytes), pinned f41f35d3a, for independent integration; its other target remains active. These queued wins are not included in published coverage. T24017 was reviewed against CODING_STANDARDS.md and independently passed all27 hashes, guards and allthree literal C proofs before publication979a83671; its ticket is Done.

## Latest integrated follow-through

GYOZI T24019, BUNKASAI T24030, RPG_BAT T24027 and SHOUGATU T24026 code handoffs were independently reviewed against CODING_STANDARDS.md and passed all27 original hashes, header/global/prototype guards, target-INCLUDE_ASM rejection and default literal C proofs. Six functions/4776 bytes added after979a83671. Shared overlay headers preserve all existing code bytes. All FAKE views and scheduling forms remain documented. T24030 is Done; other partially matched family tickets remain In Review. T24015 and T24027 residual drafts are included with their code handoffs. Source-only T24018 and Astra T24020–23 archives preserve negative results without coverage claims.
