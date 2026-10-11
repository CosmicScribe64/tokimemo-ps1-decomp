---
id: T-9610
title: Aggregate and local-copy types
status: Done
assignee: r8-types
created: 2026-10-11
updated: 2026-10-11
links: ["[[data-types]]", "[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal
Recover recurring aggregate layouts from wave-8 rejected drafts, supply shared SDK and overlay types, measure IDO local frame rules, and retain verified matches.

## Acceptance criteria

- [x] Inventory recurring local-copy, by-value, stride and packet shapes.
- [x] Add evidence-backed shared/overlay layouts with sizes and offsets.
- [x] Document measured local aggregate frame rules and remaining gaps.
- [x] Verify retained functions and clean 27-unit build, header/global/prototype guards and progress.
- [x] Inline standards/spec review, with no unresolved findings.

## Notes
Shared brief: scratchpad/tooling8-brief.md. No commits or branch changes; orchestrator owns integration. Existing tools/*.py belong to r8-fixes; avoid overlapping edits.

## Comments

2026-10-11: Claimed; read shared brief, standards, toolchain and relevant workflow/type notes. Baseline worktree is clean.

2026-10-11: In Review. Shared SDK layouts and two callback matches retained; four packet/matrix users refactored without intended byte changes. Failed bodies restored. ABI and frame tests pass. Inline standards/spec review against HEAD is underway; clean build and guards next. See matching-notes T-9610 for all rejected experiments.

2026-10-11: Done. Inline review against HEAD `1e24c0394a695c64200126de3beff00e579f930a`, as required by the shared brief, completed on both axes:

- Standards: C89, fixed-width layouts, shared declarations, original byte preservation, no new FAKE or compiler special case. Corrected DR_TPAGE's reference-manual discrepancy to eight bytes and removed an unused test import. No unresolved findings.
- Spec: all eight lists inventoried, shared and overlay layouts recovered, IDO local/frame rules measured and documented. Existing type_recovery.py already collects symbol users, so no duplicate recovery path was added. Two new functions retained; unresolved candidates and EN_NICHI's residual frame gap are documented, not claimed as matches. No unresolved findings.

Final Docker proof: clean configure + ninja produced **27 `: OK` lines**; `build/headers.ok` and `build/globals.ok` exist. Fresh resolved diffs pass DATE func_80153CEC, GEKO func_80141760, EN_NICHI func_80136700, and TACO func_8013F874/func_80153E78/func_8015B3B0. New ABI test (1), local-frame tests (2), and existing frame-pass tests (20) pass. `sync_protos.py --check-branch`: zero new disagreements, 17 known. Progress **4870/6958 functions, 776660/2279368 bytes**, gain **2 functions / 356 bytes**. `git diff --check` passes.

Integration: all edits remain uncommitted. Existing Python tools and compiler passes are untouched; only two new test scripts were added under tools/. Full proof logs and rejected experiments are in scratchpad/r8-types (clean-build-final.log, final-checks.log). Follow-up candidates are in [[matching-notes]], with SHUGAKU's cross-function regression called out explicitly.
