---
id: T-24037
title: Recover EVENT scene siblings
status: Done
assignee: complete_render_family
created: 2026-10-17
updated: 2026-10-20
links: ['[[event-family-results]]']
---

## Goal

Port func_80102E54 (672 B) and func_80103774 (628 B) in src/ovl/EVENT/801024B0.c from matched DATE func_801556D8 and func_801560E0. Bounded twenty-five minute novel pass, one compiler.

## Acceptance criteria

- Any claimed win has no target INCLUDE_ASM and passes normal default literal proof plus full EVENT checksum.
- Preserve useful negative evidence; independent standards review before Done.

## Notes

Earlier T-9520/T-9710 attempts failed on unresolved local/table/argument types. Existing EvPairs8 recovers the second function's stack-copy table. Header changes require exact prior notice to root.

## Results

func_80103774 is an unguarded 628 B C match from matched DATE func_801560E0. It reuses DisplayFixActor44 and EvPairs8, replaces sibling copied coordinate arrays with the target's global arrays, and preserves the target's flag-before-type clear order. No new FAKE in this win.

Four root-approved EVENT load-time views: u8 flags D_8012274C[8], s16 x D_80122754[8], s16 y D_80122764[8], u8 counters D_80122774[8]. Target lbu/sb stride1 and lh stride2 plus bounded-eight loops establish widths/extents. Canonical main scalars remain untouched, wrapped only in standard MAIN_API_OVERRIDE guards; EVENT.h supplies its distinct views and exact comments. No duplicate closure declarations.

func_80102E54 remains guarded at aligned edit cost6, with typed source archive drafts/T-24037. The constant8 uses v0 instead of target v1 and flags-table v0 initialization falls after both byte stores instead of between them. Independent stores worsen85; reverse chain, full-pool addresses, and a meaningful flags pointer do not improve6. Stop rather than expand exhausted variants.

Self-review against CODING_STANDARDS.md: C89, typed SDK/game aggregates, no toolchain/flags changes, no raw data/assembly listings, source guard maintained for the near miss, shared-header overrides preserve main-context types. Independent review/all-overlay integration pending at root.

Final validation: no target INCLUDE_ASM for func_80103774; source gate passes; normal default literal proof with --resolve --no-build --version default matches628 B. EVENT SHA1 `ff7184d7e92b7aff372450ac3b8c2bdd42e55a5e` passes. Headers/globals/main_api/whitespace checks pass.

## Independent root review

Reviewed production source/header changes against CODING_STANDARDS.md; no unresolved findings. Actual unguarded C target passed default linked proof and all27 binary hashes plus headers/globals/prototype guards. Preserved guarded siblings and explicit existing FAKE documentation. Ticket remains In Review because the assigned sibling is unresolved. Published on main.

## Comments

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
