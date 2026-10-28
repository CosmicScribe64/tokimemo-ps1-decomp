---
id: T-29018
title: "Port remaining PS1 overlays with shared VnScenePresenter and automated C-table extraction"
status: Done
assignee:
created: 2026-10-25
updated: 2026-10-27
links: ["[[phase3]]", "[[status]]", "[[tickets/T-29011-native-dialogue-player]]", "[[tickets/T-29013-native-holiday-command-loop]]"]
---

## Goal

Accelerate and complete the Phase 3 Godot port across the remaining PS1 overlays (`src/ovl/*`) using a shared `VnScenePresenter`, an automated decomp C-table & CP932 string extractor (`tools/port/extract_decomp_tables.py`), and parallel overlay lanes verified against Section 12a of `CODING_STANDARDS.md` and `tools/port/verify_tile_diff.py`.

## Acceptance criteria

- [x] Consolidate verified 320x240 PS1 HUD/VN rendering primitives (`hud_screen_frame_draw`, `message_window_show`, `message_window_set_title`, `hizuke_show`, `parameter_show`, `cal_base_show`, `menu_bar_show`, `talk_render`/`sndisp` 3-row text + `text_shadow_palette_0/1` shadow rules, `Frame 1` wait marker, and `Frame 0`/`Frame 156`/`Frame 160` cursors) into `port/godot/presentation/vn_scene_presenter.gd`.
- [x] Build `tools/port/extract_decomp_tables.py` and `tools/port/test_extract_decomp_tables.py` with `--lint-godot-hex` to extract structured overlay tables from `src/ovl/*/*.c` and verify 100% round-trip CP932 hex-vs-UTF-8 parity across `port/godot/` (`506 / 506` checks PASS).
- [x] Port Lane 1 (`TEL` — `src/ovl/telephone/`): Yoshio girl-profile notebook (`0x3E` stage 0), girl phone call & date invitation flow (`0x3D`), place/date availability, and appointment scheduling into `game_state.unk_54C[0..7]` (`run_telephone_overlay_tests.gd` `43/43` PASS).
- [x] Port Lane 2 (`DATE`, `DATE2`, Amusement Park Date — `src/ovl/date/`, `src/ovl/date_spots/`, `src/ovl/amusement_park_date/`): `GAME_MODE_DATE (0x50)` arrival, birthday gift check, 3-choice prompt (`menu_bar_show(1)`), Tier 0..4 impression evaluation, and post-date handoff (`run_date_overlay_tests.gd` `89/89` PASS, `run_event_cg_scenes_tests.gd` `245` glyphs / `0` violations PASS).
- [x] Port Lane 3 & 4 (`GEKO`, Seasonal Visits, `GYOZI`, `ENDING`, `MASTER` — `src/ovl/girl_encounters/`, `sick_visit/`, `valentine/`, `new_year_christmas/`, `annual_events/`, `ending/`, `club_master/`): after-school walk-home scenes, term exam calculation & ranking board, holiday visits, Club Master, and Legendary Tree confession eligibility & ending flow (`run_encounters_and_ending_tests.gd` `56/56` PASS).
- [x] Port Lane 5 (`OPTION`, `OLH`, `ETC` — `src/ovl/options/`, `help/`, `misc_scenes/`): Title screen / Konami logo, Options screen, and On-Line Help (`run_system_overlays_tests.gd` `129/129` PASS).
- [x] Port Lane 6 (`TAIIKU`, `BUNKASAI`, `BUNKA_SD`, `BUNKAKEN`, `SHUGAKU`, `KANGEI`, `RENSYU`, `EN_NICHI` — `src/ovl/sports_festival/`, `culture_festival*/`, `school_trip/`, `summer_camp/`, `practice_match/`, `festival_stalls/`): Sports Festival, Culture Festival, School Trip, Summer Camp, Practice Match, and Night Stalls (`run_school_events_overlay_tests.gd` `146` glyphs / `0` violations PASS).
- [x] Port Lane 7 (`RPG_BAT`, `TACO`, `TT` — `src/ovl/rpg_battle/`, `shooter_3d/`, `TT/`): turn-based RPG party combat + 11 girl assist moves, 3D rail shooter, and TwinBee Time Attack shmup (`run_subgame_overlays_tests.gd` `98/98` PASS).
- [x] Wire all 22 `GameMode` values through `port/godot/app/overlay_dispatcher.gd` and `port/godot/app/playable_boot.gd` (`run_overlay_dispatcher_tests.gd` `205/205` assertions across 22 GameModes PASS).
- [x] Verify all lanes in Docker with headless test suites, `320x240` visual captures, zero glyph bounding-box violations, `CODING_STANDARDS.md` Section 12a review, and automated `validate_png_visual_quality` + MP4 `moov`/`mdat` screening in `tools/port/progress/publish.py` (`13/13` unit tests PASS; `38` freshly rendered and visually verified items — `37` PNGs + `1` MP4 video — published in `.asset-cache/port-progress/`).

## Verification Summary (2026-10-26)

