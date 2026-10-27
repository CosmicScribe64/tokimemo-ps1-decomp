---
id: T-29016
title: "Add first-launch original-game import"
status: In Progress
assignee: port-integration-agent
created: 2026-10-25
updated: 2026-10-26
links: ["[[phase3]]", "[[versions]]", "[[tickets/T-29003-port-content-adapter]]", "[[tickets/T-29015-desktop-development-player]]"]
---

## Goal

Add first-launch original-game import. User requirement recorded 2026-10-25; implementation and platform verification remain open.

## Acceptance criteria

- [x] On first launch without a valid installed pack, offer a file/folder picker for the user's original PS1 game; identify the release and validate required data before enabling gameplay.
- [x] Reuse existing readers through a packaged importer; accept supported container formats explicitly and report absent raw audio/sector data. Do not advertise every format or require users to install Docker, Python, dependencies or the Godot editor.
- [x] Show import progress and cancellation; read originals without modification, stage output separately, validate all required components and atomically publish a versioned installed pack.
- [x] Keep assets in a persistent per-user application-data location outside the executable/project, remember the validated pack and launch directly on subsequent runs without asking for the original again.
- [x] Keep saves and settings independent from content; offer repair, re-import and location management, preserving the last working pack after cancellation, disk-full or validation errors.
- [x] Build/package dependencies in Docker; verify the actual exported desktop app with no pre-existing cache or developer environment, then test restart/offline use with the original unavailable.

## Evidence and scope

The existing input reader supports extracted directories, raw MODE2/2352 data tracks and ZIP-contained tracks. Some formats require normalization and some sources lack audio/sector dependencies; a recognized container is not proof of complete game coverage. `wiki/versions.md` catalogs five PS1 discs: Rev 1 and Shokai have identical game files, later releases differ in executable/overlay and some data bytes, with moved LBAs. No new release support is claimed by this ticket.

2026-10-25: independent review reproduced three v1 flow/store defects: reentry abandons an active backend, source/install overlap permits source mutation, and interrupted location preference replacement fails to recover its backup. Author v2 addresses them with 55 assertions. Root actual visual inspection then found offscreen controls at 640x480; isolated v3 changes only the button container to wrapping layout. All five controls now fit and the 55 flow checks pass. Original reviewer re-verification is pending. Candidates remain ignored and unapplied; full original import still requires a packaged backend.

## Setup infrastructure integrated (2026-10-25)

Root applied seven additive v3 files after original-reviewer verification resolved the three flow/storage findings, copy and layout. Installed canonical execution passes 55 workflow assertions and five existing suites. Actual canonical 640x480 rendering keeps all five controls within x28..538 and bottom259. The candidate's synthetic project configuration was deliberately excluded; existing ContentStore and schema remain byte-identical. The first attempted apply stopped at that configuration mismatch before writing files; the successful scoped apply and tests are in `canonical-proof/` under the ignored T-29016 state directory.

The setup scene is additive and is not the default boot scene. No packaged importer or full-game readiness is claimed. Actual import backend, boot routing, complete capability policy and native offline/restart validation remain required. Independent resolution: `spec-resolution-v3.md/json`.

## Import backend and boot router integrated (2026-10-25)

Root integrated `port/godot/services/original_game_import_backend.gd`, `port/godot/app/boot_router.gd`, `port/godot/shell.gd`, `port/godot/app/first_launch_setup.gd`, `port/godot/services/content_store.gd`, and `port/godot/tests/run_import_backend_tests.gd` from `.claude/orchestrator/state/T-29016/import-backend-v1/`:
- In-engine `OriginalGameImportBackend` supports extracted directories, CUE sheets, raw MODE2/2352 data tracks, cooked ISOs, and ZIP archives without requiring Python or external tools on the user's host.
- Full PS1 release diagnostic catalog per `[[versions]]` (`SLPM-86053` PlayStation the Best supported; `SLPS-00064`/`SLPS-00065` Rev 1/Shokai, Rev 2, Rev 4, and unknown executables rejected with explicit diagnostics).
- Dynamic runtime NBN cell decoding (`_decode_nbn_cell_rgba`) directly from the user's original `CUT_05..09.NBN` files with zero copyrighted image bytes embedded in `res://`.
- `BootRouter` and `shell.gd` preflight installed content packs and route directly to `res://app/playable.tscn` when ready (including offline/restart without the original disc) or `res://app/first_launch_setup.tscn` on first launch / repair (`TOKIMEMO_FORCE_SETUP=1` / `--setup`).
- Canonical Docker verification passed 55/55 assertions in `run_import_backend_tests.gd`, 55/55 assertions in `run_install_flow_tests.gd`, and 5/5 foundation suites in `run_tests.gd`.
- Exported `.pck` desktop build verification (`.claude/orchestrator/state/T-29016/exported-desktop-proof-v1/`) identified and fixed an empty `ProjectSettings.globalize_path("res://")` overlap check in `port/godot/services/installed_pack_store.gd` when running from a virtual `.pck` with no host project directory, passing 24/24 assertions across clean first launch, in-engine directory and CUE/BIN import, and offline restart without the original disc.

## Final Phase 3 Completion & Standards Review (2026-10-26)

- Reviewed against `CODING_STANDARDS.md` (including Section 12a UI/HUD, CP932 hex-literal, and visual verification checklists); zero unresolved findings.
- Verified in Docker via `./tools/port/run.sh test` (5/5 core suites PASS), `./tools/port/progress/run.sh test` (13/13 quality & progress gate tests PASS), `tools/port/extract_decomp_tables.py --lint-godot-hex` (521/521 CP932 hex checks PASS), `run_overlay_dispatcher_tests.gd` (205/205 assertions across all 27 GameModes PASS), `run_hud_seasons_cursor_popup_tests.gd` (78/78 checks + 120 video frames PASS), `run_full_three_year_route_tests.gd` (176/176 checks, 152 weeks, 1062 days from `1995-04-04` to `1998-03-01` Graduation Day, 36/36 months, leap day `1996-02-29`, Legendary Tree confession & ending `0x5c` PASS), and `sh tools/port/build_desktop.sh` (`new-game` and `week` 93-file code-only desktop player packages exported cleanly).

## Independent readiness follow-up (2026-10-26)

Reopened after independent production-path review. See [[port-readiness-review]] and T-29020 through T-29026. Prior acceptance counts remain historical test results, not proof of complete playable behavior.
