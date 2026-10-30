---
type: verification
updated: 2026-10-30
---

# Playtest and differential harness

This page describes the checkpoint catalog and the tools that compare the original game (PCSX-Redux) with the port at those checkpoints. The design is [[phase4-design-2026-10-29]] (Playtest, WP-01 to WP-06). Work is tracked in [[tickets/T-39018-playtest-differential-harness]]. Tiers and modes are in [[port-testing]].

Nothing here is parity acceptance. The exact RGBA gate stays `tools/port/compare_capture_pair.py`. Every other report says what it compared and what it did not.

## Modes and statuses

Every catalog entry lists the modes it applies to:

- **F** (fidelity: Japanese, 4:3, native 320×240 canvas) must match exactly.
- **E** (English) must keep the F state trace. Its pixels may differ only inside text and art regions declared from source.
- **M** (modern presentation) must keep the inner 320×240 canvas hash and the trace. Only pixels outside the canvas may change.

Each entry has one expected status:

| Status | Meaning |
|---|---|
| `exact` | Passes only on an exact match: RGBA for a frame, iteration counts for frame timing, in-scope fields for a state checkpoint. |
| `known_diff` | Cites a defect ID and pins a residual (RGB555 count and bbox, plus the exact RGBA count). Fails if the residual grows, its bbox moves, or the count stays while the bbox changes (`known_diff_changed`). Reports `promote` only when `compare_capture_pair` finds the whole frame equal. A frame that is clean except in the declared x=0/y=0 exclusions is `known_diff_clean_except_exclusions`: re-pin advised, never a promotion. |
| `unsynchronizable` | Listed with its reason and never compared. |

## Checkpoint catalog (WP-01)

`config/port/playtest-checkpoints.json` follows the schema `tools/port/schemas/playtest-checkpoints.v1.schema.json`. `tools/port/playtest_catalog.py` checks what a schema cannot: unique keys, that defect and reach references resolve, that seed states carry provenance and the label "path not verified", that the frame-timing entries cover the fixture's substeps, that known_diff numbers are consistent, and that no image data is embedded. The catalog stores predicates, hashes, counts and `.asset-cache` paths only.

| Group | Entries | Status |
|---|---|---|
| `rei-stage8-10` | 29 keys, stage 8 substeps 0-7, stage 9 substeps 0-11, stage 10 substeps 0-8 | 25 `known_diff`, pinned from T-39008 `pairs-v2`. `stage8-0`, `stage10-0`, `stage10-1` and `stage10-8` (the port failure frame) are `unsynchronizable`, with the T-39008 reasons. |
| `first-school` | 1 | `known_diff` T-39008-FS1. The pair is not iteration-matched (title timer 184 vs 116). |
| `name-entry` | 7 steps of mode 0x14 | `exact`, not yet compared |
| `newgame-golden` | a1 to a8 of `.asset-cache/emu/newgame-golden-v1` | `exact` (state kind), not yet compared. The timing-only fields from the golden README are excluded. |
| `opening-frame-timing` | 104 substeps of `opening_frame_timing.json` | `exact`. T-39002 route-pad2 and route-pad3 passed. |

The defect table maps D1 to T-39030, D2 to T-39031 and D3 (with D4 and D5) to T-39032. It also covers the capture-domain issues C1, the open differences D6, the first-school synchronization gap FS1 and finding T-39020-F1, the port's mixed colour expansion.

The pinned residuals use RGB555 with native `zero_extended`, port `source_proven_mixed` and the x=0/y=0 exclusions. Every port pixel of the 25 pairs decodes under the mixed rule, so none is rejected. In 16 of the 25 pairs the residual bbox is [1,160,319,239]: only the HUD frame and message window differ, which matches C1 (checker phase) and D6 (message window). The other nine pairs reach into the scene: 8-2 (D1), 9-1, 9-2, 9-3 (D2), 9-4 and 9-5 (D5), 10-2 to 10-7.

## Tools