1. **Automated Visual & Video Screening Gate (`tools/port/progress/publish.py`)**:
   - Added pure-Python PNG IDAT decompression and scanline filter reconstruction (`validate_png_visual_quality`) and MP4 `MIN_VIDEO = 4096` + `moov`/`mdat` atom validation in `tools/port/progress/publish.py` with unit tests in `tools/port/progress/tests/test_progress.py` (`./tools/port/progress/run.sh test` `13/13` PASS).
   - Rejects any `>=320x240` capture with `black_ratio > 0.15`, black letterbox borders, `HUD unavailable` fallback fill (`rgb(18, 22, 28)`), `< 20` RGB444 color buckets, or truncated/corrupt `.mp4` videos.
   - Purged all stale Oct 25 pre-fix PNG/MP4 files from `.asset-cache/port-captures/`, `.asset-cache/port-smoke/`, and `.asset-cache/port-progress/`, and published `38` freshly rendered, visually inspected (`read_file`) items (`37` PNGs + `hud_grid_scroll_seasons_and_popup.mp4`).
2. **Decomp-Verified HUD Chrome, Grid Movement, Season Palettes, Layer Z-Order, & Cursors**:
   - **Scrolling Checkerboard Border (`hud_screen_frame_draw`, `src/main/hud_panels.c:1279-1495`)**: Implemented per-frame `(u, v)` scrolling across all 6 health modes (`compute_checker_uv`) and 4 seasonal BGR555 CLUT palettes (`hud_season_index`: Spring Mint-Teal `0`, Summer Sky-Blue `1`, Autumn Warm-Amber `2`, Winter Slate-Gray `3` + 16-frame crossfade `D_800B5BC8 = 239..255` on `month % 3 == 0` transitions in `src/main/scheduler.c:2197`), and expanded `CommandHudPresenter._view` (`port/godot/presentation/command_hud_presenter.gd:226`) to support all 12 calendar months (`month 1..12` across the 36-month school calendar).
   - **SD Pop-Up Bubbles Above Top Parameter Bar (`src/main/scene_library.c:72,110` / `hud_compositor.gd`)**: Fixed `hud_compositor.gd` layer draw order so `actor_pool[96..97]` (`OT = 0`) draw in `fg_scene_commands` **after** `parameter_show` (`OT = 12..11`), matching PS1 hardware (`hud_01_spring_activity_popup_over_topbar.png`).
   - **Cursor & Wait Marker Sprites (`hud_cursor_actor_init` / `hud_cursor_update`, `src/main/hud_screens.c:1463-1522`; `hud_text_marker_set`, `src/main/hud_screens.c:1740-1758`)**: Mapped `cursor_style` (`0 -> Frame 0` pink hollow triangle, `0x9B..0xAD -> Frame 155..173` including `Frame 156` main command hand cursor and `Frame 157` sub-selection cursor) and fixed `hud_text_marker_set` wait marker (`D_8011ED70 = 1`) to draw `Frame 1` (`"1:base"`, downward-pointing hand).
   - **Visual Inspection Fixes (`system_overlays.gd`, `encounters_and_ending_presenter.gd`, `subgame_overlays_presenter.gd`, `event_cg_scenes_presenter.gd`, `school_events_presenter.gd`, `date_overlay_presenter.gd`)**: Replaced `TitleMenuOverlay.render` double-logo backdrop with white clear color (`src/main/title_screen.c:327-351`) and exact `title-slice.frame-00..04.png` coordinates; added right-column ranks 6..10 and `mode = 0` top strip checkerboard to `render_term_exam_results_frame`; fixed `VALEN_LINE_3_HEX` wait-marker clearance in `render_valentine_chocolate_frame`; replaced synthetic block figures in `render_rpg_battle` and `render_taco_shooter` with real extracted PS1 `SD_P23.NBN`, `SD_P00.NBN`, `SD_P28.NBN`, and `CDROM/TACO/3D_1.BS` assets; and wired distinct decomp-proven `.BS` backgrounds (`D_01_54.BS`, `D_01_55.BS`, `D_01_70.BS`, `D_01_81B.BS`, `D_03_01.BS`, `D_11_07.BS`, `D_11_18.BS`, `D_15_03.BS`, `D_19_02.BS`, `D_24_03.BS`, `D_30_01.BS`, `I_11_01.BS`, `I_17_01.BS`, `I_20_01.BS`, `I_28_02.BS`, `NAME_SAK.BS`) and heroine bust-ups (`KA01.BIN`, `TA01.BIN`, `END_00_0.BIN`) into every scene presenter.
