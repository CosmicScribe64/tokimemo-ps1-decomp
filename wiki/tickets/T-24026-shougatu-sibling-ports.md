---
id: T-24026
title: Port three SHOUGATU high-similarity siblings
status: Done
assignee: complete_render_family
created: 2026-10-17
updated: 2026-10-20
links: ['[[shougatu-family-results]]']
---

## Goal

Port func_80138240, func_8013F224 and func_80140510 from matched GYOZI/DATE siblings, using existing typed views.

## Acceptance criteria

- Unguarded C targets pass normal default literal proof and SHOUGATU checksum.
- Source and evidence reviewed against CODING_STANDARDS.md before Done.

## Notes

Exclusive sources: src/ovl/SHOUGATU/80137FF0.c and src/ovl/SHOUGATU/8013F190.c. Bounded twenty-minute pass; no unrelated functions. Prior T-22020 did not start func_80138240. GYOZI T-20006 and DATE T-20600 provide proven sibling recipes.

## Results

Two unguarded C wins: func_8013F224 316 B and func_80140510 392 B (708 B total). Both pass the INCLUDE_ASM source gate and normal default linked literal proof. SHOUGATU full unit checksum passes. SDK POLY_G3, existing callback table and actor/game types reused, no shared-header changes. Builder explicit colour masks/comma lines are marked FAKE with provenance. The dispatcher remains guarded, with source-only evidence in drafts/T-24026 and a concrete four-word stack layout blocker.

Self-review against CODING_STANDARDS.md: plain C89, typed recovered views/SDK structures, target assembly fallback absent for both wins, FAKE explained, no flags/toolchain/raw data changes. In Review for parent's independent standards review and all-overlay integration.

Validation: SHOUGATU SHA1 `d956b1df6fbf9d1f8261d7e1f98ea301f7eb66a9`; `check_headers.py` headers OK; `migrate_globals.py --check` globals OK; `sync_protos.py --check` main_api OK; `git diff --check` clean.

## Independent root integration review

Production changes reviewed against CODING_STANDARDS.md. No unresolved findings; existing SDK types and aggregates retained, necessary ABI views explained, FAKE forms marked, no compiler/config exceptions. All27 binary hashes and header/global/prototype guards pass. Each new target lacks its INCLUDE_ASM fallback and passes normal-default literal linked-byte proof. Root published verified code on main; unresolved family functions are not counted.

## Comments

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
