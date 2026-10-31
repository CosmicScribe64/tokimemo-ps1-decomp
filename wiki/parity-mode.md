---
type: design
updated: 2026-10-30
---

# Parity mode

Parity mode plays the port as the original game. Every enhancement is locked to its original value, and features the original game lacks are refused. Modern options can keep growing outside it. Testing uses it to check that no modern feature changes core gameplay. Ticket: [[tickets/T-39230-parity-mode]]. Related pages: [[phase4-design-2026-10-29]] (settings), [[port-testing]] (modes F/E/M) and [[playtest]] (harness).

## The contract

- **One switch.** `profile.mode` in `tokimemo.port-settings.v2` (`user://desktop-settings.json`) is `parity` or `modern`. Parity is stricter than the "Original (PS1 exact)" preset: it also pins the language to Japanese with no packs, the frame limit to 60, mono output off and the preset to Original, and it refuses quick save, quick load, return to title and the Ctrl skip shortcut.
- **What stays adjustable.** Only registry entries with policy `host` can change in Parity mode: window mode, size and monitor, vsync, interface size, volumes and mutes, output device, bindings, swap confirm, deadzone, menu repeat and prompt style. None of them reaches native state. Bindings and swap confirm change which physical control produces a PS1 bit, never the bit.
- **Refusal, not reset.** In Parity mode, `set_value`, `set_values`, `apply_preset` (anything but Original), `begin_display` (a locked display key), `apply_profile` and `save()` refuse a non-original value with "Parity mode keeps the original game...". The service enforces this, so a disabled row or a direct write (the pack pickers write `settings.language` themselves) cannot get past it.
- **Unregistered means refused.** Parity treats a preference key the registry does not know as a violation. A feature id it does not know is refused in both modes. A settings row the registry does not know is locked in Parity.
- **Quick save, quick load and Continue.** `BoundarySaveControls.request()` (`app/boundary_save_controls.gd`) checks `parity_refusal()` first, so F5, F9, the pause menu and title Continue are all refused in Parity and the refusal shows in the save notice. Title Continue is refused too: it restores the port-only quick slot, which the original game does not have, so `title_continue.gd` `continue_check()` shows Continue disabled with the Parity refusal. A Parity player starts with New Game. A save the original game has (memory card slots) would be a new registered feature.
- **Leaving Parity** keeps the original values. The player then picks a preset or changes options.

## Selecting it

| Where | How |
|---|---|
| First launch | The Welcome step has a "Play mode" row (Modern preselected) with the explanation. Choosing Parity locks Game language and Preset (with the Parity badge). Continue calls `set_mode("parity")`. |
| Settings | The Game page (now always present) has "Play mode" and the explanation. Quick save and Load stay on the page and are locked in Parity. |
| Launch | `--parity` (after `--` on the Godot command line) or `TOKIMEMO_PARITY=1` forces Parity for that session only. The saved profile, including its mode and enhancement values, is kept aside. Host settings changed during the session are written back into it. The display recovery marker follows the same rule: its `previous` and `candidate` are the saved display with only the session's host keys copied in, so a crash during Keep/Revert never restores locked values into the saved profile. While forced, the Play mode row is locked and says why. |

Migration: a v2 file written before `profile.mode` existed, and every v1 file, loads as `modern`, the behaviour it was written under. The key is late-added, so its absence is not reported as a reset. An invalid value resets to `modern` and is reported. A saved Parity file that a hand edit moved off an original value is re-locked on load. The keys go to `parity_relocked` and `reset_keys`, and the runtime shows a notice.

## UI

In Parity mode the settings shell runs `SettingsWidgets.lock_parity_rows` after building the pages, and the pause menu and the Welcome step do the same. Each locked row is disabled but stays focusable. It gets a `[Parity]` badge (in the title row for options and sliders, in the text for buttons) and the refusal as its tooltip. The header badge reads "Parity mode: ON (original game, enhancements locked)" and adds "; forced by ..." when a launch option forced it.

