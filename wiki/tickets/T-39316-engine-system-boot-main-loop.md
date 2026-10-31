---
id: T-39316
title: Port the engine system, boot, audio-driver and main-loop files (WP-E)
status: In Progress
assignee: worker agent
created: 2026-10-30
updated: 2026-10-30
links: ["[[tickets/T-39301-engine-api-core]]", "[[engine-porting-contract]]", "[[psyq-sdk]]"]
---

## Goal

Port the remaining `src/main` files onto the engine and add the frame-timing policy the user chose, so the engine's main loop can run frames.

Files: `sound_driver.c`, `overlay_loader.c`, `game_init.c`, `boot_assets.c`, `main_loop.c`, `pad_input.c`, `mouse.c`, `title_screen.c`, `movie_player.c`, `model_3d.c`, `debug_hud_palette.c`, `debug_display.c`, `vram_viewer.c` and `reload_executable.c`.

## Frame-timing policy (user decision, 2026-10-30)

Every port frame meets its budget. GetRCnt(0xF2000001) reports a line count under `frame_scanline_limit`, so the game takes its "wait for vblank" branch every frame. VSync(0) ends the frame: the engine yields to the host frame loop and `frame_count` advances on the next frame, never inside a call. This is a documented port policy, not a measurement. It differs from the original only in frames where the PS1 itself overran its budget.

## Acceptance criteria

- [ ] Every function of these files is ported, with function-differential tests from emulator captures, or listed as untested with a reason.
- [ ] The frame-timing policy is implemented in one place (the SDK etc. module and the frame loop), documented in [[engine-porting-contract]], and switched off nowhere else.
- [ ] An engine frame loop can run the game's main loop from a state for N frames, until its first refusal, and the run reports that refusal.
- [ ] The core suite, `run_engine_diff.sh --all`, the signature check and the opening frame-timing gate pass.

## Comments
