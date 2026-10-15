---
id: T-12010
title: Codex cx1 main helper matching
status: In Progress
assignee: Codex cx1
created: 2026-10-15
updated: 2026-10-15
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Deep matching pass over cx1 twelve functions in main objects 800420D0, 800451D0, 80047550 and 80059710. Work only in the cx1 worktree.

## Acceptance criteria

- [ ] Investigate assigned functions and document guarded drafts and blockers.
- [ ] Promoted C passes linked funcdiff and binary checksum verification.
- [ ] Final configure/build verifies 27 binaries; headers, globals and branch prototypes pass.
- [ ] Submit for review against CODING_STANDARDS.md.

## Notes

Package ../codex/cx1-list.txt. Baseline Docker configure/build passed 27/27 SHA1; log /tmp/cx1-baseline.log. Three workers share Docker, subsequent ninja uses -j2. InitMouse has a documented missing data-ownership range; func_800422C8 has a documented call-result register mismatch.

## Comments

Review pending.

### InitMouse matched (16 bytes)

The original two stores share one lui $at. All code references to the pointer pair are in object 80059710; its remaining mouse state occupies 800E36C8..800E36EA, with zero padding through 800E36F0. Added an owned-data range 800E36C0..800E36F0 and generated its island with tools/data_island.py. Defined only the two zero-initialized buffer pointers; other data remains INCLUDE_RODATA. Linked funcdiff proves 16 bytes, configure/ninja -j2 passes 27/27, headers and globals OK. Evidence: build/scratch/InitMouse-match.diff and build/scratch/mouse-island-build.log. No FAKE.

### Initial near-miss experiments

file_checksum_ok: 4 listing lines, sltu/andi scheduling swap. Autotweak (385 variants), all 720 loop statement permutations and 42 manual source-line/mask variants unchanged. func_80049044: 12 listing lines (six word substitutions), ugen loads into t6/t7 instead of v1; 255 autotweak candidates unchanged. Traced uopt reused from the orchestrator-provided existing binary at build/scratch/r9-uopt/variants/trace; regdiff confirms ugen expression temporary rather than uopt allocation.
