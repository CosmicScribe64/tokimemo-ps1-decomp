---
id: T-27028
title: Phase 2 naming and port spec for overlay TACO (shooter mini game)
status: Done
assignee: p2a-o-taco (Sonnet)
created: 2026-10-24
updated: 2026-10-24
links: [[[tickets/T-27000-phase-2]], [[spec/o-taco]]]
---

## Goal

Roadmap phase 2 (readable code), port-first scope, for the unit `src/ovl/TACO`: name the game logic, state and data tables of the shooter mini game, name the hardware layer only at module level, and write the scene-flow spec the Godot port needs. Pass writes tables and wiki pages only; no source, header or symbol file is edited.

## Acceptance criteria

- [x] `config/renames/proposals/o-taco.tsv`: one row per rename (kind scope old new note); all 406 functions of the overlay named; 179 data labels; 70 field, 24 type, 40 file, 185 const rows; 9 comment rows for O.BIN-named functions the overlay calls (913 rows in all); 14 rows marked HYPOTHESIS.
- [x] `wiki/spec/o-taco.md`: stage machine, state and data with the new names, rules (formulas, thresholds, transitions), connections, open questions; linked from `wiki/spec/index.md`.
- [x] No game text, assets, asm or screenshots committed; no edits under `src/`, `include/` or `config/symbol*` files.
- [x] Code review against `CODING_STANDARDS.md` (docs-only change) and merge of the table by the phase 2 merge step.

## Notes

- Prefix: `shooter_` for functions and data, `Shooter*` for types, `SHOOTER_*` for constants, `shooter_boss1_`..`shooter_boss3_` for the boss code and tables. Proposed from the glossary seed (mini games: `shooter` for TACO); the splat segment and disc name stay TACO.
- Field rows use the current type names as scope (`TcActor`, `Tc14`, ...); the type rows give the new names.
- Source files are proposed under `src/ovl/shooter/`; three files mix topics (`stage_name_entry.c` also holds the glyph drawer and the star field, `boss1_boss3.c` interleaves boss 1 and boss 3, `hud_secondary_weapons.c`); splitting is a later step.
- Method: read all 38 C files, the call graph (cluster per stage handler), and the data tables in `asm/ovl/TACO/data`; rules were checked against the C. The emulator was not used: the scene needs the unlock flag (`D_800E7D50` bit 0), which has no cheap entry in the current tooling (no memory poke; a GDB run would be needed).
- Main-exe objects the scene uses (model handles `D_80127080`, coordinate objects `D_80127480`, rotation table `D_80128880`, camera and lights `D_801227xx`, reference matrix `D_80129C40`) belong to the main unit and are not renamed here. Main functions called by the overlay (`func_80042908` set stage, `func_8004284C` next step, `func_80042940` set step, `func_800AA460` is libgte VectorNormal, `func_8004ACC8` full-screen flash) are listed in the spec for the main pass.
- Libgte copies in the overlay are identified by the matrix rows they touch: `func_8015CF30` is RotMatrixZ, `func_8015D0D0` RotMatrixX, `func_8015D270` RotMatrixY; proposed as `shooter_rot_matrix_z/x/y` so that they cannot clash with names the main pass gives its own copies.
- Dead code (no caller in the overlay): shooter_palette_reset, shooter_explode_dead_enemies, shooter_nop, shooter_menu_cursor_move, shooter_snapshot_screen_copy, shooter_load_texture_page_32, shooter_upload_rect_256, shooter_boss2_sway_motion_alt, shooter_set_model_clut_and_page, shooter_spawn_patroller (so enemy type 11 never spawns), shooter_count_free_coordinates.

## Comments

### Review round 1 fixes (2026-10-24)

- Sound ids: a scene call passes an even first argument, so `func_801335A0` plays 0x600 + the low byte, not 0x500 +. All notes and the spec now cite the played id (boss 1 defeat 0x608, boss 2 defeat 0x604, title 0x604 to 0x607, opening, boss cadences); bank 6 constants 0x604 to 0x608 and the bank 5 item sounds 0x511, 0x512, 0x514, 0x516 added.
- Pad enum renamed to `PadBits` (core's name); the six conflicting core values are marked in the notes and listed in spec section 14 for the core owner.
- `TcActor.unk7A` notes now say it is the HP bar count on a boss part; type notes flag the `Tc14` alias and the settings types that overlap core; seven comment rows that duplicate core were dropped.

### Review round 2 (2026-10-24): approved

- No blocking findings. Both round-1 blockers are fixed and were checked against the C: scene sound ids (`func_801335A0` ORs the low byte into 0x600 for an even first argument; cooldown only for first argument 1, id below 10 and `D_8015EDB0` nonzero; played ids confirmed for title frames 0x40/0x80/0xA0/0xC0, boss 1 defeat and fight end, boss 2 defeat and its 0x600 cadence) and the pad enum (pad word is `(~byte2 << 8) | ~byte3`, `src/main/80042A00.c` lines 147-171, so L2=1, R2=2, L1=4, R1=8, SELECT=0x100, START=0x800; the TACO values are right, `core.tsv` is wrong).
- Table checks: 913 rows, all with 5 tab-separated fields, no duplicate keys, no invalid identifiers, case conventions hold, no clashes with `src/`, `include/`, `config/*.txt` or the other phase 2 tables except the `PAD_*` rows; all 38 source files have a file row; exactly 14 rows carry HYPOTHESIS.
- Scope: the diff touches only the table, the spec, the spec index, this ticket and the log. No `src/`, `include/` or `config/symbol*` edits, no game text or assets.
- Evidence sample (about 25 rows, 9 spec claims) confirmed against the C or data.
- Open for the core owner: six `core.tsv` pad values conflict (spec section 14).
