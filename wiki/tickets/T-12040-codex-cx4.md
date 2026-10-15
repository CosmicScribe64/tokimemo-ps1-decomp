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
