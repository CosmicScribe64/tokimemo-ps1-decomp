---
id: T-25011
title: "Class A closers: KANGEI x2, VALEN func_80132D0C, GEKO func_80136C20, TACO func_801511C0/func_8013B460, GYOZI func_80140E0C/func_8013907C"
status: In Progress
assignee: sonnet-5.5
created: 2026-10-21
updated: 2026-10-21
links: [[[plan-remaining]], [[as1-scheduler-rule]], [[playbook]], [[data/remaining-2026-10-20]]]
---

## Goal

Close the eight class A near misses (10 rows or fewer) of [[plan-remaining]], one function at a time, using [[as1-scheduler-rule]] (block weights, visit order, the `volatile` lever on the global whose `lui` must stay in its block) and `tools/order_search.py` / `tools/autotweak.py`. No per-function compiler options and no build-step changes (CODING_STANDARDS 7a). A match counts only with funcdiff MATCH and a clean 27/27 build.

## Notes

Single Sonnet 5.5 subagent owns all of `src/` and `include/`; the orchestrator owns the wiki, review, CI and pushes.
