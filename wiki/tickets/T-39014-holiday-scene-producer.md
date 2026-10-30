---
id: T-39014
title: Implement holiday scene content producer
status: In Progress
assignee: Claude (fan-out worker)
created: 2026-10-29
updated: 2026-10-29
links: ["[[port-pause-2026-10-29]]", "[[port-adversarial-review]]"]
---

## Goal

Add a source-backed holiday_scene/holiday_text content producer to the installed runtime builder, with authenticated original identities, and integrate holiday setup from source.

## Acceptance criteria

- [ ] Implementation follows original source with explicit rejection where unsupported; no synthetic readiness, injected state or fabricated fields.
- [ ] Docker tests pass, including negative cases; evidence limits stated.
- [ ] Independent adversarial review against CODING_STANDARDS.md with all findings resolved.

## Notes

Started in the 2026-10-29 fan-out; worked in an isolated worktree, integrated by root after review.

2026-10-29 finding: the `holiday_scene` role is not media. `HolidaySceneAdapter.open` reads it as `tokimemo.native-holiday-boundary-extension.v1`, bound to an April 4 RAM capture (`SOURCE_RAM_SHA` 7623f081..., file pin c960d279...). That file lived only in the ignored `.claude/orchestrator/state/T-29013/evidence/` and is gone. Its ten fields split in two. Four are fixed by `holiday_open_init` in the original executable: `dial_keys` (0xFF x7 cleared at open, key 7 never written because the write index wraps at 7, so its `.data` initializer 0 stays), `dial_pos` (0), `icon_count` (0xE), `background_brightness` (`set_dec_bri(0x80)` on both the cached and the CD-decode path). Six are live-session state written by many paths before the holiday: `icon_mode` (settings, memory-card load), `input_device` (`pad_input.c` every frame), `screen_brightness`, `tile_layer_enabled`, `boot_hold`, `clubs`. A content file that gives the six fixed values would be injected state, so the producer leaves them out.

2026-10-29 implementation (worktree, uncommitted): `tools/port/holiday_scene_content.py` reads the pinned MODE2 track (whole-track SHA 28c3c501...), finds the `SLPM_86.053` ISO record (LBA 24, size 667648, record LBA 22 offset 270), checks the EXE SHA (bae1514f...) and PS-X EXE header (load 0x80041000, text 0xA2800), then checks 14 original instruction words in `holiday_open_init`, `holiday_open_wait_bg` and `set_dec_bri`. It decodes the dial-ring, dial-position and `icon_count` addresses from the lui/immediate pairs, reads the ring's `.data` bytes, and pins five witnesses (holiday.c, background_images.c, game_state.h, holiday.data.s, holiday_open_init.s) with anchors. Output `tokimemo.holiday-scene-source.v1` has `scene_owned` values plus `session_owned_fields` names with reasons, and no values for them. Manifest SHA256 f6a695efc026d9ebfe8896fc5d5d73c7d7a0ef6627e163b10693584912eb3cb5 (`.asset-cache/holiday-scene-v1/profile`). The builder now runs it as the `holiday_scene` file role and adds a coverage limitation when the role is present. `HolidaySceneAdapter.open` rejects the fresh profile by name and lists the missing session fields. New `open_fresh`/`load_fresh_profile`/`compose_fresh` accept it only with exactly the six session fields from the caller; production does not call `open_fresh` yet because no fresh-session supplier exists (same blocker as `hud_context`).

Tests: `tools/port/test_holiday_scene_content.py` has 6 tests: decoder, changed instruction/data/header, changed witness, original profile, wrong ISO path/LBA, and an altered whole-track copy, which is rejected without output. All pass with `test_build_installed_runtime` (18/18) in `tokimemo-recovery-splat`. `tests/holiday/run_holiday_scene_profile_tests.gd` passes 25 checks: original bytes, altered profile, absent file, explicit `open()` rejection, missing, extra or mistyped session fields. Core runner passes 5 suites. Not run: `run_scene_adapter_tests.gd` and the other legacy holiday suites, which need the deleted April 4 extension. The `open_fresh` media path was not run end to end.

2026-10-29 bundle: full builder run (`tokimemo-recovery-splat`, track1 as input and track) wrote the new `.asset-cache/port/installed-runtime-holiday-v1`. It has 21 available roles, and `holiday_scene` is `packs/holiday_scene/manifest.json` with SHA f6a695ef..., the same as the standalone export. `hud_context` is the only unavailable role. The coverage limitation records that the holiday_scene session fields are still unsupplied. Exporter work directories were removed. The gameplay capability is still unavailable and no route was run.

2026-10-29 review fixes (v2, all five findings confirmed and fixed):
- Role: the fresh profile is now published as `holiday_scene_source` (builder `ROLE_KINDS`, `FILE_ROLES` in `installed_launch_profile.gd`, not `GAMEPLAY_ROLES`). The builder's new `UNSUPPLIED_ROLES` always records `holiday_scene` ("session-owned boundary fields (icon_mode,input_device,screen_brightness,tile_layer_enabled,boot_hold,clubs) have no fresh-session supplier") and `hud_context` as gaps, and `_coverage()` refuses a bundle that claims either role. `installed-runtime-holiday-v1` predates this and still shows the old `holiday_scene` role, so do not use it.
- `compose_fresh` now applies `game_state.h` bounds: u8 for `icon_mode`, `input_device` and `screen_brightness`; s8 for `tile_layer_enabled` and `boot_hold`; exactly 12 `clubs` entries, each 0..65535. The port stores one u16 per ClubRecord; `rules/school_day.gd` wraps it at 65535. `rules/holiday.gd` still enforces the route limits (`icon_mode` 0, `input_device` 1).
- `open_fresh` now checks the profile, then the session, then the next room, and returns a separate error for each. The order lets tests reach every gate without a seam.
- The producer pins only the opcode, rs and rt of the six address words (mask 0xFFFF0000). The decoded lui/immediate addresses must equal the source symbols, so that check can now fail. The profile bytes did not change (SHA f6a695ef...).
- Tests: Python 22/22 (`tokimemo-recovery-splat`, including original-track cases). `run_holiday_scene_profile_tests.gd` passes 61 checks, including a synthetic installed bundle that reports `holiday_scene` in `missing_gameplay_roles`. Five adapter mutations each fail the suite: removing the size check, the compose branch, the profile branch, the club count or the scalar bounds. Core runner passes 5 suites.

2026-10-29 fresh-handoff check: the `holiday_scene` gap stays. At command select the fresh handoff knows these values:
- `icon_mode` 0 (HUD session)
- `screen_brightness` 0x80 (stage 21 / room finish)
- `clubs` (rule_state, now projected)

These are still missing:
- `input_device`, which pad_input.c can rewrite every frame and which the port state does not track.
- `tile_layer_enabled` and `boot_hold`, which settings_command_screen_setup_reset sets to 0. The production setup_reset service records neither.

The holiday needs all six values at holiday entry, days later, and the school-day and event writers in between are not all modelled. No supplier was added.

2026-10-29 fresh handoff v2: golden values at command select for this route only (newgame-golden-v1 a6-a8, sets A and B). `input_device` is 1, `tile_layer_enabled` 0 and `boot_hold` 0. They are evidence for this route at this point in time. They are not defaults for the holiday producer, and no supplier was added.

## Comments
