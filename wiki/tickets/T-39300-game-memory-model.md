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

- [x] `tools/port/gen_game_memory.py` (Docker) generates typed GDScript views from `include/game/*.h` and `include/main/*.h`: `GameState` at 0x800E6280 and the gameplay globals the scheduler and overlays use, with widths and signedness from the headers. The generated file is reproducible and checked by a test.
- [x] `core/game_memory.gd`: a 2 MiB main-RAM model with u8/s8/u16/s16/u32/s32 access through `native_math`, and loading of the executable's data sections from the installed disc at their original addresses. Original bytes stay outside Git.
- [x] A two-way bridge between `rule_state` and the memory model. The opening frame-timing route (pad 2) stays at 0 mismatches and the April loop route is identical before and after.
- [x] A comparison tool reports, field by field, port memory against an emulator RAM dump taken at the same route key.
- [x] At the first command select (34/1/0/0, frame 13863) port memory equals emulator RAM for `GameState` and the listed globals. Every field is either equal, or listed with the ticket that brings it in.

## Notes

- `GameState` is 0x1A90 bytes with 161 documented fields ([[game-state]]). The port's `rule_state` has about 40 keys.
- Emulator RAM dumps: `tools/emu/run.sh ... --ram-dump`. The new-game golden states are under `.asset-cache/emu/newgame-golden-v1`.
- Dev seeding of port memory from an emulator state is allowed for development only, and must be labelled as such. Acceptance starts at New Game.

## Comments

### 2026-10-30 - layout generator (deliverable 1)

`tools/port/gen_game_memory.py` computes the layout of every struct in `include/*.h`, `include/game/*.h`, `include/main/*.h` and `include/main/import_views/*.h` with C alignment and bit-field packing, then checks every `/* 0xNNN */` and `/* size 0xNN */` comment against the computation (one rule needed: a bit-field comment may name its unit or the byte holding its first bit, as in `ColorPair555`). It writes `port/godot/core/game_layout.gd`: offset constants, a `STRUCTS` table and a `GLOBALS` table of 1,000+ extern data symbols with addresses from `config/symbol_addrs*.txt` (GameState 0x1A90, 161 fields, SystemData 0xEE0). `tools/port/test_gen_game_memory.py` (8 tests) regenerates and compares, and checks the 161 GameState offsets, the widths and the size against the header text. See [[game-memory-model]].

### 2026-10-30 - memory model (deliverable 2)

`port/godot/core/game_memory.gd`: 2 MiB, `u8`..`s32` through `native_math`, latched faults, typed views, PS-X EXE loader from the installed bundle (SHA-256 `bae1514f...` pinned, read at runtime only). Tests: `tests/core/run_game_memory_tests.gd`, registered in `tests/run_tests.gd`. Core suite: `PASS: 9 test suite(s) ran`.

### 2026-10-30 - bridge (deliverable 3)

`core/rule_state_bridge.gd` pushes and pulls the 40 mapped `rule_state` keys, the core fields and the `opening` GameState fields; `PORT_ONLY_KEYS` lists the keys with no RAM home. Not wired into the frame loop, so behaviour cannot change. Both gates run twice (before and after the boot defaults): opening frame timing `pad 2, 104 substeps, 6709 frames to 12/0/0, 0 mismatches`, exit 0; April loop route `PLAYTEST stop=app_failure key=34/1/0/0 frame=16694`, `compare_route_modes.py` IDENTICAL against `merge-pending-2026-10-30/loop-route-fast` (playtest.json, states, transitions, frames, PNGs).

### 2026-10-30 - comparison tool and dump (deliverable 4)

`tools/port/compare_game_memory.py` names differing fields from the generated layout (`girls[3].tokimeki`, bit-field names inside words); `tools/port/test_compare_game_memory.py` (7 tests) covers it on synthetic images. The playtest observer dumps the memory at 34/1/0/0 to `<out>/game-memory/34-1-0-0.ram.bin` (dev evidence, ignored data); `tests/support/run_dump_from_state.gd` rebuilds a dump from `states/34-1-0-0.json` without a new route run.

### 2026-10-30 - emulator comparison at the first command select (deliverable 5)

Emulator dump: `tools/emu/route_driver/run.sh` from `newgame-golden-v1` set A `stage20-entry.sstate` with a one-checkpoint script (checkpoint `select`, `ram: true`), vsync 488, RAM frame_count 21332 (post-update 21333); it equals the golden a7 pair's frame. Port memory (`core/game_state_boot.gd` defaults, then the bridge): 133 of 159 comparable GameState fields equal, 5,982 of 6,800 bytes; first run before the boot defaults: 121 of 159. Remaining differences (26 fields, one `scene_work` global, three clock values) each carry a reason and a follow-up in `config/port/game-memory-known-diffs.json`: engine writes go to [[tickets/T-39301-engine-api-core]] and [[tickets/T-39302-generic-ps1-renderer]]; the club draw of girl 2 and `scene_work` need the boot/title/file-select history. `SystemData` (informational): 3,739 of 3,808 bytes equal; the file-select slot data, the other seeds and the metrics differ.

### 2026-10-30 - review fixes

Independent review fixes: the bridge no longer pushes or pulls `opening.unsourced` keys (scene_work, background_cache, background_show), so the executable's own `.data` for `bg_lba_cache` is kept; `compare_game_memory.py` reports those globals as "unsourced" from the dump metadata (`unsourced_keys`) and never as equal or different; `new_game_memory` refuses a state without the adapter's ownership record (`opening.unsourced` and `unread_before_owned_write`; the live route renames `opening.origin`) and `push(with_core=false)` keeps C defaults; the playtest memory dump needs `TOKIMEMO_DUMP_GAME_MEMORY=1`; `gen_game_memory.py` evaluates constants with an ast-based integer evaluator; bit positions are tested against independent sources (word_views.h, girl.h, display_records.h, rules/new_game.gd). `port/opening-to-gameplay` merged. Comparison unchanged: 133 of 159 comparable GameState fields equal; scene_work, bg_lba_cache and dec_bg_show now read "unsourced".

