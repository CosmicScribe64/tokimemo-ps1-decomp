---
id: T-12010
title: Codex cx1 main helper matching
status: In Progress
assignee: Codex cx1
created: 2026-10-15
updated: 2026-10-15
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Deep matching pass over cx1 twelve functions in main objects 800420D0, 800451D0, 80047550 and 80059710. Work only in the cx1 worktree.

## Acceptance criteria

- [ ] Investigate assigned functions and document guarded drafts and blockers.
- [ ] Promoted C passes linked funcdiff and binary checksum verification.
- [ ] Final configure/build verifies 27 binaries; headers, globals and branch prototypes pass.
- [ ] Submit for review against CODING_STANDARDS.md.

## Notes

Package ../codex/cx1-list.txt. Baseline Docker configure/build passed 27/27 SHA1; log /tmp/cx1-baseline.log. Three workers share Docker, subsequent ninja uses -j2. InitMouse has a documented missing data-ownership range; func_800422C8 has a documented call-result register mismatch.

## Comments

Review pending.
