---
id: T-9600
title: Wave-8 tool fixes
status: Done
assignee: r8-fixes
created: 2026-10-11
updated: 2026-10-11
links: ["[[matching-notes]]", "[[register-allocation-wave7]]", "[[toolchain]]"]
---

## Goal

Fix the seven wave-8 tooling reports from the shared tooling8 brief; preserve every matched function.

## Acceptance criteria

- [x] Function-filtered inventory skips absent assembly with a note.
- [x] Permuter handles K&R parameters and implicit callees.
- [x] Funcdiff resolves PC16, paired string HI16, and short absolute symbols.
- [x] Frame pass supports the negative-SP shape with a uniform tested rule.
- [x] Diagnose duplicate m2c stores; fix wrapper or document workaround.
- [x] Duplicate copiers use the migration GameState mapping.
- [x] Header tools reject unterminated conditionals and interleaved typedefs.
- [x] Regression tests, clean 27-unit build, guards, progress and inline standards/spec review pass.

## Notes

All tooling runs through tools/docker.sh. No source/header edits or commits planned. Scratch belongs to the shared scratchpad/r8-fixes directory.

## Comments

2026-10-11: Read shared brief, project instructions, standards and wave-8 reports. Claimed ticket; implementation starting.


2026-10-11: First complete Docker suite passed 584 tests; clean configure/ninja passed all 27 SHA1 checks and both stamps. Progress remains 4868/6958, 776304/2279368 bytes. Branch check: zero new disagreements, 17 known. Full register inventory runs (1189 rows, 737 functions). A read-only replay of the reported GYOZI-to-SHOUGATU clone now plans GameState.unk_110A correctly.

2026-10-11 inline standards/spec review: verified all seven reports and CODING_STANDARDS sections 2, 7/7a, 8a, 10, 11 and 13. Found and resolved a safety edge: a plain move of sp must not acquire permission for negative indexed displacements. Frame tracking now distinguishes variable-index bases from plain sp copies, with regressions. Explicit interior views conflicting with a known source declaration are skipped rather than silently changing access widths. Added a 102-seed synthetic K&R mutation regression (including both reported delete seeds) and a worker entry-point smoke test. Final verification of these review refinements pending.


2026-10-11 final verification/review: Done. Docker unittest discovery passed 587 tests in 40.653 seconds, including every report's regression, 102 K&R mutation seeds, real IDO biased-index compilation and both copier paths. Final clean rm -rf asm build / configure.py / ninja passed 27/27 SHA1 checks (no -k); build/headers.ok and build/globals.ok exist. Fresh sync_protos --check-branch passes with zero new disagreements (17 known). Fresh TACO func_801361E8/func_80136438/func_80136688 and BUNKA_SD func_80132DFC resolved diffs all MATCH; RPG_BAT PC16 functions and strNextVlc also passed with the new resolver. Progress is unchanged at 4868/6958 functions, 776304/2279368 bytes. Functions gained: zero. Previously matched linked bytes remain identical.

Inline final standards/spec review against the HEAD working diff: no unresolved findings. No function-specific compiler rules, source/header/config changes, raw assets or commits. Ticket/board/index consistency and git diff --check pass. Changed eight tool scripts (register_shapes, permute, funcdiff, frame_pass, dupes, check_headers, sync_protos, m2c), eight test files, and ticket/wiki documentation. The m2c table-store issue is confirmed upstream; the explicit element workaround is documented and no speculative draft rewrite is installed. All changes remain uncommitted for the orchestrator. Logs: shared scratchpad/r8-fixes/tests-reviewed.log, clean-build-reviewed.log, progress-reviewed.log and inventory.log.
