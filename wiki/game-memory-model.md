---
type: tool
updated: 2026-10-30
sources: ["tools/port/gen_game_memory.py", "port/godot/core/game_memory.gd", "port/godot/core/game_layout.gd", "port/godot/core/rule_state_bridge.gd", "port/godot/core/game_state_boot.gd", "tools/port/compare_game_memory.py", "config/port/game-memory-known-diffs.json"]
---

# Game memory model (T-39300, layer 1 of [[port-completion-plan]])

One 2 MiB RAM image with the original layout, so ported C reads and writes the fields where the C does. Ticket: [[tickets/T-39300-game-memory-model]]. Field map of the object it models: [[game-state]].

## Pieces

| File | What it does |
|---|---|
| `tools/port/gen_game_memory.py` | Parses `include/*.h`, `include/game/*.h`, `include/main/*.h` (and `import_views`), computes every struct layout with C alignment and bit-field packing, and checks each `/* 0xNNN */` and `/* size 0xNN */` comment against it. Writes `port/godot/core/game_layout.gd`: `SIZEOF_<STRUCT>`, `<STRUCT>_<FIELD>` offsets, a `STRUCTS` table (offset, width, signedness, count, nested struct, stride, bit shift and width) and a `GLOBALS` table (address, width, signedness, count) for every extern data symbol with an address in `config/symbol_addrs*.txt`. Deterministic; `tools/port/test_gen_game_memory.py` regenerates and compares, and checks the 161 GameState offsets and the 0x1A90 size against the header text. |
| `core/game_memory.gd` | RAM at 0x80000000 (KUSEG/KSEG1 aliases accepted), `u8`..`s32` access through `native_math`, latched faults for out-of-range and misaligned access, `View` objects for GameState, nested records, bit-fields and globals, snapshots, and `load_executable_file`: the PS-X EXE text image goes in at its `t_addr` from the user's installed bundle (`audio/first-arrival/executable.bin`, SHA-256 pinned). Original bytes are read at runtime only. |
| `core/rule_state_bridge.gd` | `push(state, memory)` and `pull(memory, state)` for the 40 mapped `rule_state` keys, the core fields, and the `opening` fields that are GameState fields. `pull` assigns only values that differ at the C width. Port-only keys (`PORT_ONLY_KEYS`) have no RAM home and are untouched. Placeholder keys (`opening.unread_before_owned_write`) are neither pushed nor pulled. The frame loop does not call the bridge. |
| `core/game_state_boot.gd` | The plain stores of `game_state_init`, `pad_defaults_set`, `boot_settings_defaults_set` and the full-name derivation of `name_entry_profile_store`, as stores into the memory. `new_game_memory(state)` applies defaults, then the bridge, then the derived names. |
| `tools/port/compare_game_memory.py` | Field-by-field report of a port RAM dump against an emulator dump, by header field name. `config/port/game-memory-known-diffs.json` gives each remaining difference its reason and ticket. |
| `port/godot/tests/support/game_memory_dump.gd` | Development dump of the memory at a route key; the playtest observer writes `<out>/game-memory/34-1-0-0.ram.bin` at the first command select. `run_dump_from_state.gd` rebuilds it from a route state file (`states/34-1-0-0.json`). |

## Using it

```sh
# regenerate and test the layout (Docker)
docker run --rm -u $(id -u):$(id -g) -v "$PWD:/work" -w /work tokimemo-recovery python3 tools/port/gen_game_memory.py
docker run --rm -v "$PWD:/work" -w /work tokimemo-recovery python3 -m unittest discover -s tools/port -p 'test_*game_memory.py'

# compare a route dump with the emulator's RAM at the same key
docker run --rm -v "$PWD:/work:ro" -v "$A:/a:ro" -w /work tokimemo-recovery python3 -I tools/port/compare_game_memory.py \
  /a/<route-out>/game-memory/34-1-0-0.ram.bin /a/<emulator-run>/<checkpoint>.ram.bin --known config/port/game-memory-known-diffs.json
```

The emulator side is `tools/emu/route_driver/run.sh` with a script whose checkpoint has `ram: true` (see [[emulator]]); `--ram-dump` of `tools/emu/run.sh` also works. A dump is game data: keep it under ignored `.asset-cache/`.

## State at the first command select (input set A)

Port memory against the emulator at 0x34/1/0/0: 133 of 159 comparable GameState fields equal (5,982 of 6,800 bytes), the 26 others and the three clock fields listed with reasons in `config/port/game-memory-known-diffs.json`. The differences are engine writes (draw arenas, VRAM loader queues, menu build, text system, overlay pointers, memory-card job) and two boot-history values (the club draw of girl 2 and `scene_work`). They belong to [[tickets/T-39301-engine-api-core]] and [[tickets/T-39302-generic-ps1-renderer]].

## Limits

- The dump is a dev observation. Seeding a run from an emulator RAM image would be dev-only and never acceptance.
- The bridge is not wired into the frame loop. Modules that move onto the memory call `push`/`pull` explicitly until their `rule_state` keys retire.
- `sprite_quads`, `bg_layers` and the other tables outside GameState are generated as globals but not initialised by `game_state_boot.gd`.
