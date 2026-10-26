---
id: T-29014
title: Extend original content coverage to ordinary commands 1 through 6
status: In Progress
assignee: port-command-content-agent
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]", "[[tickets/T-29003-port-content-adapter]]", "[[tickets/T-29009-ordinary-week-continuation]]"]
---

## Goal

Make ordinary commands 1 through 6 usable through the same original-input content boundary as commands 0 and 7. Keep native eligibility, resource selection, animation and outcome semantics; do not substitute another command's art or timing.

## Acceptance criteria

- [ ] Resolve each command's background and SD selections from original executable tables and native callers, including applicable seasonal/conditional variants.
- [ ] Extend reusable readers/exporters and strict runtime descriptors with original input identities, actual geometry, palettes, sequence/frame domains and rendering metadata; exports remain ignored.
- [ ] Reject unavailable required variants before gameplay mutation, with exact diagnostics rather than fallback art.
- [ ] Integrate command capabilities and actual route behavior without changing other commands, save identities or RNG order.
- [ ] Review source, actual exports and negative cases independently; compare reachable gameplay checkpoints and rendered output against the original before claiming route acceptance.

## Notes

Prepare an isolated packet first; root serializes canonical application. Holiday content, HUD production export and rest-week/save fixes have separate owners. Reuse verified table readers and media validators. No emulator use without a serialized lease. Full seasonal coverage remains explicit even if the first implementation covers only the opening spring week. Gemini owns translation; original data never enters Git or res://.

## Comments

2026-10-25: opened to advance full-game command coverage in parallel with first-slice integration. An inventory alone is not completion.

## Command 1–3 content integrated (2026-10-25)

The four-file exporter/loader/test tranche is applied after independent source review and re-verification of both portability and external-pack findings. Installed execution passes five invented Python tests and 25 Godot assertions. A fresh canonical original-disc export reproduces all 103 reviewed files exactly, manifest SHA256 `b26ef7e5c0301633c45d9a6a898ca4bdb7be68ee30f163825eed827e4f5c0c28`. Proof: `.claude/orchestrator/state/T-29014/canonical-acceptance.json`; resolved review `spec-resolution-v2.json` in the same directory.

Only opening-week non-summer no-companion content for commands 1–3 is accepted. App/gameplay composition, commands 4–6 and other event/season variants remain open; the ticket stays In Progress. Original pixels and tables remain external under ignored `.asset-cache/port/spring-command-current-236ab3/`.

## Commands 5–6 content integrated (2026-10-25)

Applied five additive files after independent Standards/spec review. Installed clean CLI reproduces all 125 reviewed files and manifest `7c67739f2dcbc5a9c644e670d0c0e5ea91869ab851d686e77cbfbf6e188105b1` at `.asset-cache/port/weekday56-current-e593`. Fourteen Python tests, 90 actual-pack Godot checks and five shared suites pass in a disposable copy of the current project. Proof is `.claude/orchestrator/state/T-29014/weekday56-canonical-proof/`. Only normal-health/no-vacation spring commands 5/6, extra15 and scene0 are supported. Gameplay wiring, command4, other variants and exported-runtime path-helper adoption remain open. No original pixels or tables entered Git.

## Ordinary commands 1–3 and 5–6 gameplay wiring integrated (2026-10-25)

Integrated `port/godot/services/ordinary_command_media_adapter.gd`, `port/godot/tests/app/run_ordinary_commands_1_6_tests.gd`, `ExternalContentPath.is_external_path` adoption in `port/godot/services/spring_school_day_content_store.gd` and gameplay wiring across `port/godot/flow/command_screen.gd`, `port/godot/rules/scheduler.gd`, `port/godot/rules/ordinary_week.gd`, `port/godot/rules/school_day.gd`, `port/godot/scenes/school_day_controller.gd` and `port/godot/app/playable_boot.gd` from `.claude/orchestrator/state/T-29014/gameplay-wiring-v1/`. Root review preserved canonical `_weighted_tokimeki`, `_closeness` and `resolve_initial_encounters` in `scheduler.gd`. Canonical Docker verification passes 238/238 checks across commands 0..3 and 5..7 under render schedules 0/1/3, preserving exact command 0/7 state/event/RNG traces and rejecting command 4 and unsupported companion/health/vacation variants before mutation. Command 4 club variants and seasonal/companion coverage remain open.

## Command 4 club variants and live HUD captures integrated (2026-10-25)

Integrated the Command 4 club activity content exporter (`tools/port/club_command_content.py`, `tools/port/test_club_command_content.py`), external pack store (`port/godot/services/club_command_content_store.gd`, `port/godot/tests/run_club_content_store.gd`, manifest SHA256 `00b844999ad42c174a4163906ca6552929d48ebbe9b96e12d6285b7e2aae65d9` across all 11 clubs), and Command 4 gameplay/media/biorhythm/practice-point rules (`port/godot/services/ordinary_command_media_adapter.gd`, `port/godot/rules/school_day.gd`, `port/godot/rules/scheduler.gd`, `port/godot/rules/ordinary_week.gd`, `port/godot/flow/command_screen.gd`, `port/godot/scenes/school_day_controller.gd`, `port/godot/app/playable_boot.gd`, `port/godot/tests/app/run_club_command_tests.gd`, `port/godot/tests/app/run_ordinary_commands_1_6_tests.gd`) from `.claude/orchestrator/state/T-29014/command4-club-v1/`. Root review preserved canonical `_weighted_tokimeki`, `_closeness`, and `resolve_initial_encounters` in `scheduler.gd` and Sunday holiday integration in `playable_boot.gd`. Canonical Docker verification passes 6/6 Python exporter tests, 45/45 club content store assertions, 278/278 Command 4 club integration checks, and 238/238 ordinary commands 1–6 checks. Also generated 20 live HUD captures in `.claude/orchestrator/state/T-29014/live-captures-v1/` and published 7 captures to the local progress gallery. Summer/winter seasonal and companion variants remain open.

## Seasonal, health, vacation and companion content integrated (2026-10-25)

Integrated the seasonal/companion ordinary command exporter (`tools/port/seasonal_command_content.py`, `tools/port/test_seasonal_command_content.py`, manifest SHA256 `981a3a9d63f1fc5a4a2b7fb85060b81363cf77bd8ce8917d01cbbf8d570636cb` across 155 exported files) and runtime integration (`port/godot/services/seasonal_command_content_store.gd`, `port/godot/services/ordinary_command_media_adapter.gd`, `port/godot/rules/school_day.gd`, `port/godot/scenes/school_day_controller.gd`, `port/godot/tests/app/run_seasonal_command_tests.gd`) from `.claude/orchestrator/state/T-29014/seasonal-companion-v1/`. Canonical Docker verification passes 9/9 Python exporter tests, 266/266 seasonal/companion/vacation/health checks in `run_seasonal_command_tests.gd`, 238/238 checks in `run_ordinary_commands_1_6_tests.gd`, and 278/278 checks in `run_club_command_tests.gd`.
