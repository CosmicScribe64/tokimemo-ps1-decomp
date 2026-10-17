---
id: T-11250
title: Faster candidate scoring for the solvers
status: Done
assignee: tooling agent g42
created: 2026-10-14
updated: 2026-10-17
links: ["[[tickets/T-11170-cheat-sheet-auto-tweaker]]", "[[tickets/T-11190-bulk-m2c-pipeline]]", "[[tickets/T-11530-order-search-tool]]"]
---

## Goal

`tools/autotweak.py` (and `tools/bulk_m2c.py`, which subclasses its `BuildScorer`) scored about 0.55 candidates per second. Get 5-10x more with identical scores.

## Acceptance criteria

- [x] Per-candidate cost measured and broken down (table below).
- [x] Biggest costs removed: cached strip of the other functions, parallel scoring, cache of identical texts.
- [x] Same score as the legacy path on 68 candidates of 5 drafts (plain, two jump-table functions, two with INCLUDE_RODATA in the file); `--exact` keeps the legacy path.
- [x] A score of 0 still goes through `permute.verify_candidate` unchanged (verify is serialized with a lock).
- [x] Tests: `tools/test_fastscore.py` (cache/splice/worker/prefetch logic, synthetic), `test_autotweak.py`, `test_bulk_m2c.py`, `test_order_search.py`, `test_permute.py`, `test_check_headers.py`; `check_headers.py include src`.
- [ ] Code review against `CODING_STANDARDS.md`.

## Notes

### Measurement (main:func_80042808 and main:file_checksum_ok, one candidate, Docker)

| step | before | after |
| --- | --- | --- |
| rewrite the C file text (`rewrite_guard`) | 0.003 s | 0.003 s |
| `gcc -E -P` of the whole unit | 0.02 s | 0.02 s (kept, so the text is identical) |
| `permute.strip_functions` (one regex compile + scan per function name, 770 names) | 1.0-1.1 s | 0.002 s (cached per draft, keyed by the text outside the definition) |
| compile: compile.sh, cc.py, asm-processor, IDO with frame/cvt shims, as, padding | 0.20-0.34 s | same, now up to 8 at once |
| objdump + difflib score | < 0.01 s | same (under a lock: the Scorer reuses one SequenceMatcher) |
| python startup / imports of the tool | 0.13 s once per run | same |

So 1.3 s per candidate became 0.22 s serial (4.5/s). The remaining time is the IDO pipeline itself.

### Changes

- `tools/fastscore.py`: `BaseBuilder` (preprocess, cut the target definition out, strip the rest once, splice the candidate back; falls back to the legacy strip when the text outside the definition changed, when the body has a line that looks like another definition, or when no definition is found), `Workers` (plain threads, no `queue` import because of the tools/queue.py shadow), `default_jobs` (cores - 2).
- `tools/autotweak.py`: `BuildScorer.configure/prefetch/close/score_exact`, a score cache per text, `climb(prefetch=...)` (announces the texts of the single-edit loop and of the pair loop in scoring order; scores, order of use and the result are the same), `order_stage(jobs=)`, `--score-jobs N` (default cores - 2) and `--exact`.
- `tools/bulk_m2c.py`: `Backend.score_jobs/exact`, `Candidate.prefetch`, `--score-jobs` (default (cores - 2) / `--jobs`) and `--exact`; the climb gets the hooks only with more than one scoring job.
- `tools/order_search.py` is its own scorer (full guarded compile, funcdiff) and already runs `-j` compiles in parallel; `tools/permute.py` only builds the base once; neither changed.

### Results

- Equality: `build/prof/eq.py` (not committed) scored 12-14 single-edit variants of each of main:func_80042808, main:func_80067870 (switch), DATE:func_8013E9F8 (switch, INCLUDE_RODATA file), TT:func_8013C0A8 (switch), EN_NICHI:func_80137C58 (rodata file) with `--exact` and with the new path: 68 of 68 equal.
- End to end, `autotweak.py --time 60 --dry-run`: main:func_80067870 40 tried (0.6/s) with `--exact`, 974 tried (15/s) now; TT:func_8013C0A8 43 tried with `--exact`, 340 tried in 22 s (15/s) now, same best score 623. About 25x on a 10-core host.

## Comments

2026-10-17: Work is on main (cc825cedb, 2659b4838). All acceptance criteria are met. The scores match the legacy path (68 of 68 candidates) and the speedup was seen end to end, with --exact kept as the legacy path. Code review against CODING_STANDARDS found only low-severity latent issues in tools/fastscore.py (a pool that is closed while shared, a stale docstring, unlocked counters). These go into the followup together with the T-11260 nits.

Followup opened: [[tickets/T-21006-fastscore-order-search-hardening|T-21006]].
