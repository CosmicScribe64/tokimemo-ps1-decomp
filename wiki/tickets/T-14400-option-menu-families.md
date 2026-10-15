---
id: T-14400
title: Match OPTION menu and state families
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[matching-notes]]", "[[decompile-workflow]]"]
---

## Goal

Increase verified OPTION game-code bytes from origin/main a46afeb8 in isolated codex/option-14400. Own only OPTION source/header; preserve existing C and original checkout.

## Acceptance criteria

- Full-translation-unit default literal linked-byte proof for enabled C and affected OPTION SHA1.
- Header/global/prototype checks pass; review against CODING_STANDARDS.md.
- Docker only, one compiler at a time; no generated asm or game data committed.
- Precise negative ledger; no repeated documented probes without new evidence.

## Notes

Prior work: T-12900/12901/12902; Bunka, ETC and GEKO family findings. Focus on coherent medium/large state/table families and actual layout recovery.

## Comments

- 2026-10-15: Created isolated checkout from latest origin/main. Read wiki index, local instructions, standards and previous OPTION evidence before probing.

- First checkpoint: func_8013BFF8 (952 bytes) and func_8013AF10 (1196 bytes), total 2148. Both have full-TU default literal linked-byte proof, and OPTION SHA1 passes. Evidence: build/option14400/checkpoint-proof.log and checkpoint-build.log. No existing C function body changed. New bitfield views model observed word/halfword/byte loads and masked stores; consistent local GameState pointer closes aggregate address differences. No new FAKE idiom.
