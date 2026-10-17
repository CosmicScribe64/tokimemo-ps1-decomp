---
id: T-21009
title: "Draft archive hygiene: target-instruction rows in comparison.txt and untested one-shot repair tools"
status: Backlog
assignee:
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-15100-unaligned-draft-repairs]]"]
---

## Goal

Draft archive hygiene: target-instruction rows in comparison.txt and untested one-shot repair tools. (1) Decide, against CODING_STANDARDS section 10 and the licensing-clean rule, whether drafts/**/comparison.txt may stay in the repo: 73 tracked files (T-14500: 18, T-14900: 14, T-15600: 13, T-15700: 28), T-15100's nine alone hold about 5,550 rows of original instruction words with mnemonics. If not, keep only row counts and scores in result.json, gitignore the files and drop them from the tree (history rewrite is the owner's call). (2) tools/unaligned_draft_repairs.py, tools/date_draft_repairs.py and tools/main_draft_repairs.py are one-shot scripts with hard-coded paths and no tests; the scopes they served were redrafted by the types-first passes and tools/redraft.py (T-20800). Retire them or add path arguments and a test with synthetic input (section 11).

## Acceptance criteria

- [ ] The work in the Goal is done, or each part left undone is recorded in this ticket with the reason.
- [ ] Code review against CODING_STANDARDS.md has no open findings.

## Notes

Opened from the 2026-10-17 ticket cleanup review of [[tickets/T-15100-unaligned-draft-repairs|T-15100]]. Overlapping drafts-archive followups: [[tickets/T-21010-drafts-archive-scrub-instruction-rows|T-21010]], [[tickets/T-21011-drafts-listings-and-t16100-tools|T-21011]]; decide the policy once and apply it to all three.

## Comments
