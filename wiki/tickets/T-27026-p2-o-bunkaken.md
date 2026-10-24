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

- Prefix: `tour_` (ADR row for BUNKAKEN: folder `culture_festival_tour`, types `Tour*`, constants `TOUR_*`). Round 1 of the review changed this from the first draft's `festival_visit_`; the six-club menu confirms the ADR's "confirm" status. Shared festival words of main scope use the `culture` stem and the BUNKASAI table's names.
- The shared festival words (`D_80122E98..EC0`, `D_800E7D4E`) are main-scope rows with the note REQUEST; BUNKASAI and BUNKA_SD use the same words, so expect the BUNKASAI table to propose names for them too.
- Evidence: the matched C, the overlay data tables, the disc file names (BUNKA, BS, BUSTUP, VAB folders, mapped by sector), core and scheduler spec tables, and the emulator (a local copy of the probe runner with `--poke` and `--sample`, kept untracked in `build/scratch`): phases, invitation pick, club-menu routing, end-of-visit gains, refused-girl penalty, fortune lookup, the Shiori birthday phase and the Shiori-at-her-club route. Stage shows and TT launches were not run.
- Counts of the table: see the log entry.

## Comments

### Fix pass after review round 1 (2026-10-24)

Blocking: prefix, folder and constants moved to the ADR row (`tour_`, `culture_festival_tour`, `TOUR_`); abbreviations `lit`, `comp`, `y95`-`y97`, `min` spelled out; spec 5.2 rewritten for girl 0's seed behaviour, with an emulator check. Non-blocking: 5.3 and 5.5 wording, cross-unit notes in the spec (open questions), setter and wait-counter names. Awaiting review round 2.
