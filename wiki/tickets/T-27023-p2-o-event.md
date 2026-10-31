---
id: T-27023
title: Phase 2 analysis of overlay EVENT (dead development copy of the girl-scene and date-place code)
status: Done
assignee: claude (Sonnet analyst)
created: 2026-10-24
updated: 2026-10-24
links: ["[[tickets/T-27000-phase-2]]", "`spec/o-event`", "`spec/o-event-steps`", "[[tickets/T-0200-event-gyozi-loader-and-address]]"]
---

## Goal
Roadmap phase 2 for unit o-event (overlay EVENT, 804 C functions in 18 files, `src/ovl/EVENT`): name the entry points, the step-table state machines, every function of the scene logic and the data tables it reads, and write the scene-flow spec. Analysis only: renames are applied later through `tools/rename.py`.

## Acceptance criteria

- [x] `config/renames/proposals/o-event.tsv`: one row per rename (func, data, field, type, file, dir, const), one name per address, notes carry the evidence, game-meaning guesses start with HYPOTHESIS; the 1099 rows left after setting aside the 19 file, 1 dir and 17 const rows pass `tools/rename.py plan` with no problem (the dry run takes no file, dir or const rows from this table)
- [x] Rows the tool cannot take as overlay rows are in two side tables with the same format: `o-event-devmain.tsv` (121 aliases of stale main addresses) and `o-event-deferred.tsv` (6 data names declared in `include/main_api.h`)
- [x] `wiki/spec/o-event.md` (state machines, rules, data, dev-to-shipped address map, relation to the live overlays, open questions) and the appendix `wiki/spec/o-event-steps.md` (all 95 step tables); linked from `wiki/spec/index.md`
- [x] No `src/`, `include/` or symbol file edits; no game text, assets, asm or screenshots committed
- [x] Review against `CODING_STANDARDS.md` and the spec (nothing in `src/` changed: only tables and wiki pages)