## Feature registry

`port/godot/services/feature_registry.gd` lists every preference key (`PREFERENCES`), every non-preference feature (`FEATURES`) and every settings, pause and Welcome row (`ROWS`, plus the `binding:`, `page:` and `preset:` prefixes and the per-bus rows). Each entry declares:

- `class`: `presentation_only`, `input_ux` or `game_affecting`;
- `parity`: `original` (pinned), `host` (adjustable) or `refused` (feature only);
- `invariance`: `state_and_image`, `state_only` or `excluded`;
- `invariant`: what stays true, and which test covers it;
- `modern`: the "everything on" value for the invariance harness (a `note` explains when it equals the original).

Rules that `spec_problem` enforces: a `game_affecting` entry is never `host` and always `excluded`. A preference is never `refused`. A feature is never `original`. A non-excluded preference has a `modern` value.

The `game_affecting` entries today are `accessibility.text_speed` and `accessibility.auto_delay`. Both hold the presentation in `first_arrival_boot.gd`, which returns before the native tick. The others are `accessibility.auto_advance` (generates input), `accessibility.skip`, `feature.quick_save`, `feature.quick_load`, `feature.return_to_title`, `feature.prologue_skip_shortcut` and `feature.latin_name_typing` (English-mode keyboard typing on NAME_ENT, which turns keys into native tab, scroll, cell and START actions and so sets the stored name bytes). A future fast-forward or save manager must be added here as `game_affecting`. Until then the registry test fails on its key or row, and Parity locks it.

`state_only` entries may change native pixels but never state. They are `accessibility.font_scale`, `backdrop`, `reduce_flashes` (drawn into the native Image by `first_arrival_scene_presenter.gd` and `dialogue_textbox_presenter.gd`) and every `language.*` key. The `language.bundle` invariant covers the bundle only: `en` also makes Latin name typing available, which is registered separately and excluded, and the invariance routes do not type.

## Test gates

| Producer | Gate |
|---|---|
| `tests/opening/run_installed_rei_banner.gd` `_start` (so also the frame-timing route, opening trace emitter, presentation pacing and input-script routes) | `ParityMode.gate(self, "/out")` fails the route before any content loads unless Parity is active with no violation, or `MODERN_INVARIANCE=1`. It writes `/out/parity-mode.json` (`tokimemo.parity-mode-identity.v1`). A QA observer that applies modern preferences on purpose overrides `parity_evidence_route()` to return false: it runs in any mode and its record says gate `not_evidence`, which no comparison accepts. |
| `tests/support/checkpoint_observer.gd` `record()` | Receipts carry `settings.play_mode`, `parity_forced_by`, `invariance_run`, `parity_violations`, `parity_registry_sha256` and `parity_locked_values_sha256`. A receipt is refused unless the run is clean Parity or an explicit modern-invariance run. |
| `tools/port/run_opening_frame_timing.sh`, `run_input_script_route.sh`, `run_opening_trace_differential.sh`, `run_presentation_pacing.sh`, `run_installed_route.sh` | Pass `TOKIMEMO_PARITY=${TOKIMEMO_PARITY:-1}`. Their output is compared with the original, so they refuse `MODERN_INVARIANCE=1` (exit 2) and pass `MODERN_INVARIANCE=0`. The one exception is `run_input_script_route.sh` with the invariance route and a `modern` or `modern_state_only` profile, which only `run_parity_invariance.sh` sets. The trace differential runs `parity_gate.py` on the port run before comparing. |
| `tools/port/run_differential_suite.py` | `compare` needs `port_parity` (and `timing.port_parity`) pins to an accepted `parity-mode.json`, else exit 2 before anything is compared. `captures-from-pairs` needs `--port-parity` (and `--timing-parity`). The plan's env sets `TOKIMEMO_PARITY=1`. This covers the golden entries too. |
| `tools/port/rei_stage_capture_pairs.py` | v2 receipts must all carry an accepted play mode. A legacy run directory needs `parity-mode.json`. |
| `tools/port/parity_gate.py` | Shared checks and a CLI (`parity_gate.py <run dir>`). The CLI and every comparison against the original accept clean Parity runs only, with no option or environment variable that widens them. Only `parity_invariance.py` passes `allow_modern_invariance`. Every accepted identity and receipt must name the registry and locked-value digests pinned in `port/godot/services/parity-registry-pin.json`; other digests are refused with "rerun under the current registry". |
| `port/godot/services/parity-registry-pin.json` | The reviewed `registry_sha256` and `locked_values_sha256`. `run_feature_registry.gd` fails until the pin matches the registry, so a registry change updates the pin, and evidence made before it is refused. |

