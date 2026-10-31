---
id: T-27026
title: Phase 2 naming and port spec for overlay BUNKAKEN (culture festival, visitor variant)
status: Done
assignee: p2a-o-bunkaken (Sonnet)
created: 2026-10-24
updated: 2026-10-24
links: `[tickets/T-27000-phase-2`, `spec/o-bunkaken`]
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

### Review round 2 (2026-10-24, Opus 5.5, approved, no findings)

All three blocking findings of round 1 are resolved in commit 2d3372522.

- Prefix, folder and constants: all 457 rows follow the ADR row for BUNKAKEN: folder `culture_festival_tour`, header `include/ovl/culture_festival_tour.h`, prefix `tour_` (201 funcs, 30 data), types `Tour*` (10) and constants `TOUR_*` (172). A grep finds no `festival_visit` or `FV_` outside the log history entry, which keeps the old name on purpose. The spec's Naming paragraph adopts the ADR row unchanged and explains why the six-club menu confirms it. The 9 main-scope REQUEST words (`D_80122E98` to `D_80122EC0`) carry the `culture_` stem.
- Abbreviations: a token scan of every new name finds no `lit`, `comp`, `y95` to `y97`, `FV`, `min`, `wrapup`, `fn` or `bustup`; they are spelled out (`literature`, `computer`, `1995` to `1997`, `minimum`, `wrap_up`, `handler`, `portrait`). The remaining short forms (`bg`, `lba`, `init`, `param`) are rows of the GLOSSARY abbreviation table. The one exception is `tt` (`tt_stages_cleared`, `tt_unlocked`, `tour_stage_tt_narrator`, `TOUR_STAGE_TT_NARRATOR`): it is not in the table and the spec says the glossary row is still open, but it is the TT overlay's own stem and the o-tt table uses it throughout, so it is not blocking.

The reviewer found no new clear errors in the rows or lines the fix touched. The branch changes only `config/renames/proposals/o-bunkaken.tsv`, `spec/o-bunkaken`, [[spec/index]], the ticket and the log: nothing under `src/`, `include/` or `config/symbol*` (checked against the merge base). Set Done. The last acceptance item (the rename tool run: plan, apply, 27/27 build) stays open and is not part of this branch. The board card is not touched here (the kanban sync belongs to the merge script).
