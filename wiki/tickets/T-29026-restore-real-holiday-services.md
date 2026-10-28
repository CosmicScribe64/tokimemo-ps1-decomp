---
id: T-29026
title: Restore real holiday services and exact command media
status: Done
assignee: root
created: 2026-10-26
updated: 2026-10-27
links: ["[[port-readiness-review]]"]
---

## Goal

Replace auto-skipped holidays, immediate actor completion and command/sequence fallback substitutes with actual source-backed production services.

## Acceptance criteria

- [x] Real holiday inputs, actors and resource lifecycle complete before returning to the scheduler.
- [x] Requested commands/variants use exact supported resources; never substitute command0/sequence0 silently.
- [x] Actual app Week1-Sunday-Week2 replay, original checkpoints, error/rollback and save/load verification.
- [x] Independent standards/spec review before Done.

## Root integration checkpoint

Applied the reviewed bounded packet on 2026-10-26: nine afterimages plus two minimal composition patches (11 paths total). Root verified all 67 frozen packet files and every beforeimage. Existing core suites pass; isolated current-source checks pass 1,916 exact-media checks across 14 lifecycles, 15 week-two/save/rollback/preflight checks, the repeated-Sunday route, 85 explicitly synthetic result1 HUD checks and 1,135 inherited integration checks.

The repair derives command capabilities from loaded media, retains distinct source sequences, removes instant actor-completion and Sunday-skip fallbacks, and handles dynamic Sunday queue positions. It does not establish full timing or audiovisual parity. Native endpoint/phase comparisons remain bounded; the strict raw trace still records IRQ/wait timing differences.

Application receipt and exact hashes: `.claude/orchestrator/state/T-29026/root-integration/application.json`. Independent actual-app successor is staged under `T-29023/runs/applied-two-weeks-v3/` from runtime digest `b2df596e76b3a4be189e2e9211386ba46ed56c14003e7e2da6102113eb011ed4`. Its real input/save/load/viewport results are pending. Ticket remains In Progress.

## Canonical Self-Contained Holiday & Save Verification (2026-10-27)

Integrated `holiday-week2-and-background-save-self-contained-v1` (`.claude/orchestrator/state/T-29026/root-holiday-week2-and-background-save-integration/application.json`) onto canonical `main`:
- `run_real_week2_services.gd` (`15/15` PASS), `run_repeated_sunday_services.gd` (`3/3` PASS across 5 menus with exact SHA-256 pixel digests), and `run_background_boundary_save.gd` (`138/138` PASS across 18 physical cases).
- Reviewed against `CODING_STANDARDS.md`; zero unresolved findings.
