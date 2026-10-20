---
id: T-24517
title: Reconstruct MASTER animation flow and pointer lifetimes
status: In Progress
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[tickets/T-24514-master]]", "[[families/master-family-results]]"]
---

## Goal

Deep region-by-region reconstruction of MASTER func_801363F4, preserving target-specific flow and recovering clone/point-loop lifetimes.

## Acceptance criteria

- Any claimed match requires literal default C, linked bytes, all27 checksums, interface guards and standards/spec self-review.
- Otherwise isolate a concrete source or backend reproducer; no compiler-impossibility inference from a few variants.

## Notes

Base c768eda884, isolated /private/tmp/tokimemo-t24517, branch codex/t24517-master-flow. MASTER-only ownership. Existing T24514 candidate has original64-byte frame and first213 instructions exact; copy-end temporary, clone pointer comparison and spill homes remain. T24092 already fixes the historical unaligned-copy F/B direction issue; this lane does not repeat that fix.

## Comments

- Read index first, current status/playbook/standards/local guidance, function catalog, T24514 source/handoff and MASTER family history. Root explicitly requests deeper structural reconstruction, not tiny type grids. No agents, main_api edits, inline assembly, compiler patches or unrelated ownership.

- Structural evidence: target reuses clone spill sp34 as later speed spill sp34; point pointer uses sp30. All-indexed source restores original clone sltu/bnez compare but initially has +4 spill homes and653 instructions. A real RNG result local restores call-before-point-y-store order, all original spill homes,64-byte frame and652 instructions; aligned score330 (T24514 baseline342). Original first213 instructions still exact. Generic work-pointer reuse438 is merely a negative source control, not a backend conclusion.

## Structural checkpoint

`drafts/T-24517/func_801363F4.c` preserves the all-indexed/value source before further register work. `drafts/T-24517/checkpoint.py` recompiles the full translation unit privately, preserves relocation annotations, and reports652 positions,213 exact diagnostic prefix rows,167 register-only substitutions, and0 other rows. Score330 is the aligned full-function diagnostic cost. Section-owned strings/tables remain funcdiff-normalized, so this is not literal linked-byte equality or a C match.

The original flow is recovered independently of the register question: clone sltu/bnez comparison, call/store order, frame64, and clone/speed spill34 with point spill30. The first copy-end register t2/t3 does not alone establish a compiler defect; investigate uopt assigned ranges versus ugen temporary choice separately. Production remains unchanged. Ticket stays In Progress for the deeper reconstruction requested by root.
