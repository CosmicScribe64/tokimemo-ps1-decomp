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
