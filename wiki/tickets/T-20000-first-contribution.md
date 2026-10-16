---
id: T-20000
title: Avoid whole-tree cache scans for scoped queues
status: In Review
assignee: Codex
created: 2026-10-16
updated: 2026-10-16
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Make file-scoped work-queue queries usable on slow bind mounts by avoiding validation of a cache covering the whole checkout. Ask the maintainer for a preferred communication platform in the first pull request.

## Acceptance criteria

- [x] Scoped queries skip the whole-tree signature scan and leave the shared cache untouched.
- [x] Global ranking, planning and calibration continue to validate the full cache.
- [x] Synthetic regressions reproduce the bugs before the fixes; all 54 queue tests pass afterward in Docker, including the review follow-up.
- [x] Real scoped query output is identical before/after; measured 53.402 s before and 5.312 s after on this Windows/WSL bind mount.
- [x] Untouched game-source baseline: all 27 binaries SHA1 OK, symbols/headers/globals OK.
- [x] Local review against CODING_STANDARDS.md; awaiting maintainer review.
- [x] Submit the pull request with the communication question: https://github.com/CosmicScribe64/tokimemo-ps1-decomp/pull/36.

## Notes

Started from upstream main a4c75a6373aadeb2e2b67634510dc5ce4d6bdc4a on branch codex/first-contribution. Windows host with an existing Ubuntu-24.04 WSL Docker daemon; no host toolchain or dependency installation. The Docker image was built from tools/Dockerfile. The game image stays under gitignored game/.

The first bounded SHOUGATU/DATE2 selector probes did not match and were kept under gitignored build/T-20000/. No production C or header changed. During candidate selection, even a scoped queue call spent substantial time before its first progress message: Cache.__init__ computed tree_signature before the --files scope was considered. The fix computes the scope first and disables the whole-tree cache for a narrow load. This deliberately rereads the requested functions; it neither trusts an unvalidated full cache nor rewrites that cache with partial results.

Benchmark command inside Docker: python3 tools/queue.py --files 800451D0 --next 10. The pre-fix source was frozen locally; its complete stdout equalled the fixed source's stdout. Timings are one before/after measurement on this checkout, not a platform-independent speed guarantee. Scoped loads still collect string symbols and source/config metadata; this change only removes the unrelated whole-tree cache signature scan.

Verification: tools/docker.sh python3 tools/test_queue.py (52 tests); the two scoped-query regressions fail against the original code and pass afterward. The third new test checks that --by bytes, --plan and --calibrate with --files still validate the full cache. Existing cache invalidation and ranking tests pass.

## Comments

2026-10-16 local review against CODING_STANDARDS.md: no findings. Python-only CLI change; no compiler passes, game C, headers, configs or generated data changed. Tests cover missing/existing cache, unchanged output, edited asm visibility, preservation of cache contents, and global-mode validation. No unverified match claims. Ticket remains In Review for upstream review.

PR #36 submitted; upstream main 639693d2 merged into the contribution branch. The append-only wiki log conflict was resolved by retaining both sides. Before integration, ninja had no work and sync_protos --check-branch reported 0 new / 15 known disagreements.

Maintainer review follow-up (2026-10-16): empty comma-separated --files entries now drop before scope/cache selection and final filtering. Leading/trailing/repeated commas no longer broaden a valid or unknown prefix to every file; an all-empty list uses the normal whole-project cache. Two new regressions fail before the fix and pass afterward. Updated load_cached's docstring to distinguish main's uncached scoped path from explicit full-cache callers, and its progress message now names selected files for a scoped load. Docker tools/test_queue.py: 54 tests pass (build/T-20000/review-tests.log); both pre-fix failures recorded in review-regressions-before.log. Local standards review: no findings; only intended Python/wiki edits, no game data. Awaiting maintainer acceptance.

Coordination response: claim new files/function groups through a GitHub issue before starting; use Discussions for questions/general coordination. The maintainer handles wiki conflicts when merging, so no further main merges are needed on this PR.
