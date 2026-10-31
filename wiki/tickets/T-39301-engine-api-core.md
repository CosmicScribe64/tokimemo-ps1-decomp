---
id: T-39301
title: Port the main-executable engine API under its C names (M2)
status: In Progress
assignee:
created: 2026-10-30
updated: 2026-10-30
links: ["[[port-completion-plan]]", "[[tickets/T-39300-game-memory-model]]"]
---

## Goal

Port the main-executable functions that overlays call, one GDScript file per C file (`port/godot/engine/`), on the game memory model. This is layer 2 of [[port-completion-plan]].

## Acceptance criteria

- [ ] The top 100 engine functions by overlay call sites (94% of 26,381 calls) are ported under their C names, each with a function-differential test from emulator captures.
- [ ] Text, audio and VRAM calls are backed by the existing text layer, native audio engine and upload bridge.
- [ ] The existing `rules/` modules for these C files are moved into the layout without changing route results.

## Notes

Most of these functions live in `scene_library.c`, `hud_panels.c`, `menu_speaker_girl.c`, `text_system.c`, `scene_display.c`, `frame_render_audio.c`, `hud_screens.c`, `memory_card.c`, `cd_queue.c`, `background_images.c`, `actor_sprites.c`, `game_mode.c` and `gpu_primitives.c`.

## Comments

2026-10-30 (base worker, branch `port/m2-engine-base`): the engine layout, the contract and the tier-1 harness are in; `src/main/game_mode.c` and `src/main/cd_queue.c` are ported on them. Contract and commands: [[engine-porting-contract]].

- Layout: `port/godot/engine/engine.gd` (facade: game memory, disc pack, renderer, CD drive model, text-layer and audio slots, module discovery, `call_c`, refusals), one module per C file (`game_mode.gd`, `cd_queue.gd`, `reset_iris_wipe.gd`), `_cd_drive.gd` (libcd replacement for `cd_queue.c`). An unported callee latches a refusal naming caller and callee and halts the engine; nothing is invented.
- Harness: `tools/emu/func_capture/{run.sh,pack.sh,capture.lua,pack.py}` capture N hits of a function (arguments, stack words, 2 MiB RAM at entry, v0/v1 and the RAM diff at return, plus the effects of listed unported callees) under `.asset-cache/m2-engine/`; `tools/port/run_engine_diff.sh` replays them against the ported functions (`tests/engine/engine_diff*.gd`), comparing v0 and the whole RAM and naming differing addresses by GameState field or global. Specs: `config/port/engine-diff/*.json`. Hand checks: `tests/engine/*_test.gd` (in the core suite).
- `game_mode_reset_to_title` is defined in `src/main/reset_iris_wipe.c`; it is ported in `engine/reset_iris_wipe.gd` with `cd_decimal_digit` and `cd_binary_coded_decimal_to_integer` (`cd_iris_texture_build` and `cd_iris_wipe_draw` stay unported).
- Captures used (all from `build/emu/states` and `.asset-cache/emu*` states; `gm-reset` is a constructed state: bad game mode 0x99 poked at vsync 30 of `restored-title.sstate` to reach `game_mode_reset_to_title`): `probe1`, `gm-route`, `gm-evt`, `gm-reset` (game_mode), `cd-title`, `cd-xa-a`, `cd-xa-b`, `cd-route`, `cd-evt`, `cd-xa` (cd_queue, reset_iris_wipe).
- The harness found three things worth keeping: K&R `u8` arguments are spilled with `sw` into the caller's o32 argument home (excluded by default, with its reason); `cd_command_push` copies 16 bytes of a 4-byte parameter, so the stale stack bytes had to be reproduced (`engine.push_frame`, buffer at `frame + 0x28`); the capture of a pump hit that lists `VSync` as a callee records libcd's own VSync calls, which the port does not make (list a callee only when the C source calls it).
- Not done: acceptance criteria of the ticket (top 100 functions, text/audio/VRAM backing, `rules/` moved into the layout). Remaining files: see the work-package proposal in the log entry of 2026-10-30 (engine base report).
