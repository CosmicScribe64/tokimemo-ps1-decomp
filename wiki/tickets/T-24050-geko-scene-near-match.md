---
id: T-24050
title: Close GEKO scene setup near match
status: In Review
assignee: complete_render_family
created: 2026-10-18
updated: 2026-10-18
links: ['[[geko-family-results]]']
---

## Goal

Recover func_80134808 (432 B) in src/ovl/GEKO/80134040.c using concrete aggregate/lifetime evidence and nearest matched siblings. Reuse existing tree and one Docker compiler; no unrelated files.

## Acceptance criteria

- Any win is unguarded C, normal default literal proof and GEKO checksum verified.
- Preserve corrected source and a reproducer if unresolved; independent review before Done.

## Notes

Read T-9510 historical mismatch and current GEKO family/typed draft evidence before probing. GEKO80142760 and GEKO8013FA60 are separate ownership scopes. Header changes require coordination.

## Results

No new C match. Production source/headers are unchanged. Source-only drafts/T-24050 contains the 80-edit typed draft and a tested Docker scratch-only reproducer. Current census has no matched relative at similarity0.70 or above.

Correct word CharFlags.b1 test and natural control flow score103; independent record-array base spelling scores80. The latter source spelling is marked FAKE because it prevents the compiler from promoting the shared GameState base. Original duplicate tie guard remains, with its unknown rationale documented. Explicit next-index/join, historical-flow word access and combined current-type joins score109/102/132/88. Best-state byte-width views leave80/88. Stop rather than repeat exhausted forms.

Concrete blocker: target next-index and best-index lifetimes and common-update CFG still allocate saved registers differently. No type/header expansion is justified. Self-review: source/markdown/tested reproducer only; no assembly listing, raw game data or compiler exceptions. Independent review pending.

Final preservation: GEKO SHA1 fa37853a9bd8df628c6313b0a4af8ec7568482cd passes; headers/globals/main_api and whitespace checks pass. Tested reproducer returns aligned cost80 without modifying production source.
