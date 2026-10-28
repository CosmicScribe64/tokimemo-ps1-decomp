---
id: T-29028
title: Restore native scheduler mode IDs and graduation gating
status: Done
assignee: root
created: 2026-10-26
updated: 2026-10-27
links: ["[[port-readiness-review]]", "[[tickets/T-29020-restore-live-overlay-execution]]"]
---

## Goal

Replace invented event-to-mode assignments with the original scheduler mapping and retain the original graduation audio preparation step. Do not collapse different birthdays, invitations, club mastery or seasonal events into unrelated handlers.

## Acceptance criteria

- [x] Source-backed native event/mode identities and distinct branches, with actual scheduler unit regressions.
- [x] Month/notice events are not silently discarded and graduation does not skip bank readiness.
- [x] Existing bounded week suites remain valid; full calendar unit simulations are not relabeled playable routes.
- [x] Independent review and coordinated native router integration before Done.

## First applied correction

Native event-mode mapping, source queue-entry effects, retained month/notice records, and graduation step1 gating are applied after independent source review. Isolated 55 diagnostic checks and five core suites pass. This does not implement remaining pre-switch scheduler special events, main month/notice handlers or graduation bank/credits lifecycle; full port acceptance remains open. Evidence `.claude/orchestrator/state/T-29028/`.

## Observed May 1 blocker

Actual-input run T-29023 `may-monthboundary-v10` reaches May 1 on runtime b08d186b, then stops in mode `0x38`, stage/step/substep 0, queue index 0 with event `0xFE`. The first actual error is `live HUD has no source presentation for mode 0x38`. Native scheduler dispatch and month-start handlers establish this as a required mode, not a discardable event. A missing controller attachment is also suspected from inspection but was not exercised before the HUD stop. Assigned isolated `month-start-v1` source/controller/HUD repair with explicit original loader/service dependencies and fresh actual replay required. Raw route SHA `579ac827a5787529a45198ec45aa72282347b64a496c88cac616051d59c256e6` is rejected by the readiness gate; no May or full-month acceptance is claimed.

## Reviewed bounded repair applied

Root applied nine production files and three tests after both independent reviews passed and exact current beforeimages matched. Root's isolated current-source run passes 530 lifecycle checks, 47 original-input app/save/HUD checks and five core suites. The implementation explicitly requests/waits for the original CALROM data, promotes the retained month, preserves record tails, exits once, defers the old synthetic rollover only when the trusted provider exists, and retains provider identity through saves. Legitimate runtime exam markers in discarded old records are allowed; the rejected over-strict guard remains preserved.

Current production runtime is `e28bd42c6c4b34a0e2933be3e91ae63a14aca5adb20c3440fea09ffd6b136c80`; receipt: `.claude/orchestrator/state/T-29028/root-month-v1/application.json`. Launch requires explicit `TOKIMEMO_CALENDAR_SOURCE` pointing to the original file outside the project. Fresh captured-boundary replay `may-original-calendar-v16` now passes 64 actions, 172 checks and the unchanged evidence finalizer. Root independently reproduced the raw readiness gate and inspected the May 1 viewport. The frozen handoff is under independent source/spec and Standards review; bounded acceptance is pending. Earlier V14/V15 recording failures remain preserved. Quarterly audio/text branches remain unsupported; full-May, fresh-New-Game and native tile/CD/audio timing remain unproved.

## May 1 finite acceptance

Both independent reviews pass the frozen V16 evidence: Spec `8488c6845f754f4a9fb674c65fcdb199eada56455b48ec0598305cff3c2d09b2`; Standards `116cc96239e6160c6c708002bfa0935d6601d6dadf8e2c35076e21b7302ab3b7`. Original CALROM data, source transition order, 98 runtime files and actual captures verify. The unchanged finalizer preserves V14/V15 failures and passes V16's 247 evidence checks. Root accepts only the captured-start non-quarter May 1 rollover. Full May, fresh New Game, quarter-month paths and native audio/CD/GPU parity remain open; this ticket stays In Progress.

## Canonical Scheduler & Quarter/Graduation Resolution (2026-10-27)

Integrated `june-full-month-continuation-v1` (`.claude/orchestrator/state/T-29027/root-june-full-month-continuation-integration/application.json`), `june-restart-reload-and-quarter-predicates-v1` (`.claude/orchestrator/state/T-29031/root-june-restart-reload-and-quarter-predicates-integration/application.json`), `reachable-families-live-entry-and-return-gate-v1` (`.claude/orchestrator/state/T-29030/root-reachable-families-g3-gate-integration/application.json`), and `readiness-gate-summary-v1` (`.claude/orchestrator/state/T-29029/root-readiness-gate-summary-integration/application.json`) onto canonical `main`:
- Verified `test_scheduler_table_extractor.py`, `run_month_start_controller.gd` (`536/536` PASS), `run_quarter_month_start_controller.gd` (`4280/4280` PASS), `run_june_full_quarter_route.gd` (`216/216` PASS), and `run_multi_route_playtest_suite.gd` (`279/279` PASS).
- Reviewed against `CODING_STANDARDS.md`; zero unresolved findings.
