---
id: T-27023
title: Phase 2 analysis of overlay EVENT (dead development copy of the girl-scene and date-place code)
status: In Review
assignee: claude (Sonnet analyst)
created: 2026-10-24
updated: 2026-10-24
links: ["[[tickets/T-27000-phase-2]]", "[[spec/o-event]]", "[[spec/o-event-steps]]", "[[tickets/T-0200-event-gyozi-loader-and-address]]"]
---

## Goal
Roadmap phase 2 for unit o-event (overlay EVENT, 804 C functions in 18 files, `src/ovl/EVENT`): name the entry points, the step-table state machines, every function of the scene logic and the data tables it reads, and write the scene-flow spec. Analysis only: renames are applied later through `tools/rename.py`.

## Acceptance criteria

- [x] `config/renames/proposals/o-event.tsv`: one row per rename (func, data, field, type, file, dir, const), one name per address, notes carry the evidence, game-meaning guesses start with HYPOTHESIS; the table passes `tools/rename.py plan` with no problem (file, dir and const rows were taken out of the dry run, as the tool requires)
- [x] Rows the tool cannot take as overlay rows are in two side tables with the same format: `o-event-devmain.tsv` (121 aliases of stale main addresses) and `o-event-deferred.tsv` (6 data names declared in `include/main_api.h`)
- [x] `wiki/spec/o-event.md` (state machines, rules, data, dev-to-shipped address map, relation to the live overlays, open questions) and the appendix `wiki/spec/o-event-steps.md` (all 95 step tables); linked from `wiki/spec/index.md`
- [x] No `src/`, `include/` or symbol file edits; no game text, assets, asm or screenshots committed
- [ ] Review against `CODING_STANDARDS.md` and the spec (nothing in `src/` changed: only tables and wiki pages)

## Notes
- Identity: EVENT.EXN is a development build of the girl-scene code (14 girls, 73 scenes), the place-date code (21 places, a 61-step arrival phase and per-place tables) and the amusement-park ride movies. It is linked against another build of the main program, so it can never run in the shipped game: 135 distinct main-side call and table targets, none a function start of the shipped exe (53 below its text start); 175 main-side data addresses, 51 in shipped rodata. The 24 shipped loader sectors do not include EVENT's. The script loaders read the same sector counts as the shipped `SERIFU/EVENT_xx.BIN` (10 of 12) and `DATE_xx.BIN` (10 of 11) files, so it is an earlier state of the same game. Evidence in [[spec/o-event]] section 1.
- Method: opcode-shape siblings of every EVENT function in all other units (`tools/siblings.py` internals, 675 of 804 at ratio 0.90 or more); call-by-call alignment of clones gives a stable dev-to-shipped map of main functions and data (121 aliases); table alignment against DATE tables gave the dev step primitives (`scene_wait_cd_read`, `dec_init`, `change_dec_bg`, `normal_date_girl_in/out`, ...). Strings give place and scene labels; the order of the 21 place tables was checked against core's `Place` enum.
- Naming: prefix `spot_` as the ADR map has it ("confirm unused" is confirmed; the map description is too narrow, see the spec's open questions). Girls by given name (core `GirlId`), places by core `Place`. Step functions are `spot_<owner>_<role>_<NN>` by table position, shared steps `spot_shared_...`, effects and rule steps individually named.
- Result: 1126 rows in `o-event.tsv` (805 func, 178 data, 67 type, 39 field, 17 const, 19 file, 1 dir), 14 with HYPOTHESIS; plus 121 rows in `o-event-devmain.tsv` (15 with HYPOTHESIS) and 6 in `o-event-deferred.tsv`.
- Port-first scope: the unit is dead code. The spec says so, keeps the rules it implements (special event chooser, outcome scoring, close step, ride choice) as a cross-check for DATE and names only what is needed to read the structure; the hardware layer is named at module level only.
- No O.BIN-named function is defined in this unit, so there are no `comment` rows.
- The emulator cannot start EVENT (its main-side calls do not exist), so the proof is static. One probe from the saved title-menu state (`tools/emu/run.sh boot --dump`, state under the ignored `build/emu/`): RAM at the dev `step_next` address 0x80011DE0.. is zero, the shipped `main_step_next` at 0x8004284C is code, 0x800F6E84 is zero and `D_80123110` holds NAME_ENT's entry. Facts about the live twins (DATE, GEKO) come from their matched C.
- Tool findings: `rename.py plan` refuses overlay rows for addresses that are not defined anywhere (the stale dev addresses) and fails its gcc proof for EVENT-range data whose only declaration is in `include/main_api.h` (six names: `D_8012076C`, `D_80121884`, `D_801220EC`, `D_8012277C`, `D_80120798`, `D_801206E8`). Both are listed in the spec's open questions.

## Comments
