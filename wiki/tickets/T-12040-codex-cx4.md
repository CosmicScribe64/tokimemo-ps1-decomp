---
id: T-12040
title: Codex cx4 RPG battle matching
status: In Progress
assignee: Codex cx1
created: 2026-10-15
updated: 2026-10-15
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Deep matching pass over the eleven functions in ../codex/cx4-list.txt, owning only RPG_BAT objects 801340F0, 8015A1F0, 8014DF60 and 801321D0. Branch cx4 starts at w1-good.

## Acceptance criteria

- [ ] Investigate assigned functions and document guarded drafts and blockers.
- [ ] Every promoted function passes linked funcdiff and overlay checksum verification.
- [ ] Final configure/build verifies 27 binaries, headers, globals and branch prototypes.
- [ ] Submit for standards review.

## Notes

Docker only, ninja -j2 while workers share resources. Traced uopt copied by orchestrator to build/scratch/r9-uopt/variants/trace. No new dependencies or toolchain installation. Baseline log: build/scratch/baseline.log.

## Comments

Independent review required before Done.

Baseline completed: Docker configure/ninja -j2 passes all 27 binary checksums, headers and globals (build/scratch/baseline.log).

### func_80132EE0 matched (256 bytes)

The saved m2c draft retained an initial counter local across calls, creating a stack spill and stale-value behavior. Replaced those uses with the current shared D_8015EDB0.unk_04 field, matching the neighboring movement helpers. Linked funcdiff MATCH256 and RPG_BAT SHA1/globals OK. No FAKE. Evidence build/scratch/32EE0-match.diff and 32EE0-build.log.

Initial full-file measurements: 5A74417,4E30010,3415C22,3457022,5A8B427,32EE041; other saved drafts require missing declarations or current aggregate members before scoring. Literal diff rows supersede older ticket counts.

### func_8014E300 matched (692 bytes)

The first position/displacement store pair needed an aggregate view of the shared EC28..EC57 work range. A local RpgMotion30 pointer makes both loads and stores use that range, preventing as1 from hoisting the second coordinate load ahead of the first displacement store. Source-only or destination-only views and line joining leave ten rows. Plain C matches692 linked bytes; full header rebuild27/27, headers/globals OK. No FAKE. Evidence build/scratch/third-variants.log, 4E300-match.diff and 4E300-build.log.
