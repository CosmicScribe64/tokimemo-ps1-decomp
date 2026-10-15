---
id: T-13201
title: Match event selector handlers with direct case stores
status: In Progress
assignee: codex-events-13200
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-13200-event-family-transfer]]"]
---

## Goal
Transfer the ordinary switch-case assignment form to related event selector handlers.

## Acceptance criteria
- Full-TU candidate score, literal default linked byte proof and affected overlay SHA1.
- Preserve existing C and canonical aggregate fields.

## Notes
First bounded SHOUGATU 8013BDE0 probe: base215 rows, direct case stores0. Scratch build/events13200.

## Comments

SHOUGATU func_8013BDE0 accepted: 656 literal linked bytes with default build and SHOUGATU SHA1 OK (build/events13200/switch-proof.txt and switch-build.log). Two GYOZI siblings remain assembly: 39E4C196->247 direct->33 split flag stores; E824141->188 direct->24 narrow result/split mask. Default proof lines for those unchanged ASM functions are controls, not coverage gains. No global headers changed.
