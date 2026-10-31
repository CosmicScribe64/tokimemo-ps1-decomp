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

2026-10-30 (WP-A worker, branch `port/m2-engine-a-draw-core`): all eight files are ported on the engine (114 functions: 9 gpu_primitives, 7 gpu_effects, 11 screen_layers, 21 actor_sprites, 26 frame_render_audio, 13 background_images, 25 text_system, 2 reset_iris_wipe). Hand checks for every function (`tests/engine/*_test.gd`), differential hits from emulator captures for everything the available save states reach (title, school first line, prologue, birthday event, name entry), the rest listed `untested` with a reason in `config/port/engine-diff/<file>.json`. Gates after the last code change: core suite PASS (11 suites), `run_engine_diff.sh --all` RESULT: PASS, `check_engine_signatures.py` 148 checked 0 errors, opening frame timing pad 2: 104 substeps, 6709 frames to 12/0/0, 0 mismatches. Per-package notes below.

Missing data symbols (name, address, type, C file; the module uses a named constant until the layout has them):
- `D_800B3D28` 0x800B3D28 u8, `D_800B3D2C` 0x800B3D2C u8, `D_800B3D4C` 0x800B3D4C u8, `D_800B3D50` 0x800B3D50 u8: `src/main/frame_render_audio.c` (sound-bank load flags and poll counter; addresses from the `.data` order of the C file). Unused declared neighbours: `D_800B3D14`, `D_800B3D18`, `D_800B3D1C`.
- `D_800B5934` 0x800B5934 s32: `src/main/background_images.c` (cleared by `dec_bg_reset`).
- `D_800B4030`, `text_font_range_table_end`: `src/main/text_system.c` (INCLUDE_RODATA only, never used by a function).

Requests for the orchestrator (not done by this worker): see "Facade and SDK requests" in the package notes.

## Package notes (WP-A draw core)

### Decisions

