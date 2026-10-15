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

- Character sibling func_8013B568 adds 2320 verified bytes (cumulative 4468). Both direct and pointer bitfield forms score zero; direct canonical form retained. The full 32-bit view recovers b0..b3, b6..b8, f9, b12/b13, f20 and f24; direct case stores remove m2c's artificial cross-case goto. Full-TU default literal proof and OPTION SHA1 pass: build/option14400/b568-proof.log, b568-build.log.
- Fourth settings sibling 8013A7EC: initial bitfield/pointer draft remains distant (282/209 rows); not enabled. Inspect structural/induction differences before further transfer.

- Bounded grid-family test: 801378DC and 80138080 each 232 full-TU rows after recovering four s16[16] arrays and sprite fields. Wide state/induction declarations worsen to 332, and meaningful local declaration-order changes do not improve this. Original assembly retained. Scratch: grid-family.py, grid-types.py, grid-refine.py, grid-locals.py under build/option14400. No added padding or speculative source kept.
- 8013A7EC negative refinement: partial pointer only for bitfield views plus signed loop locals and unsigned byte selector gives 76 rows; a weekday snapshot gives 77. Full pointer and narrow saved-month forms worsen to 209/224+ rows. Remaining save/frame and loop branch gap is not solved. Scratch scripts a7ec-refine.py, a7ec-structural.py, a7ec-weekday.py. No source promoted.

- Audio row family: func_80138C20 (472), func_80138FC0 (472), func_80138DF8 (456), total 1400 / cumulative 5868 bytes. Sixteen-byte row arrays, a real table pointer, shared loop index and ordinary for-loop structure recover the original allocation and scheduling. All three have full-TU default literal proof and OPTION SHA1: audio-trio-proof.log / audio-trio-build.log. No new FAKE workaround. A discarded remove-else-zero probe left an index uninitialized; it is not valid evidence for matching and was never promoted. The accepted third sibling initializes each loop explicitly.
- Audio control func_80139A30: canonical draft 292 rows; full GameState pointer 337. No promotion; temporary control prototypes removed. Bitfield sources and original assembly preserved for all rejected candidates.
