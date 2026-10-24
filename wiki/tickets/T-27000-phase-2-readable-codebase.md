---
id: T-27000
title: "Phase 2: human-readable codebase"
status: In Progress
assignee: orchestrator
created: 2026-10-24
updated: 2026-10-24
links: [[[phase2]], [[tickets/T-27001-phase-2-conventions]], [[tickets/T-27003-pilot-obin-names-overlays]], [[tickets/T-27004-shared-core-names]], [[tickets/T-20500-emulator-assisted-naming]], [[emulator]]]
---

## Goal

Roadmap phase 2: make the matched code readable. Someone looking at `src/` and `include/` sees no functions,
files or headers named by number; symbols, fields, types, constants and locals carry names backed by evidence
(`docs/adr/0001-phase-2-naming.md`). Plan and work order: [[phase2]]. Every rename keeps the build at 27/27 sha1.

## Acceptance criteria

- [ ] Conventions decided and written: ADR 0001, CODING_STANDARDS sections 4 and 5, `GLOSSARY.md` (T-27001;
      tick when T-27001 is Done).
- [ ] Rename tool with ledger, replay and check (T-27002).
- [ ] O.BIN names reach the overlays; alias debt cleared (T-27003).
- [ ] Shared core named: GameState fields, core types, first enums, top 100 main functions (T-27004).
- [ ] Main modules M1-M9 and all 26 overlays named, each with English file and header names.
- [ ] Locals, parameters and constants named per unit.
- [ ] `include/main_api.h` and `include/game.h` deleted: every C file includes the topic headers it uses, and
      CODING_STANDARDS section 8a and `tools/check_headers.py` name the topic headers.
- [ ] Final sweep: `count_names.py` shows only placeholders the standards allow (unused or unknown fields,
      jump tables, optional SDK names); living wiki pages refreshed.

## Notes

Child tickets: T-27001 conventions, T-27002 rename tool (written separately), T-27003 pilot, T-27004 shared
core; unit tickets for steps 3-8 are created when a unit starts. The emulator ([[emulator]], T-20500) supplies
runtime evidence.

## Comments

- 2026-10-24 (w19-conventions): created; conventions in T-27001.
- 2026-10-24 (w19-conventions): review fixes. The first criterion is open again until T-27001 is Done; the
  deletion of the two umbrella headers is a criterion of its own (final sweep, step 8 of [[phase2]]).
