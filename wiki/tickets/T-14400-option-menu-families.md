---
id: T-14400
title: Match OPTION menu and state families
status: Done
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

- Final checks: headers OK; globals OK; sync_protos --check-branch OK (0 new / 12 known disagreements). OPTION SHA1 3fd443d1e32404e5febd57d0586ef1c70ce8a811. All six fresh default linked proofs pass after formatting/comments, 5868 bytes total. Source-only commits: d63d1842, 90580c83, 24c028b4; field-comment follow-up ff37e3ab. No push or PR.
- Final natural calendar-loop check remains 78 rows (shared induction local 76), so the function is still assembly. The accepted gains span packed settings and text-table families; grid/control families are precise negative results, not coverage.
- Moved to In Review for standards/spec review against a46afeb8...HEAD. Existing C body preservation audit is in build/option14400/body-audit.log.

- Standards review found three first-member byte-offset accesses in BFF8 despite existing GameState fields (CODING_STANDARDS 8). Replaced with g->unk_725/726/727 and reran all literal proofs. Removed two redundant audio initialization statements after dedicated full-TU probes proved them unnecessary (both remain zero rows); no artificial initialization remains. Removed duplicate ticket card caused by preexisting repeated In Review headings. Awaiting re-review.

## Final review and handoff

Standards/spec code-review skill axes independently reviewed a46afeb8...916b4079, then re-reviewed e63fe3e4/150489c4. Original first-member-access finding resolved; both axes report no outstanding actionable findings. Reviewers read proof logs and source; they did not independently compile. Final headers/globals pass, prototype guard passes with zero new / twelve known disagreements. All six default literal proofs and OPTION SHA1 pass after the reviewed fixes; all 33 pre-existing C bodies in touched files are unchanged. Ticket Done covers this bounded OPTION family pass, not the game-wide 80% goal.

| Function | Literal linked bytes | Family |
| --- | ---: | --- |
| func_8013AF10 | 1196 | Player settings |
| func_8013B568 | 2320 | Character settings |
| func_8013BFF8 | 952 | Schedule settings |
| func_80138C20 | 472 | XA row menu |
| func_80138DF8 | 456 | Sequence row menu |
| func_80138FC0 | 472 | SE row menu |
| **Total** | **5868** | **6 functions** |

Branch codex/option-14400, base a46afeb8, worktree /Users/lila/Documents/Projects/tokimemo-wt/option-14400. Source batches d63d1842, 90580c83, 24c028b4; comments ff37e3ab; review fixes e63fe3e4 and 150489c4. Integrate the complete branch range so the review fixes are included. No push or PR. Original checkout source remains unchanged. No new FAKE workaround, guarded draft, compiler change, generated assembly edit, or shared main-header change. OPTION remainder from the assigned 29612-byte/44-function baseline is 23744 bytes/38 functions.

Final evidence in ignored build/option14400: review-proof.log, review-build.log, headers.log, globals.log, protos.log, body-audit.log. Precise negative probes remain there and are summarized above. No copyright game input/output is committed. Future work should recover the existing sprite-local state/lifetime forms before revisiting grid handlers; do not repeat the recorded bounded probes without a new hypothesis.
