---
id: T-12903
title: Match small OPTION audio handlers
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-12900-small-option-main]]"]
---

## Goal

Bounded work on the 132/200-byte guarded handlers in src/ovl/OPTION/801389A0.c, discovered in the refreshed queue.

## Acceptance criteria

- Literal linked-byte proof and OPTION SHA1 for enabled C.
- Review CODING_STANDARDS.md before Done.

## Comments

- 2026-10-15: Queue reveals two small candidates outside the old cx6 file list but within the assigned OPTION ownership. T-12902 saved drafts did not immediately match after canonicalization; park difficult register families.
