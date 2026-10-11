---
id: T-9410
title: Register-allocation shapes after wave 7
status: Done
assignee: r7-regs
created: 2026-10-11
updated: 2026-10-11
links: ["[[data/t0018-cases]]", "[[matching-notes]]", "[[decompile-workflow]]"]
---

## Goal

Cluster the T-0018 rows against original assembly, test C idioms for recurring
register-allocation shapes, and land byte-verified matches.

## Acceptance criteria

- [x] Reproducible cluster counts with synthetic tooling tests.
- [x] Experiment evidence for constants, DATE products, fade/girl, GameState and ENDING remainder shapes.
- [x] Successful idioms documented and resolved rows identified without deleting history.
- [x] Clean 27-unit build, header/global guards, prototype check and progress verified.
- [x] Inline standards review completed.

## Notes

Shared tooling7 brief governs ownership and verification. No commits or branch
operations. No uniform compiler pass without broad deterministic evidence.

## Comments

- 2026-10-11: Read shared brief, standards and prior workflow evidence. Starting
  original-assembly generation for the row inventory.
- 2026-10-11: Five gains: main `func_8005AE60`; ENDING `func_80138278`,
  `func_8013855C`, `func_80138FC8`, `func_801391E4`. Real resolved diffs and
  both affected-unit checksums pass. The four ENDING functions have marked
  FAKE dead tests; `func_8013855C` also has a marked narrow constant local.
  No new pass, header change or existing-tool edit. New standalone classifier
  and tests are the only tooling additions (not shared r7-fixes files).
- 2026-10-11: Moved to In Review. Inline review uses the local code-review
  skill's standards/spec axes against the uncommitted changes from HEAD,
  adapted to the shared brief's explicit inline-review/no-commit instruction.
  Detailed counts, unsuccessful variants and limits: [[register-allocation-wave7]].
- 2026-10-11: Final clean build: 27/27 SHA1 OK, headers.ok/globals.ok present,
  sync_protos.py --check-branch exit 0. Progress 4734/6958, 729612 bytes:
  +5 functions and +2768 bytes. Classifier tests: 3 pass; diff whitespace check
  passes. Standards review checked C89, fixed-width locals, existing GameState
  and array views, marked FAKEs, unchanged headers/config/pass behavior, source
  placement and raw-data exclusion. Spec review checked all five requested
  shapes, counts and resolution records. No unresolved findings; no universal
  C-impossibility claim. Moved to Done. Changes remain uncommitted for the
  orchestrator, per the shared brief.
