---
id: T-39230
title: Add a locked Parity mode and parity test gate
status: In Progress
assignee: Claude (worker)
created: 2026-10-30
updated: 2026-10-30
links: ["[[phase4-design-2026-10-29]]", "[[port-testing]]", "[[tickets/T-30002-modern-settings]]"]
---

## Goal

User request: a parity ("true") mode for playing and testing that guarantees core gameplay is unchanged, while modern features keep growing outside it.

## Acceptance criteria

- [ ] Top-level Parity mode, selectable at first launch, in settings and from the command line or environment. It locks every enhancement off (not just a preset), shows a visible badge, and is persisted.
- [ ] Feature registry: every modern feature declares a class (presentation-only, input/UX, or game-affecting, e.g. fast-forward, save manager, skip). Parity mode refuses anything active outside the registry. A test fails if a preference key or feature is unregistered.
- [ ] Parity gate: the opening frame-timing route, emulator differential runs, golden comparisons and the long-route playtest run in Parity mode by default and fail if parity is not active.
- [ ] Modern-vs-parity invariance: the same input script under "everything on" must produce identical game state and native frames. Game-affecting features are explicitly excluded or have their own declared invariants.
- [ ] Docs in the wiki; review against CODING_STANDARDS.md.

## Notes

### 2026-10-30 worker (isolated worktree; not committed, not reviewed)

Base: `refs/snapshots/phase4-base-2026-10-29` + `phase4-wave1-combined.v2.patch` + `wing-art.v2.delta.patch`. Delta: `.asset-cache/parity-mode/parity.delta.patch`. Contract and details: [[parity-mode]].

- **Mode.** `profile.mode` (`parity` | `modern`) is in preferences v2 as a late key: old v2 and v1 files migrate to `modern`, and an invalid value resets and is reported. It is selectable on the Welcome step ("Play mode", Modern preselected), on the Settings Game page (always present now, with the explanation) and with `--parity` / `TOKIMEMO_PARITY=1`, which force Parity for that session and never rewrite the saved profile. `UserPreferencesService` refuses every non-original value of a locked key (`set_value(s)`, `apply_preset`, `begin_display`, `apply_profile`, `save()`). It also re-locks a hand-edited Parity file on load. In Parity, the UI disables locked rows and gives them a `[Parity]` badge. The header badge says Parity mode is on. The pause menu locks Quick save, Load and Return to title.
- **Registry.** `services/feature_registry.gd` holds 52 preference keys, 10 features and 68 rows. Each entry has a class, a parity policy, an invariance class, an invariant and a modern value. `tests/services/run_feature_registry.gd` enumerates `defaults()` and every settings, pause and Welcome row (all capabilities on). It fails on an unregistered or unclassified key or row, and on a stale entry. Invented specs and an invented row prove each rule. Unregistered keys or features are refused by Parity.
- **Gates.** `run_installed_rei_banner.gd` `_start` covers the frame-timing, trace, pacing and input-script routes. The other gates are the checkpoint observer `record()`, `run_differential_suite.py` (compare and captures-from-pairs, which includes the golden entries), `rei_stage_capture_pairs.py`, the trace differential script, and `parity_gate.py`. The five route runners default to `TOKIMEMO_PARITY=1`. `MODERN_INVARIANCE=1` is the only exception.
- **Invariance.** `tests/support/parity_invariance.gd` (profiles from the registry, frame records, comparator), the route `run_parity_invariance_route.gd`, `tools/port/run_parity_invariance.sh` and `parity_invariance.py` (with a parity control run). The input script player now presses PS1 buttons through the active mapping.
- **Tests (Docker).** `sh tools/port/run_parity_mode_tests.sh`: 14 Godot runs pass (including the 4 launch variants), plus 50 Python tests. The full playtest unit suite passes (104 tests). `run_tests.gd` passes. Six settings tests that fail on the base tree (they need mounts or content) fail with identical messages. `run_preferences_migration.gd` needed `profile.mode` in its ALT table.
- **Real run.** `.asset-cache/parity-mode/invariance-seed-v3` is `baseline_nondeterministic`: the installed entry seeds the RNG from the host clock, so two Parity runs differ. Native Images are equal on 380 of 381 frames in every pair. Root hook 0 in [[parity-mode]] pins the seed.
- Boot files are untouched; hooks for root are listed in [[parity-mode]].

