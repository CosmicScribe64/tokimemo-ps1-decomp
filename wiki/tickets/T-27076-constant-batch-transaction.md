---
id: T-27076
title: Validate a constant batch transactionally
status: Done
assignee: constant-batch-operator
created: 2026-10-24
updated: 2026-10-24
links: ["[[rename-tool]]", "[[tickets/T-27051-parallel-overlay-reconciliation]]", "[[tickets/T-27066-remaining-overlay-completion]]"]
---

## Goal

Avoid repeating the entire 13,000-row naming guard after each individually proved
constant edit. Prepare an ignored integration coordinator that retains per-request
count, inverse, provenance, GCC and declaration proofs, validates the whole batch at
the end, and restores the whole authorized source/ledger write set on failure.

## Acceptance criteria

- [x] Frozen producer/tool pins and all request proofs remain enforced; no numeric text replacement occurs outside rename.py.
- [x] Only post-write global checks are deferred; the real final global check is mandatory before success.
- [x] Source/header/ledger snapshots and refusal gates preserve pre-existing work; failed operations or final validation restore the complete batch.
- [x] Disposable success, mid-apply failure and final-check failure probes pass; original producer files and live tool files remain unchanged.
- [x] Independent review approves the exact coordinator before live use; matching proof remains required before committing its output.

## Notes

The current T-27064 base process is allowed to finish without interruption. Its
supplement also retains the reviewed route. The proposed transaction targets the
remaining disjoint packets; it does not relax any final gate or alter game logic.

The base297/supplement27 route completed successfully. Root then used independently
approved coordinator5709cbfa13cfb79fddf633e9d2563f0154881cf0158bb2917fd588051f36f82a
for the remaining370 sites: status success,48 changed paths, final naming guard
12977 rows. Disposable actual rollback and report-fence findings were resolved.
The selected source wave passes its combined725-site matching proof; the reviewed output is ready for commit.

## Verified completion

Independent actual-source review passes; the combined725-site source proof has437 exact ABI views and no unexpected paths. Clean Docker configure/build/progress and every guard pass, independent binaries27/27,6952functions,2276920bytes; naming guard12984 rows. Evidence: ignored next-integration/constant-wave/{source-proof,clean-proof}.json and actual-source-review.md. This closes the bounded ticket, not all of phase2.
