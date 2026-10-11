---
id: T-9400
title: Wave-7 tool fixes
status: Done
assignee: r7-fixes
created: 2026-10-11
updated: 2026-10-11
links: ["[[build-system]]", "[[decompile-workflow]]"]
---

## Goal
Fix seven wave-7 tooling failures, with synthetic regression tests and Docker build evidence.

## Acceptance criteria
- [x] Exact unit/object filtering in dupes and neardupes.
- [x] Data-island includes cover every generated piece byte-for-byte.
- [x] K&R definitions produce unprototyped declarations.
- [x] Every funcdiff DIFF has a verdict; table/string naming noise resolves.
- [x] Permuter accepts candidates whose body passes the real build.
- [x] Build rejects overlay addresses in the shared main symbol file.
- [x] General orphan-string ownership solution or documented limitation.
- [x] Clean 27-unit checksums, progress baseline, branch check, tests and inline review.

## Notes
Shared brief read from scratchpad/tooling7-brief.md. Leave changes uncommitted for the orchestrator.

## Comments
2026-10-11: Initial causes: basename prefix filtering; pointer-only data pieces omitted; K&R definitions excluded by sync_protos regex; successful permuter builds rejected for outside-body edits. Baseline clean Docker build passed 27 checksums.

## Verification
- Docker baseline and final clean configure/ninja: 27/27 sha1 OK.
- build/headers.ok, build/globals.ok, build/symbols.ok present.
- sync_protos --check-branch: 0 new disagreements, 17 known.
- ninja progress: 4729/6958, 726844/2279368 bytes; no function gains or regressions.
- All 30 tools/test_*.py modules passed in Docker. New/updated regressions cover both --apply entrypoints, byte reconstruction of all data pieces including pointer-only and unaligned labels, K&R multiline and one-line definitions, internal labels and empty/external scoped strings, final DIFF verdicts, permuter advisory declarations, shared-symbol contamination, orphan string queue/cache handling and one-overlay-file splitting. Tests affected by later edits rerun.
- OPTION func_8013C52C: automatic table relocation MATCH. GEKO func_8013CD18: naming-only DIFF; --resolve MATCH. Both --resolve MATCH after clean build.
- Queue --blocked --files TACO/8013DB80: func_8013F250 has SO.
- Isolated TACO split: 4 writes, no added prototypes; original overlay sha1 OK; new func_8013F250.s has both .asciz blocks. Recipe in [[build-system]]. Trial did not modify the worktree's src/include/config objects. The C draft remains the matching agent's follow-up.

## Inline review against CODING_STANDARDS.md
Reviewed configure.py, all ten changed/new tool modules and their nine test modules. Python tooling only; no host dependencies, new compiler pass, C/header edit, fakematch or raw game-data edit. Exact file selection rejects overlay/address collisions and prefixes. Island include generation shares the piece-start rule with the generator; synthetic reconstruction verifies no holes or repeats. String fallback reads only the scoped original unit's rodata and leaves table contents to sha1. Every DIFF ends with a verdict, including --resolve. Permuter still requires real function and unit verification and restores the original file. Shared-symbol guard fails the build and is a dependency of C compilation. Partial overlay split preserves other C files. No unresolved review findings.

2026-10-11: Inline review complete; no findings outstanding. Final post-review funcdiff tests: 46 passed; ninja again passed all 27 checksums, branch check OK. Leave all edits uncommitted for orchestrator review/commit.
