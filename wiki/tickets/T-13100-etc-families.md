---
id: T-13100
title: Transfer source-family evidence to ETC
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[bunka-family-results]]", "[[matching-breakthroughs]]"]
---

## Goal
Increase verified ETC bytes using bounded source-family reconstruction from aa2b3158.

## Acceptance criteria
- Preserve all existing C; production ownership limited to ETC sources/header.
- Full-TU exact scoring, default literal linked proof, affected overlay SHA1.
- Review CODING_STANDARDS before Done; retain failures only in ignored scratch.

## Notes
Original checkout is read only. One compiler initially, existing Docker image only.

## Comments
2026-10-15: Created isolated codex/etc-13100 worktree; read Bunka transfer evidence. Test consistent GameState pointer and real bitfield accesses, then held-out siblings; do not repeat broad default sweep.

2026-10-15 checkpoint: 8014ABD0.c adds func_8014B1B4 (1436), func_8014BC00 (480), func_8014C06C (484): 2400 literal linked bytes, default build, ETC SHA1 OK. Proof: build/etc13100/family1-proof.log and family1-build.log. First direct score 4 -> consistent GameState pointer 0; both held-out siblings direct 0. Pointer-table declarations model lw with scaled indices. No existing C changed.

Career checkpoint [[tickets/T-13101-etc-career-family]] adds 1132 bytes; cumulative seven functions / 3532 literal linked bytes, ETC SHA1 OK.

2026-10-15: Combined checkpoint: eleven functions / 5900 literal linked bytes; final default proofs and ETC SHA1 pass. All 91 existing C bodies unchanged; headers/globals pass. Details in [[etc-family-results]]. Awaiting standards/spec review.

2026-10-15 review: repository code-review skill applied to aa2b3158...aa1c51ec. GPT-6.1 Sol medium reviewers: standards axis no findings; spec axis no actionable findings. Documentation observation about missing prototype result resolved here: sync_protos.py --check-branch passed, **0 new / 15 known disagreements**, branch check OK. Header/global guards pass; all eleven combined default literal proofs and ETC SHA1 pass. Existing-body preservation audit: 91/91 unchanged. No production edits after reviewed commit. Ticket complete within its bounded ETC scope; broad game-wide 80% goal remains ongoing.
