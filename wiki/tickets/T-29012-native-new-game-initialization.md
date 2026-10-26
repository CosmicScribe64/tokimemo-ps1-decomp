---
id: T-29012
title: "Port native New Game initialization and profile rules"
status: Done
assignee: port-new-game-agent
created: 2026-10-25
updated: 2026-10-26
links: ["[[phase3]]", "[[tickets/T-29002-native-runtime-and-oracle]]"]
---

## Goal

Implement source-backed New Game initialization and profile-confirmation rules so the port can eventually start without a captured emulator state. This contributes to P3.3; it does not replace title, name-entry UI, voiced prologue or their acceptance gates.

## Owned scope

Additive `port/godot/rules/new_game.gd`, focused tests and any strict original-table adapter needed by those rules. Root owns canonical application; prepare an ignored immutable packet. Do not modify the shared controller, app, schemas, existing rules or translation pipeline.

## Acceptance

- Port actual `src/main/game_init.c` initialization and source-reachable profile rules, preserving integer widths, RNG calls/order and native behavior including unusual blood-type ranges.
- Read source tables through strict original-input adapters; no extracted data in Git or res:// and no invented production defaults.
- Produce an explicit complete owned-state projection and precise unsupported dependencies; do not pass a partial snapshot as a ready command boundary.
- Compare original initialization/profile-confirmation states and RNG endpoints through native evidence or reproducible emulator recipes. Synthetic tests supplement those comparisons.
- Reject invalid input and missing content before changing caller state; test independent initialization and replay.
- Independent Standards and native/Spec reviews, resolved findings and root integration are required before Done.

## Limits

Native Japanese name-entry limits remain the Phase 3 behavior. Later translation layout is separate. Gemini owns all translation wording and its pipeline. A completed initializer alone is not a complete New Game feature.

2026-10-25 v2 review checkpoint: original-function comparisons pass nineteen cases at author scope, but Standards review identified original girl birthday/blood arrays embedded in GDScript. The Phase 3 external-content boundary applies to those character initialization values; no exception is approved. Author is preparing a separate v3 that moves them into source/native-bound external content. Preserve v2 evidence and rerun endpoint comparisons after the adapter change. No canonical application or Done status.

2026-10-25: root integrated the four reviewed New Game initializer/original-table adapter files, composing v3 with the v4 leading-double-slash path correction. Both independent review axes pass. Actual installed code passes 109 Godot checks, 13 Python tests and all 19 preserved original full-modeled-state/profile/RNG comparisons; five existing core/content/rule/week suites pass. Exact hashes/review binding: `.claude/orchestrator/state/T-29012/v4-path-fix/live-application.json`. These are function-level native comparisons over 0x1A8C modeled bytes, not a fresh normal New Game capture or proof of the final four unowned GameState bytes. Profile-entry UI and opening/prologue routing continue separately; no captured-gameplay default is introduced.

2026-10-25: integrated seven independently reviewed additive files for the separate New Game name/profile entry scene. Exact packet `a6c643993e2c71aa14e0484ea9e9d016d2d236ab0455a1e4d4c8372eae9bb730`; installed receipt `.claude/orchestrator/state/T-29012/ui-route-v1-root-review/live-application.json`. Canonical checks pass: 63 controller cases, five invented-input adapter cases and eight actual Godot/Mesa UI checks. Independent Spec review additionally decoded all 34 kana-row pointers and 45 preset entries and checked actual input/focus behavior. Original text/font/presets remain authenticated external data. The scene performs name completion and birthday finalization at their source actions and preserves explicit unknown state; it stops at the prologue. This is a development action flow, not native UI layout/timing, a complete opening, or a ready gameplay session. The author continues source-backed prologue and optional explicit START-skip routing; normal story remains required.

2026-10-25: applied the finite four-file v2 correction discovered after the v1 integration. Native CIRCLE-on-DONE sets tab 4 and the next name initialization resets to hiragana; keyboard START retains the selected kana tab. The UI now distinguishes the Done button from START and consumes Enter before a focused Godot Button can activate itself. Independent Spec and root finite Standards review pass; actual canonical 85-controller and ten-rendered-UI checks pass. Exact receipt `.claude/orchestrator/state/T-29012/ui-route-v2/root-application.json`. Original decoders/content packs are unchanged; old captures/desktop exports remain historical v1 evidence, not proof of the corrected keyboard behavior.

## Bank0 loading control integrated (2026-10-25)

Applied the four-file prologue bank provider after independent Standards/source review PASS. The loader preserves original cache decisions, request ordering, byte-width counters, readiness/timeout predicates and ordered audio operations. Its content adapter verifies original source calls and ISO identities and exports authenticated metadata outside the project. Root installed tests pass: 24 Godot control checks and six Python checks in both normal and optimized execution. Review independently reproduced ten ISO bindings, two sequence headers and the external pack. Evidence: `.claude/orchestrator/state/T-29012/prologue-bank-provider-v1/{spec-standards-review,canonical-application}.json`.

Actual CD transport, driver readiness delivery, playback, native function timing and the connected normal prologue remain open. This component does not make New Game playable; T-29012 remains In Progress.

## Player-facing UI direction (2026-10-25)

User wants the finished app to feel like a game, not a developer tool. Remove default diagnostic headings, raw state/RNG/indices, implementation explanations and permanent control banners as scene integration proceeds. Debug information remains available only on request or in logs. Original game menus/dialogue remain; setup/settings/errors use concise useful copy. The current placeholder layout is not the target design. HUD and New Game owners have this requirement.

## Entry presentation cleanup integrated (2026-10-25)

