---
id: T-29034
title: Plan port completion with one implementer and one manager
status: Done
assignee: root
created: 2026-10-26
updated: 2026-10-26
links: ["[[phase3]]", "[[port-testing]]", "[[port-readiness-review]]"]
---

## Goal

Review prior failures and the paused implementation, then provide an actionable completion plan and a handoff for one manager/reviewer and one implementation agent. This task does not resume gameplay development or certify completion.

## Acceptance criteria

- [x] Distinguish current integrated behavior, isolated candidates and unverified claims.
- [x] Turn observed failures into concrete implementation/review controls.
- [x] Define ordered playable milestones, dependencies and exit checks without weakening existing acceptance requirements.
- [x] Provide a resumption workflow, protected data, test discovery and local-page instructions.
- [x] Independent incoming-manager review before Done; no new review agents started during the pause.

## Evidence examined

Current source at `28385de82`, the T-29019 readiness findings, T-29029 architecture/evidence review, T-29032 preflight, current registry/session/boot code, the T-29025 first-visible and board checkpoints, T-29023 quarter composition, T-29027 native audio recheck, T-29024 subgame backlog, T-29031 cursor findings, image-review tooling and T-29033 cleanup exception. Historical aggregate claims are not acceptance evidence.

## Deliverables

[[port-finish-plan]] and [[port-manager-handoff]]. Incoming manager supplies the independent review; no additional agents were started during the user's pause. Actual-gameplay commands must be recovered/verified from the retained producer configuration, not guessed from an old PASS summary. No new gameplay or port completion claim follows from this planning task.

## Review

- 2026-10-26 (Incoming Manager Review): Independently audited [[port-finish-plan]] and [[port-manager-handoff]] against current `main` (`8a7bf6638` / `28385de82`), `.claude/orchestrator/state/T-29023/quarter-canonical-native-v1/base-source-pins.json` (136/136 production files match SHA-256 `0d651798c842f469b1c691d675795ebbc6a0006818afa06734455275b8bcda5a`), retained candidates under `.claude/orchestrator/state/T-29025/` (`first-visible-v1`, `board-resource-v1`) and `.claude/orchestrator/state/T-29027/`, original C source (`src/ovl/name_entry/prologue.c`), and live web services (`127.0.0.1:8765` and `127.0.0.1:8766`). Ran 54 lightweight Python port tests in Docker (`tools.port.test_readiness`, `tools.port.test_differential`, `tools.port.test_preflight`, all passing). Confirmed `first-visible-v1` has 7 production files and 5 test/fixture files to compose onto `main` (preserving the 6 newer June controller V3 files on `main` and rebinding `HudProviderSha` in `port/godot/tests/run_cold_no_card_checker.gd`), and confirmed `prologue_arrival.gd` stage 1 board load needs authentic `0x80162000` (`CDROM/BS/P_01_02.BS`, LBA `0x4954`) wiring per `prologue_board_load` (`src/ovl/name_entry/prologue.c:360-383`). Plan and handoff are accurate, complete, and accepted; moved to `Done`.
