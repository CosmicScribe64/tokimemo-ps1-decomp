---
id: T-12903
title: Match small OPTION audio handlers
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-12900-small-option-main]]"]
---

## Goal

Bounded work on the 132/200-byte guarded handlers in src/ovl/OPTION/801389A0.c, discovered in the refreshed queue.

## Acceptance criteria

- Literal linked-byte proof and OPTION SHA1 for enabled C.
- Review CODING_STANDARDS.md before Done.

## Comments

- 2026-10-15: Queue reveals two small candidates outside the old cx6 file list but within the assigned OPTION ownership. T-12902 saved drafts did not immediately match after canonicalization; park difficult register families.

## Final verification and review (2026-10-15)

- Docker full default build: 27/27 binary SHA1 checks pass; final source-comment rebuild passes main/OPTION checks again.
- Final funcdiff --resolve proves default C linked bytes: OPTION func_80137E38 360, main func_8007A868 188, main func_80079F00 264; total 812 bytes.
- Main SHA1: e823bd844a8f8fa4d05483b59c66bc54b8393b26. OPTION SHA1: 3fd443d1e32404e5febd57d0586ef1c70ce8a811.
- sync_protos --check-branch: branch check OK, 0 new disagreements, 15 known.
- Separate code-review skill standards/spec reviewers found no correctness, scope, or FAKE issue. Standards requests for source-local matching comments and one-source-file commits are resolved and re-reviewed. No outstanding findings.
- Proofs retained locally under build/t12900/experiments/: all27.txt, final-build.txt, final-proof.txt, protos.txt. Scratch is ignored; no game inputs or generated output committed.

## Bounded negative result

No source changes or additional matches. The 132-byte func_80139F8C remains at 11 full-unit diff rows after 14 bounded variants (integer widths, cached flags, switch form). Its differences are initial word register choice and the final call-argument move. The 200-byte func_80139E3C draft was inspected but not changed. Existing guarded fallbacks remain intact.
