---
id: T-29034
title: Plan port completion with one implementer and one manager
status: In Review
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
- [ ] Independent incoming-manager review before Done; no new review agents started during the pause.

## Evidence examined

Current source at `28385de82`, the T-29019 readiness findings, T-29029 architecture/evidence review, T-29032 preflight, current registry/session/boot code, the T-29025 first-visible and board checkpoints, T-29023 quarter composition, T-29027 native audio recheck, T-29024 subgame backlog, T-29031 cursor findings, image-review tooling and T-29033 cleanup exception. Historical aggregate claims are not acceptance evidence.

## Deliverables

[[port-finish-plan]] and [[port-manager-handoff]]. Incoming manager supplies the independent review; no additional agents were started during the user's pause. Actual-gameplay commands must be recovered/verified from the retained producer configuration, not guessed from an old PASS summary. No new gameplay or port completion claim follows from this planning task.
