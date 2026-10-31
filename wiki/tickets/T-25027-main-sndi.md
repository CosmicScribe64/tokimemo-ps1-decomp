---
id: T-25027
title: MAIN src/main/8004F870.c sndi reconstruction
status: Done
assignee: Codex
created: 2026-10-22
updated: 2026-10-26
links: ["[[tickets/T-25022-main-reload-boundary]]", "`plan-remaining`"]
---

## Goal

Match sndi (1052 bytes) within the exclusively owned src/main/8004F870.c. Compare current and recovered drafts, read frozen negative ledger, establish whole-TU evidence, and repair only concrete semantic/type/control gaps. One worker, no Astra or shared-header changes. Root owns integration.

## Acceptance criteria

- [ ] Default C literal linked bytes match target, excluding assembly fallback.
- [ ] Root review and executable/declaration integration checks pass.

## Notes

Previous T-24501 sources are unmatched; recovered canonical127/raw274 metrics are not a whole-TU baseline. Do not repeat frozen cursor/induction grids. Preserve T-25022 consultation pin6fbe9a03e.

## Measured checkpoint

Current tracked baseline reproduced target263/built265words, aligned150/printed150; older branch-reset260/211/211. Recovered raw body's one-byte placeholders, extra call arguments and lost found join are already repaired in the tracked draft; privately corrected recovered body is260/315/315 with frame140, not a better starting point.

Concrete target control gap: G-scan first reads stage[0], whereas tracked while(stage[i]) retains an unwanted index-derived cursor IV. Explicit stage[0] guard plus do/while latch restores G indexed-loop structure:264words,123/123. Target-timed first-scan join-reset alternate264/125/127 preserved beside it. Both frame118/all five buffer bases correct. Word parity controls regress261/186/186 or188printed. Source and complete audit at drafts/T-25027/main/sndi/.

Fresh normal/traced objects resolve identically,264words. First register divergence remains converted base a0 versus a1; trace reproduces base7.333/constant6.667 and parity20/cursor11 priorities. Cursor home44 versus target30/34 and old-index copies remain. No new semantic/type gap justifies repeating frozen induction grids. No production/header/tool edits, extra worker or Astra. No match; ticket stays In Progress pending root review of checkpoint/blocker.

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
