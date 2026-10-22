---
id: T-25034
title: MAIN src/main/80046500.c full-width tile extent recovery
status: In Progress
assignee: Codex
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-25032-main-candidate-audit]]"]
---

## Goal

Match func_80046AC8 using proven full-width tile extent tests, narrowing only for TILE stores. Exclusive file src/main/80046500.c. Preserve newest baseline and corrected semantic source regardless score. Inspect exact first divergence after whole-TU evidence; bounded45minute investigation, no broad grids/shared-header/build-rule/Astra changes.

## Acceptance criteria

- [ ] Four extent arithmetic/test paths preserve target signedness and full-word branch values.
- [ ] Whole-TU baseline and corrected candidate measured; default C literal proof required for promotion.
- [ ] Root review and integration gates before Done.

## Notes

T-25032 pin62948769f proves retained s16 extent defect in newest T-24501 source. One worker, isolated tree, root owns integration. No root checkout edit or push.

## Active reconstruction

Whole-TU baseline reproduced target674/built680words, aligned738/printed796. Changing only extent s16->s32 yields671words,613/649; all four tile branches now use full signed words without halfword narrowing. Cases0/1 compute offset minus signed radius plus120/160; cases2/3 signed negated offset minus radius plus120/160, store low16 only. Corrected baseline retained privately regardless score.

Continuing sustained matching at root direction, not stopping after the repair. Target-guided channel/callback lifetime probes expose mask elimination versus signed conversions; primitive-pointer role reuse reduces frameB8->B0. No match or production/shared-header change. Detailed controls remain in build/T-25034 pending stable checkpoint.

## Astra hands-on continuation

Human explicitly requests Astra and sustained resolution. Prior time/probe stopping limits are superseded: continue this exclusively owned function until literal semantic matching implementation and checks, or a concrete external blocker actually prevents further work. Ordinary matching difficulty and an unresolved packet are not resolution. One Astra worker now owns source experiments; Sol coordinator stops writing source. No extra workers, shared-header/build-rule changes or root integration.

Latest corrected private source671words, aligned367/printed389 (target674), frameB0 versusA8. Shared primitive, color/UV and reverse-radius/extent roles plus direct low-channel expression improve613->405->367. Full-width extent repair remains. Preserved current source at drafts/T-25034/main/func_80046AC8/body.c; detailed handoff/controls privately in build/T-25034/handoff.md. Matching continues rather than closing on this checkpoint.

### Latest frame correction

Astra verified saved direct-low-channel built output: frame is already0xA8, not the earlier0xB0 recorded in the handoff. Incoming argument homesB8/BC/C0/C4 match target. Shared extent role plus removal of the separate low-channel local closes frame growth. Local spill homes and color-expression ranges remain; prior logs are preserved rather than rewritten. Full-word extent semantics remain intact.

## User-directed wrap-up

User requested wrap-up, commit and push on 2026-10-22, superseding sustained matching. Astra stopped all experiments. Conservative candidate674target/671built padded words,361aligned/381printed; forward-join alternative674/670,360/380. Both retain full-word extents and target0xA8 frame. Sources, findings and Docker replay are archived under drafts/T-25034. Ticket remains In Progress: no match, promotion or coverage credit. Root owns main integration.