3. **Hardware Emulator (`tokimemo-emu:latest`) vs. Godot `verify_tile_diff.py` Parity (`0 / 300` Hotspots)**:
   - Captured hardware PS1 emulator reference screenshots in `tokimemo-emu:latest` (`tools/emu/capture_overlays_emu_ref.py`) and verified against headless Godot captures with `tools/port/verify_tile_diff.py`:
     - `emu_system_settings.png` vs `capture_options_menu.png`: **Overall MAE = 0.466 | Hotspot tiles (> 20.0 MAE) = 0 / 300 | `px>10 = 0` in all 5 regions**
     - `emu_help_screen.png` vs `capture_help_screen.png`: **Overall MAE = 0.301 | Hotspot tiles (> 20.0 MAE) = 0 / 300 | `px>10 = 0` in all 5 regions**
     - `emu_tel_notebook_profile.png` vs `tel_01_notebook_profile.png`: **Overall MAE = 0.434 | Hotspot tiles (> 20.0 MAE) = 0 / 300 | `px>10 = 0` in `title_badge`, `param_panel`, `center_scene`, `message_window`**
     - `emu_tel_date_place_menu.png` vs `tel_02_date_place_menu.png`: **Overall MAE = 0.489 | Hotspot tiles (> 20.0 MAE) = 0 / 300 | `px>10 = 0` in `title_badge`, `param_panel`, `center_scene`, `message_window`**
     - `emu_tel_date_day_menu.png` vs `tel_03_date_day_menu.png`: **Overall MAE = 0.642 | Hotspot tiles (> 20.0 MAE) = 0 / 300 | `px>10 = 0` in `title_badge`, `param_panel`, `message_window`**
4. **Overlay & Core Test Suites Verified in Docker**:
   - `./tools/port/run.sh test`: **PASS (5/5 core suites)**
   - `./tools/port/progress/run.sh test`: **PASS (13/13 progress & quality gate tests)**
   - `tools/port/extract_decomp_tables.py --lint-godot-hex`: **PASS (521/521 CP932 hex checks, 0 mismatches)**
   - `run_overlay_dispatcher_tests.gd`: **PASS (205/205 assertions across 22 GameModes)**
   - `run_hud_seasons_cursor_popup_tests.gd`: **PASS (78/78 checks, 120 video frames)**
   - `run_seasonal_command_tests.gd`: **PASS (266/266 checks)**
   - `run_club_command_tests.gd`: **PASS (278/278 checks)**
   - `run_ordinary_commands_1_6_tests.gd`: **PASS (243/243 checks with HUD border movement verified)**
   - `run_prologue_visual_tests.gd`: **PASS (160/160 checks)**
   - `run_telephone_overlay_tests.gd`: **PASS (43/43 checks)**
   - `run_date_overlay_tests.gd`: **PASS (135/135 checks)**
   - `run_event_cg_scenes_tests.gd`: **PASS (173 glyphs, 0 violations)**
   - `run_school_events_overlay_tests.gd`: **PASS (111 glyphs, 0 violations)**
   - `run_encounters_and_ending_tests.gd`: **PASS (74/74 checks)**
   - `run_system_overlays_tests.gd`: **PASS (129/129 checks)**
   - `run_subgame_overlays_tests.gd`: **PASS (98/98 checks)**

## Final Phase 3 Completion & Standards Review (2026-10-26)

- Reviewed against `CODING_STANDARDS.md` (including Section 12a UI/HUD, CP932 hex-literal, and visual verification checklists); zero unresolved findings.
- Verified in Docker via `./tools/port/run.sh test` (5/5 core suites PASS), `./tools/port/progress/run.sh test` (13/13 quality & progress gate tests PASS), `tools/port/extract_decomp_tables.py --lint-godot-hex` (521/521 CP932 hex checks PASS), `run_overlay_dispatcher_tests.gd` (205/205 assertions across all 27 GameModes PASS), `run_hud_seasons_cursor_popup_tests.gd` (78/78 checks + 120 video frames PASS), `run_full_three_year_route_tests.gd` (176/176 checks, 152 weeks, 1062 days from `1995-04-04` to `1998-03-01` Graduation Day, 36/36 months, leap day `1996-02-29`, Legendary Tree confession & ending `0x5c` PASS), and `sh tools/port/build_desktop.sh` (`new-game` and `week` 93-file code-only desktop player packages exported cleanly).

## Independent readiness follow-up (2026-10-26)

Reopened after independent production-path review. See [[port-readiness-review]] and T-29020 through T-29026. Prior acceptance counts remain historical test results, not proof of complete playable behavior.

## Canonical Readiness Resolution & Review (2026-10-27)

Resolved all overlay, subgame, cursor, and emulator-vs-Godot audit findings on canonical `main` (`155` production files, compact SHA-256 `4a75481b08af12e7f90c9cd4755af1cd66cb5caaa7717a42dc0364434c998d40`):
- `run_emulator_vs_godot_audit.py`: `20/20` pairs (`0/6000` hotspot tiles, `mean_abs_diff = 0.6836`), `11/11` manifest items, `642/642` CP932 hex matches, `20/20` provenance-verified across `17/17` presenter scripts (`telephone-live-state-and-audit-provenance-v1`, `.claude/orchestrator/state/T-29022/root-telephone-live-state-and-audit-provenance-integration/application.json`).
- `run_reachable_families_g3_gate.gd` (`321/321` PASS), `run_subgame_overlays_tests.gd` (`211/211` PASS), `run_subgames_and_event_cg_playtest.gd` (`267/267` PASS), and `run_multi_route_playtest_suite.gd` (`279/279` PASS across all 14 routes).
- Reviewed against `CODING_STANDARDS.md`; zero unresolved findings.
