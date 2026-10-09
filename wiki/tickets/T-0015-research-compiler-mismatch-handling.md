---
id: T-0015
title: "Research: compiler mismatch handling"
status: In Review
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[matching-notes]]", "[[toolchain]]", "[[tickets/T-0100-older-mips-compiler-emulation]]"]
---

## Goal

Find how early Japanese PS1 developers (Konami in particular) compiled, which MIPS compilers add 16 bytes to frames, and how established matching decomps treat systematic compiler-output mismatches, so the project can choose between a post-processing pass and an older-compiler emulation for the +16 frame.

## Acceptance criteria

- [x] Raw source note in `wiki/raw/` with a URL for every claim and unverified items marked.
- [x] Synthesized page [[compiler-mismatch-research]] with a ranked recommendation.
- [x] [[index]] and [[log]] updated; card moved on [[kanban]].

## Notes

Research only, no code changes, no downloads.

## Comments

Result: [[compiler-mismatch-research]], raw note `raw/compiler-mismatch-research-sources.md`. Ranked recommendation: (1) documented, uniform frame-rewrite stage in tools/cc.py (maspsx-style emulation), (2) time-boxed Ultrix/NEWS-OS compiler test under T-0100 (needs user decision on OS images), (3) leaf-only decompilation meanwhile. Awaiting code review before Done.
