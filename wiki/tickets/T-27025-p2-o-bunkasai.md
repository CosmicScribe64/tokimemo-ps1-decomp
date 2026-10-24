---
id: T-27025
title: Phase 2 naming and port spec for overlay BUNKASAI (culture festival, club-member variant)
status: In Review
assignee: p2a-o-bunkasai (Sonnet)
created: 2026-10-24
updated: 2026-10-24
links: [[[tickets/T-27000-phase-2]], [[spec/o-bunkasai]]]
---

## Goal

Roadmap phase 2 for the BUNKASAI overlay, port-first scope: name and explain the game logic (state, rules, flow, data tables), name the hardware layer only at module level, write the scene-flow spec. Tables and wiki only; no change to `src/`, `include/` or symbol files.

## Acceptance criteria

- [x] `config/renames/proposals/o-bunkasai.tsv`: one row per rename (func, data, field, type, const, file); one name per (unit, address); game-meaning guesses start with HYPOTHESIS; the O.BIN names the overlay calls already have comment rows in the main tables, so none is repeated.
- [x] `wiki/spec/o-bunkasai.md`: entry and exit, the overlay reload, the phase machine, the 165-stage table by block, the (club, year) -> exhibit table, the visit scene pattern, the special scenes (stage shows, fortune telling, TT game exhibit, birthday scene), the rules (result formula, event log, thresholds, flags), data tables, connections and open questions; linked from `wiki/spec/index.md`.
- [x] Every one of the 207 functions and 20 source files has a proposed English name.
- [x] No edits under `src/`, `include/` or `config/symbol*` files.
- [ ] Review of names, evidence and hypothesis marks (round 1: not approved, fixes below; round 2 pending); merge of the table by the phase 2 merge step (the rename tool run, `plan`, `apply` and the 27/27 build, is a later step).

## Notes

- Prefix `culture_` (the ADR map and the Module words table of `GLOSSARY.md` already list BUNKASAI with it); types `Culture...`, constants `CULTURE_...`, folder `src/ovl/culture_festival/`, header `include/ovl/culture_festival.h`.
- The player's club id (below 6) decides BUNKASAI over BUNKAKEN; the stage table is indexed by `GameState.stage` and consists of one block per heroine (visit blocks) and four club-mate blocks. Stage function names are `culture_<girl>_<visit|member>_<exhibit>`.
- The table has no rows with scope `main` since review round 1. The scene variables at `D_80122E98..D_80122EC0` and `D_800E7D4E` have their one row in the merged BUNKAKEN table (`culture_display_skip`, `culture_refused_girl` for `D_80122EA4`, and the same names as before for the rest), `D_800E7D4F` (`extra_mode_flags`) and `func_80041168` (`vram_clear_blocks`) in the merged m-flow table; the fields `club`, `girl`, `tt_unlocked`, `direct_mode` and `stage_select` are BUNKAKEN's rows too. The spec uses those names (section 3.1).
- Counts: 344 rows = 207 func, 57 data, 3 field, 10 type, 21 file, 46 const (was 360: the 16 main-scope rows are gone). Own HYPOTHESIS marks are in the spec, not in the table rows: the tour reading of BUNKAKEN, Megumi and Yumi not met in 1995, the intent of the pad check in stage 164, the three birthday options being presents, and the "unlock" reading of `tt_unlocked` (o-tt's, used by the OPTION row). The HYPOTHESIS names of m-scheduler (`acquainted`, `festival_call_95/96`) and core (`ExtraModeFlags`, `CultureFestivalDispatch`) are marked where the notes and the spec use them. Words that the glossary lacked are now rows of `GLOSSARY.md` (spec section 14); the table adds no abbreviation.
- The `rename.py plan` run (with the gcc proof, on the base merged with w2-good `2c825bfa1`) over the func, data, field and type rows (277) reports no problem. `file` and `const` rows are written by the tool's separate modes, so the dry run refuses a mixed table; they follow the o-taco and o-rpgbat tables.
- Emulator: a scratch copy of the wrapper (`build/emu_scratch/`, not committed) added memory pokes and a value series; the title-menu state was poked into mode 2 with overlay target 0x70. Checked: phase and stage order for four club and member routes, the result formula (all 13 tokimeki words and the affection), the fortune table orientation, the stage show return point, the TT launch path and the birthday scene path with a menu choice. Further checks: 78 runs reproduce the block entry tables (all club and year pairs for girl 0, six pairs for girls 1 to 10); event log entry, festival-call and acquainted bits, the member entries of Mio, Yuina and Ayako, the TT launch and return path, and the birthday scene path were read back. Details in the spec.
- The dialogue group numbers inside each scene function are script indices; they are left in the C (the spec gives the shared patterns).

## Comments

### Review round 1 fixes (2026-10-24)

The review was not approved: five blocking findings, all reconciliation with names merged after the branch started or the abbreviation rule. The branch merged w2-good (`2c825bfa1`: m-flow, o-date, o-bunkaken, o-event). Fixes:

- `func_80041168`: the row is dropped, the spec uses m-flow's `vram_clear_blocks`.
- `D_800E7D4F`: the row is dropped, the spec uses `extra_mode_flags` (m-flow, core's HYPOTHESIS type `ExtraModeFlags`).
- `D_80122E98`: the row is dropped, the spec uses BUNKAKEN's `culture_display_skip`; `disp` is not on the Abbreviations table.
- `Flags7D4F.unk_2_4`: the row is dropped; the three bits keep BUNKAKEN's and o-tt's name `stage_select`.
- New names with abbreviations written out: `culture_fortune_compat_table` is `culture_fortune_blood_table` and `CultureCompatTable` is `CultureFortuneBloodTable`; field `stage_fn` is `stage_function`.
- All 11 main-scope data and function rows and the 5 fields on `BunkaDispatch8` and `Flags7D4F` were identical copies of BUNKAKEN's or m-flow's rows or differed as above, so they are dropped and the merge step has one row per main address.
- `CULTURE_PHASE_GATE_*` are `CULTURE_PHASE_BACKGROUND_LOAD/SHOW/FADE_IN`: the code reads sector 0x4029 as a background and no run showed the picture, so the notes and the spec no longer say gate.
- `culture_unused_girl_bytes` is `culture_unused_bytes` (the note gives the bytes and says the copy is never read; the type was already `CultureUnusedBytes`).
- Notes that named the flag bit 2 `acquainted` and the bits 12 and 13 `festival_call_95/96` now say that these are m-scheduler's HYPOTHESIS names.
- Spec: the "135 scene functions of one pattern" is 133 of the pattern plus the stage-78 stub and the Ayako follow-up; the open questions say that `D_80122EC0` keeps one `s32` and the two overlays use their own constants.
- Glossary: six rows added to `GLOSSARY.md` (exhibit, club mate, visitor, fortune telling, backstage, tokimeki gain).
