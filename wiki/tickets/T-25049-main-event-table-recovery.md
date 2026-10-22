---
id: T-25049
title: Recover MAIN event selection table draft
status: In Progress
assignee: orchestrator
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-25044-parameter-up-down]]"]
---

## Goal

Recover a compiling, typed whole-TU candidate for remaining func_80073CB0 in src/main/800737A0.c, then pursue literal matching. Root owns this file after T-25044 completed; T-25048 works in80075320.c.

## Acceptance criteria

- [ ] Table layouts and source views supported by assembly and existing declarations.
- [ ] Reproducible Docker whole-TU baseline without placeholders.
- [ ] Literal match, declaration checks and full binary gate before promotion.

## Notes

Historical T-17000 candidate exists. First inspect its blockers and canonical table declarations before changing shared headers. Production remains assembly until verified.

## Recovery evidence

Docker whole-TU byte-table candidate compiles322/322 words with148 aligned differences. T-17000's recorded380 score is historical, not a fresh replay. Assembly's unscaled lbu indexing proves old s32 table views wrong; private C0/D0 u8 regions fix this. Second fallback must read unk_0F4.h (D_800E6374 at80074074), not old unk_000. Correcting that field leaves diagnostic cost148, so do not rely on this metric as semantic proof. First residue is initial pointer/value v0/a0 allocation, then promoted switch selectors. Source and replay retained in drafts/T-25049; production untouched.
