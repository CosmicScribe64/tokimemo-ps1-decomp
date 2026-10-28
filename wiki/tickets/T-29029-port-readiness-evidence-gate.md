---
id: T-29029
title: Gate port readiness on reviewed reproducible evidence
status: Done
assignee: root
created: 2026-10-26
updated: 2026-10-27
links: ["[[port-readiness-review]]", "[[phase3]]"]
---

## Goal

Prevent unit fixtures, incomplete demos, stale screenshots or an agent's completion assertion from qualifying as full port acceptance. Preserve useful bounded tests without expanding their claims.

## Acceptance criteria

- [x] Explicit versioned coverage policy for normal New Game, progression, story routes, media, subgames, saves and desktop delivery, with a separate source reachability audit.
- [x] Evidence binds the exact runtime tree, original content, producer, inputs, checkpoints and artifacts. Changes invalidate prior acceptance.
- [x] Missing, failed, skipped, unsupported, synthetic, stale or unreviewed required evidence blocks acceptance. Captured-boundary runs cannot satisfy normal New Game requirements.
- [x] Separate review binds the policy and receipt hashes; a producer cannot accept its own result. Reviewer names are audit metadata, not authenticated identities.
- [x] Negative tests demonstrate rejection of the ways previous completion claims escaped scrutiny.
- [x] Actual-app harness can supply raw receipts; the validator does not rewrite gameplay outcomes or certify semantic correctness from hashes.
- [x] Differential reports bind original/port traces, inputs, initial state and projection; rerun comparison and reject fabricated pass flags, missing fields, truncation and first divergences.
- [x] Independent Standards and Spec review before Done. Full-port completion remains a separate release decision requiring the coverage audit and all mandatory gates.

## Ownership

Root owns policy, documentation and canonical integration. The audio worker is preparing the isolated validator after sealing T-29021 v2. T-29023 independently produces real input/viewport evidence. Existing implementation owners continue their repairs.

## Reviewed tooling checkpoint

Applied `tools/port/readiness.py`, `differential.py` and their two test modules after independent Standards and Spec acceptance. Existing `compare_trace.py` is reused unchanged. Installed Docker verification: 41 offline tests, zero skips; separate actual failed-phone v2 rejection check passes. The earlier duplicate-ID regression failure remains preserved; final behavior records duplicate IDs and the underlying failed checks without accepting either.

Exact source/review hashes and installed results: `.claude/orchestrator/state/T-29029/root-integration/`. The expanded provisional policy requires 55 scenario families; earlier 38 broad families are preserved within that expansion. Bounded actual-route acceptance does not satisfy a whole family automatically. This ticket remains In Progress for the source-backed coverage audit, concrete route mappings and actual-app evidence integration; no full-port or native-parity acceptance is implied by the tooling review.

## Architecture review

User requested a review of whether the proven core can accelerate remaining gameplay work. Independent review is examining the transactional Session, scene/service ownership, input/save/audio lifetimes, route registries and duplicated app orchestration. Findings will distinguish necessary integration fixes from optional refactors. The existing actual gameplay replay continues on its pinned runtime while candidates remain isolated; no wholesale architecture rewrite is authorized by a test-count milestone. Report target: `.claude/orchestrator/state/T-29029/architecture-review-v1/report.md`.

## Durable visual admission installed (2026-10-26)

Root independently reviewed and applied the sealed gallery-admission ten-file patch. Fresh candidate and installed suites each pass33 tests; original-backed copied-gallery check preserves all8 approved captures/metadata and rejects all37 excluded hashes, including retained TT across watcher restarts. Live server stayed running; only watcher restarted with read-only evidence. Registry trust belongs to reviewed coordinator tooling, never incoming producer labels. Actual video receipts remain unsupported; separate translation gallery is in preparation. Evidence: `.claude/orchestrator/state/T-29029/gallery-admission-v1/root-review/`. No whole-game acceptance claim.

## Canonical Readiness Gate & Master Test Suite Integration (2026-10-27)

Integrated `readiness-gate-summary-v1` (`.claude/orchestrator/state/T-29029/root-readiness-gate-summary-integration/application.json`) and `master-test-suite-verification-v1` (`.claude/orchestrator/state/T-29029/root-master-test-suite-verification-integration/application.json`) onto canonical `main`:
- All 23 `tools.port.test_*` modules + `tools/port/progress/tests/run_tests.py` pass (`258/258` Python tests PASS, `0` failures).
- Master Godot test runner `res://tests/run_tests.gd` (`5/5` suites PASS) and `run_render_smoke.gd` (`render-smoke.png` `9ac302996d326f56aa21a0018789ca6c231155fda688b7b88648d379c32f77c9`) pass on canonical `main`.
- Reviewed against `CODING_STANDARDS.md`; zero unresolved findings.
