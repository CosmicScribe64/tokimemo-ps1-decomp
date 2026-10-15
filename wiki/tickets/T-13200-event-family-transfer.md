---
id: T-13200
title: Transfer verified event handler source families
status: In Progress
assignee: codex-events-13200
created: 2026-10-15
updated: 2026-10-15
links: ["[[bunka-family-results]]"]
---

## Goal
Increase literal verified game bytes in GYOZI and SHOUGATU from aa2b3158 using bounded source family experiments. Original checkout is read-only.

## Acceptance criteria
- Preserve existing C and full translation-unit compiler context.
- Literal linked byte proof and affected binary SHA1 for every accepted match.
- No generated assembly edits, global header changes, or metric changes.
- Review against CODING_STANDARDS.md before Done.

## Notes
Created isolated codex/events-13200 worktree. Reuse existing Docker image and copy ignored integration inputs. Read bunka-family-results, matching-breakthroughs, broad-coverage-results and coding standards. Local GameState pointer, actual CharFlags bitfields and mode/store order are the first bounded hypotheses.

## Comments
