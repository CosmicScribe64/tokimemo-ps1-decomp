---
id: T-27057
title: Avoid redundant header-closure scans in rename proofs
status: Done
assignee: orchestrator
created: 2026-10-24
updated: 2026-10-24
links: ["[[rename-tool]]", "[[tickets/T-27042-enum-rename-support]]"]
---

## Goal

Avoid evaluating Tree.closure as an unused dict.get fallback when its cached closure exists. The new shared enum ledger exercises this per source/per enum chain; preserve proof semantics while removing repeated scans.

## Acceptance criteria

- [x] Cached empty/nonempty closures avoid fallback evaluation; unindexed C files still use fallback.
- [x] Docker rename regression suite and independent review pass; no game source changes.
- [x] Existing scope/check/proof behavior unchanged and logged before Done.

## Comments

2026-10-24: Found Python eager fallback evaluation in closure_files while real 69-row shared enum proofs amplified repeated scans. Existing cached results were already used; only the unnecessary fallback computation is removed.

2026-10-24 implementation: closure_files now uses an existing cached closure directly and computes Tree.closure only for a missing entry. Empty cached sets remain valid cache hits. Two regressions assert no fallback call for populated/empty cache entries and correct fallback for unindexed sources. All78rename tests pass in Docker. No game source mutation; independent review pending.

2026-10-24 closeout: integrated implementation and independent Standards/Spec reviews pass; all findings resolved. T-27044 has clean27/27 plus unchanged path-mapped declaration views; T-27054 has exact reviewed manifest integration and23header/95prototype tests; T-27057 has78rename tests and both reviews; T-27058 has63exact declaration moves, all437views0/0/0, clean27/27 and post-review27/27 with6952functions. The two T-27058 evidence/direct-include findings were fixed in0be7fbcb7 and independently verified. Broader phase2 remains In Progress.
