---
id: T-39300
title: Model the original RAM layout as the port's game memory (M1)
status: In Progress
assignee: root (orchestration), worker agent
created: 2026-10-30
updated: 2026-10-30
links: ["[[port-completion-plan]]", "[[game-state]]", "[[data-types]]", "[[emulator]]", "[[tickets/T-39002-restore-port-runtime]]"]
---

## Goal

Give the port one game memory with the original layout, so every ported C function reads and writes the same fields as the original. This is layer 1 of [[port-completion-plan]].

## Acceptance criteria

- [ ] `tools/port/gen_game_memory.py` (Docker) generates typed GDScript views from `include/game/*.h` and `include/main/*.h`: `GameState` at 0x800E6280 and the gameplay globals the scheduler and overlays use, with widths and signedness from the headers. The generated file is reproducible and checked by a test.
- [ ] `core/game_memory.gd`: a 2 MiB main-RAM model with u8/s8/u16/s16/u32/s32 access through `native_math`, and loading of the executable's data sections from the installed disc at their original addresses. Original bytes stay outside Git.
- [ ] A two-way bridge between `rule_state` and the memory model. The opening frame-timing route (pad 2) stays at 0 mismatches and the April loop route is identical before and after.
- [ ] A comparison tool reports, field by field, port memory against an emulator RAM dump taken at the same route key.
- [ ] At the first command select (34/1/0/0, frame 13863) port memory equals emulator RAM for `GameState` and the listed globals. Every field is either equal, or listed with the ticket that brings it in.

## Notes

- `GameState` is 0x1A90 bytes with 161 documented fields ([[game-state]]). The port's `rule_state` has about 40 keys.
- Emulator RAM dumps: `tools/emu/run.sh ... --ram-dump`. The new-game golden states are under `.asset-cache/emu/newgame-golden-v1`.
- Dev seeding of port memory from an emulator state is allowed for development only, and must be labelled as such. Acceptance starts at New Game.

## Comments
