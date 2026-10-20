---
id: T-24521
title: Match remaining TAIIKU functions
status: In Progress
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[playbook]]", "[[families/taiiku-family-results]]"]
---

## Goal

Find verified literal-C matches among the eleven remaining TAIIKU functions in the isolated T-24521 tree. Coordinator T-24520 integrates pinned commits.

## Acceptance criteria

- Literal default C compiles and links byte-identically; all 27 binary checks and declaration guards pass.
- Standards review, FAKE explanations, and attribution recorded before completion.

## Notes

Baseline dba33c4f5. Docker tools only, worker compilers -j1. Prior T-11640/T-16551/T-16552/T-19004 negative evidence read before selecting hypotheses. No broad exhausted grids, toolchain patches, or coverage from source-only drafts.

## Comments

- 2026-10-20: Claimed isolated branch codex/t24521-taiiku under /private/tmp/tokimemo-t24521.

- Fresh typed loader `func_80134060` now compiles: 379 diagnostic rows initially; page construction after each handle lookup aligns the original a2 pointer spill and s0/s1 ranges; eliminating redundant page local and reusing the lookup/modulo local yields 146 rows, then a missing upper scalar slot plus separated first lookup yields 140. Equal instruction count, frame and table offsets now match. Remaining differences start at the repeated 0x80000206 constant's v1 allocation instead of t1, then the ugen ring. A chain assignment is being tested; no broad search or toolchain patch.

### Verified match: func_80134060 (1,348 bytes)

- Literal default C replaces its INCLUDE_ASM. Existing `TaiShorts12` tables, shared index local, lookup-before-handle calculation, and the 10-trip `!=` loop reproduce the structure.
- Two FAKE constructs are documented at the source: unused upper word fixes table homes; unsigned first page-table store keeps the repeated handle constant in a ugen temporary and aligns the entire ring.
- Docker `ninja -j1 build/ovl/TAIIKU.ok`: TAIIKU SHA1 `306d1013b9be465819525d1fb4dc0e86b422a846` OK. `funcdiff.py --resolve --version default TAIIKU:func_80134060`: MATCH, privately linked 1,348 bytes equal target.
- 27/27 binary SHA1 checks: private TAIIKU output and 26 unchanged root outputs mounted read-only at `/baseline-build`; root independently verifies integration. No claim that this lane rebuilt those other 26 outputs.
- `check_headers.py`: OK. `sync_protos.py --check-branch`: OK, 0 new disagreements, 12 known. Initial `migrate_globals.py --check` required DATE/TACO original assembly not generated in this focused tree; generated only their split stamps and reran the guard.
- 2026-10-20: Standards self-review: C89, fixed-width types, existing aligned aggregates, canonical declarations, FAKE reasons, no raw data or generated assembly edits; no findings. Parent independently inspected the final source and found no blocking standards issue; private whole-overlay SHA1 confirms new void declarations preserve all callers. Lane remains In Progress for the other ten functions.
