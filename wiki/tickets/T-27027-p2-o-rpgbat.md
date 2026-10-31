---
id: T-27027
title: Phase 2 naming and port spec for overlay RPG_BAT (RPG battle scene)
status: Done
assignee: p2a-o-rpgbat (Sonnet)
created: 2026-10-24
updated: 2026-10-24
links: `[tickets/T-27000-phase-2`, `spec/o-rpgbat`]
---

## Goal

Roadmap phase 2 for the RPG_BAT overlay, port-first scope: name and explain the game logic (state, rules, flow, data tables), name the hardware layer only at module level, write the scene-flow spec. Tables and wiki only; no change to `src/`, `include/` or symbol files.

## Acceptance criteria

- [x] `config/renames/proposals/o-rpgbat.tsv`: one row per rename (func, data, field, type, const, file) plus comment rows for the O.BIN names the overlay uses; one name per (unit, address); game-meaning guesses start with HYPOTHESIS.
- [x] `wiki/spec/o-rpgbat.md`: entry and launch codes, stage table, channel model, start values, damage and AI rules, girl behaviors, end conditions, data tables, connections, open questions; linked from `wiki/spec/index.md`.
- [x] Every one of the 328 functions and 61 source files has a proposed English name.
- [x] No edits under `src/`, `include/` or `config/symbol*` files (checked against w2-good before the merge).
- [x] Review of names, evidence and hypothesis marks (approved after the second fix pass) and merge of the table by the phase 2 merge step; the rename tool run (plan, apply, 27/27 build) is a later step.

## Notes

- Prefix: `rpg_battle_` (English form of the overlay name, as `sports_festival_` for TAIIKU); the audit's `rpg_bat_` is the shorter alternative. Types keep the existing `Rpg` prefix. Enum constants: `RPG_*`; the shared enums are core's `Club` and `GirlId`, which this table does not redeclare (the only local addition is `RpgPartner` 14, HYPOTHESIS). Trip encounter names use the English animals: snake, deer, bear.
- Evidence came from the matched C, its callers (DATE, GEKO, SHUGAKU) and the main tables for clubs and girls. The emulator was not used: reaching the scene needs a save at a trigger point and `tools/emu` has no memory poke.
- Local names of locals and the display-record field names were left alone (graphics unit).
- Counts in the table: see the log entry.

## Comments

- 2026-10-24 review fix pass: launch codes 4 and 5 rewritten from `func_8013C7D0` (D_800CA2F8 is the dating place, not a girl; code 5 clauses completed); `ClubId`/`GirlId` blocks and 15 duplicate `comment main` rows dropped; `RpgBattleState` field rows use the old type name `RpgStat`; counter `D_8015EC34` renamed `rpg_battle_escalation_counter` (the no-club special writes the tier back); unproven 'park' dropped; habu renamed snake; `stage_next` now `main_stage_next`; stage 14, state 0x104, knockdown gate, aliens pool and boss round weights corrected in the spec. Open for the orchestrator: core `PadBits` conflict (see the spec's open questions).
- 2026-10-24 second review fix pass: `himoo` identifier stems became `yuina` (the spec says once that Himoo is Yuina, girl id 2); the robot enums are `RPG_ENC_ROBOT` and `RPG_LAUNCH_ROBOT`; the `RpgRec18` field and type rows are dropped and `D_8015EC58` is `rpg_battle_move_params` with a per-use note (girl run, basketball dunk rectangle, art move); ten text-buffer rows with no code reference are dropped and the arrays are described in the first row of each; result constants are `RPG_RESULT_GOOD`/`_BAD` with the callers' branches as evidence and the aliens exception; `D_8015EC38..EC4C` are `rpg_battle_bg_word_0..5` (wave and aliens-scene uses), and the voice block, scratch and move-parameter aliasing is stated in the notes and in spec sections 4 and 6; the pad conflict text is gone (PAD_START 0x0800 is core's); spec section 7 uses core names with a placeholder map; stages 12 and 13 follow core's function names (`RPG_STAGE_RESET_AUDIO`, `RPG_STAGE_BANK_RELOAD`); the loot description says draws. Table now 650 rows (328 func, 150 data, 80 const, 62 file, 21 field, 8 type, 1 comment), 9 HYPOTHESIS rows. Open for the orchestrator: `wiki/spec/index.md` is created here and other phase 2 units add the same file, so expect a merge conflict.

### Review round 3 (2026-10-24): approved

- All nine findings of the previous review are resolved; no new clear errors in the rows or spec lines the fixes touched. The diff against w2-good is the table, the spec, `wiki/spec/index.md`, this ticket and `wiki/log.md`; `src/`, `include/` and symbol files are untouched.
- Himoo as identifier stem: gone from the table (`yuina` in the four dialog functions, `RPG_ENC_ROBOT`, `RPG_LAUNCH_ROBOT`, file note `enemy_robot.c`); the spec says once that Himoo is Yuina (girl id 2, `GIRL_YUINA`).
- `RpgRec18` and `RpgMotion20` field and type rows are gone and the header names are not renamed; `D_8015EC58` is `rpg_battle_move_params` with a per-use note that matches the C (girl run in 801504E0.c, basketball dunk rectangle in 801321D0.c, art move in 80154430.c).
- The ten unreferenced text buffers have no rows; every func and data old name exists in the source, no duplicate names or keys, five columns per row, ASCII only.
- Result constants are neutral (`RPG_RESULT_GOOD` 0, `RPG_RESULT_BAD` 1) and the notes cite the callers' branches (DATE in 80132000.c, SHUGAKU) and the aliens exception.
- Merge-time checks: no `himoo` left in the table, 650 rows (328 func, 150 data, 80 const, 62 file, 21 field, 8 type, 1 comment), 9 rows start with HYPOTHESIS, and the spec is linked from `wiki/spec/index.md`.
