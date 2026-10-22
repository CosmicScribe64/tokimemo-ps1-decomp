---
id: T-25033
title: Test canonical check_para_limit call in src/ovl/SHOUGATU/8013E330.c
status: In Progress
assignee: codex
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-25031-small-overlay-candidate-triage]]", "[[tickets/T-24523-seasonal-source-hypotheses]]"]
---

## Goal

One whole-TU control for func_8013E50C: newest corrected T-24523 body versus existing canonical check_para_limit() with no arguments. Establish prior exact-control provenance first. Callee argument independence supports validity but does not promise matching output.

## Acceptance criteria

- [ ] Check prior ledger for the exact canonical no-argument control.
- [ ] Measure baseline/control whole-TU lengths, resolved differences and frame/table homes.
- [ ] Continue only on concrete improvement; otherwise preserve bounded negative without a grid.

## Notes

Exclusive src/ovl/SHOUGATU/8013E330.c, only func_8013E50C. Isolated /private/tmp/tokimemo-t25023. T-25031 separately pinned e211b9bb5. No shared-header/build change, main push or other functions. Preserve prior field fix/FAKE provenance. Root owns integration and review.

## Initial material progress

Read37 tracked narrow E50C bodies: none has the exact zero-argument helper call. Archived corrected baseline118target/124candidate words,152printed/aligned; canonicalcall117words,99/99. Reconciled a newer in-tree local-layout improvement omitted by that archive: i and girl are declared above the tables. Preserve this layout plus unk_10F8 fix: baseline124words,150/150; canonicalcall117words,93/93, first28 words equal and frame58/table homes40/30 exact. Production untouched while private candidates are measured. Root requested sustained matching through concrete progress; continuing from the corrected in-tree canonical body, not the stale table-layout archive.

## Sustained matching progress and Astra handoff

Corrected source now guarded at5printed/aligned lines,117/118words, exact58frame and40/30table homes. Canonical final helper plus matched GYOZI caller controls remove broad drift: integer847B8 return view87cost, u8 status-result narrowing9, byte-view first argument5. Explicit status(girl,sp40.v,girl) reaches118words but emits zero at call rather than entry and regresses bases (42printed/10cost), rejected. Only entry a2-zero/store/call uses remain.

Actual in-tree guarded C reproduces5/5. Default SHOUGATU checksum, headers/global/prototype guards pass (0new/15known); fallback checksum is not C proof. Fresh pre/post-uopt and stock as1 captures are faithful across all four relevant sections. Source bestguard and precise ledger/packet preserved in drafts/T-25033/README.md. Human/root explicitly now request sole hands-on Astra worker; coordinator idles, no duplicate writer. Ticket remains In Progress for sustained literal closure, not parked or complete.
