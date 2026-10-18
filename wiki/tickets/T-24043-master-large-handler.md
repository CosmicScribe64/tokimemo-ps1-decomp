---
id: T-24043
title: Recover larger MASTER final-event handler
status: In Progress
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[master-family-results]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Recover MASTER func_80139CC8 in src/ovl/MASTER/80139000.c, approximately 2,748 instruction bytes, using the established real display-record types and current family source evidence.

## Acceptance criteria
- Read old family/probe results before new work; bounded novel source pass near 25 minutes.
- Preserve completed 87e08d84b ancestry in reused built /private/tmp/tokimemo-complete-etc on codex/complete-master-large; no reset or new checkout.
- One GPT-6.1 Sol medium compiler total, Docker only, ninja -j1. No other source/config/header edits without prior notification.
- Fix semantic/type issues before allocation experiments; retain useful source-only corrections and concrete blockers if unmatched.
- Actual C wins remove target guard/INCLUDE_ASM before explicit normal-default linked proof, checksum and all guards; send pinned wins promptly. Root owns review, all-27 checks, integration and push.

## Prior evidence
T-18600 typed draft improved 885 to 678 rows using real display records and rerolled constant loops; broad redraft alone did not match. Existing types: DisplayFixActor44, Fix32, u8 D_8013C788[5] (element 4 was D_8013C78C), u8 D_8013C780 and s16 D_8013C794. matching-notes older correctness findings: a swap uses two distinct LO targets under one shared high relocation; m2c incorrectly emitted the fifth-byte target for both stores. Repair actual array-byte semantics before tuning registers. Older investigation suggested data ownership might be needed for this one shared-high swap; no config/data-island change is authorized merely by that old suggestion, report concrete need first.

## Comments
Parent confirms both T-24038 wins reviewed and staged for combined verification, not yet published at assignment. T-24030 BUNKASAI is published at d3965d02e; T-24031 archive is published at 8cc5fe522. Preserve all completed source bodies, headers and proof provenance; only the newly assigned large MASTER target is claimed.
