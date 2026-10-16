---
id: T-12900
title: Small OPTION and main audio matching
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

## Final verification and review (2026-10-15)

- Docker full default build: 27/27 binary SHA1 checks pass; final source-comment rebuild passes main/OPTION checks again.
- Final funcdiff --resolve proves default C linked bytes: OPTION func_80137E38 360, main func_8007A868 188, main func_80079F00 264; total 812 bytes.
- Main SHA1: e823bd844a8f8fa4d05483b59c66bc54b8393b26. OPTION SHA1: 3fd443d1e32404e5febd57d0586ef1c70ce8a811.
- sync_protos --check-branch: branch check OK, 0 new disagreements, 15 known.
- Separate code-review skill standards/spec reviewers found no correctness, scope, or FAKE issue. Standards requests for source-local matching comments and one-source-file commits are resolved and re-reviewed. No outstanding findings.
- Proofs retained locally under build/t12900/experiments/: all27.txt, final-build.txt, final-proof.txt, protos.txt. Scratch is ignored; no game inputs or generated output committed.

## Handoff

Branch codex/small-option-main, base 95c3fc9f, worktree /Users/lila/Documents/Projects/tokimemo-wt/small-option-main. Original checkout was read-only.

| Function | Unit | Verified text bytes | Match commit |
| --- | --- | ---: | --- |
| func_80137E38 | OPTION | 360 | 7cb341f6 |
| func_8007A868 | main | 188 | f1744de9 |
| func_80079F00 | main | 264 | 41d7a47e |

Source-comment follow-ups: 5ea4c9e7 and d83ae1a4. Integrate the branch range from 95c3fc9f through the final documentation commit. Earlier unpublished 6aa2f091/cbe7698f were split and superseded; use the replacement history. No push or PR.
