---
id: T-39301
title: Port the main-executable engine API under its C names (M2)
status: Ready
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
