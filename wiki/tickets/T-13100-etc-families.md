---
id: T-13100
title: Transfer source-family evidence to ETC
status: In Progress
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
