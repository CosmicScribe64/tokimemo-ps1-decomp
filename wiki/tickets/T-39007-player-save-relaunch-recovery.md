---
id: T-39007
title: Complete player Continue and save recovery lifecycle
status: In Progress
assignee: Codex
created: 2026-10-29
updated: 2026-10-30
links: ["[[port-adversarial-review]]", "[[port-testing]]"]
---

## Goal

Resolve review R10–R12: expose normal Continue and explicit backup recovery, and persist ending clear/album state through real storage.

## Acceptance criteria

- [ ] Normal launch offers a valid Continue path without replaying New Game.
- [ ] Corrupt/incomplete saves provide actionable recovery with valid backups preserved.
- [ ] Ending clear state is marked complete only after successful persistence; failures remain recoverable.
- [ ] Actual quit/relaunch tests cover normal save, backup recovery, failed writes and clear-state/album retention.
- [ ] Independent code review resolves all findings against CODING_STANDARDS.md.

## Notes

Save codec passed37 integrity checks, but production routing and clear-save behavior are missing. Review report distinguishes serialization from player lifecycle acceptance.

## 2026-10-29 implementation notes (worktree, uncommitted)

Implemented R10-R12 in an isolated worktree; status unchanged (needs independent review).

- R10 Continue: `port/godot/app/title_continue.gd` is a new installed title owner. `BootRouter` routes a ready install to it instead of `new_game_entry` when session or system save files exist (`TitleContinue.has_save_files`). Continue is enabled only for a valid primary save with no interrupted files whose source/content/behavior versions equal `BoundarySaveControls.profile_versions()`. It hands `{slot, checksum_sha256}` to `playable.tscn` through `InstalledLaunch.change_scene(..., continue_request)`. `playable_boot` then skips the new-game entry and calls `BoundarySaveControls.continue_from`, which rechecks the checksum and loads through the existing admission/neutral-input/restore path. Source reference: title -> NAME_ENT file screens -> weekly cycle (wiki spec m-flow 3.8). The port keeps one session slot; the 14 source slots and copy/delete screens are not modelled.
- R12 recovery: `SaveService` gained `inspect_slot`, `recover_from_backup` (primary must be missing/invalid, backup valid, no staging; invalid primary renamed to `*.invalid-N` first), `set_aside_staging` (`*.tmp` -> `*.tmp.interrupted-N`) and `set_aside_invalid`. Nothing is deleted; set-aside names are bounded to 8. The title offers each action only when it applies, for both the session slot and the system record.
- R11 clear save: new `services/system_record_service.gd` (format `tokimemo.port-system`; port subset of `SystemSaveImage`: `girl_clear_counters` = `system_data.flags[16]` cap 0x63, `album_state`, `album_entries`; any other field is rejected) and `services/clear_save_transaction.gd`. `ending_overlay` clear-save phase is now a yes/no prompt (circle yes, cross no; wiki spec o-etc 6.3). Yes sets `clear_save_status = requested`; `playable_boot` runs the transaction between steps and delivers an `ending.clear_save` COMPLETION event; `school_day_controller.service_completion` applies it. `clear_save_completed` and `album_clear_mask` come only from a read-back receipt; forged status/receipt fields are discarded each step. Failure returns to the prompt (retry or decline). Counters increment before every write attempt and are not rolled back in memory, as `clear_save_write_step` substep 0 does (src/ovl/misc_scenes/staff_roll_and_clear_save.c). Album save follows `album_event_log_pack` (src/ovl/misc_scenes/album_save_flow.c), but the port session keeps no 256-record event log, so the production path rejects album save explicitly.
- Port difference: the source also writes the clear into the selected game slot; the finished-ending state is not a stable session boundary, so no game slot is written.
- Tests: `tests/services/run_save_recovery.gd` (41 checks) and `tools/port/run_save_relaunch_tests.sh` + `tests/services/run_save_relaunch.gd`: seven separate Godot processes over one host user-data dir (write, continue via real `BootRouter` with an installed content root, corrupt, recover, X11 keyboard Continue with screenshot, read-only-volume write failure, retry). Updated graduation/G3/route suites to deliver the host completion or decline with cross.
- Limit: the captured development boundary needed by `playable_boot` is not on this machine, so Continue was verified up to the hand-off into the playable app (which then stops on the missing boundary); the session snapshot in the relaunch test is a codec fixture stamped with this build's profile identity.

## 2026-10-29 review fixes (worktree, uncommitted)

