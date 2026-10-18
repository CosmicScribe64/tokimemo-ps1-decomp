---
id: T-24052
title: Match SHOUGATU dialogue siblings
status: Done
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[shougatu-family-results]]", "[[tickets/T-20004-gyozi-dialogue-followup]]"]
---

## Goal
Recover func_80133208 (488 bytes) and func_801333F0 (544 bytes) in src/ovl/SHOUGATU/801330C0.c using the matched GYOZI dialogue source.

## Acceptance criteria
- [x] Read prior target/type and sibling evidence; port source with one serial Docker compiler.
- [x] Any promotion is actual unguarded normal default C with literal resolved linked proof and SHOUGATU checksum.
- [x] Preserve useful negative evidence and leave In Review for root review.

## Notes
Start 8be265d2a. Only assigned TU, no header/config changes without notice. T-17500 has stale raw GameState/table accesses at120/141 rows. T-20004 matched the GYOZI siblings with byte sound table, direct signed timer updates, byte postincrement and a marked FAKE unchanged-byte |=0 store. SHOUGATU adds an input skip block and has threshold0x28. No push/merge.

## First verified checkpoint

func_80133208 is actual unguarded normal C, no target fallback. Ported matched GYOZI source with SHOUGATU skip block, threshold0x28, table/field views and five-step selector. One inherited FAKE unchanged-byte |=0 store is reticketed T-24052. Fresh Docker ninja -j1 compiled/linked SHOUGATU and passed unit checksum/headers; literal tools/funcdiff.py func_80133208 --unit SHOUGATU --resolve --no-build --version default reports MATCH488 bytes. No header/config changes. Second target remains in progress; no push/merge.

## Root independent review

First488-byte sibling independently reviewed against CODING_STANDARDS.md and published f90483388. Plain C89, inherited unchanged-byte store explicitly FAKE, no header/config changes; actual-C gate and literal default MATCH488, all27 hashes and interface guards pass. Ticket remains In Progress for second sibling.
## Final verified checkpoint

func_801333F0 also matches as actual unguarded normal default C:544 linked bytes, no fallback. Plain halfword selector self-stores survive this target compiler and match; compound no-ops, unsigned views, locals, volatile stores and assignment predicates did not. Both unchanged halfword stores are marked FAKE/T-24052; the inherited unchanged-byte store remains marked. No volatile access or custom compiler behavior in accepted source.

Fresh Docker ninja -j1 compiled the actual TU, linked SHOUGATU and passed checksum/headers. Both literal default resolved proofs rerun: func_801333F0 MATCH544 and func_80133208 MATCH488, total1032 new C bytes in this ticket. No header/config changes. Source-only bodies and bounded negative ledger in drafts/T-24052/README.md. In Review for root review/integration; no push/merge.

## Root final review

Second544-byte handler reviewed against CODING_STANDARDS.md: C89, canonical fields, no headers/config/compiler changes; both unchanged halfword stores explicitly FAKE and justified by target compiler/negative controls. Independent Docker all27 checksums, header/global/prototype guards and actual-C gates passed; literal default MATCH544 and488. Published44ab1ad82. Both ticket targets verified, no open findings; moved In Review to Done.
