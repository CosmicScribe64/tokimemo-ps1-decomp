# Main executable objects

Navigation map for the 34 readable C files in `src/main/`. Original ranges are half-open text ranges from [`config/objects/main.txt`](../../config/objects/main.txt). Roles follow the reviewed [main filename map](../../wiki/spec/file-naming-main.md). This is the living main prefix map required by [Phase 2 naming](../../wiki/phase2-naming.md); grouped identifier ownership is listed below. O.BIN identifiers retain their original names.

## Original objects

| Original range | File | Object role |
|---|---|---|
| `80041000–800420D0` | `game_init.c` | Display/VRAM and initial game-state setup |
| `800420D0–80042540` | `main_loop.c` | Executable entry, startup loop and frame dispatch |
| `80042540–80042A00` | `game_mode.c` | Mode dispatch and mode/stage/step updates |
| `80042A00–80043510` | `pad_input.c` | Pad/mouse polling, button mapping and reset/pause input |
| `80043510–800451D0` | `frame_render_audio.c` | Frame rendering, palette/texture uploads and sound-bank operations |
| `800451D0–80046500` | `cd_queue.c` | CD command queue, reads and XA requests |
| `80046500–80047550` | `reset_iris_wipe.c` | Reset helpers and iris/radial wipe setup |
| `80047550–800490C0` | `actor_sprites.c` | Actor sprites, texture pages and screen brightness |
| `800490C0–80049FF0` | `gpu_primitives.c` | GPU primitive setup and drawing |
| `80049FF0–8004AD60` | `gpu_effects.c` | Background layers, circles and gradient effects |
| `8004AD60–8004C3F0` | `debug_hud_palette.c` | Debug-number display and color/sepia palette helpers |
| `8004C3F0–8004E500` | `screen_layers.c` | Screen pages, SRN layers and MOD tile/glyph transfer |
| `8004E500–8004F6B0` | `text_system.c` | Text layout, glyph lookup/upload and drawing |
| `8004F6B0–8004F870` | `vram_viewer.c` | Developer display-origin viewer |
| `8004F870–80053650` | `menu_speaker_girl.c` | Menu/choice flow, speaker and girl queries |
| `80053650–80056300` | `memory_card.c` | Memory-card/save handling plus MDEC still-image buffer helpers |
| `80056300–800563F0` | `reload_executable.c` | Shutdown and executable reload (`exit_function`) |
| `800563F0–80057390` | `movie_player.c` | STR movie playback, MDEC decode and subtitles |
| `80057390–80058D20` | `background_images.c` | MDEC still-background decode, upload and display |
| `80058D20–80059710` | `model_3d.c` | 3D view/light setup and model linking/updating |
| `80059710–80059A20` | `mouse.c` | Mouse sampling, sensitivity and cursor movement |
| `80059A20–80059B40` | `debug_display.c` | Mode/stage/timer diagnostics and work-queue flag |
| `80059B40–8005A0B0` | `boot_assets.c` | Boot asset loading, pointer relocation and title setup |
| `8005A0B0–80061710` | `home_screens.c` | Settings, club, rumor and seasonal screen preparation |
| `80061710–80062CD0` | `title_screen.c` | Title models, scene load and start selection |
| `80062CD0–800674B0` | `hud_panels.c` | Portrait, parameter, calendar, message and command panels |
| `800674B0–8006CB30` | `hud_screens.c` | Calendar, save/load, appointment, club and command screens |
| `8006CB30–800737A0` | `scheduler.c` | Schedule and day-event dispatch |
| `800737A0–80075320` | `school_day.c` | School-day loop |
| `80075320–800789E0` | `holiday.c` | Days-off activities, calls, appointments and clubs |
| `800789E0–80079B10` | `overlay_loader.c` | Overlay loading/verification, TT setup and system-file operations |
| `80079B10–8007C030` | `sound_driver.c` | Audio command queue, bank loading, BGM/effects, XA voice and CD-level meter |
| `8007C030–80085E30` | `scene_library.c` | Date/scene steps, portraits, dialogue, girl selection and related utilities |
| `80085E30–80086810` | `scene_display.c` | VBlank, scroll/fade/cursor display helpers and mixed portrait/best-girl helpers |

## Prefix ownership

