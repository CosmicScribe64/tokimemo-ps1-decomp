---
id: T-29018
title: "Port remaining PS1 overlays with shared VnScenePresenter and automated C-table extraction"
status: In Progress
assignee:
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]", "[[status]]", "[[tickets/T-29011-native-dialogue-player]]", "[[tickets/T-29013-native-holiday-command-loop]]"]
---

## Goal

Accelerate and complete the Phase 3 Godot port across the remaining PS1 overlays (`src/ovl/*`) using a shared `VnScenePresenter`, an automated decomp C-table & CP932 string extractor (`tools/port/extract_decomp_tables.py`), and parallel overlay lanes verified against Section 12a of `CODING_STANDARDS.md` and `tools/port/verify_tile_diff.py`.

## Acceptance criteria

- [ ] Consolidate verified 320x240 PS1 HUD/VN rendering primitives (`hud_screen_frame_draw`, `message_window_show`, `message_window_set_title`, `hizuke_show`, `parameter_show`, `cal_base_show`, `menu_bar_show`, `talk_render`/`sndisp` 3-row text + `text_shadow_palette_0/1` shadow rules, `0xF0` wait marker, and `Frame 156` hand cursor) into `port/godot/presentation/vn_scene_presenter.gd`.
- [ ] Build `tools/port/extract_decomp_tables.py` and `tools/port/test_extract_decomp_tables.py` with `--lint-godot-hex` to extract structured overlay tables from `src/ovl/*/*.c` and verify 100% round-trip CP932 hex-vs-UTF-8 parity across `port/godot/`.
- [ ] Port Lane 1 (`TEL` — `src/ovl/telephone/`): Yoshio girl-profile notebook (`0x3E` stage 0), girl phone call & date invitation flow (`0x3D`), place/date availability, and appointment scheduling into `game_state.unk_54C[0..7]`.
- [ ] Port Lane 2 (`DATE`, `DATE2`, Amusement Park Date — `src/ovl/date/`, `src/ovl/date_spots/`, `src/ovl/amusement_park_date/`): `GAME_MODE_DATE (0x50)` arrival, birthday gift check, 3-choice prompt (`menu_bar_show(1)`), Tier 0..4 impression evaluation, and post-date handoff.
- [ ] Port Lane 3 & 4 (`GEKO`, Seasonal Visits, `GYOZI`, `ENDING`, `MASTER` — `src/ovl/girl_encounters/`, `sick_visit/`, `valentine/`, `new_year_christmas/`, `annual_events/`, `ending/`, `club_master/`): after-school walk-home scenes, term exam calculation & ranking board, holiday visits, Club Master, and Legendary Tree confession eligibility & ending flow.
- [ ] Port Lane 5 (`OPTION`, `OLH`, `ETC` — `src/ovl/options/`, `help/`, `misc_scenes/`): Title screen / Konami logo, Options screen, and On-Line Help.
- [ ] Port Lane 6 (`TAIIKU`, `BUNKASAI`, `BUNKA_SD`, `BUNKAKEN`, `SHUGAKU`, `KANGEI`, `RENSYU`, `EN_NICHI` — `src/ovl/sports_festival/`, `culture_festival*/`, `school_trip/`, `summer_camp/`, `practice_match/`, `festival_stalls/`): Sports Festival, Culture Festival, School Trip, Summer Camp, Practice Match, and Night Stalls.
- [ ] Port Lane 7 (`RPG_BAT`, `TACO`, `TT` — `src/ovl/rpg_battle/`, `shooter_3d/`, `TT/`): turn-based RPG party combat + 11 girl assist moves, 3D rail shooter, and TwinBee Time Attack shmup.
- [ ] Verify all lanes in Docker with headless test suites, `320x240` visual captures, zero glyph bounding-box violations, and `CODING_STANDARDS.md` Section 12a review before merging and publishing to `.asset-cache/port-progress/`.
