---
id: T-12901
title: Verify close OPTION and audio drafts
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Maximize verified code bytes with bounded attempts on src/ovl/OPTION/*.c and src/main/80079B10.c. Based on origin/main 95c3fc9f in codex/small-option-main.

## Acceptance criteria

- Literal linked-byte funcdiff proof for every enabled C function.
- Affected binary SHA1 matches; normal default build remains exact.
- No unowned source or generated asm changes; review against CODING_STANDARDS.md before Done.

## Notes

Queued cx6/cx8 lists are leads only. Reuse full translation units for scoring. Docker only, one compiler at a time. Parent T-12900; first bounded pass T-12901.

## Comments

- 2026-10-15: Claimed the previously unstarted OPTION and main audio files. No matches claimed yet.

- OPTION func_80137E38: funcdiff --resolve proves linked bytes equal target (360 bytes). Word-array indexing restores ugen temporary numbering.
- Main func_8007A868: eight bounded full-unit variants; aggregate accesses plus final unk_08 store yield zero diff rows. Literal and whole-unit verification pending.

- Verified funcdiff --resolve: OPTION func_80137E38 360 bytes; main func_8007A868 188 bytes. Both compare default C build, with literal linked-byte proof. Total 548 bytes.
- Docker ninja -j1 build/ovl/OPTION.ok build/SLPM_86.053.ok: both SHA1 checks OK. No header changes. Awaiting standards/spec review.

## Final verification and review (2026-10-15)

- Docker full default build: 27/27 binary SHA1 checks pass; final source-comment rebuild passes main/OPTION checks again.
- Final funcdiff --resolve proves default C linked bytes: OPTION func_80137E38 360, main func_8007A868 188, main func_80079F00 264; total 812 bytes.
- Main SHA1: e823bd844a8f8fa4d05483b59c66bc54b8393b26. OPTION SHA1: 3fd443d1e32404e5febd57d0586ef1c70ce8a811.
- sync_protos --check-branch: branch check OK, 0 new disagreements, 15 known.
- Separate code-review skill standards/spec reviewers found no correctness, scope, or FAKE issue. Standards requests for source-local matching comments and one-source-file commits are resolved and re-reviewed. No outstanding findings.
- Proofs retained locally under build/t12900/experiments/: all27.txt, final-build.txt, final-proof.txt, protos.txt. Scratch is ignored; no game inputs or generated output committed.
