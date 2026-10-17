---
id: T-21008
title: "TACO func_8014A480: apply the T-13801 three-row form to the NON_MATCHING draft"
status: Backlog
assignee:
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-13801-taco-actor-family]]", "[[tickets/T-13800-taco-families]]"]
---

## Goal

TACO func_8014A480: apply the T-13801 three-row form to the NON_MATCHING draft. func_8014A480 (796 bytes) is a NON_MATCHING draft in src/ovl/TACO/80149F90.c at 12 differing lines (T-19003: spill slots 4 bytes low). T-13801 reached the original 128-byte frame and only three differing rows (final actor-base add commuted; constant versus address lui pair) using separate x/y/index locals, no cached selector or index locals, the MATRIX aggregate copy, TcPos vectors and mixed pointer-addition order (see taco-family-results). The scratch files survive in /Users/lila/Documents/Codex/2026-10-15/help-me-free-up-space-x20/outputs/tokimemo-worktree-backups/taco-13800.tar.gz (taco-13800/build/t13801/a480-*.c) and branch codex/taco-13800. Rebuild that form in Docker against current main (types changed since 97ec697f), compare with the guard, keep the better one under NON_MATCHING with its line count, then try the T-19019 func_801497EC idioms on the last rows. Do not repeat the variants listed as dead ends in taco-family-results.

## Acceptance criteria

- [ ] The work in the Goal is done, or each part left undone is recorded in this ticket with the reason.
- [ ] Code review against CODING_STANDARDS.md has no open findings.

## Notes

Opened from the 2026-10-17 ticket cleanup review of [[tickets/T-13801-taco-actor-family|T-13801]], [[tickets/T-13800-taco-families|T-13800]].

## Comments
