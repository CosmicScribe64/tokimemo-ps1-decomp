---
id: T-39045
title: Keep a newer-version settings file when booting from the backup
status: Backlog
assignee:
created: 2026-10-30
updated: 2026-10-30
links: ["[[tickets/T-39007-player-save-relaunch-recovery]]", "[[tickets/T-30002-modern-settings]]", "[[phase4-design-2026-10-29]]", "port/godot/services/user_preferences_service.gd", "port/godot/tests/services/run_settings_file_recovery.gd"]
---

## Goal

Stop the settings service from destroying a newer-version settings file. Owner: the settings owner (`port/godot/services/user_preferences_service.gd`). Found by WP-10 (playtest-qa); the test package does not change the service.

## Acceptance criteria

- [ ] Booting with a newer-schema `desktop-settings.json` (the test uses `tokimemo.port-settings.v3`; v2 is current since T-30002 WP1) next to a valid current-version `.backup` uses the backup's settings and leaves the newer file in place (or in a location that later saves never rotate away).
- [ ] `tools/port/run_save_relaunch_tests.sh --settings-matrix NEW_DIR` no longer reports XFAIL for `settings-newer-with-backup`; its strict check then reports XPASS, so the expected failure in `run_settings_file_recovery.gd` (`NEWER_BACKUP_DEFECT`) must be removed in the same change.
- [ ] Code review against CODING_STANDARDS.md.

## Notes

Cause: `load_preferences` falls back to `path + ".backup"`, sets `recovered`, and then `save()`s. `save()` rotates the current file (the newer one) to `.backup` and writes the backup's settings over `desktop-settings.json`. The newer file survives only as `.backup` until the next settings save, which overwrites it. Evidence: `.asset-cache/phase4-playtest-qa/settings-matrix-v2/proof/settings-newer-with-backup.json` (recorded then as a known finding; the runner now reports it as XFAIL and exits 3).

Ticket number chosen by the playtest-qa package; root may renumber it on integration.

Phase 4 wave 1 integration (2026-10-30): with settings-core v2 the old fixture (a `tokimemo.port-settings.v2` file) became a current-version file that per-key validation accepts, so the fixture now names `tokimemo.port-settings.v3`. The defect is unchanged under core v2: the settings matrix still ends XFAIL (exit 3) on `settings-newer-with-backup` only.

## Comments
