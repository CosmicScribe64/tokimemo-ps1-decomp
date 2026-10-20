---
id: T-24028
title: Recover DATE and EVENT short movie siblings
status: Done
assignee: Codex
created: 2026-10-18
updated: 2026-10-20
links: ["[[date-family-results]]", "[[event-family-results]]"]
---

## Goal
Recover DATE func_8014718C (1996 bytes) and EVENT func_8011D5C4 (2036 bytes) using matched DATE2 func_801348F0 as a read-only source reference.

## Acceptance criteria
- [x] Bounded 20-25 minute source-shape/type pass; one Docker compiler and ninja -j1.
- [x] Any promotion is actual unguarded normal C, verified by literal default linked funcdiff and unit checksum.
- [x] Preserve useful source-only failed hypotheses; leave In Review for root standards review.

## Notes
Start 9d8cd8719, existing complete-bunkaken worktree. Read T-17201/T-22009 and relevant family notes. Prior DATE guard has 26 differing lines; EVENT hand draft has 409 rows and frame48 versus64. Only DATE/801461B0.c and EVENT/8011C650.c are owned; coordinate before EVENT header edits. No push/merge.

## Comments

## Result
Zero new C bytes. Improved existing DATE guard from 26 to 11 differing instructions (52 to 22 printable rows); EVENT is a source-only seven-instruction near miss (14 printable rows), down from T-22009's distant draft. Exact frames 0x40 and instruction counts 499/509. All fades, loops and callback spills now match through the source sibling's explicit fade-copy form. Required source-shaping comments are marked FAKE.

Both retain the same old T-17201 timer-narrowing gap: candidate raw counter in v1, narrowed stored range in t0/a3, then v1 copy for divides; target raw counter in t0/a3, narrowed temporary t8, copy back into t0/a3. DATE also reverses the final timer/state stores. Width, cast, register, split narrowing, global-update/compare and marked dead-boundary probes do not solve it. No compiler or flag change.

Reproduction and detailed negative evidence: drafts/T-24028/. Both archive probe commands rechecked 22/14 rows. EVENT needs s32 self view in scratch; header stays void, EVENT source remains assembly. Normal Docker ninja -j1 DATE.ok/EVENT.ok rebuilt DATE, headers OK, DATE checksum OK; the guard fallback contributes zero C bytes. git diff --check clean.

Status In Review for root standards review. No push/merge; DATE2 and other owned files untouched.

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
