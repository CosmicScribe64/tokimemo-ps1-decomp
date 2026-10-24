---
id: T-27027
title: Phase 2 naming and port spec for overlay RPG_BAT (RPG battle scene)
status: In Review
assignee: p2a-o-rpgbat (Sonnet)
created: 2026-10-24
updated: 2026-10-24
links: [[[tickets/T-27000-phase-2]], [[spec/o-rpgbat]]]
---

## Goal

Roadmap phase 2 for the RPG_BAT overlay, port-first scope: name and explain the game logic (state, rules, flow, data tables), name the hardware layer only at module level, write the scene-flow spec. Tables and wiki only; no change to `src/`, `include/` or symbol files.

## Acceptance criteria

- [x] `config/renames/proposals/o-rpgbat.tsv`: one row per rename (func, data, field, type, const, file) plus comment rows for the O.BIN names the overlay uses; one name per (unit, address); game-meaning guesses start with HYPOTHESIS.
- [x] `wiki/spec/o-rpgbat.md`: entry and launch codes, stage table, channel model, start values, damage and AI rules, girl behaviors, end conditions, data tables, connections, open questions; linked from `wiki/spec/index.md`.
- [x] Every one of the 328 functions and 61 source files has a proposed English name.
- [ ] Review of names, evidence and hypothesis marks (Opus), then the rename tool run (plan, apply, 27/27 build).

## Notes

- Prefix: `rpg_battle_` (English form of the overlay name, as `sports_festival_` for TAIIKU); the audit's `rpg_bat_` is the shorter alternative. Types keep the existing `Rpg` prefix. Enum constants: `RPG_*`; the shared enums are core's `Club` and `GirlId`, which this table does not redeclare (the only local addition is `RpgPartner` 14, HYPOTHESIS). Trip encounter names use the English animals: snake, deer, bear.
- Evidence came from the matched C, its callers (DATE, GEKO, SHUGAKU) and the main tables for clubs and girls. The emulator was not used: reaching the scene needs a save at a trigger point and `tools/emu` has no memory poke.
- Local names of locals and the display-record field names were left alone (graphics unit).
- Counts in the table: see the log entry.

## Comments

- 2026-10-24 review fix pass: launch codes 4 and 5 rewritten from `func_8013C7D0` (D_800CA2F8 is the dating place, not a girl; code 5 clauses completed); `ClubId`/`GirlId` blocks and 15 duplicate `comment main` rows dropped; `RpgBattleState` field rows use the old type name `RpgStat`; counter `D_8015EC34` renamed `rpg_battle_escalation_counter` (the no-club special writes the tier back); unproven 'park' dropped; habu renamed snake; `stage_next` now `main_stage_next`; stage 14, state 0x104, knockdown gate, aliens pool and boss round weights corrected in the spec. Open for the orchestrator: core `PadBits` conflict (see the spec's open questions).
