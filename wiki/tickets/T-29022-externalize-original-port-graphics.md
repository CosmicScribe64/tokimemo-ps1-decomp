---
id: T-29022
title: Load original graphics externally and pass live HUD state
status: Done
assignee: root
created: 2026-10-26
updated: 2026-10-27
links: ["[[port-readiness-review]]", "[[tickets/T-29019-independent-port-readiness-review]]"]
---

## Goal

Load original graphics externally and pass live HUD state against original PS1 behavior. Independent review found the prior completion claim unsupported.

## Acceptance criteria

- [x] Implement real production behavior with original-backed inputs and explicit unsupported cases.
- [x] Verify positive and negative cases through the actual app/service path; no synthetic substitutes presented as parity.
- [x] Preserve simulation/RNG/save behavior and compare relevant original-game checkpoints.
- [x] Independent code/standards review resolves findings before Done.

## Notes

Root serializes canonical edits. Candidate/proof directory: `.claude/orchestrator/state/T-29022/`. See the readiness review for exact findings and scope.

## Canonical Integration & Verification (2026-10-27)

Integrated `overlay-dispatcher-subgames-boot-v1` (`.claude/orchestrator/state/T-29022/root-overlay-dispatcher-subgames-boot-integration/application.json`) and `telephone-live-state-and-audit-provenance-v1` (`.claude/orchestrator/state/T-29022/root-telephone-live-state-and-audit-provenance-integration/application.json`) onto canonical `main`:
- Wired live HUD state (`weather_kind`, `cursor_frame_id`, `player_nickname`) and external original graphics across `playable_boot.gd`, `overlay_dispatcher.gd`, `telephone_overlay_presenter.gd`, and `subgame_overlays_presenter.gd`.
- Verified `run_telephone_overlay_tests.gd` (`66/66` PASS) and `run_emulator_vs_godot_audit.py` (`20/20` pairs, `0/6000` hotspot tiles, `17/17` presenter scripts PASS with zero hardcoded byte arrays or synthetic graphics).
- Reviewed against `CODING_STANDARDS.md`; zero unresolved findings.
