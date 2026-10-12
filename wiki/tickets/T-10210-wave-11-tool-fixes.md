---
id: T-10210
title: Wave-11 tool fixes
status: Done
assignee: r12-fixes
created: 2026-10-11
updated: 2026-10-11
links: ["[[matching-notes]]", "[[decompile-workflow]]", "[[toolchain]]"]
---

## Goal

Repair source-state duplicate classification, invalid permuter bases, and m2c shared-HI/member drafts; investigate EN_NICHI/TACO local frames without regressing matched code.

## Acceptance criteria

- [x] Each wrapper fix has unit regression coverage.
- [x] Frame finding has reproducible evidence; any pass change preserves every matched function.
- [x] Docker tests, clean 27-unit build, progress and prototype/header/global guards pass.
- [x] Inline review against CODING_STANDARDS.md resolves all findings.

## Notes

Shared tooling12-brief.md read. No edits to configure.py, tools/split_objects.py or config/. Leave changes uncommitted for the orchestrator. Scratch: scratchpad/r12-fixes/.

## Comments

2026-10-11: Claimed after reading index, brief, local instructions, standards and wave-11 reports. Generated outputs are absent; disc/ exists. Existing m2c test explicitly reproduces lost shared-HI store offsets.

2026-10-11: Done. dupes/neardupes classify from C source; permute.py validates the base before searching; m2c.py keeps distinct %lo stores and avoids malformed negative members; frame finding in [[decompile-workflow]] (no pass change). Inline review against CODING_STANDARDS.md: no findings.
