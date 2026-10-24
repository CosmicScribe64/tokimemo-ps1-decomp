---
id: T-27041
title: Apply readable scheduler and day-event names
status: In Progress
assignee: orchestrator
created: 2026-10-24
updated: 2026-10-24
links: ["[[phase2]]", "[[rename-tool]]", "[[tickets/T-27000-phase-2]]"]
---

## Goal

Reconcile and apply body-backed names from src/main/8006CB30.c, keeping the day queue, girl selection and event routing readable for the Godot port.

## Acceptance criteria

- [ ] Evidence and supported scope documented; no unsupported semantic claims.
- [ ] Meaningful Docker validation; applied game names preserve clean 27/27 hashes and 6,952 functions.
- [ ] Independent Standards and Spec review resolved before Done.
- [ ] Wiki and ledger/replay behavior updated as relevant.

## Comments

2026-10-24: Claimed for the sustained phase 2 continuation.

2026-10-24 pre-application review: independent Sol 6.1 checked all 40 target names, bodies/callers, six data globals, core retention, prefixes and collisions; approved with one evidence-note correction (weekly HUD initializer is stage0, not step0). Corrected that plus three requested precision notes: at least two met girls, state mask0x4 and year>=98 date predicate. Reconciler plan passed 40rows/10files before these note-only changes. Root will apply through rename.py and run clean proof; no post-change hash claim yet.

2026-10-24 application: corrected plan exited0 (40rows/10files), apply exited0 with builtin proofs and renames OK(780ledger rows). Applied 34 function and6 data names, including all33 remaining func_ definitions in the scheduler object; retained existing core/O.BIN names. Living spec now uses applied ledger spellings and correctly describes last-picked-girl behavior (only consecutive repeats blocked). Clean Docker build is running; independent final code review remains pending.
