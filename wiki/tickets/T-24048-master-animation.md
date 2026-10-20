---
id: T-24048
title: Reconstruct MASTER animation state machine
status: Done
assignee: codex/complete-option
created: 2026-10-18
updated: 2026-10-20
links: ["[[master-family-results]]", "[[completion-run-2026-10-17]]"]
---

## Goal

Reconstruct func_801344E0 in src/ovl/MASTER/80133680.c with a bounded semantic, aggregate and control-flow pass. Other MASTER translation units remain reserved.

## Acceptance criteria

- [x] Read previous target evidence and recover a current-header source foundation.
- [ ] Default actual C literal proof and MASTER checksum for any promoted match.
- [x] Preserve useful source and reproducer, document failures without claiming coverage.
- [x] Independent review against CODING_STANDARDS.md before Done.

## Notes

Prior work: T-9960 recovered sprites/coordinate pairs and a 420-row form; T-10630 reported a 350-row source-only handoff, unavailable in this checkout's scratchpad; T-18600 retained a 475-row typed/rerolled body. Old copy-state observations in matching-notes are hypotheses, not an impossibility proof. Reuse the built worker tree at 2c7dbab72, one Docker compiler, no header or config changes without coordination.

## Comments

2026-10-18: claimed this TU only after reading current main completion checkpoint f55a2d7e9 (81.1%). No new matching claim.

### Bounded result

Zero matches/bytes; production TU remains untouched. Current shared types reproduce475/469. Seventeen measured forms (including final readability verification) establish434/428 selected source; original copy end-register state, hoisted clone bound/spill homes,72-versus64 frame and byte-counter v0/v1 promotion remain. Selected source uses actual shared aggregates and pointer fields; no FAKE workaround retained. Rejected alias probes are FAKE marked and preserved only in ignored build/T-24048-probes. See drafts/T-24048/README.md for reproduction and all bounded negatives. Independent standards review remains pending; In Review.

Validation: baseline `ninja -j1 build/ovl/MASTER.ok` passes, MASTER SHA1 `7303c196a7a2fd8423da1e66e5a49511e9805bbc`. check_headers and migrate_globals --check pass; sync_protos --check-branch reports0 new/15 known and passes. Saved-source reproducer freshly compiles434/428. No default literal C proof is claimed because the target remains ASM. No production C/header/config diff; git diff --check passes.

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
