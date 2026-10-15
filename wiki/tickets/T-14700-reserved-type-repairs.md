---
id: T-14700
title: Repair fifteen reserved aggregate and callback drafts
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[matching-notes]]", "[[data-types]]"]
---

## Goal
Recover coherent compiling C drafts for the fifteen reserved T-14500 failures. Production C and original attempts remain unchanged. Worktree starts at 48362009.

## Acceptance criteria
- All fifteen have complete alternative body, declarations, TU, provenance and default full-TU score or exact blocker.
- No production promotion without literal-byte proof and affected binary checksum.
- Review against CODING_STANDARDS.md before Done.

## Notes
Inputs are hard linked; generated ASM is private. Compiler and Python run serially in Docker.

## Comments

## Results

All fifteen reserved failures now compile in the default IDO 5.3 full-TU path. Zero zero-score candidates. Scores and full provenance are in drafts/T-14500/repairs/types/catalog.json. No production C or header changed; no promotion or literal-byte match claim. Immutable inputs were hard-linked on the host because separate Docker bind mounts reject cross-device links; all analysis/compiler Python ran through tools/docker.sh. Generated ASM was private.

Recovered byte tables (6/21/54/66/78/297 bytes), signed halfword tables (13/64 entries), eleven-word and two-word lookups, callback tables (including four corrected tail lengths), GsWord packed accesses, and the ETC contiguous 14-element halfword/word arrays. Complete pre-repair compiler diagnostics preserved, including previously truncated BUNKAKEN errors.

Residuals: every candidate has nonzero codegen differences. Original default-arm uninitialized byte reads and partially initialized packed bits are explicitly documented. ETC was reconstructed against target operations and requires runtime validation. Production integration must reconcile provisional draft declarations with shared header rules and perform default literal proof plus affected SHA1.

## Validation

- Serial full-TU compilation and default resolved comparison succeeded for 15/15.
- Frozen production src/ and include/ remain unchanged.
- Two direct-expression table drafts score 2; natural used-index alternatives scored 14 and were archived, not selected.
- Independent standards/spec code review completed; both findings resolved in ce44a7e0. Both reviewers rechecked the correction with no further findings.
- Unknown-target failure exit verified; artifact SHA256 and all baseline source snapshots audited.
- Done applies to this draft-only archive scope; matching and production integration remain future work.
