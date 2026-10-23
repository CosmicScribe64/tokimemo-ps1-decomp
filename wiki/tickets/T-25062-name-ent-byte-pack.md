---
id: T-25062
title: Match NAME_ENT byte pack and zero loop
status: In Progress
assignee: Codex Astra
created: 2026-10-22
updated: 2026-10-22
links: ["[[playbook]]", "[[tickets/T-25057-name-ent-pass-investigation]]", "[[tickets/T-24132-name-ent-final-4]]"]
---

## Goal

Qualify and match NAME_ENT func_8013AF78 (296 bytes) in exclusively owned src/ovl/NAME_ENT/80139740.c. Continue in /private/tmp/tokimemo-t24515 on codex/t25057 after clean T-25057 pin b05288c94. No other function, header or build-tool ownership.

## Acceptance criteria

- [ ] Preserve causal pass-level evidence and tested best source.
- [ ] Literal default-C linked MATCH, all 27 hashes and declaration guards pass before promotion.
- [ ] Direct standards/spec review resolves findings; root integrates and accepts.

## Notes

Existing tokimemo-decomp-arm64 image only; disc read-only. Historical guard has roughly 99 resolved rows: pointer-zero loop shape and byte-pack allocation differ. Read historical controls before editing. Root reserves the ticket and will coordinate a compiler slot after preparation.

## Comments

2026-10-22: Ticket created before investigation. T-25057 is independently pinned and remains In Review for root integration.
