---
id: T-39313
title: Port the engine menu, speaker and memory card files (WP-C)
status: In Progress
assignee: worker agent
created: 2026-10-30
updated: 2026-10-30
links: ["[[tickets/T-39301-engine-api-core]]", "[[engine-porting-contract]]", "[[tickets/T-39007-player-save-relaunch-recovery]]"]
---

## Goal

Port `menu_speaker_girl.c`, `hud_screens.c` and `memory_card.c` onto the engine, following [[engine-porting-contract]].

## Acceptance criteria

- [ ] Every function of these files is ported, with function-differential tests from emulator captures, or listed as untested with a reason.
- [ ] The core suite, `run_engine_diff.sh --all` and the opening frame-timing gate pass.

## Notes

## Comments
