---
id: T-39018
title: Build playtest and differential harness
status: In Progress
assignee: Claude (phase 4 workers)
created: 2026-10-29
updated: 2026-10-30
links: ["[[phase4-design-2026-10-29]]", "[[port-testing]]"]
---

## Goal

Implement the playtest design: checkpoint catalog, observer receipts, colour-domain reports, differential suite runner, opening trace differential, shared input scripts, invariance matrix, frame pacing, English lint, contact sheets/review queue, offline CI.

## Acceptance criteria

- [ ] Each work package's acceptance in [[phase4-design-2026-10-29]] met with Docker tests.
- [ ] Independent review against CODING_STANDARDS.md with findings resolved.

## Notes

### Sub-tickets WP-01 to WP-06 (2026-10-30, playtest-diff worker; not committed, not reviewed)

The page is [[playtest]]. Run outputs are in `.asset-cache/phase4-playtest/`. Unit tests: `tools/port/run_playtest_unit_tests.sh`, 115 Python tests. Godot synthetic suites: observer 25/25 checks, player 101/101. Nothing in `app/`, `services/`, `flow/` or `presentation/` changed. The fidelity path (native 320×240 Image, RNG, state) is only read.

- **WP-01 catalog (T-39018).** Added `config/port/playtest-checkpoints.json` (149 entries), the schema, `tools/port/playtest_catalog.py` and its test (13 tests, including negatives).
  - The 25 Rei stage pairs are `known_diff`. Their RGB555 residuals are pinned from the T-39008 `pairs-v2` evidence: native `zero_extended`, port `source_proven_mixed`, x0/y0 exclusions. All 25 decode without rejection.
  - `stage8-0`, `stage10-0`, `stage10-1` and `stage10-8` are `unsynchronizable`, with the T-39008 reasons.
  - first-school is `known_diff` T-39008-FS1.
  - 7 name-entry steps, the golden a1..a8 (state kind) and the 104 opening-timing substeps are `exact`.
  - Defect IDs: T-39030 (D1), T-39031 (D2), T-39032 (D3/D4/D5), T-39008-C1, T-39008-D6, T-39008-FS1, T-39020-F1.
- **WP-02 observer v2 (T-39019).** Added `tests/support/checkpoint_observer.gd`, `run_checkpoint_observer_tests.gd` and the receipt v2 schema.
  - `run_installed_rei_banner.gd` writes `checkpoint-receipts.json`. This file had pending T-39008 changes; the base snapshot already contains them.
  - `rei_stage_capture_pairs.py` takes v2 receipts with no post-hoc binding and re-checks each binding. 4 new tests.
  - Real use: the seed route and the opening-trace run wrote 51 and 36 receipts with no refusals. The pair builder paired the seed-run receipts directly.
- **WP-03 colour-domain report (T-39020).** `rgb555_diagnostic.py` gained `projection_report` and a CLI (`--exclude`, `--out`). `compare_capture_pair.py` is unchanged. 10 tests. README section added. Real use: the 25 pairs-v2 projections and first-school.
- **WP-04 suite runner (T-39021).** Added `run_differential_suite.py` (`plan`, `captures-from-pairs`, `compare`) and 17 tests covering every result transition.
  - Real run on pairs-v2 plus route-pad2: passed (25 stable, 4 listed, 104 timing exact).
  - Seed-route timing: 2 `exact_fail` (0/2/1 and 6/0/8).
  - Seed-route pairs: 15 regressed, 9 improved, 1 bbox moved. These are not like-for-like, because the player names differ.
- **WP-05 opening trace (T-39022).** Added `config/port/opening-trace-projection.json`, `tools/port/opening_trace.py` (12 tests), `tests/opening/run_opening_trace_emitter.gd` and `tools/port/run_opening_trace_differential.sh`.
  - The lip-sync byte 0x80120668, CPU cycles and vsync are excluded by name, each with a written reason.
  - The native rand count comes from an exec breakpoint on `rand` 0x800AE0D0.
  - Two native parts are needed, because emu.py caps a run at 4096 samples.
  - Real run: first divergence is `rand_calls` at iteration 650 (32 vs 34). D1 reproduces later, at iteration 4548 (8/0/2, native 128, port 63, one iteration). Per-field first divergences are in the report.
- **WP-06 input script (T-39023).** Added the schema, `tools/emu/input_script.py` (preflight, compile and verify; 12 tests), `tests/support/input_script_player.gd`, `run_input_script_player_tests.gd`, `tests/support/run_input_script_route.gd`, `tools/port/run_input_script_route.sh` and `config/port/input-scripts/new_game_to_school_handoff.json`. The script is derived from newgame-golden-v1 set B, with provenance.
  - Port Xvfb run (v3): New Game, then name entry, then the opening, up to the declared stop at stage 12.
  - Findings: the port pointer origin differs from the native restored state, so the blank names drew presets. The port is +11 and +12 iterations at 0/2/1 and 6/0/8 at the set B onset phase.
  - Not run: the emulator side of the seed script, which needs per-segment calibration runs.

Agent review fixes (2026-10-30, agent review only, not human approval):

- WP-04: `promote` now needs the whole-frame exact gate (`compare_capture_pair` pixels `equal`). A frame that differs only in the declared exclusions is `known_diff_clean_except_exclusions` (pass, re-pin advised). The same residual count with a different bbox is `known_diff_changed` (fails). Tests cover an edge-only difference and a bbox moving past each pinned edge.
- WP-02: `tools/port/checkpoint_receipt.py` recomputes `receipt_sha256` (Godot JSON canonical form, checked against Godot 4.7.2 and 88 real receipts) and the PNG and RGBA8 hashes. `rei_stage_capture_pairs.py` refuses any tampered receipt. The test fixture uses real digests, and negative tests edit frame_count, draw_count, extra, settings and the observer hashes.
- WP-05: the trace report lists `never_defined` (a configured `when_port_defined` field the port never writes; this makes the report unequal and exits 1) and `gated_native_values`. Rerun: `wp05-opening-trace-v1/compare-v6`.
- WP-06: `expect_state` (names, birthdays, blood) is checked by the port player, the route runner and `input_script.py verify`. The seed script pins the set B values. Rerun `wp06-seed-route-v4` fails at family-name (a random preset name), as it should. A test now covers the last-vsync compile rule on multi-vsync iterations. A refused name-entry checkpoint now fails the route.
- WP-01: a negative test covers a primary defect repeated in `related_defects`.

Not done, for root or follow-up:

- The seed route cannot produce like-for-like name-entry or Rei captures until the port name-pointer origin matches the native start (proposed core-port sub-ticket).
- State-kind comparison for golden a1..a8: the suite reports `not_implemented`.
- The emulator-side compile and run of the seed script.
- The pointer origin and the phase-1 timing findings need core-port sub-tickets.
- Adding [[playtest]] to the index, the log entry and the kanban updates belong to root.

## Comments