Receipts and runs made before this change have no play mode and are refused. Rerun them under Parity.

## Invariance harness

`tests/support/parity_invariance.gd` builds three profiles from the registry: `parity`, `modern` (every `state_and_image` entry at its `modern` value) and `modern_state_only` (plus the `state_only` entries). For each completed game iteration it records `{frame, phase, state, image}`, where `image` is the RGBA8 sha256 of the native 320x240 Image. `modern` must equal `parity` on state and image, and `modern_state_only` on state. Excluded entries are never enabled. Every report lists them with their invariants. The input script player presses PS1 buttons through the active mapping (`input_script_player.gd` `button_index`), so swap confirm and remapped bindings deliver the same native bits.

- T0: `res://tests/support/run_parity_invariance_tests.gd` uses the real settings runtime and remaps, and a real `GameSession` with a synthetic mode handler and a synthetic native producer. It proves the harness and the mapping, not the game. Its negatives cover a game_affecting hold leaking into the tick, a presentation value drawn into the native Image, a player that ignores the mapping, and a modern profile while Parity is forced.
- T1/T3: `tools/port/run_parity_invariance.sh <out> <script.json>` runs `run_parity_invariance_route.gd` four times through `run_input_script_route.sh` (now with `INPUT_SCRIPT_ROUTE` and `PARITY_INVARIANCE_PROFILE`): parity, a parity control, modern and modern_state_only. `tools/port/parity_invariance.py` then compares the runs into `invariance-report.json`. If the control differs from the first parity run, the route is not reproducible and the status is `baseline_nondeterministic`, so no modern verdict is drawn. Exit 0 invariant, 1 diverged or nondeterministic, 2 invalid.
- Entry frames hash the entry state, RNG, model frame count, name pointer and sampled input. The snapshot's window geometry and clock policy are left out because they are host data. Opening frames use `GameSession.state_hash()`.

## Results (2026-10-30)

These runs are T3 (Xvfb/llvmpipe) and were checked by an agent only, not reviewed by a human. Outputs are in `.asset-cache/parity-mode/invariance-seed-v1` to `-v3`. All of them used `config/port/input-scripts/new_game_to_school_handoff.json`. All four runs stop at the known T-39023 `family-name` expect_state failure (a random preset name), after 381 entry iterations.

- The v3 report says `baseline_nondeterministic`. The two Parity runs already differ in the entry `rng` part from frame 0, because the installed entry seeds from the host clock (root hook 0). Modern and modern_state_only differ from Parity in the same part and nowhere else.
- In all three comparisons the native Image is identical on 380 of 381 frames. The exception is frame 380, which draws the RNG-chosen preset name. Inside the reached frames, this run found no change that only modern settings cause. It is not invariance evidence until the seed is pinned.
- v1 also hashed the snapshot's window geometry and clock policy, which made modern differ for host reasons. v2 and later hash only game state.

## Running the tests

`sh tools/port/run_parity_mode_tests.sh` runs the Godot suites headless in `tokimemo-port`: Parity preferences and runtime, the registry and UI, the harness, the observer, the player, the settings suites and four launch variants. It then runs the Python suites in `tokimemo-content`. `test_parity_gate` and `test_parity_invariance` are also in `run_playtest_unit_tests.sh` and `run_offline_suite.sh`.

