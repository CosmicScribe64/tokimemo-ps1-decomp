---
id: T-24027
title: RPG_BAT state-machine completion
status: In Progress
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: ["[[rpg-bat-family-results]]"]
---

## Goal

Bounded current-type reconstruction of func_80154DD8, func_80154760 and func_8015528C in src/ovl/RPG_BAT/80154430.c. Other files remain unchanged.

## Acceptance criteria

- [ ] Default literal linked C proof without target INCLUDE_ASM and affected RPG_BAT checksum for every match.
- [ ] Preserve useful source drafts and provenance for unresolved functions.
- [ ] Header/global/prototype checks and independent review.

## Notes

Read wiki index, RPG_BAT family, T-17100, T-17106 and related matched sibling recipes. Earlier evidence: inner selector scalar-address view may fix v0/v1, direct aggregate updates affect allocation; 54760 has unresolved post-loop index/address shape, 54DD8 quotient webs. Avoid repeating exhausted brute force. Docker only, one compiler/ninja-j1, bounded25min.

## Verified batch 1

func_80154760 matched1656 default linked bytes. Natural post-loop flag stores (record136 separately, for i=1..5) reproduce the original peeled loop and li s3,2; directly writing selected index D_8015EC70 fixes remaining index v0/v1 allocation. Existing implicit-int func_8013E97C override is required (without it85 rows); extra F0F4 override is unnecessary. Two FAKE marks retain known scalar selector-address view (T-17114) and unsigned palette constant (T-17106). Only header addition is the recovered function's s32 prototype. Actual target INCLUDE_ASM removed; default --resolve --version default --no-build proves1656 linked bytes; RPG_BAT checksum passes, SHA1 142d13e317aa7ae6def6a300085c38f645c19db6.

Batch1 guard checks: headers OK, globals OK, prototype branch check0 new/15 known disagreements. Independent review pending.
