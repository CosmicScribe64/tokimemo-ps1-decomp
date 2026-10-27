---
id: T-29030
title: Unify native route identities and executable capabilities
status: In Progress
assignee: root
created: 2026-10-26
updated: 2026-10-26
links: ["[[phase3]]", "[[port-testing]]", "[[port-readiness-review]]"]
---

## Goal

Remove disagreement between native mode routing, overlay dispatch and content capability checks without changing the proven Session ordering or treating a registered callback as a playable route.

## Acceptance criteria

- [ ] Audit native mode IDs and reachable consumers against original dispatch/source; distinguish legacy placeholders from implemented handlers.
- [ ] One authoritative mode identity registry is consumed by routing and presentation; route capabilities additionally require actual validated services/content.
- [ ] Auto-exit or skip handlers cannot advertise implemented gameplay merely because they are bound.
- [ ] Regression tests reject conflicting identities, missing services and unsupported branches; source-backed existing modes retain behavior.
- [ ] Preserve input, RNG, clocks, save and service ownership; no broad core rewrite or invented fallback behavior.
- [ ] Independently review the finite implementation, then replay affected actual routes before acceptance.

## Evidence

The current architecture review under T-29029 identifies duplicate router/dispatcher mode tables and capability binding as a reuse bottleneck. The legacy telephone profile mapping differs from the verified native mode. Full report: `.claude/orchestrator/state/T-29029/architecture-review-v1/`. This ticket is a focused follow-up; it does not block independent New Game service fixes or authorize replacing the running replay's pinned runtime.

## Reviewed finite registry applied (2026-10-26)

Root applied five exact afterimages following independent current-runtime review and original 68 dispatch/48 loader-pair checks. Actual canonical Docker registry230 and original-calendar month-start530 checks pass cleanly. Current production identity is `e7b4e2b732978d343c1d559b1430ac9731dea096b2bd232e5f01a4ca7cf2443a` (99 files). Legacy input-ignoring overlay auto-binding and direct date-preview injection are removed; unsupported events now stop transactionally. Actual route acceptance remains open. New Game must explicitly compose reviewed loader/name-entry capabilities into this registry rather than bypass it. Root proof: `.claude/orchestrator/state/T-29030/registry-v1/root-live/`; independent report: `.claude/orchestrator/state/T-29030/standards-review-v1/report.md`.