## Boot hooks owed by root

The boot files (`app/first_arrival_boot.gd`, `new_game_entry_boot.gd`, `playable_boot.gd`, `overlay_dispatcher.gd`) were not edited. Settings reach the boots through `ModernSettings`, and the service refuses changes. Quick save and load are gated in `boundary_save_controls.gd`, which `playable_boot._input` calls first. So F5/F9 need no boot change. Root still owes the hooks below. Hook 0 is required for invariance, and hook 5 is required once Latin typing is wired into the entry boot:

0. **Required for route-level invariance.** In `app/new_game_entry_boot.gd` (around line 136), the installed profile replaces `TOKIMEMO_NEW_GAME_SEED` with `Time.get_ticks_usec()`. Honour a pinned seed first, then fall back to the clock: `var pinned := OS.get_environment("TOKIMEMO_NEW_GAME_SEED"); seed = pinned if pinned.is_valid_int() else str(Time.get_ticks_usec() & 0xffffffff)`. Players keep the clock seed. The runners already pass the variable through (`run_input_script_route.sh`; `run_parity_invariance.sh` defaults it to 19950404).
1. `app/first_arrival_boot.gd`, `_physics_process`, the line `if Input.is_action_just_pressed("port_skip"): skipping = settings.request_prologue_skip(...)`. Nothing to change: `request_prologue_skip` already returns false in Parity. Optionally show `ParityMode.REFUSAL` once.
2. `app/playable_boot.gd` and `app/new_game_entry_boot.gd`: any future fast-forward, turbo or save-manager entry point must call `ParityMode.refusal("<feature id>", ModernSettings.preferences.parity_active())` and register its id in `FEATURES` first.
3. `app/overlay_dispatcher.gd`: an overlay that offers a modern shortcut uses the same `refusal` call. No change today.
4. Title screen (`app/title_continue.gd`, not root-owned): optional "Parity" marker next to the F10 hint when `parity_active()`. Continue is already refused in Parity (see the contract).
5. **Required where Latin typing is enabled.** On current main, `app/new_game_entry_boot.gd` sets `latin.enabled = _english_selected()` in setup (about line 163) and `latin.enabled = english` on language change (about line 232). Use `latin.enabled = Latin.allowed(_english_selected(), ModernSettings.preferences.parity_active())` at both places, and the same for `english`. `Latin.allowed` is in `flow/name_entry_latin_page.gd`. Today the bundle is pinned to `ja` in Parity, so this is defence in depth.

## Integrating with current main

The delta is built on `refs/snapshots/phase4-base-2026-10-29` plus the wave-1 and wing-art patches, as the workflow requires. Main has moved on. When integrating:

- `tools/port/run_installed_route.sh`: main's `ROUTE_OBSERVER` docker branch (command, playtest, modern, playthrough) passes neither variable. Add `-e TOKIMEMO_PARITY="${TOKIMEMO_PARITY:-1}" -e MODERN_INVARIANCE=0` to it. The `modern` and `playthrough` observers (`tests/qa/run_modern_command_route.gd`, `run_english_playthrough.gd`) apply modern `ROUTE_PREFS` on purpose. Each needs `func parity_evidence_route() -> bool: return false`, and that branch should pass `TOKIMEMO_PARITY=0` for them.
- `services/settings_runtime.gd`: add the forced-Parity and relock notices to main's longer notice chain in `_ready`, after the error and recovery notices. In `_language_applied`, when `preferences.parity_active()` and `choice != "ja"`, drop the result and call `_request_language(preferences.settings.language)`. This covers a bundle load still in flight when `set_mode("parity")` runs. `select_language` already goes through `set_values`, which Parity refuses.
- `tests/services/settings_v2_fakes.gd`: main's runtime already declares `display_frame`, `language_bundle` and `select_language`. Drop the fake's own declarations on main.
- After the merge, rerun `sh tools/port/run_parity_mode_tests.sh`.