Five review findings re-verified against the code and source and fixed; the earlier notes above that conflict with this section are superseded. Status unchanged (needs independent review).

- Forged "saved" state (high): confirmed. `_sanitize_clear_save` used to keep `saved` whenever the receipt was well formed, and `verify_receipt` ran in no production path. Now `saved` is not a storable status (`prompt`, `requested`, `failed` only), and every step discards receipts, `clear_save_completed` and `album_clear_mask`. `apply_clear_save_completion` finishes the ending itself. `school_day_controller.service_completion` builds that finished step through `OverlayDispatcher.clear_save_step_out` and holds it in a controller member, not in rule state. The overlay step of the same frame uses it only while the stored request still matches. Tests: a forged state with a complete receipt and a random 64-hex checksum stays unfinished under pads 0, START, UP and DOWN (relaunch continue phase, live graduation gate through the session).
- Continue from env strings (medium): confirmed. `BoundarySaveControls.continue_content_error` now runs the same input checks as `playable_boot._load_inputs` for a restored session. The declared boundary and table SHA-256 values must equal `PlayableBoot.BOUNDARY_SHA`/`TABLES_SHA`, and both files must exist outside the project and hash to them (`verified_external`, now shared with `playable_boot`). The title enables Continue only when this passes. **Installed (non-dev) launches cannot enable Continue:** no launcher provides the captured boundary and tables, so the title shows Continue disabled with the reason. The relaunch test asserts that, checks the preflight mechanics with temporary files, and checks the playable hand-off separately as mechanics only. Env-injected values are not production Continue evidence.
- Clear-save input (medium): confirmed. The prompt now follows `clear_save_ask_answer` (src/ovl/misc_scenes/staff_roll_and_clear_save.c:520-531): circle is yes, cross is no, and nothing else is read. The cursor and START handling are gone.
- Failed-write behaviour (medium): confirmed. The source goes to mode_step 0xFF (`clear_save_no_card_step`, 791-822), which waits for 0x860 twice and then retries the card check, with no decline. The port keeps the failure as `failed` with its reason, and one 0x860 press returns to the yes/no ask. This is labelled in code as a **port difference**: the source has no decline here, and a save-folder write error may never clear. **The ending overlay, including the clear-save question and the failure message, is unpresented:** nothing in presentation/, app/ or scenes/ renders it, and no 12a parity was done.
- `step_active_overlay` storage (low): confirmed. The clear-save hook is removed. `step_active_overlay` now refuses while a clear save is `requested`, so storage runs and completions arrive only through `_physics_process` as a Session ServiceEvent.
- Recovery check (low): confirmed. Each branch is now asserted on its own: with no backup, `recover_from_backup` fails and `set_aside_invalid` succeeds. A new case publishes twice, corrupts the file, recovers from the backup and checks that the earlier record comes back.
- Runs (Docker, tokimemo-port): `run_save_recovery.gd` passes 48 checks. `run_save_relaunch_tests.sh` passes all 7 phases (.asset-cache/port/save-relaunch-t39007-v8, content root .asset-cache/port/next-boundary-current/content-install). `run_live_overlay_graduation_gate.gd` fails 4 of 82, the same 4 external-HUD/photo-pack failures as unchanged HEAD (4 of 75). `run_system_overlays_tests.gd` fails 1 of 129, the same missing-HUD failure as HEAD. The G3 gate and the route suites cannot run here because their asset caches are missing.

### 2026-10-29 fresh handoff v2 (worktree, uncommitted)

The v1 handoff patch made every New Game boot stop. `playable_boot._load_inputs` refused `TOKIMEMO_BOUNDARY_SHA`, but `save_controls.configure` (`profile_versions`) requires it. The fresh launch check (`FreshHandoff.launch_input_error`) now refuses only the inputs that carry captured state: `TOKIMEMO_BOUNDARY`, `TOKIMEMO_HUD_CONTEXT` and `TOKIMEMO_HUD_CONTEXT_SHA`. `run_fresh_gameplay_handoff.gd` checks that the launch check and `BoundarySaveControls.configure` pass in one environment. A fresh-origin save identity that does not reuse the boundary SHA is still open.

- 2026-10-30 (Lane A): WP-10 `--settings-matrix` reports XFAIL (exit 3) while [[tickets/T-39045-settings-newer-version-file-loss]] is open; every other check passes (`.asset-cache/lane-a-2026-10-30/settings-matrix-2`). The matrix now checks that the window scale stays neutral and that aspect and filter reach NativeFrameLayout, because settings_runtime no longer writes them to the window.
