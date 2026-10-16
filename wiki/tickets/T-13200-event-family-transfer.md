---
id: T-13200
title: Transfer verified event handler source families
status: Done
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
- No generated assembly or metric changes. Shared API declarations only when required by standards; validate all27 binary checksums.
- Review against CODING_STANDARDS.md before Done.

## Notes
Created isolated codex/events-13200 worktree. Reuse existing Docker image and copy ignored integration inputs. Read bunka-family-results, matching-breakthroughs, broad-coverage-results and coding standards. Local GameState pointer, actual CharFlags bitfields and mode/store order are the first bounded hypotheses.

## Comments

Source work complete; two-axis review and failed probes summarized in [[event-family-results]]. At that checkpoint final all27 validation was pending; now completed below.

Final review/closure2026-10-15: GPT6.1Sol medium Standards and Spec reviews complete; all findings resolved. Final default literal proof656+1328+1440=3424 bytes, independent27/27 binary hashes, headers/globals and prototype branch check pass (0new/15known). All744 baseline bodies unchanged. Evidence: build/events13200/final-proof.txt, final-hashes.txt, all27-build.log, final-protos.txt, existing-c-audit.txt and progress.txt. Final closure review confirms six particle negatives add no production code or coverage. Ticket Done; broader80% objective belongs to integration and is not claimed complete.
