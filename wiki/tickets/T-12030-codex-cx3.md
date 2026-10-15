---
id: T-12030
title: Match cx3 main menu functions
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]"]
---

## Goal

Deep matching pass on the 15 assigned functions in src/main/8005A0B0.c.

## Acceptance criteria

- [ ] Each retained match has exact linked-byte and unit SHA1 verification.
- [ ] Retained near misses are guarded, counted and documented.
- [ ] Final configure/build, headers, globals and prototype checks pass.
- [ ] Code review against CODING_STANDARDS.md before Done.

## Notes

Baseline Docker configure and ninja passed all 27 SHA1 targets (13 before restarting with -j2 and 14 after). Logs: /tmp/cx3-baseline.log, /tmp/cx3-baseline-resume.log. Only the assigned C file, include/config and wiki may change. No main-checkout edits.

## Comments

- 2026-10-15: claimed package cx3; baseline verified.
