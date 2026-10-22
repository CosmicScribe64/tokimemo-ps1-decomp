---
id: T-25024
title: Qualify TEL 80136724 in src/ovl/TEL/80132000.c
status: In Review
assignee: Sol 6.1 medium
created: 2026-10-22
updated: 2026-10-22
links: ["[[plan-remaining]]", "[[matching-notes]]"]
---

## Goal

Qualify and repair the recovered TEL func_80136724 body against original semantics, then establish a complete-TU resolved baseline. Exclusive source ownership: src/ovl/TEL/80132000.c, only func_80136724.

## Acceptance criteria

- [x] Audit undefined fields, semantic holes, renamed helpers and call ABI using target evidence.
- [x] Preserve source hash, compiler context, word counts, printed lines, changed positions, aligned cost and earliest divergence separately.
- [x] Preserve a faithful candidate or a precise blocker/replay. No assembly-fallback or draft coverage claims.
- [x] Standards/spec self-review; finish In Review for coordinator review.

## Notes

Pinned base: 883760224a9ff9712c4850ca7662c5ce3df2625d. Main had advanced to bfa45d03bd00f57500cdfb2212c0d722ffe35f7e; pinned base is its ancestor. Isolated worktree /private/tmp/tokimemo-t25024, branch codex/t25024-tel-qualification. No shared headers/build tooling or full27 integration builds in this lane.

## Result

Zero new C matches/bytes. Production source is identical to the pinned base. Source-only checkpoint at drafts/T-25024 contains hand and pointer candidate bodies, exact hashes, context hashes, baseline results and complete-TU Docker replay. Raw corpus holes were already repaired by tracked drafts; qualification adds no newly justified semantic source repair. Residual full semantic equivalence, especially all call argument values and paths, is not proved by the donor or call-address audit.

Primary changed ordinal positions: hand 1372, pointer 1403, both 1643 words versus target1645. Secondary printed lines/aligned cost: hand1048/1038, pointer984/970. Lower aligned cost is not an improvement in changed positions or byte coverage. Pointer source SHA25667dd63003ff880c85cd487f64142d0c330455fdfa8a296c044e91fc67cc8cbad; hand19a48e33187dbd6f4c5e1aad373993c167ab40da401b458abe9338a2231b3c55. Both archived replays reproduce these metrics.

Earliest pointer differences: word12 BEQ displacement10410647 versus10410645; word85 first register-only difference (target t2, candidate t1); word326/target0x80136C3C NOP versus candidate move-v1-zero, moved from target word331/0x80136C50. Remaining17 register/branch-offset-masked structural hunks concern initialization placement, redundant address setup and packed-entry scheduling. Original frame0x198 retained. No new concrete semantic defect or donor-backed source control found; no month/promotion/volatile/source-shape/declaration sweep repeated.

## Comments

### Standards/spec self-review (2026-10-22)

Applied `.claude/skills/code-review/SKILL.md` to the non-empty worktree diff against fixed base883760224. Explicit single-worker assignment supersedes that skill's parallel-reviewer steps. Standards: only new source drafts/replay/results and wiki changes; C89 bodies use existing SDK/project types, old field/type aliases repaired without headers, padding retains its FAKE ticket; no inline asm, generated assembly, objects, raw data, ucode or full-TU/header context staged. Replay uses Docker-only project compiler path. Existing source remains INCLUDE_ASM; no match claim. Spec: pinned isolated worktree, only assigned function investigated, no sharedroot/header/build-tool edits, no full27 build, no unsolicited messages or additional agents. No unresolved standards/spec findings. Matching residue remains explicitly open; ticket is In Review for coordinator review.

Validation: default TEL unit checksum passes, explicitly with assembly fallback; complete-TU hand and pointer source replays pass at recorded metrics. Declaration guards and final diff checks recorded below. No literal default-C proof exists, so no all27 integration gate is due in this lane.

Final guard results: `check_headers.py` passes. Initial `migrate_globals.py --check` could not validate five existing DATE/TACO keep proofs because this fresh tree had split only TEL; generated DATE/TACO split inputs (no C build) and reran. See final log entry for successful rerun/prototype results. `git diff --check` passes. No production diff.

Rerun: `migrate_globals.py --check` passes; `sync_protos.py --check-branch` reports 0 new disagreements, 12 known and branch check OK in the partial-split tree. It notes three previously known void-result rows not found (GYOZI func_8008D610 and MAIN SsVabTransCompleted/func_800AD950); no baseline file was edited. Root retains full integration/prototype-corpus validation. Exact earliest BEQ targets: original 0x80138074 vs candidate 0x8013806C (word 12). No new causal source control is justified; stop here for coordinator review/ownership decision.
