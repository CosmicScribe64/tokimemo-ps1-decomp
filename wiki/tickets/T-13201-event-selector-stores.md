---
id: T-13201
title: Match event selector handlers with direct case stores
status: Done
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

Source work complete; two-axis review and failed probes summarized in [[event-family-results]]. At that checkpoint final all27 validation was pending; now completed below.

Final review/closure2026-10-15: GPT6.1Sol medium Standards and Spec reviews complete; all findings resolved. Final default literal proof656+1328+1440=3424 bytes, independent27/27 binary hashes, headers/globals and prototype branch check pass (0new/15known). All744 baseline bodies unchanged. Evidence: build/events13200/final-proof.txt, final-hashes.txt, all27-build.log, final-protos.txt, existing-c-audit.txt and progress.txt. Final closure review confirms six particle negatives add no production code or coverage. Ticket Done; broader80% objective belongs to integration and is not claimed complete.
