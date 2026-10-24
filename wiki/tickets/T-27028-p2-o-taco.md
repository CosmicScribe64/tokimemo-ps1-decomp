---
id: T-27028
title: Phase 2 naming and port spec for overlay TACO (shooter mini game)
status: In Review
assignee: p2a-o-taco (Sonnet)
created: 2026-10-24
updated: 2026-10-24
links: [[[tickets/T-27000-phase-2]], [[spec/o-taco]]]
---

## Goal

Roadmap phase 2 (readable code), port-first scope, for the unit `src/ovl/TACO`: name the game logic, state and data tables of the shooter mini game, name the hardware layer only at module level, and write the scene-flow spec the Godot port needs. Pass writes tables and wiki pages only; no source, header or symbol file is edited.

## Acceptance criteria

- [x] `config/renames/proposals/o-taco.tsv`: one row per rename (kind scope old new note); all 406 functions of the overlay named; 179 data labels; 70 field, 24 type, 40 file, 176 const rows; 16 comment rows for O.BIN-named functions the overlay calls; 14 rows marked HYPOTHESIS.
- [x] `wiki/spec/o-taco.md`: stage machine, state and data with the new names, rules (formulas, thresholds, transitions), connections, open questions; linked from `wiki/spec/index.md`.
- [x] No game text, assets, asm or screenshots committed; no edits under `src/`, `include/` or `config/symbol*` files.
- [ ] Code review against `CODING_STANDARDS.md` (docs-only change) and merge of the table by the phase 2 merge step.

## Notes

- Prefix: `shooter_` for functions and data, `Shooter*` for types, `SHOOTER_*` for constants, `shooter_boss1_`..`shooter_boss3_` for the boss code and tables. Proposed from the glossary seed (mini games: `shooter` for TACO); the splat segment and disc name stay TACO.
- Field rows use the current type names as scope (`TcActor`, `Tc14`, ...); the type rows give the new names.
- Source files are proposed under `src/ovl/shooter/`; three files mix topics (`stage_name_entry.c` also holds the glyph drawer and the star field, `boss1_boss3.c` interleaves boss 1 and boss 3, `hud_secondary_weapons.c`); splitting is a later step.
- Method: read all 38 C files, the call graph (cluster per stage handler), and the data tables in `asm/ovl/TACO/data`; rules were checked against the C. The emulator was not used: the scene needs the unlock flag (`D_800E7D50` bit 0), which has no cheap entry in the current tooling (no memory poke; a GDB run would be needed).
- Main-exe objects the scene uses (model handles `D_80127080`, coordinate objects `D_80127480`, rotation table `D_80128880`, camera and lights `D_801227xx`, reference matrix `D_80129C40`) belong to the main unit and are not renamed here. Main functions called by the overlay (`func_80042908` set stage, `func_8004284C` next step, `func_80042940` set step, `func_800AA460` is libgte VectorNormal, `func_8004ACC8` full-screen flash) are listed in the spec for the main pass.
- Libgte copies in the overlay are identified by the matrix rows they touch: `func_8015CF30` is RotMatrixZ, `func_8015D0D0` RotMatrixX, `func_8015D270` RotMatrixY; proposed as `shooter_rot_matrix_z/x/y` so that they cannot clash with names the main pass gives its own copies.
- Dead code (no caller in the overlay): shooter_palette_reset, shooter_explode_dead_enemies, shooter_nop, shooter_menu_cursor_move, shooter_snapshot_screen_copy, shooter_load_texture_page_32, shooter_upload_rect_256, shooter_boss2_sway_motion_alt, shooter_set_model_clut_and_page, shooter_spawn_patroller (so enemy type 11 never spawns), shooter_count_free_coordinates.

## Comments
