---
id: T-9810
title: Permuter farm for near misses
status: Done
assignee: r9-farm
created: 2026-10-11
updated: 2026-10-11
links: ["[[decompile-workflow]]"]
---

## Goal

Run a CPU-only, resumable permuter farm over existing near-miss drafts without changing the working source tree.

## Acceptance criteria

- [x] Collect scoped drafts and scores from wave scratch directories, NON_MATCHING blocks and T-0018 rows; normalize and rank the queue.
- [x] Configurable 20-minute budget and half-core parallelism; persistent Docker workers.
- [x] Verify score-zero candidates with funcdiff --resolve and unit sha1 in isolated copies; export wins with FAKE annotations where required.
- [x] Resume interrupted work, extend improving searches, skip exhausted searches.
- [x] Docker tests, usage documentation, inline standards/spec review.
- [x] About one-hour real run with four jobs; report queue, wins and candidate timings.

## Notes

Scratch root: /private/tmp/claude-501/-Users-lila-Documents-Projects-tokimemo-decomp/43264b98-bb59-4c1b-ad99-a2084d55fcf0/scratchpad/r9-farm/.
No commits, branches, stashes or direct src/ edits. Only containers started by this task may be stopped.

## Comments

2026-10-11: Read wiki index, AGENTS.md/local instructions, CODING_STANDARDS.md (including 7/7a), decompile workflow and permute.py. The wrapper's verification mutates its root temporarily; farm verification must use a private root. Current worktree has no generated asm/build, so workers must configure and split their copies.

2026-10-11: Implemented collector/locked state/persistent launcher/private verification and 13 synthetic tests. Initial Docker collection: 1,076 scoped candidates; unknown diffs sort last and already matched functions are excluded. All 13 tests passed. Four short smoke workers are preparing isolated builds before a full one-hour run. Live checkout remains unchanged under src/.

2026-10-11: Inline review found and resolved three correctness issues: CLI --no-verify's nonzero exit for zero-score outputs (use wrapper run API), historical build-error rows with edits=0 (rank as unknown), and recollection changing a live claim's fingerprint (defer reset until completion). Added coverage for these cases. Earlier full regression: 57 tests passed in 42.600 seconds. The four-worker one-hour run is completing its remaining private splits. Reporting exports per-attempt timings to scratch report.md/report.json.

2026-10-11: Final regression passed 58 tests in 40.473 seconds (40 existing permuter + 18 farm). Inline standards/spec review covers all new tooling and documentation relative to uncommitted HEAD: Docker-only compilation, read-only checkout mount, immutable raw inputs, CPU limits, correct overlay scope, seed/zero verification, process-group cleanup, atomic state, live-owner exclusion, guarded recollection, bounded extension/exhaustion, conservative FAKE comments, and no compiler emulation special cases. All findings resolved. Real-run results remain required before Done; no src/ changes, commits, stashes or branches.

2026-10-11: Real setup profiling exposed exponential regex backtracking in tools/permute.py's strip_functions/def_regex. Fixed both without changing the compile pipeline or per-function rules. The stalled 54,221-byte real context now scans in 3.046 seconds. Added two synthetic parser regressions; all 60 tests pass in Docker (38.592 seconds). Final hour workers restarted with the scanner fix and persistent incremental roots; initial failed starts are retained in the append-only log, and no matches are claimed from them.

2026-10-11: Corrected metadata/scoping collection yields 1,090 candidates. All 21 farm tests pass (11.795 seconds); 42 existing wrapper tests passed in the previous full run. Reports merge worker journals so recollection does not lose historical timings. A real verification control on already matched VALEN func_80132000 passed funcdiff --resolve and the VALEN unit sha1 in 43.763 seconds, with source restoration asserted; it is not a new farm win. Four real searches continue; GYOZI func_80137F48 reached score 135.

2026-10-11 inline final review (uncommitted changes against HEAD): Standards pass, Spec pass for implemented behavior; the real-run duration criterion awaits worker completion. Reviewed tools/permuter_farm.py, its Docker launcher and tests, shared permute.py parser changes/tests, and wiki usage. All 64 combined tests passed in Docker (29.681 seconds); git diff --check passes. No unresolved findings, no source/SDK declaration or compiler flag changes, no new emulation pass, no copyrighted outputs staged. Reviewed four verified seed exports: RPG_BAT func_80138170 (unsigned signed-selector view), RPG_BAT func_8014FDBC (conservative pointer/integer view), TACO func_801522B0 (redundant casts/ranges/goto), and plain C TT func_80133970. Specific FAKE notes retained where needed; bodies/verified layout unchanged. Ticket moved to In Review while the requested hour finishes.

## Final results

Four CPU-only Docker workers ran for 3,655.075 seconds (60.92 minutes including
preparation and bounded shutdown). Queue snapshot: 1,090 scoped candidates. The
hour completed 25 attempts on 21 functions; the complete journal has 28 attempts
including three earlier helper failures. Final state: 4 won, 13 hopeless, 1,073
pending, no running claims. All four worker containers exited; only the task-owned
monitor was stopped afterward.

All wins are recovered score-zero C drafts, verified by `funcdiff --resolve` and
the full unit sha1. No new random-mutation win was found.

| Unit | Function | Seconds including setup/verification |
|---|---|---:|
| RPG_BAT | func_80138170 | 60.066 |
| RPG_BAT | func_8014FDBC | 114.300 |
| TACO | func_801522B0 | 48.324 |
| TT | func_80133970 | 47.141 |

Every attempt's duration, budget and best score is recorded in
scratchpad/r9-farm/report.md and report.json. The hour-only summary is
scratchpad/r9-farm/hour-summary.json. Actual attempt durations ranged from 9.128
to 1,220.312 seconds; full-budget searches generally took about 1,210-1,220 seconds.
Wins and individual verification/review JSON files are under
scratchpad/r9-farm/wins/<unit>/<function>.c.

Final inline review also removed early exit after the first verified zero, so
**every** score-zero output is checked; added a regression proving later outputs
are verified even after a win. All 65 combined tests passed in Docker (35.055
seconds). The real run encountered only one zero per winning seed, so its outcome
is unchanged by that correction. Standards and spec review pass with no unresolved
findings. Ticket/card moved to Done after review and the completed real run.

No live src/ changes, commits, stashes, branches, host dependency installations or
raw source modifications. All changes remain uncommitted for the orchestrator.
