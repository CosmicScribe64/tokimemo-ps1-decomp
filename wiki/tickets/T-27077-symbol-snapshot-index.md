---
id: T-27077
title: Index immutable symbol snapshot lookups
status: In Progress
assignee: symbol-index
created: 2026-10-24
updated: 2026-10-24
links: ["[[rename-tool]]", "[[tickets/T-27076-constant-batch-transaction]]"]
---

## Goal

Remove the measured repeated full-symbol scan from owner/address lookups while
preserving SymbolTable's documented immutable-snapshot semantics. This makes the
mandatory naming guard practical for the remaining integration and final reviews.

## Acceptance criteria

- [x] Owned lines, address lines, alias order, deduplication and main/EVENT overlap exactly match the uncached implementation.
- [x] Returned lists remain fresh; caches belong only to an instance, and a rebuilt snapshot observes file changes.
- [x] Relevant synthetic tests and real-table equivalence/benchmark pass in Docker.
- [x] Independent review approves the exact patch; integrate only after frozen producer runs no longer require the original tool hash.
- [ ] Actual source guard passes with unchanged source/config behavior before Done.

## Notes

Read-only stack samples identify owned_lines/names_at inside old_name_hits as a
validation hotspot. The active base/supplement process remains untouched. The
ignored candidate is `.claude/orchestrator/state/T-27077/candidate.patch`
(SHA-256 `0b8e62f0a5eb9e4e1afb1cbf87e887cf1aae3558c44b99c53793d65315b5f5ed`);
its result bundle is `.claude/orchestrator/state/T-27077/candidate-result.json`
(SHA-256 `dfe795e513c5545e49214a237321925efe950536b496f1fadfbffe57ebf1cc6f`).
The Docker candidate run passed 4 focused tests and 129 rename/symbol-guard tests.
The independent real-table oracle compared 10,320 owner/address groups across 27
owners using one old scan per owner before independent address grouping. A
32-key/16-pass benchmark returned equal answers at 55.13x in this run.

Root integrated the exact independently approved patch after the 370-site transaction
completed. The combined selector/cache/rename/symbol suite passes 144 tests; result
hashes are recorded in ignored T-27075/integrated-tools-proof.json. The final live
source guard and matching proof follow the remaining 31-site gameplay packet.
