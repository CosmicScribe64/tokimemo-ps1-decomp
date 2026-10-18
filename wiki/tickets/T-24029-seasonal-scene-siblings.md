---
id: T-24029
title: Recover GYOZI and SHOUGATU scene sibling pair
status: In Review
assignee: complete_render_family
created: 2026-10-17
updated: 2026-10-17
links: ['[[shougatu-family-results]]']
---

## Goal

Recover GYOZI func_801368B8 (2256 B) and SHOUGATU func_801350F8 (2308 B) from shared structure in a bounded twenty-five minute pass.

## Acceptance criteria

- Unguarded C proof and full affected checksum for any claimed win.
- Preserve useful source-only negative evidence and retain matching default fallback otherwise.
- Independent standards review before Done.

## Notes

Exclusive sources src/ovl/GYOZI/80136220.c and src/ovl/SHOUGATU/80134890.c. Shared-header edits require prior notice to root. Prior T-15100 reports broad unaligned drafts (701/995 differing rows); T-17500 contains a typed SHOUGATU foundation.

## Results

No new C matches. Production sources restored unchanged; source-only typed checkpoints in drafts/T-24029. Current full-file aligned edit costs are GYOZI 586 and SHOUGATU 656. Older 701/995 row measurements use older contexts and are not a direct comparison. GYOZI is the stronger foundation. See the archive README for specific repairs, exhausted variants and next hypotheses.

The novel work used existing fixed-point word and actor views, simplified artificial condition flags and byte-counter temporaries, repaired the table member, and tested the early initialized selector local. The remaining blockers are broad copy scheduling, base and live-range differences rather than one compiler instruction. No shared header was expanded. Scratch-only declarations remain confined to the source archive. Self-review: source-only C/markdown archive, no assembly listing/raw assets/toolchain flags; independent review pending.

Restored production build verification: GYOZI SHA1 `43f92523116e7b6cd772d4397712a52eeda89703` and SHOUGATU SHA1 `d956b1df6fbf9d1f8261d7e1f98ea301f7eb66a9` both pass. Production source diff is empty; `git diff --check` passes.
