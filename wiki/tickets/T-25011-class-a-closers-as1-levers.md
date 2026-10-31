---
id: T-25011
title: "Class A closers: KANGEI x2, VALEN func_80132D0C, GEKO func_80136C20, TACO func_801511C0/func_8013B460, GYOZI func_80140E0C/func_8013907C"
status: Done
assignee: sonnet-5.5
created: 2026-10-21
updated: 2026-10-26
links: `[plan-remaining`, [[as1-scheduler-rule]], [[playbook]], [[data/remaining-2026-10-20]]]
---

## Goal

Close the eight class A near misses (10 rows or fewer) of `plan-remaining`, one function at a time, using [[as1-scheduler-rule]] (block weights, visit order, the `volatile` lever on the global whose `lui` must stay in its block) and `tools/order_search.py` / `tools/autotweak.py`. No per-function compiler options and no build-step changes (CODING_STANDARDS 7a). A match counts only with funcdiff MATCH and a clean 27/27 build.

## Notes

Single Sonnet 5.5 subagent owns all of `src/` and `include/`; the orchestrator owns the wiki, review, CI and pushes.

## Result (2026-10-21)

Zero matches. `autotweak` on all class A guards: KANGEI func_8013852C 2 rows (280 variants), GEKO func_80136C20 4 (1766), TACO func_8013B460 4 (1940), TACO func_801511C0 2 (298), GYOZI func_80140E0C 5 (340), GYOZI func_8013907C 23 (644), GEKO func_80133024 4 (323): no change. KANGEI func_80134724: default arm rewritten as `(u16)v * 4 + s + 3` (FAKE), which fixes the as1 delay-slot pick but leaves one register chain (default-arm temps t1..t3 vs t0..t2), 4 rows; the old 2-row figure was not reproducible. VALEN func_80132D0C and main func_8005B43C have no guarded draft in `src/`. Counting rows: use `funcdiff ... | grep -E "MATCH|^  [-+]"`, not `@row`. Build clean: no FAILED, 27/27.

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
