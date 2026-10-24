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
- [ ] Review of names, evidence and hypothesis marks; merge of the table by the phase 2 merge step (the rename tool run, `plan`, `apply` and the 27/27 build, is a later step).

## Notes

- Prefix `culture_` (the ADR map and the Module words table of `GLOSSARY.md` already list BUNKASAI with it); types `Culture...`, constants `CULTURE_...`, folder `src/ovl/culture_festival/`, header `include/ovl/culture_festival.h`.
- The player's club id (below 6) decides BUNKASAI over BUNKAKEN; the stage table is indexed by `GameState.stage` and consists of one block per heroine (visit blocks) and four club-mate blocks. Stage function names are `culture_<girl>_<visit|member>_<exhibit>`.
- Rows with scope `main` (the scene variables at `D_80122E98..D_80122EC0`, the two TT bytes `D_800E7D4E/F`, and the helper `func_80041168` as `vram_slots_clear`) are main symbols shared with BUNKAKEN, BUNKA_SD, TT and other overlays; they must be reconciled with those units. They sit in this table because this overlay is the first to explain them; the main owner applies them (overlay rows cannot rename main symbols). `D_80122EA4` is left unnamed for BUNKAKEN.
- Counts: 360 rows = 208 func (207 overlay + 1 main), 67 data (57 overlay + 10 main), 8 field, 10 type, 21 file, 46 const. No row starts with HYPOTHESIS: the names say what the code does; the spec states its four hypotheses (the tour reading of BUNKAKEN, Megumi and Yumi not met in 1995, the intent of the pad check in stage 164, the three birthday options being presents). Words that the glossary lacks are listed in spec section 14.
- The `rename.py plan` dry run passes for the func, data, field and type rows (two collisions were fixed: `culture_fortune_lookup`, field `stage_fn`). `file` and `const` rows are written by the tool's separate modes, so the dry run refuses a mixed table; they follow the o-taco and o-rpgbat tables.
- Emulator: a scratch copy of the wrapper (`build/emu_scratch/`, not committed) added memory pokes and a value series; the title-menu state was poked into mode 2 with overlay target 0x70. Checked: phase and stage order for four club and member routes, the result formula (all 13 tokimeki words and the affection), the fortune table orientation, the stage show return point, the TT launch path and the birthday scene path with a menu choice. Further checks: 78 runs reproduce the block entry tables (all club and year pairs for girl 0, six pairs for girls 1 to 10); event log entry, festival-call and acquainted bits, the member entries of Mio, Yuina and Ayako, the TT launch and return path, and the birthday scene path were read back. Details in the spec.
- The dialogue group numbers inside each scene function are script indices; they are left in the C (the spec gives the shared patterns).

## Comments
