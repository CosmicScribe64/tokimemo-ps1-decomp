---
type: plan
updated: 2026-10-20
---

# Plan for the remaining functions

For any agent, including cheap ones, working on the last functions. State on 2026-10-20: 6,825 / 6,952 functions (92.2%), 127 left. The full list with classes, sizes, row counts and notes is [[data/remaining-2026-10-20]]. Read this page, then [[playbook]]; look up a function's history in [[notes/functions]].

## Current execution plan (2026-10-22)

This section supersedes the older compiler-lever recommendation and historical class counts below. T-25013/15 closed their tested levers negatively; T-25014/16 broad shape/salvage passes produced no matches. That bounds the tested approaches, not proof that all remaining functions require compiler changes.

Baseline at 7c87f223e, independently measured in Docker: 6833/6952 functions; 2104676/2276920 bytes (92.4%). Remaining: 119 functions, 172244 bytes. Coordination: [[tickets/T-25017-completion-plan]].

1. Recover the remote scratch QUEUE.md, source candidates, experiment ledger and useful traces referenced by T-25016. They were not included in the pulled tracked files. Reconcile by unit/function against current source, including guarded functions; discard already matched entries. Prefer the newest in-tree body only after comparing it with archived candidates. Use whole-TU resolved diffs with a single documented row-count convention, not packet/canonical scores. Record source hash, compiler context, bytes, unresolved semantics and first divergence. Do not rerun the bulk census/salvage to reconstruct missing remote artifacts without first trying to recover them.
2. Sol lane A: near-set closure, one file at a time. Reproduce GEKO func_80136C20 and KANGEI func_8013852C (reported 2 rows), then TACO func_8014AB50 (constant allocation) and GYOZI func_80140E0C (zero fold). A low row count is not a promise of an easy match. Start only with an untested causal hypothesis grounded in the trace/precedent; skip exhausted tails without one. KANGEI func_80134724, TACO func_801511C0 and BUNKA_SD func_80139290 form the second queue. For 39290 specifically, map traced block 60 and its nine moved instructions back to the source before modifying scheduling inputs.
3. Sol lane B: reconstruction on a file disjoint from lane A. Select a whole-TU measured medium candidate with a concrete type, loop, alias, buffer or call-signature defect and a matched donor. Do not select solely by packet score or function size. The bulk shortlist is provisional until recovered artifacts are verified; top bulk checks reportedly remain 210+ resolved rows. Remove semantic holes (especially zeroed M2C_ERROR constructs) before treating a compiling draft as useful code. Take one representative per shared failure family, then transfer only validated fixes to siblings. Grow to the largest functions after the family representation is understood.
4. Astra: bounded consultations on exact remaining blockers, maximum three causal hypotheses per packet, with source, target difference, faithful pre/post-pass evidence and tried-variant ledger. Idle the requesting Sol lane during the consult. Prioritize a mechanism shared by several functions; do not restart generic compiler-option, volatile, declaration-permutation or source-shape sweeps. Any proposed global build rule must be independently evidenced, uniform and pass the matched corpus before adoption.
5. Work in 45-minute initial investigations, reassessing after 20 minutes without evidence improvement. Continue beyond that when a concrete causal result is moving toward a match; otherwise preserve the best source and blocker, switch to the next justified target. Avoid per-experiment orchestration round trips: workers may execute their bounded causal plan autonomously and report a match, blocker or end-of-budget result. Keep two Sol lanes at most, Astra only as needed.
6. Root integrates: literal linked default-C proof (ASM fallback excluded), source/FAKE review, all 27 checksums and header/global/prototype checks. Commit each match independently; batch compatible integration builds to avoid redundant rebuilds. Update authoritative counts and transfer successful mechanisms to siblings. Monitor contributor claims about every 30 minutes, without posting messages.

Success is zero unmatched game functions and full-scope byte proof, with drafts/research preserved. Measure verified bytes and functions per worker-hour; diagnostic score changes are evidence, not completion. Review lane allocation after the first two investigations. No credible same-day completion estimate follows from the research.

## Historical recommendation (2026-10-21, superseded)

Classes A and B were swept to exhaustion (T-24142, T-25011) and the class C/D reconstructions stall 70-100 rows away (T-25012). The remaining functions are blocked by compiler behaviour the build does not yet model. The next piece of work is [[tickets/T-25013-compiler-level-levers]]: pick one lever (redundant promoted-global store, 8-byte locals shift, no CSE of relational booleans, as1 block-weight/delay-slot pick, uopt colouring order), implement it as one uniform rule in `tools/cvt_pass.py` / `tools/ugen_pass.py` with evidence and a unit test, prove no matched object changes, then promote the functions it unblocks. Use the strongest model; the per-function loop below is only worth running again after a new lever lands.

## Ground rules

