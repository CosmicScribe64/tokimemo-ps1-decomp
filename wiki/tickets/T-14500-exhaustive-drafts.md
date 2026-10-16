---
id: T-14500
title: Draft and score every remaining game function
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[exhaustive-drafts]]"]
---

## Goal

Inventory every configured game INCLUDE_ASM, retain guarded drafts, generate missing drafts and score each using the default full translation unit. Keep failures and reproducible source/header context. No unverified draft enters production.

## Acceptance criteria

- Every inventory entry has a bounded attempt and explicit outcome.
- Draft context, declarations, diagnostics and hashes are retained.
- Zero scores require later literal linked proof and affected checksum before promotion.

## Comments

2026-10-15: Isolated branch from 48362009; immutable generated inputs hard-linked from integration to avoid duplicated disc images.

## Publishing checkpoint

All 1,066 default attempts are complete (29 zero candidates, 908 other scores, 129 explicit failures). Initial artifacts are immutable; 33 independent source/type repairs and completed automatic alternatives are joined separately. The final target remains usable reconstructed C for every remaining function. This ticket stays In Progress while compiler failures and residual synthetic-expression flags remain. A live serial repair pass continues through bounded fallbacks; no unverified draft entered production.

## User-requested stopping point

Stopped after the existing final no-unknown-inference pass, with all 1,066 initial scopes accounted for and 257 automatic attempts preserved. Fifty-two initial failures are now compiling alternatives (33 manual plus 19 automatic). Final quality: 849 compiling/pattern-audit-clear, 140 compiling/review-needed, 77 uncompilable; no absent scope. Ticket remains In Progress because usable reconstruction for every function is incomplete. See [[exhaustive-drafts]] for restart commands and exact residual ledgers. No compiler work remains running.
