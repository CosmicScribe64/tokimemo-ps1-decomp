---
id: T-16990
title: Wave 3 conveyor (types-first passes feeding a matcher pool)
status: Done
assignee: claude-orchestrator
created: 2026-10-16
updated: 2026-10-17
links: [[speedup-research-2026-10-16]], [[tickets/T-16200-matching-wave-2]], [[taiiku-family-results]], [[taco-family-results]]
---

## Goal

Clear as many of the remaining 831 non-TT functions (725 KB) as the weekly usage allows, using what the pilot showed
(T-16550, T-16560: 64 matches / 41.4 KB in about 20 agent-hours, mostly Sonnet).

## Plan

- Types-first pass (Opus) on 19 units: main, RPG_BAT, DATE, ETC, EVENT, SHOUGATU, GYOZI, GEKO, OPTION, NAME_ENT,
  SHUGAKU, BUNKA_SD, EN_NICHI, ENDING, KANGEI, BUNKAKEN, MASTER, BUNKASAI, DATE2 (tickets T-17000..T-18800, one block
  of 100 per unit; packages T-<block>+1..). Each recovers the unit's types, redrafts with the pilot drivers
  (`drafts/T-16550/redraft.py`, `drafts/T-16560/redraft.py`) and packages the unit deterministically.
- 32 direct packages (T-19001..T-19032) for TAIIKU/TACO leftovers and the small units (OLH, OMIMAI, RENSYU, TEL, VALEN).
- Pool of 7 workers (3 prefer types passes): Sonnet matcher (about 90 minutes), Opus closer for near misses of 20 rows
  or fewer, Sonnet review that runs the locked, self-cleaning merge script (clean 27/27, push to main).
- Usage gate before each task: wait at 5-hour 80%, stop new work at weekly 85%.

## Acceptance criteria

- [x] Every started package reviewed, merged or dropped, ticket Done
- [x] Integration tree clean 27/27, headers, globals, prototypes
- [x] Results and blockers recorded here

## Comments

### 2026-10-17: results, paused for a type round and a tooling round

User decision: pause the conveyor, run a cross-unit type round and a tooling round, then restart. New packages were
refused at setup (pause flag in the orchestrator's setup script); the seven running packages finished and merged.

- 19 types-first passes: 69 matches / 39,460 bytes; 126 packages produced.
- 77 packages worked (Sonnet matcher, Opus closer on near misses of 20 rows or fewer): 170 matches / 127,844 bytes,
  27 of them from the closer; every worked package reviewed and merged.
- 81 packages not started (pause); they return to the next inventory.
- Total 239 matches / about 167 KB; project 6,076 -> 6,358/6,958 functions, 67.2% -> 75.8% of bytes (contributor PRs
  included in the totals).
- Blockers: 102 notes, clustered by the tooling round (T-20800..); compiler-limited cases in [[build-step-gaps]].
