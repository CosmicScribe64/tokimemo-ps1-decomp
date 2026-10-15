---
id: T-13700
title: Reconstruct NAME_ENT families
status: In Review
assignee: codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[broad-coverage-results]]", "[[matching-notes]]"]
---

## Goal

Find new matching C in NAME_ENT through coherent type and control-flow reconstruction. Preserve existing matching functions and avoid the exhausted T-11570 selector-copy family.

## Acceptance criteria

- Full-TU exact scoring, literal linked-byte proof, NAME_ENT SHA1 for accepted C.
- Review against CODING_STANDARDS.md; record negative evidence.

## Notes

Baseline origin/main 5ccb8765. Isolated codex/name-13700 worktree. Read T-11570, bunka-family-results and broad-coverage-results. All toolchain work uses the existing Docker image.

## Comments

### Bounded prologue setup probes

All scratch-only in build/name13700; no accepted C. func_80142A80 default reconstruction 18 exact full-TU rows; repairing malformed m2c Shift-JIS escape and a 4-byte buffer leaves 12 stack-layout rows. Buffer sizes 8/16 and offset-12 form fail to match; no artificial padding accepted. func_80142E24 word/pointer address-table reconstruction scores 328/324 rows; natural 2x4 nested/row loops score 357/331, worse than historical 56-row default. func_8014472C m2c omits an unused four-entry averaging computation present in target; typed draft126 rows, abandoned. No shared declarations changed. Pivot to larger unattempted menu family39F30/40F90/41820 per historical T-11010.

## Result

Zero accepted functions / zero verified bytes. Production C and headers unchanged. Detailed score ledger and local reproduction locations: [[name-ent-family-results]]. Awaiting review of documentation and negative evidence; no matching claim or default-binary regression claim.
