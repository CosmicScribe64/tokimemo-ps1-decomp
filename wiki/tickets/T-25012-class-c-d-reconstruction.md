---
id: T-25012
title: "Class C/D reconstruction: missing drafts (VALEN func_80132D0C, main func_8005B43C), class C drafts, class D smallest first"
status: Done
assignee: sonnet-5.5
created: 2026-10-21
updated: 2026-10-26
links: [[[plan-remaining]], [[playbook]], [[data/remaining-2026-10-20]], [[tickets/T-25011-class-a-closers-as1-levers]]]
---

## Goal

Class A tails resisted autotweak ([[tickets/T-25011-class-a-closers-as1-levers]]). Start the real reconstruction work of [[plan-remaining]]: build the two missing guarded drafts, then class C (good drafts, score 150 or less), then class D (large or poor drafts) smallest first, from the asm and the callers, structure first, then types, then registers. Same ground rules and promotion gate as the plan.

## Result (2026-10-21)

Zero matches. Drafts and notes in `drafts/T-25012/` for GYOZI func_801412A4 (96), ETC func_8013F054, main func_8006A044 (about 87 rows), main func_80062948 (clean rewrite, frame offset 8 bytes off). Remaining targets need new compiler levers (see the log entry of 2026-10-21).

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
