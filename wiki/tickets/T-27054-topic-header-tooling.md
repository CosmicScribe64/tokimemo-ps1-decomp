---
id: T-27054
title: Support canonical topic headers in header tooling
status: Done
assignee: parallel-header-chat
created: 2026-10-24
updated: 2026-10-24
links: ["[[phase2]]", "[[tickets/T-27000-phase-2]]"]
---

## Goal

Implement and review check_headers/sync_protos support for the three-header scheduler/day-event/school-day pilot, preserving existing behavior and deliberate ABI views. Work in ignored scratch until primary integration.

## Acceptance criteria

- [x] Concrete evidence-backed output matching this scope.
- [x] Independent review findings resolved and appropriate Docker proofs recorded before Done.
- [x] No false phase2-completion claim; wiki/tracker and original game data preserved.

## Notes

Primary chat owns source/header applications and shared wiki/commits; work follows the reviewed header-split pilot.

## Comments

2026-10-24: Created for the user-prioritized structural work/final review gates.

2026-10-24 scratch implementation checkpoint: baseline originals pinned from HEAD `2f0a3cef59f73bfdf2c8fce81cd5da9e6538842b`
under `.claude/orchestrator/state/T-27054/base/tools`; modified copies only under `work/tools`.
Explicit registered owners are main_api plus `main/schedule.h`, `main/day_event.h`, `main/school_day.h`;
no helper or config file added. Scratch check_headers suite passes 23 tests, existing rename suite
passes 76; baseline sync suite passes 76. New topic fixtures and strict compare support are in review.
The actual-source scratch-copy probe successfully preserved views during the exact 63-declaration
move, then found two risky changes after --fix; investigation is active. Independent review also
found a partial-mutation error path for multiline override guards, which must be fixed before handoff.
Live tools/src/include/config remain untouched; all runtime tests use Docker with --cpus=2 and the
repository mounted read-only. No build or new checksum claim.

2026-10-24 reviewed handoff: unified patch `.claude/orchestrator/state/T-27054/topic-header-tooling.patch`,
SHA256 `167a8eb2ac3e21659e230fa71602731b3da418bb88cb9f3f1f02a01a44dae57f`, scoped to check_headers,
sync_protos and their two existing synthetic suites; no new helper/config. `patch_manifest.json`
contains baseline/reviewed hashes and confirmed all four live files still equal the frozen originals.
Independent Sol 6.1 review passed 118/118 (95 sync/23 headers), with all findings resolved. Additional
scratch rename 78 and dupes/aggregate/srcscan/bulk 159 tests pass, 355 total across those runs.

The final actual-copy probe at HEAD `2298a9b2fe2dedd3842d5e3c62d7ce43400bd4e1` uses root's logical
`scheduler.c`/`school_day.c` paths; all 63 topic declarations remain owned (32/5/26), all 34 prototype
omissions and six GYOZI compatibility lists are preserved, and both extraction and subsequent --fix
compare exactly 0 benign/0 check/0 risky. Header checks are clean, non-pilot API declarations and all
source C bytes remain unchanged. Results: `T-27054/pilot_probe_result.json`; these are scratch-copy
proofs, not a live application or game build. The failed earlier probe was resolved by retaining
deliberate benign override declarations. The independent multiline-guard partial-write finding was
resolved with complete declaration guards and transactional exact-byte rollback plus regression tests.

Root applies the reviewed patch, then extracts the actual 63 declarations with a fresh snapshot and
`--compare <snapshot> --strict-compare`, guards and clean 27/27/6,952 proof. Ordinary --compare and
legacy-only umbrella workflows retain their prior behavior; --fix's success alone does not prove
zero benign changes. Full umbrella deletion remains outside this bounded pilot. No live tools/src/
include/config change, game build, commit or final Astra launch occurred in this lane. Root owns
shared log/registration and ticket/board transition; worker leaves status In Progress to avoid a
kanban mismatch. Implementation output is complete and reviewed, ready for primary integration.

2026-10-24 primary integration: applied the reviewed four-file patch after git apply --check; verified exact patch SHA, all four committed baseline SHA values and all four reviewed-result SHA values. The first path-filter expected git-style diff headers; corrected validation for the handoff's standard unified ---/+++ headers before accepting results. Root integrated tests pass23header+95prototype cases; live T27058extraction then confirms63canonical owners,437unchanged Cfiles,34omissions and0/0/0views. Broader worker355test/probe/review evidence is retained in ignoredmanifest. Tooling is now live, not merely waiting in scratch; final root review remains pending.

2026-10-24 closeout: integrated implementation and independent Standards/Spec reviews pass; all findings resolved. T-27044 has clean27/27 plus unchanged path-mapped declaration views; T-27054 has exact reviewed manifest integration and23header/95prototype tests; T-27057 has78rename tests and both reviews; T-27058 has63exact declaration moves, all437views0/0/0, clean27/27 and post-review27/27 with6952functions. The two T-27058 evidence/direct-include findings were fixed in0be7fbcb7 and independently verified. Broader phase2 remains In Progress.
