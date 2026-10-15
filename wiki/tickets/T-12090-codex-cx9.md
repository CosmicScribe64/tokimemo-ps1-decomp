---
id: T-12090
title: Match cx9 BUNKA_SD functions
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: []
---

## Goal

Deep matching pass over ten assigned BUNKA_SD functions, prioritizing verified byte gains.

## Acceptance criteria

- [ ] Exact linked and overlay checksum proof for every match.
- [ ] Retain only measured guards within about thirty differing rows.
- [ ] Full configure/build and prototype verification; independent standards review before Done.

## Notes

Owned files: src/ovl/BUNKA_SD/80132000.c, 80134260.c and 801388D0.c. No main or other worktree edits. Deadline 21:45 UTC.

## Comments

2026-10-15: Read wiki index, project/local instructions, coding standards, workflow and register cheat sheet. Searched historical evidence for all ten functions. Baseline Docker configure and ninja -j2 pass 27/27 SHA1 checks, headers and globals. Prior small fade loops have known allocation gaps; prioritize the 1164/1604-byte functions and their setup family after checking the old score-zero dispatcher.

### func_80132428: MATCH, 448 text bytes

Canonical GameState selector/timer access and six missing overlay forward declarations make the old score-zero dispatcher compile in its complete translation unit. Plain C, no FAKE. Exact production linked comparison and BUNKA_SD SHA1 pass; headers/globals pass. Worklist size512 includes the64-byte jump table.
