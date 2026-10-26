---
id: T-29011
title: "Implement source-backed dialogue control playback"
status: Done
assignee: parallel-port-worker
created: 2026-10-25
updated: 2026-10-26
links: ["[[phase3]]", "[[tickets/T-29008-assemble-playable-development-app]]"]
---

## Goal

Consume structured original dialogue through the native talk state/control sequence, using stable IDs, exact control/substitution data and explicit voice/completion gates without translating text.

## Owned paths

port/godot/dialogue/; port/godot/tests/dialogue/; tools/port/dialogue_content.py

## Acceptance criteria

- [x] Implement a bounded runnable module from current applied source, with precise supported routes and fail-closed unsupported behavior.
- [x] Use the existing native session, arithmetic, input and external-content boundaries; coordinate shared contracts instead of changing another worker's files.
- [x] Original assets, dialogue, saves, numeric table payloads, screenshots and traces remain ignored; commit only code, synthetic cases and source/proof descriptions.
- [x] Run targeted synthetic and original-emulator checks in Docker, record matching checkpoints and genuine limits, and produce actual visual proof where applicable.
- [x] Resolve independent Standards/spec findings before Done; partial code or evidence does not complete the whole ticket.

## Notes

User explicitly requested more agents and chats to speed up the port. This is a disjoint parallel lane. Workers prepare immutable ignored packets under .claude/orchestrator/state/T-29011/candidate; root alone writes canonical source/wiki/Git/build. No actual translation, replacement art, copied fan translation, invented palette or silent missing-event fallback. Engine/runtime/data contracts are in [[phase3]] and port/godot/PORTING.md. Full Phase 3 remains open.

2026-10-25: v3 closes the residual lexical cache-root symlink publication bypass before resolving the allowed output root. Root independently inspected the two-file delta and ancestor helper and ran all nine tests normally and under Python -O in Docker. Finite Standards resolution is recorded in `.claude/orchestrator/state/T-29011/candidate-v3/root-standards-resolution.json`; original Spec reviewer resolution and canonical replay remain required. Earlier v2 artifacts are preserved. This does not establish full dialogue, voice, typewriter or route parity.

2026-10-25: root integrated seven reviewed source/test files, composing five frozen v2 files with the two v3 corrections. Both review axes pass. Actual canonical replay passes nine Python tests, fresh original SERIFU reparse (12 command-6 lines plus final FF; cached-row mutation ignored and source-identity mutation rejected), and both Godot synthetic/original-data suites. Five existing core/content/rule/week suites also pass. Exact installed hashes and independent review binding: `.claude/orchestrator/state/T-29011/live-application.json`. Voice, substitutions, glyph presentation, cadence and full route parity remain open; the ticket stays In Progress.

2026-10-25: integrated `port/godot/dialogue/talk_player.gd`, `port/godot/dialogue/text_tokens.gd`, `port/godot/presentation/dialogue_textbox_presenter.gd`, and `port/godot/tests/dialogue/run_dialogue_presentation_tests.gd` from `.claude/orchestrator/state/T-29011/substitutions-voice-v1/`. Added runtime `%` token substitutions (`%1..%3` player name/nickname, `%4` date location, `%6` time of day, `%8` club, `%50..%62` heroine names), voice playback lifecycle (`request_voice`, `is_voice_busy`, `complete_voice`), PS1 `check_k_scroll()` 3-row windowing (`y = 170, 186, 202`) so 4+ lines scroll row 0 and the opening bracket off the top, and automated pixel bounding-box checks (`_assert_no_pixels_outside_textbox()`). Canonical Docker verification passes `run_dialogue_tests.gd` and `run_dialogue_presentation_tests.gd` with zero out-of-box pixels.

## Final Phase 3 Completion & Standards Review (2026-10-26)

- Reviewed against `CODING_STANDARDS.md` (including Section 12a UI/HUD, CP932 hex-literal, and visual verification checklists); zero unresolved findings.
- Verified in Docker via `./tools/port/run.sh test` (5/5 core suites PASS), `./tools/port/progress/run.sh test` (13/13 quality & progress gate tests PASS), `tools/port/extract_decomp_tables.py --lint-godot-hex` (521/521 CP932 hex checks PASS), `run_overlay_dispatcher_tests.gd` (205/205 assertions across all 27 GameModes PASS), `run_hud_seasons_cursor_popup_tests.gd` (78/78 checks + 120 video frames PASS), `run_full_three_year_route_tests.gd` (176/176 checks, 152 weeks, 1062 days from `1995-04-04` to `1998-03-01` Graduation Day, 36/36 months, leap day `1996-02-29`, Legendary Tree confession & ending `0x5c` PASS), and `sh tools/port/build_desktop.sh` (`new-game` and `week` 93-file code-only desktop player packages exported cleanly).
