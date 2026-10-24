---
id: T-27026
title: Phase 2 naming and port spec for overlay BUNKAKEN (culture festival, visitor variant)
status: In Review
assignee: p2a-o-bunkaken (Sonnet)
created: 2026-10-24
updated: 2026-10-24
links: [[[tickets/T-27000-phase-2]], [[spec/o-bunkaken]]]
---

## Goal

Roadmap phase 2 for the BUNKAKEN overlay (main mode 0x71), port-first scope: name and explain the game logic (state, rules, flow, data tables), name the hardware layer only at module level, write the scene-flow spec. Tables and wiki only; no change to `src/`, `include/` or symbol files.

## Acceptance criteria

- [x] `config/renames/proposals/o-bunkaken.tsv`: one row per rename (func, data, field, type, const, file); one name per (unit, address); game-meaning guesses start with HYPOTHESIS.
- [x] `wiki/spec/o-bunkaken.md`: entry and trigger, why two variants, shared words, entry phases, the 142-entry stage machine (invitation, companion pick, club menu, routing, three exhibit templates, sub-modes), rules (invitation pick, points tables, refusal, absence, parameter gates, birthday phases, event log, fortune telling), data tables and disc files, quirks, open questions, stage catalog; linked from `wiki/spec/index.md`.
- [x] Every one of the 201 functions and 18 source files has a proposed English name.
- [x] No edits under `src/`, `include/` or `config/symbol*` files.
- [ ] Review of names, evidence and hypothesis marks; the rename tool run (plan, apply, 27/27 build) is a later step.

## Notes

- Prefix: `festival_visit_` (English form of the overlay's job; the abbreviation "bunkaken" is not explained by any string or file, the audit's `bunkaken_` is the alternative). Types `FestivalVisit*`, constants `FV_*`.
- The shared festival words (`D_80122E98..EC0`, `D_800E7D4E`) are main-scope rows with the note REQUEST; BUNKASAI and BUNKA_SD use the same words, so expect the BUNKASAI table to propose names for them too.
- Evidence: the matched C, the overlay data tables, the disc file names (BUNKA, BS, BUSTUP, VAB folders, mapped by sector), core and scheduler spec tables, and the emulator (a local copy of the probe runner with `--poke` and `--sample`, kept untracked in `build/scratch`): phases, invitation pick, club-menu routing, end-of-visit gains, refused-girl penalty, fortune lookup, the Shiori birthday phase and the Shiori-at-her-club route. Stage shows and TT launches were not run.
- Counts of the table: see the log entry.

## Comments