| Package | Tool | Notes |
|---|---|---|
| WP-02 | `port/godot/tests/support/checkpoint_observer.gd` | Writes `tokimemo.checkpoint-receipt.v2`. The PNG is hashed from its encoded bytes, written and re-read before the receipt exists. The receipt holds clocks, the RNG draw count, settings, pack hashes, the observer and producer script hashes and the renderer. `verify()` detects any change to the PNG or the receipt. `tools/port/checkpoint_receipt.py` is the Python side of the same check: it recomputes `receipt_sha256` with the observer's canonical form (Godot 4.7.2 `JSON.stringify` text, checked in Docker and against the 88 real receipts of `wp06-seed-route-v3`, `wp05-opening-trace-v1/port` and `wp02-observer-synthetic`) and the PNG and decoded RGBA8 hashes. `run_installed_rei_banner.gd` now writes `checkpoint-receipts.json`. `rei_stage_capture_pairs.py` uses these receipts with no post-hoc binding, and refuses any receipt whose fields or PNG changed after binding (fixes T-39008 C2). Schema: `tools/port/schemas/checkpoint-receipt.v2.schema.json`. |
| WP-03 | `tools/port/rgb555_diagnostic.py` | Projection report, see `tools/port/README.capture-pair.md`. |
| WP-04 | `tools/port/run_differential_suite.py` | `plan` (Docker capture steps), `captures-from-pairs`, `compare`. Writes reports only under `.asset-cache` or outside the repository. |
| WP-05 | `config/port/opening-trace-projection.json`, `tools/port/opening_trace.py`, `port/godot/tests/opening/run_opening_trace_emitter.gd`, `tools/port/run_opening_trace_differential.sh` | Per-iteration opening trace through `compare_trace.py`. The native rand count comes from an exec breakpoint on `rand` (0x800AE0D0). |
| WP-06 | `tools/port/schemas/input-script.v1.schema.json`, `tools/emu/input_script.py`, `port/godot/tests/support/input_script_player.gd`, `tests/support/run_input_script_route.gd`, `tools/port/run_input_script_route.sh`, `config/port/input-scripts/new_game_to_school_handoff.json` | One script drives both sides. The emulator side compiles to `emu.py --input` vsync edges from a calibration trace. The port side sends joypad InputEvents at game-update boundaries. Each name-entry segment carries `expect_state`: the set B names, birthdays and blood types, in the native working-buffer encoding (0x8014CC74..0x8014CCA7). The port player, the route runner and `input_script.py verify` (native trace with dump `8014CC74:52`) fail when a value differs or cannot be observed. |

Unit tests: `tools/port/run_playtest_unit_tests.sh` (Python, tokimemo-content with pinned jsonschema). Godot synthetic suites: `res://tests/support/run_checkpoint_observer_tests.gd` and `res://tests/support/run_input_script_player_tests.gd` (tokimemo-port, headless).

## Results so far (2026-10-30)

All runs are OpenBIOS/PCSX-Redux and Xvfb/llvmpipe (tiers T1 to T3), not hardware and not human review. Outputs are under `.asset-cache/phase4-playtest/`.

- **Suite on T-39008 `pairs-v2`** (`wp04-suite-rei-pairs-v2`): 25 `known_diff_stable`, 4 `unsynchronizable_listed`, and 104 frame-timing `exact_pass` against T-39002 route-pad2. On the full catalog the suite fails as intended: 8 name-entry entries are `missing_capture` and 8 golden entries are `not_implemented`.
- **Seed input script on the port** (`wp06-seed-route-v3`): `new_game_to_school_handoff.json` ran as joypad InputEvents from New Game through name entry and the unskipped opening to stage 12, the declared port stop. The run emitted 51 v2 receipts and no observer refusals. Each START reached the transition the script expects, but the entered values did not match set B, so this run is not like-for-like (see below). Two findings:
  - The port pointer starts at the desktop centre ("prior native FILESEL cursor unavailable"), while the native state's pointer was left elsewhere by the T-39004 chain. The scripted CIRCLE presses therefore missed a kana, and the blank names drew random presets (rng.draw_count 0 to 1 at name-1-2). Expect_next did not check names, so v3 passed anyway; that gap is now closed by `expect_state`.
  - At the native set B onset phase (1), the port spends 224 iterations in 0/2/1 against 213 natively, and 180 in 6/0/8 against 168. The total to 12/0/0 is 6730 against 6707. T-39002 checked only phases 4 and 5.
- **v2 receipts in the pair builder** (`wp02-pairs-seed-route-v3`): `rei_stage_capture_pairs.py` paired the seed-run receipts directly, with no post-hoc binding file. The suite then reported 15 `regressed`, 9 `improved` and 1 `bbox_moved` (9-6, the eye region). These are not like-for-like port regressions: the port player names differ from the native pass-b run.
- **Seed input script with `expect_state`** (`wp06-seed-route-v4`, 2026-10-30): the same route now fails at the first segment: `names.family: expected 82bf814081400000, observed 8d8296d800000000` (a random preset). The port name-pointer origin has to match the native start before the seed route can produce like-for-like name-entry and Rei captures. The native start is the restored `name-ready-v1` state and the port start is a fresh New Game, so the route is not yet the design's normal New Game on both sides.
- **Opening trace** (`wp05-opening-trace-v1`, pad 2): 6700 compared iterations. 10 native iterations had no vsync of their own and were listed and dropped. The first divergence is `rand_calls` at iteration 650 (native 32, port 34). Later first divergences per field: `prologue_scene_counter` at 1233 (native 0, port 320); D_801220E0..E2 at 1724 (stage 3/0/1, native 128, port 0, when the port first defines them); `actor96_x` at 2059. **D1 reproduces** at iteration 4548, the first 8/0/2 iteration: native 128, port 63. In this build the port value catches up one iteration later, so D1 is now a one-iteration lag rather than a missing write. The same run passed the T-39002 timing fixture (6709, 0 mismatches) and wrote 36 v2 receipts. `compare-v6` reran the comparison with the gating report: no configured field is `never_defined`. `gated_native_values` shows what the gating left out: D_801220E0..E2 hold 128 natively at iterations 0-1 (before the port defines them at 1724), and `actor96_x` is non-zero natively on 1243 iterations (468-1723) where the port no longer defines it.
