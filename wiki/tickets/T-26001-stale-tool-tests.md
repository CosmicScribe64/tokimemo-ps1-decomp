---
id: T-26001
title: Fix the stale test_build_deps and test_protoverride fixtures
status: Done
assignee: orchestrator
created: 2026-10-24
updated: 2026-10-24
links: [[[tooling]], [[tickets/T-25608-gate-main-func_8006B0C8]]]
---

## Goal

The full tool suite (`python3 -m unittest discover -s tools -p 'test_*.py'`) had 11 failures and 1 error on main, found when the T-25608 merge check ran it. Both were test fixtures that fell behind later tool changes; the build itself was 27/27.

## Acceptance criteria

- [x] `tools/test_build_deps.py`: the fake project copies `tools/ugen_pass.py`, which every compiled object depends on since the block-copy pass was enabled (T-24092); all 11 `NinjaDependencies` tests failed with "ugen_pass.py ... missing and no known rule to make it".
- [x] `tools/test_protoverride.py`: the fake context of the compile_unit test gets the `name` that `funcdiff_verify.compile_unit` now reads for `cc.toolchain_of`.
- [x] Full suite: 1572 tests OK (3 skipped).

## Comments

- 2026-10-24 (orchestrator): fixed both fixtures; test-only change, no tool or build behaviour changed. Merge check runs the full suite.
