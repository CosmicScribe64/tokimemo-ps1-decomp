---
id: T-24570
title: Match MAIN func_80054E6C through its metric-record word view
status: In Review
assignee: Codex Luna high
created: 2026-10-20
updated: 2026-10-20
links: ["[[families/main-family-results]]", "[[matching-notes]]"]
---

## Goal

Match MAIN `func_80054E6C` in literal default C using the recovered `GsRec0FC` aggregate and its supported 32-bit word view, if that source model closes the existing alias/CSE spill mismatch.

## Acceptance criteria

- [x] Verify the record layout and the target's word access against the canonical types and assembly.
- [x] Record the full-translation-unit literal baseline and the focused hypothesis result.
- [x] Preserve one concise negative result.
- [x] Keep the production function unchanged because the hypothesis did not match.

## Notes

The existing T-24123 proposal records a focused prior attempt at slot-relative pointer bases and frame padding. The new hypothesis modeled the word load of each 4-byte `GsRec0FC` record explicitly, rather than reading records by indexing from the preceding `unk_0F8` field. `GsRec0FC` is two signed halfwords (size 4); its canonical type has no word member. A local diagnostic union with `GsWord` tested that view without changing shared headers.

## Comments

Baseline and trial each produced 84 printable difference rows in Docker full-TU `funcdiff --resolve --version c --unit main`; both remained DIFF. Expected frame is 0x80 and the current C proposal remains 0x40. The explicit word-view union changed allocator choices but did not reduce the row count. The temporary source edit was reverted. No production source or shared-header change remains.
