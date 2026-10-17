---
id: T-21010
title: "Remove original instruction rows from the drafts/ archive and fix its one-off scripts"
status: Backlog
assignee:
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-14600-date-draft-repairs]]", "[[tickets/T-15700-taco-taiiku-draft-repairs]]", "[[tickets/T-16000-main-draft-compilation]]", "[[tickets/T-14500-exhaustive-drafts]]"]
---

## Goal

Remove original instruction rows from the drafts/ archive and fix its one-off scripts. CODING_STANDARDS section 10 allows small asm excerpts only, and drafts/README.md says generated assembly is excluded, but main tracks 251 comparison.txt/difference.txt files (53,374 rows: instruction word plus mnemonic for the original side and ours; T-14600 9 files/2,361 rows, T-15700 28/9,828, T-16000 13/7,675, the rest in T-14500, T-14800, T-14900, T-15600, T-16100) and 110 assembly-evidence.txt files (4,675 lines of addressed disassembly, all T-16100). Decide the policy with the owner, then replace these with score, row count, class and sha256 (regenerate on demand with tools/funcdiff.py in Docker), stop the writers (tools/date_draft_repairs.py, tools/main_draft_repairs.py, the T-16100 drivers) from emitting them, and say in the ticket whether the repo was ever pushed publicly and whether a history rewrite is wanted. Same ticket: give tools/date_draft_repairs.py and tools/main_draft_repairs.py a non-zero exit on failure and a unit test, or move them under their drafts/ folders as archive-only scripts; optionally drop the 2,813 worker.log and 2,725 phase.txt files and re-measure the 222 MB archive.

## Acceptance criteria

- [ ] The work in the Goal is done, or each part left undone is recorded in this ticket with the reason.
- [ ] Code review against CODING_STANDARDS.md has no open findings.

## Notes

Opened from the 2026-10-17 ticket cleanup review of [[tickets/T-14600-date-draft-repairs|T-14600]], [[tickets/T-15700-taco-taiiku-draft-repairs|T-15700]], [[tickets/T-16000-main-draft-compilation|T-16000]], [[tickets/T-14500-exhaustive-drafts|T-14500]]. Overlapping drafts-archive followups: [[tickets/T-21009-draft-archive-hygiene-comparison-rows|T-21009]], [[tickets/T-21011-drafts-listings-and-t16100-tools|T-21011]]; decide the policy once and apply it to all three.

## Comments