| ADR subsystem | Logical file(s) | Prefixes |
|---|---|---|
| Boot, startup and frame-loop helpers | `game_init.c`, `main_loop.c`, `boot_assets.c`; paused callback `boot_paused_step` in `pad_input.c` | `boot_` |
| Debug display helpers/state | Actual debug helpers in `debug_hud_palette.c` and `debug_display.c`, with shared diagnostic state | `debug_` |
| Main loop and game mode (`80042540`) | `game_mode.c` | `game_mode_`, `mode_stage_`, `mode_step_`; O.BIN `game_main` |
| Pad and system helpers (`80042A00`) | `pad_input.c`, `main_loop.c` system setup | `pad_`, `sys_` |
| 2D drawing, palettes, VRAM loads (`80043510`) | `frame_render_audio.c`, selected display helpers in `actor_sprites.c` | `draw_`, `vram_` |
| CD reads (`800451D0`, `80046500`) | `cd_queue.c`, `reset_iris_wipe.c` | `cd_` |
| Texture pages, sprites, display actors, GPU setup (`80047550`, `800490C0`) | `actor_sprites.c`, `gpu_primitives.c`, actor sequence helper `actor_pair_swap_sequences` in `scene_display.c` | `tpage_`, `sprite_`, `actor_`, `gpu_` |
| Primitives, color bars, sepia (`80049FF0`, `8004AD60`) | `gpu_effects.c`, `debug_hud_palette.c` | `prim_`, `color_` |
| Screen layers, model transfer (`8004C3F0`) | `screen_layers.c` | `layer_`, `model_`; O.BIN `srn_`, `mod_` |
| Kanji text (`8004E500`) | `text_system.c` | `text_`; O.BIN `k_disp_`, `set_kanji_` |
| Menus, speaker labels, girl records and queries (`8004F6B0`, `8004F870`) | `vram_viewer.c`, `menu_speaker_girl.c` | `menu_`, `speaker_`, `girl_` |
| Memory card, timers (`80053650`) | `memory_card.c`, frame timer callbacks in `main_loop.c` | `memory_card_`, `timer_` |
| STR movies (`800563F0`) | `movie_player.c` | `movie_` |
| Executable reload (`80056300`) | `reload_executable.c` | O.BIN `exit_function` is retained; no new prefix is assigned here |
| MDEC backgrounds (`80057390`) | `background_images.c`, two still-background decoder helpers in `memory_card.c` | `bg_`; O.BIN `dec_bg_` |
| 3D setup, mouse (`80058D20`, `80059710`) | `model_3d.c`, `mouse.c`, mouse-to-pad conversion in `pad_input.c` | `view_`, `mouse_` |
| System menus: settings, club joining, rumors (`8005A0B0`) | `home_screens.c` | `settings_`, `club_`, `rumor_` |
| Title menu (`80061710`) | `title_screen.c` | `title_` |
| HUD: portraits, parameters, calendar, message window (`80062CD0`, `800674B0`) | `hud_panels.c`, `hud_screens.c` | `hud_`, `portrait_`, `param_`, `calendar_`, `message_` |
| Scheduler and day events (`8006CB30`) | `scheduler.c` | `schedule_`, `day_event_` |
| School-day loop (`800737A0`) | `school_day.c` | `school_day_` |
| Days off: appointments, calls, clubs (`80075320`) | `holiday.c` | `holiday_`, `appointment_` |
| Overlay loader (`800789E0`) | `overlay_loader.c` | `ovl_` |
| Dates, girl selection, faces, SD sprites, dialogue, scene pictures (`8007C030`) | `scene_library.c`, `select_girl_by_scores` in `scene_display.c` | `face_`, `sd_`, `talk_`, `name_code_`, `scene_`, `select_girl_`; O.BIN `normal_date_` |
| Vsync timer, scroll, fades, finger cursor (`80085E30`) | `scene_display.c` | `vsync_`, `fade_`, `cursor_` |
| Sound driver, bank wait/error and CD-level helpers | `frame_render_audio.c`, `sound_driver.c`; leading meter helpers in `scene_library.c`; hypothesis names `audio_bank_wait_latch` in `scene_display.c` and `audio_debug_load_error_report` in `debug_display.c` | `audio_` |
| Shared records and name tables | Shared headers/data, not one object | `game_state`, `system_data`, `place_` |
