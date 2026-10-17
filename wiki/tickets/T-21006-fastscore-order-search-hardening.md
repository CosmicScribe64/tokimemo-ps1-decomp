---
id: T-21006
title: "fastscore and order_search hardening nits"
status: Backlog
assignee:
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-11250-faster-candidate-scoring]]", "[[tickets/T-11260-fast-scoring-order-search-permute]]"]
---

## Goal

fastscore and order_search hardening nits. Small fixes from the T-11250 and T-11260 reviews, with tests in tools/test_fastscore.py and tools/test_order_search.py. (a) fastscore.Workers.submit after close() queues a job no thread will run, so Job.result() hangs; shared_workers() closes and replaces a pool that other scorers may still hold when a larger size is requested. Make submit raise after close, and make shared_workers grow without closing a pool in use, or reject a larger size. (b) The Job docstring names a cancel() that does not exist. BaseBuilder hits/misses are updated outside the lock. (c) order_search.rescore_exact worker threads do not catch exceptions, so an unexpected error drops a state silently and can leave the written order unscored; collect the error and re-raise after join. (d) The order_search --fast help text and the compile_variant_fast docstring say 3-4x, but T-11260 measured 2-3x, and no gain for searches below a few hundred variants; correct the text. No behaviour change for scores or write-back.

## Acceptance criteria

- [ ] The work in the Goal is done, or each part left undone is recorded in this ticket with the reason.
- [ ] Code review against CODING_STANDARDS.md has no open findings.

## Notes

Opened from the 2026-10-17 ticket cleanup review of [[tickets/T-11250-faster-candidate-scoring|T-11250]], [[tickets/T-11260-fast-scoring-order-search-permute|T-11260]].

## Comments