## Notes
- Identity: EVENT.EXN is a development build of the girl-scene code (14 girls, 73 scenes), the place-date code (21 places, a 61-step arrival phase and per-place tables) and the amusement-park ride movies. It is linked against another build of the main program, so it can never run in the shipped game: 135 distinct main-side call and table targets, none a function start of the shipped exe (53 below its text start); 175 main-side data addresses, 51 in shipped rodata. The 24 shipped loader sectors do not include EVENT's. The script loaders read the same sector counts as the shipped `SERIFU/EVENT_xx.BIN` (10 of 12) and `DATE_xx.BIN` (10 of 11) files, so it is an earlier state of the same game. Evidence in `spec/o-event` section 1.
- Method: opcode-shape siblings of every EVENT function in all other units (`tools/siblings.py` internals, 675 of 804 at ratio 0.90 or more); call-by-call alignment of clones gives a stable dev-to-shipped map of main functions and data (121 aliases); table alignment against DATE tables gave the dev step primitives (`scene_wait_cd_read`, `dec_init`, `change_dec_bg`, `normal_date_girl_in/out`, ...). Strings give place and scene labels; the order of the 21 place tables was checked against core's `Place` enum.
- Naming: prefix `spot_` as the ADR map has it ("confirm unused" is confirmed; the map description is too narrow, see the spec's open questions). Girls by given name (core `GirlId`), places by core `Place`. Step functions are `spot_<owner>_<role>_<NN>` by table position, shared steps `spot_shared_...`, effects and rule steps individually named.
- Result: 1136 rows in `o-event.tsv` (805 func, 178 data, 67 type, 49 field, 17 const, 19 file, 1 dir), 17 whose note starts with HYPOTHESIS; plus 121 rows in `o-event-devmain.tsv` (15 with HYPOTHESIS) and 6 in `o-event-deferred.tsv`.
- Port-first scope: the unit is dead code. The spec says so, keeps the rules it implements (special event chooser, outcome scoring, close step, ride choice) as a cross-check for DATE and names only what is needed to read the structure; the hardware layer is named at module level only.
- No O.BIN-named function is defined in this unit, so there are no `comment` rows.
- The emulator cannot start EVENT (its main-side calls do not exist), so the proof is static. One probe from the saved title-menu state (`tools/emu/run.sh boot --dump`, state under the ignored `build/emu/`): RAM at the dev `step_next` address 0x80011DE0.. is zero, the shipped `main_step_next` at 0x8004284C is code, 0x800F6E84 is zero and `D_80123110` holds NAME_ENT's entry. Facts about the live twins (DATE, GEKO) come from their matched C.
- Tool findings: `rename.py plan` refuses overlay rows for addresses that are not defined anywhere (the stale dev addresses) and fails its gcc proof for EVENT-range data whose only declaration is in `include/main_api.h` (six names: `D_8012076C`, `D_80121884`, `D_801220EC`, `D_8012277C`, `D_80120798`, `D_801206E8`). Both are listed in the spec's open questions.

## Comments

### 2026-10-24 review round 1 fixes
The reviewer found three blocking problems; all three are fixed in the table and the spec.
- Rule status: the numbers of 7.5 and 7.6 are EVENT's own and differ from the shipped DATE (factor b of the outcome score is nonlinear there, the third outcome term goes to the syoushin word, the close step puts the tokimeki bonuses under the looks test and subtracts 0x28 only for a repeated appointment place). New section 7.0 of `spec/o-event` lists the differences found (12 rows, DATE authoritative); sections 7.5, 7.6, 10 and the function notes say so. The fix pass found more divergences than the review listed: the third-term destination, the athletics threshold of one chooser event, outcome ids one lower, the ride latch and a wrong girl index in the park bonus.
- The ride variant is bit 0 (parity) of the girl's event counter of the ride, not a "ride-seen bit"; the same parity picks the hero show variant and the other place variants. Game meaning marked HYPOTHESIS (7.2, 7.7, `spot_ride_setup`, `SPOT_VARIANT_ALTERNATE`).
- Core names: the EVENT private views now use `syoushin`, `event_counters`, `girl_last_date_spot`, `saved_mode/stage/step`, `appt_place_0` (HYPOTHESIS, as in core), `appointment_flag`, `pad_pressed`, `screen_center_x/y`; spec section 9 has the dev-to-shipped offset map of these fields. Two names stay open between units (`GameState.unk_014/018`, the `Rec24` members): spec section 11, questions 8 and 9.
- Non-blocking: counts reworded (1099 plan rows; 675 siblings recounted with the sibling index: closest sibling in DATE 559, GEKO 87, GYOZI 9, DATE2 5, others 15); the 7.1 table text that was cut at 130 characters is complete; 10 members named (athletics, saved_*, appointment_flag, pad_pressed and others), 18 `unk_` members listed as intentional; the 25 position-only step functions now have behaviour notes; parameter identities (trivia, athletics, looks) are corroborated by the DATE twins and lost their HYPOTHESIS marks; Yuina scene 4 now shows its stat deltas (+2a -10s).
- Table now 1136 rows: 10 field rows added (`player_flags`, `athletics`, `saved_mode`, `saved_stage`, `saved_step`, `appointment_flag`, `pad_pressed`, `EventStateFixView.girls`, `EventTransitionState.step_timer` and `step`). `rename.py plan` over the 1099 non-file/dir/const rows: see the log entry for the result.

### 2026-10-24 review approved
Round 2 review: approved, no findings. All three blocking findings of round 1 are resolved, and no factual error was found in the rows or spec lines the fix touched. Each claim was checked against the C in the worktree.
- Rule status: spec section 7.0 is new and says the shipped DATE is authoritative; sections 7.5, 7.6 and 10 are reworded to match. Factor b of the outcome score is linear in EVENT (`func_80118DAC`) and nonlinear in DATE (`func_8013C410`), and the spec states both. In the close step (EVENT `func_80119110`, DATE `func_8013CDF8`) the tokimeki bonuses, the 0x28 reduction, the record-flag set and the last-date-spot store sit under different tests, and the spec matches the C.
- Extra differences found by the fix pass check out: the third outcome term goes to the syoushin word in DATE and to the other word in EVENT (term table values match as halfwords versus words), the repeat-reaction conditions in DATE are stricter, and the Yuina park bonus uses a different girl index.
- Scope: tables and wiki only. Nothing under `src/`, `include/` or `config/symbol*` changed on the branch (`git diff --name-only w2-good...HEAD` over those paths is empty). Moved to Done; the board card is moved by the kanban sync in the merge step.
