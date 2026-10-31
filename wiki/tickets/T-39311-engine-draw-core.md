---
id: T-39311
title: Port the engine draw core (WP-A)
status: In Progress
assignee: worker agent
created: 2026-10-30
updated: 2026-10-30
links: ["[[tickets/T-39301-engine-api-core]]", "[[engine-porting-contract]]"]
---

## Goal

Port `text_system.c`, `frame_render_audio.c`, `background_images.c`, `actor_sprites.c`, `gpu_primitives.c`, `gpu_effects.c`, `screen_layers.c` and the two unported functions of `reset_iris_wipe.c` onto the engine, following [[engine-porting-contract]].

## Acceptance criteria

- [ ] Every function of these files is ported, with function-differential tests from emulator captures, or listed as untested with a reason.
- [ ] Drawing goes through the generic renderer, text through the text layer, and audio through the native audio engine.
- [ ] The core suite, `run_engine_diff.sh --all` and the opening frame-timing gate pass.

## Notes

## Comments