- One ticket per agent run (`wiki/tickets/_template.md`), one function or one C file at a time. Claim the file in the ticket title before editing; never edit a file another open ticket names.
- Work in your own git worktree from `main`. Never force-push. Commit each match on its own: `T-NNNN: match UNIT:func`.
- A match counts only when `tools/docker.sh python3 tools/funcdiff.py --resolve UNIT:func` prints `MATCH` and a clean build passes 27/27: `tools/docker.sh sh -c 'rm -rf build && python3 configure.py && ninja'`. Also run `tools/docker.sh python3 tools/check_headers.py`, `tools/migrate_globals.py --check` and `tools/sync_protos.py --check-branch`.
- No inline asm. Every FAKE comment names the ticket and the reason. Keep a `#ifdef NON_MATCHING` guard only under about 30 rows, with `/* NON_MATCHING: T-NNNN, N rows; what is left */`; otherwise restore `INCLUDE_ASM` and put the draft in `drafts/T-NNNN/UNIT/func/`.
- No compiler options per file or per function, and no build-step changes, to make one function match: that is a fakematch (`CODING_STANDARDS.md` 7a). A build step needs one uniform rule, evidence that it models the original, and every matched function unchanged; raise it as a ticket instead.
- Time box: 45 minutes per function in classes A-C. If nothing improves the row count in 20 minutes, stop, write what you tried in the NON_MATCHING note and in the ticket, and move on. Do not repeat a variant the note already lists.
- Finish every run with: the notes section in [[matching-notes]] (`## T-NNNN: ...`, what matched, what was tried, what is left), ticket In Review, a log entry, and a push.

## Classes and what to do

### A. Near misses, guarded, 10 rows or fewer (8 functions)

Almost all are scheduling or register-order tails. Do not start from a fresh draft.

1. Read the note and the function's sections in [[notes/functions]]; list what was already tried.
2. Classify the diff with funcdiff `--resolve` and `tools/regdiff.py` (read the FIRST DIVERGENCE only):
   - **An instruction moved into or out of a branch or load delay slot, or a `lui` hoisted into an earlier block**: this is as1's cross-block filler. Apply [[as1-scheduler-rule]]: work out the block weights and the visit order, then pick a lever. The proven lever is `volatile` on the plain global whose `lui` must stay in its block (declare it in the unit header with a FAKE comment). It does not work through a cast (`*(volatile T *)&X` changes the address code). If the needed candidate sits behind a block of weight +6 or more, no source lever is known yet: record the weights in the note and stop.
   - **Two instructions in the wrong order inside one block**: as1's local order follows source lines. Put the statements on one source line or reorder them ([[playbook]] section on as1 order); `tools/order_search.py` automates it.
   - **Registers renamed, same instructions**: uopt colouring or ugen temporaries. [[register-cheatsheet]] and [[playbook]] sections 1-2: callee and return types first (`tools/viewsweep.py`), then a no-op that costs a ugen temporary, then variable lifetimes. Run `tools/autotweak.py UNIT:func --time 300`.
   - **An argument home store early or late**: [[playbook]] home-store recipes (an empty test on the argument, its first use, implicit int).
3. If the class is not one of these, write it up and stop; a human or an Opus closer takes it.

### B. Guarded, more than 10 rows (18 functions)

Same as A, but first cut the row count with types: run `tools/viewsweep.py`, check the structs the function touches in [[data-types]] and [[game-state]], and retype locals (u8/s16 widths, pointer vs index). Most B functions drop to A after the types are right. Keep the guard only while it stays under about 30 rows.

### C. Not guarded, good draft (11 functions, draft score 150 or less)

Normal matching loop from [[playbook]]: put the best draft (path in the table) in as a guard, fix types and control flow, then viewsweep, autotweak, regdiff, order_search. Expect most of these to match or to become A.

### D. Large or poor drafts (90 functions)

Reconstruction, one function per run, biggest first only if the agent can take it; otherwise smallest first.

1. Start from the asm, not from the old draft: read `asm/.../nonmatchings/<file>/<func>.s` and the callers. Write the control flow first (loops, switch, early returns) so funcdiff shows structure rows going to zero, then types, then registers.
2. Reuse what exists: matched siblings in the same file and overlay ([[families/]] pages, `tools/twin_sweep.py` for exact and near copies), the shared structs ([[data-types]], [[game-state]]), and the m2c output in `drafts/`.
3. Commit progress as a guarded draft only once it is under about 30 rows; otherwise keep the draft in `drafts/T-NNNN/` with a short `NOTES.md` (what is done, row count, next step) so the next run continues instead of restarting.
4. Stop when the row count stops falling for 30 minutes; write the notes.

## Reporting back

Each run reports: ticket, functions matched (with funcdiff `MATCH` lines), guards left with row counts, and the branch or commit pushed. The orchestrator merges only work that passes the clean 27/27 build and the linked funcdiff check, and updates [[status]].
