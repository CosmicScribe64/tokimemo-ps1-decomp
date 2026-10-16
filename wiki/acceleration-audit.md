---
type: concept
updated: 2026-10-15
sources: ["tools/permute.py", "tools/bench_strip.py", "wiki/tickets/T-12200-acceleration-audit.md"]
---

# Matching acceleration audit

Snapshot at base `7a65b3d8`; active cx worklists inspected read-only on 2026-10-15. No TT work or modifications to existing worktrees. See [[tickets/T-12200-acceleration-audit]].

## Measured implementation

`strip_functions` previously scanned the entire preprocessed translation unit for each name, including hundreds of header prototypes. It now enumerates signature locations once and applies the unchanged definition regex at those offsets. The first definition per name, K&R conversion, PERM exclusions, brace matching, reverse-order edits, and all verification gates are preserved.

One container, existing `tokimemo-decomp-arm64` image, `--cpus=1 --memory=1g`, with other workers running. Sample: first eight sorted main sources plus every thirtieth sorted non-TT overlay source, first definition per file, three repetitions. All **63 compiler-input comparisons across 21 files were identical**. Sum of per-file median stripping times: **14.633 s before, 0.800 s after (18.29x)**. This saves about **0.659 s per sampled strip**, not per cached candidate. Timing includes regex cache effects under a fixed before/after order; it is a bounded setup benchmark, not an end-to-end solver throughput claim.

Reproduce from the repository root inside the same capped container:

```sh
python3 tools/bench_strip.py --baseline build/acceleration/permute_before.py
```

Export the baseline first with `git show 7a65b3d8:tools/permute.py > build/acceleration/permute_before.py`. The benchmark preprocesses ordinary source/header text and checks exact output equality; it needs no game binaries. It fails on preprocessing or equality errors. A single-file smoke run of the committed harness measured 0.581 s versus 0.028 s (21.1x). Raw measurements remain ignored in `build/acceleration/strip-results.jsonl`.

Validation: 55 permute, 20 fastscore, 75 autotweak, 65 bulk_m2c, and 14 order_search tests passed (229 total); `check_headers.py include src` passed. No game C, compiler flags, scoring, guards, or linked verification changed. No new match or whole-game checksum claim is made; rebuilding all binaries would not exercise this setup-only change.

## Priorities for the matching owner

1. **Exact-verify the queued zero-score seed early.** cx9 has BUNKA_SD:func_80132428 (512 listed bytes) with an old score of zero. Promote its verification ahead of blind searches if the owner can adjust scheduling. This is a lead, not a match; score provenance and current full-unit behavior must be checked. The draft belongs to cx9 and was not applied here.
2. **Adopt the reviewed strip optimization for new draft setup.** Existing [[tickets/T-11250-faster-candidate-scoring]] caching already removes repeated stripping inside a search. Expected savings are about 0.66 s per cold strip on this sample, or approximately 66 s over 100 such setups. No sustained candidate-rate multiplier is established.
3. **Use bounded, full-unit-guided searches on close families.** cx4 lists RPG_BAT:func_8013415C and func_80134570, each 1,096 listed bytes with old score 60. Investigate one and carry justified source idioms to the sibling before independent exhaustive searches. Their equal scores and sizes suggest a reuse opportunity, not proof of matching bodies. Score values across different scoring implementations are not directly comparable.
4. **Budget concurrency globally.** Three independent solvers defaulting to `cores - 2` can request 24 scoring workers on a 10-CPU VM; order_search defaults to all CPUs. Use explicit `--score-jobs 2` for autotweak, `--jobs 1 --score-jobs 2` for bulk_m2c, and `-j 2` for order_search/permuter during the three-worker run. This is an allocation recommendation, not a measured speedup. Container CPU quotas do not by themselves make `os.cpu_count()` quota-aware. Leave room for exact builds and the outside contributor. No defaults or other workers were changed.
5. **Refresh duplicate reports after verified integrations, before generating fresh drafts.** Existing dupes/neardupes already handle symbol mapping, constants, and guarded checks; [[tickets/T-7030-tooling-wave-4-bug-fixes]] documents 163 + 25 copied functions historically. Run read-only reports against a consistent generated-asm snapshot, then assign copies by owned file. This audit did not run or measure a new reuse sweep; generated assembly is absent in this worktree.

## Avoid repeating known negative experiments

- [[tickets/T-11260-fast-scoring-order-search-permute]] found 0/22 stripped scores equal to full-unit scores, including a different best-candidate ranking. Keep exact rescoring and final linked-byte verification; fast zero is insufficient.
- [[ido-52-evaluation]] reports IDO 5.2 changes no output versus 5.3 plus existing passes on 2,564 controls; 4.1 mixtures lose 80 or more. Switching compilers is not an evidenced short-term acceleration.
- [[tickets/T-9800-uopt-register-allocation-trace-and-uniform-patch]] rejected all eight uniform allocator candidates because they regress matching controls. Prefer source-idiom experiments until a new discriminating hypothesis exists.
- Avoid broad concurrent brute force or overlapping package ownership. The active run's status combines an older table with later append-only updates; use exact verification records rather than treating the initial table as current progress.
