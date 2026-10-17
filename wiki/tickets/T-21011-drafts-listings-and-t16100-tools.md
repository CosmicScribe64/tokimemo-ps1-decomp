---
id: T-21011
title: "Keep game-derived instruction listings out of committed drafts/ and test or retire the one-shot T-16100 tools"
status: Backlog
assignee:
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-16100-flagged-draft-pass]]"]
---

## Goal

Keep game-derived instruction listings out of committed drafts/ and test or retire the one-shot T-16100 tools. drafts/T-16100 commits 165 difference.txt and 110 assembly-evidence.txt files (raw instruction words and disassembly of the original code, about 40,000 lines); drafts/T-16000, T-14500, T-14800, T-14900 and T-15600 hold comparison.txt/evidence.txt files of the same kind (check each). CODING_STANDARDS section 10 allows small excerpts only, and the project rule is no committed disassembly. Public main has no drafts/ today. Decide per file type whether to strip the listings (keep scores, hashes, body.c, context.c) or keep drafts/ off the published branch, make tools/refresh_draft_ledgers.py and the draft tools work without the listings, and regenerate them on demand from scratch. Also either add tests for the integrity checks in tools/t16100_join.py or move the one-shot tools/t16100_batch1-3.py into drafts/T-16100 as provenance, since tools/ scripts need tests (section 11).

## Acceptance criteria

- [ ] The work in the Goal is done, or each part left undone is recorded in this ticket with the reason.
- [ ] Code review against CODING_STANDARDS.md has no open findings.

## Notes

Opened from the 2026-10-17 ticket cleanup review of [[tickets/T-16100-flagged-draft-pass|T-16100]]. Overlapping drafts-archive followups: [[tickets/T-21009-draft-archive-hygiene-comparison-rows|T-21009]], [[tickets/T-21010-drafts-archive-scrub-instruction-rows|T-21010]]; decide the policy once and apply it to all three.

## Comments
