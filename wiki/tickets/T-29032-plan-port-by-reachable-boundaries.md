---
id: T-29032
title: Plan port implementation by reachable boundaries
status: In Progress
assignee: root
created: 2026-10-26
updated: 2026-10-26
links: ["[[phase3]]", "[[port-testing]]"]
---

## Goal

Expose known unsupported branches before long gameplay runs and order implementation around connected player milestones. Do not replace actual gameplay or original differential testing with a static checklist.

## Findings

- June1 was predictable: `month_start_controller.gd:120` rejects months divisible by3. This is a known missing branch, not an unexpected regression. May1 coverage never implied quarter coverage.
- The plan already required branch coverage and fail-closed runtime behavior, but orchestration did not turn those requirements into an actionable upcoming-boundary dependency list early enough.
- Repeated command0 weekly runs validated saves and continuity but had diminishing discovery value for different choices, calendar branches and route families.
- Audio first-use and native-extension editor qualification should precede substantial dependent launch work. Runtime-only tests did not expose editor initialization failures.
- Image checks reproduced the generation template instead of independently validating source menu composition; the same specification error could pass both renderer and checker.

## Required changes

- [ ] Add a bounded implementation-gap preflight alongside existing readiness tooling, with source predicates/locators, route case, dependency, owner/ticket, implementation state and evidence state. Unsupported and untested remain distinct.
- [ ] Start with month boundaries: ordinary, quarter, September notice, year rollover, and vacation-dependent bank selection. Bind original source rather than extrapolating June to every quarter.
- [ ] Before each long run, state known expected stops and missing services. Run targeted original-backed branch regressions first; preserve actual long-run verification afterward.
- [ ] Maintain a connected milestone chain: original import → New Game entry → opening/dialogue/audio → normal command loop → calendar/event branches → route/ending → reload/package/platform verification.
- [ ] Qualify shared native dependencies with a minimal editor import, runtime load, real output and lifecycle failure probe before expanding their consumers.
- [ ] Independent review checks the specification/context, not only generated templates or producer-declared pass counts.
- [ ] Reuse exact reviewed saves for short regressions; reserve full startup/long routes for integration milestones and final acceptance.
- [ ] Review this policy and the preflight implementation before marking this ticket Done.

## Limits

Static searches can miss indirect dispatch and data-driven branches. A clean preflight does not establish completeness, gameplay reachability, native parity, or packaged usability.

## Installed bounded preflight

V2 independently accepted at `.claude/orchestrator/state/T-29032/month-preflight-review/acceptance-v2.md`, installed in `tools/port/preflight.py`, `tools/port/test_preflight.py`, `port/verification/month-boundaries.json`. Root13testsPASS. InventorySHA29e131d26829b7cc18ae3b0100cc6a62462bceaf4ff0f87b0679eb90ed0dcfe7. Explicit June1 CLI selects known unsupported before gameplay; exit2 is expected and cannot count as success. Historical May95 scope is recipe/date/runtime-specific; generic ordinary month remains untested. Broader boundary coverage and execution of this policy across remaining routes keep this ticket In Progress.
