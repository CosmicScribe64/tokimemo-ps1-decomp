---
id: T-27004
title: "Shared core: GameState fields, core types, first enums, top 100 functions"
status: Backlog
assignee:
created: 2026-10-24
updated: 2026-10-24
blocked-by: [T-27002, T-27003]
links: [[[phase2]], [[game-state]], [[data-types]], [[emulator]], [[tickets/T-20500-emulator-assisted-naming]], [[tickets/T-27000-phase-2-readable-codebase]]]
---

## Goal

Name the shared core that about half of every reader's view goes through: GameState (`D_800E6280`, 161
top-level fields, 12,306 uses in 295 files), the shared types of `include/game.h` and `include/main_api.h`
(the 0x44 display pools first), the first enums and the 100 most used main functions (48% of function-name
uses). Conventions: `docs/adr/0001-phase-2-naming.md`; stems: `GLOSSARY.md`.

## Acceptance criteria

- [ ] GameState and its known fields named (calendar bytes, birthday, player names, the nine parameters,
      girl records, club flags, appointments, day-event queue, event log, current girl, game mode, stage and
      step, frame counter, menu mode); unknown fields stay `unk_XX`.
- [ ] Shape-named core types renamed (`Rec38`, `Rec34`, `Rec24`, `DisplayActor44`, `Work800E7D10`, ...),
      size suffixes dropped; hypotheses marked.
- [ ] First enums in their topic headers: girl, place, club, parameter, weekday, game mode, day-event type;
      fields keep their widths with an `/* enum X */` comment.
- [ ] The 100 most used main functions named, with evidence per row; O.BIN names kept.
- [ ] Evidence for every name in the rename tables; emulator runs for meaning claims where the code is not
      enough; Opus review of every table.
- [ ] Clean build 27/27 sha1 after each batch.

## Notes

- Builds on the GameState naming pilot of T-20500 and the field map in [[game-state]].
- Tools that hardcode `D_800E6280` or field names (audit 2.2) have to be updated or moved to `tools/legacy/`
  by T-27002 first.

## Comments
