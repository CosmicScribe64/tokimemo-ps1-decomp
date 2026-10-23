---
id: T-24018
title: Recover large DATE and EVENT sibling state machines
status: Done
assignee: Codex
created: 2026-10-18
updated: 2026-10-20
links: ["[[date-family-results]]", "[[event-family-results]]"]
---

## Goal
Port newly matched sibling source forms to DATE func_801462B0 and EVENT func_8011C750/func_8011DDB8. DATE2 source is reference only.

## Acceptance criteria
- [x] Bounded source-based sibling attempts with one Docker compiler and ninja -j1.
- [x] Any new C match has no target INCLUDE_ASM or guard, literal default linked proof and unit checksum.
- [x] Preserve useful source-only failed hypotheses; leave In Review for root review.

## Notes
Start 9d7049463, existing complete-bunkaken worktree. Prior T-12100 DATE recovery was distant. T-23004 EVENT frames were exact but allocation remained. Exclusive source files 801461B0.c and EVENT/8011C650.c; notify root before EVENT header edits. No push/merge.

## Comments

## Result
No new matching C. Production source and headers unchanged; all three targets remain INCLUDE_ASM. Source-only ports and reproducible full-TU scoring harness are in drafts/T-24018/.

DATE func_801462B0: DATE2 port plus DATE-specific callback branches restores target frame 0x68 and all 951 instructions. First 442 instructions exact, 434 printable diff rows remain, mostly later save-value/selector allocation. Callback arrays and return views do not solve it.

EVENT func_8011C750: source port using existing views reaches 167 printable rows, 924 versus 925 target instructions. The direct exit-state reload improves 317 to 167. Callback s32 views and page-byte access forms do not close the remaining register/branch gap.

EVENT func_8011DDB8: fully adapted source port still 357 printable rows, 1005 versus 998 instructions, frame 0x68 versus 0x60. T-23004 remains a better starting point; its preserved source and negative evidence were not changed. This draft needs scratch-only s32 self/callee views for return-call branches; no header edit proposed.

All three archive commands reproduced their recorded scores in Docker. Baseline ninja -j1 DATE.ok/EVENT.ok reports no work; independent Docker hashes confirm both binaries exact. These are assembly-fallback controls, zero C progress. git diff --check clean. Leave In Review for root standards review; no push or merge.

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
