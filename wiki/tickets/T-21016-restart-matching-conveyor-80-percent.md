---
id: T-21016
title: "Restart the matching conveyor toward 80 percent game-code coverage"
status: Backlog
assignee:
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-12600-byte-coverage-families]]"]
---

## Goal

Restart the matching conveyor toward 80 percent game-code coverage. T-16990 paused the wave 3 conveyor at 6,358/6,958 functions and 75.8 percent of bytes; 80 percent of the 2,279,368 game-code bytes is 1,823,494, so about 96 KB of verified matches remain. Restart after the type round (T-20900..T-20940) and the tooling round (T-20800..): rebuild the inventory (the 81 unstarted packages return to it, including the BUNKA_SD packages p02/p03 and the leftovers of T-12090, T-12030, T-12050 and T-12300), re-rank by bytes per agent-hour with the new tools (redraft, viewsweep, autotweak recipes), keep Sonnet matchers with Opus closers on near misses of 20 rows or fewer, and exclude contributor-owned overlays (TT, diman8 issue #2). Acceptance: every started package reviewed and merged, 27/27 clean build, headers, globals and prototype checks pass, coverage figure recorded.

## Acceptance criteria

- [ ] The work in the Goal is done, or each part left undone is recorded in this ticket with the reason.
- [ ] Code review against CODING_STANDARDS.md has no open findings.

## Notes

Opened from the 2026-10-17 ticket cleanup review of [[tickets/T-12600-byte-coverage-families|T-12600]].

## Comments
