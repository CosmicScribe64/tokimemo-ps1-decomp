---
id: T-25015
title: "Lever-expansion tooling programme: corpus gate, trace observatory, census/oracle, rule fuzzing, precedent miner, sweep ledger"
status: Done
assignee: octa
created: 2026-10-21
updated: 2026-10-22
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

- [x] Each tool exists under tools/ with its own tests passing.
- [x] Corpus gate proven both ways: a known-bad knob (zcopy) is caught
      changing corpus functions; the baseline knob set changes none.
- [x] Observatory reproduces the T-25013 evidence for one known case
      (GEKO func_80136C20 weights / ENDING colour or CSE trace).
- [x] Census runs over every packet available remotely and buckets
      them; the delay-slot oracle names the fill pairs for the GEKO bed.
- [x] Rule fuzzing completes a single-knob pass over corpus + targets
      and reports; pairwise pass over the active knob set.
- [x] Ledger records the T-25014 runs retroactively and one new batch.

## Notes

- Branch octa/t25015-tooling, stacked on octa/t25014-shape-automation.
- Tools are developed against the remote scratch tree and packets;
  corpus data and run outputs stay in remote scratch, tools and tests
  are what land in the repo.
- Testing across the remaining functions follows the build-out, using
  packets fetched per function; the full 119-function census still
  wants the asm bundle from the host when convenient.

## Comments

### Results (Octa, remote, 2026-10-21)

All six tools are landed under tools/ with passing unit tests:
corpus_build.py + corpus_check.py (29-function corpus from matched
src; identity check changes 0, zcopy-off caught changing 5, exit
codes as designed), trace_lab.py (reproduces the T-25013 GEKO
weights: stall block 1, switch block 7, plus ucode renders),
census.py (buckets + delay-slot oracle; the GEKO stall/fill is named
exactly), rule_fuzz.py (staged singles/gate/pairs), precedent.py
(1,894 matched functions indexed by construct: 93 empty-if probes,
565 byte views, 22 volatile-global, 24 goto, 5 mul1), sweep_ledger.py
(T-25014 runs recorded; batch mode skips exhausted entries).

Fuzz verdict (three beds: main func_8006A044, GEKO func_80136C20,
ENDING func_801387B0): singles pass over ~660 OPTN values found 10
active knobs; none improves any bed. The corpus gate then shows none
would be adoptable anyway (404:0 changes 28 of 29 corpus functions,
404:2 and 407:0 change 8, 405:0 changes 5, 406:0 changes 3; the rest
crash uopt). All 45 pairwise combinations of the active set were
also tried: no pair improves any bed. The OPTN space, singly and in
pairs, is now closed with lab evidence, not hand sweeps.

Census across the remaining set (driver run once the host's asm tree
was mirrored into the remote scratch, 14,087 listings): 96
INCLUDE_ASM-carried functions found in src; references built for all
of them from the bundle with zero failures. Candidates were the
limitation: only 9 functions have a candidate in a form the driver
can use (a simple NON_MATCHING guarded body or a full-function
draft); 87 keep their work as fragments in drafts/ or in guard forms
the extractor does not yet read, and 3 of the 9 drafts compiled to
objects without the expected symbol. Of the 6 scored, the buckets
were instruction (3), register-only (2), addressing (1), all 75+
rows in the packet metric. Two honest limits for a v2: the packet
metric overstates register-only cases once alignment slips (a
register-canonicalized diff would rank near misses correctly, e.g.
KANGEI func_80134724 reads 113 here against ~4 rows in-tree), and
candidate extraction needs to learn the fragment layout in drafts/.

### Outcome

The instrumentation programme is complete and the ticket goes to In
Review. The levers these tools create are real but so far negative
in result: they close the OPTN space definitively and give the
project a local uniformity gate, a trace observatory, a precedent
index, and a sweep ledger. The next productive step they point to is
candidate reconstruction for the 87 draft-fragment functions, now
that references for all of them build locally.
