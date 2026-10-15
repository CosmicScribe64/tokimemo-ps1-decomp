---
id: T-11910
title: Research external matching acceleration techniques
status: In Review
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[external-matching-research]]"]
---

## Goal
Identify externally supported improvements beyond existing matching tools, preserving full-translation-unit and literal linked-byte verification.

## Acceptance criteria
- Prioritized primary-source findings, compatibility limits, and bounded Docker experiments.
- No changes to game code, compiler defaults, or other workers' checkouts.

## Notes
Dedicated external-matching-research worktree. Main checkout is read-only. No throughput claims without local measurements.

## Comments

2026-10-15: Research and synthetic Docker checks complete. See [[external-matching-research]] and raw/external-matching-research-2026-10-15.md. No production code or game files changed. Checks: public positive and negative diagnostic fixtures, 31 passing MIPS reference tests / seven private-image skips, synthetic AS1 trace text-fidelity check. Independent review pending; status In Review.
