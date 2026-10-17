---
id: T-11230
title: m2c variant sweep in bulk_m2c
status: Done
assignee: agent
created: 2026-10-14
updated: 2026-10-17
links: [[[decompile-workflow]], [[tickets/T-11190-bulk-m2c-pipeline]], [[tickets/T-11210-bulk-m2c-draft-failure-classes]], tools/bulk_m2c.py, tools/m2c.py, tools/test_bulk_m2c.py]
---

## Goal

m2c's draft depends on its options. For each target, `tools/bulk_m2c.py` makes several drafts with different option sets and keeps the best-scoring one before the autotweak climb. A retry mode runs the sweep again on earlier near misses and failures.

## Acceptance criteria

- [x] `--variants N` (default 1, today's behaviour) takes the first N of a fixed, ordered list `VARIANTS` of option sets. Each set only uses options that m2c 708d2d2 (tools/Dockerfile) or tools/m2c.py has, with a comment per entry.
- [x] Each variant goes through the same cleanup, compile fixes and scoring; the lowest score wins, a tie keeps the earlier variant, a score of 0 ends the sweep. Then match/verify, climb, near/fail as before. The winner is in state.jsonl (`variant`, `tried`) and in near.txt (sixth column).
- [x] `--retry near,fail` processes those state entries again and replaces their lines; `--max-score S` limits retried near entries to scores of at most S.
- [x] The summary lists wins per variant (matches and near misses, sweeps only).
- [x] Synthetic tests: variant list, best pick, retry selection, state replacement, summary. test_bulk_m2c, test_autotweak, test_order_search, check_headers, test_check_headers pass.
- [ ] Real-build trial (orchestrator): `--unit GYOZI --limit 5 --variants 6`, then the full `--retry near,fail` run.

## Notes

Variant order: default, no-andor, reg-vars (`--reg-vars saved`), void, no-switches, no-callee-args, no-postinc, no-unk-inference, no-context, gotos-only. Left out: `--stack-structs` (prints template structs only), `--no-casts` (IDO needs the casts), `--valid-syntax` (M2C_* macros, refused by cleanup), formatting options.

A variant whose m2c output repeats an earlier one's is not compiled again. If the winner is not the last variant compiled, it is compiled again, because all scorers of a function share one work directory. When no variant scores, the first failure is recorded.

All option names were checked with `m2c --help` in the toolchain image, and each flag was run on a synthetic asm function.

## Comments

- 2026-10-14 agent: implemented on branch t11230-m2c-variants; In Review until the real-build trial and the code review.

2026-10-17 cleanup: merged (df9e32d5d). Tests cover the variant list, best pick, retry selection, state replacement and summary, and code review found nothing. The real-build trial was run: 17f9572ef added 5 matches from a partial retry, and T-11270 then ran the full near+fail retry to 738 of 1167 entries. The unfinished 429 entries are covered by T-14500, which made a bounded attempt on every remaining function. Closed.
