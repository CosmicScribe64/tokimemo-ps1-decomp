---
id: T-29010
title: "Render the native HUD and game glyphs"
status: In Progress
assignee: parallel-port-worker
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]", "[[tickets/T-29008-assemble-playable-development-app]]"]
---

## Goal

Load original glyph/icon metadata and render the first command/activity HUD with native logical layout, preserving long-text interfaces for later translation.

## Owned paths

port/godot/presentation/native_glyphs.gd; port/godot/ui/native_hud.gd; port/godot/ui/command_hud.gd; port/godot/tests/hud/; tools/port/font_content.py

## Acceptance criteria

- [ ] Implement a bounded runnable module from current applied source, with precise supported routes and fail-closed unsupported behavior.
- [ ] Use the existing native session, arithmetic, input and external-content boundaries; coordinate shared contracts instead of changing another worker's files.
- [ ] Original assets, dialogue, saves, numeric table payloads, screenshots and traces remain ignored; commit only code, synthetic cases and source/proof descriptions.
- [ ] Run targeted synthetic and original-emulator checks in Docker, record matching checkpoints and genuine limits, and produce actual visual proof where applicable.
- [ ] Resolve independent Standards/spec findings before Done; partial code or evidence does not complete the whole ticket.

## Notes

User explicitly requested more agents and chats to speed up the port. This is a disjoint parallel lane. Workers prepare immutable ignored packets under .claude/orchestrator/state/T-29010/candidate; root alone writes canonical source/wiki/Git/build. No actual translation, replacement art, copied fan translation, invented palette or silent missing-event fallback. Engine/runtime/data contracts are in [[phase3]] and port/godot/PORTING.md. Full Phase 3 remains open.

2026-10-25 review/integration checkpoint: the original font packet is frozen at `.claude/orchestrator/state/T-29010/candidate/candidate-files.sha256` and under independent review. Root prepared a separate two-file cache-containment correction at `font-path-fix-v2/`, with five passing Python tests including tracked child-cache lookalikes, external roots, parent traversal and symlinks; no canonical application yet. A captured original command-screen viewer is oracle evidence only and does not count as dynamic HUD implementation or belong in the port progress gallery. The existing worker now performs source/runtime-backed room, MAINROM actor/icon, calendar/parameter and text composition under Sol6.1.

2026-10-25 second finite finding: independent native/Spec review reproduced incorrect fade quantization in the glyph image helper. Modulating expanded 8-bit colors produced native RGB555 (15,19,19) for source channels (20,25,25) at brightness 0x60; native modulation produces (15,18,18). Root's isolated cumulative `font-fix-v3/` modulates five-bit channels before display expansion and adds faded/full/zero-brightness regressions. Its Godot HUD suite passes; original-palette replay and both independent resolution reviews are pending. No font files applied canonically yet.

2026-10-25 accepted font component: both independent review axes accept cumulative v3. Root applied six exact reviewed production/test files, excluding the candidate project and captured-screen viewer. Canonical Docker tests pass five Python cases and the HUD suite. The independent original-data probe rerun against installed canonical code verifies every one of 3,418 glyph bitmaps, range boundaries and all 2,064 palette/brightness images. Evidence: `live-font-application.json` and `live-font-proof/` under this ticket's ignored state directory. Actual dynamic command-screen composition remains in progress; this acceptance covers the font/getter and source-backed HUD record helpers, not full HUD parity.

2026-10-25: dynamic HUD candidate reproduces 64 actor frame/layer/position records for original selections 0 and 7; 29 owner checks and actual Mesa rendering proof are available in `.claude/orchestrator/state/T-29010/dynamic-hud/`. Root inspection found its exporter still requires private oracle RAM for initial state and unpinned room/calendar copies. This is useful rendering evidence, not a reusable production importer. Owner is preparing a versioned original-input adapter with runtime state supplied by the live session, keeping captured actors and comparison fixtures outside runtime content. GPU gradient, MDEC color and predecessor-framebuffer differences remain explicit. No captured-frame viewer is accepted as port gameplay.

2026-10-25: root visually inspected `proof-final/live-command-0.png`, checked the live capture code uses the source compositor through NativeCanvas, and published only the actual Godot command-0/7 captures to the local visual progress page. Served manifest hashes are `a5ea9d562d63dff023179a6481fa2c3185b5ae8dcbecde0f655b1b272dbdae15` and `9bbd3ea8a3f96440581da9402bca49d01cbda2b9697911a786ccaef0d1032796`. Original emulator screenshots and the manually altered date/parameter test remain excluded. This publishes work-in-progress rendering, not production integration or full original-GPU parity; estimates stay unchanged.

2026-10-25: integrated the reviewed composed HUD app successor (`command_hud_presenter.gd`, `hud_compositor.gd`, `live_hud_adapter.gd`, `tools/port/hud_content.py`, `tools/port/test_hud_content.py`), the POSIX `//` alias fix via `ExternalContentPath.is_external_path` (also adopted in `school_day_media_store.gd` and `weekday_56_content_store.gd`), and `playable_boot.gd` composition with `boundary_save_controls.gd` from `.claude/orchestrator/state/T-29010/composed-app-v2/`. Canonical Docker verification passes 8/8 Python HUD exporter tests, 9/9 slash alias checks, 10/10 external path checks, 50/50 live HUD app checks (across render schedules 0/1/3 and save/restore rebinding), and 243/335-step live Mesa HUD captures with clean default viewport. Published 4 new live HUD captures to the local progress gallery. Sunday holiday HUD integration remains tracked with [[tickets/T-29013-native-holiday-command-loop]].

2026-10-25: integrated `port/godot/presentation/holiday_hud_adapter.gd`, `tools/port/holiday_hud_content.py`, `port/godot/tests/hud/checkpoint_smoke.gd`, and `playable_boot.gd` Sunday holiday HUD composition from `.claude/orchestrator/state/T-29013/app-integration-v2/`. Also generated and published 7 live HUD captures for ordinary commands 1, 2, 3, 5, and 6 (`.claude/orchestrator/state/T-29014/live-captures-v1/`) and 3 live Sunday HUD captures (`.claude/orchestrator/state/T-29013/app-integration-v2/proof/`) to the local progress gallery.
