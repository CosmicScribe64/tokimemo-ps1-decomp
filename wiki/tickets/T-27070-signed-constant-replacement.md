---
id: T-27070
title: Preserve whole signed expressions in constant replacement
status: Done
assignee: constant-tooling
created: 2026-10-24
updated: 2026-10-24
links: ["[[tickets/T-27062-main-essential-locals-constants]]", "[[rename-tool]]"]
---

## Goal

Fix the reproduced legacy const bug that replaces only a negative expression's positive leaf, leaving unary minus and reversing a negative named definition. Preserve original C types, exact source provenance, proofs and replay; no game-source edits by worker.

## Acceptance criteria

- [x] Whole supported unary expressions replaced; signs and suffix types preserved, ambiguous forms fail closed.
- [x] Regressions cover signed/unsigned/long, parentheses, comparison/assignment/call sites, and source preprocessing provenance.
- [x] Independent review and full rename suite pass; root integrates then applies reviewed sites with matching proof.

## Work notes

2026-10-24 scratch implementation: copied the live 98-test tools (`09a84de71`, captured at HEAD `c5ceee60e5fc201efc008dad2c7a8cc35d327dde`) into ignored `.claude/orchestrator/state/T-27070/{baseline,result}/`. Positive-only legacy requests retain their existing path. A request containing negative definitions selects whole typed AST literal expressions and maps their integer tokens through a verified same-source function/declaration and unchanged preprocessed function token stream. Operand parentheses are included with the unary minus; outer/call parentheses are preserved. U/L suffix types use explicit casts, so unsigned negation retains its original wrap/type. Unsupported ranges/numeric forms, ambiguous value mappings and unproved preprocessing fail closed. The request uses versioned signed const metadata for active-definition validation, check/replay and existing source/header/ledger rollback; inverse/gcc/prototype proofs run before application.

All 111 rename tests pass (13 new). Coverage includes comparisons, assignments, calls, -1, -(1), -((1L)), unsigned/unsigned-long forms, -0x20 graph-scale calls, mixed positive/negative definitions, compiler evaluation of actual generated expression value/size/signedness, preprocessing counterexamples, range ambiguity, proof rollback, both defines/enum repeated replay, inactive definitions, function/tag/member ledger chains and CLI routing. Frozen `tooling.patch` SHA-256 is `d3fce4d87d50a7dfacb12484b7a074579bd8dd539a4004eeb7ef5abef83bdea5`; manifest records baseline/result hashes. Luna Standards review is running; root owns independent Sol Spec review. Final compiler/view dry-run proof for the three actual `hud_biorhythm_value(..., -0x20)` calls is running read-only. No live tools/source/Git/build/apply/commit changes were made by this worker.

2026-10-24 final worker handoff: independent Luna Standards review found no actionable issues. Its initial pending-proof caveat is resolved: the final dry run names exactly three signed -32 HUD graph calls and passes whole-expression provenance plus inverse/gcc/prototype-view proofs without flushing the live tree. `synthetic-proof.txt` records all 111 tests passing; `hud-negative-dry-run.txt` records the actual planned header/source diff and proof. Patch/hash unchanged. Root still owns independent Sol Spec review, integration and matching-build proof; status stays In Progress.

2026-10-24 root integration: independent Sol Spec review approved the exact frozen patch with
no findings; five targeted Docker tests independently checked value/type preservation,
parentheses, provenance, replay and rollback. Root applied the two-file patch and reran all
111 tests successfully in 5.959 seconds. Actual three-site application and its matching
proof remain part of the next source wave; no source-completion claim yet.

Final integration: all three negative graph calls are named with the exact signed scale,
alongside the reviewed positive families (112 literals total). The clean build, all 27
independent hashes, 6,952-function census and 437 unchanged ABI views pass. T-27073 resolves
the cross-family replay guard; all 126 current rename tests pass and actual-source review
finds no issue. This bounded tooling/site task is Done; phase 2 remains open.
