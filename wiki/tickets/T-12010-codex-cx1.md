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

### func_80042134 matched (404 bytes)

Migrated the saved main-loop draft to GameState fields and used a local GameState pointer only for the loop-tail selector load. This reproduces the original s2 base saved at entry. Declared later helpers func_800422C8 and func_80042418 once in main_api.h to prevent implicit-return-type conflicts. Plain C linked funcdiff MATCH (404 bytes), main SHA1 OK; all-overlay rebuild in progress due to header update. No FAKE. Evidence build/scratch/42134-match.diff and 42134-build.log.

### func_80049044 matched (112 bytes)

A five-minute permuter run improved its stripped score from 90 to 50 using a u8 pointer to the brightness field. Full-unit variants show the simple initialized pointer (with both loads and stores through it) is actually exact. The original six-word ugen mismatch disappears. Plain C linked funcdiff MATCH (112 bytes), main SHA1 and globals OK; headers unchanged since the preceding 27/27 build. No FAKE: the pointer is an ordinary field view, with no alias offset or dead statement. Evidence build/scratch/49044-pointer.log and 49044-match.diff.
