---
id: T-12800
title: Large EVENT TEL and main matching
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[decompile-workflow]]"]
---

## Goal

Recover byte-identical C in the assigned files; prioritize largest functions using full-TU evidence.

## Acceptance criteria

- Resolved full-TU byte proof and affected binary SHA1 for each win.
- Review against CODING_STANDARDS.md; final 27 checks.

## Notes

Isolated branch codex/t12800-large-event-tel, origin/main 95c3fc9f. No changes to original checkout or other workers. Parent T-12800; scope main/8006CB30.c, EVENT/8010FE10.c, TEL/80132000.c. At most two concurrent compilers.

## Comments