### 2026-10-30 worker, review fixes (v2 delta; not committed, not reviewed)

Delta: `.asset-cache/parity-mode/parity.v2.delta.patch` (same base; includes v1). Findings:

1. **F5/F9 and Continue in Parity (high): fixed.** `BoundarySaveControls.request()` now returns the Parity refusal first (static `parity_refusal(app, action)` reads `ModernSettings`, and the launch options when there is no autoload). This covers F5, F9, the pause menu and `continue_from`. Decision: title Continue is refused in Parity. It restores the port-only quick slot, so `continue_check()` shows it disabled with the refusal. No boot edit is needed because `playable_boot._input` already calls `handle_input` first. `run_parity_mode.gd` drives F5/F9, `request` and `continue_check` against a Parity `ModernSettings`, with a Modern control. The wiki claim that Parity needs no boot or app hook is corrected.
2. **Forced-session display marker (medium): fixed.** `save()` writes `_persisted_marker()`: in a forced session, the marker's previous and candidate come from the saved display with only the session's host keys. The new test `_forced_display_marker` does a forced `begin_display` and then an unforced relaunch, which keeps aspect `auto` and filter `linear`. Its negative control is a saved Parity profile. The test fails with the old marker.
3. **MODERN_INVARIANCE widening the gates (medium): fixed.** `parity_gate.py` has no `--modern-invariance` option and no environment fallback (`invariance_requested` was removed). `run_differential_suite.py` (compare, captures-from-pairs) and `rei_stage_capture_pairs.py` have neither option nor parameter. Only `parity_invariance.py` passes `allow_modern_invariance`. The trace differential, frame timing, pacing and installed route runners exit 2 on `MODERN_INVARIANCE=1` and pass 0. The input-script runner accepts it only for the invariance route with a modern profile. Tests cover the CLI, the env var, the pair builder and the suite.
4. **Stale base / main conflicts (medium): partly rejected.** The workflow fixes the delta's base (snapshot plus wave-1 plus wing-art) and asks for a diff against it, so the delta was not rebased onto main. On this base, `ROUTE_OBSERVER`, `display_frame` and `select_language` do not exist, and the suite passes. Exact merge steps for main are in [[parity-mode]] under "Integrating with current main": the ROUTE_OBSERVER env, the notice chain, a `_language_applied` guard for an in-flight `en` load, and the fake redeclarations. A forward-compatible API was added for main's modern and playthrough QA observers: `parity_evidence_route()` makes `ParityMode.gate(..., evidence=false)` write gate `not_evidence`, which `parity_gate.py` refuses.
5. **Registry digests ignored (low): fixed.** `port/godot/services/parity-registry-pin.json` pins `registry_sha256` and `locked_values_sha256`. `run_feature_registry.gd` fails until the pin matches. `parity_gate.check_identity` and `check_receipt_settings` refuse other digests ("rerun under the current registry"). Receipts now also carry `parity_locked_values_sha256` (schema updated).
6. **Latin name typing (low): fixed.** `feature.latin_name_typing` is registered (game_affecting, refused, excluded). `NameEntryLatinPage.allowed(english, parity_active)` is the gate. The `language.bundle` invariant now excludes typing. This base's `new_game_entry_boot.gd` does not wire Latin typing. Main's does, so root hook 5 in [[parity-mode]] gives the exact lines.

Tests (Docker): `run_parity_mode_tests.sh` passes 14 Godot runs and 52 Python tests. `run_playtest_unit_tests.sh` passes 106. `run_tests.gd` and `run_save_notice_ui.gd` pass. The four comparing runners exit 2 under `MODERN_INVARIANCE=1`.

## Comments