Root applied the independently approved four-file v2 packet, manifest `74757a37da862316018747f9b517063476899c29a67b6ed681c82d8638e0ba58`. Normal entry screens omit persistent instructions, raw state/RNG and development paragraphs; technical diagnostics are opt-in or logged on load failure. Profile fields use readable values. Controller, initialization and original-data validation remain unchanged. Canonical execution passes eight UI/diagnostic checks, 85 controller checks, five shared suites and 13 actual Mesa-rendered UI checks. Evidence is `entry-ui-cleanup-v2/canonical-proof/` under the ignored T-29012 state directory. This does not complete the prologue or establish native macOS startup.

## Connected opening route, VAB memory installer and native bank witness (2026-10-25)

1. Repaired the PCSX-Redux bank witness harness in `.claude/orchestrator/state/T-29012/connected-control-v2/capture_bank_witness.py` by relocating the synthetic trampoline from `0x801D0000` (which collided with the `0x801C8000..0x801F0000` music body destination buffer) to `0x80150000..0x80151000`, adding an explicit eight-region pairwise disjointness assertion (`assert_pairwise_disjoint`) and initializing `CdInit`, `sys_init` and `SpuInit(0)` before `audio_init()`. Exclusive Docker execution (`run_witness.sh`) passes all 21 marker breakpoints and records 120 binary dumps in `native/receipt.json`, proving `prologue_arrival` (`SYS_MIX` slot 0 + `PRO_VOL` slot 1) and `normal_gameplay` (`COM_MIX` slot 0 + `SPRING` slot 1) bank readiness transitions.
2. Integrated nine additive files into `port/godot/` (`flow/new_game_command_reload.gd`, `flow/new_game_state_adapter.gd`, `flow/prologue_arrival.gd`, `rules/new_game_route_state.gd`, `services/native_vab_memory.gd` and four test suites `tests/run_new_game_command_reload.gd`, `tests/run_new_game_state_adapter.gd`, `tests/run_native_vab_memory.gd`, `tests/run_connected_opening_route.gd`). Canonical Docker verification passes 110/110 assertions across committed name entry -> fresh route state adapter (`0x15` stage 0) -> prologue arrival + stage 21 finish -> command reload (`0x34` stage `0xFF`) -> April 4 command screen (`0x34` stage 1). Normal prologue stages 1..20 remain open.

## Normal 22-stage prologue progression and playable opening join integrated (2026-10-25)

Integrated the full 22-stage (`0..21`) prologue state machine (`port/godot/flow/prologue_arrival.gd`), command screen reload/setup stages `0xFF` and `0` (`port/godot/flow/new_game_command_reload.gd`), `mode_router.gd` (`0x15` binding), `school_day_controller.gd`, and `playable_boot.gd` (`TOKIMEMO_NEW_GAME_BOOT` / `run_new_game_opening_to_command_ready`) from `.claude/orchestrator/state/T-29012/prologue-stages-v1/`. Root review fixed `build_command_screen` club icon masking (`club_id >= 11` disables icon 4 on April 4) and preserved `T-29013` Sunday holiday and `T-29014` Command 4 club integrations in `school_day_controller.gd` and `playable_boot.gd`. Canonical Docker verification passes 1,996/1,996 prologue stage checks (`run_prologue_stages.gd`), 78/78 connected opening route checks (`run_connected_opening_route.gd`, including non-skip 22-stage progression into April 4 command selection and April 5 weekday activity mode `0x40`), and 11/11 command reload checks.

## Authentic 320x240 Prologue VN visual presentation and 16x16 tile verification integrated (2026-10-25)

Integrated `port/godot/presentation/prologue_presenter.gd` and `port/godot/tests/run_prologue_visual_tests.gd` from `.claude/orchestrator/state/T-29012/prologue-visual-v1/`. Matches `prologue_mode_handler` (`src/ovl/name_entry/prologue.c:2808-2820`), `hud_screen_frame_draw(0)` / `hud_screen_frame_draw(2)` (`src/main/hud_panels.c:1366-1461`), `hizuke_init` / `hizuke_show` Stage 0 date pillar (`1995-04-04` Tuesday) and Stage 1 `hizuke_disp_switch(0)` hiding, `message_window_set_title("プロローグ")` (`0x71..0x73` wooden badge), `gnsx` / `sndi` speaker centering and 3-row scroll (`y = 170, 186, 202`), and `hud_text_marker_set` (`0xF0` prompt marker). Canonical Docker verification passes `160/160` visual checks and `tools/port/verify_tile_diff.py --threshold 15.0 --max-hotspots 0` across all 5 PS1 emulator captures (`stage0_arrival`, `stage0_arrival_with_badge`, `stage1_board`, `stage13_shiori_line18`, `stage14_shiori_line19`) with `0 / 300` hotspot tiles and `Overall MAE = 0.185..0.258`.

## Final Phase 3 Completion & Standards Review (2026-10-26)

- Reviewed against `CODING_STANDARDS.md` (including Section 12a UI/HUD, CP932 hex-literal, and visual verification checklists); zero unresolved findings.
- Verified in Docker via `./tools/port/run.sh test` (5/5 core suites PASS), `./tools/port/progress/run.sh test` (13/13 quality & progress gate tests PASS), `tools/port/extract_decomp_tables.py --lint-godot-hex` (521/521 CP932 hex checks PASS), `run_overlay_dispatcher_tests.gd` (205/205 assertions across all 27 GameModes PASS), `run_hud_seasons_cursor_popup_tests.gd` (78/78 checks + 120 video frames PASS), `run_full_three_year_route_tests.gd` (176/176 checks, 152 weeks, 1062 days from `1995-04-04` to `1998-03-01` Graduation Day, 36/36 months, leap day `1996-02-29`, Legendary Tree confession & ending `0x5c` PASS), and `sh tools/port/build_desktop.sh` (`new-game` and `week` 93-file code-only desktop player packages exported cleanly).
