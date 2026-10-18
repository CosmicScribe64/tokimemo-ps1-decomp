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

- Root Sol worker T24013: RPG_BAT `80137280.c` / `801525B0.c`, three remaining helpers; reuses `codex/complete-option`.
- Root Astra closer T24014: TACO `80149F90.c`, `func_8014A480`; test specific PRE expression representative hypotheses after T22025, reuses `codex/complete-bunkaken`.

- Root Sol worker T24008: TAIIKU `80133C80.c`, five runner helpers; reuses `codex/complete-main`.
- Completed bounded Astra pass T24009 (source archive b7d446437 published as 4375a2858): ENDING `801396E0.c`, `func_8013A2E8` (4,648 instruction bytes), bounded novel source hypotheses for the documented assembler scheduling limit; reuses `codex/complete-bunkaken`.

- Completed bounded pass T24006: MAIN8004C3F0 rendering family, Sol6.1 medium; reuses codex/complete-main checkout. MAIN8006CB30 counter is parked after Sol20 + Astra19 negative variants; draft differs by one increment instruction. ArchivesT24001 are on main.
- Completed bounded pass T24007: GEKO801323E0 family, Sol6.1 medium; reuses codex/complete-option. Earlier OPTION and SHOUGATU bounded packages have committed residual drafts.
- Completed Astra closer T24005: BUNKAKEN80143DD0, 3,232 bytes, now unguarded and verified. Marked FAKE empty selector test fixed the final four instruction words. Reuses codex/complete-bunkaken. Earlier BUNKAKEN packageT24003 has2/3 matched; remaining loader draft318 rows preserved.
- Completed Sol coordinating chat01a14d91-82a4-7eb1-89d3-8c3dc2d084f8, T24010: DATE80141EE0/8015A640 (T24011) and ETC80132880 (T24012); two workers, bounded Astra assistance for a concrete DATE near miss. Parent alone integrates/pushes.
- Dedicated Astra chat01a14d9f-757c-7661-a87e-e183dc7e7687, T24020: reserved MAIN8007C030 dialogue_speak_part (~8980B), TEL80132000 func_80136724, EVENT8010FE10 func_801103A0. Starts dialogue, one hard target at a time. These files are excluded from other assignments.

TT/DATE2 local reservations and GitHub contributor claims#50/#56/#57 are excluded. Check-decomp-contributor-updates heartbeat checks PRs/issues every30minutes. Initial check found no open PRs. Do not message contributors without user authorization. Old integration automation remains paused; this explicit goal run is active.

## Integration and resource rules

Use `/Users/lila/Documents/Projects/tokimemo-wt/w2-int`, branchw2-int, and shared `.claude/orchestrator/state/int-merge.lock`. `w2-good` tracks verified integration. Main user checkout is fast-forwarded after publication; preserve untracked results.txt and all unrelated work.

Local merge-codex scripts preserve conflicts for manual resolution and retain worker checkouts; no automatic resets or forced worktree deletion. Wiki unions preserve append-only evidence, then kanban_sync runs in Docker. Source/header conflicts require deliberate resolution. First resumed integration received a clean668-step build; subsequent source changes use dependency-aware rebuild plus explicit hashes of all27 outputs, function proofs and guards. No per-function compiler exceptions or inline assembly substitutes.

Docker only for Python, toolchains and builds. Reuse built worker trees; immutable disc inputs use APFS clones or hardlinks. One compiler per worker, generally ninja-j1; root integration at-j2. Source-family recovery uses Sol6.1 medium; Astra receives bounded concrete blockers and negative-probe evidence. Preserve useful drafts without claiming compilation/audit clarity proves matching. See [[tickets/T-24000-complete-matching]], [[build-step-gaps]], [[matching-notes]] and per-family tickets before repeating work.

## Latest contributor and archive check

2026-10-18 06:16 UTC (Oct17 local): GitHub had no open PRs. Issues #50, #56, #57 remain open with no new comments; reservations unchanged. Next periodic check around 06:46 UTC. No messages posted.

DATE and ETC final coordinator/worker handoffs are now on main, including source-only residual drafts and fixed DATE diagnostic failure handling. No additional C bytes are claimed for these archives. The GEKO probe failure accounting was also repaired after review: candidate errors retain diagnostics and exit nonzero. All current source wins retain independent literal proofs.

Local wiki-only merge helper rejects any staged path outside wiki/ and retains the integration lock/ancestry checks; binary checks are repeated for source integration, not solely for prose updates.

## 80% milestone verification

TAIIKU func_80137EA8 (692 bytes) and RPG_BAT func_80139510 (2,424 bytes) were independently reviewed against CODING_STANDARDS.md and verified from unguarded default linked function bytes before publication. Both integrations pass all 27 binary hashes and header/global/prototype guards. TAIIKU uses a matched sibling and a real four-byte result array; RPG_BAT uses SDK RECTs, shared aggregates, required callback declarations and explicitly marked two-word frame padding. No unresolved source findings. Family tickets remain open while their remaining functions are investigated.
