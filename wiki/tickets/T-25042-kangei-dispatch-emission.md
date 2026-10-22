---
id: T-25042
title: Match KANGEI default-arm dispatch scheduling
status: In Progress
assignee: codex
created: 2026-10-22
updated: 2026-10-22
links: [[[tickets/T-24201-cloud-packet-kangei-80134724]], [[tickets/T-25011-class-a-closers-as1-levers]], [[tickets/T-25041-kangei-preload-schedule]]]
---

## Goal

Match KANGEI func_80134724 using source-derived default-arm emission that preserves both the range-check delay slot and temporary registers.

## Acceptance criteria

- [x] Read T-24201/T-24902/T-24516 and current T-25011 history before experiments.
- [x] Reproduce current full-TU baseline and capture actual uopt/ugen/as1 boundary.
- [ ] Literal guarded/default C proof, affected checksum and declaration checks.
- [ ] Root standards/spec review before Done.

## Notes

Exclusive source src/ovl/KANGEI/80134120.c. Released80137B90 after pin c0249b80c. Reuse /private/tmp/tokimemo-t25037 on codex/t25042-kangei-dispatch. No additional agents, compiler/header/build changes or broad old grids. Parent owns root integration and full27 gate.

## Comments

2026-10-22: The source has advanced beyond the old two-position scheduling miss: T-25011's u16 default-arm cast fixes the slot but shifts its temporary chain by one. First step is to reproduce this current result and compare the actual expression emission with the uncast control before proposing a distinct store-promotion or temporary-elimination form.

2026-10-22 phase finding: cast default shift t1 fails the mover path test while uncast t0 passes; both blocks have weight4. Named conversion locals restore t0 but their empty boundary disappears before the mover and earlier allocations change. New source and trace negatives are preserved in drafts/T-25042/README.md. Production guard remains unchanged. Continue investigating a source edge that survives through global scheduling but disappears in later branch cleanup; no match or external blocker claimed.