- **SDK and cross-package calls are `call_c` and refuse by name.** No module ports libgpu, libgte, libetc, libc `rand`/`printf` or the sound driver, so every such call (`AddPrim`, `SetSemiTrans`, `SetShadeTex`, `func_8009F040/F004/F054/F068/F07C/F090/F0A4/F0B8/F0F4/F11C` (the Set* primitive initialisers), `func_8009D294` (SetDrawMode), `func_8009ECB0` (GetTPage), `func_8009ED74` (GetClut), `func_8009C674` (DrawSync), `func_8009C7F8/C884/C8E0/C93C` (ClearImage, LoadImage, StoreImage, MoveImage), `func_80098650`, `func_80098680/B80/D00` (sprite sorting), `func_80099540 ... 8009AD00`, `func_800AD950` (VSync), `rand`, `printf`, `audio_command_submit`, `audio_bgm_bank_ready`, `bg_still_*`, `disp_string` callers' neighbours) is replayed by the differential harness from the capture. Drawing therefore builds primitives and ordering-table links in RAM exactly as the C does; nothing reaches `engine.renderer` yet because the SDK calls that would feed it have no owner (see requests).
- `rsin` and `abs` run inline: `core/native_math.gd` already ports `rsin` from the asm (`Math.rsin`), and `abs` is `absi`; both are pure leaves.
- **Text** (`text_system.c`) hands nothing to a text layer: `set_kanji_string` queues into `text_list`, glyphs go to VRAM through `LoadSquare`, drawing is SPRT_8 primitives. `engine.text_layer` is not used.
- **Audio** (`frame_render_audio.c`): `audio_command` is ported and calls `audio_command_submit` (`sound_driver.c`, not in this package), so `cd_read_sectors` now refuses one call deeper (`audio_command -> audio_command_submit`); `tests/engine/cd_queue_test.gd` was updated for that. The native audio bridge is reached once `sound_driver.c` is ported on `engine.audio`.
- **Uninitialised C locals are refused, not invented:** `srn_tpage_show` with a colour mode outside 0..2 (`divisor`, `pageWidth`), `actor_draw_all` when `quad_flags & 0x03000000` is 1..3 before any `tpage` was assigned. `actor_draw_all`'s first `search_tpage(0x7FFF, i, 0)` reads its stale stack slot at `+0x90` like the original (`i` is uninitialised there).
- Struct arguments by value: `prim_fill_rect(arg0, RECT, arg3)` and `set_k_work(entry_index, TextEntry)` take the o32 register words (`rect_xy`, `rect_wh`; two words of the entry); `C_SIGNATURES` carries `"c_params": N` for the C parameter count and `tools/port/check_engine_signatures.py` accepts it (the one change outside the worker's own files).
- K&R callee mismatches: `audio_bgm_stop` passes a third argument to `audio_bank_start` and `set_kanji_string` three to `load_string`; the callees take 2 and 1 and ignore the rest, so the ports call with the declared count.
- `--ptr-bytes 8` for the captures that pass a stack `RECT` (`wpa-fr*`, `wpa-bg*`, `wpa-tx*`, `wpa-ge2`): the bytes behind a stack pointer include the callee's own argument-home spills (`sw a0..a3` into the caller frame), which the port must not write (the harness excludes that area one-sidedly).

### Captures used (game data, under `.asset-cache/m2-engine/`, never committed)

`wpa-gp1`, `wpa-gp2` (gpu_primitives), `wpa-ge1`, `wpa-ge2` (gpu_effects), `wpa-sl1` (screen_layers), `wpa-as1`, `wpa-as2` (actor_sprites), `wpa-fr1`, `wpa-fr2` (frame_render_audio), `wpa-bg2`, `wpa-bg4` (background_images), `wpa-tx1`, `wpa-tx2` (text_system). States: `restored-title.sstate` (900 frames, START at frame 40), `original-school-firstline-v1.sstate`, `evt-player-birthday-entry-v1.sstate`, `name-player-confirm-v1.sstate`. No constructed (poked) states.

### Differential results (hits passed per function; bytes compared 2 MiB per hit minus exclusions)

- gpu_primitives: SetWorkBase 4, GetWorkBase 8, goto_tpage 6, safe_env 6, dtd_on_tpage 6, dtd_on 6, prim_gradient_rect 16. Untested: `sprite_set_gpu_poly_ft4` (no caller in src/), `_sprite_set_light_effect1` (date and girl-encounter overlays only). prim_gradient_rect reached 189 of 363 instructions (the other colour modes are hand-checked).
- gpu_effects: prim_bg_slot_setup 2, prim_flash_gray 6. Untested: prim_gradient_line, prim_fill_rect, prim_disc_tiles, prim_glow_disc, prim_flash_color_wide (overlay-only callers).
- screen_layers: srn_tpage_show 6, move_255_line 1. Untested: srn_vram_set, mod_trans_vram, get_fnt, func_8004DAC4, mod_set_gpu, mod_set, srn_set, srn_init, layer_tileset_init.
- actor_sprites: actor_draw_all 12, tpage_buf_clear 1, _sys_default_tpage_set 1, load_tpage_buf_lock 6, search_tpage 16, search_tpage8 12, actor_draw_enable 1, actor_deactivate 1, actor_clear_upper 1, actor_clear 12, draw_brightness_get 8, draw_brightness_step 12. Untested: draw_shake_start/update, tpage_buf_clear_all, search_load_tpage_buf_lock, search_tpage_multi, tpage_buffer_print, actor_clear_range, draw_brightness_reset/set_rate.
- frame_render_audio: draw_frame 16, draw2d3d 1, back_clear_switch 2, load_palette 5, load_csr_tp 4, csr_load_vram 12, palette_load_vram 12, LoadSquare 12, MoveSquare 4, audio_command 7, audio_bank_load_start 1, audio_bank_transfer_start 1, audio_bank_load_poll 6, audio_bank_transfer_done 1, audio_bank_start 1. Untested: load_csr_ab, StoreSquare, func_80044434, draw_time_bars, vram_palette_row_copy, audio_bgm_stop/is_playing/current_set, audio_bank_state_reset, audio_bank_transfer_poll, audio_bgm_play_short.
- background_images: dec_bg_draw 16. Untested: the other 12 (set_dec_bri, get_last_gamen_mode, dec_bg_reset, dec_bg_show_switch/set, dec_bg_cd_read, dec_bg_decode_to_vram, bg_capture_screen, bg_wipe_step, bg_fade_set, bg_fade_draw, bg_crossfade_draw): none of the sampled states calls them.
- text_system: text_glyph_pages_clear 3, text_system_reset 3, text_set_advance 1, set_kanji_string 12, k_disp_start 3, get_k_work 6, k_reset 1, k_sub_reset 1, k_sub_disp_start 1, k_speed_set 1, k_disp_inc 16, check_set_k 2, get_now_k 6, disp_string 16, load_string 12, vload_font 16, text_glyph_expand 16. Untested: text_unused_stub, k_disp_switch, get_ksys, set_k_work, k_sub_reset_point_set, k_disp_goto_line_end, get_k_speed, check_end_k.
- reset_iris_wipe (the two new functions): both untested (school_trip and file-select overlays only); `tests/engine/reset_iris_wipe_draw_test.gd` covers both.

A hit whose original reached only part of a function leaves unreached blocks (not a gate); the hand checks cover every `switch` case, wrap and early out the C has, and the refusal paths.

### Facade and SDK requests (not made by this worker)

1. A libgpu/libgte/libetc SDK owner (a module the facade registers, since `engine.gd` is the orchestrator's): the Set* primitive initialisers, `AddPrim`/`SetSemiTrans`/`SetShadeTex`, `GetTPage`/`GetClut`/`SetDrawMode`, LoadImage/StoreImage/MoveImage/ClearImage, DrawSync/VSync (a blocking-wait model per the contract), `DrawOTag` (`func_8009AD00`) and the sprite sorters (`func_80098680/B80/D00`). Until it exists draw functions run in the harness against recorded callees and refuse by name in the game; once it exists, `engine.renderer` gets its primitives from the ordering tables these ports fill.
2. `rand` and `printf` (libc module; `printf` formatting for `actor_deactivate`, `actor_draw_all` debug lines).
3. Other C files these ports call and nobody owns yet: `sound_driver.c` (`audio_command_submit`, `audio_bgm_bank_ready`), `pad_update`, `view_models_update_draw`, `bg_still_buffers_set`, `bg_still_decode_to_vram`.
4. Container names: `tools/emu/func_capture/run.sh` names its container `m2fn-<capture name>` (this worker used capture names `wpa-*`) and `run_engine_diff.sh` uses `m2diff-$$`; ad-hoc runs used a wrapper named `wpa-diff`.
