---
id: T-0015
title: "Research: compiler mismatch handling"
status: Done
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

Code review (wiki-only change, inline gate, Standards + Spec; no sub-agents): no CODING_STANDARDS.md findings (no code, no game data, no copyrighted content staged). Spec: goal and all three acceptance criteria met. Wiki rules: every synthesized claim traces to the raw note (URLs present in the raw note; URLs were not re-fetched). Fixed on [[compiler-mismatch-research]]: NEWS-OS dev-kit and CES 1994 claims hedged as secondary/unverified; Evo's Space Adventures N64-IDO claim marked UNVERIFIED (no URL); "gxemul-class" emulator and "16 bytes never accessed" marked UNVERIFIED. `wiki/raw/` left unchanged (immutable). Log, index and kanban checked for wikilink rule and consistency. All findings resolved; Done.
