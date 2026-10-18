---
type: coordination
updated: 2026-10-17
---

# Current 100% matching run

User resumed work toward 100% verified game-code matching. Current published code checkpoint `12e1afca5` has **6,475/6,958 functions and 1,823,596/2,279,368 bytes (80.0%)**, measured with `tools/progress.py` in Docker. Relative to resumed baseline `283b86fca`: 9 functions / 10,128 bytes added. The earlier 80% milestone has been crossed; the active goal remains 100%, with 455,772 game-code bytes remaining. SDK library assembly remains separately reported and excluded from this scope.

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

Each was independently checked against default linked function bytes, all27 binary hashes, headers, global views and prototype guards before main publication. OPTION argument evaluation and ETC return liveness have explicit FAKE explanations. A default ASM fallback MATCH is not new C progress.

## Active ownership

- T24008, Sol 6.1 medium: TAIIKU `80133C80.c`; one of five runner helpers matched, bounded remainder in progress. Worktree `codex/complete-main`.
- T24015, Sol 6.1 medium: MAIN `8005A0B0.c`, five unstarted helpers from w4-p41. Worktree `codex/complete-option`.
- T24016, Sol 6.1 medium: BUNKASAI `801354B0.c` / `80154018.c`, two unstarted helpers from w4-p44. Reuses the now-idle Astra worktree `codex/complete-bunkaken` at 15133a84a; no new checkout.
- T24020, dedicated Astra chat `01a14d9f-757c-7661-a87e-e183dc7e7687`: MAIN `8007C030.c` dialogue_speak_part, TEL `80132000.c` func_80136724, EVENT `8010FE10.c` func_801103A0. One target at a time; last observed on EVENT. All three files remain reserved.

## Completed bounded passes

- T24001 MAIN counter: Sol and Astra negative variants archived; one increment-instruction mismatch remains.
- T24002 OPTION: one match; residual drafts archived. T24003 BUNKAKEN: two matches, loader draft remains. T24004 SHOUGATU: one match, residual drafts archived.
- T24005 BUNKAKEN state machine: complete 3232-byte match, independent review recorded, ticket Done.
- T24006 MAIN render family: no matches, source hypotheses and blockers archived. T24007 GEKO: one match, residual guard/drafts and diagnostic error-handling fix published.
- T24009 ENDING: no match; pointer-lifetime scheduling improvement and reproducer published at 4375a2858.
- T24010 DATE/ETC coordinator chat `01a14d91-82a4-7eb1-89d3-8c3dc2d084f8`: finished; all worker/coordinator handoffs on main. T24011 DATE no matches; T24012 ETC one match and three residual drafts.
- T24013 RPG_BAT: one 2424-byte match; two residual source drafts published at ae111b687.
- T24014 TACO: no match after specific PRE hypotheses; three-case reproducer and negative evidence published at 76dd4762f. Root Astra closer stopped; its slot now runs Sol medium T24016.

TT/DATE2 reservations and contributor claims #50/#56/#57 remain excluded. Contributor heartbeat runs every 30 minutes; no contributor messages are authorized. The old integration automation remains paused; this explicit 100% goal run is active.

## Integration and resource rules

Use `/Users/lila/Documents/Projects/tokimemo-wt/w2-int`, branchw2-int, and shared `.claude/orchestrator/state/int-merge.lock`. `w2-good` tracks verified integration. Main user checkout is fast-forwarded after publication; preserve untracked results.txt and all unrelated work.

Local merge-codex scripts preserve conflicts for manual resolution and retain worker checkouts; no automatic resets or forced worktree deletion. Wiki unions preserve append-only evidence, then kanban_sync runs in Docker. Source/header conflicts require deliberate resolution. First resumed integration received a clean668-step build; subsequent source changes use dependency-aware rebuild plus explicit hashes of all27 outputs, function proofs and guards. No per-function compiler exceptions or inline assembly substitutes.

Docker only for Python, toolchains and builds. Reuse built worker trees; immutable disc inputs use APFS clones or hardlinks. One compiler per worker, generally ninja-j1; root integration at-j2. Source-family recovery uses Sol6.1 medium; Astra receives bounded concrete blockers and negative-probe evidence. Preserve useful drafts without claiming compilation/audit clarity proves matching. See [[tickets/T-24000-complete-matching]], [[build-step-gaps]], [[matching-notes]] and per-family tickets before repeating work.

## Latest contributor and archive check

2026-10-18 06:16 UTC (Oct17 local): GitHub had no open PRs. Issues #50, #56, #57 remain open with no new comments; reservations unchanged. Next periodic check around 06:46 UTC. No messages posted.

DATE and ETC final coordinator/worker handoffs are now on main, including source-only residual drafts and fixed DATE diagnostic failure handling. No additional C bytes are claimed for these archives. The GEKO probe failure accounting was also repaired after review: candidate errors retain diagnostics and exit nonzero. All current source wins retain independent literal proofs.

Local wiki-only and source-archive merge helpers reject staged production paths and retain the integration lock/ancestry checks. Reviewed draft/prose-only changes do not require repeated binary builds; production source/header changes retain all 27 hashes, guards and literal proofs.

## 80% milestone verification

TAIIKU func_80137EA8 (692 bytes) and RPG_BAT func_80139510 (2,424 bytes) were independently reviewed against CODING_STANDARDS.md and verified from unguarded default linked function bytes before publication. Both integrations pass all 27 binary hashes and header/global/prototype guards. TAIIKU uses a matched sibling and a real four-byte result array; RPG_BAT uses SDK RECTs, shared aggregates, required callback declarations and explicitly marked two-word frame padding. No unresolved source findings. Family tickets remain open while their remaining functions are investigated.
