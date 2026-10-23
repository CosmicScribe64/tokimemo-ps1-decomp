---
id: T-25035
title: GYOZI zero carrier through late folding
status: Done
assignee: Astra GYOZI worker
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-25025-gyozi-zero-reaching-definition]]", "[[playbook]]"]
---

## Goal

Match GYOZI func_80140E0C, 388 bytes, using the new SHOUGATU late-folded zero carrier evidence. Exclusive source ownership: src/ovl/GYOZI/80140C90.c.

## Acceptance criteria

- Literal full-TU C and default-C exact match, affected overlay checksum and declaration checks.
- No inline assembly, patched instructions, per-function compiler exceptions, or semantic shortcuts.
- Root handles integration and full 27-binary gate. Review required before Done.

## Notes

Baseline 97 target / 96 candidate instructions, aligned cost 5. T-25025 established that the prior local zero dies before allocation; new work tests a distinct late-folding mechanism. Isolated tree /private/tmp/tokimemo-t25035 from 242c0c3a3. Ordinary compiler difficulty is not a stop condition; latest user instruction supersedes historical time/probe limits.

## 2026-10-22 measurements

Baseline reproduces 97 target / 96 built words, cost 5. Donor transfer (`girl = (u32)D_800F62CF < 0`, first call `func_8005E0E0(girl | 0, girl)`) restores exact entry initialization/store/call and gives 97/97 words, aligned cost 4. Remaining reload base is a0 rather than a1, with adjacent scheduling displacement. Adding the late-zero expression to the second call changes that base to v1, still cost 4. Private full-TU candidates and measurements: build/t25035/.

Focused carrier/global coalescing controls (narrow local types, byte store, first-call second-argument views) did not close this register residue. Explicit high-address second-argument controls added instructions and are rejected. A traced compiler run is inspecting the exact allocation boundary before further source changes. Source remains unchanged and guarded; no match claim.

## Completion evidence and review handoff

Promoted func_80140E0C to default C after early-global-assignment transfer from T-25033. Simpler `D_800F62CF * 0` form independently measures 97/97 words with zero differences. Both the late expression and early assignment are annotated FAKE; original byte-view/narrowing annotations retained. No ignored arguments remain. Flag tests read disjoint globals with no calls or volatile effects, so earlier source assignment preserves behavior; actual compiled store stays at the target merge.

Docker `funcdiff --resolve GYOZI:func_80140E0C`: compared default build, MATCH (linked function bytes equal target, 388 bytes). GYOZI binary SHA1 43f92523116e7b6cd772d4397712a52eeda89703 passes. Headers, globals and branch prototype guards pass, zero new disagreements. Initial global guard lacked DATE/TACO assembly references in fresh checkout; generated their split outputs and reran successfully. Full-unit checksum also covers all neighboring functions.

Moved to In Review. Self-audit against CODING_STANDARDS.md found no C89, ownership, declaration, unmarked-FAKE or raw-data issues; independent review and full 27-binary integration remain with root. No other production files changed. Evidence and rejected controls are in drafts/T-25035/README.md.

Root independent standards/spec source review passed on 2026-10-22: multiply-by-zero is defined for this byte input; early store crosses only disjoint nonvolatile reads, no calls; FAKEs are annotated; no build exceptions. Ticket remains In Review pending the combined integration gate.

## Root integration verification

Root standards/spec review has no findings. Fresh Docker rebuild and all27 binary checksums pass; both functions verified as default C with literal linked bytes, assembly fallback removed. Headers/globals pass; prototype check has zero new disagreements. Root progress6835/6952 functions,2105536/2276920 bytes. Accepted and moved to Done.
