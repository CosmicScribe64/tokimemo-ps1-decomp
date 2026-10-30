---
id: T-39001
title: Assess fresh checkout and recovery priorities
status: In Review
assignee: Codex
created: 2026-10-29
updated: 2026-10-29
links: ["[[status]]", "[[port-finish-plan]]"]
---

## Goal
Assess the freshly cloned repository, original archive, and next steps after laptop cleanup.

## Acceptance criteria
- [x] Record current source and local dependency state.
- [x] Distinguish recorded acceptance from freshly reproduced evidence.
- [x] Recommend ordered recovery and release checks.

## Notes
Assessment only; no development agents or automation resumed.

## Assessment (2026-10-29)

Initial checkout clean on main at f8497f8ca, matching the local origin/main tracking ref (remote not queried). This commit records 25 Phase 3 slices and G1–G4 verification on October 27, later than the October 26 orchestration handoff. Phases 1/2 record 27 matched binaries, 6,952 functions and 2,276,920 bytes. These are historical results, not rerun here. Older pause/handoff sections conflict with the later integration and must not be treated as current instructions to reapply packets.

Original 472 MiB 7z exists in /Users/lila/Documents. About 122 GiB available. Docker.app and CLI exist, but daemon socket is absent; image inventory and tests could not run. disc/, .asset-cache/, .claude/ and AGENTS.local.md are absent. Versioned translations/en/graphics/artwork survives. Missing ignored evidence is not restored by cloning; T-39000 explicitly describes deleting orchestration state.

Next sequence:
1. Start Docker and inventory retained images before building anything.
2. Read-only mount the original 7z, inspect and validate its contents in Docker. Existing tools/extract_disc.py expects ZIP, so do not pass 7z directly; prepare compatible input in ignored scratch or use a supported importer after inspecting its interface. Preserve original archive and avoid duplicate full caches.
3. Regenerate the minimum installed runtime with tools/port/build_installed_runtime.py and original content; rerun focused current G1/G2 checks and real input opening/save/relaunch. Audit G3/G4 tests against actual-route criteria before renewing completion claims.
4. Reconcile the recorded complete opening with first_arrival's documented stage-7 visual boundary; missing retained Rei candidate cannot be assumed available or accepted.
5. Complete G5: clean import, native macOS package, real controls/audio/focus/restart, then declared Linux/Windows checks. Keep Phase 4 expansion behind this gate.

No runtime validation, archive integrity test, tool installation, extraction, or agent/automation restart performed. Documentation-only review remains pending; ticket stays In Review.
