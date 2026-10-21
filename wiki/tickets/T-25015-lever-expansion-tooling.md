---
id: T-25015
title: "Lever-expansion tooling programme: corpus gate, trace observatory, census/oracle, rule fuzzing, precedent miner, sweep ledger"
status: In Progress
assignee: octa
created: 2026-10-21
updated: 2026-10-21
links: [[[status]], [[build-step-gaps]], [[matching-notes]], [[as1-scheduler-rule]], [[tickets/T-25013-compiler-level-levers]], [[tickets/T-25014-allocator-source-shape-automation]]]
---

## Goal

T-25013 and T-25014 were limited by instrumentation: compiler internals
were inferred from asm diffs, pipeline experiments were one-off edits,
the uniformity gate needed the host's disc tree, and past sweep results
live in prose. At Lila's direction (2026-10-21: build all of it, then
test across the remaining functions), build six tools that expand the
lever set, in this order:

1. **Regression corpus + uniformity harness** (tools/corpus_build.py,
   tools/corpus_check.py): packets built from matched functions in
   src/ (repo-only, no disc), baseline-compiled once; any pipeline
   variant is then checked for changed outputs locally, the lever_lab
   gate without the host.
2. **Trace observatory** (tools/trace_lab.py): one command renders a
   packet TU's uopt expression graph, range-to-colour table, and as1
   weights/mover decisions in diffable text; two-run diff mode.
3. **Census + delay-slot oracle** (tools/census.py): classify every
   remaining function's diff signature into buckets; for delay-slot
   diffs, report the stall/fill pairs and the weight delta that would
   produce the original's fill (lever 4 arithmetic).
4. **Rule fuzzing lab** (tools/rule_fuzz.py): enumerate candidate
   pipeline knobs (single and pairwise OPTN, named cvt_pass rewrites),
   validate each against the corpus (zero matched changes required),
   and report target improvements.
5. **Construct precedent miner** (tools/precedent.py): index the
   matched source for the constructs behind tricky codegen (empty-if
   probes, volatile globals, `* 1` wrappers, switch shapes), queryable
   per pattern, a recipe book from evidence.
6. **Sweep ledger + batch runner** (tools/sweep_ledger.py): a
   machine-readable record of what has been tried on each function
   (method, budget, best score), with shape_search batch mode that
   skips exhausted entries.

## Acceptance criteria

- [ ] Each tool exists under tools/ with its own tests passing.
- [ ] Corpus gate proven both ways: a known-bad knob (zcopy) is caught
      changing corpus functions; the baseline knob set changes none.
- [ ] Observatory reproduces the T-25013 evidence for one known case
      (GEKO func_80136C20 weights / ENDING colour or CSE trace).
- [ ] Census runs over every packet available remotely and buckets
      them; the delay-slot oracle names the fill pairs for the GEKO bed.
- [ ] Rule fuzzing completes a single-knob pass over corpus + targets
      and reports; pairwise pass over the active knob set.
- [ ] Ledger records the T-25014 runs retroactively and one new batch.

## Notes

- Branch octa/t25015-tooling, stacked on octa/t25014-shape-automation.
- Tools are developed against the remote scratch tree and packets;
  corpus data and run outputs stay in remote scratch, tools and tests
  are what land in the repo.
- Testing across the remaining functions follows the build-out, using
  packets fetched per function; the full 119-function census still
  wants the asm bundle from the host when convenient.

## Comments
