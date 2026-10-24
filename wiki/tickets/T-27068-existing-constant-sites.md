---
id: T-27068
title: Reuse proven constants at precise literal sites
status: Done
assignee: constant-tooling
created: 2026-10-24
updated: 2026-10-24
links: ["[[rename-tool]]", "[[tickets/T-27067-core-scheduler-essential-completion]]"]
---

## Goal

Allow reviewed literal replacement to reuse an existing matching enum value and select only proven expression sites. The scheduler baseball-club hunk currently exposes duplicate-member rejection and broad member-site ambiguity. Preserve proof/rollback/ledger behavior; do not replace girl7 or other unexplained values.

## Acceptance criteria

- [x] Existing-name/value/type ownership validated, mismatches/collisions rejected.
- [x] Exact reviewed semantic sites supported without blanket value replacement.
- [x] Meaningful synthetic regression tests and independent Standards/Spec reviews resolved.
- [x] Root integrates reviewed tooling and then applies proven sites with matching proof.

## Work notes

2026-10-24 scratch implementation: copied current `tools/rename.py` and `tools/test_rename.py` to ignored `.claude/orchestrator/state/T-27068/{baseline,result}/`. The two-file `tooling.patch` and `manifest.json` record baseline/result hashes; live tools were not edited. New opt-in `const --reuse ENUM` validates explicit existing enum member value/type/header ownership, then selects a unique function-scoped comparison expression or array-base index (explicit `@N` required for repeated sites). Integer U/L suffix types are preserved with casts; enum declarations and fixed-width fields stay unchanged. Reuse has inverse/gcc/prototype proofs, versioned request metadata in existing const ledger rows, stale-site check/replay, and post-write source/ledger rollback. Metadata follows function/tag/member/header ledger renames; field/local expression changes deliberately fail closed and need renewed site evidence.

Docker validation: all 93 rename tests pass, including 15 new focused tests for comparison/index separation, unselected literals, suffix types, mismatched ownership/value/type, ambiguous same-value names, shadowing, repeated sites, expression boundaries, proof failure rollback, apply/check/repeated replay, post-write rollback and rename-chain metadata. A dry run on the real `day_event_queue_build_week` selects exactly the two proven `CLUB_BASEBALL` literals and passes inverse/gcc/views proofs, with no source flush or live apply. Proofs are saved under the ignored packet. Independent Luna Standards review and root Sol 6.1 Spec review are pending; integration and matching build remain root-owned.

2026-10-24 final scratch freeze: root requested validation of the entire owning enum's range. Reuse now rejects any member with an unsigned/wider/negative/unevaluated initializer rather than assuming compatible representation. All 94 rename tests pass (16 added); the final real-source dry run still names exactly two literals with inverse/gcc/views proofs. Luna Standards review has no remaining findings after the test-loop clarification. Final patch SHA-256 `fcd25deb9140948fa4cccce142e1055353bb13c9bf317ce3b997bc2d6c3141cd`; manifest and proof logs are in the ignored T-27068 packet. Root has assigned independent Sol 6.1 Spec review. Live tooling hashes still match the baseline; no integration/apply/build/commit was performed.

2026-10-24 independent Spec finding: ordered value/type agreement did not prove source provenance. An active macro-expanded comparison could match the typed AST while an identical raw comparison under `#if 0` received the edit. The corrected scratch planner fails closed unless the full selected function token stream survives preprocessing unchanged, and anchors the AST function declaration to the same source file and unique physical declaration line. Regressions cover the reported inactive/active-macro exchange, a discarded macro argument on the same physical line, and an inactive source function identical to an active header function. All 98 tests pass (19 new reuse tests). Final scratch patch SHA-256 `68df2307fd6dccd3dcd84ff321c5ba3e7611de064d71716aaad6f12e71626a8a`; final provenance/inverse check still selects exactly two real scheduler literals, with compiler/prototype proof refresh pending. The packet baseline is now root's live tools containing the approved T-27069 filter; the result preserves that filter and its regression unchanged. T-27069's frozen packet was not modified. Root owns independent re-review and integration; no live tools/source/Git index edits by this worker.

2026-10-24 final proof refresh completed: the final source-provenance patch selects exactly two real scheduler literals and passes inverse/gcc/prototype-view proofs, with no source flush. `real-site-dry-run.txt` and `manifest.json` now record completion. Independent Spec re-review remains root-owned.

2026-10-24 root tooling integration: exact final98-test packet applied after baseline/patch/resultSHAchecks. Integrated98testsPASS2.383s. Luna Standards and independent Sol Spec review findings resolved; finalthreepreprocessing/source-provenance regressions verified. No Ctypes/declarations changed. Actualtwo-literal application and matching proof remain next; ticket InReviewuntilthatproof.

2026-10-24 source application complete: exactlytwoCLUB_BASEBALLreferencesreplaceconfirmed6values in the existingclubcomparison/recordindex. Allotherliteralsincludinggirl7remainunchanged. Fullprovenance/inverse/gcc/viewsand3923-rowledgerchecks pass. Latestincrementalbuildrecompiledmain/TEL/RPG; all27currentbinariesindependentlyrehashidenticaland6952functions/2276920bytes remain. Independentactual-sourceSpecreviewconfirmsquarterviewequivalence,2literalonlysitesandRPGsigned-bytestore target. Done.
